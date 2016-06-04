/*
 * gtk3_application.h - Linux Console Application
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

#ifndef __GRX_GTK3_APPLICATION_H__
#define __GRX_GTK3_APPLICATION_H__

#include <gio/gio.h>
#include <glib-object.h>

#include <grx/input_event.h>

#define GRX_TYPE_GTK3_APPLICATION grx_gtk3_application_get_type()

#define GRX_GTK3_APPLICATION(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GRX_TYPE_GTK3_APPLICATION, \
     GrxGtk3Application))
#define GRX_IS_GTK3_APPLICATION(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRX_TYPE_GTK3_APPLICATION))
#define GRX_GTK3_APPLICATION_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), GRX_TYPE_GTK3_APPLICATION, \
     GrxGtk3ApplicationClass))
#define GRX_IS_GTK3_APPLICATION_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), GRX_TYPE_GTK3_APPLICATION))
#define GRX_GTK3_APPLICATION_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRX_TYPE_GTK3_APPLICATION, \
     GrxGtk3ApplicationClass))

typedef struct _GrxGtk3Application GrxGtk3Application;
typedef struct _GrxGtk3ApplicationClass GrxGtk3ApplicationClass;

/**
 * GrxGtk3Application:
 *
 * #GrxGtk3Application is an opaque data structure and can only be
 * accessed using the following functions.
 */
struct _GrxGtk3Application
{
    /*<private>*/
    GApplication parent_instance;
};

GType grx_gtk3_application_get_type (void);

// end of code that could be replaced with G_DECLARE_DERIVABLE_TYPE

/**
 * GrxGtk3ApplicationClass:
 * @parent_class: the parent class struct
 * @input_event: The input event handler. Subclasses should override this method
 *               if they are interested in input events.
 * @reserved: for future use
 */
struct _GrxGtk3ApplicationClass {
    GApplicationClass parent_class;
    void (*input_event) (GrxGtk3Application *application, GrxInputEvent *event);
    gpointer reserved[6];
};

GrxGtk3Application *
grx_gtk3_application_new (GCancellable *cancellable, GError **error);

gboolean
grx_gtk3_application_is_console_active (GrxGtk3Application *application);

#endif /* __GRX_GTK3_APPLICATION_H__ */
