/**
 ** PBLITR2V.C ---- RAM to video bitblt routine for packed (8, 16, 24, 32 bpp) modes
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "grdriver.h"
#include "libgrx.h"
#include "arith.h"
#include "mempeek.h"

void _GrFrDrvPackedBitBltR2V(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,long op)
{
        ulong           doff =                 umul32(dy , dst->gf_lineoffset) + dx;
        char  far *sptr = &src->gf_baseaddr[0][(sy * src->gf_lineoffset) + sx];
        int   dskip = dst->gf_lineoffset - w;
        int   sskip = src->gf_lineoffset - w;
        int   oper  = C_OPER(op);
        int   fast  = (oper == C_IMAGE) ? 0 : ~0;
        uchar cval  = (uchar)op;
        int   ww;
#        define CPSIZE_b            sizeof(uchar)
#        define CPSIZE_w            sizeof(ushort)
#        define CPSIZE_l            sizeof(ulong)
#        define poke_b_f_image(p,v) {                                        \
            uchar val = (v);                                                \
            if(val != cval) poke_b_f(p,val);                                \
        }
#        define DO1COPY(SIZE,PTRINC,OP) {                                \
            poke_##SIZE##_f##OP(dptr,peek_##SIZE(sptr));                \
            dptr PTRINC CPSIZE_##SIZE;                                        \
            sptr PTRINC CPSIZE_##SIZE;                                        \
        }
#        define DOBCOPY(PTRINC,OP) do {                                        \
            DO1COPY(b,PTRINC,OP);                                        \
        } while(--w1 != 0)
#        define DOLCOPY(PTRINC,OP) {                                        \
            if((int)dptr & 1) { DO1COPY(b,PTRINC,OP); w1 -= 1; }        \
            if((int)dptr & 2) { DO1COPY(w,PTRINC,OP); w1 -= 2; }        \
            ww = w1 >> 2;                                                \
            do { DO1COPY(l,PTRINC,OP); } while(--ww != 0);                \
            if(w1 & 2) { DO1COPY(w,PTRINC,OP); }                        \
            if(w1 & 1) { DO1COPY(b,PTRINC,OP); }                        \
        }
        setup_far_selector(dst->gf_selector);
        do {
            uint w1 = BANKLFT(doff);
            uint w2 = w - (w1 = umin(w,w1));
            do {
                char far *dptr = &dst->gf_baseaddr[0][BANKPOS(doff)];
                CHKBANK(BANKNUM(doff));
                doff += w1;
                if((w1 & fast) >= 7) switch(oper) {
                    case C_XOR:          DOLCOPY(+=,_xor);   break;
                    case C_OR:          DOLCOPY(+=,_or);    break;
                    case C_AND:          DOLCOPY(+=,_and);   break;
                    default:          DOLCOPY(+=,);              break;
                }
                else switch(oper) {
                    case C_XOR:          DOBCOPY(+=,_xor);   break;
                    case C_OR:          DOBCOPY(+=,_or);    break;
                    case C_AND:          DOBCOPY(+=,_and);   break;
                    case C_IMAGE: DOBCOPY(+=,_image); break;
                    default:          DOBCOPY(+=,);              break;
                }
                w1 = w2;
                w2 = 0;
            } while(w1 != 0);
            doff += dskip;
            sptr += sskip;
        } while(--h != 0);
}

