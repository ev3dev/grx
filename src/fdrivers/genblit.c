/**
 ** GENBLIT.C ---- generic, VERY SLOW bitblt routine
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "grdriver.h"
#include "libgrx.h"
#include "memcopy.h"

void _GrFrDrvGenericBitBlt(GrFrame *dst,int dx,int dy,GrFrame *src,int x,int y,int w,int h,long op)
{
        GrFrame csave;
        long (*readpix)(GrFrame *,int,int) = src->gf_driver->readpixel;
        void (*drawpix)(int,int,long) = dst->gf_driver->drawpixel;
        long skipc = op ^ GrIMAGE;
        int  step  = 1;
        op &= GrCMODEMASK;
        sttcopy(&csave,&CURC->gc_frame);
        sttcopy(&CURC->gc_frame,dst);
        if((dy > y) || ((dy == y) && (dx > x))) {
            x += (w - 1); dx += (w - 1);
            y += (h - 1); dy += (h - 1);
            step = (-1);
        }
        do {
            int dxx = dx,xx = x,ww = w;
            do {
                long c = (*readpix)(src,xx,y);
                if(c != skipc) (*drawpix)(dxx,dy,(c | op));
                dxx += step; xx += step;
            } while(--ww > 0);
            dy += step; y += step;
        } while(--h > 0);
        sttcopy(&CURC->gc_frame,&csave);
}

