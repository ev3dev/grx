/**
 ** LFB8.C ---- the 256 color Super VGA linear frame buffer driver
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
#ifndef LBF_BY_NEAR_POINTER

#ifdef   __TURBOC__
#error This library will not work with as a 16-bit real-mode code
#endif

#include "libgrx.h"
#include "grdriver.h"
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
        long offs = umul32(y,SCRN->gc_lineoffset) + x;
        setup_far_selector(SCRN->gc_selector);
        return((uchar)peek_b_f(&SCRN->gc_baseaddr[0][offs]));
}

/* -------------------------------------------------------------------- */

static
#ifdef __GNUC__
inline
#endif
void drawpixel(int x,int y,long color)
{
        long offs = umul32(y,CURC->gc_lineoffset) + x;
        char far *ptr = &CURC->gc_baseaddr[0][offs];
        setup_far_selector(CURC->gc_selector);
        switch(C_OPER(color)) {
            case C_XOR: poke_b_f_xor(ptr,(uchar)color); break;
            case C_OR:  poke_b_f_or( ptr,(uchar)color); break;
            case C_AND: poke_b_f_and(ptr,(uchar)color); break;
            default:    poke_b_f(    ptr,(uchar)color); break;
        }
}

/* -------------------------------------------------------------------- */

static void drawvline(int x,int y,int h,long color)
{
        uint lwdt = CURC->gc_lineoffset;
        long offs = umul32(y,lwdt) + x;
        int  copr = C_OPER(color);
        setup_far_selector(CURC->gc_selector);
        if(DOCOLOR8(color,copr)) {
            char far *pp = &CURC->gc_baseaddr[0][offs];
            switch(copr) {
                case C_XOR: colfill_b_f_xor(pp,lwdt,(int)color,h); break;
                case C_OR:  colfill_b_f_or( pp,lwdt,(int)color,h); break;
                case C_AND: colfill_b_f_and(pp,lwdt,(int)color,h); break;
                default:    colfill_b_f(    pp,lwdt,(int)color,h); break;
            }
        }
}

/* -------------------------------------------------------------------- */

static void drawhline(int x,int y,int w,long color)
{
        long offs = umul32(y,CURC->gc_lineoffset) + x;
        int  cval = freplicate_b2l((int)color);
        int  copr = C_OPER(color);
        setup_far_selector(CURC->gc_selector);
        if(DOCOLOR8(color,copr)) {
            char far *pp = &CURC->gc_baseaddr[0][offs];
            switch(copr) {
                case C_XOR: repfill_b_f_xor(pp,cval,w); break;
                case C_OR:  repfill_b_f_or( pp,cval,w); break;
                case C_AND: repfill_b_f_and(pp,cval,w); break;
                default:    repfill_b_f(    pp,cval,w); break;
            }
        }
}

/* -------------------------------------------------------------------- */

static void drawblock(int x,int y,int w,int h,long color)
{
        uint skip = CURC->gc_lineoffset;
        long offs = umul32(y,skip) + x;
        int  cval = freplicate_b2l((int)color);
        int  copr = C_OPER(color);
        setup_far_selector(CURC->gc_selector);
        if(DOCOLOR8(color,copr)) {
            char far *ptr = &CURC->gc_baseaddr[0][offs];
            switch(copr) {
                case C_XOR:
                    do {
                      int ww = w;
                      char far *pp = ptr;
                      repfill_b_f_xor(pp,cval,ww);
                      ptr += skip;
                    } while(--h != 0);
                    break;
                case C_OR:
                    do {
                      int ww = w;
                      char far *pp = ptr;
                      repfill_b_f_or(pp,cval,ww);
                      ptr += skip;
                    } while(--h != 0);
                    break;
                case C_AND:
                    do {
                      int ww = w;
                      char far *pp = ptr;
                      repfill_b_f_and(pp,cval,ww);
                      ptr += skip;
                    } while(--h != 0);
                    break;
                default:
                    do {
                      int ww = w;
                      char far *pp = ptr;
                      repfill_b_f(pp,cval,ww);
                      ptr += skip;
                    } while(--h != 0);
                    break;
            }
        }
}

/* -------------------------------------------------------------------- */

