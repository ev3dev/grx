/**
 ** CIRCLE1.C ---- draw ellipse
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "shapes.h"

void GrEllipse(int xc,int yc,int xa,int ya,GrColor c)
{
        GrFillArg fval;
        fval.color = c;
        _GrScanEllipse(xc,yc,xa,ya,&_GrSolidFiller,fval,FALSE);
}
