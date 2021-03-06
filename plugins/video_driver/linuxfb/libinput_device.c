/*
 * libinput_device.c
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

#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <libinput.h>
#include <libudev.h>
#include <xkbcommon/xkbcommon.h>

#include <grx/device.h>
#include <grx/events.h>

#include "libinput_device.h"

struct _GrxLibinputDevice {
    GrxDevice parent_instance;
    struct libinput_device *device;
    struct xkb_state *state;
    xkb_mod_index_t shift_idx;
    xkb_mod_index_t ctrl_idx;
    xkb_mod_index_t alt_idx;
    xkb_mod_index_t logo_idx;
};

G_DEFINE_TYPE(GrxLibinputDevice, grx_libinput_device, GRX_TYPE_DEVICE)

/* Properties */

enum {
    PROP_0,
    PROP_NAME,
    PROP_SYSNAME,
    PROP_HAS_KEYBOARD,
    PROP_HAS_POINTER,
    PROP_HAS_TOUCH,
    N_PROPERTIES
};

static void set_property(
    GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id) {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}

static void get_property(
    GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    GrxLibinputDevice *self = GRX_LIBINPUT_DEVICE(object);

    switch (property_id) {
    case PROP_NAME:
        g_value_set_string(value, libinput_device_get_name(self->device));
        break;
    case PROP_SYSNAME:
        g_value_set_string(value, libinput_device_get_sysname(self->device));
        break;
    case PROP_HAS_KEYBOARD:
        g_value_set_boolean(value,
            libinput_device_has_capability(self->device, LIBINPUT_DEVICE_CAP_KEYBOARD));
        break;
    case PROP_HAS_POINTER:
        g_value_set_boolean(value,
            libinput_device_has_capability(self->device, LIBINPUT_DEVICE_CAP_POINTER));
        break;
    case PROP_HAS_TOUCH:
        g_value_set_boolean(value,
            libinput_device_has_capability(self->device, LIBINPUT_DEVICE_CAP_TOUCH));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}

/* class implementation */

static gboolean reset_calibration(GrxDevice *device)
{
    GrxLibinputDevice *self = GRX_LIBINPUT_DEVICE(device);
    float identity_matrix[6] = { 1, 0, 0, 0, 1, 0 };
    enum libinput_config_status ret;

    ret = libinput_device_config_calibration_set_matrix(self->device, identity_matrix);

    return ret == LIBINPUT_CONFIG_STATUS_SUCCESS;
}

static void finalize(GObject *object)
{
    GrxLibinputDevice *self = GRX_LIBINPUT_DEVICE(object);

    G_OBJECT_CLASS(grx_libinput_device_parent_class)->finalize(object);

    xkb_state_unref(self->state);
    libinput_device_set_user_data(self->device, NULL);
    self->device = libinput_device_unref(self->device);
}

static void grx_libinput_device_class_init(GrxLibinputDeviceClass *klass)
{
    GRX_DEVICE_CLASS(klass)->reset_calibration = reset_calibration;

    G_OBJECT_CLASS(klass)->set_property = set_property;
    G_OBJECT_CLASS(klass)->get_property = get_property;
    G_OBJECT_CLASS(klass)->finalize = finalize;

    g_object_class_override_property(G_OBJECT_CLASS(klass), PROP_NAME, "name");
    g_object_class_override_property(G_OBJECT_CLASS(klass), PROP_SYSNAME, "sysname");
    g_object_class_override_property(
        G_OBJECT_CLASS(klass), PROP_HAS_KEYBOARD, "has-keyboard");
    g_object_class_override_property(
        G_OBJECT_CLASS(klass), PROP_HAS_POINTER, "has-pointer");
    g_object_class_override_property(
        G_OBJECT_CLASS(klass), PROP_HAS_TOUCH, "has-touch");
}

static void grx_libinput_device_init(GrxLibinputDevice *self) { }

/* constructors */

/**
 * grx_libinput_device_new:
 * @device: the libinput device
 *
 * Creates a new instance of #GrxLibinputDevice
 *
 * Returns: the new instance
 */
GrxLibinputDevice *grx_libinput_device_new(
    struct libinput_device *device, struct xkb_keymap *keymap)
{
    GrxLibinputDevice *self = g_object_new(GRX_TYPE_LIBINPUT_DEVICE, NULL);
    float matrix[6];

    g_return_val_if_fail(device != NULL, NULL);
    g_return_val_if_fail(libinput_device_get_user_data(device) == NULL, NULL);
    g_return_val_if_fail(keymap != NULL, NULL);

    self->device = libinput_device_ref(device);
    libinput_device_set_user_data(device, self);

    if (libinput_device_config_calibration_get_default_matrix(device, matrix)) {
        libinput_device_config_calibration_set_matrix(device, matrix);
    }

    self->state = xkb_state_new(keymap);

    self->shift_idx = xkb_map_mod_get_index(keymap, XKB_MOD_NAME_SHIFT);
    self->ctrl_idx = xkb_map_mod_get_index(keymap, XKB_MOD_NAME_CTRL);
    self->alt_idx = xkb_map_mod_get_index(keymap, XKB_MOD_NAME_ALT);
    self->logo_idx = xkb_map_mod_get_index(keymap, XKB_MOD_NAME_LOGO);

    return self;
}

/* methods */

/**
 * grx_libinput_device_update_state:
 * @device: the device instance
 * @keycode: the XKB key code
 * @direction: the key press direction
 * @keysym: (out): the resulting keysym
 * @unichar: the resulting UTF-32 character
 *
 * Updates the state of a device when a key event occurs.
 */
void grx_libinput_device_update_state(GrxLibinputDevice *device, xkb_keycode_t keycode,
    enum xkb_key_direction direction, xkb_keysym_t *keysym, gunichar *unichar)
{
    enum xkb_state_component changed;

    // according to the libxkbcommon docs, it is conventional to get the symbol
    // and then update (in case you were wondering about the ordering here)
    *keysym = xkb_state_key_get_one_sym(device->state, keycode);
    *unichar = xkb_state_key_get_utf32(device->state, keycode);
    changed = xkb_state_update_key(device->state, keycode, direction);

    if (changed & XKB_STATE_LEDS) {
        enum libinput_led leds = 0;

        if (xkb_state_led_name_is_active(device->state, XKB_LED_NAME_CAPS) == 1) {
            leds |= LIBINPUT_LED_CAPS_LOCK;
        }
        if (xkb_state_led_name_is_active(device->state, XKB_LED_NAME_NUM) == 1) {
            leds |= LIBINPUT_LED_NUM_LOCK;
        }
        if (xkb_state_led_name_is_active(device->state, XKB_LED_NAME_SCROLL) == 1) {
            leds |= LIBINPUT_LED_SCROLL_LOCK;
        }
        libinput_device_led_update(device->device, leds);
    }
}

GrxModifierFlags grx_libinput_device_get_modifier_flags(GrxLibinputDevice *device)
{
    GrxModifierFlags flags = 0;

    g_return_val_if_fail(device != NULL, flags);

    if (xkb_state_mod_index_is_active(
            device->state, device->shift_idx, XKB_STATE_MODS_EFFECTIVE)
        == 1) {
        flags |= GRX_MODIFIER_SHIFT;
    }
    if (xkb_state_mod_index_is_active(
            device->state, device->ctrl_idx, XKB_STATE_MODS_EFFECTIVE)
        == 1) {
        flags |= GRX_MODIFIER_CTRL;
    }
    if (xkb_state_mod_index_is_active(
            device->state, device->alt_idx, XKB_STATE_MODS_EFFECTIVE)
        == 1) {
        flags |= GRX_MODIFIER_ALT;
    }
    if (xkb_state_mod_index_is_active(
            device->state, device->logo_idx, XKB_STATE_MODS_EFFECTIVE)
        == 1) {
        flags |= GRX_MODIFIER_SUPER;
    }

    return flags;
}
