/**
 **
 **     UCIRCA.C
 **
 **     Copyright (C), Michael Goffioul
 **
 **
 ** Note : compiling the library with -DUSR_KEEP_SHAPE makes a circle
 **        looks like a circle on the screen
 **/

#include "libgrx.h"
#include "usercord.h"

void GrUsrCircleArc(int xc,int yc,int r,int start,int end,int style,GrColor c)
{
#ifdef USR_KEEP_SHAPE
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(r,r,CURC->gc_xmax,CURC->gc_usrwidth);
        GrCircleArc(xc,yc,r,start,end,style,c);
#else
        GrUsrEllipseArc(xc,yc,r,r,start,end,style,c);
#endif /* USR_KEEP_SHAPE */
}
