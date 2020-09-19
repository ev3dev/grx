/*
 * patfline.c
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
 *
 *  Copyright (C) 1992, Csaba Biegl
 *    820 Stirrup Dr, Nashville, TN, 37221
 *    csaba@vuse.vanderbilt.edu
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

#include "clipping.h"
#include "globals.h"
#include "libgrx.h"
#include "mouse.h"
#include "shapes.h"

void _GrPatternFilledLine(int x1, int y1, int dx, int dy, GrxPixmap *p)
{
    union {
        GrxFrame *c;
        unsigned char *b;
    } src;
    int sy, ymajor;
    int pw, ph, px, py;
    int points, error;

    if (dx < 0) {
        x1 += dx;
        dx = -dx;
        y1 += dy;
        dy = -dy;
    }
    if (dy == 0) {
        /*int check_if_offsets_correct;*/
        _GrFillPattern(x1, y1, dx + 1, p);
        return;
    }
    if (dy >= 0) {
        sy = 1;
    }
    else {
        dy = (-dy);
        sy = (-1);
    }

    pw = p->width;
    ph = p->height;
    px = x1 % pw;
    py = y1 % ph;
    src.c = &p->source;

    if (dy > dx) {
        points = dy + 1;
        error = dy >> 1;
        ymajor = TRUE;
    }
    else {
        points = dx + 1;
        error = dx >> 1;
        ymajor = FALSE;
    }
    while (--points >= 0) {
        (*CURC->gc_driver->drawpixel)(
            x1, y1, (*src.c->driver->readpixel)(src.c, px, py));
        if (ymajor) {
            if ((error -= dx) < 0)
                error += dy, x1++, px++;
            y1 += sy, py += sy;
        }
        else {
            if ((error -= dy) < 0)
                error += dx, y1 += sy, py += sy;
            x1++, px++;
        }
        if ((unsigned)py >= (unsigned)ph) {
            if (py < 0)
                py += ph;
            else
                py -= ph;
        }
        if (px >= pw)
            px = 0;
    }
}

/**
 * grx_draw_filled_line_with_pixmap:
 * @x1: starting X coordinate
 * @y1: starting Y coordinate
 * @x2: ending X coordinate
 * @y2: ending Y coordinate
 * @p: the pixmap
 *
 * Draws a line on the current context from the starting coordinates to the
 * ending coordinates using the specified pixmap.
 *
 * For horizontal and vertical lines, see grx_draw_hline() and grx_draw_vline().
 */
void grx_draw_filled_line_with_pixmap(int x1, int y1, int x2, int y2, GrxPixmap *p)
{
    clip_line(CURC, x1, y1, x2, y2);
    mouse_block(CURC, x1, y1, x2, y2);
    _GrPatternFilledLine(x1 + CURC->x_offset, y1 + CURC->x_offset, x2 - x1, y2 - y1, p);
    mouse_unblock();
}
