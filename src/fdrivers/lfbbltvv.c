/*
 * lfbbltvv.c ---- packed video->video blit operations for
 *                 8,16,24 and 32bpp Super VGA linear frame buffer modes
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
 *
 */

/* some systems map LFB in normal user space (eg. Linux/svgalib) */
/* near pointer stuff is equal to ram stuff :)                   */
/* in this is the far pointer code using %fs descriptor          */
#ifndef LFB_BY_NEAR_POINTER

/* name of blit function to be created */
#define BLITFUNC _GrFrDrvPackedBitBltV2V_LFB

/* destination is video */
#define WRITE_FAR _f
/* source is video */
#define READ_FAR _f

/* need a selector before video access */
#define BLITSEL  dst->selector

/* source and destination may overlap */
#define BLIT_CAN_OVERLAP

#include "pblit_nb.h"

#endif
