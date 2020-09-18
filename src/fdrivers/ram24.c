/*
 * ram24.c ---- the 16M color system RAM frame driver
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
 */


#undef FAR_ACCESS
#include "driver24.h"

GrxFrameDriver _GrFrameDriverRAM24 = {
    .mode               = GRX_FRAME_MODE_RAM_24BPP, /* frame mode */
    .rmode              = GRX_FRAME_MODE_UNDEFINED, /* compatible RAM frame mode */
    .is_video           = FALSE,                    /* onscreen */
    .row_align          = 4,                        /* scan line width alignment */
    .bits_per_pixel     = 24,                       /* bits per pixel */
    .max_plane_size     = 24*16*1024L*1024L,        /* max plane size the code can handle */
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
    .bltv2r             = NULL,
    .bltr2v             = NULL,
    .getindexedscanline = _GrFrDrvGenericGetIndexedScanline,
    .putscanline        = _GrFrDrvGenericPutScanline,
};

/* some systems map LFB in normal user space (eg. Linux/svgalib) */
/* near pointer stuff is equal to ram stuff :)                   */
#ifdef LFB_BY_NEAR_POINTER

/* always do RAM to RAM blit. May result in     **
** bottom first blits but this shouldn't matter */

GrxFrameDriver _GrFrameDriverSVGA24_LFB = {
    .mode               = GRX_FRAME_MODE_LFB_24BPP, /* frame mode */
    .rmode              = GRX_FRAME_MODE_RAM_24BPP, /* compatible RAM frame mode */
    .is_video           = TRUE,                     /* onscreen */
    .row_align          = 4,                        /* line width alignment */
    .bits_per_pixel     = 24,                       /* bits per pixel */
    .max_plane_size     = 24*16*1024L*1024L,        /* max plane size the code can handle */
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
    .bltv2r             = bitblt,
    .bltr2v             = bitblt,
    .getindexedscanline = _GrFrDrvGenericGetIndexedScanline,
    .putscanline        = _GrFrDrvGenericPutScanline,
};
#endif /* defined(LFB_BY_NEAR_POINTER) */
