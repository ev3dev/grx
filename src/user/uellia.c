/**
 **
 **     UELLIA.C
 **
 **     Copyright (C), Michael Goffioul
 **
 **/

#include "libgrx.h"
#include "usercord.h"

void GrUsrEllipseArc(int xc,int yc,int xa,int ya,int start,int end,int style,GrColor c)
{
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(xa,xa,CURC->gc_xmax,CURC->gc_usrwidth);
        SCALE(ya,ya,CURC->gc_ymax,CURC->gc_usrheight);
        GrEllipseArc(xc,yc,xa,ya,start,end,style,c);
}

