/*
 * gtk3.c - Linux Console Application
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
#include <gio/gio.h>
#include <gtk/gtk.h>

#include <grx/context.h>
#include <grx/device.h>
#include <grx/draw.h>
#include <grx/extents.h>
#include <grx/gtk3_application.h>
#include <grx/mode.h>

#include "gtk3_device.h"
#include "gtk3_device_manager.h"

/**
 * SECTION:gtk3_application
 * @title: GrxGtk3Application
 * @short_description: GTK+ 3 application class
 * @include: grx-3.0.h
 *
 * #GrxGtk3Application extends #GApplication for GTK+ 3
 * applications using the Grx graphics library.
 */

typedef struct {
    GtkWidget *window;
    GrxGtk3DeviceManager *device_manager;
} GrxGtk3ApplicationPrivate;

static void initable_interface_init (GInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE (GrxGtk3Application,
    grx_gtk3_application, G_TYPE_APPLICATION,
    G_ADD_PRIVATE (GrxGtk3Application)
    G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, initable_interface_init))

/* Properties */

enum {
    PROP_0,
    PROP_DEVICE_MANAGER,
    PROP_IS_CONSOLE_ACTIVE,
    N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES] = { NULL };

/**
 * grx_gtk3_application_get_device_manager:
 * @application: a #GrxGtk3Application
 *
 * Gets the #GrxGtk3DeviceManager for this application.
 *
 * Returns: (transfer none): the #GrxGtk3DeviceManager
 */
GrxGtk3DeviceManager *
grx_gtk3_application_get_device_manager (GrxGtk3Application *application)
{
    GrxGtk3ApplicationPrivate *priv =
        grx_gtk3_application_get_instance_private (application);

    return priv->device_manager;
}

/**
 * grx_gtk3_application_is_console_active:
 * @application: a #GrxGtk3Application
 *
 * Gets the status of the #GrxGtk3Application:is-console-active property
 *
 * Returns: %TRUE if the console this application is running on is the active
 * console.
 */
gboolean
grx_gtk3_application_is_console_active (GrxGtk3Application *application)
{
    GrxGtk3ApplicationPrivate *priv =
        grx_gtk3_application_get_instance_private (application);

    return gtk_window_is_active (GTK_WINDOW (priv->window));
}

