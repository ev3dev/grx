/**
 ** MOUINFO.C ---- the mouse info data structure
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#undef         GrMouseInfo

const
struct _GR_mouseInfo * const GrMouseInfo = &_GrMouseInfo;
struct _GR_mouseInfo _GrMouseInfo = { 0 };
