/*
**  <ialloc.c>  - Source Image Utility
**                  by Michal Stencl Copyright (c) 1998
**
**
**  <e-mail>    - [stenclpmd@ba.telecom.sk]
**
** modifications by Hartmut Schirmer Copyright (c) 1998
*/

#include "libgrx.h"
#include "allocate.h"
#include "image/image.h"

int _GrImageTestSize(int wdt,int hgt)
{
  long total;
  GRX_ENTER();
  total = GrContextSize(wdt,hgt);
# ifdef _MAXMEMPLANESIZE
  if ( total > _MAXMEMPLANESIZE ) total = 0L;
# endif
  GRX_RETURN(total);
}

GrImage *_GrImageAllocate(GrContext *ctx, int nwidth,int nheight)
{
  GrImage   *img;

  GRX_ENTER();
  img = NULL;
  if ( _GrImageTestSize(nwidth, nheight) <= 0 ) goto done;
  if (!GrCreateContext(nwidth, nheight, NULL, ctx)) goto done;
  img = (GrImage *)malloc(sizeof(GrImage));
  if ( !img ) {
    GrDestroyContext(ctx);
    goto done;
  }
  img->pxp_ispixmap = 1;
  img->pxp_width  = nwidth;
  img->pxp_height = nheight;
  img->pxp_oper   = 0;
  img->pxp_source = ctx->gc_frame;
  img->pxp_source.gf_memflags =  3;/* MY_CONTEXT & MY_MEMORY */
done:
  GRX_RETURN(img);
}
