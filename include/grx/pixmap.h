/*
 * pixmap.h
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

#ifndef __GRX_PIXMAP_H__
#define __GRX_PIXMAP_H__

#include <glib-object.h>
#include <glib.h>

#include <grx/color.h>
#include <grx/common.h>
#include <grx/context.h>
#include <grx/draw.h>

/**
 * SECTION:pixmap
 * @short_description: Patterned drawing and filling primitives
 * @title: Pattern filled graphics primitives
 * @section_id: pixmap
 * @include: grx-3.0.h
 *
 * The library also supports a pattern filled version of the basic filled
 * primitives described above. These functions have similar parameter passing
 * conventions as the basic ones with one difference: instead of the color
 * value a #GrxPixmap has to be passed to them.
 */

/**
 * GrxPixmapMirrorFlags:
 * @GRX_PIXMAP_MIRROR_HORIZONTAL: inverse left-right
 * @GRX_PIXMAP_MIRROR_VERTICAL: inverse top-bottom
 *
 * Flags used by grx_pixmap_mirror().
 */
typedef enum /*< flags >*/ {
    GRX_PIXMAP_MIRROR_HORIZONTAL = 0x01,
    GRX_PIXMAP_MIRROR_VERTICAL = 0x02,
} GrxPixmapMirrorFlags;

/**
 * GrxPixmap:
 *
 * A fill pattern stored in a layout identical to the video RAM
 * for filling using 'bitblt'-s. It is mode dependent, typically one
 * of the library functions is used to build it.
 */
struct _GrxPixmap {
    /*<private>*/
    gboolean is_pixmap;  // Must be set to TRUE!
    gint width;          // pixmap width in pixels
    gint height;         // pixmap height in pixels
    GrxColorMode mode;   // bitblt mode
    GrxFrame source;     // source context for fill
    GrxContext *context; // to hold a reference
};

GType grx_pixmap_get_type(void);

GrxPixmap *grx_pixmap_new(const guint8 *pixels, gint w, gint h, const GArray *colors);
GrxPixmap *grx_pixmap_new_from_bits(
    const guint8 *bits, gint w, gint h, GrxColor fg, GrxColor bg);
GrxPixmap *grx_pixmap_new_from_context(GrxContext *context);

GrxPixmap *grx_pixmap_copy(GrxPixmap *pixmap);
void grx_pixmap_free(GrxPixmap *pixmap);

GrxPixmap *grx_pixmap_mirror(GrxPixmap *pixmap, GrxPixmapMirrorFlags flags);
GrxPixmap *grx_pixmap_stretch(GrxPixmap *pixmap, gint new_width, gint new_height);

void grx_draw_pixmap(gint x, gint y, GrxPixmap *p);
void grx_draw_pixmap_tiled(gint x1, gint y1, gint x2, gint y2, GrxPixmap *p);

void grx_draw_pixel_with_offset_pixmap(gint x0, gint y0, gint x, gint y, GrxPixmap *p);
void grx_draw_line_with_pixmap(
    gint x1, gint y1, gint x2, gint y2, GrxLineOptions *o, GrxPixmap *p);
void grx_draw_hline_with_offset_pixmap(
    gint x0, gint y0, gint x, gint y, gint width, GrxPixmap *p);
void grx_draw_box_with_pixmap(
    gint x1, gint y1, gint x2, gint y2, GrxLineOptions *o, GrxPixmap *p);
void grx_draw_circle_with_pixmap(
    gint xc, gint yc, gint r, GrxLineOptions *o, GrxPixmap *p);
void grx_draw_ellipse_with_pixmap(
    gint xc, gint yc, gint rx, gint ry, GrxLineOptions *o, GrxPixmap *p);
void grx_draw_circle_arc_with_pixmap(gint xc, gint yc, gint r, gint start, gint end,
    GrxArcStyle style, GrxLineOptions *o, GrxPixmap *p);
void grx_draw_ellipse_arc_with_pixmap(gint xc, gint yc, gint rx, gint ry, gint start,
    gint end, GrxArcStyle style, GrxLineOptions *o, GrxPixmap *p);
void grx_draw_polyline_with_pixmap(
    gint n_points, const GrxPoint *points, GrxLineOptions *o, GrxPixmap *p);
void grx_draw_polygon_with_pixmap(
    gint n_points, const GrxPoint *points, GrxLineOptions *o, GrxPixmap *p);

void grx_draw_filled_pixel_with_pixmap(gint x, gint y, GrxPixmap *p);
void grx_draw_filled_line_with_pixmap(gint x1, gint y1, gint x2, gint y2, GrxPixmap *p);
void grx_draw_filled_box_with_pixmap(gint x1, gint y1, gint x2, gint y2, GrxPixmap *p);
void grx_draw_filled_box_with_offset_pixmap(
    gint x0, gint y0, gint x1, gint y1, gint x2, gint y2, GrxPixmap *p);
void grx_draw_filled_circle_with_pixmap(gint xc, gint yc, gint r, GrxPixmap *p);
void grx_draw_filled_ellipse_with_pixmap(
    gint xc, gint yc, gint rx, gint ry, GrxPixmap *p);
void grx_draw_filled_circle_arc_with_pixmap(
    gint xc, gint yc, gint r, gint start, gint end, GrxArcStyle style, GrxPixmap *p);
void grx_draw_filled_ellipse_arc_with_pixmap(gint xc, gint yc, gint rx, gint ry,
    gint start, gint end, GrxArcStyle style, GrxPixmap *p);
void grx_draw_filled_polygon_with_pixmap(
    gint n_points, const GrxPoint *points, GrxPixmap *p);
void grx_draw_filled_convex_polygon_with_pixmap(
    gint n_points, const GrxPoint *points, GrxPixmap *p);
void grx_flood_fill_with_pixmap(gint x, gint y, GrxColor border, GrxPixmap *p);

#endif /* __GRX_PIXMAP_H__ */
