/*
 * patfplot.c
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

#include <grx/pixmap.h>

#include "globals.h"
#include "libgrx.h"
#include "clipping.h"
#include "mouse.h"

void _GrPatternFilledPlot(int x,int y,GrxPixmap *p)
{
    int xp, yp;

    xp = x % p->width;
    yp = y % p->height;
    (*CURC->gc_driver->drawpixel)(x, y,
        (*p->source.driver->readpixel)(&p->source,xp, yp)
    );
}

/**
 * grx_draw_filled_pixel_with_pixmap:
 * @x: the X coordinate
 * @y: the Y coordinate
 * @p: the pixmap
 *
 * Draw a single pixel on the current context at the specified coordinates.
 */
void grx_draw_filled_pixel_with_pixmap(int x,int y,GrxPixmap *p)
{
        clip_dot(CURC,x,y);
        mouse_block(CURC,x,y,x,y);
        _GrPatternFilledPlot(x+CURC->x_offset,y+CURC->y_offset, p);
        mouse_unblock();
}
