/**
 ** SVGA16.C ---- the 32768/65536 color Super VGA frame driver
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#ifdef   __TURBOC__
#pragma  inline
#endif

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
        long offs = umul32(y,SCRN->gc_lineoffset) + (x << 1);
        CHKBANK(BANKNUM(offs));
        setup_far_selector(SCRN->gc_selector);
        return((ushort)peek_w_f(&SCRN->gc_baseaddr[0][BANKPOS(offs)]));
}

static
#ifdef __GNUC__
inline
#endif
void drawpixel(int x,int y,long color)
{
        long offs = umul32(y,CURC->gc_lineoffset) + (x << 1);
        char far *ptr = &CURC->gc_baseaddr[0][BANKPOS(offs)];
        CHKBANK(BANKNUM(offs));
        setup_far_selector(CURC->gc_selector);
        switch(C_OPER(color)) {
            case C_XOR: poke_w_f_xor(ptr,(ushort)color); break;
            case C_OR:  poke_w_f_or( ptr,(ushort)color); break;
            case C_AND: poke_w_f_and(ptr,(ushort)color); break;
            default:    poke_w_f(    ptr,(ushort)color); break;
        }
}

static void drawhline(int x,int y,int w,long color)
{
        long offs = umul32(y,CURC->gc_lineoffset) + (x << 1);
        int  cval = freplicate_w2l((int)color);
        uint w1,w2 = BANKLFT(offs) >> 1;
        w2 = w - (w1 = umin(w,w2));
        setup_far_selector(CURC->gc_selector);
        do {
            char far *pp = &CURC->gc_baseaddr[0][BANKPOS(offs)];
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
}

static void drawvline(int x,int y,int h,long color)
{
        uint lwdt = CURC->gc_lineoffset;
        long offs = umul32(y,lwdt) + (x << 1);
        setup_far_selector(CURC->gc_selector);
        do {
            char far *pp = &CURC->gc_baseaddr[0][BANKPOS(offs)];
            uint h1 = BANKLFT(offs) / lwdt;
            h -= (h1 = umin(h,umax(h1,1)));
            CHKBANK(BANKNUM(offs));
            offs += (h1 * lwdt);
            switch(C_OPER(color)) {
                case C_XOR: colfill_w_f_xor(pp,lwdt,(int)color,h1); break;
                case C_OR:  colfill_w_f_or( pp,lwdt,(int)color,h1); break;
                case C_AND: colfill_w_f_and(pp,lwdt,(int)color,h1); break;
                default:    colfill_w_f(    pp,lwdt,(int)color,h1); break;
            }
        } while(h != 0);
}

static void drawblock(int x,int y,int w,int h,long color)
{
        long offs = umul32(y,CURC->gc_lineoffset) + (x << 1);
        int  cval = freplicate_w2l((int)color);
        int  copr = C_OPER(color);
        int  skip = CURC->gc_lineoffset - (w << 1);
        setup_far_selector(CURC->gc_selector);
        do {
            uint w1,w2 = BANKLFT(offs) >> 1;
            w2 = w - (w1 = umin(w,w2));
            do {
                char far *pp = &CURC->gc_baseaddr[0][BANKPOS(offs)];
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
}

static void drawline(int x,int y,int dx,int dy,long color)
{
        struct {
            int errsub;         /* subtract from error term */
            int erradd;         /* add to error term when carry */
            int offset1;        /* add to pointer if no carry on error term */
            int offset2;        /* add to pointer if carry / banking dir */
        } lndata;
        long offs;
        int  npts,error,xstep = 2;
        char far *ptr;

#       ifdef __GNUC__
#       ifdef __i386__
#       define ASM_LINE1(OPC) asm volatile("
            .align 2,0x90
         0: "#OPC"w %6,"I386_GCC_FAR_SELECTOR"(%0)
            subl %7,%2
            jb   1f
            subw $2,%3
            jb   2f
            decl %1
            jne  0b
            jmp  4f
            .align 2,0x90
         1: addl 4  + %7,%2
            addw 12 + %7,%3
            jb   3f
            decl %1
            jne  0b
            jmp  4f
         2: negl 8 + %7
         3: decl %1
         4: "                                                     \
         :  "=r" (ptr),                "=r" (npts), "=r" (error)  \
         :  "0"  ((short)(long)(ptr)), "1"  (npts), "2"  (error), \
            "r"  ((short)(color)),     "o"  (lndata)              \
        )
#       define ASM_LINE2(OPC) asm volatile("
            .align 2,0x90
         0: "#OPC"w %6,"I386_GCC_FAR_SELECTOR"(%0)
            subl %7,%2
            jb   1f
            addw 8 + %7,%3
            jb   2f
            decl %1
            jne  0b
            jmp  3f
            .align 2,0x90
         1: addl 4  + %7,%2
            addw 12 + %7,%3
            jb   2f
            decl %1
            jne  0b
            jmp  3f
         2: decl %1
         3: "                                                     \
         :  "=r" (ptr),                "=r" (npts), "=r" (error)  \
         :  "0"  ((short)(long)(ptr)), "1"  (npts), "2"  (error), \
            "r"  ((short)(color)),     "o"  (lndata)              \
        )
