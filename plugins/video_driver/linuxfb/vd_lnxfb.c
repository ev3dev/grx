/*
 * vd_lnxfb.c - Linux framebuffer driver
 *
 * Copyright (c) 2001 Mariano Alvarez Fernandez <malfer@telefonica.net>
 * Copyright (c) 2015-2017 David Lechner <david@lechnology.com>
 *
 * Contributions by Josu Onandia <jonandia@fagorautomation.es>
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

#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <glib-unix.h>
#include <glib.h>
#include <gmodule.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <linux/vt.h>

#include <grx/context.h>
#include <grx/draw.h>
#include <grx/events.h>
#include <grx/extents.h>

#include "arith.h"
#include "globals.h"
#include "grdriver.h"
#include "libgrx.h"
#include "libinput_device_manager.h"
#include "memcopy.h"
#include "memfill.h"

#define NUM_MODES 80 /* max # of supported modes */
#define NUM_EXTS  15 /* max # of mode extensions */
#define MM_PER_IN 25 /* millimeters per inch */

static int initted = -1;
static int fbfd = -1;
static int ttyfd = -1;
static struct fb_fix_screeninfo fbfix;
static struct fb_var_screeninfo fbvar;
static unsigned char *fbuffer = NULL;
static gboolean in_graphics_mode = FALSE;
static int graphics_vt, original_vt;
static int original_keyboard_mode;
static GrxContext *save;
static gint32 pointer_x, pointer_y;

static int detect(void)
{
    struct vt_stat vtstat;
    struct fb_con2fbmap con2fb_map;
    char ttyname[15];
    char fbname[10];
    int err;

    if (initted < 0) {
        initted = 0;

        // try the boss tty first (probably won't work unless we are root)
        ttyfd = open("/dev/tty0", O_WRONLY);
        if (ttyfd == -1) {
            g_debug("Failed to open /dev/tty0: %s", strerror(errno));
            // fall back to stdin
            ttyfd = dup(0);
        }
        // this fails if we are not a vt
        err = ioctl(ttyfd, VT_GETSTATE, &vtstat);
        if (err < 0) {
            close(ttyfd);
            g_debug("VT_GETSTATE failed: %s", strerror(-err));
            return FALSE;
        }

        // Get the next open console. The graphics will be run on this console
        // rather than the current console (which we may already be in use in
        // the case of /dev/tty0).

        err = ioctl(ttyfd, VT_OPENQRY, &graphics_vt);
        close(ttyfd);
        if (err < 0) {
            g_debug("Could not get unused VT: %s", strerror(-err));
            return FALSE;
        }

        sprintf(ttyname, "/dev/tty%d", graphics_vt);
        ttyfd = open(ttyname, O_RDWR);
        if (ttyfd == -1) {
            g_debug("Failed to open /dev/tty%d: %s", graphics_vt, strerror(errno));
            // fall back to current console
            graphics_vt = vtstat.v_active;
            sprintf(ttyname, "/dev/tty%d", graphics_vt);
            ttyfd = open(ttyname, O_RDWR);
            if (ttyfd == -1) {
                g_debug("Failed to open /dev/tty%d: %s", graphics_vt, strerror(errno));
                return FALSE;
            }
        }

        original_vt = vtstat.v_active;
        err = ioctl(ttyfd, VT_ACTIVATE, graphics_vt);
        if (err < 0) {
            close(ttyfd);
            g_debug("VT_ACTIVATE failed: %s", strerror(-err));
            return FALSE;
        }

        err = ioctl(ttyfd, VT_WAITACTIVE, graphics_vt);
        if (err < 0) {
            close(ttyfd);
            g_debug("VT_WAITACTIVE failed: %s", strerror(-err));
            return FALSE;
        }

        err = ioctl(ttyfd, KDGKBMODE, &original_keyboard_mode);
        if (err < 0) {
            ioctl(ttyfd, VT_ACTIVATE, original_vt);
            close(ttyfd);
            g_debug("KDGKBMODE failed: %s", strerror(-err));
            return FALSE;
        }

        // Then open up the first framebuffer device and use it to find the
        // fbdev that matches this console.

        fbfd = open("/dev/fb0", O_RDWR);
        if (fbfd == -1) {
            g_debug("Failed to open /dev/fb0: %s", strerror(errno));
            ioctl(ttyfd, VT_ACTIVATE, original_vt);
            close(ttyfd);
            return FALSE;
        }
        con2fb_map.console = vtstat.v_active;
        err = ioctl(fbfd, FBIOGET_CON2FBMAP, &con2fb_map);
        if (err < 0) {
            g_debug("FBIOGET_CON2FBMAP failed: %s", strerror(-err));
            ioctl(ttyfd, VT_ACTIVATE, original_vt);
            close(fbfd);
            close(ttyfd);
            return FALSE;
        }

        // Make sure there is actually a framebuffer for this console.

        if (con2fb_map.framebuffer < 0) {
            g_debug("No framebuffer device for this console");
            ioctl(ttyfd, VT_ACTIVATE, original_vt);
            close(fbfd);
            close(ttyfd);
            return FALSE;
        }

        // We already have /dev/fb0 open. If it is not the right one then we
        // need to close it and open the correct one.

        if (con2fb_map.framebuffer != 0) {
            close(fbfd);
            sprintf(fbname, "/dev/fb%d", con2fb_map.framebuffer);
            fbfd = open(fbname, O_RDWR);
            if (fbfd == -1) {
                g_debug("Failed to open /dev/fb%d: %s", con2fb_map.framebuffer,
                    strerror(errno));
                ioctl(ttyfd, VT_ACTIVATE, original_vt);
                close(ttyfd);
                return FALSE;
            }
        }

        // Finally, we have to make sure the frambuffer is compatible.

        ioctl(fbfd, FBIOGET_FSCREENINFO, &fbfix);
        ioctl(fbfd, FBIOGET_VSCREENINFO, &fbvar);
        if (fbfix.type != FB_TYPE_PACKED_PIXELS) {
            g_debug("framebuffer is not FB_TYPE_PACKED_PIXELS");
            ioctl(ttyfd, VT_ACTIVATE, original_vt);
            close(fbfd);
            close(ttyfd);
            return FALSE;
        }

        // start with pointer in center of screen
        pointer_x = fbvar.xres / 2;
        pointer_y = fbvar.yres / 2;

        initted = 1;
    }

    return initted > 0;
}

