/*
 * device_manager.h
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

#ifndef __GRX_DEVICE_MANAGER_H__
#define __GRX_DEVICE_MANAGER_H__

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include <grx/common.h>
#include <grx/events.h>

/**
 * SECTION:device_manager
 * @short_description: Device manager for input devices
 * @title: Input Device Manager
 * @section_id: device_manager
 * @include: grx-3.0.h
 *
 * TODO
 */

#define GRX_TYPE_DEVICE_MANAGER grx_device_manager_get_type()
G_DECLARE_DERIVABLE_TYPE(GrxDeviceManager, grx_device_manager, GRX, DEVICE_MANAGER, GObject)

/**
 * GrxDeviceManagerClass:
 * @parent_class: the parent class struct
 * @reserved: for future use
 *
 * The class type struct for #GrxDeviceManager.
 */
struct _GrxDeviceManagerClass
{
    GObjectClass parent_class;
    gpointer reserved[6];
};

GType grx_device_manager_get_type (void);

#endif /* __GRX_DEVICE_MANAGER_H__ */
