/*
 * color.h
 *
 * Copyright (c) 2015-2017 David Lechner <david@lechnology.com>
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

#ifndef __GRX_COLOR_H__
#define __GRX_COLOR_H__

#include <glib.h>

/**
 * SECTION:color
 * @short_description: Colors and color tables
 * @title: Color Management
 * @section_id: color
 * @include: grx-3.0.h
 *
 * The library supports two models for color management. In the 'indirect' (or
 * color table) model, color values are indices to a color table. The color
 * table slots will be allocated with the highest resolution supported by the
 * hardware with respect to the component color intensities. In the 'direct'
 * (or RGB) model, color values map directly into component color intensities
 * with non-overlapping bitfields of the color index representing the component
 * colors.
 *
 * The color table model is supported up to 256 colors. The RGB model is
 * supported for 256 colors and up.
 *
 * In the RGB model the color index map to component color intensities depend
 * on the video mode set, so it can't be assumed the component color bitfields.
 */

/**
 * GrxColor:
 *
 * 32-bit integer for storing color information.
 *
 * The lower 24 bits are the actual color value and the upper 8 bits are the
 * #GrxColorMode.
 */
typedef guint32 GrxColor;

/**
 * GrxColorMode:
 * @GRX_COLOR_MODE_WRITE: Overwrite any existing color
 * @GRX_COLOR_MODE_XOR: XOR this color with the existing color
 * @GRX_COLOR_MODE_OR: OR this color with the existing color
 * @GRX_COLOR_MODE_AND: AND this color with the existing color
 * @GRX_COLOR_MODE_IMAGE: Overwrite existing color unless this color is #GRX_COLOR_NONE.
 *                        Only works with bitblt operations.
 *
 * Values added to a #GrxColor to affect how the color is used in drawing
 * operations.
 */
typedef enum {
    GRX_COLOR_MODE_WRITE = 0UL,
    GRX_COLOR_MODE_XOR   = 0x01000000UL,
    GRX_COLOR_MODE_OR    = 0x02000000UL,
    GRX_COLOR_MODE_AND   = 0x03000000UL,
    GRX_COLOR_MODE_IMAGE = 0x04000000UL,
} GrxColorMode;

/**
 * GRX_COLOR_VALUE_MASK:
 *
 * Used to get the color portion of a #GrxColor.
 */
#define GRX_COLOR_VALUE_MASK 0x00ffffffUL

/**
 * GRX_COLOR_MODE_MASK:
 *
 * Used to get the #GrxColorMode portion of a #GrxColor.
 */
#define GRX_COLOR_MODE_MASK 0xff000000UL

/**
 * GRX_COLOR_NONE:
 *
 * Used to indicate transparency in bitblt operations.
 *
 * See #GrxColorMode.
 */
#define GRX_COLOR_NONE ((GrxColor)(GRX_COLOR_MODE_XOR | 0))

GrxColor grx_color_alloc(guint8 r, guint8 g, guint8 b);
GrxColor grx_color_alloc_inline(guint8 r, guint8 g, guint8 b);
GrxColor grx_color_alloc2(guint32 hcolor);
GrxColor grx_color_alloc2_inline(guint32 hcolor);
void grx_color_free(GrxColor c);

GrxColor grx_color_get_value(GrxColor c);
GrxColorMode grx_color_get_mode(GrxColor c);
GrxColor grx_color_to_write_mode(GrxColor c);
GrxColor grx_color_to_xor_mode(GrxColor c);
GrxColor grx_color_to_or_mode(GrxColor c);
GrxColor grx_color_to_and_mode(GrxColor c);
GrxColor grx_color_to_image_mode(GrxColor c);

/**
 * GrxColorPaletteType:
 * @GRX_COLOR_PALETTE_TYPE_GRAYSCALE: RGB color values will be converted to grayscale.
 * @GRX_COLOR_PALETTE_TYPE_COLOR_TABLE: Colors use a lookup table.
 * @GRX_COLOR_PALETTE_TYPE_RGB: RGB values are used directly.
 */
typedef enum {
    GRX_COLOR_PALETTE_TYPE_GRAYSCALE,
    GRX_COLOR_PALETTE_TYPE_COLOR_TABLE,
    GRX_COLOR_PALETTE_TYPE_RGB,
} GrxColorPaletteType;