static void reset(void)
{
    struct vt_mode vtm;
    struct vt_stat vtstat;

    g_debug("closing vd_lnxfb");
    if (fbuffer) {
        memzero(fbuffer, fbvar.yres * fbfix.line_length);
        munmap(fbuffer, fbfix.smem_len);
        fbuffer = NULL;
    }
    if (fbfd != -1) {
        close(fbfd);
        fbfd = -1;
    }
    if (ttyfd != -1) {
        // if we are still on the graphics vt, restore the original vt.
        ioctl(ttyfd, VT_GETSTATE, &vtstat);
        if (vtstat.v_active == graphics_vt) {
            g_debug("restoring tty");
            ioctl(ttyfd, KDSKBMODE, original_keyboard_mode);
            ioctl(ttyfd, KDSETMODE, KD_TEXT);
            vtm.mode = VT_AUTO;
            vtm.relsig = 0;
            vtm.acqsig = 0;
            ioctl(ttyfd, VT_SETMODE, &vtm);
            ioctl(ttyfd, VT_ACTIVATE, original_vt);
        }

        close(ttyfd);
        ttyfd = -1;
        in_graphics_mode = FALSE;
    }
    initted = -1;
}

/* release control of the vt */
static void grx_linuxfb_release(void)
{
    if (!in_graphics_mode) {
        return;
    }
    if (ttyfd < 0) {
        return;
    }
    ioctl(ttyfd, VT_RELDISP, 1);
    in_graphics_mode = FALSE;
}

