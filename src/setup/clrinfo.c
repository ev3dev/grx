/**
 ** CLRINFO.C ---- the color info data structure
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#undef         GrColorInfo

const
struct _GR_colorInfo * const GrColorInfo = &_GrColorInfo;
struct _GR_colorInfo _GrColorInfo = {
    16,                                /* ncolors: initted for 16 color VGA modes */
    0,                                /* nfree */
    0,                                /* black */
    15,                                /* white */
    FALSE,                        /* RBG         */
    { 6,    6,          6        },        /* precision */
    { 5,    5,          5        },        /* bit position */
    { 0xfc, 0xfc, 0xfc  },        /* mask */
    { 2,    2,          2        },        /* round */
    { 2,    2,          2        },        /* shift */
    2,                                /* normalization */
    {                                /* first 16 EGA/VGA colors */
        { 0,   0,   0        },        /* black */
        { 0,   0,   128 },        /* blue */
        { 0,   128, 0        },        /* green */
        { 0,   128, 128 },        /* cyan */
        { 128, 0,   0        },        /* red */
        { 128, 0,   128 },        /* magenta */
        { 128, 64,  0        },        /* brown */
        { 128, 128, 128 },        /* light gray */
        { 64,  64,  64  },        /* dark gray */
        { 64,  64,  255 },        /* light blue */
        { 64,  255, 64  },        /* light green */
        { 64,  255, 255 },        /* light cyan */
        { 255, 64,  64  },        /* light red */
        { 255, 64,  255 },        /* light magenta */
        { 255, 255, 64  },        /* yellow */
        { 255, 255, 255 }        /* white */
    }
};

