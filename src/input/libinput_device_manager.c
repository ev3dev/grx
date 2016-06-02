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
#include <linux/input.h>

#include <glib.h>
#include <glib-object.h>
#include <glib/gstdio.h>
#include <gio/gio.h>

#include <grx/extents.h>
#include <grx/input_keys.h>
#include <grx/input_event.h>
#include <grx/libinput_device.h>
#include <grx/libinput_device_manager.h>
#include <grx/mode.h>

#include "libinput_device_internal.h"


// TODO: make double press time configurable
#define DOUBLE_PRESS_TIME 500

/* translates linux key codes to GRX key codes */
static const GrxKey keymap[KEY_CNT] = {
    [KEY_RESERVED]          = GRX_KEY_NONE,
    [KEY_ESC]               = GRX_KEY_ESCAPE,
    [KEY_1]                 = GRX_KEY_1,
    [KEY_2]                 = GRX_KEY_2,
    [KEY_3]                 = GRX_KEY_3,
    [KEY_4]                 = GRX_KEY_4,
    [KEY_5]                 = GRX_KEY_5,
    [KEY_6]                 = GRX_KEY_6,
    [KEY_7]                 = GRX_KEY_7,
    [KEY_8]                 = GRX_KEY_8,
    [KEY_9]                 = GRX_KEY_9,
    [KEY_0]                 = GRX_KEY_0,
    [KEY_MINUS]             = GRX_KEY_MINUS,
    [KEY_EQUAL]             = GRX_KEY_EQUAL,
    [KEY_BACKSPACE]         = GRX_KEY_BACKSPACE,
    [KEY_TAB]               = GRX_KEY_TAB,
    [KEY_Q]                 = GRX_KEY_Q,
    [KEY_W]                 = GRX_KEY_W,
    [KEY_E]                 = GRX_KEY_E,
    [KEY_R]                 = GRX_KEY_R,
    [KEY_T]                 = GRX_KEY_T,
    [KEY_Y]                 = GRX_KEY_Y,
    [KEY_U]                 = GRX_KEY_U,
    [KEY_I]                 = GRX_KEY_I,
    [KEY_O]                 = GRX_KEY_O,
    [KEY_P]                 = GRX_KEY_P,
    [KEY_LEFTBRACE]         = GRX_KEY_LEFT_BRACKET,
    [KEY_RIGHTBRACE]        = GRX_KEY_RIGHT_BRACKET,
    [KEY_ENTER]             = GRX_KEY_ENTER,
    [KEY_LEFTCTRL]          = GRX_KEY_LEFT_CTRL,
    [KEY_A]                 = GRX_KEY_A,
    [KEY_S]                 = GRX_KEY_S,
    [KEY_D]                 = GRX_KEY_D,
    [KEY_F]                 = GRX_KEY_F,
    [KEY_G]                 = GRX_KEY_G,
    [KEY_H]                 = GRX_KEY_H,
    [KEY_J]                 = GRX_KEY_J,
    [KEY_K]                 = GRX_KEY_K,
    [KEY_L]                 = GRX_KEY_L,
    [KEY_SEMICOLON]         = GRX_KEY_SEMICOLON,
    [KEY_APOSTROPHE]        = GRX_KEY_APOSTROPHE,
    [KEY_GRAVE]             = GRX_KEY_GRAVE_ACCENT,
    [KEY_LEFTSHIFT]         = GRX_KEY_LEFT_SHIFT,
    [KEY_BACKSLASH]         = GRX_KEY_BACKSLASH,
    [KEY_Z]                 = GRX_KEY_Z,
    [KEY_X]                 = GRX_KEY_X,
    [KEY_C]                 = GRX_KEY_C,
    [KEY_V]                 = GRX_KEY_V,
    [KEY_B]                 = GRX_KEY_B,
    [KEY_N]                 = GRX_KEY_N,
    [KEY_M]                 = GRX_KEY_M,
    [KEY_COMMA]             = GRX_KEY_COMMA,
    [KEY_DOT]               = GRX_KEY_PERIOD,
    [KEY_SLASH]             = GRX_KEY_SLASH,
    [KEY_RIGHTSHIFT]        = GRX_KEY_RIGHT_SHIFT,
    [KEY_KPASTERISK]        = GRX_KEY_KP_ASTERISK,
    [KEY_LEFTALT]           = GRX_KEY_LEFT_ALT,
    [KEY_SPACE]             = GRX_KEY_SPACE,
    [KEY_CAPSLOCK]          = GRX_KEY_CAPS_LOCK,
    [KEY_F1]                = GRX_KEY_F1,
    [KEY_F2]                = GRX_KEY_F2,
    [KEY_F3]                = GRX_KEY_F3,
    [KEY_F4]                = GRX_KEY_F4,
    [KEY_F5]                = GRX_KEY_F5,
    [KEY_F6]                = GRX_KEY_F6,
    [KEY_F7]                = GRX_KEY_F7,
    [KEY_F8]                = GRX_KEY_F8,
    [KEY_F9]                = GRX_KEY_F9,
    [KEY_F10]               = GRX_KEY_F10,
    [KEY_NUMLOCK]           = GRX_KEY_KP_NUM_LOCK,
    [KEY_SCROLLLOCK]        = GRX_KEY_SCROLL_LOCK,
    [KEY_KP7]               = GRX_KEY_KP_7,
    [KEY_KP8]               = GRX_KEY_KP_8,
    [KEY_KP9]               = GRX_KEY_KP_9,
    [KEY_KPMINUS]           = GRX_KEY_KP_MINUS,
    [KEY_KP4]               = GRX_KEY_KP_4,
    [KEY_KP5]               = GRX_KEY_KP_5,
    [KEY_KP6]               = GRX_KEY_KP_6,
    [KEY_KPPLUS]            = GRX_KEY_KP_PLUS,
    [KEY_KP1]               = GRX_KEY_KP_1,
    [KEY_KP2]               = GRX_KEY_KP_2,
    [KEY_KP3]               = GRX_KEY_KP_3,
    [KEY_KP0]               = GRX_KEY_KP_0,
    [KEY_KPDOT]             = GRX_KEY_KP_PERIOD,
    [KEY_ZENKAKUHANKAKU]    = GRX_KEY_NONE,
    [KEY_102ND]             = GRX_KEY_NONE,
    [KEY_F11]               = GRX_KEY_F11,
    [KEY_F12]               = GRX_KEY_F12,
    [KEY_RO]                = GRX_KEY_NONE,
    [KEY_KATAKANA]          = GRX_KEY_NONE,
    [KEY_HIRAGANA]          = GRX_KEY_NONE,
    [KEY_HENKAN]            = GRX_KEY_NONE,
    [KEY_KATAKANAHIRAGANA]  = GRX_KEY_NONE,
    [KEY_MUHENKAN]          = GRX_KEY_NONE,
    [KEY_KPJPCOMMA]         = GRX_KEY_NONE,
    [KEY_KPENTER]           = GRX_KEY_KP_ENTER,
    [KEY_RIGHTCTRL]         = GRX_KEY_RIGHT_CTRL,
    [KEY_KPSLASH]           = GRX_KEY_KP_SLASH,
    [KEY_SYSRQ]             = GRX_KEY_PRINT_SCREEN,
    [KEY_RIGHTALT]          = GRX_KEY_RIGHT_ALT,
    [KEY_LINEFEED]          = GRX_KEY_NONE,
    [KEY_HOME]              = GRX_KEY_HOME,
    [KEY_UP]                = GRX_KEY_UP_ARROW,
    [KEY_PAGEUP]            = GRX_KEY_PAGE_UP,
    [KEY_LEFT]              = GRX_KEY_LEFT_ARROW,
    [KEY_RIGHT]             = GRX_KEY_RIGHT_ARROW,
    [KEY_END]               = GRX_KEY_END,
    [KEY_DOWN]              = GRX_KEY_DOWN_ARROW,
    [KEY_PAGEDOWN]          = GRX_KEY_PAGE_DOWN,
    [KEY_INSERT]            = GRX_KEY_INSERT,
    [KEY_DELETE]            = GRX_KEY_DELETE,
    [KEY_MACRO]             = GRX_KEY_NONE,
    [KEY_MUTE]              = GRX_KEY_NONE,
    [KEY_VOLUMEDOWN]        = GRX_KEY_NONE,
    [KEY_VOLUMEUP]          = GRX_KEY_NONE,
    [KEY_POWER]             = GRX_KEY_NONE,
    [KEY_KPEQUAL]           = GRX_KEY_NONE,
    [KEY_KPPLUSMINUS]       = GRX_KEY_NONE,
    [KEY_PAUSE]             = GRX_KEY_PAUSE,
    [KEY_SCALE]             = GRX_KEY_NONE,
    [KEY_KPCOMMA]           = GRX_KEY_NONE,
    [KEY_HANGEUL]           = GRX_KEY_NONE,
    [KEY_HANJA]             = GRX_KEY_NONE,
    [KEY_YEN]               = GRX_KEY_NONE,
    [KEY_LEFTMETA]          = GRX_KEY_LEFT_SUPER,
    [KEY_RIGHTMETA]         = GRX_KEY_RIGHT_SUPER,
    [KEY_COMPOSE]           = GRX_KEY_NONE,
};

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
            grx_event.key.key = keymap[key];
            grx_event.key.code = key;
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

guint grx_libinput_device_manager_event_add (GrxLibinputDeviceManager *context,
                                     GrxLibinputDeviceManagerSourceFunc func,
                                     gpointer user_data, GDestroyNotify notify)
{
    GrxLibinputDeviceManagerPrivate *priv = context->private;
    GSource *source;
    guint id;

    source = grx_libinput_device_manager_source_new (context);
    g_source_set_callback (source, (GSourceFunc)func, user_data, notify);
    id = g_source_attach (source, g_main_context_default ());
    g_source_unref (source);

    return id;
}
