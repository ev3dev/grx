/*
**  <iplot.c>  - Source Image Utility
**               by Michal Stencl Copyright (c) 1998
**
**
**  <e-mail>    - [stenclpmd@ba.telecom.sk]
**
** modifications by Hartmut Schirmer (c) 1998
*/

#include "libgrx.h"
#include "clipping.h"
#include "image/image.h"

void GrImagePlotAlign(int xo,int yo,int x,int y,GrImage *p)
{
   int xp, yp;
   GrColor col;

   xo = min(xo, x);
   yo = min(yo, y);
   clip_dot(CURC,x,y);
   xp = (x - xo) % p->pxp_width;
   yp = (y - yo) % p->pxp_height;
   mouse_block(CURC,x,y,x,y);
   col = (*p->pxp_source.gf_driver->readpixel)(&p->pxp_source,xp,yp);
   (*CURC->gc_driver->drawpixel)(x + CURC->gc_xoffset, y + CURC->gc_yoffset, col);
   mouse_unblock();
}
