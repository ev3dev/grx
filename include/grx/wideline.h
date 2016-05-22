/*
 * wideline.h
 *
 * Copyright (c) 2015 David Lechner <david@lechnology.com>
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

#ifndef __GRX_WIDELINE_H__
#define __GRX_WIDELINE_H__

#include <glib.h>

#include <grx/color.h>
#include <grx/common.h>

/* ================================================================== */
/*            THICK AND DASHED LINE DRAWING PRIMITIVES                */
/* ================================================================== */

/*
 * custom line options structure
 *   zero or one dash pattern length means the line is continuous
 *   the dash pattern always begins with a drawn section
 */
struct _GrxLineOptions {
    GrxColor color;                   /* color used to draw line */
    gint     width;                   /* width of the line */
    gint     n_dash_patterns;         /* length of the dash pattern */
    guint8  *dash_patterns;           /* draw/nodraw pattern */
};

void grx_draw_line_with_options(gint x1, gint y1, gint x2, gint y2, const GrxLineOptions *o);
void grx_draw_box_with_options(gint x1, gint y1, gint x2, gint y2, const GrxLineOptions *o);
void grx_draw_circle_with_options(gint xc, gint yc, gint r, const GrxLineOptions *o);
void grx_draw_ellipse_with_options(gint xc, gint yc, gint rx, gint ry, const GrxLineOptions *o);
void grx_draw_circle_arc_with_options(gint xc, gint yc, gint r, gint start, gint end,
                                      GrxArcStyle style, const GrxLineOptions *o);
void grx_draw_ellipse_arc_with_options(gint xc, gint yc, gint rx, gint ry, gint start, gint end,
                                       GrxArcStyle style, const GrxLineOptions *o);
void grx_draw_polyline_with_options(gint n_points, GrxPoint *points, const GrxLineOptions *o);
void grx_draw_polygon_with_options(gint n_points, GrxPoint *points, const GrxLineOptions *o);

#endif /* __GRX_WIDELINE_H__ */
