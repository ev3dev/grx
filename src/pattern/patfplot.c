/**
 ** patfplot.c
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu]
 **
 **  Copyright (C) 1992, Csaba Biegl
 **    820 Stirrup Dr, Nashville, TN, 37221
 **    csaba@vuse.vanderbilt.edu
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
 ** 200623 M.Alvarez, adding GrPatAlignFilledPlot
 **
 **/

#include "libgrx.h"
#include "clipping.h"

void _GrPatternFilledPlotExt(int x,int y,int sx,int sy,GrPattern *p)
{
    int xp,yp;
    
    if(p->gp_ptype == GR_PTYPE_PIXMAP) {
        xp = (x-sx) % p->gp_pxp_width;
        yp = (y-sy) % p->gp_pxp_height;
        if (xp < 0) xp += p->gp_pxp_width;
        if (yp < 0) yp += p->gp_pxp_height;
        (*CURC->gc_driver->drawpixel)(x,y,
            (*p->gp_pxp_source.gf_driver->readpixel)(&p->gp_pxp_source,xp,yp)
        );
    }
    else if(p->gp_ptype == GR_PTYPE_BITMAP) {
        xp = x & 7;
        yp = y % p->gp_bmp_height;
        (*CURC->gc_driver->drawpixel)(x,y,
            (p->gp_bmp_data[yp] & (0x80U >> xp)) ? p->gp_bmp_fgcolor : p->gp_bmp_bgcolor
        );
    }
    else if(p->gp_ptype == GR_PTYPE_GRADIENT) {
        GrColor pixel;

        if (p->gp_grd_mode == GR_RADIAL_GRADIENT) {
            pixel = _GrRadGradientColor((GrGradient *)p, x, y, sx, sy);
        } else {
            pixel = _GrLinGradientColor((GrGradient *)p, x, y, sx, sy);
        }
        (*CURC->gc_driver->drawpixel)(x, y, pixel);
    }
}

void GrPatternFilledPlot(int x,int y,GrPattern *p)
{
    clip_dot(CURC,x,y);
    mouse_block(CURC,x,y,x,y);
    _GrPatternFilledPlotExt(x+CURC->gc_xoffset,y+CURC->gc_yoffset,0,0,p);
    mouse_unblock();
}

void GrPatAlignFilledPlot(int xo,int yo,int x,int y,GrPattern *p)
{
    //xo = min(xo, x);
    //yo = min(yo, y);
    clip_dot(CURC,x,y);
    mouse_block(CURC,x,y,x,y);
    _GrPatternFilledPlotExt(x+CURC->gc_xoffset,y+CURC->gc_yoffset,
                            xo+CURC->gc_xoffset,yo+CURC->gc_yoffset,p);
    mouse_unblock();
}
