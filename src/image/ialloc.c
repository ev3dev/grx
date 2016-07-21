/*
 * ialloc.c ---- Source Image Utility
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
 * modifications by Hartmut Schirmer Copyright (c) 1998
 */

#include <grx/mode.h>

#include "libgrx.h"
#include "allocate.h"
#include "image.h"

int _GrImageTestSize(int wdt,int hgt)
{
  long total;
  GRX_ENTER();
  total = grx_screen_get_context_size(wdt,hgt);
# ifdef _MAXMEMPLANESIZE
  if ( total > _MAXMEMPLANESIZE ) total = 0L;
# endif
  GRX_RETURN(total);
}

GrxImage *_GrImageAllocate(GrxContext *ctx, int nwidth,int nheight)
{
  GrxImage   *img;

  GRX_ENTER();
  img = NULL;
  if ( _GrImageTestSize(nwidth, nheight) <= 0 ) goto done;
  if (!grx_context_new(nwidth, nheight, NULL, ctx)) goto done;
  img = (GrxImage *)malloc(sizeof(GrxImage));
  if ( !img ) {
    grx_context_unref(ctx);
    goto done;
  }
  img->is_pixmap = 1;
  img->width  = nwidth;
  img->height = nheight;
  img->mode   = 0;
  img->source = ctx->frame;
  img->source.memory_flags =  3;/* MY_CONTEXT & MY_MEMORY */
done:
  GRX_RETURN(img);
}
