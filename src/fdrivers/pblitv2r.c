/**
 ** PBLITV2R.C ---- video to RAM bitblt routine for packed (8, 16, 24, 32 bpp) modes
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "grdriver.h"
#include "libgrx.h"
#include "arith.h"
#include "mempeek.h"

void _GrFrDrvPackedBitBltV2R(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,long op)
{
        char  far *dptr = &dst->gf_baseaddr[0][(dy * dst->gf_lineoffset) + dx];
        ulong           soff =                 umul32(sy , src->gf_lineoffset) + sx;
        int   dskip = dst->gf_lineoffset - w;
        int   sskip = src->gf_lineoffset - w;
        int   oper  = C_OPER(op);
        int   fast  = (oper == C_IMAGE) ? 0 : ~0;
        uchar cval  = (uchar)op;
        int   ww;
#        define CPSIZE_b            sizeof(uchar)
#        define CPSIZE_w            sizeof(ushort)
#        define CPSIZE_l            sizeof(ulong)
#        define poke_b_image(p,v) {                                        \
            uchar val = (v);                                                \
            if(val != cval) poke_b(p,val);                                \
        }
#        define DO1COPY(SIZE,PTRINC,OP) {                                \
            poke_##SIZE##OP(dptr,peek_##SIZE##_f(sptr));                \
            dptr PTRINC CPSIZE_##SIZE;                                        \
            sptr PTRINC CPSIZE_##SIZE;                                        \
        }
#        define DOBCOPY(PTRINC,OP) do {                                        \
            DO1COPY(b,PTRINC,OP);                                        \
        } while(--w1 != 0)
#        define DOLCOPY(PTRINC,OP) {                                        \
            if((int)sptr & 1) { DO1COPY(b,PTRINC,OP); w1 -= 1; }        \
            if((int)sptr & 2) { DO1COPY(w,PTRINC,OP); w1 -= 2; }        \
            ww = w1 >> 2;                                                \
            do { DO1COPY(l,PTRINC,OP); } while(--ww != 0);                \
            if(w1 & 2) { DO1COPY(w,PTRINC,OP); }                        \
            if(w1 & 1) { DO1COPY(b,PTRINC,OP); }                        \
        }
        setup_far_selector(src->gf_selector);
        do {
            uint w1 = BANKLFT(soff);
            uint w2 = w - (w1 = umin(w,w1));
            do {
                char far *sptr = &src->gf_baseaddr[0][BANKPOS(soff)];
                CHKBANK(BANKNUM(soff));
                soff += w1;
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
            dptr += dskip;
            soff += sskip;
        } while(--h != 0);
}

