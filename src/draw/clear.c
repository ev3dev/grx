/**
 ** CLEAR.C ---- clear context, screen or clipbox
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "memcopy.h"

void GrClearClipBox(long bg)
{
        GrFilledBox(0,0,CURC->gc_xmax,CURC->gc_ymax,bg);
}

void GrClearContext(long bg)
{
        mouse_block(CURC,0,0,CURC->gc_xmax,CURC->gc_ymax);
        GrFilledBoxNC(0,0,CURC->gc_xmax,CURC->gc_ymax,bg);
        mouse_unblock();
}

void GrClearScreen(long bg)
{
        GrContext save;
        GrSaveContext(&save);
        GrSetContext(SCRN);
        GrClearContext(bg);
        GrSetContext(&save);
}

