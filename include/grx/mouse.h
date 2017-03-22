/*
 * mouse.h
 *
 * Copyright (c) 2017 David Lechner <david@lechnology.com>
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
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

/**
 * SECTION:mouse
 * @short_description: Cursors
 * @title: Graphics cursors
 * @section_id: mouse
 * @include: grx-3.0.h
 *
 * The library provides support for the creation and usage of an unlimited
 * number of graphics cursors. An application can use these cursors for any
 * purpose, e.g. mouse cursor, animation sprites. Cursors always save the area
 * they occupy before they are drawn. When moved or erased they restore this
 * area. As a general rule of thumb, an application should erase a cursor before
 * making changes to an area it occupies and redraw the cursor after finishing
 * the drawing.
 *
 * A devault mouse cursor is created during mode setting and it will be
 * automatically shown if and only if there is a pointing device attached.
 */

#ifndef __GRX_INPUT_H__
#define __GRX_INPUT_H__

#include <glib.h>

#include <grx/color.h>
#include <grx/common.h>

/**
 * GrxCursor:
 *
 * Structure to hold cursor state.
 */
typedef struct _GrxCursor GrxCursor;

GType grx_cursor_get_type(void);
GrxCursor *grx_cursor_new(guint8 *pixels, gint pitch, gint width, gint height, gint x0, gint y0, const GrxColorTable colors);
GrxCursor *grx_cursor_ref(GrxCursor *cursor);
void grx_cursor_unref(GrxCursor *cursor);
void grx_cursor_show(GrxCursor *cursor);
void grx_cursor_hide(GrxCursor *cursor);
void grx_cursor_move(GrxCursor *cursor, gint x, gint y);

/**
 * GrxCursorMode:
 * @GRX_CURSOR_MODE_NORMAL: just the cursor
 * @GRX_CURSOR_MODE_RUBBER: rectangular rubber band (XOR-d to the screen)
 * @GRX_CURSOR_MODE_LINE: line attached to the cursor
 * @GRX_CURSOR_MODE_BOX: rectangular box dragged by the cursor
 *
 * Mouse cursor modes.
 */
typedef enum {
    GRX_CURSOR_MODE_NORMAL,
    GRX_CURSOR_MODE_RUBBER,
    GRX_CURSOR_MODE_LINE,
    GRX_CURSOR_MODE_BOX,
} GrxCursorMode;

#ifndef __GI_SCANNER__
#ifndef __GTK_DOC_IGNORE__
/* TODO: This struct should probably be private */

/*
 * mouse status information
 */
extern const struct _GR_mouseInfo {
    gint   (*block)(GrxContext*,gint,gint,gint,gint); /* mouse block function */
    void  (*unblock)(guint flags); /* mouse unblock function */
    GrxCursor *cursor;             /* the mouse cursor */
    gboolean displayed;            /* cursor is (generally) drawn */
    guint    blockflag;            /* cursor temp. erase/block flag */
    gboolean docheck;              /* need to check before gr. op. to screen */
    guint    cursmode;             /* mouse cursor draw mode */
    gint     x1,y1,x2,y2;          /* auxiliary params for some cursor draw modes */
    GrxColor curscolor;            /* color for some cursor draw modes */
    gint     xpos,ypos;            /* current mouse position */
    GrxDeviceManager *device_manager;
    gulong device_added_handler;
    gulong device_removed_handler;
    guint  pointer_device_count;
} * const GrMouseInfo;

#endif /* __GTK_DOC_IGNORE__ */
#endif /* __GI_SCANNER__ */

GrxCursor *grx_mouse_get_cursor(void);
void grx_mouse_set_cursor(GrxCursor *cursor);
void grx_mouse_set_cursor_default(GrxColor fill_color, GrxColor border_color);
void grx_mouse_set_cursor_mode(int mode, ...);
void _grx_mouse_show_cursor(void);
void _grx_mouse_hide_cursor(void);
void _grx_mouse_update_cursor(void);
gboolean grx_mouse_is_cursor_shown(void);

guint grx_mouse_block(GrxContext *context, gint x1, gint y1, gint x2, gint y2);
void grx_mouse_unblock(guint flags);

#ifndef GRX_SKIP_INLINES
#define grx_mouse_get_cursor()       (GrMouseInfo->cursor)
#define grx_mouse_is_cursor_shown()  (GrMouseInfo->displayed)
#define grx_mouse_block(c,x1,y1,x2,y2) (                                            \
    (((c) ? (const GrxContext*)(c) : grx_get_current_context())->gc_is_on_screen && \
        (GrMouseInfo->docheck)) ?                                                  \
    (*GrMouseInfo->block)((c),(x1),(y1),(x2),(y2)) :                               \
    0                                                                               \
)
#define grx_mouse_unblock(f) do {                                              \
    if((f) && GrMouseInfo->displayed) {                                       \
        (*GrMouseInfo->unblock)((f));                                         \
    }                                                                          \
} while(0)
#endif  /* GRX_SKIP_INLINES */

#endif /* __GRX_INPUT_H__ */