#if defined(__GNUC__) && defined(__i386__)
static void drawline(int x,int y,int dx,int dy,long color)
{
        struct {
            int errsub;
            int erradd;
            int offset1;
            int offset2;
        } lndata;
        int  copr  = C_OPER(color);
        int  xstep = 1;
        int  npts,error;
        char far *ptr;
        long offs;

#       ifdef __GNUC__
#       ifdef __i386__
#       define ASM_LINE1(OPC) asm volatile("
            .align 2,0x90
         0: "#OPC"b %6,"I386_GCC_FAR_SELECTOR"(%0)
            subl %7,%2
            jb   1f
            decl %3
            decl %1
            jne  0b
            jmp  2f
            .align 2,0x90
         1: addl 4  + %7,%2
            addl 12 + %7,%3
            decl %1
            jne  0b
         2: "                                                     \
         :  "=r" (ptr),         "=r" (npts), "=r" (error)         \
         :  "0"  ((long)(ptr)), "1"  (npts), "2"  (error),        \
            "q"  ((char)(color)),      "o"  (lndata)              \
        )
#       define ASM_LINE2(OPC) asm volatile("
            .align 2,0x90
         0: "#OPC"b %6,"I386_GCC_FAR_SELECTOR"(%0)
            subl %7,%2
            jb   1f
            addl 8 + %7,%3
            decl %1
            jne  0b
            jmp  2f
            .align 2,0x90
         1: addl 4  + %7,%2
            addl 12 + %7,%3
            decl %1
            jne  0b
         2: "                                                     \
         :  "=r" (ptr),         "=r" (npts), "=r" (error)         \
         :  "0"  ((long)(ptr)), "1"  (npts), "2"  (error),        \
            "q"  ((char)(color)),      "o"  (lndata)              \
        )
#       endif
#       endif

        if(!DOCOLOR8(color,copr)) {
            return;
        }
        if(dy < 0) {
            y -= (dy = (-dy));
            x -= (dx = (-dx));
        }
        if(dx < 0) {
            xstep = (-1);
            dx    = (-dx);
        }
        offs = umul32(y,CURC->gc_lineoffset) + x;
        ptr  = &CURC->gc_baseaddr[0][offs];
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
                switch(copr) {
                    case C_XOR: ASM_LINE1(xor); break;
                    case C_OR:  ASM_LINE1(or);  break;
                    case C_AND: ASM_LINE1(and); break;
                    default:    ASM_LINE1(mov); break;
                }
                if(npts == 0) return;
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
        switch(copr) {
            case C_XOR: ASM_LINE2(xor); break;
            case C_OR:  ASM_LINE2(or);  break;
            case C_AND: ASM_LINE2(and); break;
            default:    ASM_LINE2(mov); break;
        }
        if(npts == 0) return;
        goto again_linear;
}
#else
static
#include "fdrivers/generic/line.c"
#endif

/* -------------------------------------------------------------------- */

static void drawbitmap(int x,int y,int w,int h,
                       char far *bmp,int pitch,int start,long fg,long bg)
{
        int fgop = C_OPER(fg);
        int bgop = C_OPER(bg);
        int dofg = DOCOLOR8(fg,fgop);
        int dobg = DOCOLOR8(bg,bgop);
        if(dofg | dobg) {
            long offs = umul32(y,CURC->gc_lineoffset) + x;
            int  once = dofg && dobg && (fgop == bgop);
            int  skip = CURC->gc_lineoffset;
            bmp   += start >> 3;
            start &= 7;
            setup_far_selector(CURC->gc_selector);
            do {
                uchar far *bp = (uchar far *)bmp;
                uchar bits    = *bp;
                uchar mask    = 0x80 >> start;
                ulong w1      = w;
                char far *pp = &CURC->gc_baseaddr[0][offs];
#               define DOBOTH(POKEOP) do {                              \
                    POKEOP(pp,((bits & mask) ? (uchar)fg : (uchar)bg)); \
                    if((mask >>= 1) == 0) bits = *++bp,mask = 0x80;     \
                    pp++;                                               \
                } while(--w1 != 0)
#               define DOFGC(POKEOP) do {                               \
                    if((mask & bits) != 0) POKEOP(pp,(uchar)fg);        \
                    if((mask >>= 1)  == 0) bits = *++bp,mask = 0x80;    \
                    pp++;                                               \
                } while(--w1 != 0)
#               define DOBGC(POKEOP) do {                               \
                    if((mask & bits) == 0) POKEOP(pp,(uchar)bg);        \
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
                    ulong     w1save = w1;
                    if(dofg) switch(fgop) {
                        case C_XOR: DOFGC(poke_b_f_xor); break;
                        case C_OR:  DOFGC(poke_b_f_or);  break;
                        case C_AND: DOFGC(poke_b_f_and); break;
                        default:    DOFGC(poke_b_f);     break;
                    }
                    if(dobg) {
                        pp   = ppsave;
                        w1   = w1save;
                        bp   = (uchar far *)bmp;
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
                offs += skip;
                bmp  += pitch;
            } while(--h != 0);
        }
}

