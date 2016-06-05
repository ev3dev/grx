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
/**
 * GrxLibinputDeviceClass:
 * @parent_class: the inherited struct
 * @reserved: for future use
 *
 * The type class struct for #GrxLibinputDevice.
 */
typedef struct {
    GrxDeviceClass parent_class;
    gpointer reserved[6];
} GrxLibinputDeviceClass;

/**
 * GrxLibinputDevice:
 *
 * #GrxLibinputDevice is an opaque data structure and can only be
 * accessed using the following functions.
 */
typedef struct {
    /* private */
    GrxDevice parent_instance;
    gpointer private;
} GrxLibinputDevice;

GType grx_libinput_device_get_type (void);

/* internal */

#include <libinput.h>
#include <xkbcommon/xkbcommon.h>

GrxLibinputDevice *grx_libinput_device_new (struct libinput_device *device,
                                            struct xkb_keymap *keymap);
void grx_libinput_device_update_state (GrxLibinputDevice *device,
                                       xkb_keycode_t keycode,
                                       enum xkb_key_direction direction,
                                       xkb_keysym_t *keysym,
                                       gunichar *unichar);

#endif /* __GRX_LIBINPUT_DEVICE_H__ */
