/**
 ** FILLBOX.C ---- draw a filled rectangle
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "clipping.h"

void GrFilledBox(int x1,int y1,int x2,int y2,long c)
{
        clip_box(CURC,x1,y1,x2,y2);
        mouse_block(CURC,x1,y1,x2,y2);
        (*FDRV->drawblock)(
            x1 + CURC->gc_xoffset,
            y1 + CURC->gc_yoffset,
            x2 - x1 + 1,
            y2 - y1 + 1,
            c
        );
        mouse_unblock();
}

void GrFilledBoxNC(int x1,int y1,int x2,int y2,long c)
{
        (*FDRV->drawblock)(
            x1 + CURC->gc_xoffset,
            y1 + CURC->gc_yoffset,
            x2 - x1 + 1,
            y2 - y1 + 1,
            c
        );
}

