/**
 ** LFB24.C ---- the 16M color Super VGA linear frame buffer driver
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 ** Andrzej Lawa [FidoNet: Andrzej Lawa 2:480/19.77]
 **/

/* some systems map LFB in normal user space (eg. Linux/svgalib) */
/* near pointer stuff is equal to ram stuff :)                   */
/* in this is the far pointer code using %fs descriptor          */
#ifndef LFB_BY_NEAR_POINTER

#ifdef   __TURBOC__
#error This library will not work with as a 16-bit real-mode code
#endif

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
        char far *p = &SCRN->gc_baseaddr[0][offs];
        setup_far_selector(SCRN->gc_selector);
        return (long)peek_24_f(p);
}

static
#ifdef __GNUC__
inline
#endif
void drawpixel(int x,int y,long color)
{
        char far *p = &CURC->gc_baseaddr[0][umul32(y,CURC->gc_lineoffset) + (x+x+x)];
        setup_far_selector(CURC->gc_selector);
        switch(C_OPER(color)) {
            case C_XOR: poke_24_f_xor(p,color);  break;
            case C_OR:  poke_24_f_or( p,color);  break;
            case C_AND: poke_24_f_and(p,color);  break;
            default:    poke_24_f(    p,color);  break;
        }
}


static void drawhline(int x,int y,int w,long color)
{
        ulong *f, five[5];
        int   bytes;
        int op =  C_OPER(color);
        char far *p = &CURC->gc_baseaddr[0][umul32(y,CURC->gc_lineoffset) + (x+x+x)];
        setup_far_selector(CURC->gc_selector);

        w = w + w + w;
        bytes = ((ulong)p) & 3;
        if (!bytes) bytes = 4;
        FillFive(five, color, bytes);
        if (bytes!=4) {
          /* we need to write up to 3 bytes for correct alignment */
          register uchar *cp = ((uchar *)five) + bytes;
          bytes = 4-bytes;
          w    -= bytes;
          switch (op) {
              case C_XOR: if (bytes&1) {
                             poke_b_f_xor(p,*cp);
                             ptrinc(p,1); ptrinc(cp,1);
                          }
                          if (bytes&2) {
                             poke_w_f_xor(p,*((ushort *)cp));
                             ptrinc(p,2);
                          }
                          break;
              case C_OR:  if (bytes&1) {
                             poke_b_f_or(p,*cp);
                             ptrinc(p,1); ptrinc(cp,1);
                          }
                          if (bytes&2) {
                             poke_w_f_or(p,*((ushort *)cp));
                             ptrinc(p,2);
                          }
                          break;
              case C_AND: if (bytes&1) {
                             poke_b_f_and(p,*cp);
                             ptrinc(p,1); ptrinc(cp,1);
                          }
                          if (bytes&2) {
                             poke_w_f_and(p,*((ushort *)cp));
                             ptrinc(p,2);
                          }
                          break;
              default:    if (bytes&1) {
                             poke_b_f(p,*cp);
                             ptrinc(p,1); ptrinc(cp,1);
                          }
                          if (bytes&2) {
                             poke_w_f(p,*((ushort *)cp));
                             ptrinc(p,2);
                          }
                          break;
          }
          if (!w) return;
        }
        f = &five[1];
        switch (op) {
            case C_XOR: rowfill_24_f_xor(p,f,w); break;
            case C_OR:  rowfill_24_f_or( p,f,w); break;
            case C_AND: rowfill_24_f_and(p,f,w); break;
            default:    rowfill_24_f(    p,f,w); break;
        }
}

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
        else _GrFrDrvPackedBitBltV2V_LFB(
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
        else _GrFrDrvPackedBitBltV2R_LFB(
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
        else _GrFrDrvPackedBitBltR2V_LFB(
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
    bitblt,
    bltv2r,
    bltr2v
};

#endif /* !defined(LFB_BY_NEAR_POINTER) */
