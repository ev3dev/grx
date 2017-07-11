/*
 * draw.h
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

#ifndef __GRX_DRAW_H__
#define __GRX_DRAW_H__

#include <glib.h>

#include <grx/color.h>
#include <grx/common.h>

/**
 * SECTION:draw
 * @short_description: Basic drawing functions
 * @title: Graphics Primitives
 * @section_id: draw
 * @include: grx-3.0.h
 *
 * These functions operate on the current context.
 */

/**
 * GRX_MAX_ANGLE_VALUE:
 *
 * The maximum allowable angle (360 degrees).
 */
#define GRX_MAX_ANGLE_VALUE      (3600)

/**
 * GrxArcStyle:
 * @GRX_ARC_STYLE_OPEN: Draws the arc only, leaving the ends open.
 * @GRX_ARC_STYLE_CLOSED_CHORD: Draws the arc and closes it with a line that is
 *                              the chord connecting the two endpoints.
 * @GRX_ARC_STYLE_CLOSED_RADIUS: Draws the arc and closes it with two lines
 *                               along the radii connecting the ends of the arc
 *                               to the center of the circle/ellipse.
 *
 * Indicates how an arc should be drawn.
 */
typedef enum {
    GRX_ARC_STYLE_OPEN           = 0,
    GRX_ARC_STYLE_CLOSED_CHORD   = 1,
    GRX_ARC_STYLE_CLOSED_RADIUS  = 2,
} GrxArcStyle;

/**
 * GrxFramedBoxColors:
 * @background: the background fill color
 * @border_top: the color for the top border
 * @border_right: the color for the right border
 * @border_bottom: the color for the bottom border
 * @border_left: the color for the left border
 *
 * Struct to hold colors for a framed box.
 */
struct _GrxFramedBoxColors {
    GrxColor background;
    GrxColor border_top;
    GrxColor border_right;
    GrxColor border_bottom;
    GrxColor border_left;
};

/**
 * GrxPoint:
 * @x: the X coordinate
 * @y: the Y coordinate
 *
 * Struct that holds coordinates of a point for use in polyline/polygon
 * functions.
 */
struct _GrxPoint {
    gint x;
    gint y;
};

void grx_clear_screen(GrxColor bg);
void grx_clear_context(GrxColor bg);
void grx_clear_clip_box(GrxColor bg);

void grx_draw_pixel(gint x, gint y, GrxColor c);
void grx_draw_line(gint x1, gint y1, gint x2, gint y2, GrxColor c);
void grx_draw_hline(gint x1, gint x2, gint y, GrxColor c);
void grx_draw_vline(gint x, gint y1, gint y2,GrxColor c);
void grx_draw_box(gint x1, gint y1, gint x2, gint y2, GrxColor c);
void grx_draw_rounded_box(gint x1, gint y1, gint x2, gint y2, gint r, GrxColor c);
void grx_draw_circle(gint xc, gint yc, gint r, GrxColor c);
void grx_draw_ellipse(gint xc, gint yc, gint rx, gint ry, GrxColor c);
void grx_draw_circle_arc(gint xc, gint yc, gint r, gint start, gint end, GrxArcStyle style, GrxColor c);
void grx_draw_ellipse_arc(gint xc, gint yc, gint rx, gint ry, gint start, gint end,GrxArcStyle style,GrxColor c);
void grx_draw_polyline(gint n_points, GrxPoint *points, GrxColor c);
void grx_draw_polygon(gint n_points, GrxPoint *points, GrxColor c);

void grx_get_last_arc_coordinates(gint *xs, gint *ys, gint *xe, gint *ye, gint *xc, gint *yc);
GArray *grx_generate_ellipse(gint xc, gint yc, gint rx, gint ry);
GArray *grx_generate_ellipse_arc(gint xc, gint yc, gint rx, gint ry, gint start, gint end);
GArray *grx_generate_points(GArray *coords);

void grx_draw_filled_box(gint x1, gint y1, gint x2, gint y2, GrxColor c);
void grx_draw_filled_rounded_box(gint x1, gint y1, gint x2, gint y2, gint r, GrxColor c);
void grx_draw_framed_box(gint x1, gint y1, gint x2, gint y2, gint width, const GrxFramedBoxColors *c);
void grx_draw_filled_circle(gint xc, gint yc, gint r, GrxColor c);
void grx_draw_filled_ellipse(gint xc, gint yc, gint rx, gint ry, GrxColor c);
void grx_draw_filled_circle_arc(gint xc, gint yc, gint r, gint start, gint end, GrxArcStyle style, GrxColor c);
void grx_draw_filled_ellipse_arc(gint xc, gint yc, gint rx, gint ry, gint start, gint end, GrxArcStyle style, GrxColor c);
void grx_draw_filled_polygon(gint n_points, GrxPoint *points, GrxColor c);
void grx_draw_filled_convex_polygon(gint n_points, GrxPoint *points, GrxColor c);

void grx_flood_fill(gint x, gint y, GrxColor border, GrxColor c);
void grx_flood_spill(gint x1, gint y1, gint x2, gint y2, GrxColor old_c, GrxColor new_c);
void grx_flood_spill2(gint x1, gint y1, gint x2, gint y2, GrxColor old_c1, GrxColor new_c1, GrxColor old_c2, GrxColor new_c2);

GrxColor grx_get_pixel_at(gint x, gint y);

void grx_bit_blt(gint x, gint y, GrxContext *src, gint x1, gint y1, gint x2, gint y2, GrxColor op);
void grx_bit_blt_1bpp(gint x, gint y, GrxContext *src, gint x1, gint y1, gint x2, gint y2, GrxColor fg, GrxColor bg);

const GrxColor *grx_get_scanline(gint x1, gint x2, gint y, guint *n);
void grx_put_scanline(gint x1, gint x2, gint y, const GrxColor *scan_line, GrxColor op);

#ifndef GRX_SKIP_INLINES
#define grx_bit_blt(x,y,s,x1,x2,y1,y2,o) \
    grx_context_bit_blt(NULL,(x),(y),(s),(x1),(x2),(y1),(y2),(o))
#define grx_bit_blt_1bpp(x,y,s,x1,y1,x2,y2,f,b) \
    grx_context_bit_blt_1bpp(NULL,(x),(y),(s),(x1),(y1),(x2),(y2),(f),(b))
#define grx_get_scanline(x1,x2,yy,n) \
    grx_context_get_scanline(NULL,(x1),(x2),(yy),(n))
#endif /* GRX_SKIP_INLINES */

#endif /* __GRX_DRAW_H__ */
