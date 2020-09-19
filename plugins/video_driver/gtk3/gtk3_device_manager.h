/*
 * gtk3_device_manager.h
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

#ifndef __GRX_GTK3_DEVICE_MANAGER_H__
#define __GRX_GTK3_DEVICE_MANAGER_H__

#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>

#include <grx/device_manager.h>
#include <grx/events.h>

#define GRX_TYPE_GTK3_DEVICE_MANAGER grx_gtk3_device_manager_get_type()
G_DECLARE_FINAL_TYPE(GrxGtk3DeviceManager, grx_gtk3_device_manager, GRX,
    GTK3_DEVICE_MANAGER, GrxDeviceManager)

#endif /* __GRX_GTK3_DEVICE_MANAGER_H__ */
