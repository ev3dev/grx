/*
 * custplne.c ---- draw an open ended dashed and/or wide polygon
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
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

#include <grx/wideline.h>

#include "libgrx.h"
#include "shapes.h"

/**
 * grx_draw_polyline_with_options:
 * @n_points: the number of points in @points
 * @points: (array length=n_points): an array of #GrxPoint
 * @o: the options
 *
 * Draw a multi-segment line on the current context that connects each point in
 * the @points array using the specified options.
 */
void grx_draw_polyline_with_options(int n, const GrxPoint *pt, const GrxLineOptions *o)
{
    GrFillArg fval;
    fval.color = o->color;
    _GrDrawCustomPolygon(n, pt, o, &_GrSolidFiller, fval, FALSE, FALSE);
}
