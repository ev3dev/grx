/**
 ** CLEARSCR.C ---- clear screen
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"

void GrClearScreen(GrColor bg)
{
        GrContext save;
        GrSaveContext(&save);
        GrSetContext(SCRN);
        GrClearContext(bg);
        GrSetContext(&save);
}
