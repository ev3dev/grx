/**
 ** PBLITR2R.C ---- RAM to RAM bitblt routine for packed (8, 16, 24, 32 bpp) modes
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "grdriver.h"
#include "libgrx.h"
#include "mempeek.h"

void _GrFrDrvPackedBitBltR2R(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,long op)
{
        char  far *dptr = &dst->gf_baseaddr[0][(dy * dst->gf_lineoffset) + dx];
        char  far *sptr = &src->gf_baseaddr[0][(sy * src->gf_lineoffset) + sx];
        int   dskip = dst->gf_lineoffset - w;
        int   sskip = src->gf_lineoffset - w;
        int   oper  = C_OPER(op);
        uchar cval  = (uchar)op;
        int   www,ww;
#        define CPSIZE_b            sizeof(uchar)
#        define CPSIZE_w            sizeof(ushort)
#        define CPSIZE_l            sizeof(ulong)
#        define poke_b_set   poke_b
#        define poke_w_set   poke_w
#        define poke_l_set   poke_l
#        define poke_b_image(p,v) {                                        \
            uchar val = (v);                                                \
            if(val != cval) poke_b(p,val);                                \
        }
#        define DOBCPYF(OP) do {                                                \
            poke_b##OP(dptr,peek_b(sptr));                                \
            dptr++; sptr++;                                                \
        } while(--ww != 0)
#        define DOBCPYR(OP) do {                                                \
            dptr--; sptr--;                                                \
            poke_b##OP(dptr,peek_b(sptr));                                \
        } while(--ww != 0)
#        define DOLCPYF(OP) {                                                \
            if((int)dptr & 1) {                                                \
                poke_b##OP(dptr,peek_b(sptr));                                \
                dptr++; sptr++; ww--;                                        \
            }                                                                \
            if((int)dptr & 2) {                                                \
                poke_w##OP(dptr,peek_w(sptr));                                \
                dptr += 2; sptr += 2; ww -= 2;                                \
            }                                                                \
            www = ww >> 2;                                                \
            do {                                                        \
                poke_l##OP(dptr,peek_l(sptr));                                \
                dptr += 4; sptr += 4;                                        \
            } while(--www != 0);                                        \
            if(ww & 2) {                                                \
                poke_w##OP(dptr,peek_w(sptr));                                \
                dptr += 2; sptr += 2;                                        \
            }                                                                \
            if(ww & 1) {                                                \
                poke_b##OP(dptr,peek_b(sptr));                                \
                dptr++; sptr++;                                                \
            }                                                                \
        }
#        define DOLCPYR(OP) {                                                \
            if((int)dptr & 1) {                                                \
                dptr--; sptr--; ww--;                                        \
                poke_b##OP(dptr,peek_b(sptr));                                \
            }                                                                \
            if((int)dptr & 2) {                                                \
                dptr -= 2; sptr -= 2; ww -= 2;                                \
                poke_w##OP(dptr,peek_w(sptr));                                \
            }                                                                \
            www = ww >> 2;                                                \
            do {                                                        \
                dptr -= 4; sptr -= 4;                                        \
                poke_l##OP(dptr,peek_l(sptr));                                \
            } while(--www != 0);                                        \
            if(ww & 2) {                                                \
                dptr -= 2; sptr -= 2;                                        \
                poke_w##OP(dptr,peek_w(sptr));                                \
            }                                                                \
            if(ww & 1) {                                                \
                dptr--; sptr--;                                                \
                poke_b##OP(dptr,peek_b(sptr));                                \
            }                                                                \
        }
        if(dptr > sptr) {
            dptr += ((h - 1) * dst->gf_lineoffset) + w;
            sptr += ((h - 1) * src->gf_lineoffset) + w;
            if((w >= 7) && (oper != C_IMAGE)) do {
                ww = w;
                switch(oper) {
                    case C_XOR:          DOLCPYR(_xor);    break;
                    case C_OR:          DOLCPYR(_or);            break;
                    case C_AND:          DOLCPYR(_and);    break;
                    default:          DOLCPYR(_set);    break;
                }
                dptr -= dskip;
                sptr -= sskip;
            } while(--h != 0);
            else do {
                ww = w;
                switch(oper) {
                    case C_XOR:          DOBCPYR(_xor);    break;
                    case C_OR:          DOBCPYR(_or);            break;
                    case C_AND:          DOBCPYR(_and);    break;
                    case C_IMAGE: DOBCPYR(_image);  break;
                    default:          DOBCPYR(_set);    break;
                }
                dptr -= dskip;
                sptr -= sskip;
            } while(--h != 0);
        }
        else {
            if((w >= 7) && (oper != C_IMAGE)) do {
                ww = w;
                switch(oper) {
                    case C_XOR:          DOLCPYF(_xor);    break;
                    case C_OR:          DOLCPYF(_or);            break;
                    case C_AND:          DOLCPYF(_and);    break;
                    default:          DOLCPYF(_set);    break;
                }
                dptr += dskip;
                sptr += sskip;
            } while(--h != 0);
            else do {
                ww = w;
                switch(oper) {
                    case C_XOR:          DOBCPYF(_xor);    break;
                    case C_OR:          DOBCPYF(_or);            break;
                    case C_AND:          DOBCPYF(_and);    break;
                    case C_IMAGE: DOBCPYF(_image);  break;
                    default:          DOBCPYF(_set);    break;
                }
                dptr += dskip;
                sptr += sskip;
            } while(--h != 0);
        }
}

