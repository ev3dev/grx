/**
 ** SVGA32H.C ---- the 16M color padded Super VGA frame driver (high)
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 ** Andrzej Lawa [FidoNet: Andrzej Lawa 2:480/19.77]
 **/

#include "grdriver.h"
#include "libgrx.h"
#include "arith.h"
#include "mempeek.h"
#include "memfill.h"

static
#ifdef __GNUC__
inline
#endif
long readpixel(GrFrame *c,int x,int y)
{
        long offs = umul32(y,SCRN->gc_lineoffset) + (x << 2);
        CHKBANK(BANKNUM(offs));
        setup_far_selector(SCRN->gc_selector);
        return((ulong)peek_l_f(&SCRN->gc_baseaddr[0][BANKPOS(offs)]) >> 8);
}

static
#ifdef __GNUC__
inline
#endif
void drawpixel(int x,int y,long color)
{
        long offs = umul32(y,CURC->gc_lineoffset) + (x << 2);
        char far *ptr = &CURC->gc_baseaddr[0][BANKPOS(offs)];
        int op = C_OPER(color);
        color <<= 8;
        CHKBANK(BANKNUM(offs));
        setup_far_selector(CURC->gc_selector);
        switch(op) {
            case C_XOR: poke_l_f_xor(ptr,color); break;
            case C_OR:  poke_l_f_or( ptr,color); break;
            case C_AND: poke_l_f_and(ptr,color); break;
            default:    poke_l_f(    ptr,color); break;
        }
}

#ifdef repfill_l_f
static void drawhline(int x,int y,int w,long color)
{
        long offs = umul32(y,CURC->gc_lineoffset) + (x << 2);
        long cval = color << 8;
        uint w1,w2 = BANKLFT(offs) >> 2;
        w2 = w - (w1 = umin(w,w2));
        setup_far_selector(CURC->gc_selector);
        do {
            char far *pp = &CURC->gc_baseaddr[0][BANKPOS(offs)];
            CHKBANK(BANKNUM(offs));
            offs += (w1 << 2);
            switch(C_OPER(color)) {
                case C_XOR: repfill_l_f_xor(pp,cval,w1); break;
                case C_OR:  repfill_l_f_or( pp,cval,w1); break;
                case C_AND: repfill_l_f_and(pp,cval,w1); break;
                default:    repfill_l_f(    pp,cval,w1); break;
            }
            w1 = w2;
            w2 = 0;
        } while(w1 != 0);
}
#else
static
#include "fdrivers/generic/hline.c"
#endif

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

static void bitblt(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,long op)
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

static void bltv2r(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,long op)
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

static void bltr2v(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,long op)
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

GrFrameDriver _GrFrameDriverSVGA32H = {
    GR_frameSVGA32H,            /* frame mode */
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
    bitblt,
    bltv2r,
    bltr2v
};