/* resume control of the vt */
static void grx_linuxfb_aquire(void)
{
    if (in_graphics_mode) {
        return;
    }
    if (ttyfd < 0) {
        return;
    }
    ioctl(ttyfd, VT_RELDISP, VT_ACKACQ);
    ioctl(ttyfd, KDSKBMODE, K_OFF);
    ioctl(ttyfd, KDSETMODE, KD_GRAPHICS);
    in_graphics_mode = TRUE;
}

void grx_linuxfb_chvt(int vt_num)
{
    if (!in_graphics_mode) {
        return;
    }
    if (ttyfd < 0) {
        return;
    }
    ioctl(ttyfd, VT_ACTIVATE, vt_num);
}

void grx_linuxfb_update_pointer(gint32 dx, gint32 dy, gint32 *x, gint32 *y)
{
    *x = pointer_x + dx;
    *y = pointer_y + dy;

    if (*x < 0) {
        *x = 0;
    }
    if (*x >= grx_get_screen_width()) {
        *x = grx_get_screen_width() - 1;
    }
    if (*y < 0) {
        *y = 0;
    }
    if (*y >= grx_get_screen_height()) {
        *y = grx_get_screen_height() - 1;
    }

    pointer_x = *x;
    pointer_y = *y;
}

void grx_linuxfb_get_pointer(guint32 *x, guint32 *y)
{
    *x = pointer_x;
    *y = pointer_y;
}

static void load_color(GrxColor c, GrxColor r, GrxColor g, GrxColor b)
{
    __u16 red, green, blue, transp;
    struct fb_cmap cmap;

    red = (r << 8);
    green = (g << 8);
    blue = (b << 8);
    transp = 0;
    cmap.start = c;
    cmap.len = 1;
    cmap.red = &red;
    cmap.green = &green;
    cmap.blue = &blue;
    cmap.transp = &transp;
    ioctl(fbfd, FBIOPUTCMAP, &cmap);
}

