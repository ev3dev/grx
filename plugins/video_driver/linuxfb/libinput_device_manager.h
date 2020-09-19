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

#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>

#include <grx/device_manager.h>
#include <grx/events.h>

#define GRX_TYPE_LIBINPUT_DEVICE_MANAGER grx_libinput_device_manager_get_type()
G_DECLARE_FINAL_TYPE(GrxLibinputDeviceManager, grx_libinput_device_manager, GRX,
    LIBINPUT_DEVICE_MANAGER, GrxDeviceManager)

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

GSource *grx_libinput_device_manager_source_new(GrxLibinputDeviceManager *context);
guint grx_libinput_device_manager_event_add(GrxLibinputDeviceManager *context);

#endif /* __GRX_LIBINPUT_DEVICE_MANAGER_H__ */
