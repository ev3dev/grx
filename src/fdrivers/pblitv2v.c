/**
 ** PBLITV2V.C ---- video to video bitblt routine for packed (8, 16, 24, 32 bpp) modes
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "grdriver.h"
#include "libgrx.h"
#include "alloca.h"
#include "arith.h"
#include "mempeek.h"

static
#ifdef  __GNUC__
inline
#endif
void dualpageblt(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h)
{
        ulong doff,soff;
        int   dskip,sskip;
        int   rb,rbb;
        int   wb,wbb;
        if(dy > sy) {
            dy         += (h - 1);
            sy         += (h - 1);
            dskip = -(dst->gf_lineoffset + w);
            sskip = -(src->gf_lineoffset + w);
        }
        else {
            dskip = dst->gf_lineoffset - w;
            sskip = src->gf_lineoffset - w;
        }
        doff = umul32(dy,dst->gf_lineoffset) + dx;
        soff = umul32(sy,src->gf_lineoffset) + sx;
        wbb  = (-1);
        rbb  = (-1);
        setup_far_selector(dst->gf_selector);
        do {
            uint w1 = BANKLFT(doff);
            uint w2 = BANKLFT(soff);
            uint w3,ww;
            w1 = umin(w,w1);
            w2 = umin(w,w2);
            usort(w1,w2);
            w3 = w  - w2;
            w2 = w2 - w1;
            if(w2 == 0) w2 = w3,w3 = 0;
            do {
                char far *dptr = &dst->gf_baseaddr[0][BANKPOS(doff)];
                char far *sptr = &src->gf_baseaddr[0][BANKPOS(soff)];
                wb = BANKNUM(doff);
                rb = BANKNUM(soff);
                if((rbb - rb) | (wbb - wb)) SRWBANK((rbb = rb),(wbb = wb));
                doff += w1;
                soff += w1;
                if(w1 >= 3) {
                    if((int)(dptr) & 1) {
                        poke_b_f(dptr,peek_b_f(sptr));
                        dptr++; sptr++; w1--;
                    }
                    if((int)(dptr) & 2) {
                        poke_w_f(dptr,peek_w_f(sptr));
                        dptr += 2; sptr += 2; w1 -= 2;
                    }
                    if((ww = w1 >> 2) > 0) do {
                        poke_l_f(dptr,peek_l_f(sptr));
                        dptr += 4; sptr += 4;
                    } while(--ww != 0);
                }
                if(w1 & 2) {
                    poke_w_f(dptr,peek_w_f(sptr));
                    dptr += 2; sptr += 2;
                }
                if(w1 & 1) {
                    poke_b_f(dptr,peek_b_f(sptr));
                }
                w1 = w2;
                w2 = w3;
                w3 = 0;
            } while(w1 != 0);
            doff += dskip;
            soff += sskip;
        } while(--h != 0);
}

#ifdef  __TURBOC__
#define TMPSIZE                2048
#else
#define TMPSIZE                8192
#endif

void _GrFrDrvPackedBitBltV2V(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,long op)
{
        GrFrame tmp;
        int tmpx,tmpn;
        if((C_OPER(op) == C_WRITE) && DRVINFO->splitbanks && ((dy != sy) || (sx >= dx))) {
            dualpageblt(dst,dx,dy,src,sx,sy,w,h);
            return;
        }
        setup_alloca();
        tmp.gf_lineoffset = (w + 7) & ~3;
        tmpn = umax(umin(h,(TMPSIZE / tmp.gf_lineoffset)),1);
        tmpx = tmp.gf_lineoffset * tmpn;
        tmp.gf_baseaddr[0] = alloca(tmpx);
        if(tmp.gf_baseaddr[0]) {
            int ydir = 0;
            tmpx = sx & 3;
            if(dy > sy) {
                dy  += h;
                sy  += h;
                ydir = ~0;
            }
            do {
                int cnt = umin(h,tmpn);
                dy -= (ydir & cnt);
                sy -= (ydir & cnt);
                _GrFrDrvPackedBitBltV2R(&tmp,tmpx,0,src,sx,sy,w,cnt,GrWRITE);
                _GrFrDrvPackedBitBltR2V(dst,dx,dy,&tmp,tmpx,0,w,cnt,op);
                dy += (~ydir & cnt);
                sy += (~ydir & cnt);
                h  -= cnt;
            } while(h != 0);
        }
        reset_alloca();
}

