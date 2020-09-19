/*
 * pfellia.c
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

#include "allocate.h"
#include "libgrx.h"
#include "shapes.h"

/**
 * grx_draw_filled_ellipse_arc_with_pixmap:
 * @xc: the X coordinate of the center of the arc
 * @yc: the Y coordinate of the center of the arc
 * @rx: the radius in the X direction
 * @ry: the radius in the Y direction
 * @start: the starting angle in 1/10ths of degrees
 * @end: the ending angle in 1/10ths of degrees
 * @style: the arc style
 * @p: the pixmap
 *
 * Draws a filled arc on the current context centered at the specified
 * coordinates from the starting angle to the ending angle with the specified
 * radii, arc style and pixmap.
 */
void grx_draw_filled_ellipse_arc_with_pixmap(
    int xc, int yc, int rx, int ry, int start, int end, GrxArcStyle style, GrxPixmap *p)
{
    GArray *points;
    GrFillArg fa;

    points = grx_generate_ellipse_arc(xc, yc, rx, ry, start, end);

    if (style == GRX_ARC_STYLE_CLOSED_RADIUS) {
        GrxPoint pt = {
            .x = xc,
            .y = yc,
        };
        g_array_append_val(points, pt);
    }

    fa.p = p;
    _GrScanPolygon(points->len, (GrxPoint *)points->data, &_GrPatternFiller, fa);
    g_array_unref(points);
}
