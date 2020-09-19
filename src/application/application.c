/*
 * application.c - GRX Application
 *
 * Copyright (c) 2016-2017 David Lechner <david@lechnology.com>
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

#include <gio/gio.h>
#include <glib-unix.h>
#include <glib.h>

#include <grx/application.h>
#include <grx/context.h>
#include <grx/draw.h>
#include <grx/events.h>
#include <grx/extents.h>
#include <grx/input_keysyms.h>
#include <grx/mode.h>

#include "marshal.h"

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
    guint event_source_id;
    guint sighup_source_id;
    guint sigint_source_id;
    guint sigterm_source_id;
} GrxApplicationPrivate;

static void initable_interface_init(GInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE(GrxApplication, grx_application, G_TYPE_APPLICATION,
    G_ADD_PRIVATE(GrxApplication)
        G_IMPLEMENT_INTERFACE(G_TYPE_INITABLE, initable_interface_init))

/* Properties */

enum { PROP_0, PROP_IS_ACTIVE, PROP_QUIT_ON_SIGNAL, N_PROPERTIES };

static GParamSpec *properties[N_PROPERTIES] = { NULL };

/**
 * GrxApplication:is-active:
 *
 * Gets if the application is active. Applications should not draw to the screen
 * when this property is %FALSE.
 */

/**
 * grx_application_is_active:
 * @application: a #GrxApplication
 *
 * Gets the status of the #GrxApplication:is-active property
 *
 * Returns: %TRUE if the application is active
 */
gboolean grx_application_is_active(GrxApplication *application)
{
    GrxApplicationPrivate *priv = grx_application_get_instance_private(application);

    return priv->active;
}

/**
 * GrxApplication:quit-on-signal:
 *
 * Gets or sets if the application should queue a %GRX_EVENT_TYPE_APP_QUIT
 * event when SIGHUP, SIGINT or SIGTERM is received.
 *
 * The default behavior is for an application to quit when any of these signals
 * are received.
 */

/**
 * grx_application_get_quit_on_signal:
 * @application: a #GrxApplication
 *
 * Gets the "quit-on-signal" property value.
 *
 * Returns: %TRUE if the @application will quit when SIGHUP, SIGINT or SIGTERM
 * is received, otherwise %FALSE.
 */
gboolean grx_application_get_quit_on_signal(GrxApplication *application)
{
    GrxApplicationPrivate *priv = grx_application_get_instance_private(application);

    return priv->sighup_source_id != 0;
}

/**
 * grx_application_set_quit_on_signal:
 * @application: a #GrxApplication
 * @value: %TRUE will cause the application to queue a %GRX_EVENT_TYPE_APP_QUIT
 * event when SIGHUP, SIGINT or SIGTERM is received. %FALSE will remove the
 * signal handlers.
 *
 * Sets the "quit-on-signal" property value.
 */
void grx_application_set_quit_on_signal(GrxApplication *application, gboolean value)
{
    static GrxEvent quit_event = { .type = GRX_EVENT_TYPE_APP_QUIT };

    GrxApplicationPrivate *priv = grx_application_get_instance_private(application);

    if (value) {
        if (priv->sighup_source_id == 0) {
            priv->sighup_source_id =
                g_unix_signal_add(SIGHUP, (GSourceFunc)grx_event_put, &quit_event);
        }
        if (priv->sigint_source_id == 0) {
            priv->sigint_source_id =
                g_unix_signal_add(SIGINT, (GSourceFunc)grx_event_put, &quit_event);
        }
        if (priv->sigterm_source_id == 0) {
            priv->sigterm_source_id =
                g_unix_signal_add(SIGTERM, (GSourceFunc)grx_event_put, &quit_event);
        }
    }
    else {
        if (priv->sighup_source_id != 0) {
            g_source_remove(priv->sighup_source_id);
            priv->sighup_source_id = 0;
        }
        if (priv->sigint_source_id != 0) {
            g_source_remove(priv->sigint_source_id);
            priv->sigint_source_id = 0;
        }
        if (priv->sigterm_source_id != 0) {
            g_source_remove(priv->sigterm_source_id);
            priv->sigterm_source_id = 0;
        }
    }
}

