/*
 * gtk3_device.h
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

#ifndef __GRX_GTK3_DEVICE_H__
#define __GRX_GTK3_DEVICE_H__

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include <grx/device.h>


#define GRX_TYPE_GTK3_DEVICE grx_gtk3_device_get_type()

#define GRX_GTK3_DEVICE(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GRX_TYPE_GTK3_DEVICE, GrxGtk3Device))
#define GRX_IS_GTK3_DEVICE(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRX_TYPE_GTK3_DEVICE))
#define GRX_GTK3_DEVICE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), GRX_TYPE_GTK3_DEVICE, GrxGtk3DeviceClass))
#define GRX_IS_GTK3_DEVICE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), GRX_TYPE_GTK3_DEVICE))
#define GRX_GTK3_DEVICE_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRX_TYPE_GTK3_DEVICE, GrxGtk3DeviceClass))
/**
 * GrxGtk3DeviceClass:
 * @parent_class: the inherited struct
 * @reserved: for future use
 *
 * The type class struct for #GrxGtk3Device.
 */
typedef struct {
    GrxDeviceClass parent_class;
    gpointer reserved[6];
} GrxGtk3DeviceClass;

/**
 * GrxGtk3Device:
 *
 * #GrxGtk3Device is an opaque data structure and can only be
 * accessed using the following functions.
 */
typedef struct {
    /* private */
    GrxDevice parent_instance;
    gpointer private;
} GrxGtk3Device;

GType grx_gtk3_device_get_type (void);

/* internal */

#include "gtk3_device_manager.h"

GrxGtk3Device *grx_gtk3_device_new (GdkDevice *device);
GrxGtk3Device *
grx_gtk3_device_lookup (GrxGtk3DeviceManager *manager, GdkDevice *device);

#endif /* __GRX_GTK3_DEVICE_H__ */
