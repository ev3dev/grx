/*
 * color.h
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

#ifndef __GRX_COLOR_H__
#define __GRX_COLOR_H__

#include <glib.h>

/* ================================================================== */
/*                            COLOR STUFF                             */
/* ================================================================== */

/* need unsigned 32 bit integer for color stuff */
typedef guint32 GrxColor;

/**
 * GrxColorMode:
 *
 * Flags to 'OR' to colors for various operations
 */
typedef enum {
    GRX_COLOR_MODE_WRITE = 0UL,           /* write color */
    GRX_COLOR_MODE_XOR   = 0x01000000UL,  /* to "XOR" any color to the screen */
    GRX_COLOR_MODE_OR    = 0x02000000UL,  /* to "OR" to the screen */
    GRX_COLOR_MODE_AND   = 0x03000000UL,  /* to "AND" to the screen */
    GRX_COLOR_MODE_IMAGE = 0x04000000UL,  /* BLIT: write, except given color */
} GrxColorMode;

#define GRX_COLOR_VALUE_MASK 0x00ffffffUL /* color value mask */
#define GRX_COLOR_MODE_MASK  0xff000000UL /* color operation mask */
#define GRX_COLOR_NONE ((GrxColor)(GRX_COLOR_MODE_XOR | 0)) /* GRX_COLOR_NONE is used for "no" color */

GrxColor grx_color_get_value(GrxColor c);
GrxColorMode grx_color_get_mode(GrxColor c);
GrxColor grx_color_to_write_mode(GrxColor c);
GrxColor grx_color_to_xor_mode(GrxColor c);
GrxColor grx_color_to_or_mode(GrxColor c);
GrxColor grx_color_to_and_mode(GrxColor c);
GrxColor grx_color_to_image_mode(GrxColor c);

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
    unsigned int  RGBmode;              /* set when RGB mode */
    unsigned int  prec[3];              /* color field precisions */
    unsigned int  pos[3];               /* color field positions */
    unsigned int  mask[3];              /* masks for significant bits */
    unsigned int  round[3];             /* add these for rounding */
    unsigned int  shift[3];             /* shifts for (un)packing color */
    unsigned int  norm;                 /* normalization for (un)packing */
    struct {                            /* color table for non-RGB modes */
        unsigned char r,g,b;            /* loaded components */
        unsigned int  defined:1;        /* r,g,b values are valid if set */
        unsigned int  writable:1;       /* can be changed by 'grx_color_info_set_color' */
        unsigned long int nused;        /* usage count */
    } ctable[256];
} * const GrColorInfo;

#endif /* __GI_SCANNER__ */

void     grx_color_info_reset_colors(void);
void     grx_color_info_set_rgb_color_mode(void);
void     grx_color_info_refresh_colors(void);

gint     grx_color_info_n_colors(void);
gint     grx_color_info_n_free_colors(void);

GrxColor grx_color_info_get_black(void);
GrxColor grx_color_info_get_white(void);

GrxColor grx_color_info_build_rgb_color_t(gint r, gint g, gint b);
GrxColor grx_color_info_build_rgb_color_r(gint r, gint g, gint b);
gint     grx_color_info_get_red_value(GrxColor c);
gint     grx_color_info_get_green_value(GrxColor c);
gint     grx_color_info_get_blue_value(GrxColor c);

GrxColor grx_color_info_alloc_color(gint r, gint g, gint b);    /* shared, read-only */
GrxColor grx_color_info_alloc_color_id(gint r, gint g, gint b); /* potentially inlined version */
GrxColor grx_color_info_alloc_color2(glong hcolor);             /* shared, read-only, 0xRRGGBB */
GrxColor grx_color_info_alloc_color2_id(glong hcolor);          /* potentially inlined version */
GrxColor grx_color_info_alloc_cell(void);                       /* unshared, read-write */

/**
 * grx_color_info_alloc_ega_colors:
 *
 * Returns: (array fixed-size=16): array of color indexes
 */
GrxColor *grx_color_info_alloc_ega_colors(void); /* shared, read-only standard EGA colors */

void     grx_color_info_set_color(GrxColor c, gint r, gint g, gint b);
void     grx_color_info_free_color(GrxColor c);
void     grx_color_info_free_cell(GrxColor c);

