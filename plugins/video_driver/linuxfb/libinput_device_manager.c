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

#include <grx/device.h>
#include <grx/events.h>
#include <grx/extents.h>
#include <grx/input_keys.h>
#include <grx/input_keysyms.h>
#include <grx/mode.h>

#include "libinput_device.h"
#include "libinput_device_manager.h"
#include "globals.h"


// TODO: make double press time configurable
#define DOUBLE_PRESS_TIME 500

struct _GrxLibinputDeviceManager
{
    GrxDeviceManager parent_instance;
    struct libinput *libinput;
    struct xkb_context *xkb;
    struct xkb_keymap *keymap;
    GList *devices;
};

static void initable_interface_init (GInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE (GrxLibinputDeviceManager,
    grx_libinput_device_manager, GRX_TYPE_DEVICE_MANAGER,
    G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, initable_interface_init))

/* class implementation */

static void finalize (GObject *object)
{
    GrxLibinputDeviceManager *self = GRX_LIBINPUT_DEVICE_MANAGER (object);

    G_OBJECT_CLASS (grx_libinput_device_manager_parent_class)->finalize (object);

    xkb_keymap_unref (self->keymap);
    xkb_context_unref (self->xkb);
    self->libinput = libinput_unref (self->libinput);
}

static void
grx_libinput_device_manager_class_init (GrxLibinputDeviceManagerClass *klass)
{
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
}

/* interface implementation */

static gboolean
init (GInitable *initable, GCancellable *cancellable, GError **error)
{
    GrxLibinputDeviceManager *self = GRX_LIBINPUT_DEVICE_MANAGER (initable);
    struct udev *udev;

    udev = udev_new ();
    self->libinput = libinput_udev_create_context (&libinput_interface_impl, self, udev);
    udev_unref (udev);
    if (!self->libinput) {
        g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED,
                     "Failed to create libinput context.");
        return FALSE;
    }

    if (libinput_udev_assign_seat (self->libinput, "seat0") < 0) {
        g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED,
                     "Failed to assign seat.");
        goto err1;
    }

    self->xkb = xkb_context_new (XKB_CONTEXT_NO_FLAGS);
    if (!self->xkb) {
        g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED,
                     "Failed to create xkb context.");
        goto err1;
    }

    self->xkb = xkb_context_new (XKB_CONTEXT_NO_FLAGS);
    if (!self->xkb) {
        g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED,
                     "Failed to create xkb context.");
        goto err1;
    }

    self->keymap = xkb_keymap_new_from_names (self->xkb, NULL,
                                              XKB_KEYMAP_COMPILE_NO_FLAGS);
    if (!self->keymap) {
        g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED,
                     "Failed to create xkb keymap.");
        goto err2;
    }

    return TRUE;

err2:
    xkb_context_unref (self->xkb);
err1:
    self->libinput = libinput_unref (self->libinput);

    return FALSE;
}

static void initable_interface_init (GInitableIface *iface)
{
    iface->init = init;
}

/* source implementation */

static gboolean source_prepare (GSource *source, gint *timeout)
{
    GrxLibinputDeviceManager *instance =
        ((GrxLibinputDeviceManagerSource *)source)->instance;
    enum libinput_event_type type;

    type = libinput_next_event_type (instance->libinput);
    *timeout = -1;

    return type != LIBINPUT_EVENT_NONE;
}

static gboolean source_check (GSource *source)
{
    GrxLibinputDeviceManager *instance =
        ((GrxLibinputDeviceManagerSource *)source)->instance;
    enum libinput_event_type type;

    libinput_dispatch (instance->libinput);
    type = libinput_next_event_type (instance->libinput);

    return type != LIBINPUT_EVENT_NONE;
}

// TODO: put this in a header file
extern void grx_linuxfb_chvt (int vt_num);
extern void grx_linuxfb_update_pointer (gint32 dx, gint32 dy, gint32 *x, gint32 *y);

