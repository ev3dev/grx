/**
 **     UPELLIA.C
 **
 **     Copyright (C) 1997, Michael Goffioul
 **
 **       goffioul@emic.ucl.ac.be
 **
 **/

#include "libgrx.h"
#include "usercord.h"


void GrUsrPatternedEllipseArc(int xc,int yc,int xa,int ya,int start,int end,int style,GrLinePattern *lp)
{
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(xa,xa,CURC->gc_xmax,CURC->gc_usrwidth);
        SCALE(ya,ya,CURC->gc_ymax,CURC->gc_usrheight);
        GrPatternedEllipseArc(xc,yc,xa,ya,start,end,style,lp);
}
