/**
 ** CIRCLE3.C ---- draw circle
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "shapes.h"

void GrCircle(int xc,int yc,int r,GrColor c)
{
        GrEllipse(xc,yc,r,r,c);
}
