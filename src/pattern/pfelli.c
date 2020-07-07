/**
 ** pfelli.c
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
 ** 200624 M.Alvarez, adding GrPatAlignFilledEllipse
 **
 **/

#include "libgrx.h"
#include "clipping.h"
#include "shapes.h"

void GrPatternFilledEllipse(int xc,int yc,int xa,int ya,GrPattern *p)
{
    GrFillArg fa;

    fa.p = p;
    _GrScanEllipse(xc,yc,xa,ya,&_GrPatternFiller,fa,TRUE);
}

void GrPatAlignFilledEllipse(int xo,int yo,int xc,int yc,int xa,int ya,GrPattern *p)
{
    GrFillArg fa;

    //xo = min(xo, xc-xa);
    //yo = min(yo, yc-ya);
    xo += CURC->gc_xoffset;
    yo += CURC->gc_yoffset;

    fa.pa.p = p;
    fa.pa.xo = xo;
    fa.pa.yo = yo;
    _GrScanEllipse(xc,yc,xa,ya,&_GrPatternAlignFiller,fa,TRUE);
}
