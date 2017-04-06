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
#ifdef USE_GRX_INTERNAL_DEFINITIONS

G_GNUC_INTERNAL extern GrxFrameDriver
/*
 * Standard frame drivers in GRX
 */
                                        /* Linear Framebuffer Modes : */
_GrFrameDriverMONO01_LFB,               /* mono, 0 = white, 1 = black */
_GrFrameDriverMONO10_LFB,               /* mono, 1 = white, 0 = black */
_GrFrameDriverSVGA8_LFB,                /* (Super) VGA 256 color */
_GrFrameDriverSVGA16_LFB,               /* Super VGA 32768/65536 color */
_GrFrameDriverSVGA24_LFB,               /* Super VGA 16M color */
_GrFrameDriverSVGA32L_LFB,              /* Super VGA 16M color padded #1 */
_GrFrameDriverSVGA32H_LFB,              /* Super VGA 16M color padded #2 */

_GrFrameDriverXWIN8,                    /* X 8 bpp */
_GrFrameDriverXWIN16,                   /* X 16 bpp */
_GrFrameDriverXWIN24,                   /* X 24 bpp */
_GrFrameDriverXWIN32L,                  /* X 32 bpp padded #1 */
_GrFrameDriverXWIN32H,                  /* X 32 bpp padded #2 */
        
_GrFrameDriverWIN32_4,                  /* WIN32 4 bpp */
_GrFrameDriverWIN32_8,                  /* WIN32 8 bpp */
_GrFrameDriverWIN32_24,                 /* WIN32 24 bpp */

_GrFrameDriverSDL8,                     /* SDL 8 bpp */
_GrFrameDriverSDL16,                    /* SDL 16 bpp */
_GrFrameDriverSDL24,                    /* SDL 24 bpp */
_GrFrameDriverSDL32L,                   /* SDL 32 bpp padded #2 */
_GrFrameDriverSDL32H,                   /* SDL 32 bpp padded #2 */

_GrFrameDriverRAM1,                     /* mono */
_GrFrameDriverRAM4x1,                   /* 16 color planar */
_GrFrameDriverRAM8,                     /* 256 color */
_GrFrameDriverRAM16,                    /* 32768/65536 color */
_GrFrameDriverRAM24,                    /* 16M color */
_GrFrameDriverRAM32L,                   /* 16M color padded #1 */
_GrFrameDriverRAM32H,                   /* 16M color padded #2 */
_GrFrameDriverRAM3x8,                   /* 16M color planar (image mode) */
/*
 * This is a NULL-terminated table of frame driver descriptor pointers. Users
 * can provide their own table with only the desired (or additional) drivers.
 * Otherwise the table from the GRX library is linked, which includes ALL
 * currently available drivers (i.e. the ones above).
 */
*_GrFrameDriverTable[];

/*
 * frame driver function types used inside GRX
 */
typedef GrxColor  (*_GR_readPix)(GrxFrame *,int,int);
typedef void     (*_GR_drawPix)(int,int,GrxColor);
typedef void     (*_GR_blitFunc)(GrxFrame *dst,int dx,int dy,
                                 GrxFrame *src,int  x,int  y,
                                 int w,int h,GrxColor op);
typedef GrxColor *(*_GR_getIndexedScanline)(GrxFrame *c,int x,int y,
                                               int w, int *indx);
typedef void     (*_GR_putScanline)(int x,int y,int w,
                                    const GrxColor *scl,GrxColor op);

/*
 * Frame driver utility functions
 */
G_GNUC_INTERNAL void _GrFrDrvGenericBitBlt(GrxFrame *dst,int dx,int dy,GrxFrame *src,int x,int y,int w,int h,GrxColor op);
G_GNUC_INTERNAL void _GrFrDrvPackedBitBltR2R(GrxFrame *dst,int dx,int dy,GrxFrame *src,int x,int y,int w,int h,GrxColor op);
G_GNUC_INTERNAL void _GrFrDrvPackedBitBltR2V(GrxFrame *dst,int dx,int dy,GrxFrame *src,int x,int y,int w,int h,GrxColor op);
G_GNUC_INTERNAL void _GrFrDrvPackedBitBltV2R(GrxFrame *dst,int dx,int dy,GrxFrame *src,int x,int y,int w,int h,GrxColor op);
G_GNUC_INTERNAL void _GrFrDrvPackedBitBltV2V(GrxFrame *dst,int dx,int dy,GrxFrame *src,int x,int y,int w,int h,GrxColor op);
G_GNUC_INTERNAL void _GrFrDrvPackedBitBltR2V_LFB(GrxFrame *dst,int dx,int dy,GrxFrame *src,int x,int y,int w,int h,GrxColor op);
G_GNUC_INTERNAL void _GrFrDrvPackedBitBltV2R_LFB(GrxFrame *dst,int dx,int dy,GrxFrame *src,int x,int y,int w,int h,GrxColor op);
G_GNUC_INTERNAL void _GrFrDrvPackedBitBltV2V_LFB(GrxFrame *dst,int dx,int dy,GrxFrame *src,int x,int y,int w,int h,GrxColor op);

G_GNUC_INTERNAL void _GrFrDrvGenericPutScanline(int x,int y,int w,const GrxColor *scl, GrxColor op );
G_GNUC_INTERNAL GrxColor *_GrFrDrvGenericGetIndexedScanline(GrxFrame *c,
                                           int x,int y,int w,
                                           int *indx         );

G_GNUC_INTERNAL void _GrFrDrvGenericStretchBlt(GrxFrame *dst,int dx,int dy,int dw,int dh,
                               GrxFrame *src,int sx,int sy,int sw,int sh,
                               GrxColor op);

/*
 * Commonly used video driver data structures
 */
G_GNUC_INTERNAL extern GrxVideoModeExt _GrViDrvEGAVGAtextModeExt;
G_GNUC_INTERNAL extern GrxVideoModeExt _GrViDrvEGAVGAcustomTextModeExt;

GrxVideoMode * _gr_select_mode(GrxVideoDriver *drv,int w,int h,int bpp,
                              int txt,unsigned int *ep);

#endif /* USE_GRX_INTERNAL_DEFINITIONS */

#endif /* whole file */
