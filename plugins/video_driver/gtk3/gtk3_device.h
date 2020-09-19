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

#include <gdk/gdk.h>
#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>

#include <grx/device.h>

#define GRX_TYPE_GTK3_DEVICE grx_gtk3_device_get_type()
G_DECLARE_FINAL_TYPE(GrxGtk3Device, grx_gtk3_device, GRX, GTK3_DEVICE, GrxDevice)

/* internal */

#include "gtk3_device_manager.h"

GrxGtk3Device *grx_gtk3_device_new(GdkDevice *device);
GrxGtk3Device *grx_gtk3_device_lookup(GrxGtk3DeviceManager *manager, GdkDevice *device);

#endif /* __GRX_GTK3_DEVICE_H__ */
