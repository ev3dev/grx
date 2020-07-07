/**
 ** pataligf.c ---- data structure for pattern align filler
 **
 ** Copyright (c) 2020 Mariano Alvarez Fernandez
 ** [e-mail: malfer@telefonica.net]
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
 ** 200623 M.Alvarez, rewriting
 **
 **/

#include "libgrx.h"
#include "shapes.h"

static void pixel_paf(int x, int y, GrFillArg fval)
{
    _GrPatternFilledPlotExt(x, y, fval.pa.xo, fval.pa.yo, fval.pa.p);
}

static void line_paf(int x1, int y1, int dx, int dy, GrFillArg fval)
{
    _GrPatternFilledLineExt(x1, y1, dx, dy, fval.pa.xo, fval.pa.yo, fval.pa.p);
}

static void scan_paf(int x, int y, int width, GrFillArg fval)
{
    _GrFillPatternExt(x, y, fval.pa.xo, fval.pa.yo, width, fval.pa.p);
}

GrFiller _GrPatternAlignFiller = {
    pixel_paf,
    line_paf,
    scan_paf
};
