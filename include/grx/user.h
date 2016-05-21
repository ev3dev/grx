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
#include <grx/context.h>
#include <grx/pattern.h>
#include <grx/wideline.h>

/* ================================================================== */
/*               DRAWING IN USER WINDOW COORDINATES                   */
/* ================================================================== */

void grx_user_set_window(gint x1, gint y1, gint x2, gint y2);
void grx_user_get_window(gint *x1, gint *y1, gint *x2, gint *y2);
void grx_user_convert_user_to_screen(gint *x, gint *y);
void grx_user_convert_screen_to_user(gint *x, gint *y);

void grx_user_draw_point(gint x, gint y, GrxColor c);
void grx_user_draw_line(gint x1, gint y1, gint x2, gint y2, GrxColor c);
void grx_user_draw_hline(gint x1, gint x2, gint y, GrxColor c);
void grx_user_draw_vline(gint x, gint y1, gint y2, GrxColor c);
void grx_user_draw_box(gint x1, gint y1, gint x2, gint y2,GrxColor c);
void grx_user_draw_filled_box(gint x1, gint y1, gint x2, gint y2,GrxColor c);
void grx_user_draw_framed_box(gint x1, gint y1, gint x2, gint y2, gint wdt,
                              GrxFramedBoxColors *c);
void grx_user_draw_circle(gint xc, gint yc, gint r, GrxColor c);
void grx_user_draw_ellipse(gint xc, gint yc, gint xa, gint ya, GrxColor c);
void grx_user_draw_circle_arc(gint xc, gint yc, gint r, gint start, gint end,
                              GrxArcStyle style, GrxColor c);
void grx_user_draw_ellipse_arc(gint xc, gint yc, gint xa, gint ya, gint start, gint end,
                               GrxArcStyle style, GrxColor c);
void grx_user_draw_filled_circle(gint xc, gint yc, gint r, GrxColor c);
void grx_user_draw_filled_ellipse(gint xc, gint yc, gint xa, gint ya, GrxColor c);
void grx_user_draw_filled_circle_arc(gint xc, gint yc, gint r, gint start, gint end,
                                     GrxArcStyle style, GrxColor c);
void grx_user_draw_filled_ellipse_arc(gint xc, gint yc, gint xa, gint ya, gint start, gint end,
                                      GrxArcStyle style,GrxColor c);
void grx_user_draw_polyline(gint numpts, gint points[][2], GrxColor c);
void grx_user_draw_polygon(gint numpts, gint points[][2], GrxColor c);
void grx_user_draw_filled_convex_polygon(gint numpts, gint points[][2], GrxColor c);
void grx_user_draw_filled_polygon(gint numpts, gint points[][2], GrxColor c);
void grx_user_flood_fill(gint x, gint y, GrxColor border, GrxColor c);

GrxColor grx_user_get_pixel_at(gint x, gint y);
GrxColor grx_context_get_pixel_at_user(GrxContext *c, gint x, gint y);

void grx_user_draw_line_with_options(gint x1, gint y1, gint x2, gint y2, const GrxLineOptions *o);
void grx_user_draw_box_with_options(gint x1, gint y1, gint x2, gint y2, const GrxLineOptions *o);
void grx_user_draw_circle_with_options(gint xc, gint yc, gint r, const GrxLineOptions *o);
void grx_user_draw_ellipse_with_options(gint xc, gint yc, gint xa, gint ya, const GrxLineOptions *o);
void grx_user_draw_circle_arc_with_options(gint xc, gint yc, gint r, gint start, gint end,
                                           GrxArcStyle style, const GrxLineOptions *o);
void grx_user_draw_ellipse_arc_with_options(gint xc, gint yc, gint xa, gint ya, gint start, gint end,
                                            GrxArcStyle style, const GrxLineOptions *o);
void grx_user_draw_polyline_with_options(gint numpts, gint points[][2], const GrxLineOptions *o);
void grx_user_draw_polygon_with_options(gint numpts, gint points[][2], const GrxLineOptions *o);

void grx_user_draw_line_with_pattern(gint x1, gint y1, gint x2, gint y2, GrxLinePattern *lp);
void grx_user_draw_box_with_pattern(gint x1, gint y1, gint x2, gint y2, GrxLinePattern *lp);
void grx_user_draw_circle_with_pattern(gint xc, gint yc, gint r, GrxLinePattern *lp);
void grx_user_draw_ellipse_with_pattern(gint xc, gint yc, gint xa, gint ya, GrxLinePattern *lp);
void grx_user_draw_circle_arc_with_pattern(gint xc, gint yc, gint r, gint start, gint end,
                                           GrxArcStyle style, GrxLinePattern *lp);
void grx_user_draw_ellipse_arc_with_pattern(gint xc, gint yc, gint xa, gint ya, gint start, gint end,
                                            GrxArcStyle style, GrxLinePattern *lp);
void grx_user_draw_polyline_with_pattern(gint numpts, gint points[][2], GrxLinePattern *lp);
void grx_user_draw_polygon_with_pattern(gint numpts, gint points[][2], GrxLinePattern *lp);

void grx_user_draw_filled_point_with_pattern(gint x, gint y, GrxPattern *p);
void grx_user_draw_filled_line_with_pattern(gint x1, gint y1, gint x2, gint y2, GrxPattern *p);
void grx_user_draw_filled_box_with_pattern(gint x1, gint y1, gint x2, int y2, GrxPattern *p);
void grx_user_draw_filled_circle_with_pattern(gint xc, gint yc, gint r, GrxPattern *p);
void grx_user_draw_filled_ellipse_with_pattern(gint xc, gint yc, gint xa, gint ya, GrxPattern *p);
void grx_user_draw_filled_circle_arc_with_pattern(gint xc, gint yc, gint r, gint start, gint end,
                                                  GrxArcStyle style, GrxPattern *p);
void grx_user_draw_filled_ellipse_arc_with_pattern(gint xc, gint yc, gint xa, gint ya, gint start, gint end,
                                                   GrxArcStyle style, GrxPattern *p);
void grx_user_draw_filled_convex_polygon_with_pattern(gint numpts, gint points[][2], GrxPattern *p);
void grx_user_draw_filled_polygon_with_pattern(gint numpts, gint points[][2], GrxPattern *p);
void grx_user_flood_fill_with_pattern(gint x, gint y, GrxColor border, GrxPattern *p);

void grx_user_draw_char_with_text_options(gint chr, gint x, gint y, const GrxTextOption *opt);
void grx_user_draw_text_with_text_options(gchar *text, gint length, gint x, gint y,
                                          const GrxTextOption *opt);
void grx_user_draw_text(gchar *text, gint x, gint y, GrxColor fg, GrxColor bg);

#endif /* __GRX_USER_H__ */
