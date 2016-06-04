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

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include <grx/input_event.h>

/**
 * SECTION:gtk3_device_manager
 * @short_description: Device manager for input devices using GTK+ 3
 * @title: GTK+ 3 Device Manager
 * @section_id: gtk3_device_manager
 * @include: grx-3.0.h
 *
 * TODO
 */

#define GRX_TYPE_GTK3_DEVICE_MANAGER grx_gtk3_device_manager_get_type()

#define GRX_GTK3_DEVICE_MANAGER(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GRX_TYPE_GTK3_DEVICE_MANAGER, \
        GrxGtk3DeviceManager))
#define GRX_IS_GTK3_DEVICE_MANAGER(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRX_TYPE_GTK3_DEVICE_MANAGER))
#define GRX_GTK3_DEVICE_MANAGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), GRX_TYPE_GTK3_DEVICE_MANAGER, \
        GrxGtk3DeviceManagerClass))
#define GRX_IS_GTK3_DEVICE_MANAGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), GRX_TYPE_GTK3_DEVICE_MANAGER))
#define GRX_GTK3_DEVICE_MANAGER_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRX_TYPE_GTK3_DEVICE_MANAGER, \
        GrxGtk3DeviceManagerClass))

typedef struct _GrxGtk3DeviceManager GrxGtk3DeviceManager;
typedef struct _GrxGtk3DeviceManagerClass GrxGtk3DeviceManagerClass;

/**
 * GrxGtk3DeviceManager:
 *
 * #GrxGtk3DeviceManager is an opaque data structure and can only be
 * accessed using the following functions.
 */
struct _GrxGtk3DeviceManager
{
    /* private */
    GObject parent_instance;
    gpointer private;
};

/**
 * GrxGtk3DeviceManagerClass:
 * @parent_class: the parent class struct
 * @reserved: for future use
 *
 * The class type struct for #GrxGtk3DeviceManager.
 */
struct _GrxGtk3DeviceManagerClass
{
    GObjectClass parent_class;
    gpointer reserved[6];
};

/**
 * GrxGtk3DeviceManagerSource:
 *
 * The #GrxGtk3DeviceManagerSource struct is an opaque data type representing a
 * #GrxGtk3DeviceManager event source.
 */
typedef struct {
    /*<private>*/
    GSource source;
    GrxGtk3DeviceManager *instance;
    guint last_button;
    guint last_button_time;
} GrxGtk3DeviceManagerSource;

/**
 * GrxGtk3DeviceManagerSourceFunc:
 * @event: pointer to the event
 * @user_data: data passed to the function, set when the source was created
 *
 * Specifies the type of function passed to grx_gtk3_device_manager_event_add().
 */
typedef void (*GrxGtk3DeviceManagerSourceFunc)(GrxInputEvent *event,
                                             gpointer user_data);

GType grx_gtk3_device_manager_get_type (void);
GrxGtk3DeviceManager *
grx_gtk3_device_manager_new (GCancellable *cancellable, GError **error);
GSource *grx_gtk3_device_manager_source_new (GrxGtk3DeviceManager *context);
guint grx_gtk3_device_manager_event_add (GrxGtk3DeviceManager *context,
                                     GrxGtk3DeviceManagerSourceFunc func,
                                     gpointer user_data, GDestroyNotify notify);

#endif /* __GRX_GTK3_DEVICE_MANAGER_H__ */
