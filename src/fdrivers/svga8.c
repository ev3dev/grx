/**
 ** svga8.c ---- the 256 color (Super) VGA frame driver
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
#include "docolor.h"
#include "mempeek.h"
#include "memfill.h"


/* frame offset address calculation */
#define FOFS(x,y,lo) umuladd32((y),(lo),(x))


static INLINE
GrColor readpixel(GrFrame *c,int x,int y)
{
        GR_int32u offs;
        GRX_ENTER();
        offs = FOFS(x,y,SCRN->gc_lineoffset);
        CHKBANK(BANKNUM(offs));
        setup_far_selector(SCRN->gc_selector);
        GRX_RETURN((GR_int8u)peek_b_f(&SCRN->gc_baseaddr[0][BANKPOS(offs)]));
}


static INLINE
void drawpixel(int x,int y,GrColor color)
{
        GR_int32u offs;
        char far *ptr;
        GRX_ENTER();
        offs = FOFS(x,y,CURC->gc_lineoffset);
        ptr  = &CURC->gc_baseaddr[0][BANKPOS(offs)];
        CHKBANK(BANKNUM(offs));
        setup_far_selector(CURC->gc_selector);
        switch(C_OPER(color)) {
            case C_XOR: poke_b_f_xor(ptr,(GR_int8u)color); break;
            case C_OR:  poke_b_f_or( ptr,(GR_int8u)color); break;
            case C_AND: poke_b_f_and(ptr,(GR_int8u)color); break;
            default:    poke_b_f(    ptr,(GR_int8u)color); break;
        }
        GRX_LEAVE();
}


static void drawhline(int x,int y,int w,GrColor color)
{
        int copr;
        GRX_ENTER();
        copr = C_OPER(color);
        if(DOCOLOR8(color,copr)) {
          GR_int32u offs = FOFS(x,y,CURC->gc_lineoffset);
          unsigned int w1 = BANKLFT(offs);
          unsigned int w2 = w - (w1 = umin(w,w1));
          GR_repl cval = freplicate_b(color);
          setup_far_selector(CURC->gc_selector);
          do {
            char far *pp = &CURC->gc_baseaddr[0][BANKPOS(offs)];
            CHKBANK(BANKNUM(offs));
            offs += w1;
            switch(copr) {
                case C_XOR: repfill_b_f_xor(pp,cval,w1); break;
                case C_OR:  repfill_b_f_or( pp,cval,w1); break;
                case C_AND: repfill_b_f_and(pp,cval,w1); break;
                default:    repfill_b_f(    pp,cval,w1); break;
            }
            w1 = w2;
            w2 = 0;
          } while(w1 != 0);
        }
        GRX_LEAVE();
}


static void drawvline(int x,int y,int h,GrColor color)
{
        int copr;
        GRX_ENTER();
        copr = C_OPER(color);
        if(DOCOLOR8(color,copr)) {
          unsigned int lwdt = CURC->gc_lineoffset;
          GR_int32u offs = FOFS(x,y,lwdt);
          setup_far_selector(CURC->gc_selector);
          do {
            char far *pp = &CURC->gc_baseaddr[0][BANKPOS(offs)];
            unsigned int h1 = BANKLFT(offs) / lwdt;
            h -= (h1 = umin(h,umax(h1,1)));
            CHKBANK(BANKNUM(offs));
            offs += umul32(h1,lwdt);
            switch(copr) {
                case C_XOR: colfill_b_f_xor(pp,lwdt,(int)color,h1); break;
                case C_OR:  colfill_b_f_or( pp,lwdt,(int)color,h1); break;
                case C_AND: colfill_b_f_and(pp,lwdt,(int)color,h1); break;
                default:    colfill_b_f(    pp,lwdt,(int)color,h1); break;
            }
          } while(h != 0);
        }
        GRX_LEAVE();
}


static void drawblock(int x,int y,int w,int h,GrColor color)
{
        int copr;
        GRX_ENTER();
        copr = C_OPER(color);
        if(DOCOLOR8(color,copr)) {
          unsigned int skip = CURC->gc_lineoffset;
          GR_int32u offs = FOFS(x,y,skip);
          GR_repl   cval = freplicate_b(color);
          skip -= w;
          setup_far_selector(CURC->gc_selector);
          do {
            unsigned int w1 = BANKLFT(offs);
            unsigned int w2 = w - (w1 = umin(w,w1));
            do {
                char far *pp = &CURC->gc_baseaddr[0][BANKPOS(offs)];
                CHKBANK(BANKNUM(offs));
                offs += w1;
                switch(copr) {
                    case C_XOR: repfill_b_f_xor(pp,cval,w1); break;
                    case C_OR:  repfill_b_f_or( pp,cval,w1); break;
                    case C_AND: repfill_b_f_and(pp,cval,w1); break;
                    default:    repfill_b_f(    pp,cval,w1); break;
                }
                w1 = w2;
                w2 = 0;
            } while(w1 != 0);
            offs += skip;
          } while(--h != 0);
        }
        GRX_LEAVE();
}


