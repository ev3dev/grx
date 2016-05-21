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

/* ================================================================== */
/*                       GRAPHICS PRIMITIVES                          */
/* ================================================================== */

#define GRX_MAX_POLYGON_POINTS   (1000000)
#define GRX_MAX_ELLIPSE_POINTS   (1024 + 5)
#define GRX_MAX_ANGLE_VALUE      (3600)

/**
 * GrxArcStyle:
 * @GRX_ARC_STYLE_OPEN: Draws the arc only, leaving the ends open.
 * @GRX_ARC_STYLE_CLOSED_CHORD: Draws the arc and closes it with a line that is
 *                              the chord connecting the two endpoints.
 * @GRX_ARC_STYLE_CLOSED_RADIUS: Draws the arc and closes it with two lines
 *                               along the radii connecting the ends of the arc
 *                               to the center of the circle/ellipse.
 */
enum _GrxArcStyle {
    GRX_ARC_STYLE_OPEN           = 0,
    GRX_ARC_STYLE_CLOSED_CHORD   = 1,
    GRX_ARC_STYLE_CLOSED_RADIUS  = 2,
};

struct _GrxFramedBoxColors {                        /* framed box colors */
    GrxColor background;
    GrxColor border_top;
    GrxColor border_right;
    GrxColor border_bottom;
    GrxColor border_left;
};

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
void grx_draw_filled_box(gint x1, gint y1, gint x2, gint y2, GrxColor c);
void grx_draw_framed_box(gint x1, gint y1, gint x2, gint y2, gint wdt, const GrxFramedBoxColors *c);
gboolean grx_generate_ellipse(gint xc, gint yc, gint xa, gint ya, GrxPoint points[GRX_MAX_ELLIPSE_POINTS]);
gboolean grx_generate_ellipse_arc(gint xc, gint yc, gint xa, gint ya, gint start, gint end, GrxPoint points[GRX_MAX_ELLIPSE_POINTS]);
void grx_get_last_arc_coordinates(gint *xs, gint *ys, gint *xe, gint *ye, gint *xc, gint *yc);
void grx_draw_circle(gint xc, gint yc, gint r, GrxColor c);
void grx_draw_ellipse(gint xc, gint yc, gint xa, gint ya, GrxColor c);
void grx_draw_circle_arc(gint xc, gint yc, gint r, gint start, gint end, GrxArcStyle style, GrxColor c);
void grx_draw_ellipse_arc(gint xc, gint yc, gint xa, gint ya, gint start, gint end,GrxArcStyle style,GrxColor c);
void grx_draw_filled_circle(gint xc, gint yc, gint r, GrxColor c);
void grx_draw_filled_ellipse(gint xc, gint yc, gint xa, gint ya, GrxColor c);
void grx_draw_filled_circle_arc(gint xc, gint yc, gint r, gint start, gint end, GrxArcStyle style, GrxColor c);
void grx_draw_filled_ellipse_arc(gint xc, gint yc, gint xa, gint ya, gint start, gint end, GrxArcStyle style, GrxColor c);
void grx_draw_polyline(gint n_points, GrxPoint *points, GrxColor c);
void grx_draw_polygon(gint n_points, GrxPoint *points, GrxColor c);
void grx_draw_filled_convex_polygon(gint n_points, GrxPoint *points, GrxColor c);
void grx_draw_filled_polygon(gint n_points, GrxPoint *points, GrxColor c);
void grx_bit_blt(GrxContext *dst, gint x, gint y, GrxContext *src, gint x1, gint y1, gint x2, gint y2, GrxColor op);
void grx_bit_blt_1bpp(GrxContext *dst, gint dx, gint dy, GrxContext *src, gint x1, gint y1, gint x2, gint y2, GrxColor fg, GrxColor bg);
void grx_flood_fill(gint x, gint y, GrxColor border, GrxColor c);
void grx_flood_spill(gint x1, gint y1, gint x2, gint y2, GrxColor old_c, GrxColor new_c);
void grx_flood_spill2(gint x1, gint y1, gint x2, gint y2, GrxColor old_c1, GrxColor new_c1, GrxColor old_c2, GrxColor new_c2);
void grx_context_flood_spill(GrxContext *ctx, gint x1, gint y1, gint x2, gint y2, GrxColor old_c, GrxColor new_c);
void grx_context_flood_spill2(GrxContext *ctx, gint x1, gint y1, gint x2, gint y2, GrxColor old_c1, GrxColor new_c1, GrxColor old_c2, GrxColor new_c2);

