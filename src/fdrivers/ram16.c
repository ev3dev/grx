/**
 ** RAM16.C ---- the 32768/65536 color system RAM frame driver
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 **/

#include "grdriver.h"
#include "libgrx.h"
#include "arith.h"
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
        return((ushort)peek_w(&c->gf_baseaddr[0][(y * c->gf_lineoffset) + (x << 1)]));
}

static
#ifdef __GNUC__
inline
#endif
void drawpixel(int x,int y,long color)
{
        char far *ptr = &CURC->gc_baseaddr[0][(y * CURC->gc_lineoffset) + (x << 1)];
        switch(C_OPER(color)) {
            case C_XOR: poke_w_xor(ptr,(ushort)color); break;
            case C_OR:  poke_w_or( ptr,(ushort)color); break;
            case C_AND: poke_w_and(ptr,(ushort)color); break;
            default:    poke_w(    ptr,(ushort)color); break;
        }
}

static void drawhline(int x,int y,int w,long color)
{
        long offs = umul32(y,CURC->gc_lineoffset) + (x << 1);
        char far *pp = &CURC->gc_baseaddr[0][(unsigned)offs];
        int  cval = freplicate_w2l((int)color);
        switch(C_OPER(color)) {
            case C_XOR: repfill_w_xor(pp,cval,w); break;
            case C_OR:  repfill_w_or( pp,cval,w); break;
            case C_AND: repfill_w_and(pp,cval,w); break;
            default:    repfill_w(    pp,cval,w); break;
        }
}

static void drawvline(int x,int y,int h,long color)
{
        uint lwdt = CURC->gc_lineoffset;
        long offs = umul32(y,lwdt) + (x << 1);
        char far *pp = &CURC->gc_baseaddr[0][(unsigned)offs];
        switch(C_OPER(color)) {
            case C_XOR: colfill_w_xor(pp,lwdt,(int)color,h); break;
            case C_OR:  colfill_w_or( pp,lwdt,(int)color,h); break;
            case C_AND: colfill_w_and(pp,lwdt,(int)color,h); break;
            default:    colfill_w(    pp,lwdt,(int)color,h); break;
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

static void bitblt(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,long op)
{
        if(GrColorMode(op) == GrIMAGE) _GrFrDrvGenericBitBlt(
            dst,dx,dy,
            src,sx,sy,
            w,h,
            op
        );
        else _GrFrDrvPackedBitBltR2R(
            dst,(dx << 1),dy,
            src,(sx << 1),sy,
            (w << 1),h,
            op
        );
}

GrFrameDriver _GrFrameDriverRAM16 = {
    GR_frameRAM16,              /* frame mode */
    GR_frameUndef,              /* compatible RAM frame mode */
    FALSE,                      /* onscreen */
    4,                          /* scan line width alignment */
    1,                          /* number of planes */
    16,                         /* bits per pixel */
#ifdef __TURBOC__
    65520L,                     /* max plane size the code can handle */
#else
    16*16*1024L*1024L,          /* max plane size the code can handle */
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

static void bltv2v(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,long op)
{
        if(GrColorMode(op) == GrIMAGE) _GrFrDrvGenericBitBlt(
            dst,dx,dy,
            src,sx,sy,
            w,h,
            op
        );
        else _GrFrDrvPackedBitBltV2V(
            dst,(dx << 1),dy,
            src,(sx << 1),sy,
            (w << 1),h,
            op
        );
}

static void bltv2r(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,long op)
{
        if(GrColorMode(op) == GrIMAGE) _GrFrDrvGenericBitBlt(
            dst,dx,dy,
            src,sx,sy,
            w,h,
            op
        );
        else _GrFrDrvPackedBitBltV2R(
            dst,(dx << 1),dy,
            src,(sx << 1),sy,
            (w << 1),h,
            op
        );
}

static void bltr2v(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,long op)
{
        if(GrColorMode(op) == GrIMAGE) _GrFrDrvGenericBitBlt(
            dst,dx,dy,
            src,sx,sy,
            w,h,
            op
        );
        else _GrFrDrvPackedBitBltR2V(
            dst,(dx << 1),dy,
            src,(sx << 1),sy,
            (w << 1),h,
            op
        );
}


GrFrameDriver _GrFrameDriverSVGA16_LFB = {
    GR_frameSVGA16_LFB,         /* frame mode */
    GR_frameRAM16,              /* compatible RAM frame mode */
    TRUE,                       /* onscreen */
    4,                          /* scan line width alignment */
    1,                          /* number of planes */
    16,                         /* bits per pixel */
    16*16*1024L*1024L,          /* max plane size the code can handle */
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