#ifndef __GI_SCANNER__
/* TODO: This struct should probably be private */

/*
 * color system info structure (all [3] arrays are [r,g,b])
 */
extern const struct _GR_colorInfo {
    GrxColor       ncolors;             /* number of colors */
    GrxColor       nfree;               /* number of unallocated colors */
    GrxColor       black;               /* the black color */
    GrxColor       white;               /* the white color */
    GrxColorPaletteType palette_type;
    unsigned int  prec[3];              /* color field precisions */
    unsigned int  pos[3];               /* color field positions */
    unsigned int  mask[3];              /* masks for significant bits */
    unsigned int  round[3];             /* add these for rounding */
    unsigned int  shift[3];             /* shifts for (un)packing color */
    unsigned int  norm;                 /* normalization for (un)packing */
    struct {                            /* color table for non-RGB modes */
        unsigned char r,g,b;            /* loaded components */
        unsigned int  defined:1;        /* r,g,b values are valid if set */
        unsigned int  writable:1;       /* can be changed by 'grx_color_cell_set' */
        unsigned long int nused;        /* usage count */
    } ctable[256];
} * const GrColorInfo;

#endif /* __GI_SCANNER__ */

GrxColor grx_color_get_black(void);
GrxColor grx_color_get_white(void);

/**
 * GRX_COLOR_BLACK:
 *
 * Convenience macro for grx_color_get_black().
 */
#define GRX_COLOR_BLACK grx_color_get_black()

/**
 * GRX_COLOR_WHITE:
 *
 * Convenience macro for grx_color_get_white().
 */
#define GRX_COLOR_WHITE grx_color_get_white()

GrxColor grx_color_build_rgb(guint8 r, guint8 g, guint8 b);
GrxColor grx_color_build_rgb_round(guint8 r, guint8 g, guint8 b);
GrxColor grx_color_build_grayscale(guint8 r, guint8 g, guint8 b);
guint8 grx_color_get_red(GrxColor c);
guint8 grx_color_get_green(GrxColor c);
guint8 grx_color_get_blue(GrxColor c);

void grx_color_query(GrxColor c, guint8 *r, guint8 *g, guint8 *b);
void grx_color_query_inline(GrxColor c, guint8 *r, guint8 *g, guint8 *b);
void grx_color_query2(GrxColor c, guint32 *hcolor);
void grx_color_query2_inline(GrxColor c, guint32 *hcolor);

/**
 * GrxEgaColorIndex:
 * @GRX_EGA_COLOR_INDEX_BLACK: black
 * @GRX_EGA_COLOR_INDEX_BLUE: blue
 * @GRX_EGA_COLOR_INDEX_GREEN: green
 * @GRX_EGA_COLOR_INDEX_CYAN: cyan
 * @GRX_EGA_COLOR_INDEX_RED: red
 * @GRX_EGA_COLOR_INDEX_MAGENTA: magenta
 * @GRX_EGA_COLOR_INDEX_BROWN: brown
 * @GRX_EGA_COLOR_INDEX_LIGHT_GRAY: light gray
 * @GRX_EGA_COLOR_INDEX_DARK_GRAY: dark gray
 * @GRX_EGA_COLOR_INDEX_LIGHT_BLUE: light blue
 * @GRX_EGA_COLOR_INDEX_LIGHT_GREEN: light green
 * @GRX_EGA_COLOR_INDEX_LIGHT_CYAN: light cyan
 * @GRX_EGA_COLOR_INDEX_LIGHT_RED: light red
 * @GRX_EGA_COLOR_INDEX_LIGHT_MAGENTA: light magenta
 * @GRX_EGA_COLOR_INDEX_YELLOW: yellow
 * @GRX_EGA_COLOR_INDEX_WHITE: white
 *
 * Indexes for use with the array returned by grx_color_alloc_ega_colors().
 */
