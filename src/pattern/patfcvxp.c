/*
 * patfcvxp.c
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

#include "libgrx.h"
#include "shapes.h"

/**
 * grx_draw_filled_convex_polygon_with_pattern:
 * @n_points: the number of points in @points
 * @points: (array length=n_points): an array of #GrxPoint
 * @pattern: the pattern
 *
 * Draw a filled polygon on the current context that connects each point in
 * the @points array using the specified pattern.
 *
 * Coordinate arrays can either contain or omit the closing edge of the polygon.
 * It will be automatically appended to the list if it is missing.
 *
 * This version is slightly more efficient that grx_draw_filled_polygon() but
 * requires that the polygon is convex. It can also be used to fill some concave
 * polygons whose boundaries do not intersect any horizontal scan line more than
 * twice. It can also be used to fill several disjoint nonoverlapping polygons
 * in a single operation.
 */
void grx_draw_filled_convex_polygon_with_pattern(int n,GrxPoint *pt,GrxPattern *p)
{
        GrFillArg fa;

        fa.p = p;
        _GrScanConvexPoly(n,pt,&_GrPatternFiller,fa);
}
