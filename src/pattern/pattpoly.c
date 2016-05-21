/*
 * pattpoly.c
 *
 * Copyright (C) 1997, Michael Goffioul
 * [e-mail : goffioul@emic.ucl.ac.be]
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
 *
 *  Copyright (C) 1992, Csaba Biegl
 *    820 Stirrup Dr, Nashville, TN, 37221
 *    csaba@vuse.vanderbilt.edu
 *
 * This file is part of the GRX graphics library.
 *
 * The GRX graphics library is free software; you can redistribute it
 * and/or modify it under some conditions; see the "copying.grx" file
 * for details.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <grx/draw.h>

#include "libgrx.h"
#include "shapes.h"

void grx_draw_polyline_with_pattern(int numpts,GrxPoint *points,GrxLinePattern *lp)
{
        GrFillArg fval;

        fval.p = lp->pattern;
        _GrDrawCustomPolygon(numpts,points,lp->options,
                             &_GrPatternFiller,fval,FALSE,FALSE);
}

void grx_draw_polygon_with_pattern(int numpts,GrxPoint *points,GrxLinePattern *lp)
{
        GrFillArg fval;

        fval.p = lp->pattern;
        _GrDrawCustomPolygon(numpts,points,lp->options,
                             &_GrPatternFiller,fval,TRUE,FALSE);
}

void grx_draw_box_with_pattern(int x1,int y1,int x2,int y2,GrxLinePattern *lp)
{
        GrFillArg fval;
        GrxPoint points[4];

        points[0].x = x1; points[0].y = y1;
        points[1].x = x1; points[1].y = y2;
        points[2].x = x2; points[2].y = y2;
        points[3].x = x2; points[3].y = y1;
        fval.p = lp->pattern;
        _GrDrawCustomPolygon(4,points,lp->options,
                             &_GrPatternFiller,fval,TRUE,FALSE);
}