static gboolean
source_dispatch (GSource *source, GSourceFunc callback, gpointer user_data)
{
    GrxLibinputDeviceManager *instance =
        ((GrxLibinputDeviceManagerSource *)source)->instance;
    struct libinput_event *event;
    struct libinput_device *device;
    GrxLibinputDevice *grx_device;
    enum libinput_event_type type;
    GrxEvent grx_event = { 0 };

    event = libinput_get_event (instance->libinput);
    if (!event) {
        return G_SOURCE_CONTINUE;
    }

    type = libinput_event_get_type (event);

    device = libinput_event_get_device (event);
    grx_device = libinput_device_get_user_data (device);
    if (!grx_device) {
        grx_device = grx_libinput_device_new (device, instance->keymap);
        if (type != LIBINPUT_EVENT_DEVICE_ADDED) {
            // we are making the assumption that the instance->devices list will
            // take ownership of the reference to grx_device
            g_critical ("New device without LIBINPUT_EVENT_DEVICE_ADDED event");
        }
    }

// we do not handle all cases of the enum and that is OK
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"

    grx_event.type = GRX_EVENT_TYPE_NONE;
    switch (type) {
    case LIBINPUT_EVENT_DEVICE_ADDED:
        instance->devices = g_list_append (instance->devices, grx_device);
        g_signal_emit_by_name (instance, "device-added",
                               grx_device);
        break;
    case LIBINPUT_EVENT_DEVICE_REMOVED:
        instance->devices = g_list_remove (instance->devices, grx_device);
        g_signal_emit_by_name (instance, "device-removed",
                               grx_device);
        g_object_unref (grx_device);
        break;
    case LIBINPUT_EVENT_KEYBOARD_KEY:
        {
            struct libinput_event_keyboard *keyboard;
            uint32_t key;
            xkb_keycode_t keycode;

            keyboard = libinput_event_get_keyboard_event (event);
            key = libinput_event_keyboard_get_key (keyboard);
            keycode = key + 8; // xkb keymaps are usually offset by 8
            switch (libinput_event_keyboard_get_key_state (keyboard)) {
            case LIBINPUT_KEY_STATE_PRESSED:
                grx_event.type = GRX_EVENT_TYPE_KEY_DOWN;
                grx_libinput_device_update_state (grx_device, keycode,
                    XKB_KEY_DOWN, &grx_event.key.keysym, &grx_event.key.unichar);
                break;
            case LIBINPUT_KEY_STATE_RELEASED:
                grx_event.type = GRX_EVENT_TYPE_KEY_UP;
                grx_libinput_device_update_state (grx_device, keycode,
                    XKB_KEY_UP, &grx_event.key.keysym, &grx_event.key.unichar);
                break;
            }
            switch (grx_event.key.keysym) {
            case GRX_KEY_XF86Switch_VT_1:
            case GRX_KEY_XF86Switch_VT_2:
            case GRX_KEY_XF86Switch_VT_3:
            case GRX_KEY_XF86Switch_VT_4:
            case GRX_KEY_XF86Switch_VT_5:
            case GRX_KEY_XF86Switch_VT_6:
            case GRX_KEY_XF86Switch_VT_7:
            case GRX_KEY_XF86Switch_VT_8:
            case GRX_KEY_XF86Switch_VT_9:
            case GRX_KEY_XF86Switch_VT_10:
            case GRX_KEY_XF86Switch_VT_11:
            case GRX_KEY_XF86Switch_VT_12:
                if (grx_event.type == GRX_EVENT_TYPE_KEY_DOWN) {
                    grx_linuxfb_chvt (grx_event.key.keysym & 0xf);
                }
                // prevent these key presses/releases from being queued
                grx_event.type = GRX_EVENT_TYPE_NONE;
                break;
            }
            grx_event.key.code = key;
            grx_event.key.device = GRX_DEVICE (grx_device);
        }
        break;
    case LIBINPUT_EVENT_POINTER_MOTION:
    case LIBINPUT_EVENT_POINTER_MOTION_ABSOLUTE:
        {
            struct libinput_event_pointer *pointer;
            double dx, dy;
            gint32 x, y;

            pointer = libinput_event_get_pointer_event (event);
            if (type == LIBINPUT_EVENT_POINTER_MOTION) {
                dx = libinput_event_pointer_get_dx (pointer);
                dy = libinput_event_pointer_get_dy (pointer);
                grx_linuxfb_update_pointer(dx, dy, &x, &y);
            } else /* LIBINPUT_EVENT_POINTER_MOTION_ABSOLUTE */ {
                x = libinput_event_pointer_get_absolute_x_transformed (pointer,
                                                    grx_get_screen_width ());
                y = libinput_event_pointer_get_absolute_y_transformed (pointer,
                                                    grx_get_screen_height ());
            }

            grx_event.type = GRX_EVENT_TYPE_POINTER_MOTION;
            grx_event.motion.x = x;
            grx_event.motion.y = y;
            grx_event.motion.device = GRX_DEVICE (grx_device);

            MOUINFO->xpos = grx_event.motion.x;
            MOUINFO->ypos = grx_event.motion.y;
            _grx_mouse_update_cursor();
        }
        break;
    case LIBINPUT_EVENT_POINTER_BUTTON:
        {
            struct libinput_event_pointer *pointer;
            uint32_t button, time;

            pointer = libinput_event_get_pointer_event (event);
            button = libinput_event_pointer_get_button (pointer);
            switch (libinput_event_pointer_get_button_state (pointer)) {
            case LIBINPUT_BUTTON_STATE_PRESSED:
                grx_event.type = GRX_EVENT_TYPE_BUTTON_PRESS;
                // double press detection
                time = libinput_event_pointer_get_time (pointer);
                if (button == ((GrxLibinputDeviceManagerSource *)source)->last_button &&
                    (time - ((GrxLibinputDeviceManagerSource *)source)->last_button_time) < DOUBLE_PRESS_TIME)
                {
                    // double press does two callbacks, one here for
                    // GRX_EVENT_BUTTON_PRESS and one in the usual way
                    // for GRX_EVENT_BUTTON_DOUBLE_PRESS
                    grx_event_put (&grx_event);
                    grx_event.type = GRX_EVENT_TYPE_BUTTON_DOUBLE_PRESS;
                    ((GrxLibinputDeviceManagerSource *)source)->last_button = 0;
                    ((GrxLibinputDeviceManagerSource *)source)->last_button_time = 0;
                } else {
                    ((GrxLibinputDeviceManagerSource *)source)->last_button = button;
                    ((GrxLibinputDeviceManagerSource *)source)->last_button_time = time;
                }
                break;
            case LIBINPUT_BUTTON_STATE_RELEASED:
                grx_event.type = GRX_EVENT_TYPE_BUTTON_RELEASE;
                break;
            }
            grx_event.button.button = button;
            grx_event.button.device = GRX_DEVICE (grx_device);
        }
        break;
    case LIBINPUT_EVENT_TOUCH_DOWN:
    case LIBINPUT_EVENT_TOUCH_UP:
    case LIBINPUT_EVENT_TOUCH_MOTION:
    case LIBINPUT_EVENT_TOUCH_CANCEL:
        {
            struct libinput_event_touch *touch;
            int32_t slot;
            double x = 0.0;
            double y = 0.0;

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
                grx_event.type = GRX_EVENT_TYPE_TOUCH_DOWN;
                break;
            case LIBINPUT_EVENT_TOUCH_UP:
                grx_event.type = GRX_EVENT_TYPE_TOUCH_UP;
                break;
            case LIBINPUT_EVENT_TOUCH_MOTION:
                grx_event.type = GRX_EVENT_TYPE_TOUCH_MOTION;
                break;
            case LIBINPUT_EVENT_TOUCH_CANCEL:
                grx_event.type = GRX_EVENT_TYPE_TOUCH_CANCEL;
                break;
            }
            grx_event.touch.id = slot;
            grx_event.touch.x = x;
            grx_event.touch.y = y;
            grx_event.touch.device = GRX_DEVICE (grx_device);
        }
        break;
    }

#pragma GCC diagnostic pop

    if (grx_event.type != GRX_EVENT_TYPE_NONE) {
        grx_event_put (&grx_event);
    }

    libinput_event_destroy (event);

    return G_SOURCE_CONTINUE;
}

static void source_finalize (GSource *source)
{
    GrxLibinputDeviceManager *instance =
        ((GrxLibinputDeviceManagerSource *)source)->instance;

    g_list_free_full (instance->devices, (GDestroyNotify)g_object_unref);

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
    GSource *source;

    source = g_source_new (&source_funcs, sizeof(GrxLibinputDeviceManagerSource));
    ((GrxLibinputDeviceManagerSource *)source)->instance = g_object_ref(G_OBJECT (context));
    g_source_add_unix_fd (source, libinput_get_fd (context->libinput), G_IO_IN);

    return source;
}

guint grx_libinput_device_manager_event_add (GrxLibinputDeviceManager *context)
{
    GSource *source;
    guint id;

    source = grx_libinput_device_manager_source_new (context);
    id = g_source_attach (source, g_main_context_default ());
    g_source_unref (source);

    return id;
}
