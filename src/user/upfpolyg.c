/*
 * upfpolyg.c
 *
 * Copyright (C) 1997, Michael Goffioul
 * [goffioul@emic.ucl.ac.be]
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
#include <grx/pixmap.h>

#include "globals.h"
#include "libgrx.h"
#include "usercord.h"

/**
 * grx_user_draw_filled_polygon_with_pixmap:
 * @n_points: the number of points in @points
 * @points: (array length=n_points): an array of #GrxPoint
 * @p: the pixmap
 *
 * Draw a filled polygon on the current context that connects each point in
 * the @points array using the specified pixmap.
 *
 * Coordinate arrays can either contain or omit the closing edge of the polygon.
 * It will be automatically appended to the list if it is missing.
 */
void grx_user_draw_filled_polygon_with_pixmap(
    gint numpts, const GrxPoint *points, GrxPixmap *p)
{
    GrxPoint *tmp = g_newa(GrxPoint, numpts);

    for (gint pt = 0; pt < numpts; pt++) {
        tmp[pt] = points[pt];
        U2SX(tmp[pt].x, CURC);
        U2SY(tmp[pt].y, CURC);
    }

    grx_draw_filled_polygon_with_pixmap(numpts, tmp, p);
}
