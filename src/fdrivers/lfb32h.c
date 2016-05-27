/*
 * lfb32h.c ---- the 16M color padded SVGA linear frame buffer driver (high)
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu].
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
 *
 * Contributions by: (See "doc/credits.doc" for details)
 * Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 * Andrzej Lawa [FidoNet: Andrzej Lawa 2:480/19.77]
 */

/* some systems map LFB in normal user space (eg. Linux/svgalib) */
/* near pointer stuff is equal to ram stuff :)                   */
/* in this is the far pointer code using %fs descriptor          */
#ifndef LFB_BY_NEAR_POINTER

/* -------------------------------------------------------------------- */

#define PIX2COL(col) ((col)>>8)
#define COL2PIX(col) ((col)<<8)
#define FAR_ACCESS
#include "driver32.h"

/* -------------------------------------------------------------------- */

GrxFrameDriver _GrFrameDriverSVGA32H_LFB = {
    .mode               = GRX_FRAME_MODE_LFB_32BPP_HIGH, /* frame mode */
    .rmode              = GRX_FRAME_MODE_RAM_32BPP_HIGH, /* compatible RAM frame mode */
    .is_video           = TRUE,                          /* onscreen */
    .row_align          = 4,                             /* line width alignment */
    .num_planes         = 1,                             /* number of planes */
    .bits_per_pixel     = 32,                            /* bits per pixel */
    .max_plane_size     = 32*16*1024L*1024L,             /* max plane size the code can handle */
    .init               = NULL,
    .readpixel          = readpixel,
    .drawpixel          = drawpixel,
    .drawline           = drawline,
    .drawhline          = drawhline,
    .drawvline          = drawvline,
    .drawblock          = drawblock,
    .drawbitmap         = drawbitmap,
    .drawpattern        = drawpattern,
    .bitblt             = bitblt,
    .bltv2r             = bltv2r,
    .bltr2v             = bltr2v,
    .getindexedscanline = _GrFrDrvGenericGetIndexedScanline,
    .putscanline        = _GrFrDrvGenericPutScanline,
};

#endif
