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

#ifndef __INPUT_LIBINPUT_DEVICE_INTERNAL_H__
#define __INPUT_LIBINPUT_DEVICE_INTERNAL_H__

#include <libinput.h>
#include <xkbcommon/xkbcommon.h>

#include <grx/libinput_device.h>

GrxLibinputDevice *grx_libinput_device_new (struct libinput_device *device,
                                            struct xkb_keymap *keymap);
void grx_libinput_device_update_state (GrxLibinputDevice *device,
                                       xkb_keycode_t keycode,
                                       enum xkb_key_direction direction,
                                       xkb_keysym_t *keysym,
                                       gunichar *unichar);

#endif /* __INPUT_LIBINPUT_DEVICE_INTERNAL_H__ */
