/*
 * grdriver.h ---- utilities for frame buffer and video card drivers
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

#ifndef __GRDRIVER_H_INCLUDED__
#define __GRDRIVER_H_INCLUDED__

#include <glib.h>

#include <grx/mode.h>

/*
** The following definitions shouldn't be used in portable
** or binary distributed programs for compatibility with
** future versions of GRX
*/

/*
 * Standard frame drivers in GRX
 */

/* Linear Framebuffer Modes : */
G_GNUC_INTERNAL extern GrxFrameDriver
    _GrFrameDriverMONO01_LFB; /* mono, 0 = white, 1 = black */
G_GNUC_INTERNAL extern GrxFrameDriver
    _GrFrameDriverMONO10_LFB; /* mono, 1 = white, 0 = black */
G_GNUC_INTERNAL extern GrxFrameDriver
    _GrFrameDriverCFB2_LFB; /* 4 color packed pixels */
G_GNUC_INTERNAL extern GrxFrameDriver
    _GrFrameDriverSVGA8_LFB; /* (Super) VGA 256 color */
G_GNUC_INTERNAL extern GrxFrameDriver
    _GrFrameDriverSVGA16_LFB; /* Super VGA 32768/65536 color */
G_GNUC_INTERNAL extern GrxFrameDriver
    _GrFrameDriverSVGA24_LFB; /* Super VGA 16M color */
G_GNUC_INTERNAL extern GrxFrameDriver
    _GrFrameDriverSVGA32L_LFB; /* Super VGA 16M color padded #1 */
G_GNUC_INTERNAL extern GrxFrameDriver
    _GrFrameDriverSVGA32H_LFB; /* Super VGA 16M color padded #2 */

G_GNUC_INTERNAL extern GrxFrameDriver _GrFrameDriverRAM1;   /* mono */
G_GNUC_INTERNAL extern GrxFrameDriver _GrFrameDriverRAM2;   /* 4 color */
G_GNUC_INTERNAL extern GrxFrameDriver _GrFrameDriverRAM8;   /* 256 color */
G_GNUC_INTERNAL extern GrxFrameDriver _GrFrameDriverRAM16;  /* 32768/65536 color */
G_GNUC_INTERNAL extern GrxFrameDriver _GrFrameDriverRAM24;  /* 16M color */
G_GNUC_INTERNAL extern GrxFrameDriver _GrFrameDriverRAM32L; /* 16M color padded #1 */
G_GNUC_INTERNAL extern GrxFrameDriver _GrFrameDriverRAM32H; /* 16M color padded #2 */

/*
 * frame driver function types used inside GRX
 */
typedef GrxColor (*_GR_readPix)(GrxFrame *, int, int);
typedef void (*_GR_drawPix)(int, int, GrxColor);
typedef void (*_GR_blitFunc)(GrxFrame *dst, int dx, int dy, GrxFrame *src, int x, int y,
    int w, int h, GrxColor op);
typedef GrxColor *(*_GR_getIndexedScanline)(
    GrxFrame *c, int x, int y, int w, int *indx);
typedef void (*_GR_putScanline)(int x, int y, int w, const GrxColor *scl, GrxColor op);

/*
 * Frame driver utility functions
 */
G_GNUC_INTERNAL void _GrFrDrvGenericBitBlt(GrxFrame *dst, int dx, int dy, GrxFrame *src,
    int x, int y, int w, int h, GrxColor op);
G_GNUC_INTERNAL void _GrFrDrvPackedBitBltR2R(GrxFrame *dst, int dx, int dy,
    GrxFrame *src, int x, int y, int w, int h, GrxColor op);

G_GNUC_INTERNAL void _GrFrDrvGenericPutScanline(
    int x, int y, int w, const GrxColor *scl, GrxColor op);
G_GNUC_INTERNAL GrxColor *_GrFrDrvGenericGetIndexedScanline(
    GrxFrame *c, int x, int y, int w, int *indx);

G_GNUC_INTERNAL void _GrFrDrvGenericStretchBlt(GrxFrame *dst, int dx, int dy, int dw,
    int dh, GrxFrame *src, int sx, int sy, int sw, int sh, GrxColor op);

/*
 * Commonly used video driver data structures
 */
G_GNUC_INTERNAL extern GrxVideoModeExt _GrViDrvEGAVGAtextModeExt;
G_GNUC_INTERNAL extern GrxVideoModeExt _GrViDrvEGAVGAcustomTextModeExt;

GrxVideoMode *_gr_select_mode(
    GrxVideoDriver *drv, int w, int h, int bpp, int txt, unsigned int *ep);

#endif /* whole file */
