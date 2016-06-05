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

#include <grx/input_event.h>

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

#define GRX_DEVICE_MANAGER(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GRX_TYPE_DEVICE_MANAGER, \
        GrxDeviceManager))
#define GRX_IS_DEVICE_MANAGER(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRX_TYPE_DEVICE_MANAGER))
#define GRX_DEVICE_MANAGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), GRX_TYPE_DEVICE_MANAGER, \
        GrxDeviceManagerClass))
#define GRX_IS_DEVICE_MANAGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), GRX_TYPE_DEVICE_MANAGER))
#define GRX_DEVICE_MANAGER_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRX_TYPE_DEVICE_MANAGER, \
        GrxDeviceManagerClass))

typedef struct _GrxDeviceManager GrxDeviceManager;
typedef struct _GrxDeviceManagerClass GrxDeviceManagerClass;

/**
 * GrxDeviceManager:
 *
 * #GrxDeviceManager is an opaque data structure and can only be
 * accessed using the following functions.
 */
struct _GrxDeviceManager
{
    /* private */
    GObject parent_instance;
    gpointer private;
    gpointer reserved[6];
};

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
