/*
 * ctx2pnm.c ---- saves a context in a PNM file
 *
 * Copyright (C) 2000,2001 Mariano Alvarez Fernandez
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

#include <grx/context.h>
#include <grx/draw.h>
#include <grx/error.h>
#include <grx/extents.h>

/**
 * grx_context_save_to_pbm:
 * @context: (nullable): Context to be saved or %NULL to use the global context
 * @filename: (type filename): Name of pbm file
 * @comment: (nullable): string saved like a comment in the pbm file
 * @error: pointer to hold an error or %NULL to ignore
 *
 * Dump a context in a PBM file (black and white)
 *
 * This routine works both in RGB and palette modes
 * If the pixel color isn't Black it asumes White
 *
 * Returns: %TRUE on success, otherwise %FALSE
 */
gboolean grx_context_save_to_pbm(
    GrxContext *grc, const char *pbmfn, const char *docn, GError **error)
{
    FILE *f;
    GrxContext grcaux;
    char cab[81];
    int currentbyte = 0, currentbit = 7;
    int x, y;

    if ((f = fopen(pbmfn, "wb")) == NULL) {
        g_set_error(error, G_IO_ERROR, g_io_error_from_errno(errno),
            "Failed to open '%s'", pbmfn);
        return FALSE;
    }

    grx_save_current_context(&grcaux);
    if (grc != NULL)
        grx_set_current_context(grc);
    sprintf(cab, "P4\n#");
    fwrite(cab, 1, strlen(cab), f);
    if (docn != NULL)
        fwrite(docn, 1, strlen(docn), f);
    sprintf(cab, "\n%d %d\n", grx_get_width(), grx_get_height());
    fwrite(cab, 1, strlen(cab), f);
    for (y = 0; y < grx_get_height(); y++) {
        for (x = 0; x < grx_get_width(); x++) {
            if (grx_get_pixel_at(x, y) == GRX_COLOR_BLACK) {
                currentbyte |= 1 << currentbit;
            }
            currentbit--;
            if (currentbit < 0) {
                fwrite(&currentbyte, 1, 1, f);
                currentbyte = 0;
                currentbit = 7;
            }
        }
        if (currentbit < 7) {
            fwrite(&currentbyte, 1, 1, f);
            currentbyte = 0;
            currentbit = 7;
        }
    }
    grx_set_current_context(&grcaux);
    fclose(f);

    return TRUE;
}

/**
 * grx_context_save_to_pgm:
 * @context: (nullable): Context to be saved or %NULL to use the global context
 * @filename: (type filename): Name of pgm file
 * @comment: (nullable): string saved like a comment in the pgm file
 * @error: pointer to hold an error or %NULL to ignore
 *
 * Dump a context in a PGM file (gray scale)
 *
 * This routine works both in RGB and palette modes
 * The colors are quantized to gray scale using .299r + .587g + .114b
 *
 * Returns: %TRUE on success, otherwise %FALSE
 */
gboolean grx_context_save_to_pgm(
    GrxContext *grc, char *pgmfn, char *docn, GError **error)
{
    FILE *f;
    GrxContext grcaux;
    char cab[81];
    unsigned char grey;
    unsigned char rgb[3];
    int x, y;

    if ((f = fopen(pgmfn, "wb")) == NULL) {
        g_set_error(error, G_IO_ERROR, g_io_error_from_errno(errno),
            "Failed to open '%s'", pgmfn);
        return FALSE;
    }

    grx_save_current_context(&grcaux);
    if (grc != NULL)
        grx_set_current_context(grc);
    sprintf(cab, "P5\n#");
    fwrite(cab, 1, strlen(cab), f);
    if (docn != NULL)
        fwrite(docn, 1, strlen(docn), f);
    sprintf(cab, "\n%d %d\n255\n", grx_get_width(), grx_get_height());
    fwrite(cab, 1, strlen(cab), f);
    for (y = 0; y < grx_get_height(); y++)
        for (x = 0; x < grx_get_width(); x++) {
            grx_color_query(grx_get_pixel_at(x, y), &rgb[0], &rgb[1], &rgb[2]);
            grey = (0.229 * rgb[0]) + (0.587 * rgb[1]) + (0.114 * rgb[2]);
            fwrite(&grey, 1, 1, f);
        }
    grx_set_current_context(&grcaux);
    fclose(f);

    return 0;
}

/**
 * grx_context_save_to_ppm:
 * @context: (nullable): Context to be saved or %NULL to use the global context
 * @filename: (type filename): Name of ppm file
 * @comment: (nullable): string saved like a comment in the ppm file
 * @error: pointer to hold an error or %NULL to ignore
 *
 * Dump a context in a PPM file (real color)
 *
 * This routine works both in RGB and palette modes
 *
 * Returns: %TRUE on success, otherwise %FALSE
 */
gboolean grx_context_save_to_ppm(
    GrxContext *grc, char *ppmfn, char *docn, GError **error)
{
    FILE *f;
    GrxContext grcaux;
    char cab[81];
    unsigned char brgb[3];
    int x, y;
    unsigned char r, g, b;

    if ((f = fopen(ppmfn, "wb")) == NULL) {
        g_set_error(error, G_IO_ERROR, g_io_error_from_errno(errno),
            "Failed to open '%s'", ppmfn);
        return FALSE;
    }

    grx_save_current_context(&grcaux);
    if (grc != NULL)
        grx_set_current_context(grc);
    sprintf(cab, "P6\n#");
    fwrite(cab, 1, strlen(cab), f);
    if (docn != NULL)
        fwrite(docn, 1, strlen(docn), f);
    sprintf(cab, "\n%d %d\n255\n", grx_get_width(), grx_get_height());
    fwrite(cab, 1, strlen(cab), f);
    for (y = 0; y < grx_get_height(); y++)
        for (x = 0; x < grx_get_width(); x++) {
            grx_color_query(grx_get_pixel_at(x, y), &r, &g, &b);
            brgb[0] = r;
            brgb[1] = g;
            brgb[2] = b;
            fwrite(brgb, 1, 3, f);
        }
    grx_set_current_context(&grcaux);
    fclose(f);

    return 0;
}
