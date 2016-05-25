/*
 * input.h
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

#ifndef __GRX_INPUT_CONTEXT_H__
#define __GRX_INPUT_CONTEXT_H__

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include <grx/libinput_device.h>

/**
 * SECTION:input
 * @short_description: Common input framework
 * @title: Input
 * @section_id: input
 * @include: grx-3.0.h
 *
 * TODO
 */

/**
 * GrxInputEventType:
 * @GRX_INPUT_EVENT_TYPE_NONE: indicates there is no event
 * @GRX_INPUT_EVENT_TYPE_KEY_DOWN: key press event
 * @GRX_INPUT_EVENT_TYPE_KEY_UP: key release event
 * @GRX_INPUT_EVENT_TYPE_BUTTON_PRESS: button press event
 * @GRX_INPUT_EVENT_TYPE_BUTTON_RELEASE: button release event
 * @GRX_INPUT_EVENT_TYPE_BUTTON_DOUBLE_PRESS: button double-press event
 * @GRX_INPUT_EVENT_TYPE_TOUCH_DOWN: begin touch event
 * @GRX_INPUT_EVENT_TYPE_TOUCH_MOTION: touch motion event
 * @GRX_INPUT_EVENT_TYPE_TOUCH_UP: end touch event
 * @GRX_INPUT_EVENT_TYPE_TOUCH_CANCEL: cancel touch event
 *
 * Indicates the type of input event.
 */
typedef enum {
    GRX_INPUT_EVENT_TYPE_NONE,
    GRX_INPUT_EVENT_TYPE_KEY_DOWN,
    GRX_INPUT_EVENT_TYPE_KEY_UP,
    GRX_INPUT_EVENT_TYPE_BUTTON_PRESS,
    GRX_INPUT_EVENT_TYPE_BUTTON_RELEASE,
    GRX_INPUT_EVENT_TYPE_BUTTON_DOUBLE_PRESS,
    GRX_INPUT_EVENT_TYPE_TOUCH_DOWN,
    GRX_INPUT_EVENT_TYPE_TOUCH_MOTION,
    GRX_INPUT_EVENT_TYPE_TOUCH_UP,
    GRX_INPUT_EVENT_TYPE_TOUCH_CANCEL,
} GrxInputEventType;

typedef struct {
    GrxInputEventType type;
    GrxLibinputDevice *device;
} GrxAnyEvent;

typedef struct {
    GrxInputEventType type;
    GrxLibinputDevice *device;
    guint key;
} GrxKeyEvent;

typedef struct {
    GrxInputEventType type;
    GrxLibinputDevice *device;
    guint button;
} GrxButtonEvent;

typedef struct {
    GrxInputEventType type;
    GrxLibinputDevice *device;
    gint id;
    gint x;
    gint y;
} GrxTouchEvent;

typedef union {
    GrxInputEventType type;
    GrxAnyEvent any;
    GrxKeyEvent key;
    GrxButtonEvent button;
    GrxTouchEvent touch;
} GrxInputEvent;

#endif /* __GRX_INPUT_CONTEXT_H__ */
