/**
 ** FDTABLE.C ---- a table of available font drivers
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "grfontdv.h"

GrFontDriver *_GrFontDriverTable[] = {
#ifdef __XWIN__
    &_GrFontDriverXWIN,
#endif
    &_GrFontDriverGRX,
    &_GrFontDriverBGI,
    NULL
};

