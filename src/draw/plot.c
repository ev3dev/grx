/**
 ** PLOT.C ---- pixel draw routines
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "arith.h"
#include "clipping.h"

void GrPlot(int x,int y,GrColor c)
{
        clip_dot(CURC,x,y);
        mouse_block(CURC,x,y,x,y);
        (*FDRV->drawpixel)(
            x + CURC->gc_xoffset,
            y + CURC->gc_yoffset,
            c
        );
        mouse_unblock();
}