/* -------------------------------------------------------------------- */

void _GrFrDrvPackedBitBltR2V_LFB(GrFrame *dst,int dx,int dy,
                                 GrFrame *src,int sx,int sy,
                                 int w,int h,long op)
{
        char far *dptr = &dst->gf_baseaddr[0][umul32(dy,dst->gf_lineoffset)+dx];
        char far *sptr = &src->gf_baseaddr[0][(sy * src->gf_lineoffset) + sx];
        ulong dskip = dst->gf_lineoffset - w;
        ulong sskip = src->gf_lineoffset - w;
        int   oper  = C_OPER(op);
        int   fast  = (oper == C_IMAGE) ? 0 : ~0;
        uchar cval  = (uchar)op;
        int   ww;
#       define CPSIZE_b     sizeof(uchar)
#       define CPSIZE_w     sizeof(ushort)
#       define CPSIZE_l     sizeof(ulong)
#       define poke_b_f_image(p,v) {                                    \
            uchar val = (v);                                            \
            if(val != cval) poke_b_f(p,val);                            \
        }
#       define DO1COPY(SIZE,PTRINC,OP) {                                \
            poke_##SIZE##_f##OP(dptr,peek_##SIZE(sptr));                \
            dptr PTRINC CPSIZE_##SIZE;                                  \
            sptr PTRINC CPSIZE_##SIZE;                                  \
        }
#       define DOBCOPY(PTRINC,OP) do {                                  \
            DO1COPY(b,PTRINC,OP);                                       \
        } while(--w1 != 0)
#       define DOLCOPY(PTRINC,OP) {                                     \
            if((ulong)dptr & 1) { DO1COPY(b,PTRINC,OP); w1 -= 1; }        \
            if((ulong)dptr & 2) { DO1COPY(w,PTRINC,OP); w1 -= 2; }        \
            ww = w1 >> 2;                                               \
            do { DO1COPY(l,PTRINC,OP); } while(--ww != 0);              \
            if(w1 & 2) { DO1COPY(w,PTRINC,OP); }                        \
            if(w1 & 1) { DO1COPY(b,PTRINC,OP); }                        \
        }
        setup_far_selector(dst->gf_selector);
        do {
            ulong w1 = w;
            if((w1 & fast) >= 7) switch(oper) {
                case C_XOR:   DOLCOPY(+=,_xor);   break;
                case C_OR:    DOLCOPY(+=,_or);    break;
                case C_AND:   DOLCOPY(+=,_and);   break;
                default:      DOLCOPY(+=,);       break;
            }
            else switch(oper) {
                case C_XOR:   DOBCOPY(+=,_xor);   break;
                case C_OR:    DOBCOPY(+=,_or);    break;
                case C_AND:   DOBCOPY(+=,_and);   break;
                case C_IMAGE: DOBCOPY(+=,_image); break;
                default:      DOBCOPY(+=,);       break;
            }
            dptr += dskip;
            sptr += sskip;
        } while(--h != 0);
}

#undef CPSIZE_b
#undef CPSIZE_w
#undef CPSIZE_l
#undef poke_b_f_image
#undef DO1COPY
#undef DOBCOPY
#undef DOLCOPY

void _GrFrDrvPackedBitBltV2R_LFB(GrFrame *dst,int dx,int dy,
                                 GrFrame *src,int sx,int sy,
                                 int w,int h,long op)
{
        char far *dptr = &dst->gf_baseaddr[0][(dy * dst->gf_lineoffset) + dx];
        char far *sptr = &src->gf_baseaddr[0][umul32(sy,src->gf_lineoffset)+sx];
        ulong dskip = dst->gf_lineoffset - w;
        ulong sskip = src->gf_lineoffset - w;
        int   oper  = C_OPER(op);
        int   fast  = (oper == C_IMAGE) ? 0 : ~0;
        uchar cval  = (uchar)op;
        int   ww;
#       define CPSIZE_b     sizeof(uchar)
#       define CPSIZE_w     sizeof(ushort)
#       define CPSIZE_l     sizeof(ulong)
#       define poke_b_image(p,v) {                                      \
            uchar val = (v);                                            \
            if(val != cval) poke_b(p,val);                              \
        }
