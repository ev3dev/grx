/**
 ** RAM24.C ---- the 16M color system RAM frame driver
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 **/


#undef FAR_ACCESS
#include "fdrivers/driver24.h"

GrFrameDriver _GrFrameDriverRAM24 = {
    GR_frameRAM24,              /* frame mode */
    GR_frameUndef,              /* compatible RAM frame mode */
    FALSE,                      /* onscreen */
    4,                          /* scan line width alignment */
    1,                          /* number of planes */
    24,                         /* bits per pixel */
#ifdef __TURBOC__
    65520L,                     /* max plane size the code can handle */
#else
    24*16*1024L*1024L,          /* max plane size the code can handle */
#endif
    NULL,
    readpixel,
    drawpixel,
    drawline,
    drawhline,
    drawvline,
    drawblock,
    drawbitmap,
    drawpattern,
    bitblt,
    NULL,
    NULL,
    _GrFrDrvGenericGetIndexedScanline,
    _GrFrDrvGenericPutScanline
};

/* some systems map LFB in normal user space (eg. Linux/svgalib) */
/* near pointer stuff is equal to ram stuff :)                   */
#ifdef LFB_BY_NEAR_POINTER

/* always do RAM to RAM blit. May result in     **
** bottom first blits but this shouldn't matter */

GrFrameDriver _GrFrameDriverSVGA24_LFB = {
    GR_frameSVGA24_LFB,         /* frame mode */
    GR_frameRAM24,              /* compatible RAM frame mode */
    TRUE,                       /* onscreen */
    4,                          /* line width alignment */
    1,                          /* number of planes */
    24,                         /* bits per pixel */
    24*16*1024L*1024L,          /* max plane size the code can handle */
    NULL,
    readpixel,
    drawpixel,
    drawline,
    drawhline,
    drawvline,
    drawblock,
    drawbitmap,
    drawpattern,
    bitblt,
    bitblt,
    bitblt,
    _GrFrDrvGenericGetIndexedScanline,
    _GrFrDrvGenericPutScanline
};
#endif /* defined(LFB_BY_NEAR_POINTER) */
