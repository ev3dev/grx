/**
 ** LINENC.C ---- line drawing (no clip)
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "clipping.h"

void GrLineNC(int x1,int y1,int x2,int y2,GrColor c)
{
        (*FDRV->drawline)(
            x1 + CURC->gc_xoffset,
            y1 + CURC->gc_yoffset,
            x2 - x1,
            y2 - y1,
            c
        );
}

