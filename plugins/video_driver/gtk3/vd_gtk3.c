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
#include <gmodule.h>
#include <gtk/gtk.h>

#include <grx/events.h>

#include "libgrx.h"
#include "globals.h"
#include "grdriver.h"
#include "gtk3_device.h"

// We want to make sure the line_length will match the rowstride of the GtkPixbuf.
// Since we are defining modes statically, we need to do the same calculation
// that gdk_pixbuf_new() does to initialize the rowstride value. This assumes
// no alpha, so 3 channels.
#define ROWSTRIDE(w) (((w) * 3 + 3) & ~3)

static gboolean gtk_init_ok = FALSE;
static GtkWidget *image;

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
    GdkPixbuf *pixbuf;
    GdkPixbufSimpleAnim *animation;

    if (!detect ()) {
        return FALSE;
    }

    g_return_val_if_fail (image != NULL, FALSE);

    pixbuf = gdk_pixbuf_new (GDK_COLORSPACE_RGB, FALSE, 8, mode->width,
                             mode->height);

    g_return_val_if_fail (pixbuf != NULL, FALSE);
    g_return_val_if_fail (gdk_pixbuf_get_rowstride (pixbuf) == mode->line_offset,
                          FALSE);

    if (!noclear) {
        gdk_pixbuf_fill (pixbuf, 0);
    }

    mode->extended_info->frame = gdk_pixbuf_get_pixels (pixbuf);

    // Using an animation get the pixbuf to automatically refresh on the screen.
    // GRX does not have a way of notifying that a context is "dirty".
    animation = gdk_pixbuf_simple_anim_new (mode->width, mode->height, 25);
    gdk_pixbuf_simple_anim_add_frame (animation, pixbuf);
    gdk_pixbuf_simple_anim_set_loop (animation, TRUE);
    gtk_image_set_from_animation (GTK_IMAGE (image),
                                  GDK_PIXBUF_ANIMATION (animation));

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
    .cpos               = { 0, 8, 16 },
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
        .line_offset    = 168,
        .extended_info  = &text_mode_ext
    },
    {
        .present        = TRUE,
        .bpp            = 24,
        .width          = 320,
        .height         = 240,
        .line_offset    = ROWSTRIDE (320),
        .extended_info  = &graphics_mode_ext
    },
    {
        .present        = TRUE,
        .bpp            = 24,
        .width          = 640,
        .height         = 480,
        .line_offset    = ROWSTRIDE (640),
        .extended_info  = &graphics_mode_ext
    },
    {
        .present        = TRUE,
        .bpp            = 24,
        .width          = 800,
        .height         = 600,
        .line_offset    = ROWSTRIDE (800),
        .extended_info  = &graphics_mode_ext
    },
    {
        .present        = TRUE,
        .bpp            = 24,
        .width          = 1024,
        .height         = 768,
        .line_offset    = ROWSTRIDE (1024),
        .extended_info  = &graphics_mode_ext
    },
    {
        // dynamic mode (keep as last element of array)
        .bpp            = 24,
        .extended_info  = &graphics_mode_ext
    }
};

// map GDK events to GRX events
static const GrxEventType event_type_map[GDK_EVENT_LAST] = {
    [GDK_KEY_PRESS]         = GRX_EVENT_TYPE_KEY_DOWN,
    [GDK_KEY_RELEASE]       = GRX_EVENT_TYPE_KEY_UP,
    [GDK_BUTTON_PRESS]      = GRX_EVENT_TYPE_BUTTON_PRESS,
    [GDK_BUTTON_RELEASE]    = GRX_EVENT_TYPE_BUTTON_RELEASE,
    [GDK_2BUTTON_PRESS]     = GRX_EVENT_TYPE_BUTTON_DOUBLE_PRESS,
    [GDK_TOUCH_BEGIN]       = GRX_EVENT_TYPE_TOUCH_DOWN,
    [GDK_TOUCH_UPDATE]      = GRX_EVENT_TYPE_TOUCH_MOTION,
    [GDK_TOUCH_END]         = GRX_EVENT_TYPE_TOUCH_UP,
    [GDK_TOUCH_CANCEL]      = GRX_EVENT_TYPE_TOUCH_CANCEL,
};

static gboolean on_event (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    // only propagate GRX events if the window is active
    if (gtk_window_is_active (GTK_WINDOW (gtk_widget_get_toplevel (widget)))) {
        GrxEvent grx_event = { 0 };

        grx_event.type = event_type_map[event->type];
        switch (event->type) {
        case GDK_KEY_PRESS:
        case GDK_KEY_RELEASE:
            // GDK does not provide a device for keyboard events
            grx_event.key.device = NULL;
            grx_event.key.keysym = event->key.keyval;
            grx_event.key.unichar = gdk_keyval_to_unicode (event->key.keyval);
            grx_event.key.code = event->key.hardware_keycode;
            break;
        case GDK_BUTTON_PRESS:
        case GDK_BUTTON_RELEASE:
        case GDK_2BUTTON_PRESS:
            // grx_event.button.device = GRX_DEVICE (grx_gtk3_device_lookup
            //     (device_manager, event->button.device));
            grx_event.button.button = event->button.button;
            break;
        case GDK_TOUCH_BEGIN:
        case GDK_TOUCH_UPDATE:
        case GDK_TOUCH_END:
        case GDK_TOUCH_CANCEL:
            // grx_event.touch.device = GRX_DEVICE (grx_gtk3_device_lookup
            //     (device_manager, event->button.device));
            grx_event.touch.id = 0;
            grx_event.touch.x = event->touch.x;
            grx_event.touch.y = event->touch.y;
            break;
        default:
            return FALSE;
        }

        grx_event_put (&grx_event);

        // we have handled the GDK event, so don't propagate it.
        return TRUE;
    }

    return FALSE;
}

