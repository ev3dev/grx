/**
 ** ftable.c ---- a table of available frame drivers
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
 **
 **/

#include "libgrx.h"
#include "grdriver.h"

GrFrameDriver *_GrFrameDriverTable[] = {
/* first the drivers for video based context */
#if defined(__MSDOS__) 
    &_GrFrameDriverEGAVGA1,
    &_GrFrameDriverEGA4,
    &_GrFrameDriverSVGA4,
    &_GrFrameDriverVGA8X,
#endif
#if defined(__MSDOS__)
    &_GrFrameDriverSVGA8,
    &_GrFrameDriverSVGA16,
    &_GrFrameDriverSVGA24,
    &_GrFrameDriverSVGA32L,
    &_GrFrameDriverSVGA32H,
#endif
#if  defined(__DJGPP__) || defined(XF86DGA_FRAMEBUFFER)
    &_GrFrameDriverSVGA8_LFB,
    &_GrFrameDriverSVGA16_LFB,
    &_GrFrameDriverSVGA24_LFB,
    &_GrFrameDriverSVGA32L_LFB,
    &_GrFrameDriverSVGA32H_LFB,
#endif
#if defined(__linux__) && !defined(__XWIN__)
    &_GrFrameDriverSVGA8_LFB,
    &_GrFrameDriverLNXFB_16,
    &_GrFrameDriverSVGA24_LFB,
    &_GrFrameDriverLNXFB_32L,
    &_GrFrameDriverLNXFB_32H,
#endif
#ifdef __XWIN__
    &_GrFrameDriverXWIN8,
    &_GrFrameDriverXWIN16,
    &_GrFrameDriverXWIN24,
    &_GrFrameDriverXWIN32L,
    &_GrFrameDriverXWIN32H,
#endif
#ifdef __WIN32__
    &_GrFrameDriverWIN32_8,
    &_GrFrameDriverWIN32_24,
#endif
/* now the drivers for RAM based context */
    &_GrFrameDriverRAM1,
    &_GrFrameDriverRAM4,
    &_GrFrameDriverRAM8,
    &_GrFrameDriverRAM16,
    &_GrFrameDriverRAM24,
    &_GrFrameDriverRAM32L,
    &_GrFrameDriverRAM32H,
    NULL
};