typedef enum {
    GRX_EGA_COLOR_INDEX_BLACK,
    GRX_EGA_COLOR_INDEX_BLUE,
    GRX_EGA_COLOR_INDEX_GREEN,
    GRX_EGA_COLOR_INDEX_CYAN,
    GRX_EGA_COLOR_INDEX_RED,
    GRX_EGA_COLOR_INDEX_MAGENTA,
    GRX_EGA_COLOR_INDEX_BROWN,
    GRX_EGA_COLOR_INDEX_LIGHT_GRAY,
    GRX_EGA_COLOR_INDEX_DARK_GRAY,
    GRX_EGA_COLOR_INDEX_LIGHT_BLUE,
    GRX_EGA_COLOR_INDEX_LIGHT_GREEN,
    GRX_EGA_COLOR_INDEX_LIGHT_CYAN,
    GRX_EGA_COLOR_INDEX_LIGHT_RED,
    GRX_EGA_COLOR_INDEX_LIGHT_MAGENTA,
    GRX_EGA_COLOR_INDEX_YELLOW,
    GRX_EGA_COLOR_INDEX_WHITE,
} GrxEgaColorIndex;

GrxColor *grx_color_alloc_ega_colors(void);

/**
 * GrxColorCell:
 *
 * A mutable color value.
 */
typedef GrxColor GrxColorCell;

GrxColorCell grx_color_cell_alloc(void);
void grx_color_cell_set(GrxColorCell c, guint8 r, guint8 g, guint8 b);
void grx_color_cell_free(GrxColorCell c);

/* color info */

void     grx_color_info_reset_colors(void);
void     grx_color_info_set_palette_type_rgb(void);
GrxColorPaletteType grx_color_info_get_palette_type(void);
void     grx_color_info_refresh_colors(void);

gint     grx_color_info_n_colors(void);
gint     grx_color_info_n_free_colors(void);


gint     grx_color_info_get_save_buffer_size(void);
void     grx_color_info_save_colors(gpointer buffer);
void     grx_color_info_restore_colors(gpointer buffer);

#ifndef GRX_SKIP_INLINES
#define grx_color_get_value(c)         ((GrxColor)(c) & GRX_COLOR_VALUE_MASK)
#define grx_color_get_mode(c)          ((GrxColor)(c) & GRX_COLOR_MODE_MASK)
#define grx_color_to_write_mode(c)     (grx_color_get_value(c) | GRX_COLOR_MODE_WRITE)
#define grx_color_to_xor_mode(c)       (grx_color_get_value(c) | GRX_COLOR_MODE_XOR)
#define grx_color_to_or_mode(c)        (grx_color_get_value(c) | GRX_COLOR_MODE_OR)
#define grx_color_to_and_mode(c)       (grx_color_get_value(c) | GRX_COLOR_MODE_AND)
#define grx_color_to_image_mode(c)     (grx_color_get_value(c) | GRX_COLOR_MODE_IMAGE)
#define grx_color_info_n_colors()      (GrColorInfo->ncolors)
#define grx_color_info_n_free_colors() (GrColorInfo->nfree)
#define grx_color_info_get_pallete_type() (GrxColorInfo->palette_type)
#define grx_color_get_black() (                                           \
    (GrColorInfo->black == GRX_COLOR_NONE) ?                                   \
    (grx_color_get_black)() :                                             \
    GrColorInfo->black                                                         \
)
#define grx_color_get_white() (                                           \
    (GrColorInfo->white == GRX_COLOR_NONE) ?                                   \
    (grx_color_get_white)() :                                             \
    GrColorInfo->white                                                         \
)
#define grx_color_build_rgb(r,g,b) ((                             \
    ((GrxColor)((int)(r) & GrColorInfo->mask[0]) << GrColorInfo->shift[0]) |   \
    ((GrxColor)((int)(g) & GrColorInfo->mask[1]) << GrColorInfo->shift[1]) |   \
    ((GrxColor)((int)(b) & GrColorInfo->mask[2]) << GrColorInfo->shift[2])     \
    ) >> GrColorInfo->norm                                                     \
)
#define grx_color_build_rgb_round(r,g,b) grx_color_build_rgb( \
    (((unsigned int)(r)) > GrColorInfo->mask[0]) ? 255 : (unsigned int)(r) + GrColorInfo->round[0], \
    (((unsigned int)(g)) > GrColorInfo->mask[1]) ? 255 : (unsigned int)(g) + GrColorInfo->round[1], \
    (((unsigned int)(b)) > GrColorInfo->mask[2]) ? 255 : (unsigned int)(b) + GrColorInfo->round[2]  \
)
#define grx_color_build_grayscale(r,g,b) \
    ((((r) * 19595 + (g) * 38470 + (b) * 7471) >> 16) >> (8 - g_bit_nth_lsf(GrColorInfo->ncolors, 0)))

