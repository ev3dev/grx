/*
 * bitblt1b.c ---- bitblt from a 1bpp context
 *
 * Copyright (c) 2001 Josu Onandia
 * [e-mail: jonandia@fagorautomation.es].
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
 */

#include "globals.h"
#include "mouse.h"
#include "libgrx.h"
#include "clipping.h"

/**
 * grx_context_bit_blt_1bpp:
 * @context: the destination context
 * @x: the destination X coordinate
 * @y: the destination Y coordinate
 * @src: (nullable): the source context or %NULL to use the current context
 * @x1: the source bounding rectangle left coordinate
 * @y1: the source bounding rectangle top coordinate
 * @x2: the source bounding rectangle right coordinate
 * @y2: the source bounding rectangle bottom coordinate
 * @fg: the foreground color
 * @bg: the background color
 *
 * Copies an area bounded by x1, y2, x2, y2 in the source context to the
 * destination context at the location specified by dx, dy using the specified
 * operation.
 *
 * The source must be a 1 bit per pixel bitmap. The foreground color is used
 * for bits = 1 and the background color is used for bits = 0.
 */
void grx_context_bit_blt_1bpp(GrxContext *dst,int dx,int dy,
                  GrxContext *src,int x1,int y1,int x2,int y2,
                  GrxColor fg, GrxColor bg)
{
  int oldx1,oldy1;
  int oldx2,oldy2;
  int dstx2,dsty2;

  if(dst == NULL) dst = CURC;
  if(src == NULL) src = CURC;
        
  if(src->gc_driver->num_planes != 1 || src->gc_driver->bits_per_pixel != 1)
    return;
        
  isort(x1,x2); oldx1 = x1;
  isort(y1,y2); oldy1 = y1;
  cxclip_ordbox(src,x1,y1,x2,y2);
  oldx1 = (dx  += (x1 - oldx1));
  oldy1 = (dy  += (y1 - oldy1));
  oldx2 = dstx2 = dx + x2 - x1;
  oldy2 = dsty2 = dy + y2 - y1;
  clip_ordbox(dst,dx,dy,dstx2,dsty2);
  x1 += (dx - oldx1);
  y1 += (dy - oldy1);
  x2 -= (oldx2 - dstx2);
  y2 -= (oldy2 - dsty2);
  mouse_block(src,x1,y1,x2,y2);
  mouse_addblock(dst,dx,dy,dstx2,dsty2);

  (dst->gc_driver->drawbitmap)((dx + dst->x_offset),(dy + dst->y_offset),
    (x2 - x1 + 1),(y2 - y1 + 1),src->gc_base_address.plane0,src->gc_line_offset,
    /*alex:the offset should anyway be the x1,y1 point in src, as clipped*/
    (x1 + (y1 * (src->gc_line_offset << 3))),fg,bg);

  mouse_unblock();
}
