/**
 ** CIRCLE.C ---- draw circle arc
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "shapes.h"

void GrCircleArc(int xc,int yc,int r,int start,int end,int style,GrColor c)
{
        GrEllipseArc(xc,yc,r,r,start,end,style,c);
}

