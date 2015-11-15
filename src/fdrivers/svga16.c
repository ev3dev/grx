/**
 ** svga18.c ---- the 32768/65536 color Super VGA frame driver
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu].
 **
 ** This file is part of the GRX graphics library.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 **/

#include "libgrx.h"
#include "grdriver.h"
#include "arith.h"
#include "mempeek.h"
#include "memfill.h"

#if BYTE_ORDER!=HARDWARE_BYTE_ORDER
#error Mismatching byte order between ram and video ram !
#endif

/* frame offset address calculation */
#define FOFS(x,y,lo) umuladd32((y),(lo),((x) << 1))


static INLINE
GrColor readpixel(GrFrame *c,int x,int y)
{
        GR_int32u offs;
        GRX_ENTER();
        offs = FOFS(x,y,SCRN->gc_lineoffset);
        CHKBANK(BANKNUM(offs));
        setup_far_selector(SCRN->gc_selector);
        GRX_RETURN((GR_int16u)peek_w_f(&SCRN->gc_baseaddr[0][BANKPOS(offs)]));
}


static INLINE
void drawpixel(int x,int y,GrColor color)
{
        GR_int32u offs;
        char *ptr;
        GRX_ENTER();
        offs = FOFS(x,y,CURC->gc_lineoffset);
        ptr  = &CURC->gc_baseaddr[0][BANKPOS(offs)];
        CHKBANK(BANKNUM(offs));
        setup_far_selector(CURC->gc_selector);
        switch(C_OPER(color)) {
            case C_XOR: poke_w_f_xor(ptr,(GR_int16u)color); break;
            case C_OR:  poke_w_f_or( ptr,(GR_int16u)color); break;
            case C_AND: poke_w_f_and(ptr,(GR_int16u)color); break;
            default:    poke_w_f(    ptr,(GR_int16u)color); break;
        }
        GRX_LEAVE();
}


static void drawhline(int x,int y,int w,GrColor color)
{
        GR_int32u offs;
        GR_repl cval;
        unsigned int w1,w2;
        GRX_ENTER();
        offs = FOFS(x,y,CURC->gc_lineoffset);
        w2  = BANKLFT(offs) >> 1;
        w2 = w - (w1 = umin(w,w2));
        cval = freplicate_w(color);
        setup_far_selector(CURC->gc_selector);
        do {
            char *pp = &CURC->gc_baseaddr[0][BANKPOS(offs)];
            CHKBANK(BANKNUM(offs));
            offs += (w1 << 1);
            switch(C_OPER(color)) {
                case C_XOR: repfill_w_f_xor(pp,cval,w1); break;
                case C_OR:  repfill_w_f_or( pp,cval,w1); break;
                case C_AND: repfill_w_f_and(pp,cval,w1); break;
                default:    repfill_w_f(    pp,cval,w1); break;
            }
            w1 = w2;
            w2 = 0;
        } while(w1 != 0);
        GRX_LEAVE();
}


static void drawvline(int x,int y,int h,GrColor color)
{
        unsigned int lwdt;
        GR_int32u offs;
        GRX_ENTER();
        lwdt = CURC->gc_lineoffset;
        offs = FOFS(x,y,lwdt);
        setup_far_selector(CURC->gc_selector);
        do {
            char *pp = &CURC->gc_baseaddr[0][BANKPOS(offs)];
            unsigned int h1 = BANKLFT(offs) / lwdt;
            h -= (h1 = umin(h,umax(h1,1)));
            CHKBANK(BANKNUM(offs));
            offs += (h1 * lwdt);
            switch(C_OPER(color)) {
                case C_XOR: colfill_w_f_xor(pp,lwdt,(GR_int16u)color,h1); break;
                case C_OR:  colfill_w_f_or( pp,lwdt,(GR_int16u)color,h1); break;
                case C_AND: colfill_w_f_and(pp,lwdt,(GR_int16u)color,h1); break;
                default:    colfill_w_f(    pp,lwdt,(GR_int16u)color,h1); break;
            }
        } while(h != 0);
        GRX_LEAVE();
}


static void drawblock(int x,int y,int w,int h,GrColor color)
{
        GR_int32u offs;
        GR_repl cval;
        int copr, skip;
        GRX_ENTER();
        skip = CURC->gc_lineoffset;
        offs = FOFS(x,y,skip);
        skip -= w << 1;
        cval = freplicate_w(color);
        copr = C_OPER(color);
        setup_far_selector(CURC->gc_selector);
        do {
            unsigned int w1,w2 = BANKLFT(offs) >> 1;
            w2 = w - (w1 = umin(w,w2));
            do {
                char *pp = &CURC->gc_baseaddr[0][BANKPOS(offs)];
                CHKBANK(BANKNUM(offs));
                offs += (w1 << 1);
                switch(copr) {
                    case C_XOR: repfill_w_f_xor(pp,cval,w1); break;
                    case C_OR:  repfill_w_f_or( pp,cval,w1); break;
                    case C_AND: repfill_w_f_and(pp,cval,w1); break;
                    default:    repfill_w_f(    pp,cval,w1); break;
                }
                w1 = w2;
                w2 = 0;
            } while(w1 != 0);
            offs += skip;
        } while(--h != 0);
        GRX_LEAVE();
}


static
#include "fdrivers/generic/line.c"


static
#include "fdrivers/generic/bitmap.c"


static
#include "fdrivers/generic/pattern.c"


static void bitblt(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,GrColor op)
{
        GRX_ENTER();
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
        GRX_LEAVE();
}


static void bltv2r(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,GrColor op)
{
        GRX_ENTER();
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
        GRX_LEAVE();
}


static void bltr2v(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,GrColor op)
{
        GRX_ENTER();
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
        GRX_LEAVE();
}


GrFrameDriver _GrFrameDriverSVGA16 = {
    GR_frameSVGA16,             /* frame mode */
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
    bitblt,
    bltv2r,
    bltr2v,
    _GrFrDrvGenericGetIndexedScanline,
    _GrFrDrvGenericPutScanline
};
