/**
 ** RAM32H.C ---- the 16M color padded system RAM frame driver (high)
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 **/

#include "grdriver.h"
#include "libgrx.h"
#include "mempeek.h"
#include "memfill.h"

#if BYTE_ORDER!=HARDWARE_BYTE_ORDER
#error Mismatching byte order between ram and video ram !
#endif

static
#ifdef __GNUC__
inline
#endif
long readpixel(GrFrame *c,int x,int y)
{
        char far *ptr = &c->gf_baseaddr[0][(y * c->gf_lineoffset) + (x << 2)];
        return((ulong)peek_l(ptr) >> 8);
}

static
#ifdef __GNUC__
inline
#endif
void drawpixel(int x,int y,long color)
{
        char far *ptr = &CURC->gc_baseaddr[0][(y * CURC->gc_lineoffset) + (x << 2)];
        int op = C_OPER(color);
        color <<= 8;
        switch(op) {
            case C_XOR: poke_l_xor(ptr,color); break;
            case C_OR:  poke_l_or(ptr,color);  break;
            case C_AND: poke_l_and(ptr,color); break;
            default:    poke_l(ptr,color);     break;
        }
}

#ifdef colfill_l
static void drawvline(int x,int y,int h,long color)
{
        uint lwdt = CURC->gc_lineoffset;
        long offs = umul32(y,lwdt) + (x << 2);
        char far *pp = &CURC->gc_baseaddr[0][offs];
        int  cval = color<<8;
        switch(C_OPER(color)) {
            case C_XOR: colfill_l_xor(pp,lwdt,cval,h); break;
            case C_OR:  colfill_l_or( pp,lwdt,cval,h); break;
            case C_AND: colfill_l_and(pp,lwdt,cval,h); break;
            default:    colfill_l(    pp,lwdt,cval,h); break;
        }
}
#else
static
#include "fdrivers/generic/vline.c"
#endif

#ifdef repfill_l
  static
# ifdef __GNUC__
    inline
# endif
void drawhline(int x,int y,int w,long color)
{
        long offs = umul32(y,CURC->gc_lineoffset) + (x << 2);
        char far *pp = &CURC->gc_baseaddr[0][offs];
        int  cval = color<<8;
        switch(C_OPER(color)) {
            case C_XOR: repfill_l_xor(pp,cval,w); break;
            case C_OR:  repfill_l_or( pp,cval,w); break;
            case C_AND: repfill_l_and(pp,cval,w); break;
            default:    repfill_l(    pp,cval,w); break;
        }
}
#else
static
#include "fdrivers/generic/hline.c"
#endif

static
#include "fdrivers/generic/block.c"

static
#include "fdrivers/generic/line.c"

static
#include "fdrivers/generic/bitmap.c"

static
#include "fdrivers/generic/pattern.c"

static void bitblt(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,long op)
{
        if(GrColorMode(op) == GrIMAGE) _GrFrDrvGenericBitBlt(
            dst,dx,dy,
            src,sx,sy,
            w,h,
            op
        );
        else _GrFrDrvPackedBitBltR2R(
            dst,(dx << 2),dy,
            src,(sx << 2),sy,
            (w << 2),h,
            op
        );
}

GrFrameDriver _GrFrameDriverRAM32H = {
    GR_frameRAM32H,             /* frame mode */
    GR_frameUndef,              /* compatible RAM frame mode */
    FALSE,                      /* onscreen */
    4,                          /* scan line width alignment */
    1,                          /* number of planes */
    32,                         /* bits per pixel */
#ifdef __TURBOC__
    65520L,                     /* max plane size the code can handle */
#else
    32*16*1024L*1024L,          /* max plane size the code can handle */
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

#ifdef LFB_BY_NEAR_POINTER

static void bltv2v(GrFrame *dst,int dx,int dy,
                   GrFrame *src,int sx,int sy,
                   int w,int h,long op)
{
        if(GrColorMode(op) == GrIMAGE) _GrFrDrvGenericBitBlt(
            dst,dx,dy,
            src,sx,sy,
            w,h,
            op
        );
        else _GrFrDrvPackedBitBltV2V(
            dst,(dx << 2),dy,
            src,(sx << 2),sy,
            (w << 2),h,
            op
        );
}

static void bltv2r(GrFrame *dst,int dx,int dy,
                   GrFrame *src,int sx,int sy,
                   int w,int h,long op)
{
        if(GrColorMode(op) == GrIMAGE) _GrFrDrvGenericBitBlt(
            dst,dx,dy,
            src,sx,sy,
            w,h,
            op
        );
        else _GrFrDrvPackedBitBltV2R(
            dst,(dx << 2),dy,
            src,(sx << 2),sy,
            (w << 2),h,
            op
        );
}

static void bltr2v(GrFrame *dst,int dx,int dy,
                   GrFrame *src,int sx,int sy,
                   int w,int h,long op)
{
        if(GrColorMode(op) == GrIMAGE) _GrFrDrvGenericBitBlt(
            dst,dx,dy,
            src,sx,sy,
            w,h,
            op
        );
        else _GrFrDrvPackedBitBltR2V(
            dst,(dx << 2),dy,
            src,(sx << 2),sy,
            (w << 2),h,
            op
        );
}

GrFrameDriver _GrFrameDriverSVGA32H_LFB = {
    GR_frameSVGA32H_LFB,        /* frame mode */
    GR_frameRAM32H,             /* compatible RAM frame mode */
    TRUE,                       /* onscreen */
    4,                          /* line width alignment */
    1,                          /* number of planes */
    32,                         /* bits per pixel */
    32*16*1024L*1024L,          /* max plane size the code can handle */
    NULL,
    readpixel,
    drawpixel,
    drawline,
    drawhline,
    drawvline,
    drawblock,
    drawbitmap,
    drawpattern,
    bltv2v,
    bltv2r,
    bltr2v
};

#endif
