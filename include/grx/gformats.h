/*
 * gformats.h
 *
 * Copyright (c) 2015,2017 David Lechner <david@lechnology.com>
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

/**
 * SECTION:gformats
 * @short_description: Loading and saving image data to standard formats (JPEG, PNG, PNM)
 * @title: Image File Formats
 * @section_id: gformats
 * @include: grx-3.0.h
 *
 * These functions are used to load data from disk and store data to disk using
 * standard file formats.
 */

#ifndef __GRX_GFORMATS_H__
#define __GRX_GFORMATS_H__

#include <glib.h>

#include <grx/common.h>

/**
 * GrxPnmFormat:
 * @GRX_PNM_FORMAT_ASCII_PBM: P1: black and white (1bpp) ascii file format
 * @GRX_PNM_FORMAT_ASCII_PGM: P2: grayscale ascii file format
 * @GRX_PNM_FORMAT_ASCII_PPM: P3: RGB ascii file format
 * @GRX_PNM_FORMAT_BINARY_PBM: P4: black and white (1bpp) binary file format
 * @GRX_PNM_FORMAT_BINARY_PGM: P5: grayscale binary file format
 * @GRX_PNM_FORMAT_BINARY_PPM: P6: RGB binary file format
 *
 * PNM file formats.
 *
 * Note: GRX supports load/save of binaries formats (4,5,6) only.
 */
typedef enum {
    GRX_PNM_FORMAT_ASCII_PBM  = 1,
    GRX_PNM_FORMAT_ASCII_PGM  = 2,
    GRX_PNM_FORMAT_ASCII_PPM  = 3,
    GRX_PNM_FORMAT_BINARY_PBM = 4,
    GRX_PNM_FORMAT_BINARY_PGM = 5,
    GRX_PNM_FORMAT_BINARY_PPM = 6,
} GrxPnmFormat;

/* The PNM functions */

gboolean grx_context_save_to_pbm(GrxContext *context, const gchar *filename, const gchar *comment, GError **error);
gboolean grx_context_save_to_pgm(GrxContext *context, const gchar *filename, const gchar *comment, GError **error);
gboolean grx_context_save_to_ppm(GrxContext *context, const gchar *filename, const gchar *comment, GError **error);
gboolean grx_context_load_from_pnm(GrxContext *context, const gchar *filename, GError **error);
gboolean grx_check_pnm_file(const gchar *filename, GrxPnmFormat *format, gint *width, gint *height, gint *maxval);
gboolean grx_context_load_from_pnm_data(GrxContext *context, const guint8 *buffer);
gboolean grx_check_pnm_data(const guint8 *buffer, GrxPnmFormat *format, gint *width, gint *height, gint *maxval);

gboolean grx_context_save_to_png(GrxContext *context, const gchar *filename, GError **error);
gboolean grx_context_load_from_png(GrxContext *context, const gchar *filename, gboolean use_alpha, GError **error);
gboolean grx_check_png_file(const gchar *filename, gint *width, gint *height);

gboolean grx_context_load_from_jpeg(GrxContext *context, const gchar *filename, gint scale, GError **error);
gboolean grx_check_jpeg_file(const gchar *filename, gint *width, gint *height);
gboolean grx_context_save_to_jpeg(GrxContext *context, const gchar *filename, gint quality, GError **error);
gboolean grx_context_save_to_jpeg_grayscale(GrxContext *context, const gchar *filename, gint quality, GError **error);

#endif /* __GRX_GFORMATS_H__ */
