/**
 ** RAM24.C ---- the 16M color system RAM frame driver
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 **/


#include "grdriver.h"
#include "libgrx.h"
#include "access24.h"
#include "arith.h"
#include "mempeek.h"

#if BYTE_ORDER!=HARDWARE_BYTE_ORDER
#error Mismatching byte order between ram and video ram !
#endif

static
#ifdef __GNUC__
inline
#endif
long readpixel(GrFrame *c,int x,int y)
{
        long offs   = umul32(y,SCRN->gc_lineoffset) + (x + x + x);
        char far *p = &SCRN->gc_baseaddr[0][(unsigned)offs];
        return (long)peek_24(p);
}

static
#ifdef __GNUC__
inline
#endif
void drawpixel(int x,int y,long color)
{
        long offs   = umul32(y,CURC->gc_lineoffset) + (x + x + x);
        char far *p = &CURC->gc_baseaddr[0][(unsigned)offs];
        switch(C_OPER(color)) {
            case C_XOR: poke_24_xor(p,color);  break;
            case C_OR:  poke_24_or( p,color);  break;
            case C_AND: poke_24_and(p,color);  break;
            default:    poke_24(    p,color);  break;
        }
}

#ifndef __SHARED__

static void drawhline(int x,int y,int w,long color)
{
        ulong *f, five[5];
        int   bytes;
        int op =  C_OPER(color);
        long offs   = umul32(y,CURC->gc_lineoffset) + (x + x + x);
        char far *p = &CURC->gc_baseaddr[0][(unsigned)offs];

        w = w + w + w;
        bytes = (int) (((ulong)p) & 3);
        if (!bytes) bytes = 4;
        FillFive(five, color, bytes);
        if (bytes!=4) {
           uchar *cp = ((uchar *)five) + bytes;
          bytes = 4 - bytes;
          w    -= bytes;
          switch (op) {
              case C_XOR: if (bytes&1) {
                             poke_b_xor(p,*cp);
                             ptrinc(p,1); ptrinc(cp,1);
                          }
                          if (bytes&2) {
                             poke_w_xor(p,*((ushort *)cp));
                             ptrinc(p,2);
                          }
                          break;
              case C_OR:  if (bytes&1) {
                             poke_b_or(p,*cp);
                             ptrinc(p,1); ptrinc(cp,1);
                          }
                          if (bytes&2) {
                             poke_w_or(p,*((ushort *)cp));
                             ptrinc(p,2);
                          }
                          break;
              case C_AND: if (bytes&1) {
                             poke_b_and(p,*cp);
                             ptrinc(p,1); ptrinc(cp,1);
                          }
                          if (bytes&2) {
                             poke_w_and(p,*((ushort *)cp));
                             ptrinc(p,2);
                          }
                          break;
              default:    if (bytes&1) {
                             poke_b(p,*cp);
                             ptrinc(p,1); ptrinc(cp,1);
                          }
                          if (bytes&2) {
                             poke_w(p,*((ushort *)cp));
                             ptrinc(p,2);
                          }
                          break;
          }
          if (!w) return;
        }
        f = &five[1];
        switch (op) {
            case C_XOR: rowfill_24_xor(p,f,w); break;
            case C_OR:  rowfill_24_or( p,f,w); break;
            case C_AND: rowfill_24_and(p,f,w); break;
            default:    rowfill_24(    p,f,w); break;
        }
}
#else
# include "fdrivers/generic/hline.c"
#endif

static
#include "fdrivers/generic/vline.c"

static
#include "fdrivers/generic/line.c"

static
#include "fdrivers/generic/block.c"

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
            dst,(dx * 3),dy,
            src,(sx * 3),sy,
            (w * 3),h,
            op
        );
}

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
};

/* some systems map LFB in normal user space (eg. Linux/svgalib) */
/* near pointer stuff is equal to ram stuff :)                   */
/* in this is the far pointer code using %fs descriptor          */
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
            dst,(dx * 3),dy,
            src,(sx * 3),sy,
            (w * 3),h,
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
            dst,(dx * 3),dy,
            src,(sx * 3),sy,
            (w * 3),h,
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
            dst,(dx * 3),dy,
            src,(sx * 3),sy,
            (w * 3),h,
            op
        );
}

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
    bltv2v,
    bltv2r,
    bltr2v
};
#endif /* defined(LFB_BY_NEAR_POINTER) */
