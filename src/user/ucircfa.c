/**
 **
 **     UCIRCFA.C
 **
 **     Copyright (C), Michael Goffioul
 **
 **
 ** Note : compiling the library with -DUSR_KEEP_SHAPE makes a circle
 **        looks like a circle on the screen
 **/

#include "libgrx.h"
#include "usercord.h"

void GrUsrFilledCircleArc(int xc,int yc,int r,int start,int end,int style,GrColor c)
{
#ifdef USR_KEEP_SHAPE
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(r,r,CURC->gc_xmax,CURC->gc_usrwidth);
        GrFilledCircleArc(xc,yc,r,start,end,style,c);
#else
        GrUsrFilledEllipseArc(xc,yc,r,r,start,end,style,c);
#endif /* USR_KEEP_SHAPE */
}

