/**
 ** COLORBW.C ---- standard colors: black and white
 **
 ** Copyright (c) 1998 Hartmut Schirmer
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"

#ifdef GrBlack
#undef GrBlack
#endif
GrColor GrBlack(void)
{
        GRX_ENTER();
        if(CLRINFO->black == GrNOCOLOR) CLRINFO->black = GrAllocColor(0,0,0);
               GRX_RETURN(CLRINFO->black);
}

#ifdef GrWhite
#undef GrWhite
#endif
GrColor GrWhite(void)
{
        GRX_ENTER();
        if(CLRINFO->white == GrNOCOLOR) CLRINFO->white = GrAllocColor(255,255,255);
        GRX_RETURN(CLRINFO->white);
}

