/**
 ** RAM8.C ---- the 256 color system RAM frame driver
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 **/

#include "grdriver.h"
#include "libgrx.h"
#include "arith.h"
#include "docolor.h"
#include "mempeek.h"
#include "memfill.h"

static
#ifdef __GNUC__
inline
#endif
long readpixel(GrFrame *c,int x,int y)
{
        return((uchar)peek_b(&c->gf_baseaddr[0][(y * c->gf_lineoffset) + x]));
}

static
#ifdef __GNUC__
inline
#endif
void drawpixel(int x,int y,long color)
{
        char far *ptr = &CURC->gc_baseaddr[0][(y * CURC->gc_lineoffset) + x];
        switch(C_OPER(color)) {
            case C_XOR: poke_b_xor(ptr,(uchar)color); break;
            case C_OR:  poke_b_or( ptr,(uchar)color); break;
            case C_AND: poke_b_and(ptr,(uchar)color); break;
            default:    poke_b(    ptr,(uchar)color); break;
        }
}

static void drawhline(int x,int y,int w,long color)
{
    int copr = C_OPER(color);
    if(DOCOLOR8(color,copr)) {
        char far *pp = &CURC->gc_baseaddr[0][(y * CURC->gc_lineoffset) + x];
        int  cval = freplicate_b2l((int)color);
        switch(C_OPER(color)) {
            case C_XOR: repfill_b_xor(pp,cval,w); break;
            case C_OR:  repfill_b_or( pp,cval,w); break;
            case C_AND: repfill_b_and(pp,cval,w); break;
            default:    repfill_b(    pp,cval,w); break;
        }
    }
}

static void drawvline(int x,int y,int h,long color)
{
        uint lwdt = CURC->gc_lineoffset;
        long offs = umul32(y,lwdt) + x;
        int  copr = C_OPER(color);
        if(DOCOLOR8(color,copr)) {
            char far *pp = &CURC->gc_baseaddr[0][offs];
            switch(copr) {
                case C_XOR: colfill_b_xor(pp,lwdt,(int)color,h); break;
                case C_OR:  colfill_b_or( pp,lwdt,(int)color,h); break;
                case C_AND: colfill_b_and(pp,lwdt,(int)color,h); break;
                default:    colfill_b(    pp,lwdt,(int)color,h); break;
            }
        }
}

static
#include "fdrivers/generic/block.c"

static
#include "fdrivers/generic/line.c"

static
#include "fdrivers/generic/bitmap.c"

static
#include "fdrivers/generic/pattern.c"

GrFrameDriver _GrFrameDriverRAM8 = {
    GR_frameRAM8,               /* frame mode */
    GR_frameUndef,              /* compatible RAM frame mode */
    FALSE,                      /* onscreen */
    4,                          /* line width alignment */
    1,                          /* number of planes */
    8,                          /* bits per pixel */
#ifdef __TURBOC__
    65520L,                     /* max plane size the code can handle */
#else
    8*16*1024L*1024L,           /* max plane size the code can handle */
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
    _GrFrDrvPackedBitBltR2R,
    NULL,
    NULL,
};


#ifdef LFB_BY_NEAR_POINTER
/* -------------------------------------------------------------------- */

GrFrameDriver _GrFrameDriverSVGA8_LFB = {
    GR_frameSVGA8_LFB,              /* frame mode */
    GR_frameRAM8,               /* compatible RAM frame mode */
    TRUE,                       /* onscreen */
    4,                          /* scan line width alignment */
    1,                          /* number of planes */
    8,                          /* bits per pixel */
    8*16*1024L*1024L,           /* max plane size the code can handle */
    NULL,
    readpixel,
    drawpixel,
    drawline,
    drawhline,
    drawvline,
    drawblock,
    drawbitmap,
    drawpattern,
    _GrFrDrvPackedBitBltV2V,
    _GrFrDrvPackedBitBltV2R,
    _GrFrDrvPackedBitBltR2V
};

#endif