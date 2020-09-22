/*
 * mscursor.c ---- the mouse cursor related stuff
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

#include <stdarg.h>

#include <grx/device.h>
#include <grx/draw.h>

#include "clipping.h"
#include "globals.h"
#include "input.h"
#include "libgrx.h"
#include "mempeek.h"

#define MSCURSOR    (MOUINFO->cursor)
#define SPECIALMODE (MOUINFO->cursor_mode != GRX_CURSOR_MODE_NORMAL)
#define BLOCKED     1
#define ERASED      2

static void draw_special(void)
{
    int xpos = MSCURSOR->xcord;
    int ypos = MSCURSOR->ycord;
    int check = MOUINFO->docheck;
    GrxContext csave;
    MOUINFO->docheck = FALSE;
    grx_save_current_context(&csave);
    grx_set_current_context(SCRN);
    switch (MOUINFO->cursor_mode) {
    case GRX_CURSOR_MODE_RUBBER:
        grx_draw_box(xpos, ypos, MOUINFO->x1, MOUINFO->y1, MOUINFO->curscolor);
        break;
    case GRX_CURSOR_MODE_LINE:
        grx_draw_line(xpos, ypos, MOUINFO->x1, MOUINFO->y1, MOUINFO->curscolor);
        break;
    case GRX_CURSOR_MODE_BOX:
        grx_draw_box((xpos + MOUINFO->x1), (ypos + MOUINFO->y1), (xpos + MOUINFO->x2),
            (ypos + MOUINFO->y2), MOUINFO->curscolor);
        break;
    default:
        break;
    }
    grx_set_current_context(&csave);
    MOUINFO->docheck = check;
}

static void move_mouse(void)
{
    if ((MOUINFO->xpos != MSCURSOR->xcord) || (MOUINFO->ypos != MSCURSOR->ycord)) {
        int check = MOUINFO->docheck;
        int dospec = SPECIALMODE && MSCURSOR->displayed;
        MOUINFO->docheck = FALSE;
        if (dospec)
            draw_special();
        grx_cursor_move(MSCURSOR, MOUINFO->xpos, MOUINFO->ypos);
        if (dospec)
            draw_special();
        MOUINFO->docheck = check;
    }
}

static void draw_mouse(void)
{
    int check = MOUINFO->docheck;
    MOUINFO->docheck = FALSE;
    grx_cursor_show(MSCURSOR);
    if (SPECIALMODE)
        draw_special();
    MOUINFO->docheck = check;
}

static void erase_mouse(void)
{
    int check = MOUINFO->docheck;
    MOUINFO->docheck = FALSE;
    if (SPECIALMODE)
        draw_special();
    grx_cursor_hide(MSCURSOR);
    MOUINFO->docheck = check;
}

static void enable_mouse_cursor(void)
{
    if (!MOUINFO->cursor) {
        return;
    }
    if (MOUINFO->enabled) {
        return;
    }
    MOUINFO->enabled = TRUE;
    if (MOUINFO->displayed) {
        return;
    }
    MOUINFO->docheck = TRUE;
    MOUINFO->blockflag = 0;
}

static void disable_mouse_cursor(void)
{
    if (!MOUINFO->cursor) {
        return;
    }
    if (!MOUINFO->enabled) {
        return;
    }
    if (MOUINFO->blockflag) {
        return;
    }
    MOUINFO->enabled = FALSE;
    if (!MOUINFO->displayed) {
        return;
    }
    MOUINFO->displayed = FALSE;
    MOUINFO->docheck = FALSE;
    erase_mouse();
}

/**
 * grx_mouse_set_cursor:
 * @cursor: (nullable): the cursor
 *
 * Sets the mouse cursor to the specified cursor.
 */
void grx_mouse_set_cursor(GrxCursor *cursor)
{
    if (cursor == MSCURSOR) {
        return;
    }

    if (MOUINFO->cursor) {
        grx_cursor_unref(MOUINFO->cursor);
        MOUINFO->cursor = NULL;
    }

    if (cursor) {
        g_return_if_fail(COMPATIBLE(cursor));

        if (cursor->displayed) {
            grx_cursor_hide(cursor);
        }
        MOUINFO->cursor = grx_cursor_ref(cursor);
    }
}

/**
 * grx_mouse_set_cursor_default:
 * @fill_color: the cursor fill color
 * @border_color: the cursor border color
 *
 * Sets the mouse cursor to a default pointer shape.
 */