static void
set_property (GObject *object, guint property_id, const GValue *value,
              GParamSpec *pspec)
{
    switch (property_id) {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
get_property (GObject *object, guint property_id, GValue *value,
              GParamSpec *pspec)
{
    GrxGtk3Application *self = GRX_GTK3_APPLICATION (object);
    GrxGtk3ApplicationPrivate *priv =
        grx_gtk3_application_get_instance_private (self);

    switch (property_id) {
    case PROP_DEVICE_MANAGER:
        g_value_set_object (value, priv->device_manager);
        break;
    case PROP_IS_CONSOLE_ACTIVE:
        g_value_set_boolean (value,
                             gtk_window_is_active (GTK_WINDOW (priv->window)));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

/* class implementation */

static void on_window_is_active (GObject *window, GParamSpec *pspec,
                                 gpointer user_data)
{
    GObject *self = G_OBJECT (user_data);

    g_object_notify_by_pspec (self, properties[PROP_IS_CONSOLE_ACTIVE]);
}

static void on_window_destroy (GtkWidget *widget, gpointer user_data)
{
    GApplication *application = user_data;

    g_application_quit (application);
}

static void startup (GApplication *application)
{
    GrxGtk3Application *self = GRX_GTK3_APPLICATION (application);
    GrxGtk3ApplicationPrivate *priv =
            grx_gtk3_application_get_instance_private (self);

    G_APPLICATION_CLASS (grx_gtk3_application_parent_class)->
        startup (application);

    g_application_hold (application);
    gtk_widget_show_all (priv->window);
    g_signal_connect (priv->window, "notify::is-active",
                      (GCallback)on_window_is_active, self);
    g_signal_connect (priv->window, "destroy", (GCallback)on_window_destroy,
                      application);
}

static void
input_event (GrxGtk3Application *application, GrxInputEvent *event)
{
}

static void finalize (GObject *object)
{
    GrxGtk3Application *self = GRX_GTK3_APPLICATION (object);
    GrxGtk3ApplicationPrivate *priv =
            grx_gtk3_application_get_instance_private (self);

    G_OBJECT_CLASS (grx_gtk3_application_parent_class)->finalize (object);

    g_object_unref (G_OBJECT (priv->device_manager));
}

static void
grx_gtk3_application_class_init (GrxGtk3ApplicationClass *klass)
{
    G_OBJECT_CLASS (klass)->set_property = set_property;
    G_OBJECT_CLASS (klass)->get_property = get_property;

    properties[PROP_DEVICE_MANAGER] =
        g_param_spec_object ("device-manager",
                             "input device manager",
                             "the input device manager for this application.",
                             GRX_TYPE_GTK3_DEVICE_MANAGER,
                             G_PARAM_READABLE);
    properties[PROP_IS_CONSOLE_ACTIVE] =
        g_param_spec_boolean ("is-console-active",
                              "console is active",
                              "Gets if the console this application is running on is the active console.",
                              FALSE /* default value */,
                              G_PARAM_READABLE);
    g_object_class_install_properties (G_OBJECT_CLASS (klass),
                                       N_PROPERTIES,
                                       properties);

    G_APPLICATION_CLASS (klass)->startup = startup;
    G_OBJECT_CLASS (klass)->finalize = finalize;

    klass->input_event = input_event;
}

static void
grx_gtk3_application_init (GrxGtk3Application *self)
{
    GrxGtk3ApplicationPrivate *priv =
        grx_gtk3_application_get_instance_private (self);

    priv->device_manager = g_object_new (GRX_TYPE_GTK3_DEVICE_MANAGER, NULL);
}

/* interface implementation */

// map GDK events to GRX events
static const GrxInputEventType event_type_map[GDK_EVENT_LAST] = {
    [GDK_KEY_PRESS]         = GRX_INPUT_EVENT_TYPE_KEY_DOWN,
    [GDK_KEY_RELEASE]       = GRX_INPUT_EVENT_TYPE_KEY_UP,
    [GDK_BUTTON_PRESS]      = GRX_INPUT_EVENT_TYPE_BUTTON_PRESS,
    [GDK_BUTTON_RELEASE]    = GRX_INPUT_EVENT_TYPE_BUTTON_RELEASE,
    [GDK_2BUTTON_PRESS]     = GRX_INPUT_EVENT_TYPE_BUTTON_DOUBLE_PRESS,
    [GDK_TOUCH_BEGIN]       = GRX_INPUT_EVENT_TYPE_TOUCH_DOWN,
    [GDK_TOUCH_UPDATE]      = GRX_INPUT_EVENT_TYPE_TOUCH_MOTION,
    [GDK_TOUCH_END]         = GRX_INPUT_EVENT_TYPE_TOUCH_UP,
    [GDK_TOUCH_CANCEL]      = GRX_INPUT_EVENT_TYPE_TOUCH_CANCEL,
};

static gboolean on_event (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    GrxGtk3Application *self = GRX_GTK3_APPLICATION (user_data);
    GrxGtk3ApplicationClass *klass = GRX_GTK3_APPLICATION_GET_CLASS (self);
    GrxGtk3ApplicationPrivate *priv =
        grx_gtk3_application_get_instance_private (self);

    // only propagate GRX events if the window is active
    if (gtk_window_is_active (GTK_WINDOW (priv->window))) {
        GrxInputEvent grx_event;

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
            grx_event.button.device = GRX_DEVICE (grx_gtk3_device_lookup
                (priv->device_manager, event->button.device));
            grx_event.button.button = event->button.button;
            break;
        case GDK_TOUCH_BEGIN:
        case GDK_TOUCH_UPDATE:
        case GDK_TOUCH_END:
        case GDK_TOUCH_CANCEL:
            grx_event.touch.device = GRX_DEVICE (grx_gtk3_device_lookup
                (priv->device_manager, event->button.device));
            grx_event.touch.id = 0;
            grx_event.touch.x = event->touch.x;
            grx_event.touch.y = event->touch.y;
            break;
        default:
            return FALSE;
        }

        klass->input_event (self, &grx_event);

        // we have handled the GDK event, so don't propagate it.
        return TRUE;
    }

    return FALSE;
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

static gboolean init (GInitable *initable, GCancellable *cancellable,
                      GError **error)
{
    GrxGtk3Application *self = GRX_GTK3_APPLICATION (initable);
    GrxGtk3ApplicationPrivate *priv =
        grx_gtk3_application_get_instance_private (self);
    GdkPixbufSimpleAnim *animation;
    GdkPixbuf *pixbuf;
    GtkWidget *image;
    GtkWidget *event_box;
    gboolean ret;

    grx_set_error_handling (FALSE);
    ret = grx_set_mode_default_graphics (TRUE);
    if (!ret) {
        g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED,
            "Could not set graphics mode.");
        return ret;
    }

    priv->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position (GTK_WINDOW (priv->window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable (GTK_WINDOW (priv->window), FALSE);

    // Using an animation get the pixbuf to automatically refresh on the screen.
    // GRX does not have a way of notifying that a context is "dirty".
    animation = gdk_pixbuf_simple_anim_new (grx_get_screen_width (),
                                            grx_get_screen_height (), 25);
    pixbuf = GDK_PIXBUF (grx_get_virtual_video_mode ()->user_data);
    gdk_pixbuf_simple_anim_add_frame (animation, pixbuf);
    gdk_pixbuf_simple_anim_set_loop (animation, TRUE);
    image = gtk_image_new_from_animation (GDK_PIXBUF_ANIMATION (animation));

    // Have to have a GtkEventBox to handle events since GtkImage doesn't have
    // a GdkWindow.
    event_box = gtk_event_box_new ();
    gtk_widget_set_can_focus (event_box, TRUE); // for keyboard input
    gtk_widget_set_events (event_box, GDK_BUTTON_PRESS_MASK |
                           GDK_BUTTON_RELEASE_MASK | GDK_KEY_PRESS_MASK |
                           GDK_KEY_RELEASE_MASK | GDK_TOUCH_MASK);
    g_signal_connect (G_OBJECT (event_box), "event", (GCallback)on_event, self);
    g_signal_connect (G_OBJECT (event_box), "enter-notify-event",
                      (GCallback)set_cursor, "none");
    g_signal_connect (G_OBJECT (event_box), "leave-notify-event",
                      (GCallback)set_cursor, "default");

    gtk_container_add (GTK_CONTAINER (priv->window), event_box);
    gtk_container_add (GTK_CONTAINER (event_box), image);

    // It is important to call this after setting the graphics mode, otherwise
    // it could fail because GTK is not initialized.
    if (!g_initable_init (G_INITABLE (priv->device_manager), cancellable, error)) {
        return FALSE;
    }

    return g_application_register (G_APPLICATION (initable), cancellable, error);
}

static void initable_interface_init (GInitableIface *iface)
{
    iface->init = init;
}

/* constructors */

/**
 * grx_gtk3_application_new:
 * @cancellable: (nullable): a #GCancellable, or %NULL
 * @error: a pointer to a NULL #GError, or %NULL
 *
 * Create a new instance of #GrxGtk3Application. The console will be
 * set to graphics mode or there will be an @error if setting graphics mode
 * failed.
 *
 * Returns: (nullable): New instance of #GrxGtk3Application or %NULL
 * if init() failed.
 */
GrxGtk3Application *
grx_gtk3_application_new (GCancellable *cancellable, GError **error)
{
    gboolean ret;

    GrxGtk3Application *app =
        g_object_new (GRX_TYPE_GTK3_APPLICATION, NULL);

    ret = init (G_INITABLE (app), cancellable, error);
    if (!ret) {
        g_object_unref (app);
        return NULL;
    }

    return app;
}
