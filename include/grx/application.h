/*
 * application.h - GRX Application
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

#ifndef __GRX_APPLICATION_H__
#define __GRX_APPLICATION_H__

#include <gio/gio.h>
#include <glib-object.h>

#include <grx/common.h>

#define GRX_TYPE_APPLICATION grx_application_get_type()

#define GRX_APPLICATION(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GRX_TYPE_APPLICATION, GrxApplication))
#define GRX_IS_APPLICATION(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRX_TYPE_APPLICATION))
#define GRX_APPLICATION_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), GRX_TYPE_APPLICATION, GrxApplicationClass))
#define GRX_IS_APPLICATION_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), GRX_TYPE_APPLICATION))
#define GRX_APPLICATION_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRX_TYPE_APPLICATION, GrxApplicationClass))

typedef struct _GrxApplication GrxApplication;
typedef struct _GrxApplicationClass GrxApplicationClass;

/**
 * GrxApplication:
 *
 * #GrxApplication is an opaque data structure and can only be
 * accessed using the following functions.
 */
struct _GrxApplication
{
    /*<private>*/
    GApplication parent_instance;
};

GType grx_application_get_type (void);

// end of code that could be replaced with G_DECLARE_DERIVABLE_TYPE

/**
 * GrxApplicationClass:
 * @parent_class: the parent class struct
 * @input_event: The input event handler. Subclasses should override this method
 * if they are interested in input events.
 * @reserved: for future use
 */
struct _GrxApplicationClass {
    GApplicationClass parent_class;
    void (*input_event) (GrxApplication *application, GrxEvent *event);
    gpointer reserved[6];
};

GrxApplication *
grx_application_new (GError **error);
GrxApplication *
grx_application_new_full (const gchar *id, GApplicationFlags flags, GError **error);

gboolean
grx_application_is_active (GrxApplication *application);
gboolean
grx_application_get_quit_on_signal (GrxApplication *application);
void
grx_application_set_quit_on_signal (GrxApplication *application, gboolean value);

#endif /* __GRX_APPLICATION_H__ */
