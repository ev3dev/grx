/**
 ** MAJORLN3.C ---- lines parallel with the X axis (no clip)
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "clipping.h"

void GrHLineNC(int x1,int x2,int yy,GrColor c)
{
        isort(x1,x2);
        (*FDRV->drawhline)(
            x1 + CURC->gc_xoffset,
            yy + CURC->gc_yoffset,
            x2 - x1 + 1,
            c
        );
}
