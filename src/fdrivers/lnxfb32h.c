/**
 ** lnxfb32h.c ---- the 32bpp linux frame buffer driver (high)
 **
 ** Copyright (c) 2019 Mariano Alvarez Fernandez
 ** [e-mail: malfer@telefonica.net]
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
 ** Reading individual pixels from the linux fb is a slow operation, so the
 ** idea of this framedriver is minimize and group reads. Probably using
 ** a double buffer will be better because you have never to read from the
 ** fb, but it doubles the memory required.
 **
 ** This driver doesn't use all the memfill, mempeek infraestructure, only
 ** C standard bit operations and C standard functions.
 **/

#include "libgrx.h"
#include "grdriver.h"
#include "arith.h"
#include "allocate.h"

#define PIX2COL(col) ((col)>>8)
#define COL2PIX(col) ((col)<<8)

#include "lnxfb32.h"

GrFrameDriver _GrFrameDriverLNXFB_32H = {
    GR_frameLNXFB_32H,          /* frame mode */
    GR_frameRAM32H,             /* compatible RAM frame mode */
    TRUE,                       /* onscreen */
    4,                          /* line width alignment */
    1,                          /* number of planes */
    32,                         /* bits per pixel */
    32*16*1024L*1024L,          /* max plane size the code can handle */
    NULL,
    readpixel,
    drawpixel,
    drawline,
    drawhline,
    drawvline,
    drawblock,
    drawbitmap,
    drawpattern,
    bitbltv2v,
    bitbltv2r, //v2r
    bitbltr2v, //r2v
    getindexedscanline,
    putscanline
};
