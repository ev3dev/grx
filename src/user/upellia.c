/**
 ** upellia.c
 **
 ** Copyright (C) 1997, Michael Goffioul
 ** [goffioul@emic.ucl.ac.be]
 **
 ** This file is part of the GRX graphics library.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 **/

#include "libgrx.h"
#include "usercord.h"


void GrUsrPatternedEllipseArc(int xc,int yc,int xa,int ya,int start,int end,GrxArcStyle style,GrxLinePattern *lp)
{
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(xa,xa,CURC->x_max,CURC->user_width);
        SCALE(ya,ya,CURC->y_max,CURC->user_height);
        grx_draw_ellipse_arc_with_pattern(xc,yc,xa,ya,start,end,style,lp);
}
