/*
 * makepat.c
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
 *
 * Copyright (C) 1992, Csaba Biegl
 *   820 Stirrup Dr, Nashville, TN, 37221
 *   csaba@vuse.vanderbilt.edu
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

#include <glib-object.h>

#include <grx/pixmap.h>

#include "colors.h"
#include "globals.h"
#include "libgrx.h"

#define MY_MEMORY  1 /* set if my context memory */
#define MY_CONTEXT 2 /* set if my context structure */

#define BEST_MAX_LINE    128
#define BEST_MAX_CONTEXT 2048L

G_DEFINE_BOXED_TYPE(GrxPixmap, grx_pixmap, grx_pixmap_copy, grx_pixmap_free);

/*
 * try to replicate a pixmap for faster bitblt-s
 * in bitplane modes it is especially desirable to replicate until
 * the width is a multiple of 8
 */
static int _GrBestPixmapWidth(int wdt, int hgt)
{
    int total = grx_screen_get_context_size(wdt, hgt);
    int linelen = grx_screen_get_line_offset(wdt);
    int factor = 1;
    int test;

    if (total == 0L)
        return 0;
    if ((test = (int)(BEST_MAX_CONTEXT / total)) > factor)
        factor = test;
    if ((test = (BEST_MAX_LINE / linelen)) < factor)
        factor = test;
    while ((factor >>= 1) != 0)
        wdt <<= 1;
    return wdt;
}

/**
 * grx_pixmap_new:
 * @pixels: (array): a two dimensional array of bytes
 * @w: the width of the pixel array
 * @h: the height of the pixel array
 * @colors: (element-type GrxColor) (nullable): color table for the pixel array
 *
 * Build a pixmap from @pixels.
 *
 * The elements of the @pixels array are used as indices in @colors. This means
 * that pixmaps created this way can use at most 256 colors. NOTE: any color
 * modifiers (GrXOR, GrOR, GrAND) OR-ed to the elements of the color table are
 * ignored.
 *
 * Returns: (nullable): a new #GrxPixmap or %NULL if there was an error.
 */
GrxPixmap *grx_pixmap_new(const unsigned char *pixels, int w, int h, const GArray *ct)
{
    GrxContext csave, cwork;
    GrxPixmap *result;
    unsigned char *src;
    int wdt, wdt2, fullw;
    int hgt;
    GrxColor color;

    if ((fullw = _GrBestPixmapWidth(w, h)) <= 0)
        return NULL;
    result = (GrxPixmap *)malloc(sizeof(GrxPixmap));
    if (result == NULL)
        return NULL;

    if (!grx_context_new(fullw, h, NULL, &cwork)) {
        free(result);
        return NULL;
    }
    csave = *CURC;
    *CURC = cwork;
    for (hgt = 0; hgt < h; hgt++) {
        for (wdt2 = fullw; (wdt2 -= w) >= 0;) {
            src = (unsigned char *)pixels;
            for (wdt = 0; wdt < w; wdt++) {
                color = *src++;
                if (ct != NULL) {
                    color = grx_color_lookup(ct, color);
                }
                (*CURC->gc_driver->drawpixel)(wdt2 + wdt, hgt, (color & C_COLOR));
            }
        }
        pixels += w;
    }
    *CURC = csave;
    result->source = cwork.frame;
    result->source.memory_flags = (MY_CONTEXT | MY_MEMORY);
    result->is_pixmap = TRUE;
    result->width = fullw;
    result->height = h;
    result->mode = 0;
    result->context = NULL;

    return result;
}

/**
 * grx_pixmap_new_from_bits:
 * @bits: the bitmap data
 * @w: the width of the bitmap data
 * @h: the height of the bitmap data
 * @fg: the forground color
 * @bg: the background color
 *
 * Builds a pixmap fill pattern from bitmap data. It is useful if the width of
 * the bitmap pattern is not eight as such bitmap patterns can not be used to
 * build a GrBitmap structure.
 *
 * Returns: (nullable): a new #GrxPixmap or %NULL if there was an error.
 */
