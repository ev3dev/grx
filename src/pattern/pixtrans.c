/**
 ** pixtrans.c ---- Pixmap trandformations
 **
 ** Copyright (c) 1998 Michal Stencl [stenclpmd@ba.telecom.sk]
 ** Copyright (c) 2020 M.Alvarez [malfer at telefonica.net]
 **
 ** This file is part of the GRX graphics library.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 **/

#include "libgrx.h"
#include "grdriver.h"

GrPixmap *GrPixmapInverse(GrPixmap *p,int flag)
{
    GrContext ctx, save;
    GrColor col;
    GrPixmap *pix;
    int yy, xx, sidex, sidey, width, height, xs, ys = 0;

    width = p->pxp_width;
    height = p->pxp_height;
    pix = _GrCreatePixmap(&ctx,width,height);
    if ( !pix ) return NULL;

    save = *CURC;
    *CURC = ctx;
    sidex = (flag & GR_PIXMAP_INVLR) ? -1 : 1;
    sidey = (flag & GR_PIXMAP_INVTD) ? -1 : 1;
    yy    = (flag & GR_PIXMAP_INVTD) ? height-1 : 0;
    do {
        xx = (flag & GR_PIXMAP_INVLR) ? width-1 : 0;
        xs = 0;
        do {
            col = (*p->pxp_source.gf_driver->readpixel)(&p->pxp_source,xs,ys);
            (*CURC->gc_driver->drawpixel)(xx, yy, col);
            xx += sidex;
        } while(++xs < width);
        yy += sidey;
    } while(++ys < height);

    *CURC = save;
    return pix;
}

GrPixmap *GrPixmapStretch(GrPixmap *p,int nwidth,int nheight)
{
    GrContext ctx;
    GrPixmap *pix;

    GRX_ENTER();
    pix = _GrCreatePixmap(&ctx,nwidth,nheight);
    if (pix)
        _GrFrDrvGenericStretchBlt(&ctx.gc_frame,0,0,nwidth,nheight,
                              &p->pxp_source,0,0,p->pxp_width,p->pxp_height,
                              p->pxp_oper);
    GRX_RETURN(pix);
}
