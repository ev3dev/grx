/**
 ** lfb8.c ---- the 256 color Super VGA linear frame buffer driver
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu].
 **
 ** This file is part of the GRX graphics library.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 ** Andrzej Lawa [FidoNet: Andrzej Lawa 2:480/19.77]
 **
 **/

/* some systems map LFB in normal user space (eg. Linux/svgalib) */
/* near pointer stuff is equal to ram stuff :)                   */
/* in this is the far pointer code using %fs descriptor          */
#ifndef LFB_BY_NEAR_POINTER

/* -------------------------------------------------------------------- */

#define FAR_ACCESS
#include "driver8.h"

/* -------------------------------------------------------------------- */

GrxFrameDriver _GrFrameDriverSVGA8_LFB = {
    .mode               = GRX_FRAME_MODE_LFB_8BPP, /* frame mode */
    .rmode              = GRX_FRAME_MODE_RAM_8BPP, /* compatible RAM frame mode */
    .is_video           = TRUE,                    /* onscreen */
    .row_align          = 4,                       /* scan line width alignment */
    .num_planes         = 1,                       /* number of planes */
    .bits_per_pixel     = 8,                       /* bits per pixel */
    .max_plane_size     = 8*16*1024L*1024L,        /* max plane size the code can handle */
    .init               = NULL,
    .readpixel          = readpixel,
    .drawpixel          = drawpixel,
    .drawline           = drawline,
    .drawhline          = drawhline,
    .drawvline          = drawvline,
    .drawblock          = drawblock,
    .drawbitmap         = drawbitmap,
    .drawpattern        = drawpattern,
    .bitblt             = bitblit,
    .bltv2r             = bltv2r,
    .bltr2v             = bltr2v,
    .getindexedscanline = getindexedscanline,
    .putscanline        = putscanline,
};

#endif
