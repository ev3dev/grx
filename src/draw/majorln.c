/**
 ** MAJORLN.C ---- lines parallel with the X or Y axis
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "clipping.h"

void GrHLine(int x1,int x2,int yy,long c)
{
        clip_hline(CURC,x1,x2,yy);
        mouse_block(CURC,x1,yy,x2,yy);
        (*FDRV->drawhline)(
            x1 + CURC->gc_xoffset,
            yy + CURC->gc_yoffset,
            x2 - x1 + 1,
            c
        );
        mouse_unblock();
}

void GrVLine(int xx,int y1,int y2,long c)
{
        clip_vline(CURC,xx,y1,y2);
        mouse_block(CURC,xx,y1,xx,y2);
        (*FDRV->drawvline)(
            xx + CURC->gc_xoffset,
            y1 + CURC->gc_yoffset,
            y2 - y1 + 1,
            c
        );
        mouse_unblock();
}

void GrHLineNC(int x1,int x2,int yy,long c)
{
        isort(x1,x2);
        (*FDRV->drawhline)(
            x1 + CURC->gc_xoffset,
            yy + CURC->gc_yoffset,
            x2 - x1 + 1,
            c
        );
}

void GrVLineNC(int xx,int y1,int y2,long c)
{
        isort(y1,y2);
        (*FDRV->drawvline)(
            xx + CURC->gc_xoffset,
            y1 + CURC->gc_yoffset,
            y2 - y1 + 1,
            c
        );
}


