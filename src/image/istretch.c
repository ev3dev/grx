/*
 * istretch.c ---- Source Image Utility
 *
 * Copyright (c) 1998 by Michal Stencl and Hartmut Schirmer
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

#include "libgrx.h"
#include "grdriver.h"
#include "clipping.h"
#include "image/image.h"

/**
 * grx_image_stretch:
 * @image:
 * @new_width:
 * @new_height:
 *
 * Creates a new image stretching p to @new_width by @new_height.
 *
 * Returns: (transfer full):
 */
GrxImage *grx_image_stretch(GrxImage *p,int nwidth,int nheight)
{
  GrxContext  ctx;
  GrxImage   *img;

  GRX_ENTER();
  img = _GrImageAllocate(&ctx,nwidth,nheight);
  if ( img )
    _GrFrDrvGenericStretchBlt(&ctx.frame,0,0,nwidth,nheight,
                              &p->source,0,0,p->width,p->height,
                              p->mode);
  GRX_RETURN(img);
}
