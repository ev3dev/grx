/**
 ** FILLCIR1.C ---- filled circle
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "shapes.h"

void GrFilledCircle(int xc,int yc,int r,GrColor c)
{
        GrFilledEllipse(xc,yc,r,r,c);
}
