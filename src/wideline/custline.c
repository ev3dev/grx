/**
 ** CUSTLINE.C ---- wide and/or dashed line 
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "shapes.h"

void GrCustomLine(int x1,int y1,int x2,int y2,GrLineOption *o)
{
        GrFillArg fval;
        int pt[2][2];
        pt[0][0] = x1; pt[0][1] = y1;
        pt[1][0] = x2; pt[1][1] = y2;
        fval.color = o->lno_color;
        _GrDrawCustomPolygon(2,pt,o,&_GrSolidFiller,fval,FALSE,FALSE);
}

