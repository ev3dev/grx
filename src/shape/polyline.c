/*
 * POLYLINE.C ---- draw an open ended polygon
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu] See "copying.grx" for details.
 */

#include "libgrx.h"
#include "shapes.h"

/**
 * grx_draw_polyline:
 * @n_points: the number of points in @points
 * @points: (array length=n_points): an array of #GrxPoint
 * @c: the color
 *
 * Draw a multi-segment line on the current context that connects each point in
 * the @points array using the specified color.
 */
void grx_draw_polyline(int n, GrxPoint *pt, GrxColor c)
{
    GrFillArg fval;
    fval.color = c;
    _GrDrawPolygon(n, pt, &_GrSolidFiller, fval, FALSE);
}