static void on_window_notify_is_active (GObject *object, GParamSpec *pspec,
                                        gpointer user_data)
{
    GrxEvent event;

    event.type = gtk_window_is_active (GTK_WINDOW (object)) ?
        GRX_EVENT_TYPE_APP_ACTIVATE : GRX_EVENT_TYPE_APP_DEACTIVATE;

    grx_event_put (&event);
}

static void on_window_destroy (GtkWidget *widget, gpointer user_data)
{
    GrxEvent event;

    event.type = GRX_EVENT_TYPE_APP_QUIT;

    grx_event_put (&event);
}

/*
 * set_cursor:
 *
 * Sets the cursor for the specified widget.
 *
 * The @event parameter is needed because this is a handler for
 * GtkWidget::enter-notify-event and GtkWidget::enter-leave-event
 */
static void set_cursor (GtkWidget *widget, GdkEvent *event, gchar *name)
{
    GdkCursor *mouse_cursor;

    mouse_cursor = gdk_cursor_new_from_name (gtk_widget_get_display (widget),
                                             name);
    gdk_window_set_cursor (gtk_widget_get_window (widget), mouse_cursor);
    g_object_unref (G_OBJECT (mouse_cursor));
}

static gboolean init (const gchar *options)
{
    GrxGtk3DeviceManager *device_manager;
    GError *err = NULL;
    GtkWidget *window;
    GtkWidget *event_box;

    detect ();
    g_return_val_if_fail (gtk_init_ok, FALSE);
    g_return_val_if_fail (image == NULL, FALSE);

    device_manager = g_initable_new (GRX_TYPE_GTK3_DEVICE_MANAGER, NULL, &err, NULL);
    if (!device_manager) {
        g_debug ("%s", err->message);
        g_error_free (err);

        return FALSE;
    }

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
    g_signal_connect (window, "notify::is-active",
                      (GCallback)on_window_notify_is_active, NULL);
    g_signal_connect (window, "destroy", (GCallback)on_window_destroy, NULL);

    // Have to have a GtkEventBox to handle events since GtkImage doesn't have
    // a GdkWindow.
    event_box = gtk_event_box_new ();
    gtk_widget_set_can_focus (event_box, TRUE); // for keyboard input
    gtk_widget_set_events (event_box, GDK_BUTTON_PRESS_MASK |
                           GDK_BUTTON_RELEASE_MASK | GDK_KEY_PRESS_MASK |
                           GDK_KEY_RELEASE_MASK | GDK_TOUCH_MASK);
    g_signal_connect (G_OBJECT (event_box), "event", (GCallback)on_event, NULL);
    g_signal_connect (G_OBJECT (event_box), "enter-notify-event",
                      (GCallback)set_cursor, "none");
    g_signal_connect (G_OBJECT (event_box), "leave-notify-event",
                      (GCallback)set_cursor, "default");

    image = gtk_image_new ();

    gtk_container_add (GTK_CONTAINER (window), event_box);
    gtk_container_add (GTK_CONTAINER (event_box), image);
    gtk_widget_show_all (window);

    DRVINFO->device_manager = GRX_DEVICE_MANAGER (device_manager);

    return TRUE;
}

static GrxVideoMode *
select_mode (GrxVideoDriver *driver, gint width, gint height, gint bpp,
             gboolean text, guint *ep)
{
    if (!text) {
        int i;

        for (i = 0; i < G_N_ELEMENTS(video_modes) - 1; i++) {
            GrxVideoMode *mode = &video_modes[i];

            if (mode->extended_info == &text_mode_ext) {
                continue;
            }
            if (mode->present && mode->width == width && mode->height == height) {
                goto out;
            }
        }
        video_modes[i].present = TRUE;
        video_modes[i].width = width;
        video_modes[i].height = height;
        video_modes[i].line_offset = ROWSTRIDE(width);
    }
out:
    return _gr_select_mode (driver, width, height, bpp, text, ep);
}

static guint get_dpi(GrxVideoDriver *driver)
{
    GdkScreen *screen;
    gdouble res;

    screen = gdk_screen_get_default();
    if (!screen) {
        return GRX_DEFAULT_DPI;
    }

    res = gdk_screen_get_resolution(screen);
    if (res < 0) {
        return GRX_DEFAULT_DPI;
    }

    return (guint)res;
}

G_MODULE_EXPORT GrxVideoDriver grx_gtk3_video_driver = {
    .name           = "gtk3",
    .flags          = GRX_VIDEO_DRIVER_FLAG_USER_RESOLUTION,
    .modes          = video_modes,
    .n_modes        = G_N_ELEMENTS (video_modes),
    .detect         = detect,
    .init           = init,
    .reset          = reset,
    .select_mode    = select_mode,
    .get_dpi        = get_dpi,
};
