/*
**  ibuild.c  - Source Image Utility
**              by Michal Stencl Copyright (c) 1998
**
**
**  <e-mail>    - [stenclpmd@ba.telecom.sk]
**
** modifications by Hartmut Schirmer (c) 1998
*/

#include "libgrx.h"
#include "clipping.h"
#include "mempeek.h"
#include "image/image.h"

GrImage *GrImageBuild(char *pixels,int w,int h,GrColorTableP colors)
{
  GrImage   *img;
  GRX_ENTER();
  img = NULL;
  if ( pixels ) do {
    GrContext  ctx, save;
    GrColor    col;
    int yy=0, xx;
    img = _GrImageAllocate(&ctx,w,h);
    if ( !img ) break;
    save = *CURC;
    *CURC = ctx;
    do {
      xx = 0;
      do {
        col = peek_b(pixels);
        ptrinc(pixels,1);
        if ( colors ) col = GR_CTABLE_COLOR(colors,col);
        (*CURC->gc_driver->drawpixel)(xx, yy, (col & C_COLOR));
      } while(++xx < w);
    } while(++yy < h);
    *CURC = save;
  } while(0);
  GRX_RETURN(img);
}
