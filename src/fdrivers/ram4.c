/**
 ** RAM4.C ---- the 16 color RAM frame driver
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "grdriver.h"
#include "libgrx.h"
#include "arith.h"
#include "memfill.h"

static
#ifdef __GNUC__
inline
#endif
long readpixel(GrFrame *c,int x,int y)
{
        uint offs = (y * c->gf_lineoffset) + (x >> 3);
        uint mask = 0x80 >> (x &= 7);
        return((long)(
            (((c->gf_baseaddr[0][offs] & mask)           ) |
             ((c->gf_baseaddr[1][offs] & mask) << 1) |
             ((c->gf_baseaddr[2][offs] & mask) << 2) |
             ((c->gf_baseaddr[3][offs] & mask) << 3)
            ) >> (7 - x)
        ));
}

static
#ifdef __GNUC__
inline
#endif
void drawpixel(int x,int y,long color)
{
        uint offs = (y * CURC->gc_lineoffset) + (x >> 3);
        uint mask = 0x80 >> (x &= 7);
        uint cval = (uint)color << (7 - x);
        int  op;
        switch(op = C_OPER(color)) {
          case C_WRITE:
          case C_AND:
            CURC->gc_baseaddr[0][offs] &= ((cval     ) | ~mask);
            CURC->gc_baseaddr[1][offs] &= ((cval >> 1) | ~mask);
            CURC->gc_baseaddr[2][offs] &= ((cval >> 2) | ~mask);
            CURC->gc_baseaddr[3][offs] &= ((cval >> 3) | ~mask);
            if(op != C_WRITE) break;
          case C_OR:
            CURC->gc_baseaddr[0][offs] |= ((cval     ) & mask);
            CURC->gc_baseaddr[1][offs] |= ((cval >> 1) & mask);
            CURC->gc_baseaddr[2][offs] |= ((cval >> 2) & mask);
            CURC->gc_baseaddr[3][offs] |= ((cval >> 3) & mask);
            break;
          default:
            CURC->gc_baseaddr[0][offs] ^= ((cval     ) & mask);
            CURC->gc_baseaddr[1][offs] ^= ((cval >> 1) & mask);
            CURC->gc_baseaddr[2][offs] ^= ((cval >> 2) & mask);
            CURC->gc_baseaddr[3][offs] ^= ((cval >> 3) & mask);
            break;
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

GrFrameDriver _GrFrameDriverRAM4 = {
    GR_frameRAM4,                /* frame mode */
    GR_frameUndef,                /* compatible RAM frame mode */
    FALSE,                        /* onscreen */
    4,                                /* scan line width alignment */
    4,                                /* number of planes */
    4,                                /* bits per pixel */
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
    NULL,
};

