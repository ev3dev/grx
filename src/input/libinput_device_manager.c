/*
 * libinput_device_manager.c
 *
 * Copyright (c) 2016 David Lechner <david@lechnology.com>
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
#include <libinput.h>
#include <libudev.h>

#include <glib.h>
#include <glib-object.h>
#include <glib/gstdio.h>
#include <gio/gio.h>

#include <grx/extents.h>
#include <grx/libinput_device.h>
#include <grx/libinput_device_manager.h>
#include <grx/mode.h>

#include "libinput_device_internal.h"

/**
 * GrxLibinputDeviceManager:
 *
 * #GrxLibinputDeviceManager is an opaque data structure and can only be
 * accessed using the following functions.
 */

/**
 * GrxLibinputDeviceManagerClass:
 *
 * Nothing here yet.
 */

// TODO: make double press time configurable
#define DOUBLE_PRESS_TIME 500

typedef struct {
    struct libinput *context;
    GList *devices;
} GrxLibinputDeviceManagerPrivate;

static void initable_interface_init (GInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE (GrxLibinputDeviceManager,
    grx_libinput_device_manager, G_TYPE_OBJECT,
    G_ADD_PRIVATE (GrxLibinputDeviceManager)
    G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, initable_interface_init))

/* Properties */

enum {
    PROP_0,
    N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES] = { NULL };

static void
set_property (GObject *object, guint property_id, const GValue *value,
              GParamSpec *pspec)
{
    GrxLibinputDeviceManager *self = GRX_LIBINPUT_DEVICE_MANAGER (object);

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
    GrxLibinputDeviceManager *self = GRX_LIBINPUT_DEVICE_MANAGER (object);
    GrxLibinputDeviceManagerPrivate *priv = self->private;

    switch (property_id) {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

/* signals */

enum {
    SIG_0,
    SIG_DEVICE_ADDED,
    SIG_DEVICE_REMOVED,
    N_SIGNALS
};

static uint signals[N_SIGNALS] = { 0 };

/* class implementation */

static void finalize (GObject *object)
{
    GrxLibinputDeviceManager *self = GRX_LIBINPUT_DEVICE_MANAGER (object);
    GrxLibinputDeviceManagerPrivate *priv = self->private;

    G_OBJECT_CLASS (grx_libinput_device_manager_parent_class)->finalize (object);
    priv->context = libinput_unref (priv->context);
}

static void
grx_libinput_device_manager_class_init (GrxLibinputDeviceManagerClass *klass)
{
    G_OBJECT_CLASS (klass)->set_property = set_property;
    G_OBJECT_CLASS (klass)->get_property = get_property;

    signals[SIG_DEVICE_ADDED] =
        g_signal_new ("device-added",
                      G_TYPE_FROM_CLASS (klass),
                      G_SIGNAL_RUN_LAST,
                      0, /* class offset */
                      NULL, /* accumulator */
                      NULL, /* accumulator data */
                      NULL, /* C marshaller */
                      G_TYPE_NONE, /* return type */
                      1, /* n_params */
                      GRX_TYPE_LIBINPUT_DEVICE /* types */);
    signals[SIG_DEVICE_REMOVED] =
        g_signal_new ("device-removed",
                      G_TYPE_FROM_CLASS (klass),
                      G_SIGNAL_RUN_LAST,
                      0, /* class offset */
                      NULL, /* accumulator */
                      NULL, /* accumulator data */
                      NULL, /* C marshaller */
                      G_TYPE_NONE, /* return type */
                      1, /* n_params */
                      GRX_TYPE_LIBINPUT_DEVICE /* types */);

    G_OBJECT_CLASS (klass)->finalize = finalize;
}

static int open_restricted (const char *path, int flags, void *user_data)
{
    return g_open (path, flags, O_RDONLY);
}

static void close_restricted (int fd, void *user_data)
{
    g_close (fd, NULL);
}

static const struct libinput_interface libinput_interface_impl = {
    .open_restricted = open_restricted,
    .close_restricted = close_restricted,
};

static void
grx_libinput_device_manager_init (GrxLibinputDeviceManager *self)
{
    GrxLibinputDeviceManagerPrivate *priv =
        grx_libinput_device_manager_get_instance_private (self);

    self->private = priv;
}

/* interface implementation */

static gboolean
init (GInitable *initable, GCancellable *cancellable, GError **error)
{
    GrxLibinputDeviceManager *self = GRX_LIBINPUT_DEVICE_MANAGER (initable);
    GrxLibinputDeviceManagerPrivate *priv = self->private;
    struct udev *udev;

    udev = udev_new ();
    priv->context = libinput_udev_create_context (&libinput_interface_impl, self, udev);
    udev_unref (udev);
    if (!priv->context) {
        g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED,
                     "Failed to create libinput context.");
        return FALSE;
    }

    if (libinput_udev_assign_seat (priv->context, "seat0") < 0) {
        priv->context = libinput_unref (priv->context);
        g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED,
                     "Failed to assign seat.");
        return FALSE;
    }

    return TRUE;
}