#define grx_color_get_red(c) (                                      \
    (int)(((GrxColor)(c) << GrColorInfo->norm) >> GrColorInfo->shift[0]) &     \
    (GrColorInfo->mask[0])                                                     \
)
#define grx_color_get_green(c) (                                    \
    (int)(((GrxColor)(c) << GrColorInfo->norm) >> GrColorInfo->shift[1]) &     \
    (GrColorInfo->mask[1])                                                     \
)
#define grx_color_get_blue(c) (                                     \
    (int)(((GrxColor)(c) << GrColorInfo->norm) >> GrColorInfo->shift[2]) &     \
    (GrColorInfo->mask[2])                                                     \
)
#define grx_color_alloc_inline(r,g,b) (                                        \
    (GrColorInfo->palette_type == GRX_COLOR_PALETTE_TYPE_RGB) ?                \
    grx_color_build_rgb_round(r,g,b) :                                         \
    grx_color_alloc(r,g,b)                                                     \
)
#define grx_color_alloc2(hcolor) (grx_color_alloc(       \
    ((hcolor & 0xff0000) >> 16),                                               \
    ((hcolor & 0x00ff00) >> 8),                                                \
    (hcolor & 0x0000ff))                                                       \
)
#define grx_color_alloc2_inline(hcolor) (grx_color_alloc_inline( \
    ((hcolor & 0xff0000) >> 16),                                               \
    ((hcolor & 0x00ff00) >> 8),                                                \
    (hcolor & 0x0000ff))                                                       \
)
#define grx_color_query_inline(c,r,g,b) do {                            \
    if (GrColorInfo->palette_type != GRX_COLOR_PALETTE_TYPE_COLOR_TABLE) {     \
    *(r) = grx_color_get_red(c);                                    \
    *(g) = grx_color_get_green(c);                                  \
    *(b) = grx_color_get_blue(c);                                   \
    break;                                                                     \
    }                                                                          \
    if(((GrxColor)(c) < GrColorInfo->ncolors) &&                               \
       (GrColorInfo->ctable[(GrxColor)(c)].defined)) {                         \
    *(r) = GrColorInfo->ctable[(GrxColor)(c)].r;                               \
    *(g) = GrColorInfo->ctable[(GrxColor)(c)].g;                               \
    *(b) = GrColorInfo->ctable[(GrxColor)(c)].b;                               \
    break;                                                                     \
    }                                                                          \
    *(r) = *(g) = *(b) = 0;                                                    \
} while(0)
#define grx_color_query2_inline(c,hcolor) do {                          \
    if (GrColorInfo->palette_type != GRX_COLOR_PALETTE_TYPE_COLOR_TABLE) {     \
    *(hcolor) = grx_color_get_red(c) << 16;                         \
    *(hcolor) |= grx_color_get_green(c) << 8;                       \
    *(hcolor) |= grx_color_get_blue(c);                             \
    break;                                                                     \
    }                                                                          \
    if(((GrxColor)(c) < GrColorInfo->ncolors) &&                               \
       (GrColorInfo->ctable[(GrxColor)(c)].defined)) {                         \
    *(hcolor) = GrColorInfo->ctable[(GrxColor)(c)].r;                          \
    *(hcolor) = GrColorInfo->ctable[(GrxColor)(c)].g;                          \
    *(hcolor) = GrColorInfo->ctable[(GrxColor)(c)].b;                          \
    break;                                                                     \
    }                                                                          \
    *(hcolor) = 0;                                                             \
} while(0)
#endif  /* GRX_SKIP_INLINES */

/*
 * color table (for primitives using several colors):
 *   it is an array of colors with the first element being
 *   the number of colors in the table
 */
typedef GrxColor *GrxColorTable;

#define GRX_COLOR_TABLE_GET_SIZE(table) (                                      \
        (table) ? (unsigned int)((table)[0]) : 0U                              \
)
#define GRX_COLOR_TABLE_GET_COLOR(table,index) (                               \
        ((unsigned)(index) < GRX_COLOR_TABLE_GET_SIZE(table)) ?                \
        (table)[((unsigned)(index)) + 1] :                                     \
        GRX_COLOR_NONE                                                         \
)
#define GRX_COLOR_TABLE_GET_ALLOC_SIZE(ncolors)    ((ncolors) + 1)

#endif /* __GRX_COLOR_H__ */
