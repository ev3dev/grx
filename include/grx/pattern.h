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

/* ================================================================== */
/*             PATTERNED DRAWING AND FILLING PRIMITIVES               */
/* ================================================================== */

/**
 * GrxBitmap:
 *
 * A mode independent way to specify a fill pattern of two
 * colors. It is always 8 pixels wide (1 byte per scan line), its
 * height is user-defined. SET is_pixmap TO FALSE!!!
 */
typedef struct {
    gboolean is_pixmap;               /* type flag for pattern union */
    gint     height;                  /* bitmap height */
    guint8  *data;                    /* pointer to the bit pattern */
    GrxColor fg_color;                /* foreground color for fill */
    GrxColor bg_color;                /* background color for fill */
    gboolean free_on_pattern_destroy; /* set if dynamically allocated */
} GrxBitmap;

/**
 * GrxPixmap:
 *
 * A fill pattern stored in a layout identical to the video RAM
 * for filling using 'bitblt'-s. It is mode dependent, typically one
 * of the library functions is used to build it. SET is_pixmap TO TRUE!!!
 */
struct _GrxPixmap {
    gboolean     is_pixmap;      /* type flag for pattern union */
    gint         width;          /* pixmap width (in pixels)  */
    gint         height;         /* pixmap height (in pixels) */
    GrxColorMode mode;           /* bitblt mode (SET, OR, XOR, AND, IMAGE) */
    GrxFrame     source;         /* source context for fill */
};

/**
 * GrxPattern:
 *
 * Fill pattern union -- can either be a bitmap or a pixmap
 */
union _GrxPattern {
    gboolean  is_pixmap;              /* nonzero for pixmaps */
    GrxBitmap bitmap;                 /* fill bitmap */
    GrxPixmap pixmap;                 /* fill pixmap */
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
 *
 * Draw pattern for line drawings -- specifies both the:
 *   (1) fill pattern, and the
 *   (2) custom line drawing option
 */
struct _GrxLinePattern {
    GrxPattern     *pattern;         /* fill pattern */
    GrxLineOptions *options;         /* width + dash pattern */
};

GType grx_pattern_get_type(void);

GrxPattern *grx_pattern_create_pixmap(const guint8 *pixels, gint w, gint h,
                                      const GrxColorTable colors);
GrxPattern *grx_pattern_create_pixmap_from_bits(const guint8 *bits, gint w, gint h,
                                                GrxColor fgc, GrxColor bgc);
GrxPattern *grx_pattern_create_pixmap_from_context(GrxContext *src);

GrxPattern *grx_pattern_copy(GrxPattern *pattern);
void grx_pattern_free(GrxPattern *pattern);

void grx_draw_line_with_pattern(gint x1, gint y1, gint x2, gint y2, GrxLinePattern *line_pattern);
void grx_draw_box_with_pattern(gint x1, gint y1, gint x2, gint y2, GrxLinePattern *line_pattern);
void grx_draw_circle_with_pattern(gint xc, gint yc, gint r, GrxLinePattern *line_pattern);
void grx_draw_ellipse_with_pattern(gint xc, gint yc, gint xa, gint ya, GrxLinePattern *line_pattern);
void grx_draw_circle_arc_with_pattern(gint xc, gint yc, gint r, gint start, gint end,
                                      GrxArcStyle style, GrxLinePattern *line_pattern);
void grx_draw_ellipse_arc_with_pattern(gint xc, gint yc, gint xa, gint ya, gint start, gint end,
                                       GrxArcStyle style, GrxLinePattern *line_pattern);
void grx_draw_polyline_with_pattern(gint n_points, GrxPoint *points, GrxLinePattern *line_pattern);
void grx_draw_polygon_with_pattern(gint n_points, GrxPoint *points, GrxLinePattern *line_pattern);

void grx_draw_filled_pixel_with_pattern(gint x, gint y, GrxPattern *pattern);
void grx_draw_filled_line_with_pattern(gint x1, gint y1, gint x2, gint y2, GrxPattern *pattern);
void grx_draw_filled_box_with_pattern(gint x1, gint y1, gint x2, gint y2,GrxPattern *pattern);
void grx_draw_filled_circle_with_pattern(gint xc, gint yc, gint r,GrxPattern *pattern);
void grx_draw_filled_ellipse_with_pattern(gint xc, gint yc, gint xa, gint ya, GrxPattern *pattern);
void grx_draw_filled_circle_arc_with_pattern(gint xc, gint yc, gint r, gint start, gint end,
                                             GrxArcStyle style, GrxPattern *pattern);
void grx_draw_filled_ellipse_arc_with_pattern(gint xc, gint yc, gint xa, gint ya, gint start, gint end,
                                              GrxArcStyle style,GrxPattern *pattern);
void grx_draw_filled_convex_polygon_with_pattern(gint n_points, GrxPoint *points, GrxPattern *pattern);
void grx_draw_filled_polygon_with_pattern(gint n_points, GrxPoint *points, GrxPattern *pattern);
void grx_flood_fill_with_pattern(gint x, gint y, GrxColor border, GrxPattern *pattern);

void grx_draw_char_with_pattern(gint chr, gint x, gint y, const GrxTextOptions *opt, GrxPattern *pattern);
void grx_draw_string_with_pattern(gpointer text, gint length, gint x, gint y,
                                  const GrxTextOptions *opt, GrxPattern *pattern);
void grx_draw_string_with_pattern_ext(gpointer text, gint length, gint x, gint y,
                                      const GrxTextOptions *opt, GrxPattern *pattern);

#endif /* __GRX_PATTERN_H__ */
