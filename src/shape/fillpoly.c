/**
 ** FILLPOLY.C ---- fill an arbitrary polygon with a solid color
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "shapes.h"

void GrFilledPolygon(int n,int pt[][2],long c)
{
        GrFillArg fval;
        fval.color = c;
        _GrScanPolygon(n,pt,&_GrSolidFiller,fval);
}
