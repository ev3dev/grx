/*
 * device.c
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

#include <glib.h>
#include <glib-object.h>

#include <grx/device.h>

G_DEFINE_ABSTRACT_TYPE (GrxDevice, grx_device, G_TYPE_OBJECT)

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

static GParamSpec *properties[N_PROPERTIES] = { NULL };

/**
 * GrxDevice:name:
 *
 * A descriptive name of the device, suitable for displaying to the user.
 */

/**
 * grx_device_get_name:
 * @device: the device
 *
 * Gets the #GrxDevice:name of the device.
 *
 * Returns: (transfer full) (nullable): the name or %NULL
 */
const gchar *grx_device_get_name (GrxDevice *device)
{
    gchar *name;

    g_object_get (device, "name", &name, NULL);

    return name;
}

/**
 * GrxDevice:sysname:
 *
 * The sysfs name of the device.
 */

/**
 * grx_device_get_sysname:
 * @device: the device
 *
 * Gets the #GrxDevice:sysname of the device.
 *
 * Returns: (transfer full) (nullable): the name or %NULL
 */
const gchar *grx_device_get_sysname (GrxDevice *device)
{
    gchar *sysname;

    g_object_get (device, "sysname", &sysname, NULL);

    return sysname;
}

/**
 * GrxDevice:has-keyboard:
 *
 * Indicates that a device can produce key events.
 */

/**
 * grx_device_get_has_keyboard:
 * @device: the device
 *
 * Gets if the device has keyboard capabilities.
 *
 * Returns: %TRUE if the device can generate key events, otherwise %FALSE.
 */
gboolean grx_device_get_has_keyboard (GrxDevice *device)
{
    gboolean has_keyboard;

    g_object_get (device, "has-keyboard", &has_keyboard, NULL);

    return has_keyboard;
}

/**
 * GrxDevice:has-pointer:
 *
 * Indicates that a device can produce button/motion events.
 */

/**
 * grx_device_get_has_pointer:
 * @device: the device
 *
 * Gets if the device has mouse capabilities.
 *
 * Returns: %TRUE if the device can generate button/motion events, otherwise %FALSE.
 */
gboolean grx_device_get_has_pointer (GrxDevice *device)
{
    gboolean has_pointer;

    g_object_get (device, "has-pointer", &has_pointer, NULL);

    return has_pointer;
}

/**
 * GrxDevice:has-touch:
 *
 * Indicates that a device can produce touch events.
 */

/**
 * grx_device_get_has_touch:
 * @device: the device
 *
 * Gets if the device has touch capabilities.
 *
 * Returns: %TRUE if the device can generate touch events, otherwise %FALSE.
 */
gboolean grx_device_get_has_touch (GrxDevice *device)
{
    gboolean has_touch;

    g_object_get (device, "has-touch", &has_touch, NULL);

    return has_touch;
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
    switch (property_id) {
    case PROP_NAME:
        g_value_set_string (value, NULL);
        break;
    case PROP_SYSNAME:
        g_value_set_string (value, NULL);
        break;
    case PROP_HAS_KEYBOARD:
        g_value_set_boolean (value, FALSE);
        break;
    case PROP_HAS_POINTER:
        g_value_set_boolean (value, FALSE);
        break;
    case PROP_HAS_TOUCH:
        g_value_set_boolean (value, FALSE);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

/* class init */

static gboolean reset_calibration (GrxDevice *device)
{
    return FALSE;
}

static void
grx_device_class_init (GrxDeviceClass *klass)
{
    klass->reset_calibration = reset_calibration;

    G_OBJECT_CLASS (klass)->set_property = set_property;
    G_OBJECT_CLASS (klass)->get_property = get_property;

    properties[PROP_NAME] =
        g_param_spec_string ("name",
                             "device name",
                             "Gets a descriptive name of the device",
                             NULL /* default value */,
                             G_PARAM_READABLE);
    properties[PROP_SYSNAME] =
        g_param_spec_string ("sysname",
                             "sysfs device name",
                             "Gets the sysfs name of the device",
                             NULL /* default value */,
                             G_PARAM_READABLE);
    properties[PROP_HAS_KEYBOARD] =
        g_param_spec_boolean ("has-keyboard",
                              "device has keyboard",
                              "Gets if the device has keyboard keys",
                              FALSE /* default value */,
                              G_PARAM_READABLE);
    properties[PROP_HAS_POINTER] =
        g_param_spec_boolean ("has-pointer",
                              "device has pointer",
                              "Gets if the device has a pointing device and/or buttons",
                              FALSE /* default value */,
                              G_PARAM_READABLE);
    properties[PROP_HAS_TOUCH] =
        g_param_spec_boolean ("has-touch",
                              "device has touch",
                              "Gets if the device has a touchscreen",
                              FALSE /* default value */,
                              G_PARAM_READABLE);
    g_object_class_install_properties (G_OBJECT_CLASS (klass),
                                       N_PROPERTIES,
                                       properties);
}

static void grx_device_init (GrxDevice *self)
{
}

/* methods */

/**
 * grx_device_reset_calibration: (virtual reset_calibration)
 * @device: the device
 *
 * Resets the calibration of the device.
 *
 * This function is probably only useful to programs that are calibrating the
 * device and need to remove the existing calibration.
 *
 * Returns: %TRUE if this device can be calibrated and reseting the calibration
 * was successful, otherwise %FALSE.
 */
gboolean grx_device_reset_calibration (GrxDevice *device)
{
    GrxDeviceClass *klass;

    g_return_val_if_fail (device != NULL, FALSE);
    klass = GRX_DEVICE_GET_CLASS (device);

    return klass->reset_calibration (device);
}