static void initable_interface_init (GInitableIface *iface)
{
    iface->init = init;
}

/* constructors */

/**
 * grx_libinput_device_manager_new:
 * @cancellable: (nullable): not used - pass %NULL
 * @error: a pointer to a #GError
 *
 * Creates a new instance of #GrxLibinputDeviceManager
 *
 * Returns: the new instance or %NULL if initialization failed
 */
GrxLibinputDeviceManager *
grx_libinput_device_manager_new (GCancellable *cancellable, GError **error)
{
    GrxLibinputDeviceManager *instance = g_object_new (GRX_TYPE_LIBINPUT_DEVICE_MANAGER, NULL);
    if (!g_initable_init (G_INITABLE (instance), cancellable, error)) {
        g_object_unref (G_OBJECT (instance));
        return NULL;
    }

    return instance;
}

/* source implementation */

static gboolean source_prepare (GSource *source, gint *timeout)
{
    GrxLibinputDeviceManager *instance =
        ((GrxLibinputDeviceManagerSource*)source)->instance;
    GrxLibinputDeviceManagerPrivate *priv = instance->private;
    enum libinput_event_type type;

    type = libinput_next_event_type (priv->context);
    *timeout = -1;

    return type != LIBINPUT_EVENT_NONE;
}

static gboolean source_check (GSource *source)
{
    GrxLibinputDeviceManager *instance =
        ((GrxLibinputDeviceManagerSource*)source)->instance;
    GrxLibinputDeviceManagerPrivate *priv = instance->private;
    enum libinput_event_type type;

    libinput_dispatch (priv->context);
    type = libinput_next_event_type (priv->context);

    return type != LIBINPUT_EVENT_NONE;
}

static gboolean
source_dispatch (GSource *source, GSourceFunc callback, gpointer user_data)
{
    GrxLibinputDeviceManagerSource *real_source =
        (GrxLibinputDeviceManagerSource*)source;
    GrxLibinputDeviceManagerSourceFunc real_callback =
        (GrxLibinputDeviceManagerSourceFunc)callback;
    GrxLibinputDeviceManagerPrivate *priv = real_source->instance->private;
    struct libinput_event *event;
    struct libinput_device *device;
    GrxLibinputDevice *grx_device;
    enum libinput_event_type type;
    GrxInputEvent grx_event;

    event = libinput_get_event (priv->context);
    if (!event) {
        return G_SOURCE_CONTINUE;
    }

    type = libinput_event_get_type (event);

    device = libinput_event_get_device (event);
    grx_device = libinput_device_get_user_data (device);
    if (!grx_device) {
        grx_device = grx_libinput_device_new (device);
        if (type != LIBINPUT_EVENT_DEVICE_ADDED) {
            // we are making the assumption that the priv->devices list will
            // take ownership of the reference to grx_device
            g_critical ("New device without LIBINPUT_EVENT_DEVICE_ADDED event");
        }
    }
    grx_event.any.device = grx_device;

    grx_event.type = GRX_INPUT_EVENT_TYPE_NONE;
    switch (type) {
    case LIBINPUT_EVENT_DEVICE_ADDED:
        priv->devices = g_list_append (priv->devices, grx_device);
        g_signal_emit (real_source->instance, signals[SIG_DEVICE_ADDED], 0,
                       grx_device);
        break;
    case LIBINPUT_EVENT_DEVICE_REMOVED:
        priv->devices = g_list_remove (priv->devices, grx_device);
        g_signal_emit (real_source->instance, signals[SIG_DEVICE_REMOVED], 0,
                       grx_device);
        g_object_unref (grx_device);
        break;
    case LIBINPUT_EVENT_KEYBOARD_KEY:
        {
            struct libinput_event_keyboard *keyboard;
            uint32_t key;

            keyboard = libinput_event_get_keyboard_event (event);
            key = libinput_event_keyboard_get_key (keyboard);
            switch (libinput_event_keyboard_get_key_state (keyboard)) {
            case LIBINPUT_KEY_STATE_PRESSED:
                grx_event.type = GRX_INPUT_EVENT_TYPE_KEY_DOWN;
                break;
            case LIBINPUT_KEY_STATE_RELEASED:
                grx_event.type = GRX_INPUT_EVENT_TYPE_KEY_UP;
                break;
            }
            grx_event.key.key = key;
        }
        break;
    case LIBINPUT_EVENT_POINTER_MOTION:
        // TODO: handle event
        break;
    case LIBINPUT_EVENT_POINTER_MOTION_ABSOLUTE:
        // TODO: handle event
        break;
    case LIBINPUT_EVENT_POINTER_BUTTON:
        {
            struct libinput_event_pointer *pointer;
            uint32_t button, time;

            pointer = libinput_event_get_pointer_event (event);
            button = libinput_event_pointer_get_button (pointer);
            switch (libinput_event_pointer_get_button_state (pointer)) {
            case LIBINPUT_BUTTON_STATE_PRESSED:
                grx_event.type = GRX_INPUT_EVENT_TYPE_BUTTON_PRESS;
                // double press detection
                time = libinput_event_pointer_get_time (pointer);
                if (button == real_source->last_button &&
                    (time - real_source->last_button_time) < DOUBLE_PRESS_TIME)
                {
                    // double press does two callbacks, one here for
                    // GRX_INPUT_EVENT_BUTTON_PRESS and one in the usual way
                    // for GRX_INPUT_EVENT_BUTTON_DOUBLE_PRESS
                    real_callback (&grx_event, user_data);
                    grx_event.type = GRX_INPUT_EVENT_TYPE_BUTTON_DOUBLE_PRESS;
                    real_source->last_button = 0;
                    real_source->last_button_time = 0;
                } else {
                    real_source->last_button = button;
                    real_source->last_button_time = time;
                }
                break;
            case LIBINPUT_BUTTON_STATE_RELEASED:
                grx_event.type = GRX_INPUT_EVENT_TYPE_BUTTON_RELEASE;
                break;
            }
            grx_event.button.button = button;
        }
        break;
    case LIBINPUT_EVENT_TOUCH_DOWN:
    case LIBINPUT_EVENT_TOUCH_UP:
    case LIBINPUT_EVENT_TOUCH_MOTION:
    case LIBINPUT_EVENT_TOUCH_CANCEL:
        {
            struct libinput_event_touch *touch;
            int32_t slot;
            double x, y;

            touch = libinput_event_get_touch_event (event);
            slot = libinput_event_touch_get_slot (touch);
            if (type == LIBINPUT_EVENT_TOUCH_DOWN
                || type == LIBINPUT_EVENT_TOUCH_MOTION)
            {
                x = libinput_event_touch_get_x_transformed (touch,
                    grx_get_screen_width ());
                y = libinput_event_touch_get_y_transformed (touch,
                    grx_get_screen_height ());
            }
            switch (type) {
            case LIBINPUT_EVENT_TOUCH_DOWN:
                grx_event.type = GRX_INPUT_EVENT_TYPE_TOUCH_DOWN;
                break;
            case LIBINPUT_EVENT_TOUCH_UP:
                grx_event.type = GRX_INPUT_EVENT_TYPE_TOUCH_UP;
                break;
            case LIBINPUT_EVENT_TOUCH_MOTION:
                grx_event.type = GRX_INPUT_EVENT_TYPE_TOUCH_MOTION;
                break;
            case LIBINPUT_EVENT_TOUCH_CANCEL:
                grx_event.type = GRX_INPUT_EVENT_TYPE_TOUCH_CANCEL;
                break;
            }
            grx_event.touch.id = slot;
            grx_event.touch.x = x;
            grx_event.touch.y = y;
        }
        break;
    }

    if (grx_event.type != GRX_INPUT_EVENT_TYPE_NONE) {
        real_callback (&grx_event, user_data);
    }

    libinput_event_destroy (event);

    return G_SOURCE_CONTINUE;
}