#       define DO1COPY(SIZE,PTRINC,OP) {                                \
            poke_##SIZE##OP(dptr,peek_##SIZE##_f(sptr));                \
            dptr PTRINC CPSIZE_##SIZE;                                  \
            sptr PTRINC CPSIZE_##SIZE;                                  \
        }
#       define DOBCOPY(PTRINC,OP) do {                                  \
            DO1COPY(b,PTRINC,OP);                                       \
        } while(--w1 != 0)
#       define DOLCOPY(PTRINC,OP) {                                     \
            if((int)sptr & 1) { DO1COPY(b,PTRINC,OP); w1 -= 1; }        \
            if((int)sptr & 2) { DO1COPY(w,PTRINC,OP); w1 -= 2; }        \
            ww = w1 >> 2;                                               \
            do { DO1COPY(l,PTRINC,OP); } while(--ww != 0);              \
            if(w1 & 2) { DO1COPY(w,PTRINC,OP); }                        \
            if(w1 & 1) { DO1COPY(b,PTRINC,OP); }                        \
        }
        setup_far_selector(src->gf_selector);
        do {
            ulong w1 = w;
            if((w1 & fast) >= 7) switch(oper) {
                case C_XOR:   DOLCOPY(+=,_xor);   break;
                case C_OR:    DOLCOPY(+=,_or);    break;
                case C_AND:   DOLCOPY(+=,_and);   break;
                default:      DOLCOPY(+=,);       break;
            }
            else switch(oper) {
                case C_XOR:   DOBCOPY(+=,_xor);   break;
                case C_OR:    DOBCOPY(+=,_or);    break;
                case C_AND:   DOBCOPY(+=,_and);   break;
                case C_IMAGE: DOBCOPY(+=,_image); break;
                default:      DOBCOPY(+=,);       break;
            }
            dptr += dskip;
            sptr += sskip;
        } while(--h != 0);
}

#undef CPSIZE_b
#undef CPSIZE_w
#undef CPSIZE_l
#undef poke_b_image
#undef DO1COPY
#undef DOBCOPY
#undef DOLCOPY

void _GrFrDrvPackedBitBltV2V_LFB(GrFrame *dst,int dx,int dy,
                                 GrFrame *src,int sx,int sy,
                                 int w,int h,long op)
{
        char far *dptr = &dst->gf_baseaddr[0][umul32(dy,dst->gf_lineoffset)+dx];
        char far *sptr = &src->gf_baseaddr[0][umul32(sy,src->gf_lineoffset)+sx];
        int   dskip = dst->gf_lineoffset - w;
        int   sskip = src->gf_lineoffset - w;
        int   oper  = C_OPER(op);
        uchar cval  = (uchar)op;
        int   www,ww;
#       define CPSIZE_b     sizeof(uchar)
#       define CPSIZE_w     sizeof(ushort)
#       define CPSIZE_l     sizeof(ulong)
#       define poke_b_f_set   poke_b_f
#       define poke_w_f_set   poke_w_f
#       define poke_l_f_set   poke_l_f
#       define poke_b_f_image(p,v) {                                    \
            uchar val = (v);                                            \
            if(val != cval) poke_b_f(p,val);                            \
        }
#       define DOBCPYF(OP) do {                                         \
            poke_b_f##OP(dptr,peek_b_f(sptr));                          \
            dptr++; sptr++;                                             \
        } while(--ww != 0)
#       define DOBCPYR(OP) do {                                         \
            dptr--; sptr--;                                             \
            poke_b_f##OP(dptr,peek_b_f(sptr));                          \
        } while(--ww != 0)
