/**
 ** upfelli.c
 **
 ** Copyright (C) 1997, Michael Goffioul
 ** [goffioul@emic.ucl.ac.be]
 **
 **/

#include "libgrx.h"
#include "usercord.h"

void GrUsrPatternFilledEllipse(int xc,int yc,int xa,int ya,GrPattern *p)
{
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(xa,xa,CURC->x_max,CURC->user_width);
        SCALE(ya,ya,CURC->y_max,CURC->user_height);
        GrPatternFilledEllipse(xc,yc,xa,ya,p);
}