static void source_finalize (GSource *source)
{
    GrxLibinputDeviceManager *instance =
        ((GrxLibinputDeviceManagerSource*)source)->instance;
    GrxLibinputDeviceManagerPrivate *priv = instance->private;

    g_list_free_full (priv->devices, (GDestroyNotify)g_object_unref);

    g_object_unref(G_OBJECT (instance));
}

static GSourceFuncs source_funcs = {
    .prepare    = source_prepare,
    .check      = source_check,
    .dispatch   = source_dispatch,
    .finalize   = source_finalize,
};

/* methods */

/**
 * grx_libinput_device_manager_source_new:
 * @context: #GrxLibinputDeviceManager instance
 *
 * Creates a new source that will be dispatched when input events occur.
 *
 * The source will not initially be associated with any #GMainContext and must
 * be added to one with g_source_attach() before it will be executed.
 *
 * Returns: a new #GSource
 */
GSource *grx_libinput_device_manager_source_new (GrxLibinputDeviceManager *context)
{
    GrxLibinputDeviceManagerPrivate *priv = context->private;
    GSource *source;

    source = g_source_new (&source_funcs, sizeof(GrxLibinputDeviceManagerSource));
    ((GrxLibinputDeviceManagerSource*)source)->instance = g_object_ref(G_OBJECT (context));
    g_source_add_unix_fd (source, libinput_get_fd (priv->context), G_IO_IN);

    return source;
}

void grx_libinput_device_manager_event_add (GrxLibinputDeviceManager *context,
                                     GrxLibinputDeviceManagerSourceFunc func,
                                     gpointer user_data, GDestroyNotify notify)
{
    GrxLibinputDeviceManagerPrivate *priv = context->private;
    GSource *source;

    source = grx_libinput_device_manager_source_new (context);
    g_source_set_callback (source, (GSourceFunc)func, user_data, notify);
    g_source_attach (source, g_main_context_default ());
    g_source_unref (source);
}
