/**
 ** CLEARCLP.C ---- clear clipbox
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"

void GrClearClipBox(GrColor bg)
{
        GrFilledBox(0,0,CURC->gc_xmax,CURC->gc_ymax,bg);
}

