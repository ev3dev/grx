/**
 ** VTABLE.C ---- a table of available video drivers
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "grdriver.h"

GrVideoDriver *_GrVideoDriverTable[] = {
#if defined(__MSDOS__)
    &_GrVideoDriverHERC,
    &_GrVideoDriverSTDEGA,
    &_GrVideoDriverSTDVGA,
    &_GrVideoDriverVESA,
    &_GrVideoDriverATI28800,
    &_GrVideoDriverET4000,
    &_GrVideoDriverCL5426,
    &_GrVideoDriverMACH64,
    &_GrVideoDriverS3,
#endif
#if defined(__XWIN__)
    &_GrVideoDriverXWIN,
#endif
#if defined(__linux__) && !defined(__XWIN__)
    &_GrVideoDriverSVGALIB,
#endif
    &_GrDriverMEM,
    NULL
};