static int setmode(GrxVideoMode *mp, int noclear)
{
    struct vt_mode vtm;

    fbuffer = mp->extended_info->frame =
        mmap(0, fbfix.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (mp->extended_info->frame && ttyfd > -1) {
        ioctl(ttyfd, KDSKBMODE, K_OFF);
        ioctl(ttyfd, KDSETMODE, KD_GRAPHICS);
        vtm.mode = VT_PROCESS;
        vtm.relsig = SIGUSR1;
        vtm.acqsig = SIGUSR1;
        ioctl(ttyfd, VT_SETMODE, &vtm);
        in_graphics_mode = TRUE;
    }
    if (mp->extended_info->frame && !noclear)
        memzero(mp->extended_info->frame, fbvar.yres * fbfix.line_length);
    return mp->extended_info->frame ? TRUE : FALSE;
}

static int settext(GrxVideoMode *mp, int noclear)
{
    struct vt_mode vtm;

    if (fbuffer) {
        memzero(fbuffer, fbvar.yres * fbfix.line_length);
        munmap(fbuffer, fbfix.smem_len);
        fbuffer = NULL;
    }
    if (ttyfd > -1) {
        ioctl(ttyfd, KDSETMODE, KD_TEXT);
        vtm.mode = VT_AUTO;
        vtm.relsig = 0;
        vtm.acqsig = 0;
        ioctl(ttyfd, VT_SETMODE, &vtm);
        in_graphics_mode = FALSE;
    }
    return TRUE;
}

GrxVideoModeExt grtextextfb = {
    .mode = GRX_FRAME_MODE_TEXT, /* frame driver */
    .drv = NULL,                 /* frame driver override */
    .frame = NULL,               /* frame buffer address */
    .cprec = { 6, 6, 6 },        /* color precisions */
    .cpos = { 0, 0, 0 },         /* color component bit positions */
    .flags = 0,                  /* mode flag bits */
    .setup = settext,            /* mode set */
    .set_virtual_size = NULL,    /* virtual size set */
    .scroll = NULL,              /* virtual scroll */
    .set_bank = NULL,            /* bank set function */
    .set_rw_banks = NULL,        /* double bank set function */
    .load_color = NULL,          /* color loader */
};

static GrxVideoModeExt exts[NUM_EXTS];
static GrxVideoMode modes[NUM_MODES] = {
    /* pres.  bpp wdt   hgt   mode   scan  priv. &ext                             */
    { TRUE, 4, 80, 25, 0, 160, 0, &grtextextfb }, { 0 }
};

static int build_video_mode(GrxVideoMode *mp, GrxVideoModeExt *ep)
{
    mp->present = TRUE;
    mp->width = fbvar.xres;
    mp->height = fbvar.yres;
    mp->line_offset = fbfix.line_length;
    mp->extended_info = NULL;
    mp->user_data = 0;
    ep->drv = NULL;
    ep->frame = NULL; /* filled in after mode set */
    ep->flags = 0;
    ep->setup = setmode;
    ep->set_virtual_size = NULL; /* tbd */
    ep->scroll = NULL;           /* tbd */
    ep->set_bank = NULL;
    ep->set_rw_banks = NULL;
    ep->load_color = NULL;
    switch (fbvar.bits_per_pixel) {
    case 1:
        if (fbfix.visual == FB_VISUAL_MONO01)
            ep->mode = GRX_FRAME_MODE_LFB_MONO01;
        else if (fbfix.visual == FB_VISUAL_MONO10)
            ep->mode = GRX_FRAME_MODE_LFB_MONO10;
        else
            return FALSE;
        break;
    case 2:
        if (fbfix.visual != FB_VISUAL_STATIC_PSEUDOCOLOR && !fbvar.grayscale) {
            return FALSE;
        }
        ep->mode = GRX_FRAME_MODE_LFB_2BPP;
        break;
    case 8:
        if (fbfix.visual != FB_VISUAL_PSEUDOCOLOR)
            return FALSE;
        ep->mode = GRX_FRAME_MODE_LFB_8BPP;
        ep->load_color = load_color;
        break;
    case 15:
    case 16:
        if (fbfix.visual != FB_VISUAL_TRUECOLOR)
            return FALSE;
        ep->mode = GRX_FRAME_MODE_LFB_16BPP;
        break;
    case 24:
        if (fbfix.visual != FB_VISUAL_TRUECOLOR)
            return FALSE;
        ep->mode = GRX_FRAME_MODE_LFB_24BPP;
        break;
    case 32:
        if (fbfix.visual != FB_VISUAL_TRUECOLOR)
            return FALSE;
        ep->mode = GRX_FRAME_MODE_LFB_32BPP_LOW;
        break;
    default:
        return FALSE;
    }
    mp->bpp = fbvar.bits_per_pixel;
    ep->flags |= GRX_VIDEO_MODE_FLAG_LINEAR;
    ep->cprec[0] = fbvar.red.length;
    ep->cprec[1] = fbvar.green.length;
    ep->cprec[2] = fbvar.blue.length;
    ep->cpos[0] = fbvar.red.offset;
    ep->cpos[1] = fbvar.green.offset;
    ep->cpos[2] = fbvar.blue.offset;

    return TRUE;
}

static void add_video_mode(
    GrxVideoMode *mp, GrxVideoModeExt *ep, GrxVideoMode **mpp, GrxVideoModeExt **epp)
{
    if (*mpp < &modes[NUM_MODES]) {
        if (!mp->extended_info) {
            GrxVideoModeExt *etp = &exts[0];
            while (etp < *epp) {
                if (memcmp(etp, ep, sizeof(GrxVideoModeExt)) == 0) {
                    mp->extended_info = etp;
                    break;
                }
                etp++;
            }
            if (!mp->extended_info) {
                if (etp >= &exts[NUM_EXTS])
                    return;
                sttcopy(etp, ep);
                mp->extended_info = etp;
                *epp = ++etp;
            }
        }
        sttcopy(*mpp, mp);
        (*mpp)++;
    }
}

static gboolean console_switch_handler(gpointer user_data)
{
    GrxEvent event;

    if (in_graphics_mode) {
        // A well-behaved user program must listen for GRX_EVENT_TYPE_APP_DEACTIVATE
        // and stop drawing on the screen until GRX_EVENT_TYPE_APP_ACTIVATE is received.
        event.type = GRX_EVENT_TYPE_APP_DEACTIVATE;
        grx_event_put(&event);

        /* create a new context from the screen */
        save = grx_context_new(
            grx_get_screen_width(), grx_get_screen_height(), NULL, NULL);
        if (save == NULL) {
            g_critical("Could not allocate context for console switching.");
        }
        else {
            /* copy framebuffer to new context */
            if (grx_frame_mode_get_screen() == GRX_FRAME_MODE_LFB_MONO01) {
                /* Need to invert the colors on this one. */
                grx_context_clear(save, 1);
                grx_context_bit_blt(save, 0, 0, grx_get_screen_context(), 0, 0,
                    grx_get_screen_width() - 1, grx_get_screen_height() - 1,
                    GRX_COLOR_MODE_XOR);
            }
            else {
                grx_context_bit_blt(save, 0, 0, grx_get_screen_context(), 0, 0,
                    grx_get_screen_width() - 1, grx_get_screen_height() - 1,
                    GRX_COLOR_MODE_WRITE);
            }
        }
        grx_linuxfb_release();
    }
    else {
        grx_linuxfb_aquire();

        /* copy the temporary context back to the framebuffer */
        if (grx_frame_mode_get_screen() == GRX_FRAME_MODE_LFB_MONO01) {
            /* need to invert the colors on this one */
            grx_clear_screen(1);
            grx_context_bit_blt(grx_get_screen_context(), 0, 0, save, 0, 0,
                grx_get_screen_width() - 1, grx_get_screen_height() - 1,
                GRX_COLOR_MODE_XOR);
        }
        else {
            grx_context_bit_blt(grx_get_screen_context(), 0, 0, save, 0, 0,
                grx_get_screen_width() - 1, grx_get_screen_height() - 1,
                GRX_COLOR_MODE_WRITE);
        }
        grx_context_unref(save);

        // now it is OK for the user program to start writing to the screen again
        event.type = GRX_EVENT_TYPE_APP_ACTIVATE;
        grx_event_put(&event);
    }

    return G_SOURCE_CONTINUE;
}

static int init(const char *options)
{
    if (detect()) {
        GrxLibinputDeviceManager *device_manager;
        GrxVideoMode mode, *modep = &modes[1];
        GrxVideoModeExt ext, *extp = &exts[0];
        GrxEvent event;
        GError *err = NULL;

        device_manager =
            g_initable_new(GRX_TYPE_LIBINPUT_DEVICE_MANAGER, NULL, &err, NULL);
        if (!device_manager) {
            g_debug("%s", err->message);
            g_error_free(err);
            return FALSE;
        }

        memzero(modep, (sizeof(modes) - sizeof(modes[0])));
        if ((build_video_mode(&mode, &ext))) {
            add_video_mode(&mode, &ext, &modep, &extp);
        }

        // Handle console switching
        g_unix_signal_add(SIGUSR1, console_switch_handler, NULL);

        event.type = GRX_EVENT_TYPE_APP_ACTIVATE;
        grx_event_put(&event);

        DRVINFO->device_manager = GRX_DEVICE_MANAGER(device_manager);
        grx_libinput_device_manager_event_add(device_manager);

        return TRUE;
    }

    return FALSE;
}

static guint get_dpi(GrxVideoDriver *driver)
{
    if (fbvar.width == 0 || fbvar.height == 0) {
        return GRX_DEFAULT_DPI;
    }

    return (fbvar.xres * MM_PER_IN / fbvar.width
               + fbvar.yres * MM_PER_IN / fbvar.height)
           / 2;
}

G_MODULE_EXPORT GrxVideoDriver grx_linuxfb_video_driver = {
    .name = "linuxfb",               /* name */
    .modes = modes,                  /* mode table */
    .n_modes = itemsof(modes),       /* # of modes */
    .detect = detect,                /* detection routine */
    .init = init,                    /* initialization routine */
    .reset = reset,                  /* reset routine */
    .select_mode = _grx_select_mode, /* standard mode select routine */
    .get_dpi = get_dpi,
};
