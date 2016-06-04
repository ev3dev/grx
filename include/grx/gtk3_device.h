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

/**
 * SECTION:gtk3_device
 * @short_description: Object that represents individual input devices
 * @title: Input Devices
 * @section_id: gtk3_device
 * @include: grx-3.0.h
 *
 * TODO
 */

#define GRX_TYPE_GTK3_DEVICE grx_gtk3_device_get_type()

#define GRX_GTK3_DEVICE(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GRX_TYPE_GTK3_DEVICE, \
        GrxGtk3Device))
#define GRX_IS_GTK3_DEVICE(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRX_TYPE_GTK3_DEVICE))
#define GRX_GTK3_DEVICE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), GRX_TYPE_GTK3_DEVICE, \
        GrxGtk3DeviceClass))
#define GRX_IS_GTK3_DEVICE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), GRX_TYPE_GTK3_DEVICE))
#define GRX_GTK3_DEVICE_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRX_TYPE_GTK3_DEVICE, \
        GrxGtk3DeviceClass))
/**
 * GrxGtk3DeviceClass:
 * @parent_class: the inherited struct
 * @reserved: for future use
 *
 * The type class struct for #GrxGtk3Device.
 */
typedef struct {
    GObjectClass parent_class;
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
    GObject parent_instance;
    gpointer private;
} GrxGtk3Device;

GType grx_gtk3_device_get_type (void);
const gchar *grx_gtk3_device_get_name (GrxGtk3Device *device);
const gchar *grx_gtk3_device_get_sysname (GrxGtk3Device *device);
gboolean grx_gtk3_device_get_has_keyboard (GrxGtk3Device *device);
gboolean grx_gtk3_device_get_has_pointer (GrxGtk3Device *device);
gboolean grx_gtk3_device_get_has_touch (GrxGtk3Device *device);
gboolean grx_gtk3_device_uncalibrate (GrxGtk3Device *device);

#endif /* __GRX_GTK3_DEVICE_H__ */