void grx_mouse_set_cursor_default(GrxColor fill_color, GrxColor border_color)
{
    // clang-format off
    static const guint8 ptr12x16bits[] = {
        1,1,0,0,0,0,0,0,0,0,0,0,
        1,2,1,0,0,0,0,0,0,0,0,0,
        1,2,2,1,0,0,0,0,0,0,0,0,
        1,2,2,2,1,0,0,0,0,0,0,0,
        1,2,2,2,2,1,0,0,0,0,0,0,
        1,2,2,2,2,2,1,0,0,0,0,0,
        1,2,2,2,2,2,2,1,0,0,0,0,
        1,2,2,2,2,2,2,2,1,0,0,0,
        1,2,2,2,2,2,2,2,2,1,0,0,
        1,2,2,2,2,2,2,2,2,2,1,0,
        1,2,2,2,2,2,2,2,2,2,2,1,
        1,2,2,2,2,1,1,1,1,1,1,1,
        1,2,2,2,1,0,0,0,0,0,0,0,
        1,2,2,1,0,0,0,0,0,0,0,0,
        1,2,1,0,0,0,0,0,0,0,0,0,
        1,1,0,0,0,0,0,0,0,0,0,0,
    };
    // clang-format on

    GrxCursor *newc;
    GArray *cols;

    if (MOUINFO->displayed) {
        return;
    }

    cols = g_array_sized_new(FALSE, FALSE, sizeof(GrxColor), 2);
    g_array_append_val(cols, border_color);
    g_array_append_val(cols, fill_color);
    newc = grx_cursor_new(ptr12x16bits, 12, 12, 16, 1, 1, cols);
    g_array_unref(cols);
    if (!newc) {
        return;
    }
    grx_mouse_set_cursor(newc);
    grx_cursor_unref(newc);
}

/**
 * grx_mouse_set_cursor_mode: (skip)
 * @mode: the mode
 * @...: mode specific arguments
 *
 * Sets the mouse cursor mode.
 */
void grx_mouse_set_cursor_mode(int mode, ...)
{
    va_list ap;
    if (MOUINFO->displayed)
        return;
    va_start(ap, mode);
    switch (mode) {
    case GRX_CURSOR_MODE_BOX:
        MOUINFO->x2 = va_arg(ap, int);
        MOUINFO->y2 = va_arg(ap, int);
    case GRX_CURSOR_MODE_RUBBER:
    case GRX_CURSOR_MODE_LINE:
        MOUINFO->cursor_mode = mode;
        MOUINFO->x1 = va_arg(ap, int);
        MOUINFO->y1 = va_arg(ap, int);
        MOUINFO->curscolor = grx_color_to_xor_mode(va_arg(ap, GrxColor));
        break;
    default:
        MOUINFO->cursor_mode = GRX_CURSOR_MODE_NORMAL;
        break;
    }
    va_end(ap);
}

static int block(GrxContext *c, int x1, int y1, int x2, int y2)
{
    int mx1, my1, mx2, my2, oldblock = MOUINFO->blockflag;
    if (!c)
        c = CURC;
    if (!MOUINFO->displayed)
        return 0;
    if (!MOUINFO->docheck)
        return 0;
    if (!c->gc_is_on_screen)
        return 0;
    if (oldblock & ERASED)
        return 0;
    MOUINFO->blockflag = BLOCKED;
    isort(x1, x2);
    x1 += c->x_offset;
    x2 += c->x_offset;
    isort(y1, y2);
    y1 += c->y_offset;
    y2 += c->y_offset;
    mx1 = MSCURSOR->xwpos;
    my1 = MSCURSOR->ywpos;
    mx2 = MSCURSOR->xwork + mx1 - 1;
    my2 = MSCURSOR->ywork + my1 - 1;
    if (SPECIALMODE) {
        int cx1, cy1, cx2, cy2;
        switch (MOUINFO->cursor_mode) {
        case GRX_CURSOR_MODE_RUBBER:
        case GRX_CURSOR_MODE_LINE:
            cx1 = MSCURSOR->xcord;
            cy1 = MSCURSOR->ycord;
            cx2 = MOUINFO->x1;
            cy2 = MOUINFO->y1;
            break;
        case GRX_CURSOR_MODE_BOX:
            cx1 = MSCURSOR->xcord + MOUINFO->x1;
            cy1 = MSCURSOR->ycord + MOUINFO->y1;
            cx2 = MSCURSOR->xcord + MOUINFO->x2;
            cy2 = MSCURSOR->ycord + MOUINFO->y2;
            break;
        default:
            return 0;
        }
        isort(cx1, cx2);
        mx1 = MIN(cx1, mx1);
        mx2 = MAX(mx2, cx2);
        isort(cy1, cy2);
        my1 = MIN(cy1, my1);
        my2 = MAX(my2, cy2);
    }
    x1 = MAX(x1, mx1);
    y1 = MAX(y1, my1);
    x2 = MIN(x2, mx2);
    y2 = MIN(y2, my2);
    if ((x1 <= x2) && (y1 <= y2)) {
        MOUINFO->blockflag = oldblock | ERASED;
        MOUINFO->docheck = FALSE;
        erase_mouse();
        return ERASED;
    }
    return (oldblock & BLOCKED) ? 0 : BLOCKED;
}

