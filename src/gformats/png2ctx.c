/*
 * png2ctx.c ---- loads a context from a PNG file
 *
 * Copyright (C) 2001 Mariano Alvarez Fernandez
 * [e-mail: malfer@teleline.es]
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gio/gio.h>
#include <glib.h>
#include <png.h>

#include <grx/context.h>
#include <grx/draw.h>
#include <grx/error.h>
#include <grx/extents.h>

#ifndef png_jmpbuf
#define png_jmpbuf(png_ptr) ((png_ptr)->jmpbuf)
#endif

static gboolean readpng(FILE *f, GrxContext *grc, int use_alpha);
static gboolean querypng(FILE *f, int *w, int *h);

/**
 * grx_context_load_from_png:
 * @context: (nullable): Context to be loaded or %NULL to use the global context
 * @filename: (type filename): Name of jpeg file
 * @use_alpha: if %TRUE, use alpha channel if available
 * @error: pointer to hold an error or %NULL to ignore
 *
 * Load a context from a PNG file
 *
 * If context dimensions are less than png dimensions,
 * the routine loads as much as it can
 *
 * If color mode is not in RGB mode, the routine allocates as
 * many colors as it can
 *
 * Returns: %TRUE on success, otherwise %FALSE
 */
gboolean grx_context_load_from_png(
    GrxContext *grc, const char *pngfn, int use_alpha, GError **error)
{
    GrxContext grcaux;
    FILE *f;
    gboolean r;

    f = fopen(pngfn, "rb");
    if (f == NULL) {
        g_set_error(error, G_IO_ERROR, g_io_error_from_errno(errno),
            "Failed to open '%s'", pngfn);
        return FALSE;
    }

    grx_save_current_context(&grcaux);
    if (grc != NULL)
        grx_set_current_context(grc);
    r = readpng(f, grc, use_alpha);
    grx_set_current_context(&grcaux);

    fclose(f);

    if (!r) {
        g_set_error(
            error, GRX_ERROR, GRX_ERROR_PNG_ERROR, "Error while reading '%s'", pngfn);
    }

    return r;
}

/**
 * grx_query_png_file:
 * @filename: (type filename): Name of jpeg file
 * @width: (out): return pnm width
 * @height: (out): return pnm height
 *
 * Query width and height data from a PNG file
 *
 * Returns: %TRUE on success, otherwise %FALSE
 */
gboolean grx_query_png_file(const char *pngfn, int *width, int *height)
{
    FILE *f;
    int r;

    f = fopen(pngfn, "rb");
    if (f == NULL) {
        return FALSE;
    }

    r = querypng(f, width, height);

    fclose(f);

    return r;
}

static gboolean readpng(FILE *f, GrxContext *grc, int use_alpha)
{
    png_struct *png_ptr = NULL;
    png_info *info_ptr = NULL;
    png_byte buf[8];
    png_byte *png_pixels = NULL;
    png_byte **row_pointers = NULL;
    png_byte *pix_ptr = NULL;
    png_uint_32 row_bytes;
    png_uint_32 width;
    png_uint_32 height;
    int bit_depth;
    int color_type;
    int alpha_present;
    int interlace_method;
    int i, x, y;
    unsigned char r, g, b;
    int alpha = 0;
    unsigned char ro, go, bo;
    int maxwidth, maxheight;
    GrxColor *pColors = NULL;

    /* is it a PNG file? */
    if (fread(buf, 1, 8, f) != 8)
        return FALSE;
    if (!png_check_sig(buf, 8))
        return FALSE;

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return FALSE;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return FALSE;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return FALSE;
    }

    png_init_io(png_ptr, f);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
        &interlace_method, NULL, NULL);

    /* tell libpng to strip 16 bit/color files down to 8 bits/color */
    if (bit_depth == 16)
        png_set_strip_16(png_ptr);
    /* expand paletted colors into true RGB triplets */
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_expand(png_ptr);
    /* expand grayscale images to the full 8 bits from 1, 2, or 4 bits/pixel */
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand(png_ptr);
    /* expand paletted or RGB images with transparency to full alpha channels
       so the data will be available as RGBA quartets. */
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_expand(png_ptr);
    /* transform grayscale images into rgb */
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);
    /* prevent warnings about interlace method */
    if (interlace_method > PNG_INTERLACE_NONE
        && interlace_method < PNG_INTERLACE_LAST) {
        png_set_interlace_handling(png_ptr);
    }

    /* we don't do gamma correction by now */

    png_read_update_info(png_ptr, info_ptr);
    png_get_IHDR(
        png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

    if (color_type == PNG_COLOR_TYPE_RGB)
        alpha_present = 0;
    else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        alpha_present = 1;
    else {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return FALSE;
    }

    row_bytes = png_get_rowbytes(png_ptr, info_ptr);

    png_pixels = (png_byte *)malloc(row_bytes * height * sizeof(png_byte));
    if (png_pixels == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return FALSE;
    }

    row_pointers = (png_byte **)malloc(height * sizeof(png_bytep));
    if (row_pointers == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        free(png_pixels);
        png_pixels = NULL;
        return FALSE;
    }

    for (i = 0; i < height; i++)
        row_pointers[i] = png_pixels + i * row_bytes;

    png_read_image(png_ptr, row_pointers);

    png_read_end(png_ptr, info_ptr);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    /* write data to context */
    maxwidth = (width > grx_get_width()) ? grx_get_width() : width;
    maxheight = (height > grx_get_height()) ? grx_get_height() : height;

    pColors = malloc(maxwidth * sizeof(GrxColor));
    if (pColors == NULL) {
        free(row_pointers);
        row_pointers = NULL;
        free(png_pixels);
        png_pixels = NULL;
        return FALSE;
    }

    for (y = 0; y < maxheight; y++) {
        pix_ptr = row_pointers[y];
        if (alpha_present && use_alpha) {
            memcpy(pColors, grx_get_scanline(0, maxwidth - 1, y, NULL),
                sizeof(GrxColor) * maxwidth);
        }
        for (x = 0; x < width; x++) {
            r = *pix_ptr++;
            g = *pix_ptr++;
            b = *pix_ptr++;
            alpha = 0;
            if (alpha_present)
                alpha = *pix_ptr++;
            if (x < maxwidth) {
                if (alpha_present && use_alpha) {
                    grx_color_query(pColors[x], &ro, &go, &bo);
                    r = r * (alpha / 255.0) + ro * ((255 - alpha) / 255.0);
                    g = g * (alpha / 255.0) + go * ((255 - alpha) / 255.0);
                    b = b * (alpha / 255.0) + bo * ((255 - alpha) / 255.0);
                }
                pColors[x] = grx_color_get(r, g, b);
            }
        }
        grx_put_scanline(0, maxwidth - 1, y, pColors, GRX_COLOR_MODE_WRITE);
    }

    if (pColors)
        free(pColors);
    if (row_pointers)
        free(row_pointers);
    if (png_pixels)
        free(png_pixels);

    return TRUE;
}

static gboolean querypng(FILE *f, int *w, int *h)
{
    png_struct *png_ptr = NULL;
    png_info *info_ptr = NULL;
    png_byte buf[8];
    png_uint_32 width;
    png_uint_32 height;
    int bit_depth;
    int color_type;

    /* is it a PNG file? */
    if (fread(buf, 1, 8, f) != 8)
        return FALSE;
    if (!png_check_sig(buf, 8))
        return FALSE;

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
        return FALSE;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return FALSE;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return FALSE;
    }

    png_init_io(png_ptr, f);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    png_get_IHDR(
        png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
    *w = width;
    *h = height;

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return TRUE;
}
