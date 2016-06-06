/*
 * application.c - GRX Application
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
#include <gio/gio.h>

#include <grx/context.h>
#include <grx/draw.h>
#include <grx/extents.h>
#include <grx/events.h>
#include <grx/input_keysyms.h>
#include <grx/application.h>
#include <grx/mode.h>

/**
 * SECTION:application
 * @title: GrxApplication
 * @short_description: Linux console application class
 * @include: grx-3.0.h
 *
 * #GrxApplication extends #GApplication for applications using the Grx graphics
 * library.
 *
 * If there is a chance that your application will be run using the "linuxfb"
 * video driver, you need to monitor the "is-active" property. If you draw on
 * the screen when "is-active" %FALSE, it interfere with the application on the
 * active virtual terminal when switching consoles (e.g. ALT+CTRL+F1).
 */

typedef struct {
    gboolean active;
} GrxApplicationPrivate;

static void initable_interface_init (GInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE (GrxApplication,
    grx_application, G_TYPE_APPLICATION,
    G_ADD_PRIVATE (GrxApplication)
    G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, initable_interface_init))

/* Properties */

enum {
    PROP_0,
    PROP_IS_ACTIVE,
    N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES] = { NULL };

/**
 * grx_application_is_active:
 * @application: a #GrxApplication
 *
 * Gets the status of the #GrxApplication:is-active property
 *
 * Returns: %TRUE if the application is active
 */
gboolean
grx_application_is_active (GrxApplication *application)
{
    GrxApplicationPrivate *priv =
        grx_application_get_instance_private (application);

    return priv->active;
}

static void
set_property (GObject *object, guint property_id, const GValue *value,
              GParamSpec *pspec)
{
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
    GrxApplication *self = GRX_APPLICATION (object);
    GrxApplicationPrivate *priv =
        grx_application_get_instance_private (self);

    switch (property_id) {
    case PROP_IS_ACTIVE:
        g_value_set_boolean (value, priv->active);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

/* class implementation */

static void startup (GApplication *application)
{
    G_APPLICATION_CLASS (grx_application_parent_class)->
        startup (application);

    /* We aren't using the hold count feature, so this is never released */
    g_application_hold (application);
}

static void
input_event (GrxApplication *application, GrxEvent *event)
{
}

static void
grx_application_class_init (GrxApplicationClass *klass)
{
    G_OBJECT_CLASS (klass)->set_property = set_property;
    G_OBJECT_CLASS (klass)->get_property = get_property;

    properties[PROP_IS_ACTIVE] =
        g_param_spec_boolean ("is-active",
                              "application is active",
                              "Gets if the application is active.",
                              FALSE /* default value */,
                              G_PARAM_READABLE);
    g_object_class_install_properties (G_OBJECT_CLASS (klass),
                                       N_PROPERTIES,
                                       properties);

    G_APPLICATION_CLASS (klass)->startup = startup;

    klass->input_event = input_event;
}

static void
grx_application_init (GrxApplication *self)
{
}

/* interface implementation */

static void event_handler (GrxEvent *event, gpointer user_data)
{
    GrxApplication *self = user_data;
    GrxApplicationPrivate *priv =
        grx_application_get_instance_private (self);

    // Handle application events here. Other events are passed on only if the
    // application "is-active"
    switch (event->type) {
    case GRX_EVENT_TYPE_APP_ACTIVATE:
        if (!priv->active) {
            priv->active = TRUE;
            g_object_notify_by_pspec (G_OBJECT (self),
                                      properties[PROP_IS_ACTIVE]);
        }
        break;
    case GRX_EVENT_TYPE_APP_DEACTIVATE:
        if (priv->active) {
            priv->active = FALSE;
            g_object_notify_by_pspec (G_OBJECT (self),
                                      properties[PROP_IS_ACTIVE]);
        }
        break;
    case GRX_EVENT_TYPE_APP_QUIT:
        g_application_quit (G_APPLICATION (self));
        break;
    default:
        if (priv->active) {
            GRX_APPLICATION_GET_CLASS (self)->input_event (self, event);
        }
        break;
    }
}

static gboolean init (GInitable *initable, GCancellable *cancellable,
                      GError **error)
{
    GrxApplication *self = GRX_APPLICATION (initable);

    grx_set_error_handling (FALSE);
    if (!grx_set_mode_default_graphics (TRUE)) {
        g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED,
            "Could not set graphics mode.");
        return FALSE;
    }

    grx_events_add (event_handler, g_object_ref (self), g_object_unref);

    return g_application_register (G_APPLICATION (initable), cancellable, error);
}

static void initable_interface_init (GInitableIface *iface)
{
    iface->init = init;
}

/* constructors */

/**
 * grx_application_new:
 * @cancellable: (nullable): a #GCancellable, or %NULL
 * @error: a pointer to a NULL #GError, or %NULL
 *
 * Create a new instance of #GrxApplication. The console will be
 * set to graphics mode or there will be an @error if setting graphics mode
 * failed.
 *
 * Returns: (nullable): New instance of #GrxApplication or %NULL
 * if init() failed.
 */
GrxApplication *
grx_application_new (GCancellable *cancellable, GError **error)
{
    return g_initable_new (GRX_TYPE_APPLICATION, cancellable, error, NULL);
}
