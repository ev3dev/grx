/**
 **     UCCIRCA.C
 **
 **     Copyright (C) 1997, Michael Goffioul
 **
 **       goffioul@emic.ucl.ac.be
 **
 **
 ** Note : compiling the library with -DUSR_KEEP_SHAPE makes a circle
 **        looks like a circle on the screen
 **/

#include "libgrx.h"
#include "usercord.h"

void GrUsrCustomCircleArc(int xc,int yc,int r,int start,int end,int style,GrLineOption *lo)
{
#ifdef USR_KEEP_SHAPE
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(r,r,CURC->gc_xmax,CURC->gc_usrwidth);
        GrCustomCircleArc(xc,yc,r,start,end,style,lo);
#else
        GrUsrCustomEllipseArc(xc,yc,r,r,start,end,style,lo);
#endif /* USR_KEEP_SHAPE */
}
