/*
 * pattern.h
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

#ifndef __GRX_PATTERN_H__
#define __GRX_PATTERN_H__

#include <glib.h>
#include <glib-object.h>

#include <grx/color.h>
#include <grx/common.h>
#include <grx/context.h>
#include <grx/draw.h>

/**
 * SECTION:pattern
 * @short_description: Patterned drawing and filling primitives
 * @title: Pattern filled graphics primitives
 * @section_id: pattern
 * @include: grx-3.0.h
 *
 * The library also supports a pattern filled version of the basic filled
 * primitives described above. These functions have similar parameter passing
 * conventions as the basic ones with one difference: instead of the color
 * value a #GrxPattern has to be passed to them. The #GrxPattern union can
 * contain either a bitmap or a pixmap fill pattern. Bitmap fill patterns are
 * rectangular arrays of bits, each set bit representing the foreground color
 * of the fill operation, and each zero bit representing the background. Both
 * the foreground and background colors can be combined with any of the
 * supported logical operations. Bitmap fill patterns have one restriction:
 * their width must be eight pixels. Pixmap fill patterns are very similar to
 * contexts.
 */

/**
 * GrxBitmap:
 * @is_pixmap: Must be set to %FALSE!
 * @height: bitmap height
 * @data: pointer to the bit pattern
 * @fg_color: foreground fill color
 * @bg_color: background fill color
 * @free_on_pattern_destroy: set to %TRUE if @data should be freed
 *
 * A mode independent way to specify a fill pattern of two colors.
 * The width is always 8 pixels but the height is user-defined.
 *
 * Bitmap patterns can be easily built from initialized character arrays and
 * static structures by the C compiler, thus no special support is included in
 * the library for creating them. The only action required from the application
 * program might be changing the foreground and background colors as needed.
 */
typedef struct {
    gboolean  is_pixmap;
    gint      height;
    guint8   *data;
    GrxColor  fg_color;
    GrxColor  bg_color;
    gboolean  free_on_pattern_destroy;
} GrxBitmap;

/**
 * GrxPixmap:
 * @is_pixmap: Must be set to %TRUE!
 * @width: pixmap width in pixels
 * @height: pixmap height in pixels
 * @mode: bitblt mode
 * @source: source context for fill
 *
 * A fill pattern stored in a layout identical to the video RAM
 * for filling using 'bitblt'-s. It is mode dependent, typically one
 * of the library functions is used to build it.
 */
struct _GrxPixmap {
    gboolean      is_pixmap;
    gint          width;
    gint          height;
    GrxColorMode  mode;
    GrxFrame      source;
};

/**
 * GrxPattern:
 * @is_pixmap: %TRUE if this is a #GrxPixmap
 * @bitmap: the bitmap
 * @pixmap: the pixmap
 *
 * Fill pattern union.
 *
 * Can either be a #GrxBitmap or a #GrxPixmap.
 */
union _GrxPattern {
    gboolean  is_pixmap;
    GrxBitmap bitmap;
    GrxPixmap pixmap;
    #define gp_bmp_data               bitmap.data
    #define gp_bmp_height             bitmap.height
    #define gp_bmp_fgcolor            bitmap.fg_color
    #define gp_bmp_bgcolor            bitmap.bg_color
    #define gp_pxp_width              pixmap.width
    #define gp_pxp_height             pixmap.height
    #define gp_pxp_oper               pixmap.mode
    #define gp_pxp_source             pixmap.source
};

/**
 * GrxLinePattern:
 * @pattern: fill pattern
 * @options: custom line drawing options
 *
 * Draw pattern for line drawings.
 */
struct _GrxLinePattern {
    GrxPattern     *pattern;
    GrxLineOptions *options;
};

GType grx_pattern_get_type(void);

GrxPattern *grx_pattern_new_pixmap(const guint8 *pixels, gint w, gint h,
                                   const GArray *colors);
GrxPattern *grx_pattern_new_pixmap_from_bits(const guint8 *bits, gint w, gint h,
                                             GrxColor fg, GrxColor bg);
GrxPattern *grx_pattern_new_pixmap_from_context(GrxContext *context);

GrxPattern *grx_pattern_copy(GrxPattern *pattern);
void grx_pattern_free(GrxPattern *pattern);

void grx_draw_line_with_pattern(gint x1, gint y1, gint x2, gint y2, GrxLinePattern *lp);
void grx_draw_box_with_pattern(gint x1, gint y1, gint x2, gint y2, GrxLinePattern *lp);
void grx_draw_circle_with_pattern(gint xc, gint yc, gint r, GrxLinePattern *lp);
void grx_draw_ellipse_with_pattern(gint xc, gint yc, gint rx, gint ry, GrxLinePattern *lp);
void grx_draw_circle_arc_with_pattern(gint xc, gint yc, gint r, gint start, gint end,
                                      GrxArcStyle style, GrxLinePattern *lp);
void grx_draw_ellipse_arc_with_pattern(gint xc, gint yc, gint rx, gint ry, gint start, gint end,
                                       GrxArcStyle style, GrxLinePattern *lp);
void grx_draw_polyline_with_pattern(gint n_points, GrxPoint *points, GrxLinePattern *lp);
void grx_draw_polygon_with_pattern(gint n_points, GrxPoint *points, GrxLinePattern *lp);

void grx_draw_filled_pixel_with_pattern(gint x, gint y, GrxPattern *p);
void grx_draw_filled_line_with_pattern(gint x1, gint y1, gint x2, gint y2, GrxPattern *p);
void grx_draw_filled_box_with_pattern(gint x1, gint y1, gint x2, gint y2,GrxPattern *p);
void grx_draw_filled_circle_with_pattern(gint xc, gint yc, gint r,GrxPattern *p);
void grx_draw_filled_ellipse_with_pattern(gint xc, gint yc, gint rx, gint ry, GrxPattern *p);
void grx_draw_filled_circle_arc_with_pattern(gint xc, gint yc, gint r, gint start, gint end,
                                             GrxArcStyle style, GrxPattern *p);
void grx_draw_filled_ellipse_arc_with_pattern(gint xc, gint yc, gint rx, gint ry, gint start, gint end,
                                              GrxArcStyle style,GrxPattern *p);
void grx_draw_filled_polygon_with_pattern(gint n_points, GrxPoint *points, GrxPattern *p);
void grx_draw_filled_convex_polygon_with_pattern(gint n_points, GrxPoint *points, GrxPattern *p);
void grx_flood_fill_with_pattern(gint x, gint y, GrxColor border, GrxPattern *p);

#endif /* __GRX_PATTERN_H__ */
