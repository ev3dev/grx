/**
 ** PATTFLDF.C ---- fill an arbitrary area with a pattern
 **
 ** Copyright (c) 1997 Hartmut Schirmer
 ** [e-mail: hsc@techfak.uni-kiel.de]
 **
 ** The GRX copyright conditions by
 **   Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 **   [e-mail: csaba@vuse.vanderbilt.edu]
 ** still apply to this code. See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "clipping.h"
#include "shapes.h"

void GrPatternFloodFill(int x, int y, GrColor border, GrPattern *p)
{
        GrFillArg fval;

        fval.p = p;
        _GrFloodFill(x,y,border,&_GrPatternFiller,fval);
}
