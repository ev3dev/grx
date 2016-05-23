/*
 * ifbox.c ---- Source Image Utility
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
#include "libgrx.h"
#include "clipping.h"
#include "image.h"
#include "mouse.h"

/**
 * grx_draw_filled_box_with_image:
 * @xo: the alignment X coordinate
 * @yo: the alignment Y coordinate
 * @x1: the top X coordinate
 * @y1: the left Y coordinate
 * @x2: the right X coordinate
 * @y2: the bottom Y coordinate
 * @image: the image
 *
 * Fills the rectangle @x1, @y1, @x2, @y2 in the current context with the image.
 * This image can be offset from the rectangle by specifying @xo, @yo different
 * from @x1, @y1. The image is tiled if needed to fill the rectangle.
 */
void grx_draw_filled_box_with_image(int xo,int yo,int x1,int y1,int x2,int y2,GrxImage *p)
{
  int iwdt, ihgt, xoff, yoff, yy, xx, copyh, copyw;
  void (*bltfun)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor);
  xo = min(xo, min(x1,x2));
  yo = min(yo, min(y1,y2));
  clip_box(CURC,x1,y1,x2,y2);
  iwdt = p->width;
  ihgt = p->height;
  if ( (y2-y1) <= 0 || (x2-x1) <= 0 || iwdt <= 0 || ihgt <= 0) return;
  if (CURC->gc_is_on_screen) bltfun = CURC->gc_driver->bltr2v;
  else                   bltfun = CURC->gc_driver->bitblt;
  while (xo > x1) xo -= iwdt;
  while (yo > y1) yo -= ihgt;
  yoff = (y1-yo)%ihgt;
  yy = y1;
  mouse_block(CURC,x1,y1,x2,y2);
  x2++;
  y2++;
  do {
    xx = x1;
    copyh = min(y2-yy,ihgt-yoff);
    xoff = (x1-xo)%iwdt;
    do {
      copyw = min(x2-xx,iwdt-xoff);
      (*bltfun)( &CURC->frame, xx + CURC->x_offset, yy + CURC->y_offset,
                 &p->source,xoff,yoff,copyw,copyh,
                 p->mode
      );
      xx += iwdt-xoff;
      xoff = 0;
    } while ( xx < x2 );
    yy += ihgt-yoff;
    yoff = 0;
  } while ( yy < y2 );
  mouse_unblock();
}

/**
 * grx_draw_image:
 * @x: the left edge
 * @y: the top edge
 * @image: the image
 *
 * Draws the entire image on the current context with the top, left corner at
 * @x, @y.
 */
void  grx_draw_image(int x,int y,GrxImage *p)
{
  GRX_ENTER();
  grx_draw_filled_box_with_image(x,y,x,y,x+p->width-1,y+p->height-1,p);
  GRX_LEAVE();
}

/**
 * grx_draw_image_tiled:
 * @x1: the left edge
 * @y1: the top edge
 * @x2: the right edge
 * @y2: the bottom edge
 * @image: the image
 *
 * Draws as much of the image as possible (repeating if necessary) on the
 * current in the rectangle defined by @x1, @y1 and @x2, @y2
 */
void  grx_draw_image_tiled(int x1,int y1,int x2,int y2,GrxImage* p)
{
  GRX_ENTER();
  grx_draw_filled_box_with_image(x1,y1,x1,y1,x2,y2,p);
  GRX_LEAVE();
}
