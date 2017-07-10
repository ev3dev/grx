/*
 * iplot.c ---- Source Image Utility
 *
 * by Michal Stencl Copyright (c) 1998
 * <e-mail>    - [stenclpmd@ba.telecom.sk]
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
 *
 * modifications by Hartmut Schirmer (c) 1998
 */

#include "globals.h"
#include "mouse.h"
#include "libgrx.h"
#include "clipping.h"
#include "image.h"

/**
 * grx_draw_pixel_with_offset_pixmap:
 * @x0: the alignment X coordinate
 * @y0: the alignment Y coordinate
 * @x: the X coordinate
 * @y: the Y coordinate
 * @p: the pixmap
 *
 * Draws a single pixel of an pixmap on the current context at @x, @y.
 *
 * The color of the pixel comes from the pixmap as if @p was tiled over the
 * entrire context starting with the top/left of the pixmap at @x0, @y0.
 */
void grx_draw_pixel_with_offset_pixmap(int xo,int yo,int x,int y,GrxPixmap *p)
{
   int xp, yp;
   GrxColor col;

   xo = min(xo, x);
   yo = min(yo, y);
   clip_dot(CURC,x,y);
   xp = (x - xo) % p->width;
   yp = (y - yo) % p->height;
   mouse_block(CURC,x,y,x,y);
   col = (*p->source.driver->readpixel)(&p->source,xp,yp);
   (*CURC->gc_driver->drawpixel)(x + CURC->x_offset, y + CURC->y_offset, col);
   mouse_unblock();
}
