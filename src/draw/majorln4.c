/**
 ** MAJORLN4.C ---- lines parallel with the Y axis (no clip)
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "clipping.h"

void GrVLineNC(int xx,int y1,int y2,GrColor c)
{
        isort(y1,y2);
        (*FDRV->drawvline)(
            xx + CURC->gc_xoffset,
            y1 + CURC->gc_yoffset,
            y2 - y1 + 1,
            c
        );
}


