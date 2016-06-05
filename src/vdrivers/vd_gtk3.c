/*
 * vd_gtk3.c ---- Linux framebuffer driver
 *
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

#include <glib.h>
#include <gtk/gtk.h>

#include "libgrx.h"
#include "grdriver.h"

// We want to make sure the line_length will match the rowstride of the GtkPixbuf.
// Since we are defining modes statically, we need to do the same calculation
// that gdk_pixbuf_new() does to initialize the rowstride value. This assumes
// no alpha, so 3 channels.
#define ROWSTRIDE(w) (((w) * 3 + 3) & ~3)

static gboolean gtk_init_ok = FALSE;
static GdkPixbuf *pixbuf = NULL;

static gboolean detect(void)
{
    if (!gtk_init_ok) {
        int argc = 0;

        gtk_init_ok = gtk_init_check (&argc, NULL);

        if (!gtk_init_ok) {
            g_debug ("failed to init GTK");
        }
    }

    return gtk_init_ok;
}

static void reset(void)
{
    g_object_unref (G_OBJECT (pixbuf));
    pixbuf = NULL;
}

// static void load_color(GrxColor c, GrxColor r, GrxColor g, GrxColor b)
// {
// }

static gboolean setup_text_mode (GrxVideoMode *mode, gboolean noclear)
{
    return TRUE;
}

static gboolean setup_grapics_mode (GrxVideoMode *mode, gboolean noclear)
{
    if (!detect ()) {
        return FALSE;
    }

    g_return_val_if_fail (pixbuf == NULL, FALSE);

    pixbuf = gdk_pixbuf_new (GDK_COLORSPACE_RGB, FALSE, 8, mode->width,
                             mode->height);

    g_return_val_if_fail (pixbuf != NULL, FALSE);
    g_return_val_if_fail (gdk_pixbuf_get_rowstride (pixbuf) == mode->line_offset,
                          FALSE);

    if (!noclear) {
        gdk_pixbuf_fill (pixbuf, 0);
    }

    mode->extended_info->frame = gdk_pixbuf_get_pixels (pixbuf);
    mode->user_data = pixbuf;

    return TRUE;
}

static GrxVideoModeExt text_mode_ext = {
    .mode               = GRX_FRAME_MODE_TEXT,
    .drv                = NULL,
    .frame              = NULL,
    .cprec              = { 8, 8, 8 },
    .cpos               = { 0, 0, 0 },
    .flags              = 0,
    .setup              = setup_text_mode,
    .set_virtual_size   = NULL,
    .scroll             = NULL,
    .set_bank           = NULL,
    .set_rw_banks       = NULL,
    .load_color         = NULL,
};

static GrxVideoModeExt graphics_mode_ext = {
    .mode               = GRX_FRAME_MODE_LFB_24BPP,
    .drv                = NULL,
    .frame              = NULL,
    .cprec              = { 8, 8, 8 },
    .cpos               = { 16, 8, 0 },
    .flags              = GRX_VIDEO_MODE_FLAG_LINEAR,
    .setup              = setup_grapics_mode,
    .set_virtual_size   = NULL,
    .scroll             = NULL,
    .set_bank           = NULL,
    .set_rw_banks       = NULL,
    .load_color         = NULL,
};

static GrxVideoMode video_modes[] = {
    {
        .present        = TRUE,
        .bpp            = 4,
        .width          = 80,
        .height         = 25,
        .mode           = 0,
        .line_offset    = 168,
        .user_data      = NULL,
        .extended_info  = &text_mode_ext
    },
    {
        .present        = TRUE,
        .bpp            = 24,
        .width          = 640,
        .height         = 480,
        .mode           = 0,
        .line_offset    = ROWSTRIDE (640),
        .user_data      = NULL,
        .extended_info  = &graphics_mode_ext
    },
    {
        .present        = TRUE,
        .bpp            = 24,
        .width          = 320,
        .height         = 240,
        .mode           = 0,
        .line_offset    = ROWSTRIDE (320),
        .user_data      = NULL,
        .extended_info  = &graphics_mode_ext
    },
};

static gboolean init (const gchar *options)
{
    return TRUE;
}

GrxVideoDriver _GrVideoDriverGtk3 = {
    .name           = "gtk3",
    .adapter        = GRX_VIDEO_ADAPTER_TYPE_GTK3,
    .inherit        = NULL,
    .modes          = video_modes,
    .n_modes        = G_N_ELEMENTS (video_modes),
    .detect         = detect,
    .init           = init,
    .reset          = reset,
    .select_mode    = _gr_select_mode,
    .flags          = 0,
};
