/**
 ** RAM1.C ---- the mono system RAM frame driver
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "grdriver.h"
#include "libgrx.h"
#include "mempeek.h"

static
#ifdef __GNUC__
inline
#endif
long readpixel(GrFrame *c,int x,int y)
{
        return((long)((
            c->gf_baseaddr[0][(y * c->gf_lineoffset) + (x >> 3)] >> 
            (7 - (x & 7))
        ) & 1));
}

static
#ifdef __GNUC__
inline
#endif
void drawpixel(int x,int y,long color)
{
        char far *ptr = &CURC->gc_baseaddr[0][(y * CURC->gc_lineoffset) + (x >> 3)];
        uint cval = ((uint)color & 1) << (7 - (x &= 7));
        switch(C_OPER(color)) {
            case C_XOR: *ptr ^=  cval; break;       
            case C_OR:  *ptr |=  cval; break;       
            case C_AND: *ptr &= ~cval; break;
            default:    *ptr  = (*ptr & (~0x80 >> x)) | cval; break;
        }
}

static
#include "fdrivers/generic/hline.c"

static
#include "fdrivers/generic/vline.c"

static
#include "fdrivers/generic/block.c"

static
#include "fdrivers/generic/line.c"

static
#include "fdrivers/generic/bitmap.c"

static
#include "fdrivers/generic/pattern.c"

static
#include "fdrivers/generic/bitblt.c"

GrFrameDriver _GrFrameDriverRAM1 = {
    GR_frameRAM1,                /* frame mode */
    GR_frameUndef,                /* compatible RAM frame mode */
    FALSE,                        /* onscreen */
    4,                                /* scan line width alignment */
    1,                                /* number of planes */
    1,                                /* bits per pixel */
#ifdef __TURBOC__
    65520L,                        /* max plane size the code can handle */
#else
    16*1024L*1024L,                /* max plane size the code can handle */
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
    NULL
};