void     grx_color_info_query_color(GrxColor c, gint *r, gint *g, gint *b);
void     grx_color_info_query_color_id(GrxColor c, gint *r, gint *g, gint *b);
void     grx_color_info_query_color2(GrxColor c, glong *hcolor);
void     grx_color_info_query_color2_id(GrxColor c, glong *hcolor);

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
#define grx_color_info_get_black() (                                           \
    (GrColorInfo->black == GRX_COLOR_NONE) ?                                   \
    (grx_color_info_get_black)() :                                             \
    GrColorInfo->black                                                         \
)
#define grx_color_info_get_white() (                                           \
    (GrColorInfo->white == GRX_COLOR_NONE) ?                                   \
    (grx_color_info_get_white)() :                                             \
    GrColorInfo->white                                                         \
)
#define grx_color_info_build_rgb_color_t(r,g,b) ((                             \
    ((GrxColor)((int)(r) & GrColorInfo->mask[0]) << GrColorInfo->shift[0]) |   \
    ((GrxColor)((int)(g) & GrColorInfo->mask[1]) << GrColorInfo->shift[1]) |   \
    ((GrxColor)((int)(b) & GrColorInfo->mask[2]) << GrColorInfo->shift[2])     \
    ) >> GrColorInfo->norm                                                     \
)
#define grx_color_info_build_rgb_color_r(r,g,b) grx_color_info_build_rgb_color_t( \
    (((unsigned int)(r)) > GrColorInfo->mask[0]) ? 255 : (unsigned int)(r) + GrColorInfo->round[0], \
    (((unsigned int)(g)) > GrColorInfo->mask[1]) ? 255 : (unsigned int)(g) + GrColorInfo->round[1], \
    (((unsigned int)(b)) > GrColorInfo->mask[2]) ? 255 : (unsigned int)(b) + GrColorInfo->round[2]  \
)
#define grx_color_info_get_red_value(c) (                                      \
    (int)(((GrxColor)(c) << GrColorInfo->norm) >> GrColorInfo->shift[0]) &     \
    (GrColorInfo->mask[0])                                                     \
)
#define grx_color_info_get_green_value(c) (                                    \
    (int)(((GrxColor)(c) << GrColorInfo->norm) >> GrColorInfo->shift[1]) &     \
    (GrColorInfo->mask[1])                                                     \
)
#define grx_color_info_get_blue_value(c) (                                     \
    (int)(((GrxColor)(c) << GrColorInfo->norm) >> GrColorInfo->shift[2]) &     \
    (GrColorInfo->mask[2])                                                     \
)
#define grx_color_info_alloc_color_id(r,g,b) (GrColorInfo->RGBmode ?           \
    grx_color_info_build_rgb_color_r(r,g,b) :                                  \
    grx_color_info_alloc_color(r,g,b)                                          \
)
#define grx_color_info_alloc_color2(hcolor) (grx_color_info_alloc_color(       \
    ((hcolor & 0xff0000) >> 16),                                               \
    ((hcolor & 0x00ff00) >> 8),                                                \
    (hcolor & 0x0000ff))                                                       \
)
#define grx_color_info_alloc_color2_id(hcolor) (grx_color_info_alloc_color_id( \
    ((hcolor & 0xff0000) >> 16),                                               \
    ((hcolor & 0x00ff00) >> 8),                                                \
    (hcolor & 0x0000ff))                                                       \
)
#define grx_color_info_query_color_id(c,r,g,b) do {                            \
    if(GrColorInfo->RGBmode) {                                                 \
    *(r) = grx_color_info_get_red_value(c);                                    \
    *(g) = grx_color_info_get_green_value(c);                                  \
    *(b) = grx_color_info_get_blue_value(c);                                   \
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
#define grx_color_info_query_color2_id(c,hcolor) do {                          \
    if(GrColorInfo->RGBmode) {                                                 \
    *(hcolor) = grx_color_info_get_red_value(c) << 16;                         \
    *(hcolor) |= grx_color_info_get_green_value(c) << 8;                       \
    *(hcolor) |= grx_color_info_get_blue_value(c);                             \
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
