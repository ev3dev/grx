/**
 ** FLOODFIL.C ---- fill an arbitrary area with a solid color
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
#include "shapes.h"

void GrFloodFill(int x, int y, GrColor border, GrColor c)
{
        GrFillArg fval;
        fval.color = c;
        _GrFloodFill(x,y,border,&_GrSolidFiller,fval);
}
