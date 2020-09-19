/*
 * ftable.c ---- a table of available frame drivers
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

#include "grdriver.h"
#include "libgrx.h"

GrxFrameDriver *_GrFrameDriverTable[] = {
#if defined(XF86DGA_FRAMEBUFFER) || !defined(__WIN32__)
    &_GrFrameDriverMONO01_LFB, &_GrFrameDriverMONO10_LFB, &_GrFrameDriverCFB2_LFB,
    &_GrFrameDriverSVGA8_LFB, &_GrFrameDriverSVGA16_LFB, &_GrFrameDriverSVGA24_LFB,
    &_GrFrameDriverSVGA32L_LFB, &_GrFrameDriverSVGA32H_LFB,
#endif
#if defined(__XWIN__)
    &_GrFrameDriverXWIN8, &_GrFrameDriverXWIN16, &_GrFrameDriverXWIN24,
    &_GrFrameDriverXWIN32L, &_GrFrameDriverXWIN32H,
#endif
#if defined(__WIN32__)
    &_GrFrameDriverWIN32_8, &_GrFrameDriverWIN32_24,
#endif
    /* now the drivers for RAM based context */
    &_GrFrameDriverRAM1, &_GrFrameDriverRAM2, &_GrFrameDriverRAM8, &_GrFrameDriverRAM16,
    &_GrFrameDriverRAM24, &_GrFrameDriverRAM32L, &_GrFrameDriverRAM32H, NULL
};
