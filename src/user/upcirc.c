/**
 **     UPCIRC.C
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

void GrUsrPatternedCircle(int xc,int yc,int r,GrLinePattern *lp)
{
#ifdef USR_KEEP_SHAPE
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(r,r,CURC->gc_xmax,CURC->gc_usrwidth);
        GrPatternedCircle(xc,yc,r,lp);
#else
        GrUsrPatternedEllipse(xc,yc,r,r,lp);
#endif /* USR_KEEP_SHAPE */
}
