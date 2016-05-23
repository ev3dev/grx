/*
 * gformats.h
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

#ifndef __GRX_GFORMATS_H__
#define __GRX_GFORMATS_H__

#include <glib.h>

#include <grx/common.h>

/* ================================================================== */
/*                           PNM FUNCTIONS                            */
/* ================================================================== */

/**
 * GrxPnmFormat:
 *
 *  The PNM formats, grx support load/save of
 *  binaries formats (4,5,6) only
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

int grx_save_current_context_to_pbm(GrxContext *grc, gchar *pbmfn, gchar *docn);
int grx_save_current_context_to_pgm(GrxContext *grc, gchar *pgmfn, gchar *docn);
int grx_save_current_context_to_ppm(GrxContext *grc, gchar *ppmfn, gchar *docn);
int grx_context_load_from_pnm(GrxContext *grc, gchar *pnmfn);
int grx_check_pnm_file(gchar *pnmfn, gint *width, gint *height, gint *maxval);
int grx_context_load_from_pnm_data(GrxContext *grc, const guint8 *buffer);
int grx_check_pnm_data(const guint8 *buffer, gint *width, gint *height, gint *maxval);

/* ================================================================== */
/*                           PNG FUNCTIONS                            */
/*  these functions may not be installed or available on all system   */
/* ================================================================== */

int grx_is_png_supported(void);
int grx_save_current_context_to_png(GrxContext *grc, gchar *pngfn);
int grx_context_load_from_png(GrxContext *grc, gchar *pngfn, gboolean use_alpha);
int grx_check_png_file(gchar *pngfn, gint *width, gint *height);

/* ================================================================== */
/*                          JPEG FUNCTIONS                            */
/*  these functions may not be installed or available on all system   */
/* ================================================================== */

int grx_is_jpeg_supported(void);
int grx_context_load_from_jpeg(GrxContext *grc, gchar *jpegfn, gint scale);
int grx_check_jpeg_file(gchar *jpegfn, gint *width, gint *height);
int grx_save_current_context_to_jpeg(GrxContext *grc, gchar *jpegfn, gint quality);
int grx_save_current_context_to_jpeg_grayscale(GrxContext *grc, gchar *jpegfn, gint quality);

#endif /* __GRX_GFORMATS_H__ */
