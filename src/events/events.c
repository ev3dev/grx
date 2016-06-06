/*
 * events.c
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

#include <grx/events.h>

/**
 * GrxEvent:
 * @type: the type of the event
 * @any: the event as #GrxAnyEvent
 * @key: the event as #GrxKeyEvent
 * @button: the event as #GrxButtonEvent
 * @touch: the event as #GrxTouchEvent
 */
G_DEFINE_BOXED_TYPE (GrxEvent, grx_event, grx_event_copy, grx_event_free);

static GQueue *queue;

static inline GQueue *get_queue (void)
{
    if (!queue) {
        queue = g_queue_new ();
    }

    return queue;
}

/**
 * grx_event_new:
 * @type: the event type
 *
 * Allocates a new #GrxEvent.
 *
 * Returns: (transfer full): the event struct
 */
GrxEvent *grx_event_new (GrxEventType type)
{
    GrxEvent *event = g_slice_new0 (GrxEvent);
    event->type = type;

    return event;
}

/**
 * grx_events_pending:
 *
 * Checks if there are any events in the queue.
 *
 * Returns: %TRUE if there are events, otherwise %FALSE
 */
gboolean grx_events_pending (void)
{
    return !g_queue_is_empty (get_queue ());
}

/**
 * grx_event_peek:
 *
 * Gets the first event in the queue, but does not remove the event from the
 * queue.
 *
 * Returns: (transfer none): the event
 */
GrxEvent *grx_event_peek (void)
{
    return g_queue_peek_head (get_queue ());
}

/**
 * grx_event_get:
 *
 * Gets the first event in the queue, and removes that event from the queue.
 *
 * Free the event with grx_event_free().
 *
 * Returns: (transfer full): the event
 */
GrxEvent *grx_event_get (void)
{
    return g_queue_pop_head (get_queue ());
}

/**
 * grx_event_put:
 * @event: the event
 *
 * Adds a copy of @event to the end of the queue.
 */
void grx_event_put (GrxEvent *event)
{
    g_queue_push_tail (get_queue (), grx_event_copy (event));
}

/**
 * grx_event_copy:
 * @event: the event to copy
 *
 * Creates a copy of @event.
 *
 * Returns: (transfer full): a copy of @event
 */
GrxEvent *grx_event_copy (GrxEvent *event)
{
    g_return_val_if_fail (event != NULL, NULL);

    GrxEvent* new_event = g_slice_dup (GrxEvent, event);
    switch (event->type) {
    case GRX_EVENT_TYPE_NONE:
    case GRX_EVENT_TYPE_APP_ACTIVATE:
    case GRX_EVENT_TYPE_APP_DEACTIVATE:
    case GRX_EVENT_TYPE_APP_QUIT:
        break;
    case GRX_EVENT_TYPE_KEY_UP:
    case GRX_EVENT_TYPE_KEY_DOWN:
        if (new_event->key.device) {
            g_object_ref (new_event->key.device);
        }
        break;
    case GRX_EVENT_TYPE_BUTTON_PRESS:
    case GRX_EVENT_TYPE_BUTTON_RELEASE:
    case GRX_EVENT_TYPE_BUTTON_DOUBLE_PRESS:
        if (new_event->button.device) {
            g_object_ref (new_event->button.device);
        }
        break;
    case GRX_EVENT_TYPE_TOUCH_DOWN:
    case GRX_EVENT_TYPE_TOUCH_MOTION:
    case GRX_EVENT_TYPE_TOUCH_UP:
    case GRX_EVENT_TYPE_TOUCH_CANCEL:
        if (new_event->touch.device) {
            g_object_ref (new_event->touch.device);
        }
        break;
    }

    return new_event;
}

/**
 * grx_event_free:
 * @event: (transfer full): the event
 *
 * Frees @event.
 */
void grx_event_free (GrxEvent *event)
{
    switch (event->type) {
    case GRX_EVENT_TYPE_NONE:
    case GRX_EVENT_TYPE_APP_ACTIVATE:
    case GRX_EVENT_TYPE_APP_DEACTIVATE:
    case GRX_EVENT_TYPE_APP_QUIT:
        break;
    case GRX_EVENT_TYPE_KEY_UP:
    case GRX_EVENT_TYPE_KEY_DOWN:
        if (event->key.device) {
            g_object_unref (event->key.device);
        }
        break;
    case GRX_EVENT_TYPE_BUTTON_PRESS:
    case GRX_EVENT_TYPE_BUTTON_RELEASE:
    case GRX_EVENT_TYPE_BUTTON_DOUBLE_PRESS:
        if (event->button.device) {
            g_object_unref (event->button.device);
        }
        break;
    case GRX_EVENT_TYPE_TOUCH_DOWN:
    case GRX_EVENT_TYPE_TOUCH_MOTION:
    case GRX_EVENT_TYPE_TOUCH_UP:
    case GRX_EVENT_TYPE_TOUCH_CANCEL:
        if (event->touch.device) {
            g_object_unref (event->touch.device);
        }
        break;
    }
    g_slice_free (GrxEvent, event);
}

/* source implementation */

static gboolean source_prepare (GSource *source, gint *timeout)
{
    *timeout = -1;

    return grx_events_pending ();
}

static gboolean source_check (GSource *source)
{
    return grx_events_pending ();
}

static gboolean
source_dispatch (GSource *source, GSourceFunc callback, gpointer user_data)
{
    GrxEvent *event = grx_event_get ();

    if (!event) {
        return G_SOURCE_CONTINUE;
    }

    if (callback) {
        ((GrxEventSourceFunc)callback) (event, user_data);
    }

    grx_event_free (event);

    return G_SOURCE_CONTINUE;
}

static GSourceFuncs source_funcs = {
    .prepare    = source_prepare,
    .check      = source_check,
    .dispatch   = source_dispatch,
    .finalize   = NULL,
};

/**
 * grx_events_source_new:
 *
 * Creates a new source that will be dispatched when events occur.
 *
 * The source will not initially be associated with any #GMainContext and must
 * be added to one with g_source_attach() before it will be executed.
 *
 * Generally, you will want to use grx_events_add() instead.
 *
 * Returns: a new #GSource
 */
GSource *grx_events_source_new ()
{
    return g_source_new (&source_funcs, sizeof (GSource));
}

/**
 * grx_events_add:
 * @callback: the function to be called when an event occurs
 * @user_data: (transfer full) (nullable): user data passed to @callback
 * @notify: (nullable): callback to destroy @user_data when source is removed
 *
 * Creates a new event source and adds it to the default main context.
 *
 * Returns: the source id
 */
guint grx_events_add (GrxEventSourceFunc callback, gpointer user_data,
                      GDestroyNotify notify)
{
    GSource *source;
    guint id;

    source = grx_events_source_new ();
    g_source_set_callback (source, (GSourceFunc)callback, user_data, notify);
    id = g_source_attach (source, g_main_context_default ());
    g_source_unref (source);

    return id;
}
