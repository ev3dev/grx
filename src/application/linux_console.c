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

#include <grx/context.h>
#include <grx/draw.h>
#include <grx/linux_console_application.h>
#include <grx/mode.h>

#include "libgrx.h"

/**
 * SECTION:linux_console_application
 * @title: GrxLinuxConsoleApplication
 * @short_description: Linux console application class
 * @include: grx-3.0.h
 *
 * #GrxLinuxConsoleApplication extends #GApplication for Linux console
 * applications using the Grx graphics library.
 *
 * #GrxLinuxConsoleApplication implements console switching, so programs that
 * use #GrxLinuxConsoleApplication must use the
 * #GrxLinuxConsoleApplication:is-console-active property to tell if the console
 * is active. This property will be false if the user switches to another
 * console. If you draw to the screen during this time, it will interfere with
 * the application running on the active console.
 */

/**
 * GrxLinuxConsoleApplication:
 *
 * #GrxLinuxConsoleApplication is an opaque data structure and can only be
 * accessed using the following functions.
 */

/**
 * GrxLinuxConsoleApplicationClass:
 *
 * Nothing here yet.
 */

typedef struct {
    gboolean owns_fb;
    GrxContext *save;
} GrxLinuxConsoleApplicationPrivate;

static void initable_interface_init (GInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE (GrxLinuxConsoleApplication,
    grx_linux_console_application, G_TYPE_APPLICATION,
    G_ADD_PRIVATE (GrxLinuxConsoleApplication)
    G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, initable_interface_init))

/* Properties */

enum {
    PROP_0,
    PROP_IS_CONSOLE_ACTIVE,
    N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES] = { NULL };

/**
 * grx_linux_console_application_is_console_active:
 * @application: a #GrxLinuxConsoleApplication
 *
 * Gets the status of the #GrxLinuxConsoleApplication:is-console-active property
 *
 * Returns: %TRUE if the console this application is running on is the active
 * console.
 */
gboolean
grx_linux_console_application_is_console_active (GrxLinuxConsoleApplication *application)
{
    GrxLinuxConsoleApplicationPrivate *priv =
        grx_linux_console_application_get_instance_private (application);

    return priv->owns_fb;
}

static void
set_property (GObject *object, guint property_id, const GValue *value,
              GParamSpec *pspec)
{
    GrxLinuxConsoleApplication *self = GRX_LINUX_CONSOLE_APPLICATION (object);

    switch (property_id) {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
get_property (GObject *object, guint property_id, GValue *value,
              GParamSpec *pspec)
{
    GrxLinuxConsoleApplication *self = GRX_LINUX_CONSOLE_APPLICATION (object);
    GrxLinuxConsoleApplicationPrivate *priv =
        grx_linux_console_application_get_instance_private (self);

    switch (property_id) {
    case PROP_IS_CONSOLE_ACTIVE:
        g_value_set_boolean (value, priv->owns_fb);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

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
    G_OBJECT_CLASS (klass)->set_property = set_property;
    G_OBJECT_CLASS (klass)->get_property = get_property;

    properties[PROP_IS_CONSOLE_ACTIVE] =
        g_param_spec_boolean ("is-console-active",
                              "console is active",
                              "Gets if the console this application is running on is the active console.",
                              FALSE /* default value */,
                              G_PARAM_READABLE);
    g_object_class_install_properties (G_OBJECT_CLASS (klass),
                                       N_PROPERTIES,
                                       properties);

    G_APPLICATION_CLASS (klass)->startup = startup;
}

static void
grx_linux_console_application_init (GrxLinuxConsoleApplication *self)
{
}

/* interface implementation */

// TODO: put these in a header file
extern void grx_linuxfb_aquire (void);
extern void grx_linuxfb_release (void);

static gboolean console_switch_handler (gpointer user_data)
{
    GrxLinuxConsoleApplication *self = user_data;
    GrxLinuxConsoleApplicationPrivate *priv =
        grx_linux_console_application_get_instance_private (self);

    if (priv->owns_fb) {
        /* a well-behaved user program must listen for this property and stop
         * drawing on the screen when it is FALSE */
        priv->owns_fb = FALSE;
        g_object_notify_by_pspec (G_OBJECT (self), properties[PROP_IS_CONSOLE_ACTIVE]);

        /* create a new context from the screen */
        priv->save = grx_context_create(grx_get_screen_x(), grx_get_screen_y(), NULL, NULL);
        if (priv->save == NULL) {
            g_critical ("Could not allocate context for console switching.");
        } else {
            /* copy framebuffer to new context */
            if (grx_get_screen_frame_mode() == GRX_FRAME_MODE_LFB_MONO01) {
                /* Need to invert the colors on this one. */
                grx_context_clear(priv->save, 1);
                grx_bit_blt(priv->save, 0, 0, grx_context_get_screen(), 0, 0,
                    grx_get_screen_x()-1, grx_get_screen_y()-1, GRX_COLOR_MODE_XOR);
            } else {
                grx_bit_blt(priv->save, 0, 0, grx_context_get_screen(), 0, 0,
                    grx_get_screen_x()-1, grx_get_screen_y()-1, GRX_COLOR_MODE_WRITE);
            }
        }
        grx_linuxfb_release ();
    } else {
        grx_linuxfb_aquire ();

        /* copy the temporary context back to the framebuffer */
        if (grx_get_screen_frame_mode() == GRX_FRAME_MODE_LFB_MONO01) {
            /* need to invert the colors on this one */
            grx_clear_screen(1);
            grx_bit_blt(grx_context_get_screen(), 0, 0, priv->save, 0, 0,
                     grx_get_screen_x()-1, grx_get_screen_y()-1, GRX_COLOR_MODE_XOR);
        } else {
            grx_bit_blt(grx_context_get_screen(), 0, 0, priv->save, 0, 0,
                     grx_get_screen_x()-1, grx_get_screen_y()-1, GRX_COLOR_MODE_WRITE);
        }
        grx_context_unref(priv->save);

        /* now it is OK for user program to start writing to the screen again */
        priv->owns_fb = TRUE;
        g_object_notify_by_pspec (G_OBJECT (self), properties[PROP_IS_CONSOLE_ACTIVE]);
    }

    return G_SOURCE_CONTINUE;
}

static gboolean init (GInitable *initable, GCancellable *cancellable,
                      GError **error)
{
    GrxLinuxConsoleApplication *self = GRX_LINUX_CONSOLE_APPLICATION (initable);
    GrxLinuxConsoleApplicationPrivate *priv =
        grx_linux_console_application_get_instance_private (self);
    gboolean ret;

    grx_set_error_handling (FALSE);
    ret = grx_set_mode_default_graphics (TRUE);
    if (!ret) {
        g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED,
            "Could not set graphics mode.");
        return ret;
    }

    priv->owns_fb = TRUE;
    g_unix_signal_add (SIGUSR1, console_switch_handler, initable);
    g_object_notify_by_pspec (G_OBJECT (self), properties[PROP_IS_CONSOLE_ACTIVE]);

    return g_application_register (G_APPLICATION (initable), cancellable, error);
}

static void initable_interface_init (GInitableIface *iface)
{
    iface->init = init;
}

/* constructors */

/**
 * grx_linux_console_application_new:
 * @cancellable: (allow-none): a #GCancellable, or %NULL
 * @error: a pointer to a NULL #GError, or %NULL
 *
 * Create a new instance of #GrxLinuxConsoleApplication. The console will be
 * set to graphics mode or there will be an @error if setting graphics mode
 * failed.
 *
 * Returns: (allow-none): New instance of #GrxLinuxConsoleApplication or %NULL
 * if init() failed.
 */
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
