/**
 ** MAJORLN1.C ---- lines parallel with the X axis
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "clipping.h"

void GrHLine(int x1,int x2,int yy,GrColor c)
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
