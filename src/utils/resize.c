/**
 ** RESIZE.C ---- function to resize a two-D map of gray (0..255) pixels
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include <stdio.h>
#include <stdlib.h>

#include "libgrx.h"

static void near shrink(uchar far *ptr,int pitch,uint oldlen,uint newlen)
{
        register uchar far *dst = ptr;
        register uchar far *src = ptr;
        int  count  = newlen;
        uint weight = newlen;
        do {
            uint collect = oldlen;
            uint pixsum  = 0;
            do {
                uint factor;
                if(weight == 0) weight = newlen,src += pitch;
                if((factor = weight) > collect) factor = collect;
                pixsum  += factor * (*src);
                weight  -= factor;
                collect -= factor;
            } while(collect > 0);
            *dst = pixsum / oldlen;
            dst += pitch;
        } while(--count > 0);
#ifdef DEBUG
        if(dst != (ptr + (newlen * pitch))) {
            fprintf(stderr,"resize: dst error (shrink)\n");
            exit(1);
        }
        if(src != (ptr + ((oldlen - 1) * pitch))) {
            fprintf(stderr,"resize: src error (shrink)\n");
            exit(1);
        }
#endif
}

static void grow(uchar far *ptr,int pitch,uint oldlen,uint newlen)
{
        register uchar far *dst = ptr + (--newlen * pitch);
        register uchar far *src = ptr + (--oldlen * pitch);
        uint rpix  = *src;
        uint lpix  = rpix;
        int  count = newlen;
        int  scale = oldlen;
        do {
            if((scale -= oldlen) < 0) {
                rpix   = lpix;
                lpix   = *(src -= pitch);
                scale += newlen;
            }
            *dst = ((lpix * (newlen - scale)) + (rpix * scale)) / newlen;
            dst -= pitch;
        } while(--count >= 0);
#ifdef DEBUG
        if(dst != (ptr - pitch)) {
            fprintf(stderr,"resize: dst error (grow)\n");
            exit(1);
        }
        if(src != ptr) {
            fprintf(stderr,"resize: src error (grow)\n");
            exit(1);
        }
#endif
}

void GrResizeGrayMap(uchar far *map,int pitch,int ow,int oh,int nw,int nh)
{
        if(ow != nw) {
            uchar far *ptr = map;
            int          cnt = oh;
            if((uint)ow > (uint)nw) do {
                shrink(ptr,1,ow,nw);
                ptr += pitch;
            } while(--cnt > 0);
            else do {
                grow(ptr,1,ow,nw);
                ptr += pitch;
            } while(--cnt > 0);
        }
        if(oh != nh) {
            int cnt = nw;
            if((uint)oh > (uint)nh) do {
                shrink(map,pitch,oh,nh);
                map++;
            } while(--cnt > 0);
            else do {
                grow(map,pitch,oh,nh);
                map++;
            } while(--cnt > 0);
        }
}
