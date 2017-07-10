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
#include <grx/draw.h>

/**
 * SECTION:wideline
 * @short_description: Thick and dashed line drawing primitives
 * @title: Customized line drawing
 * @section_id: wideline
 * @include: grx-3.0.h
 *
 * The basic line drawing graphics primitives described previously always draw
 * continuous lines which are one pixel wide. This group of line drawing
 * functions can be used to draw wide and/or patterned lines. These functions
 * have similar parameter passing conventions as the basic ones with one
 * difference: instead of the color value a #GrxLineOptions is passed instead.
 */

/**
 * GrxLineOptions:
 * @color: color used to draw line
 * @width: width of the line
 * @n_dash_patterns: length of the dash pattern
 * @dash_pattern0: draw/nodraw pattern
 * @dash_pattern1: draw/nodraw pattern
 * @dash_pattern2: draw/nodraw pattern
 * @dash_pattern3: draw/nodraw pattern
 * @dash_pattern4: draw/nodraw pattern
 * @dash_pattern5: draw/nodraw pattern
 * @dash_pattern6: draw/nodraw pattern
 * @dash_pattern7: draw/nodraw pattern
 *
 * Custom line options structure.
 *
 * Zero or one dash pattern length means the line is continuous. The dash
 * pattern always begins with a drawn section.
 *
 * Example, a white line 3 pixels wide (thick) and pixmap 6 pixels draw,
 * 4 pixels nodraw:
 * |[<!-- language="C" -->
 * GrxLineOptions my_line_options;
 * ...
 * my_line_options.color = GRX_COLOR_WHITE;
 * my_line_options.width = 3;
 * my_line_options.n_dash_patterns = 2;
 * my_line_options.dash_pattern0 = 6;
 * my_line_options.dash_pattern1 = 4;
 * ...
 * ]|
 */
struct _GrxLineOptions {
    GrxColor color;
    gint     width;
    gint     n_dash_patterns;
    guint8   dash_pattern0;
    guint8   dash_pattern1;
    guint8   dash_pattern2;
    guint8   dash_pattern3;
    guint8   dash_pattern4;
    guint8   dash_pattern5;
    guint8   dash_pattern6;
    guint8   dash_pattern7;
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
