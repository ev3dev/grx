/*
 * device_manager.c
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

#include <glib.h>
#include <glib-object.h>

#include <grx/device_manager.h>


G_DEFINE_ABSTRACT_TYPE (GrxDeviceManager, grx_device_manager, G_TYPE_OBJECT)

/* signals */

enum {
    SIG_0,
    SIG_DEVICE_ADDED,
    SIG_DEVICE_REMOVED,
    N_SIGNALS
};

static uint signals[N_SIGNALS] = { 0 };

/* class implementation */

static void
grx_device_manager_class_init (GrxDeviceManagerClass *klass)
{
    signals[SIG_DEVICE_ADDED] =
        g_signal_new ("device-added",
                      G_TYPE_FROM_CLASS (klass),
                      G_SIGNAL_RUN_LAST,
                      0, /* class offset */
                      NULL, /* accumulator */
                      NULL, /* accumulator data */
                      NULL, /* C marshaller */
                      G_TYPE_NONE, /* return type */
                      1, /* n_params */
                      GRX_TYPE_DEVICE /* types */);
    signals[SIG_DEVICE_REMOVED] =
        g_signal_new ("device-removed",
                      G_TYPE_FROM_CLASS (klass),
                      G_SIGNAL_RUN_LAST,
                      0, /* class offset */
                      NULL, /* accumulator */
                      NULL, /* accumulator data */
                      NULL, /* C marshaller */
                      G_TYPE_NONE, /* return type */
                      1, /* n_params */
                      GRX_TYPE_DEVICE /* types */);
}

static void
grx_device_manager_init (GrxDeviceManager *self)
{
}
