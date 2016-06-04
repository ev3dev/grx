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

#ifndef __INPUT_GTK3_DEVICE_INTERNAL_H__
#define __INPUT_GTK3_DEVICE_INTERNAL_H__

#include <gtk/gtk.h>

#include <grx/gtk3_device.h>
#include <grx/gtk3_device_manager.h>

GrxGtk3Device *grx_gtk3_device_new (GdkDevice *device);
GrxGtk3Device *
grx_gtk3_device_lookup (GrxGtk3DeviceManager *manager, GdkDevice *device);

#endif /* __INPUT_GTK3_DEVICE_INTERNAL_H__ */
