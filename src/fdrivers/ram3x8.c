/**
 ** RAM3X8.C ---- the 16M color planar (image) system RAM frame driver
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
        int offs = (y * c->gf_lineoffset) + x;
        return(
            ((ulong)(uchar)peek_b(&c->gf_baseaddr[0][offs])      ) |
            ((ulong)(uchar)peek_b(&c->gf_baseaddr[1][offs]) <<  8) |
            ((ulong)(uchar)peek_b(&c->gf_baseaddr[2][offs]) << 16)
        );        
}

static
#ifdef __GNUC__
inline
#endif
void drawpixel(int x,int y,long color)
{
        int c1 = (int)(color);
        int c2 = (int)(color) >> 8;
        int c3 = (int)(color >> 16);
        int offs = (y * CURC->gc_lineoffset) + x;
        char far *p1 = &CURC->gc_baseaddr[0][offs];
        char far *p2 = &CURC->gc_baseaddr[1][offs];
        char far *p3 = &CURC->gc_baseaddr[2][offs];
        switch(C_OPER(color)) {
          case C_XOR:
            poke_b_xor(p1,c1);
            poke_b_xor(p2,c2);
            poke_b_xor(p3,c3);
            break;
          case C_OR:
            poke_b_or(p1,c1);
            poke_b_or(p2,c2);
            poke_b_or(p3,c3);
            break;
          case C_AND:
            poke_b_and(p1,c1);
            poke_b_and(p2,c2);
            poke_b_and(p3,c3);
            break;
          default:
            poke_b(p1,c1);
            poke_b(p2,c2);
            poke_b(p3,c3);
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

GrFrameDriver _GrFrameDriverRAM3x8 = {
    GR_frameRAM3x8,                /* frame mode */
    GR_frameUndef,                /* compatible RAM frame mode */
    FALSE,                        /* onscreen */
    4,                                /* scan line width alignment */
    3,                                /* number of planes */
    24,                                /* bits per pixel */
#ifdef __TURBOC__
    65520L,                        /* max plane size the code can handle */
#else
    8*16*1024L*1024L,                /* max plane size the code can handle */
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