static void unblock(unsigned int flags)
{
    if (!MOUINFO->displayed)
        return;
    if (flags & MOUINFO->blockflag & ERASED) {
        if (!(MOUINFO->blockflag & BLOCKED) || (flags & BLOCKED)) {
            move_mouse();
            MOUINFO->blockflag &= ~BLOCKED;
        }
        draw_mouse();
        MOUINFO->blockflag &= ~ERASED;
        MOUINFO->docheck = TRUE;
        return;
    }
    if (flags & MOUINFO->blockflag & BLOCKED) {
        if (!(MOUINFO->blockflag & ERASED)) {
            int updflag;
            test_unblock(updflag);
            if (updflag) {
                move_mouse();
            }
        }
        MOUINFO->blockflag &= ~BLOCKED;
    }
}

void _grx_mouse_update_cursor(void)
{
    if (MOUINFO->blockflag) {
        return;
    }
    if G_UNLIKELY (MOUINFO->enabled && !MOUINFO->displayed) {
        MOUINFO->displayed = TRUE;
        draw_mouse();
    }
    if (MOUINFO->displayed) {
        move_mouse();
    }
}

static void handle_device_added(
    GrxDeviceManager *device_manager, GrxDevice *device, gpointer user_data)
{
    if (grx_device_get_has_pointer(device)) {
        if (MOUINFO->pointer_device_count == 0) {
            enable_mouse_cursor();
        }
        MOUINFO->pointer_device_count++;
    }
}

static void handle_device_removed(
    GrxDeviceManager *device_manager, GrxDevice *device, gpointer user_data)
{
    if (grx_device_get_has_pointer(device)) {
        MOUINFO->pointer_device_count--;
        if (MOUINFO->pointer_device_count == 0) {
            disable_mouse_cursor();
        }
    }
}

void _GrInitMouseCursor(void)
{
    if (MSCURSOR && !COMPATIBLE(MSCURSOR)) {
        GrxCursor *obsolete = MSCURSOR;
        MOUINFO->cursor = NULL;
        grx_cursor_unref(obsolete);
    }
    if (MSCURSOR == NULL) {
        grx_mouse_set_cursor_default(GRX_COLOR_WHITE, GRX_COLOR_BLACK);
    }
    if (MSCURSOR && MSCURSOR->displayed) {
        grx_cursor_hide(MSCURSOR);
    }
    if (MOUINFO->device_manager) {
        g_signal_handler_disconnect(
            MOUINFO->device_manager, MOUINFO->device_added_handler);
        g_signal_handler_disconnect(
            MOUINFO->device_manager, MOUINFO->device_removed_handler);
        g_object_unref(MOUINFO->device_manager);
        MOUINFO->device_added_handler = 0;
        MOUINFO->device_removed_handler = 0;
        MOUINFO->device_manager = NULL;
    }
    MOUINFO->cursor_mode = GRX_CURSOR_MODE_NORMAL;
    MOUINFO->displayed = FALSE;
    MOUINFO->blockflag = 0;
    MOUINFO->docheck = FALSE;
    MOUINFO->block = block;
    MOUINFO->unblock = unblock;
    MOUINFO->pointer_device_count = 0;
    MOUINFO->device_manager = grx_get_device_manager();
    if (MOUINFO->device_manager) {
        g_object_ref(MOUINFO->device_manager);
        MOUINFO->device_added_handler =
            g_signal_connect(G_OBJECT(MOUINFO->device_manager), "device-added",
                (GCallback)handle_device_added, NULL);
        MOUINFO->device_removed_handler =
            g_signal_connect(G_OBJECT(MOUINFO->device_manager), "device-removed",
                (GCallback)handle_device_removed, NULL);
    }
}
