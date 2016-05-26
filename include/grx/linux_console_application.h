/*
 * linux_console_application.h - Linux Console Application
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

#ifndef __GRX_LINUX_CONSOLE_APPLICATION_H__
#define __GRX_LINUX_CONSOLE_APPLICATION_H__

#include <gio/gio.h>
#include <glib-object.h>

#define GRX_TYPE_LINUX_CONSOLE_APPLICATION grx_linux_console_application_get_type()

#define GRX_LINUX_CONSOLE_APPLICATION(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GRX_TYPE_LINUX_CONSOLE_APPLICATION, \
     GrxLinuxConsoleApplication))
#define GRX_IS_LINUX_CONSOLE_APPLICATION(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRX_TYPE_LINUX_CONSOLE_APPLICATION))
#define GRX_LINUX_CONSOLE_APPLICATION_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), GRX_TYPE_LINUX_CONSOLE_APPLICATION, \
     GrxLinuxConsoleApplicationClass))
#define GRX_IS_LINUX_CONSOLE_APPLICATION_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), GRX_TYPE_LINUX_CONSOLE_APPLICATION))
#define GRX_LINUX_CONSOLE_APPLICATION_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRX_TYPE_LINUX_CONSOLE_APPLICATION, \
     GrxLinuxConsoleApplicationClass))

typedef struct _GrxLinuxConsoleApplication GrxLinuxConsoleApplication;
typedef struct _GrxLinuxConsoleApplicationClass GrxLinuxConsoleApplicationClass;

/**
 * GrxLinuxConsoleApplication:
 *
 * #GrxLinuxConsoleApplication is an opaque data structure and can only be
 * accessed using the following functions.
 */
struct _GrxLinuxConsoleApplication
{
    /*<private>*/
    GApplication parent_instance;
};

GType grx_linux_console_application_get_type (void);

// end of code that could be replaced with G_DECLARE_DERIVABLE_TYPE

/**
 * GrxLinuxConsoleApplicationClass:
 * @parent_class: the parent class struct
 * @input_event: The input event handler. Subclasses should override this method
 *               if they are interested in input events. No need to chain up as
 *               the default implementation does nothing.
 * @reserved: for future use
 */
struct _GrxLinuxConsoleApplicationClass {
    GApplicationClass parent_class;
    void (*input_event) (GrxLinuxConsoleApplication *application, GrxInputEvent *event);
    gpointer reserved[6];
};

GrxLinuxConsoleApplication *
grx_linux_console_application_new (GCancellable *cancellable, GError **error);

gboolean
grx_linux_console_application_is_console_active (GrxLinuxConsoleApplication *application);

#endif /* __GRX_LINUX_CONSOLE_APPLICATION_H__ */