#       define DOLCPYF(OP) {                                            \
            if((int)dptr & 1) {                                         \
                poke_b_f##OP(dptr,peek_b_f(sptr));                      \
                dptr++; sptr++; ww--;                                   \
            }                                                           \
            if((int)dptr & 2) {                                         \
                poke_w_f##OP(dptr,peek_w_f(sptr));                      \
                dptr += 2; sptr += 2; ww -= 2;                          \
            }                                                           \
            www = ww >> 2;                                              \
            do {                                                        \
                poke_l_f##OP(dptr,peek_l_f(sptr));                      \
                dptr += 4; sptr += 4;                                   \
            } while(--www != 0);                                        \
            if(ww & 2) {                                                \
                poke_w_f##OP(dptr,peek_w_f(sptr));                      \
                dptr += 2; sptr += 2;                                   \
            }                                                           \
            if(ww & 1) {                                                \
                poke_b_f##OP(dptr,peek_b_f(sptr));                      \
                dptr++; sptr++;                                         \
            }                                                           \
        }
#       define DOLCPYR(OP) {                                            \
            if((int)dptr & 1) {                                         \
                dptr--; sptr--; ww--;                                   \
                poke_b_f##OP(dptr,peek_b_f(sptr));                      \
            }                                                           \
            if((int)dptr & 2) {                                         \
                dptr -= 2; sptr -= 2; ww -= 2;                          \
                poke_w_f##OP(dptr,peek_w_f(sptr));                      \
            }                                                           \
            www = ww >> 2;                                              \
            do {                                                        \
                dptr -= 4; sptr -= 4;                                   \
                poke_l_f##OP(dptr,peek_l_f(sptr));                      \
            } while(--www != 0);                                        \
            if(ww & 2) {                                                \
                dptr -= 2; sptr -= 2;                                   \
                poke_w_f##OP(dptr,peek_w_f(sptr));                      \
            }                                                           \
            if(ww & 1) {                                                \
                dptr--; sptr--;                                         \
                poke_b_f##OP(dptr,peek_b_f(sptr));                      \
            }                                                           \
        }
        if(dptr > sptr) {
            dptr += ((h - 1) * dst->gf_lineoffset) + w;
            sptr += ((h - 1) * src->gf_lineoffset) + w;
            if((w >= 7) && (oper != C_IMAGE)) do {
                ww = w;
                switch(oper) {
                    case C_XOR:   DOLCPYR(_xor);    break;
                    case C_OR:    DOLCPYR(_or);     break;
                    case C_AND:   DOLCPYR(_and);    break;
                    default:      DOLCPYR(_set);    break;
                }
                dptr -= dskip;
                sptr -= sskip;
            } while(--h != 0);
            else do {
                ww = w;
                switch(oper) {
                    case C_XOR:   DOBCPYR(_xor);    break;
                    case C_OR:    DOBCPYR(_or);     break;
                    case C_AND:   DOBCPYR(_and);    break;
                    case C_IMAGE: DOBCPYR(_image);  break;
                    default:      DOBCPYR(_set);    break;
                }
                dptr -= dskip;
                sptr -= sskip;
            } while(--h != 0);
        }
        else {
            if((w >= 7) && (oper != C_IMAGE)) do {
                ww = w;
                switch(oper) {
                    case C_XOR:   DOLCPYF(_xor);    break;
                    case C_OR:    DOLCPYF(_or);     break;
                    case C_AND:   DOLCPYF(_and);    break;
                    default:      DOLCPYF(_set);    break;
                }
                dptr += dskip;
                sptr += sskip;
            } while(--h != 0);
            else do {
                ww = w;
                switch(oper) {
                    case C_XOR:   DOBCPYF(_xor);    break;
                    case C_OR:    DOBCPYF(_or);     break;
                    case C_AND:   DOBCPYF(_and);    break;
                    case C_IMAGE: DOBCPYF(_image);  break;
                    default:      DOBCPYF(_set);    break;
                }
                dptr += dskip;
                sptr += sskip;
            } while(--h != 0);
        }
}

#undef CPSIZE_b
#undef CPSIZE_w
#undef CPSIZE_l
#undef poke_b_f_set
#undef poke_w_f_set
#undef poke_l_f_set
#undef poke_b_f_image
#undef DOBCPYF
#undef DOBCPYR
#undef DOLCPYF
#undef DOLCPYR

/* -------------------------------------------------------------------- */

static
#include "fdrivers/generic/pattern.c"

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
    _GrFrDrvPackedBitBltV2V_LFB,
    _GrFrDrvPackedBitBltV2R_LFB,
    _GrFrDrvPackedBitBltR2V_LFB
};

#endif
