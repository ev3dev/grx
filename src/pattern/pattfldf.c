/**
 ** pattfldf.c ---- fill an arbitrary area with a pattern
 **
 ** Copyright (c) 1997 Hartmut Schirmer
 ** [e-mail: hsc@techfak.uni-kiel.de]
 **
 ** This file is part of the GRX graphics library.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** 200624 M.Alvarez, adding GrPatAlignFloodFill
 **
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

void GrPatAlignFloodFill(int xo,int yo,int x, int y, GrColor border, GrPattern *p)
{
    GrFillArg fa;

    xo += CURC->gc_xoffset;
    yo += CURC->gc_yoffset;

    fa.pa.p = p;
    fa.pa.xo = xo;
    fa.pa.yo = yo;
    _GrFloodFill(x,y,border,&_GrPatternAlignFiller,fa);
}