#       endif
#       endif
#       ifdef __TURBOC__
#       define ASM_LINE1(OPC) {                                   \
            _ES = FP_SEG(ptr);                                    \
            _BX = FP_OFF(ptr);                                    \
            _CX = npts;                                           \
            _DX = error;                                          \
            _AX = (int)color;                                     \
          OPC##1loop:                                             \
            asm OPC  word ptr es:[bx],ax;                         \
            asm sub  dx,word ptr lndata.errsub;                   \
            asm jb   OPC##1adjust;                                \
            asm sub  bx,2;                                        \
            asm jb   OPC##1newbank1;                              \
            asm loop OPC##1loop;                                  \
            asm jmp  OPC##1done;                                  \
          OPC##1adjust:                                           \
            asm add  dx,word ptr lndata.erradd;                   \
            asm add  bx,word ptr lndata.offset2;                  \
            asm jb   OPC##1newbank2;                              \
            asm loop OPC##1loop;                                  \
            asm jmp  OPC##1done;                                  \
          OPC##1newbank1:                                         \
            asm neg  word ptr lndata.offset1;                     \
          OPC##1newbank2:                                         \
            asm dec  cx;                                          \
          OPC##1done:                                             \
            *((short *)(&ptr)) = _BX;                             \
            npts  = _CX;                                          \
            error = _DX;                                          \
        }
#       define ASM_LINE2(OPC) {                                   \
            _ES = FP_SEG(ptr);                                    \
            _BX = FP_OFF(ptr);                                    \
            _CX = npts;                                           \
            _DX = error;                                          \
            _AX = (int)color;                                     \
          OPC##2loop:                                             \
            asm OPC  word ptr es:[bx],ax;                         \
            asm sub  dx,word ptr lndata.errsub;                   \
            asm jb   OPC##2adjust;                                \
            asm add  bx,word ptr lndata.offset1;                  \
            asm jb   OPC##2newbank;                               \
            asm loop OPC##2loop;                                  \
            asm jmp  OPC##2done;                                  \
          OPC##2adjust:                                           \
            asm add  dx,word ptr lndata.erradd;                   \
            asm add  bx,word ptr lndata.offset2;                  \
            asm jb   OPC##2newbank;                               \
            asm loop OPC##2loop;                                  \
            asm jmp  OPC##2done;                                  \
          OPC##2newbank:                                          \
            asm dec  cx;                                          \
          OPC##2done:                                             \
            *((short *)(&ptr)) = _BX;                             \
            npts  = _CX;                                          \
            error = _DX;                                          \
        }
#       endif

        if(dy < 0) {
            y -= (dy = (-dy));
            x -= (dx = (-dx));
        }
        if(dx < 0) {
            xstep = (-2);
            dx    = (-dx);
        }
        offs = umul32(y,CURC->gc_lineoffset) + (x << 1);
        ptr  = &CURC->gc_baseaddr[0][BANKPOS(offs)];
        CHKBANK(BANKNUM(offs));
        setup_far_selector(CURC->gc_selector);
        if(dx > dy) {
            npts  = dx +  1;
            error = dx >> 1;
            lndata.errsub  = dy;
            lndata.erradd  = dx;
            lndata.offset1 = xstep;
            lndata.offset2 = CURC->gc_lineoffset + xstep;
            if(xstep < 0) {
              again_nonlinear:
                lndata.offset1 = 1;
                switch(C_OPER(color)) {
                    case C_XOR: ASM_LINE1(xor); break;
                    case C_OR:  ASM_LINE1(or);  break;
                    case C_AND: ASM_LINE1(and); break;
                    default:    ASM_LINE1(mov); break;
                }
                if(npts == 0) return;
                SETBANK(DRVINFO->curbank + lndata.offset1);
                goto again_nonlinear;
            }
        }
        else {
            npts  = dy +  1;
            error = dy >> 1;
            lndata.errsub  = dx;
            lndata.erradd  = dy;
            lndata.offset1 = CURC->gc_lineoffset;
            lndata.offset2 = CURC->gc_lineoffset + xstep;
        }
      again_linear:
        switch(C_OPER(color)) {
            case C_XOR: ASM_LINE2(xor); break;
            case C_OR:  ASM_LINE2(or);  break;
            case C_AND: ASM_LINE2(and); break;
            default:    ASM_LINE2(mov); break;
        }
        if(npts == 0) return;
        SETBANK(DRVINFO->curbank + 1);
        goto again_linear;
}

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
    bltr2v
};