static
#include "fdrivers/generic/line.c"


static void drawbitmap(int x,int y,int w,int h,char far *bmp,int pitch,int start,GrColor fg,GrColor bg)
{
        int fgop, bgop;
        int dofg, dobg;
        GRX_ENTER();
        fgop = C_OPER(fg);
        bgop = C_OPER(bg);
        dofg = DOCOLOR8(fg,fgop);
        dobg = DOCOLOR8(bg,bgop);
        if(dofg | dobg) {
            unsigned int skip = CURC->gc_lineoffset;
            GR_int32u offs = FOFS(x,y,skip);
            int once = dofg && dobg && (fgop == bgop);
            skip -= w;
            bmp += start >> 3;
            start &= 7;
            setup_far_selector(CURC->gc_selector);
            do {
                GR_int8u far *bp = (GR_int8u far *)bmp;
                GR_int8u bits    = *bp;
                GR_int8u mask    = 0x80 >> start;
                unsigned int  w1 = BANKLFT(offs);
                unsigned int  w2 = w - (w1  = umin(w,w1));
                do {
                    char far *pp = &CURC->gc_baseaddr[0][BANKPOS(offs)];
                    CHKBANK(BANKNUM(offs));
                    offs += w1;
#                   define DOBOTH(POKEOP) do {                              \
                        POKEOP(pp,((bits&mask)?(GR_int8u)fg:(GR_int8u)bg)); \
                        if((mask >>= 1) == 0) bits = *++bp,mask = 0x80;     \
                        pp++;                                               \
                    } while(--w1 != 0)
#                   define DOFGC(POKEOP) do {                               \
                        if((mask & bits) != 0) POKEOP(pp,(GR_int8u)fg);     \
                        if((mask >>= 1)  == 0) bits = *++bp,mask = 0x80;    \
                        pp++;                                               \
                    } while(--w1 != 0)
#                   define DOBGC(POKEOP) do {                               \
                        if((mask & bits) == 0) POKEOP(pp,(GR_int8u)bg);     \
                        if((mask >>= 1)  == 0) bits = *++bp,mask = 0x80;    \
                        pp++;                                               \
                    } while(--w1 != 0)
                    if(once) switch(fgop) {
                        case C_XOR: DOBOTH(poke_b_f_xor); break;
                        case C_OR:  DOBOTH(poke_b_f_or);  break;
                        case C_AND: DOBOTH(poke_b_f_and); break;
                        default:    DOBOTH(poke_b_f);     break;
                    }
                    else {
                        char far *ppsave = pp;
                        unsigned int w1save = w1;
                        if(dofg) switch(fgop) {
                            case C_XOR: DOFGC(poke_b_f_xor); break;
                            case C_OR:  DOFGC(poke_b_f_or);  break;
                            case C_AND: DOFGC(poke_b_f_and); break;
                            default:    DOFGC(poke_b_f);     break;
                        }
                        if(dobg) {
                            pp   = ppsave;
                            w1   = w1save;
                            bp   = (GR_int8u far *)bmp;
                            bits = *bp;
                            mask = 0x80 >> start;
                            switch(bgop) {
                                case C_XOR: DOBGC(poke_b_f_xor); break;
                                case C_OR:  DOBGC(poke_b_f_or);  break;
                                case C_AND: DOBGC(poke_b_f_and); break;
                                default:    DOBGC(poke_b_f);     break;
                            }
                        }
                    }
                    w1 = w2;
                    w2 = 0;
                } while(w1 != 0);
                offs += skip;
                bmp  += pitch;
            } while(--h != 0);
        }
        GRX_LEAVE();
}


static
#include "fdrivers/generic/pattern.c"


GrFrameDriver _GrFrameDriverSVGA8 = {
    GR_frameSVGA8,              /* frame mode */
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
    _GrFrDrvPackedBitBltR2V,
    _GrFrDrvGenericGetIndexedScanline,
    _GrFrDrvGenericPutScanline
};
