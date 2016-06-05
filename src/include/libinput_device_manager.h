/*
 * libinput_device_manager.h
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

#ifndef __GRX_LIBINPUT_DEVICE_MANAGER_H__
#define __GRX_LIBINPUT_DEVICE_MANAGER_H__

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include <grx/device_manager.h>
#include <grx/input_event.h>


#define GRX_TYPE_LIBINPUT_DEVICE_MANAGER grx_libinput_device_manager_get_type()

#define GRX_LIBINPUT_DEVICE_MANAGER(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GRX_TYPE_LIBINPUT_DEVICE_MANAGER, \
        GrxLibinputDeviceManager))
#define GRX_IS_LIBINPUT_DEVICE_MANAGER(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRX_TYPE_LIBINPUT_DEVICE_MANAGER))
#define GRX_LIBINPUT_DEVICE_MANAGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), GRX_TYPE_LIBINPUT_DEVICE_MANAGER, \
        GrxLibinputDeviceManagerClass))
#define GRX_IS_LIBINPUT_DEVICE_MANAGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), GRX_TYPE_LIBINPUT_DEVICE_MANAGER))
#define GRX_LIBINPUT_DEVICE_MANAGER_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRX_TYPE_LIBINPUT_DEVICE_MANAGER, \
        GrxLibinputDeviceManagerClass))

typedef struct _GrxLibinputDeviceManager GrxLibinputDeviceManager;
typedef struct _GrxLibinputDeviceManagerClass GrxLibinputDeviceManagerClass;

/**
 * GrxLibinputDeviceManager:
 *
 * #GrxLibinputDeviceManager is an opaque data structure and can only be
 * accessed using the following functions.
 */
struct _GrxLibinputDeviceManager
{
    /* private */
    GrxDeviceManager parent_instance;
    gpointer private;
};

/**
 * GrxLibinputDeviceManagerClass:
 * @parent_class: the parent class struct
 * @reserved: for future use
 *
 * The class type struct for #GrxLibinputDeviceManager.
 */
struct _GrxLibinputDeviceManagerClass
{
    GrxDeviceManagerClass parent_class;
    gpointer reserved[6];
};

GType grx_libinput_device_manager_get_type (void);



/**
 * GrxLibinputDeviceManagerSource:
 *
 * The #GrxLibinputDeviceManagerSource struct is an opaque data type representing a
 * #GrxLibinputDeviceManager event source.
 */
typedef struct {
    /*<private>*/
    GSource source;
    GrxLibinputDeviceManager *instance;
    guint last_button;
    guint last_button_time;
} GrxLibinputDeviceManagerSource;

/**
 * GrxLibinputDeviceManagerSourceFunc:
 * @event: pointer to the event
 * @user_data: data passed to the function, set when the source was created
 *
 * Specifies the type of function passed to grx_libinput_device_manager_event_add().
 */
typedef void (*GrxLibinputDeviceManagerSourceFunc)(GrxInputEvent *event,
                                             gpointer user_data);

GSource *grx_libinput_device_manager_source_new (GrxLibinputDeviceManager *context);
guint grx_libinput_device_manager_event_add (GrxLibinputDeviceManager *context,
                                     GrxLibinputDeviceManagerSourceFunc func,
                                     gpointer user_data, GDestroyNotify notify);

#endif /* __GRX_LIBINPUT_DEVICE_MANAGER_H__ */
