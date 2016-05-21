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
 *
 */

#include "globals.h"
#include "mouse.h"
#include "libgrx.h"
#include "clipping.h"
#include "image.h"

void grx_draw_pixel_with_image(int xo,int yo,int x,int y,GrxImage *p)
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
