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
 * TODO
 */

#define GRX_TYPE_DEVICE grx_device_get_type()

#define GRX_DEVICE(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GRX_TYPE_DEVICE, GrxDevice))
#define GRX_IS_DEVICE(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRX_TYPE_DEVICE))
#define GRX_DEVICE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), GRX_TYPE_DEVICE, GrxDeviceClass))
#define GRX_IS_DEVICE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), GRX_TYPE_DEVICE))
#define GRX_DEVICE_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRX_TYPE_DEVICE, GrxDeviceClass))

/**
 * GrxDevice:
 *
 * #GrxDevice is an opaque data structure and can only be
 * accessed using the following functions.
 */
typedef struct {
    /* private */
    GObject parent_instance;
    gpointer private;
} GrxDevice;

/**
 * GrxDeviceClass:
 * @parent_class: the inherited struct
 * @reserved: for future use
 *
 * The type class struct for #GrxDevice.
 */
typedef struct {
    GObjectClass parent_class;
    gboolean (*uncalibrate) (GrxDevice *device);
    gpointer reserved[6];
} GrxDeviceClass;


GType grx_device_get_type (void);
const gchar *grx_device_get_name (GrxDevice *device);
const gchar *grx_device_get_sysname (GrxDevice *device);
gboolean grx_device_get_has_keyboard (GrxDevice *device);
gboolean grx_device_get_has_pointer (GrxDevice *device);
gboolean grx_device_get_has_touch (GrxDevice *device);
gboolean grx_device_uncalibrate (GrxDevice *device);

#endif /* __GRX_DEVICE_H__ */
