/*
 * celli.c ---- draw dashed and/or wide ellipses
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

#include <grx/draw.h>
#include <grx/wideline.h>

#include "libgrx.h"
#include "allocate.h"
#include "shapes.h"

/**
 * grx_draw_ellipse_with_options:
 * @xc: the X coordinate of the center of the ellipse
 * @yc: the Y coordinate of the center of the ellipse
 * @rx: the radius in the X direction
 * @ry: the radius in the Y direction
 * @o: the options
 *
 * Draws an ellipse on the current context using the specified options.
 *
 * The ellipse can only draw ellipses with its major axis parallel with either
 * the X or Y coordinate axis
 */
void grx_draw_ellipse_with_options(int xc,int yc,int rx,int ry,const GrxLineOptions *o)
{
    GArray *points;
    GrFillArg fval;

    points = grx_generate_ellipse (xc, yc, rx, ry);
    fval.color = o->color;
    _GrDrawCustomPolygon (points->len, (GrxPoint *)points->data, o, &_GrSolidFiller, fval, TRUE, TRUE);
    g_array_unref (points);
}
