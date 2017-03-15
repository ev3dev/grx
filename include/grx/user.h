/*
 * user.h
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

#ifndef __GRX_USER_H__
#define __GRX_USER_H__

#include <glib.h>

#include <grx/color.h>
#include <grx/common.h>
#include <grx/draw.h>
#include <grx/text.h>

/**
 * SECTION:user
 * @short_description: Drawing in user window coordinates
 * @title: Drawing in user coordinates
 * @section_id: user
 * @include: grx-3.0.h
 *
 * There is a second set of the graphics primitives which operates in user
 * coordinates. Every context has a user to screen coordinate mapping associated
 * with it.
 *
 * If an application wants to take advantage of the user to screen coordinate
 * mapping it has to use the user coordinate version of the graphics primitives.
 * These have exactly the same parameter passing conventions as their screen
 * coordinate counterparts. NOTE: the user coordinate system is not initialized
 * by the library! The application has to set up its coordinate mapping before
 * calling any of the use coordinate drawing functions – otherwise the program
 * will almost certainly exit (in a quite ungraceful fashion) with a 'division
 * by zero' error.
 */

void grx_user_set_window(gint x1, gint y1, gint x2, gint y2);
void grx_user_get_window(gint *x1, gint *y1, gint *x2, gint *y2);
void grx_user_convert_user_to_screen(gint *x, gint *y);
void grx_user_convert_screen_to_user(gint *x, gint *y);

void grx_user_draw_pixel(gint x, gint y, GrxColor c);
void grx_user_draw_line(gint x1, gint y1, gint x2, gint y2, GrxColor c);
void grx_user_draw_hline(gint x1, gint x2, gint y, GrxColor c);
void grx_user_draw_vline(gint x, gint y1, gint y2, GrxColor c);
void grx_user_draw_box(gint x1, gint y1, gint x2, gint y2,GrxColor c);
void grx_user_draw_circle(gint xc, gint yc, gint r, GrxColor c);
void grx_user_draw_ellipse(gint xc, gint yc, gint rx, gint ry, GrxColor c);
void grx_user_draw_circle_arc(gint xc, gint yc, gint r, gint start, gint end,
                              GrxArcStyle style, GrxColor c);
void grx_user_draw_ellipse_arc(gint xc, gint yc, gint rx, gint ry, gint start, gint end,
                               GrxArcStyle style, GrxColor c);
void grx_user_draw_polyline(gint n_points, GrxPoint *points, GrxColor c);
void grx_user_draw_polygon(gint n_points, GrxPoint *points, GrxColor c);

void grx_user_draw_filled_box(gint x1, gint y1, gint x2, gint y2,GrxColor c);
void grx_user_draw_framed_box(gint x1, gint y1, gint x2, gint y2, gint width,
                              GrxFramedBoxColors *c);
void grx_user_draw_filled_circle(gint xc, gint yc, gint r, GrxColor c);
void grx_user_draw_filled_ellipse(gint xc, gint yc, gint rx, gint ry, GrxColor c);
void grx_user_draw_filled_circle_arc(gint xc, gint yc, gint r, gint start, gint end,
                                     GrxArcStyle style, GrxColor c);
void grx_user_draw_filled_ellipse_arc(gint xc, gint yc, gint rx, gint ry, gint start, gint end,
                                      GrxArcStyle style,GrxColor c);
void grx_user_draw_filled_polygon(gint n_points, GrxPoint *points, GrxColor c);
void grx_user_draw_filled_convex_polygon(gint n_points, GrxPoint *points, GrxColor c);
void grx_user_flood_fill(gint x, gint y, GrxColor border, GrxColor c);

GrxColor grx_user_get_pixel_at(gint x, gint y);
GrxColor grx_context_get_pixel_at_user(GrxContext *context, gint x, gint y);

void grx_user_draw_line_with_options(gint x1, gint y1, gint x2, gint y2, const GrxLineOptions *o);
void grx_user_draw_box_with_options(gint x1, gint y1, gint x2, gint y2, const GrxLineOptions *o);
void grx_user_draw_circle_with_options(gint xc, gint yc, gint r, const GrxLineOptions *o);
void grx_user_draw_ellipse_with_options(gint xc, gint yc, gint rx, gint ry, const GrxLineOptions *o);
void grx_user_draw_circle_arc_with_options(gint xc, gint yc, gint r, gint start, gint end,
                                           GrxArcStyle style, const GrxLineOptions *o);
void grx_user_draw_ellipse_arc_with_options(gint xc, gint yc, gint rx, gint ry, gint start, gint end,
                                            GrxArcStyle style, const GrxLineOptions *o);
void grx_user_draw_polyline_with_options(gint n_points, GrxPoint *points, const GrxLineOptions *o);
void grx_user_draw_polygon_with_options(gint n_points, GrxPoint *points, const GrxLineOptions *o);

void grx_user_draw_line_with_pattern(gint x1, gint y1, gint x2, gint y2, GrxLinePattern *lp);
void grx_user_draw_box_with_pattern(gint x1, gint y1, gint x2, gint y2, GrxLinePattern *lp);
void grx_user_draw_circle_with_pattern(gint xc, gint yc, gint r, GrxLinePattern *lp);
void grx_user_draw_ellipse_with_pattern(gint xc, gint yc, gint rx, gint ry, GrxLinePattern *lp);
void grx_user_draw_circle_arc_with_pattern(gint xc, gint yc, gint r, gint start, gint end,
                                           GrxArcStyle style, GrxLinePattern *lp);
void grx_user_draw_ellipse_arc_with_pattern(gint xc, gint yc, gint rx, gint ry, gint start, gint end,
                                            GrxArcStyle style, GrxLinePattern *lp);
void grx_user_draw_polyline_with_pattern(gint n_points, GrxPoint *points, GrxLinePattern *lp);
void grx_user_draw_polygon_with_pattern(gint n_points, GrxPoint *points, GrxLinePattern *lp);

void grx_user_draw_filled_pixel_with_pattern(gint x, gint y, GrxPattern *p);
void grx_user_draw_filled_line_with_pattern(gint x1, gint y1, gint x2, gint y2, GrxPattern *p);
void grx_user_draw_filled_box_with_pattern(gint x1, gint y1, gint x2, int y2, GrxPattern *p);
void grx_user_draw_filled_circle_with_pattern(gint xc, gint yc, gint r, GrxPattern *p);
void grx_user_draw_filled_ellipse_with_pattern(gint xc, gint yc, gint rx, gint ry, GrxPattern *p);
void grx_user_draw_filled_circle_arc_with_pattern(gint xc, gint yc, gint r, gint start, gint end,
                                                  GrxArcStyle style, GrxPattern *p);
void grx_user_draw_filled_ellipse_arc_with_pattern(gint xc, gint yc, gint rx, gint ry, gint start, gint end,
                                                   GrxArcStyle style, GrxPattern *p);
void grx_user_draw_filled_polygon_with_pattern(gint n_points, GrxPoint *points, GrxPattern *p);
void grx_user_draw_filled_convex_polygon_with_pattern(gint n_points, GrxPoint *points, GrxPattern *p);
void grx_user_flood_fill_with_pattern(gint x, gint y, GrxColor border, GrxPattern *p);

void grx_user_draw_text(const gchar *text, gint x, gint y, GrxFont *font,
    GrxColor fg, GrxColor bg, GrxTextHAlign h_align, GrxTextVAlign v_align);

#endif /* __GRX_USER_H__ */