GrxColor grx_get_pixel_at(gint x, gint y);

const GrxColor *grx_get_scanline(int x1,int x2,int yy);
const GrxColor *grx_context_get_scanline(GrxContext *ctx,int x1,int x2,int yy);
/* Input   ctx: source context, if NULL the current context is used */
/*         x1 : first x coordinate read                             */
/*         x2 : last  x coordinate read                             */
/*         yy : y coordinate                                        */
/* Output  NULL     : error / no data (clipping occured)            */
/*         else                                                     */
/*           p[0..w]: pixel values read                             */
/*                      (w = |x2-y1|)                               */
/*           Output data is valid until next GRX call !             */

void grx_put_scanline(int x1,int x2,int yy,const GrxColor *c, GrxColor op);
/* Input   x1 : first x coordinate to be set                        */
/*         x2 : last  x coordinate to be set                        */
/*         yy : y coordinate                                        */
/*         c  : c[0..(|x2-x1|] hold the pixel data                  */
/*         op : Operation (GRX_COLOR_MODE_WRITE/GRX_COLOR_MODE_XOR/GRX_COLOR_MODE_OR/GRX_COLOR_MODE_AND/GRX_COLOR_MODE_IMAGE)        */
/*                                                                  */
/* Note    c[..] data must fit GRX_COLOR_VALUE_MASK otherwise the results   */
/*         are implementation dependend.                            */
/*         => You can't supply operation code with the pixel data!  */


#ifndef GRX_SKIP_INLINES
#define grx_get_scanline(x1,x2,yy) \
        grx_context_get_scanline(NULL,(x1),(x2),(yy))
#endif

/* ================================================================== */
/*                 NON CLIPPING DRAWING PRIMITIVES                    */
/* ================================================================== */

void grx_draw_pixel_nc(gint x, gint y, GrxColor c);
void grx_draw_line_nc(gint x1, gint y1, gint x2, gint y2,GrxColor c);
void grx_draw_hline_nc(gint x1, gint x2, gint y, GrxColor c);
void grx_draw_vline_nc(gint x, gint y1, gint y2, GrxColor c);
void grx_draw_box_nc(gint x1, gint y1, gint x2, gint y2, GrxColor c);
void grx_draw_filled_box_nc(gint x1, gint y1, gint x2, gint y2, GrxColor c);
void grx_draw_framed_box_nc(gint x1, gint y1, gint x2, gint y2, gint wdt,
                            const GrxFramedBoxColors *c);
void grx_bit_blt_nc(GrxContext *dst, gint x, gint y, GrxContext *src, gint x1,
                    gint y1, gint x2, gint y2, GrxColor op);

GrxColor grx_get_pixel_nc(gint x, gint y);
GrxColor grx_context_get_pixel_nc(GrxContext *c, gint x, gint y);

#ifndef GRX_SKIP_INLINES
#define grx_draw_pixel_nc(x,y,c) (                                             \
        (*grx_get_current_frame_driver()->drawpixel)(                          \
        ((x) + grx_context_get_current()->x_offset),                           \
        ((y) + grx_context_get_current()->y_offset),                           \
        ((c))                                                                  \
        )                                                                      \
)
#define grx_get_pixel_nc(x,y) (                                                \
        (*grx_get_current_frame_driver()->readpixel)(                          \
        (GrxFrame *)(&grx_context_get_current()->frame),                       \
        ((x) + grx_context_get_current()->x_offset),                           \
        ((y) + grx_context_get_current()->y_offset)                            \
        )                                                                      \
)
#define grx_context_get_pixel_nc(c,x,y) (                                      \
        (*(c)->gc_driver->readpixel)(                                          \
        (&(c)->frame),                                                         \
        ((x) + (c)->x_offset),                                                 \
        ((y) + (c)->y_offset)                                                  \
        )                                                                      \
)
#endif  /* GRX_SKIP_INLINES */

#endif /* __GRX_DRAW_H__ */
