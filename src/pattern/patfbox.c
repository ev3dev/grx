/*
 * patfbox.c
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

#include "arith.h"
#include "clipping.h"
#include "globals.h"
#include "libgrx.h"
#include "mouse.h"
#include "shapes.h"

/**
 * grx_draw_filled_box_with_pixmap:
 * @x1: the left X coordinate
 * @y1: the top Y coordinate
 * @x2: the right X coordinate
 * @y2: the bottom Y coordinate
 * @p: the pixmap
 *
 * Draws a filled rectangle on the current context using the specified
 * coordinates and pixmap.
 */
void grx_draw_filled_box_with_pixmap(int x1, int y1, int x2, int y2, GrxPixmap *p)
{
    int width, height;

    clip_box(CURC, x1, y1, x2, y2);
    mouse_block(CURC, x1, y1, x2, y2);
    width = x2 - x1 + 1;
    height = y2 - y1 + 1;
    x1 += CURC->x_offset;
    y1 += CURC->y_offset;

    void (*bltfun)(
        const GrxFrame *, int, int, const GrxFrame *, int, int, int, int, GrxColor);
    int pwdt = p->width;
    int phgt = p->height;
    int xoff = x1 % pwdt;
    int ypos = y1;
    int yoff = ypos % phgt;
    if (CURC->gc_is_on_screen) {
        bltfun = CURC->gc_driver->bltr2v;
    }
    else {
        bltfun = CURC->gc_driver->bitblt;
    }
    while (height > 0) {
        int fillh = MIN(height, phgt - yoff);
        int linewdt = width;
        int xpos = x1;
        int xcuroff = xoff;
        while (linewdt > 0) {
            int fillw = MIN(linewdt, pwdt - xcuroff);
            (*bltfun)(&CURC->frame, xpos, ypos, &p->source, xcuroff, yoff, fillw, fillh,
                p->mode);
            linewdt -= fillw;
            xpos += fillw;
            xcuroff = 0;
        }
        height -= fillh;
        ypos += fillh;
        yoff = 0;
    }

    mouse_unblock();
}
