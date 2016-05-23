/*
 * iinverse.c ---- Source Image Utility
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

/**
 * grx_image_mirror:
 * @image: the source image
 * @flags: the mirror flags
 *
 * Creates a new image, flipping left-right and/or top-bottom as indicated by
 * @flags.
 *
 * Returns: (transfer full): the new image
 */
GrxImage *grx_image_mirror(GrxImage *p, GrxImageMirrorFlags flag)
{
  GrxContext  ctx, save;
  GrxColor    col;
  GrxImage   *img;
  int yy, xx, sidex, sidey, width, height, xs, ys = 0;
  width = p->width;
  height = p->height;
  img = _GrImageAllocate(&ctx,width,height);
  if ( !img ) return(NULL);
  save = *CURC;
  *CURC = ctx;
  sidex  = ( flag & GRX_IMAGE_MIRROR_HORIZONTAL ) ? -1 : 1;
  sidey  = ( flag & GRX_IMAGE_MIRROR_VERTICAL ) ? -1 : 1;
  yy     = ( flag & GRX_IMAGE_MIRROR_VERTICAL ) ? height-1 : 0;
  do {
    xx = ( flag & GRX_IMAGE_MIRROR_HORIZONTAL ) ? width-1 : 0;
    xs = 0;
    do {
      col = (*p->source.driver->readpixel)(&p->source,xs,ys);
      (*CURC->gc_driver->drawpixel)(xx, yy, col);
      xx += sidex;
    } while(++xs < width);
    yy += sidey;
  } while(++ys < height);
  *CURC = save;
  img->is_pixmap = 1;
  img->width  = width;
  img->height = height;
  img->mode   = 0;
  img->source = ctx.frame;
  img->source.memory_flags =  3;/* MY_CONTEXT & MY_MEMORY */
  return(img);
}