GrxPixmap *grx_pixmap_new_from_bits(
    const unsigned char *bits, int w, int h, GrxColor fgc, GrxColor bgc)
{
    GrxContext csave, cwork;
    GrxPixmap *result;
    unsigned char *src;
    int wdt, wdt2, fullw;
    int hgt, mask, byte;

    if ((fullw = _GrBestPixmapWidth(w, h)) <= 0)
        return NULL;
    result = (GrxPixmap *)malloc(sizeof(GrxPixmap));
    if (result == NULL)
        return NULL;

    if (!grx_context_new(fullw, h, NULL, &cwork)) {
        free(result);
        return NULL;
    }
    csave = *CURC;
    *CURC = cwork;
    fgc &= C_COLOR;
    bgc &= C_COLOR;
    for (hgt = 0; hgt < h; hgt++) {
        for (wdt2 = fullw; (wdt2 -= w) >= 0;) {
            src = (unsigned char *)bits;
            mask = byte = 0;
            for (wdt = w; --wdt >= 0;) {
                if ((mask >>= 1) == 0) {
                    mask = 0x80;
                    byte = *src++;
                }
                (*CURC->gc_driver->drawpixel)(
                    wdt2 + wdt, hgt, ((byte & mask) ? fgc : bgc));
            }
        }
        bits += (w + 7) >> 3;
    }
    *CURC = csave;
    result->source = cwork.frame;
    result->source.memory_flags = (MY_CONTEXT | MY_MEMORY);
    result->is_pixmap = TRUE;
    result->width = fullw;
    result->height = h;
    result->mode = 0;
    result->context = NULL;

    return result;
}

/**
 * grx_pixmap_new_from_context:
 * @context: the context
 *
 * Converts a graphics context to a pixmap fill pattern.
 *
 * This is useful when the pixmap can be created with graphics drawing
 * operations. NOTE: the pixmap pattern and the original context share the
 * drawing RAM, thus if the context is redrawn the fill pattern changes as well.
 *
 * Returns: (nullable): a new #GrxPixmap or %NULL if there was an error.
 */
GrxPixmap *grx_pixmap_new_from_context(GrxContext *context)
{
    GrxPixmap *result;

    if (context->gc_is_on_screen) {
        return NULL;
    }
    result = malloc(sizeof(*result));
    if (!result) {
        return NULL;
    }
    result->source = context->frame;
    result->source.memory_flags = MY_CONTEXT;
    result->is_pixmap = TRUE;
    result->width = context->x_max + 1;
    result->height = context->y_max + 1;
    result->mode = 0;
    if (context->gc_memory_flags & 1 /*MYCONTEXT*/) {
        result->context = grx_context_ref(context);
    }

    return result;
}

/**
 * grx_pixmap_copy:
 * @pixmap: the pixmap
 *
 * Make a copy of @pixmap.
 *
 * Returns: a new #GrxPixmap.
 */
GrxPixmap *grx_pixmap_copy(GrxPixmap *p)
{
    GrxContext src_ctx, *copy_ctx;
    GrxPixmap *copy;

    grx_context_new_full(
        p->source.driver->mode, p->width, p->height, p->source.base_address, &src_ctx);
    copy_ctx =
        grx_context_new_full(p->source.driver->mode, p->width, p->height, NULL, NULL);
    g_return_val_if_fail(copy_ctx != NULL, NULL);

    grx_context_bit_blt(copy_ctx, 0, 0, &src_ctx, 0, 0, p->width - 1, p->height - 1,
        GRX_COLOR_MODE_WRITE);
    copy = grx_pixmap_new_from_context(copy_ctx);
    grx_context_unref(copy_ctx);

    return copy;
}

/**
 * grx_pixmap_free:
 * @pixmap: the pixmap
 *
 * Frees any dynamically allocated memory associated with the pixmap.
 */
void grx_pixmap_free(GrxPixmap *p)
{
    if (!p) {
        return;
    }
    if (p->source.memory_flags & MY_MEMORY) {
        free(p->source.base_address);
    }
    if (p->context) {
        grx_context_unref(p->context);
    }
    if (p->source.memory_flags & MY_CONTEXT) {
        free(p);
    }
}