static void set_property(
    GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id) {
    case PROP_QUIT_ON_SIGNAL:
        grx_application_set_quit_on_signal(
            GRX_APPLICATION(object), g_value_get_boolean(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}

static void get_property(
    GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    GrxApplication *self = GRX_APPLICATION(object);
    GrxApplicationPrivate *priv = grx_application_get_instance_private(self);

    switch (property_id) {
    case PROP_IS_ACTIVE:
        g_value_set_boolean(value, priv->active);
        break;
    case PROP_QUIT_ON_SIGNAL:
        g_value_set_boolean(
            value, grx_application_get_quit_on_signal(GRX_APPLICATION(object)));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}

/* signals */

enum { SIG_0, SIG_EVENT, N_SIGNALS };

static uint signals[N_SIGNALS] = { 0 };

/**
 * GrxApplication::event:
 * @application: the object that received the signal
 * @event: the event that triggered the signal
 *
 * This signal is emitted when an event occurs. Handlers can return %TRUE to
 * prevent any further handlers from running.
 *
 * Returns: %TRUE if the handler handled the event, or %FALSE to continue
 *          propagating the event.
 */
static gboolean event(GrxApplication *application, GrxEvent *event)
{
    GrxApplicationPrivate *priv = grx_application_get_instance_private(application);

    switch (event->type) {
    case GRX_EVENT_TYPE_APP_ACTIVATE:
        if (!priv->active) {
            priv->active = TRUE;
            g_object_notify_by_pspec(G_OBJECT(application), properties[PROP_IS_ACTIVE]);
        }
        break;
    case GRX_EVENT_TYPE_APP_DEACTIVATE:
        if (priv->active) {
            priv->active = FALSE;
            g_object_notify_by_pspec(G_OBJECT(application), properties[PROP_IS_ACTIVE]);
        }
        break;
    case GRX_EVENT_TYPE_APP_QUIT:
        g_application_quit(G_APPLICATION(application));
        break;
    default:
        if (priv->active) {
            return FALSE;
        }
        break;
    }

    return TRUE;
}

/* class implementation */

static void event_handler(GrxEvent *event, gpointer user_data)
{
    GrxApplication *application = GRX_APPLICATION(user_data);
    gboolean handled;

    g_signal_emit(application, signals[SIG_EVENT], 0, event, &handled);
}

static void startup(GApplication *application)
{
    GrxApplicationPrivate *priv =
        grx_application_get_instance_private(GRX_APPLICATION(application));

    G_APPLICATION_CLASS(grx_application_parent_class)->startup(application);

    priv->event_source_id = grx_event_handler_add(event_handler, application, NULL);
}

static void shutdown(GApplication *application)
{
    GrxApplicationPrivate *priv =
        grx_application_get_instance_private(GRX_APPLICATION(application));

    G_APPLICATION_CLASS(grx_application_parent_class)->shutdown(application);

    g_source_remove(priv->event_source_id);
}

static void grx_application_class_init(GrxApplicationClass *klass)
{
    G_OBJECT_CLASS(klass)->set_property = set_property;
    G_OBJECT_CLASS(klass)->get_property = get_property;

    properties[PROP_IS_ACTIVE] = g_param_spec_boolean("is-active",
        "application is active", "Gets if the application is active.",
        FALSE /* default value */, G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);
    properties[PROP_QUIT_ON_SIGNAL] =
        g_param_spec_boolean("quit-on-signal", "application will quit on signal",
            "Gets or sets if the application will queue a GRX_EVENT_TYPE_APP_QUIT "
            "event when SIGHUP, SIGINT or SIGTERM is received.",
            TRUE /* default value */,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS);
    g_object_class_install_properties(G_OBJECT_CLASS(klass), N_PROPERTIES, properties);

    signals[SIG_EVENT] = g_signal_new("event", G_TYPE_FROM_CLASS(klass),
        G_SIGNAL_RUN_LAST, G_STRUCT_OFFSET(GrxApplicationClass, event),
        g_signal_accumulator_true_handled, NULL,     /* accumulator data */
        _grx_marshal_BOOLEAN__BOXED, G_TYPE_BOOLEAN, /* return type */
        1,                                           /* n_params */
        GRX_TYPE_EVENT | G_SIGNAL_TYPE_STATIC_SCOPE);

    G_APPLICATION_CLASS(klass)->startup = startup;
    G_APPLICATION_CLASS(klass)->shutdown = shutdown;

    klass->event = event;
}

static void grx_application_init(GrxApplication *self) { }

/* interface implementation */

static gboolean init(GInitable *initable, GCancellable *cancellable, GError **error)
{
    return grx_set_mode_default_graphics(TRUE, error);
}

static void initable_interface_init(GInitableIface *iface)
{
    iface->init = init;
}

/* constructors */

/**
 * grx_application_new:
 * @error: a pointer to a NULL #GError, or %NULL
 *
 * Create a new instance of #GrxApplication. The console will be
 * set to graphics mode or there will be an @error if setting graphics mode
 * failed.
 *
 * Returns: (nullable): New instance of #GrxApplication or %NULL
 * if init() failed.
 */
GrxApplication *grx_application_new(GError **error)
{
    return g_initable_new(GRX_TYPE_APPLICATION, NULL, error, NULL);
}

/**
 * grx_application_new_full:
 * @id: (nullable): the application id or %NULL
 * @flags: the application flags
 * @error: a pointer to a NULL #GError, or %NULL
 *
 * Create a new instance of #GrxApplication. The console will be
 * set to graphics mode or there will be an @error if setting graphics mode
 * failed.
 *
 * Returns: (nullable): New instance of #GrxApplication or %NULL
 * if init() failed.
 */
GrxApplication *grx_application_new_full(
    const gchar *id, GApplicationFlags flags, GError **error)
{
    g_return_val_if_fail(id != NULL && !g_application_id_is_valid(id), NULL);

    return g_initable_new(
        GRX_TYPE_APPLICATION, NULL, error, "application-id", id, "flags", flags, NULL);
}
