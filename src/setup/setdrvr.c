/*
 * setdrvr.c ---- video driver setup
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
 * Copyright (c) 2016 David Lechner <david@lechnology.com>
 *
 * This file is part of the GRX graphics library.
 *
 * The GRX graphics library is free software; you can redistribute it
 * and/or modify it under some conditions; see the "copying.grx" file
 * for details.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <ctype.h>
#include <string.h>

#include <glib.h>
#include <gmodule.h>

#include "grx/error.h"
#include "globals.h"
#include "libgrx.h"
#include "grdriver.h"
#include "arith.h"
#include "memcopy.h"
#include "memfill.h"
#include "util.h"

// List of video driver plugin file names
static const gchar * const driver_names[] = {
    "gtk3",
    "linuxfb",
    "memory",
    NULL
};

static const char *nxtoken(const char *p, char *token)
{
        while(*p == ' ') p++;
        while(*p && (*p != ' ')) *token++ = *p++;
        *token = '\0';
        return(p);
}

/**
 * grx_set_driver:
 * @driver_spec: (nullable): The driver spec.
 * @error: (nullable): Error pointer.
 *
 * The driver spec is of the format "<name>\[::<flag>\] \[gw <width>\]
 * \[gh <height>\] \[gc <colors>\]".
 *
 * - "<name>" is the name of a video driver plugin.
 * - "<flag>" is "fs" for fullscreen or "ww" for windowed (not supported by all
 *   drivers)
 * - "<width>" is the default width
 * - "<height>" is the default height
 * - "<colors>" is the default color depth. "K" and "M" suffixes are recognized.
 *
 * If @driver_spec is %NULL, then it will first check the GRX_DRIVER environment
 * variable for a driver spec. If the environment variable is not present, then
 * it will uses the driver with the highest number of modes.
 *
 * Returns: %TRUE on success, otherwise %FALSE.
 */
int grx_set_driver(const char *drvspec, GError **error)
{
        static int firsttime = TRUE;
        GrxVideoDriver *drv = NULL;
        char options[100];
        if (!drvspec) {
            drvspec = g_getenv("GRX_DRIVER");
            g_debug("Checking GRX_DRIVER environment variable: %s",drvspec);
        }
        options[0] = '\0';
        if(drvspec) {
            char t[100],name[100], *p2;
            const char *p = drvspec;
            name[0] = '\0';
            while(p = nxtoken(p,t),t[0] != '\0') {
                if(strlen(t) == 2) {
#                   define CH16(c1,c2)  (((c1) << 8) | (c2))
                    void *oaddr = NULL;
                    int  cf = FALSE;
                    switch(CH16(tolower(t[0]),tolower(t[1]))) {
                        case CH16('n','c'):
                        case CH16('g','c'): oaddr = &DRVINFO->defgc; cf = TRUE; break;
                        case CH16('t','c'): oaddr = &DRVINFO->deftc; cf = TRUE; break;
                        case CH16('g','w'): oaddr = &DRVINFO->defgw; break;
                        case CH16('t','w'): oaddr = &DRVINFO->deftw; break;
                        case CH16('g','h'): oaddr = &DRVINFO->defgh; break;
                        case CH16('t','h'): oaddr = &DRVINFO->defth; break;
                    }
                    if(oaddr) {
                        long optval;
                        p = nxtoken(p,t);
                        if(sscanf(t,"%ld",&optval) > 0) {
                            if(cf) {
                                switch(toupper(t[strlen(t) - 1])) {
                                    case 'K': optval <<= 10; break;
                                    case 'M': optval <<= 20; break;
                                }
                                *((long *)oaddr) = optval;
                                continue;
                            }
                            *((int *)oaddr) = (int)optval;
                        }
                        continue;
                    }
                }
                strcpy(name,t);
            }
            for (p2 = name; (p2 = strchr(p,':')) != NULL; p2++) {
                if (p[1] == ':') {
                    strcpy(options, &p2[2]);
                    *p2 = '\0';
                    break;
                }
            }
            if (name[0] != '\0') {
                GModule *plugin;
                gchar *file_name, module_name[128];

                g_snprintf(module_name, 128, "grx-3.0-vdriver-%s", name);
                file_name = g_module_build_path(GRX_PLUGIN_PATH, module_name);
                g_debug("Looking for plugin: %s", file_name);
                plugin = g_module_open(file_name, G_MODULE_BIND_LAZY);
                g_free(file_name);
                if (!plugin) {
                    g_debug("%s", g_module_error());
                    g_set_error(error, GRX_ERROR, GRX_ERROR_PLUGIN_FILE_NOT_FOUND,
                                "Could not find video driver plugin '%s'", module_name);
                    return FALSE;
                }
                g_snprintf(module_name, 128, "grx_%s_video_driver", name);
                if (!g_module_symbol(plugin, module_name, (gpointer *)&drv)) {
                    g_debug("%s", g_module_error());
                    g_set_error(error, GRX_ERROR, GRX_ERROR_PLUGIN_SYMBOL_NOT_FOUND,
                                "Could not symbol '%s' in plugin '%s'",
                                module_name, name);
                    return FALSE;
                }
            }
        }
        if (!drv) {
            GrxVideoDriver *dp;
            GModule *plugin;
            gchar *file_name, module_name[128];
            int i, maxmodes = 0;

            g_debug("Trying detect");
            // Try to load each video driver plugin
            for (i = 0; driver_names[i]; i++) {
                g_snprintf(module_name, 128, "grx-3.0-vdriver-%s", driver_names[i]);
                file_name = g_module_build_path(GRX_PLUGIN_PATH, module_name);
                g_debug("Looking for plugin: %s", file_name);
                plugin = g_module_open(file_name, G_MODULE_BIND_LAZY);
                g_free(file_name);
                if (!plugin) {
                    g_debug("%s", g_module_error());
                    continue;
                }
                g_snprintf(module_name, 128, "grx_%s_video_driver", driver_names[i]);
                if (!g_module_symbol(plugin, module_name, (gpointer *)&dp)) {
                    g_debug("%s", g_module_error());
                    continue;
                }
                // Use the plugin with the higest number of modes
                if (dp->detect && dp->detect()) {
                    GrxVideoDriver *dp2;
                    int nm = 0;

                    for (dp2 = dp; dp2; dp2 = dp2->inherit) {
                        nm += dp2->n_modes;
                    }
                    if (nm > maxmodes) {
                        drv = dp;
                        maxmodes = nm;
                    }
                }
            }
            if (!drv) {
                g_set_error(error, GRX_ERROR, GRX_ERROR_FAILED,
                            "No supported video driver plugins found");

                return FALSE;
            }
            g_debug("Found %s", drv->name);
        }
        _GrCloseVideoDriver();
        if(firsttime) {
            atexit(_GrCloseVideoDriver);
            firsttime = FALSE;
        }
        if(!drv->init || drv->init(options)) {
            DRVINFO->vdriver = drv;
            return TRUE;
        }

        g_set_error(error, GRX_ERROR, GRX_ERROR_FAILED,
                    "Driver '%s' failed to init", drv->name);

        return FALSE;
}

void _GrCloseVideoDriver(void)
{
        sttcopy(&DRVINFO->fdriver,&DRVINFO->tdriver);
        if(DRVINFO->vdriver != NULL) {
            if(DRVINFO->vdriver->reset) (*DRVINFO->vdriver->reset)();
            DRVINFO->vdriver = NULL;
        }
}
