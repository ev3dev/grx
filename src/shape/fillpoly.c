/*
 * fillpoly.c ---- fill an arbitrary polygon with a solid color
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

#include "libgrx.h"
#include "shapes.h"

/**
 * grx_draw_filled_polygon:
 * @n_points: the number of points in @points
 * @points: (array length=n_points): an array of #GrxPoint
 * @c: the color
 *
 * Draw a filled polygon on the current context that connects each point in
 * the @points array using the specified color.
 *
 * Coordinate arrays can either contain or omit the closing edge of the polygon.
 * It will be automatically appended to the list if it is missing.
 */
void grx_draw_filled_polygon(int n,GrxPoint *pt,GrxColor c)
{
        GrFillArg fval;
        fval.color = c;
        _GrScanPolygon(n,pt,&_GrSolidFiller,fval);
}
