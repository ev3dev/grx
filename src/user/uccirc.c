/**
 **     UCCIRC.C
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

void GrUsrCustomCircle(int xc,int yc,int r,GrLineOption *lo)
{
#ifdef USR_KEEP_SHAPE
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(r,r,CURC->gc_xmax,CURC->gc_usrwidth);
        GrCustomCircle(xc,yc,r,lo);
#else
        GrUsrCustomEllipse(xc,yc,r,r,lo);
#endif /* USR_KEEP_SHAPE */
}
