/**
 ** CLEARCTX.C ---- clear context
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"

void GrClearContext(GrColor bg)
{
        mouse_block(CURC,0,0,CURC->gc_xmax,CURC->gc_ymax);
        GrFilledBoxNC(0,0,CURC->gc_xmax,CURC->gc_ymax,bg);
        mouse_unblock();
}
