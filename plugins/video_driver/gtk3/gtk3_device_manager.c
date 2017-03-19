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

#include <grx/device.h>
#include <grx/events.h>
#include <grx/extents.h>
#include <grx/input_keys.h>
#include <grx/mode.h>

#include "gtk3_device.h"
#include "gtk3_device_manager.h"


typedef struct {
    GdkDeviceManager *gdk_device_manager;
    GHashTable *device_map;
    gulong device_added_signal_id;
    gulong device_removed_signal_id;
} GrxGtk3DeviceManagerPrivate;

static void initable_interface_init (GInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE (GrxGtk3DeviceManager,
    grx_gtk3_device_manager, GRX_TYPE_DEVICE_MANAGER,
    G_ADD_PRIVATE (GrxGtk3DeviceManager)
    G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, initable_interface_init))

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

    g_hash_table_foreach_remove (priv->device_map, free_device_map_item, NULL);
    g_hash_table_unref (priv->device_map);
}

static void
grx_gtk3_device_manager_class_init (GrxGtk3DeviceManagerClass *klass)
{
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

    // For now we only care about master devices. If there is a use case, we
    // can handle other types.
    if (gdk_device_get_device_type(device) != GDK_DEVICE_TYPE_MASTER) {
        return;
    }

    grx_device = grx_gtk3_device_new (device);
    g_hash_table_insert (priv->device_map, device, grx_device);
    g_signal_emit_by_name (self, "device-added", grx_device);
}

static void on_device_removed (GdkDeviceManager *device_manager,
                               GdkDevice *device, gpointer user_data)
{
    GrxGtk3DeviceManager *self = GRX_GTK3_DEVICE_MANAGER (user_data);
    GrxGtk3DeviceManagerPrivate *priv =
        grx_gtk3_device_manager_get_instance_private (self);
    GrxGtk3Device *grx_device;

    // For now we only care about master devices. If there is a use case, we
    // can handle other types.
    if (gdk_device_get_device_type(device) != GDK_DEVICE_TYPE_MASTER) {
        return;
    }

    grx_device = GRX_GTK3_DEVICE (g_hash_table_lookup (priv->device_map, device));
    g_hash_table_remove (priv->device_map, device);
    g_signal_emit_by_name (self, "device-removed", grx_device);
    g_object_unref (grx_device);
}

typedef struct {
    GrxDeviceManager *grx_device_manager;
    GdkDeviceManager *gdk_device_manager;
    GdkDevice *device;
} AddDeviceData;

static gboolean add_device (gpointer user_data)
{
    AddDeviceData *data = user_data;

    on_device_added (data->gdk_device_manager, data->device, data->grx_device_manager);
    g_object_unref (data->gdk_device_manager);
    g_object_unref (data->grx_device_manager);
    g_object_unref (data->device);
    g_free (data);

    return G_SOURCE_REMOVE;
}

// We are using a couple of functions that are deprecated in GDK 3.20, but we
// want to be able to build on both 3.18 (Ubuntu xenial) and 3.20 (Debian stretch)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

static gboolean
init (GInitable *initable, GCancellable *cancellable, GError **error)
{
    GrxGtk3DeviceManager *self = GRX_GTK3_DEVICE_MANAGER (initable);
    GrxGtk3DeviceManagerPrivate *priv = self->private;
    GdkDisplay *display;
    GList *devices, *item;

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

    // defer initial "device-added" signals until main loop is started
    devices = gdk_device_manager_list_devices (priv->gdk_device_manager, GDK_DEVICE_TYPE_MASTER);
    for (item = devices; item != NULL; item = g_list_next (item)) {
        AddDeviceData *data;

        data = g_malloc (sizeof(*data));
        data->grx_device_manager = g_object_ref (self);
        data->gdk_device_manager = g_object_ref (priv->gdk_device_manager);
        data->device = g_object_ref (item->data);
        g_idle_add (add_device, data);
    }
    g_list_free (devices);

    return TRUE;
}

#pragma GCC diagnostic pop

static void initable_interface_init (GInitableIface *iface)
{
    iface->init = init;
}

/* methods */

GrxGtk3Device *
grx_gtk3_device_lookup (GrxGtk3DeviceManager *manager, GdkDevice *device)
{
    GrxGtk3DeviceManagerPrivate *priv;

    g_return_val_if_fail (manager != NULL, NULL);
    g_return_val_if_fail (device != NULL, NULL);

    priv = manager->private;

    return GRX_GTK3_DEVICE (g_hash_table_lookup (priv->device_map, device));
}
