/*
 * pattline.c
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
 */

#include <grx/draw.h>

#include "libgrx.h"
#include "shapes.h"

/**
 * grx_draw_line_with_pattern:
 * @x1: starting X coordinate
 * @y1: starting Y coordinate
 * @x2: ending X coordinate
 * @y2: ending Y coordinate
 * @lp: the line pattern
 *
 * Draws a line on the current context from the starting coordinates to the
 * ending coordinates using the specified line pattern.
 */
void grx_draw_line_with_pattern(int x1,int y1,int x2,int y2,GrxLinePattern *lp)
{
        GrFillArg fval;
        GrxPoint points[2];

        points[0].x = x1;
        points[0].y = y1;
        points[1].x = x2;
        points[1].y = y2;
        fval.p = lp->pattern;
        _GrDrawCustomPolygon(2,points,lp->options,&_GrPatternFiller,fval,FALSE,FALSE);
}
