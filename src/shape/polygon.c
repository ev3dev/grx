/*
 * POLYGON.C ---- draw a closed polygon
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu] See "copying.grx" for details.
 */

#include "libgrx.h"
#include "shapes.h"

void grx_draw_polygon(int n,int pt[][2],GrxColor c)
{
        GrFillArg fval;
        fval.color = c;
        _GrDrawPolygon(n,pt,&_GrSolidFiller,fval,TRUE);
}
