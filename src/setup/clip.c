/**
 ** CLIP.C ---- clip box setting utilities
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "clipping.h"

void GrSetClipBox(int x1,int y1,int x2,int y2)
{
        cxclip_box(CURC,x1,y1,x2,y2);
        CURC->gc_xcliplo = x1;
        CURC->gc_ycliplo = y1;
        CURC->gc_xcliphi = x2;
        CURC->gc_ycliphi = y2;
}

void GrSetClipBoxC(GrContext *c,int x1,int y1,int x2,int y2)
{
        cxclip_box(c,x1,y1,x2,y2);
        c->gc_xcliplo = x1;
        c->gc_ycliplo = y1;
        c->gc_xcliphi = x2;
        c->gc_ycliphi = y2;
}

void GrResetClipBox()
{
        CURC->gc_xcliplo = 0;
        CURC->gc_ycliplo = 0;
        CURC->gc_xcliphi = CURC->gc_xmax;
        CURC->gc_ycliphi = CURC->gc_ymax;
}

void GrResetClipBoxC(GrContext *c)
{
        c->gc_xcliplo = 0;
        c->gc_ycliplo = 0;
        c->gc_xcliphi = c->gc_xmax;
        c->gc_ycliphi = c->gc_ymax;
}

