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
 */

#include <grx/draw.h>

#include "libgrx.h"
#include "shapes.h"

/**
 * grx_draw_polyline_with_pixmap:
 * @n_points: the number of points in @points
 * @points: (array length=n_points): an array of #GrxPoint
 * @o: the line options
 * @p: the pixmap
 *
 * Draw a multi-segment line on the current context that connects each point in
 * the @points array using the specified line options and pixmap.
 */
void grx_draw_polyline_with_pixmap(
    int numpts, const GrxPoint *points, const GrxLineOptions *o, GrxPixmap *p)
{
    GrFillArg fval;

    fval.p = p;
    _GrDrawCustomPolygon(numpts, points, o, &_GrPatternFiller, fval, FALSE, FALSE);
}

/**
 * grx_draw_polygon_with_pixmap:
 * @n_points: the number of points in @points
 * @points: (array length=n_points): an array of #GrxPoint
 * @o: the line options
 * @p: the pixmap
 *
 * Draw a closed polygon on the current context that connects each point in
 * the @points array using the specified line options and pixmap.
 *
 * Coordinate arrays can either contain or omit the closing edge of the polygon.
 * It will be automatically appended to the list if it is missing.
 */
void grx_draw_polygon_with_pixmap(
    int numpts, const GrxPoint *points, const GrxLineOptions *o, GrxPixmap *p)
{
    GrFillArg fval;

    fval.p = p;
    _GrDrawCustomPolygon(numpts, points, o, &_GrPatternFiller, fval, TRUE, FALSE);
}

/**
 * grx_draw_box_with_pixmap:
 * @x1: the left X coordinate
 * @y1: the top Y coordinate
 * @x2: the right X coordinate
 * @y2: the bottom Y coordinate
 * @o: the line options
 * @p: the pixmap
 *
 * Draws a rectangle on the current context using the specified coordinates
 * and line options and pixmap.
 */
void grx_draw_box_with_pixmap(
    int x1, int y1, int x2, int y2, const GrxLineOptions *o, GrxPixmap *p)
{
    GrFillArg fval;
    GrxPoint points[4];

    points[0].x = x1;
    points[0].y = y1;
    points[1].x = x1;
    points[1].y = y2;
    points[2].x = x2;
    points[2].y = y2;
    points[3].x = x2;
    points[3].y = y1;
    fval.p = p;
    _GrDrawCustomPolygon(4, points, o, &_GrPatternFiller, fval, TRUE, FALSE);
}
