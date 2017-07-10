/*
 * ihline.c ---- Source Image Utility
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
 * grx_draw_hline_with_offset_pixmap:
 * @x0: the alignment X coordinate
 * @y0: the alignment Y coordinate
 * @x: the top X coordinate
 * @y: the left Y coordinate
 * @width: the width of the line
 * @p: the pixmap
 *
 * Draws a single line of an pixmap on the current context starting at @x, @y.
 * The pixmap can be offset from the line by specifying @x0, @y0 different from
 * @x, @y. The pixmap is repeated if the width of the line is greater than the
 * width of the pixmap.
 */
void grx_draw_hline_with_offset_pixmap(int xo,int yo,int x,int y,int width,GrxPixmap *p)
{
  int x2, widthimg, yimg, ximg, xdest, ydest, cpysize;
  GrxColor optype;
  void (*bltfun)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor);
  x2 = x+width;
  xo = min(xo, min(x,x2));
  yo = min(yo, y);
  clip_hline(CURC,x,x2,y);
  width = x2 - x;
  mouse_block(CURC,x,y,x2,y);
  widthimg = p->width;
  yimg = (y - yo) % p->height;
  ximg = (x - xo) % widthimg;
  xdest = x + CURC->x_offset;
  ydest = y + CURC->y_offset;
  cpysize = widthimg - ximg;
  optype = p->mode;
  if (CURC->gc_is_on_screen) bltfun = CURC->gc_driver->bltr2v;
  else                   bltfun = CURC->gc_driver->bitblt;
  while ( width > 0 ) {
    if ( cpysize > width ) cpysize = width;
         (*bltfun)(&CURC->frame,xdest,ydest,
                   &p->source,ximg,yimg,cpysize,1,
                   optype);
    width -= cpysize;
    ximg = 0;
    xdest += cpysize;
    cpysize = widthimg;
  };
  mouse_unblock();
}
