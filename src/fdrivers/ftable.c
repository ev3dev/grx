/**
 ** FTABLE.C ---- a table of available frame drivers
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 **/

#include "grdriver.h"
#include "libgrx.h"

GrFrameDriver *_GrFrameDriverTable[] = {
/* first the drivers for video based context */
#if defined(__GO32__) || defined(__TURBOC__)
    &_GrFrameDriverHERC1,
#endif
#if !defined(__XWIN__)
    &_GrFrameDriverEGAVGA1,
    &_GrFrameDriverEGA4,
    &_GrFrameDriverSVGA4,
    &_GrFrameDriverSVGA8,
    &_GrFrameDriverVGA8X,
    &_GrFrameDriverSVGA16,
    &_GrFrameDriverSVGA24,
    &_GrFrameDriverSVGA32L,
    &_GrFrameDriverSVGA32H,
#endif
#if defined(__GO32__) || defined(LFB_BY_NEAR_POINTER)
    &_GrFrameDriverSVGA8_LFB,
    &_GrFrameDriverSVGA16_LFB,
    &_GrFrameDriverSVGA24_LFB,
    &_GrFrameDriverSVGA32L_LFB,
    &_GrFrameDriverSVGA32H_LFB,
#endif
#ifdef __XWIN__
    &_GrFrameDriverXWIN8,
    &_GrFrameDriverXWIN16,
    &_GrFrameDriverXWIN24,
#endif
/* now the drivers for RAM based context */
#if !defined(__XWIN__)
    &_GrFrameDriverRAM1,
    &_GrFrameDriverRAM4,
#endif
    &_GrFrameDriverRAM8,
    &_GrFrameDriverRAM16,
    &_GrFrameDriverRAM24,
#if !defined(__XWIN__)
    &_GrFrameDriverRAM32L,
    &_GrFrameDriverRAM32H,
    &_GrFrameDriverRAM3x8,
#endif
    NULL
};

