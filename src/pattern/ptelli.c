/*
 * ptelli.c
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
 * grx_draw_ellipse_with_pixmap:
 * @xc: the X coordinate of the center of the ellipse
 * @yc: the Y coordinate of the center of the ellipse
 * @rx: the radius in the X direction
 * @ry: the radius in the Y direction
 * @o: the line options
 * @p: the pixmap
 *
 * Draws an ellipse on the current context using the specified line options and
 * pattern.
 *
 * The ellipse can only draw ellipses with its major axis parallel with either
 * the X or Y coordinate axis.
 */
void grx_draw_ellipse_with_pixmap(
    int xc, int yc, int rx, int ry, GrxLineOptions *o, GrxPixmap *p)
{
    GArray *points;

    points = grx_generate_ellipse(xc, yc, rx, ry);
    GrFillArg fval;

    fval.p = p;
    _GrDrawCustomPolygon(
        points->len, (GrxPoint *)points->data, o, &_GrPatternFiller, fval, TRUE, TRUE);
    g_array_unref(points);
}
