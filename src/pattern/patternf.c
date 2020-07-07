/**
 ** patternf.c ---- data structure for standard pattern filler
 **
 ** Copyright (c) 1998 Hartmut Schirmer
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

static void pixel_pf(int x, int y, GrFillArg fval)
{
    _GrPatternFilledPlotExt(x, y, 0, 0, fval.p);
}

static void line_pf(int x1, int y1, int dx, int dy, GrFillArg fval)
{
    _GrPatternFilledLineExt(x1, y1, dx, dy, 0, 0, fval.p);
}

static void scan_pf(int x, int y, int width, GrFillArg fval)
{
    _GrFillPatternExt(x, y, 0, 0, width, fval.p);
}

GrFiller _GrPatternFiller = {
    pixel_pf,
    line_pf,
    scan_pf
};
