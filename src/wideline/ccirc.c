/**
 ** CCIRC.C ---- draw dashed and/or wide circles
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "shapes.h"

void GrCustomCircle(int xc,int yc,int r,GrLineOption *o)
{
        GrCustomEllipse(xc,yc,r,r,o);
}
