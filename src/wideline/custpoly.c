/**
 ** CUSTPOLY.C ---- draw a closed dashed and/or wide polygon
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "shapes.h"

void GrCustomPolygon(int n,int pt[][2],GrLineOption *o)
{
        GrFillArg fval;
        fval.color = o->lno_color;
        _GrDrawCustomPolygon(n,pt,o,&_GrSolidFiller,fval,TRUE,FALSE);
}
