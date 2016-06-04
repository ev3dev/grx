/*
 * gtk3_device_manager.c
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
#include <glib/gstdio.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

#include <grx/extents.h>
#include <grx/input_keys.h>
#include <grx/input_event.h>
#include <grx/gtk3_device.h>
#include <grx/gtk3_device_manager.h>
#include <grx/mode.h>

#include "gtk3_device_internal.h"



typedef struct {
    GdkDeviceManager *gdk_device_manager;
    GHashTable *device_map;
    gulong device_added_signal_id;
    gulong device_removed_signal_id;
} GrxGtk3DeviceManagerPrivate;

static void initable_interface_init (GInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE (GrxGtk3DeviceManager,
    grx_gtk3_device_manager, G_TYPE_OBJECT,
    G_ADD_PRIVATE (GrxGtk3DeviceManager)
    G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, initable_interface_init))

/* Properties */

// enum {
//     PROP_0,
//     N_PROPERTIES
// };

// static GParamSpec *properties[N_PROPERTIES] = { NULL };

// static void
// set_property (GObject *object, guint property_id, const GValue *value,
//               GParamSpec *pspec)
// {
//     switch (property_id) {
//     default:
//         G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
//         break;
//     }
// }

// static void
// get_property (GObject *object, guint property_id, GValue *value,
//               GParamSpec *pspec)
// {
//     switch (property_id) {
//     default:
//         G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
//         break;
//     }
// }

/* signals */

enum {
    SIG_0,
    SIG_DEVICE_ADDED,
    SIG_DEVICE_REMOVED,
    N_SIGNALS
};

static uint signals[N_SIGNALS] = { 0 };

/* class implementation */

static gboolean
free_device_map_item (gpointer key, gpointer value, gpointer user_data)
{
    g_object_unref (G_OBJECT (value));

    return TRUE;
}

static void finalize (GObject *object)
{
    GrxGtk3DeviceManager *self = GRX_GTK3_DEVICE_MANAGER (object);
    GrxGtk3DeviceManagerPrivate *priv = self->private;

    G_OBJECT_CLASS (grx_gtk3_device_manager_parent_class)->finalize (object);

    if (priv->gdk_device_manager) {
        g_signal_handler_disconnect (priv->gdk_device_manager,
                                     priv->device_added_signal_id);
        g_signal_handler_disconnect (priv->gdk_device_manager,
                                     priv->device_removed_signal_id);
    }
    g_hash_table_foreach_remove (priv->device_map, free_device_map_item, NULL);
    g_hash_table_unref (priv->device_map);
}

static void
grx_gtk3_device_manager_class_init (GrxGtk3DeviceManagerClass *klass)
{
    // G_OBJECT_CLASS (klass)->set_property = set_property;
    // G_OBJECT_CLASS (klass)->get_property = get_property;

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
                      GRX_TYPE_GTK3_DEVICE /* types */);
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
                      GRX_TYPE_GTK3_DEVICE /* types */);

    G_OBJECT_CLASS (klass)->finalize = finalize;
}

static void
grx_gtk3_device_manager_init (GrxGtk3DeviceManager *self)
{
    GrxGtk3DeviceManagerPrivate *priv =
        grx_gtk3_device_manager_get_instance_private (self);

    self->private = priv;
    priv->device_map = g_hash_table_new (g_direct_hash, g_direct_equal);
}

/* interface implementation */

static void on_device_added (GdkDeviceManager *device_manager,
                             GdkDevice *device, gpointer user_data)
{
    GrxGtk3DeviceManager *self = GRX_GTK3_DEVICE_MANAGER (user_data);
    GrxGtk3DeviceManagerPrivate *priv =
        grx_gtk3_device_manager_get_instance_private (self);
    GrxGtk3Device *grx_device;

    grx_device = grx_gtk3_device_new (device);
    g_hash_table_insert (priv->device_map, device, grx_device);
    g_signal_emit (self, signals[SIG_DEVICE_ADDED], 0, grx_device);
}

static void on_device_removed (GdkDeviceManager *device_manager,
                               GdkDevice *device, gpointer user_data)
{
    GrxGtk3DeviceManager *self = GRX_GTK3_DEVICE_MANAGER (user_data);
    GrxGtk3DeviceManagerPrivate *priv =
        grx_gtk3_device_manager_get_instance_private (self);
    GrxGtk3Device *grx_device;

    grx_device = GRX_GTK3_DEVICE (g_hash_table_lookup (priv->device_map, device));
    g_hash_table_remove (priv->device_map, device);
    g_signal_emit (self, signals[SIG_DEVICE_REMOVED], 0, grx_device);
    g_object_unref (grx_device);
}

static gboolean
init (GInitable *initable, GCancellable *cancellable, GError **error)
{
    GrxGtk3DeviceManager *self = GRX_GTK3_DEVICE_MANAGER (initable);
    GrxGtk3DeviceManagerPrivate *priv = self->private;
    GdkDisplay *display;

    display = gdk_display_get_default ();
    if (!display) {
        g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED,
                     "Failed to get default display");
        return FALSE;
    }

    priv->gdk_device_manager = gdk_display_get_device_manager (display);
    if (!priv->gdk_device_manager) {
        g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED,
                     "Failed to get default GDK device manager");
        return FALSE;
    }

    priv->device_added_signal_id = g_signal_connect (priv->gdk_device_manager,
        "device-added", (GCallback)on_device_added, self);
    priv->device_removed_signal_id = g_signal_connect (priv->gdk_device_manager,
        "device-removed", (GCallback)on_device_removed, self);

    return TRUE;
}

static void initable_interface_init (GInitableIface *iface)
{
    iface->init = init;
}

/* constructors */

/**
 * grx_gtk3_device_manager_new:
 * @cancellable: (nullable): not used - pass %NULL
 * @error: a pointer to a #GError
 *
 * Creates a new instance of #GrxGtk3DeviceManager
 *
 * Returns: the new instance or %NULL if initialization failed
 */
GrxGtk3DeviceManager *
grx_gtk3_device_manager_new (GCancellable *cancellable, GError **error)
{
    GrxGtk3DeviceManager *instance = g_object_new (GRX_TYPE_GTK3_DEVICE_MANAGER, NULL);
    if (!g_initable_init (G_INITABLE (instance), cancellable, error)) {
        g_object_unref (G_OBJECT (instance));
        return NULL;
    }

    return instance;
}

GrxGtk3Device *
grx_gtk3_device_lookup (GrxGtk3DeviceManager *manager, GdkDevice *device)
{
    GrxGtk3DeviceManagerPrivate *priv;

    g_return_val_if_fail (manager != NULL, NULL);
    g_return_val_if_fail (device != NULL, NULL);

    priv = manager->private;

    return GRX_GTK3_DEVICE (g_hash_table_lookup (priv->device_map, device));
}
