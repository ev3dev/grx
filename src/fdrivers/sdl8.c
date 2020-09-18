/*
 * sdl8.c ---- the 256 color SDL frame buffer driver
 *
 * Copyright (C) 2004 Dimitar Zhekov
 * [e-mail: jimmy@is-vn.bg]
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

/* some systems map LFB in normal user space (eg. Linux/svgalib) */
/* near pointer stuff is equal to ram stuff :)                   */
/* in this is the far pointer code using %fs descriptor          */
#ifndef LFB_BY_NEAR_POINTER

/* -------------------------------------------------------------------- */

#define FAR_ACCESS
#include "driver8.h"

#define bitblt bitblit
#include "sdlframe.h"

/* -------------------------------------------------------------------- */

GrxFrameDriver _GrFrameDriverSDL8 = {
    .mode               = GR_frameSDL8,            /* frame mode */
    .rmode              = GRX_FRAME_MODE_RAM_8BPP, /* compatible RAM frame mode */
    .is_video           = TRUE,                    /* onscreen */
    .row_align          = 4,                       /* scan line width alignment */
    .bits_per_pixel     = 8,                       /* bits per pixel */
    .max_plane_size     = 8*16*1024L*1024L,        /* max plane size the code can handle */
    .init               = NULL,
    .readpixel          = readpixel,
    .drawpixel          = sdl_drawpixel,
    .drawline           = sdl_drawline,
    .drawhline          = sdl_drawhline,
    .drawvline          = sdl_drawvline,
    .drawblock          = sdl_drawblock,
    .drawbitmap         = sdl_drawbitmap,
    .drawpattern        = sdl_drawpattern,
    .bitblt             = sdl_bitblt,
    .bltv2r             = bltv2r,
    .bltr2v             = sdl_bltr2v,
    .getindexedscanline = getindexedscanline,
    .putscanline        = sdl_putscanline,
};

#endif
