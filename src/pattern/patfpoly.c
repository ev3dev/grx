/**
 ** patfpoly.c
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
 ** 200625 M.Alvarez, adding GrPatAlignFilledPolygon
 **
 **/

#include "libgrx.h"
#include "clipping.h"
#include "shapes.h"

void GrPatternFilledPolygon(int n,int pt[][2],GrPattern *p)
{
    GrFillArg fa;

    fa.p = p;
    if(n <= 3)
        _GrScanConvexPoly(n,pt,&_GrPatternFiller,fa);
    else 
        _GrScanPolygon(n,pt,&_GrPatternFiller,fa);
}

void GrPatAlignFilledPolygon(int xo,int yo,int n,int pt[][2],GrPattern *p)
{
    GrFillArg fa;
    //int i;

    //for (i=0; i<n; i++) {
    //    xo = min(xo, pt[i][0]);
    //    yo = min(yo, pt[i][1]);
    //}
    xo += CURC->gc_xoffset;
    yo += CURC->gc_yoffset;

    fa.pa.p = p;
    fa.pa.xo = xo;
    fa.pa.yo = yo;
    if(n <= 3)
        _GrScanConvexPoly(n,pt,&_GrPatternAlignFiller,fa);
    else 
        _GrScanPolygon(n,pt,&_GrPatternAlignFiller,fa);
}
