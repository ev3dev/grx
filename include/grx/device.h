/*
 * device.h
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

#ifndef __GRX_DEVICE_H__
#define __GRX_DEVICE_H__

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

/**
 * SECTION:device
 * @short_description: Object that represents individual input devices
 * @title: Input Devices
 * @section_id: device
 * @include: grx-3.0.h
 *
 * #GrxDevice is an abstract class the represents an input device such as a
 * keyboard, mouse or touchscreen. Devices are normally handled internally
 * in the library. So, this class is normally not used unless you need to
 * differentiate between input events between individual devices (e.g. a
 * a calibration program where you only want input from the device you are
 * calibrating).
 */

/**
 * GrxDevice:
 *
 * Base class for objects that represents in input device.
 */
#define GRX_TYPE_DEVICE grx_device_get_type()
G_DECLARE_DERIVABLE_TYPE(GrxDevice, grx_device, GRX, DEVICE, GObject)

/**
 * GrxDeviceClass:
 * @parent_class: the inherited struct
 * @reset_calibration: Overriding classes should implement this method if possible.
 *                     The default implementation simply returns %FALSE.
 * @reserved: for future use
 *
 * The type class struct for #GrxDevice.
 */
struct _GrxDeviceClass {
    GObjectClass parent_class;
    gboolean (*reset_calibration) (GrxDevice *device);
    gpointer reserved[6];
};

const gchar *grx_device_get_name (GrxDevice *device);
const gchar *grx_device_get_sysname (GrxDevice *device);
gboolean grx_device_get_has_keyboard (GrxDevice *device);
gboolean grx_device_get_has_pointer (GrxDevice *device);
gboolean grx_device_get_has_touch (GrxDevice *device);
gboolean grx_device_reset_calibration (GrxDevice *device);

#endif /* __GRX_DEVICE_H__ */
