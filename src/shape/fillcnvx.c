/**
 ** FILLCNVX.C ---- fill a convex polygon with a solid color
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "shapes.h"

void GrFilledConvexPolygon(int n,int pt[][2],GrColor c)
{
        GrFillArg fval;
        fval.color = c;
        _GrScanConvexPoly(n,pt,&_GrSolidFiller,fval);
}
