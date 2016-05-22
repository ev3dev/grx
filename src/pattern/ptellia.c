/*
 * ptellia.c
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
#include "allocate.h"
#include "shapes.h"

void grx_draw_ellipse_arc_with_pattern(int xc,int yc,int rx,int ry,int start,int end,GrxArcStyle style,GrxLinePattern *lp)
{
    GrxPoint *points;
    setup_ALLOC();
    points = ALLOC(sizeof(GrxPoint) * (GRX_MAX_ELLIPSE_POINTS + 2));
    if (points != NULL)
    {
        int numpts = grx_generate_ellipse_arc(xc,yc,rx,ry,start,end,points);
        GrFillArg fval;
        int close;

        close = FALSE;
        if (style == GRX_ARC_STYLE_CLOSED_RADIUS) {
                points[numpts].x = xc;
                points[numpts].y = yc;
                numpts++;
                points[numpts].x = points[0].x;
                points[numpts].y = points[0].y;
                numpts++;
                close = TRUE;
        }
        if (style == GRX_ARC_STYLE_CLOSED_CHORD) {
                points[numpts].x = points[0].x;
                points[numpts].y = points[0].y;
                numpts++;
                close = TRUE;
        }
        fval.p = lp->pattern;
        _GrDrawCustomPolygon(numpts,points,lp->options,
                             &_GrPatternFiller,fval,close,TRUE);
        FREE(points);
    }
    reset_ALLOC();
}
