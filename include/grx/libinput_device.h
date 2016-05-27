/*
 * libinput_device.h
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

#ifndef __GRX_LIBINPUT_DEVICE_H__
#define __GRX_LIBINPUT_DEVICE_H__

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

/**
 * SECTION:libinput_device
 * @short_description: Keyboards, mice, touchscreens, etc.
 * @title: Input Device
 * @section_id: libinput_device
 * @include: grx-3.0.h
 *
 * TODO
 */

#define GRX_TYPE_LIBINPUT_DEVICE grx_libinput_device_get_type()

#define GRX_LIBINPUT_DEVICE(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GRX_TYPE_LIBINPUT_DEVICE, \
        GrxLibinputDevice))
#define GRX_IS_LIBINPUT_DEVICE(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRX_TYPE_LIBINPUT_DEVICE))
#define GRX_LIBINPUT_DEVICE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), GRX_TYPE_LIBINPUT_DEVICE, \
        GrxLibinputDeviceClass))
#define GRX_IS_LIBINPUT_DEVICE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), GRX_TYPE_LIBINPUT_DEVICE))
#define GRX_LIBINPUT_DEVICE_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRX_TYPE_LIBINPUT_DEVICE, \
        GrxLibinputDeviceClass))

typedef struct {
    /*< private >*/
    GObjectClass parent_class;
    gpointer reserved[6];
} GrxLibinputDeviceClass;

typedef struct {
    /* private */
    GObject parent_instance;
    gpointer private;
} GrxLibinputDevice;

GType grx_libinput_device_get_type (void);
const gchar *grx_libinput_device_get_name (GrxLibinputDevice *device);
const gchar *grx_libinput_device_get_sysname (GrxLibinputDevice *device);
gboolean grx_libinput_device_get_has_keyboard (GrxLibinputDevice *device);
gboolean grx_libinput_device_get_has_pointer (GrxLibinputDevice *device);
gboolean grx_libinput_device_get_has_touch (GrxLibinputDevice *device);
gboolean grx_libinput_device_uncalibrate (GrxLibinputDevice *device);

#endif /* __GRX_LIBINPUT_DEVICE_H__ */
