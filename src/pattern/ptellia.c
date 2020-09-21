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
 */

#include <grx/draw.h>

#include "libgrx.h"
#include "shapes.h"

/**
 * grx_draw_ellipse_arc_with_pixmap:
 * @xc: the X coordinate of the center of the arc
 * @yc: the Y coordinate of the center of the arc
 * @rx: the radius in the X direction
 * @ry: the radius in the Y direction
 * @start: the starting angle in 1/10ths of degrees
 * @end: the ending angle in 1/10ths of degrees
 * @style: the arc style
 * @o: the line options
 * @p: the pixmap
 *
 * Draws an arc on the current context centered at the specified coordinates
 * from the starting angle to the ending angle with the specified radii,
 * arc style and line options and pixmap.
 */
void grx_draw_ellipse_arc_with_pixmap(int xc, int yc, int rx, int ry, int start,
    int end, GrxArcStyle style, GrxLineOptions *o, GrxPixmap *p)
{
    GArray *points;
    GrFillArg fval;
    GrxPoint pt;
    gboolean close = FALSE;

    points = grx_generate_ellipse_arc(xc, yc, rx, ry, start, end);

    switch (style) {
    case GRX_ARC_STYLE_CLOSED_RADIUS:
        pt.x = xc;
        pt.y = yc;
        g_array_append_val(points, pt);
        /* fallthough */
    case GRX_ARC_STYLE_CLOSED_CHORD:
        pt.x = g_array_index(points, GrxPoint, 0).x;
        pt.y = g_array_index(points, GrxPoint, 0).y;
        g_array_append_val(points, pt);
        close = TRUE;
        break;
    default:
        break;
    }
    fval.p = p;
    _GrDrawCustomPolygon(
        points->len, (GrxPoint *)points->data, o, &_GrPatternFiller, fval, close, TRUE);
    g_array_unref(points);
}
