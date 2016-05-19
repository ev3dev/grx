/*
 * linux_console.c - Linux Console Application
 *
 * Copyright (c) 2016 David Lechner <david@lechnology.com>
 *
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
#include <glib-unix.h>
#include <grx/linux_console_application.h>

#include "libgrx.h"

static void initable_interface_init (GInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE (GrxLinuxConsoleApplication,
    grx_linux_console_application, G_TYPE_APPLICATION,
    G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, initable_interface_init))

/* class implementation */

static gboolean term_signal_handler (gpointer user_data)
{
    GrxLinuxConsoleApplication *self = user_data;

    g_application_quit (&self->parent_instance);

    return G_SOURCE_REMOVE;
}

static void startup (GApplication *application)
{
    GrxLinuxConsoleApplication *self;

    G_APPLICATION_CLASS (grx_linux_console_application_parent_class)->
        startup (application);
    self = GRX_LINUX_CONSOLE_APPLICATION (application);

    /* We aren't using the hold count feature, so this is never released */
    g_application_hold (application);

    /* gracefully handle application termination via signal */
    g_unix_signal_add (SIGHUP, term_signal_handler, self);
    g_unix_signal_add (SIGINT, term_signal_handler, self);
    g_unix_signal_add (SIGTERM, term_signal_handler, self);
}

static void
grx_linux_console_application_class_init (GrxLinuxConsoleApplicationClass *klass)
{
    G_APPLICATION_CLASS (klass)->startup = startup;
}

static void
grx_linux_console_application_init (GrxLinuxConsoleApplication *self)
{
}

/* interface implementatation */

static gboolean init (GInitable *initable, GCancellable *cancellable,
                      GError **error)
{
    gboolean ret;

    grx_set_error_handling (FALSE);
    ret = grx_set_mode_default_graphics (TRUE);

    if (!ret) {
        g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED,
            "Could not set graphics mode.");
        return ret;
    }

    return g_application_register (G_APPLICATION (initable), cancellable, error);
}

static void initable_interface_init (GInitableIface *iface)
{
    iface->init = init;
}

/* methods */

GrxLinuxConsoleApplication *
grx_linux_console_application_new (GCancellable *cancellable, GError **error)
{
    gboolean ret;

    GrxLinuxConsoleApplication *app =
        g_object_new (GRX_TYPE_LINUX_CONSOLE_APPLICATION, NULL);

    ret = init (G_INITABLE (app), cancellable, error);
    if (!ret) {
        g_object_unref (app);
        return NULL;
    }

    return app;
}
