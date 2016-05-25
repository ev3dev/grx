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

#include <grx/libinput_device.h>

GrxLibinputDevice *grx_libinput_device_new (struct libinput_device *device);

#endif /* __INPUT_LIBINPUT_DEVICE_INTERNAL_H__ */
