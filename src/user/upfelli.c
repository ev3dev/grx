/*
 * upfelli.c
 *
 * Copyright (C) 1997, Michael Goffioul
 * [goffioul@emic.ucl.ac.be]
 *
 */

#include <grx/pattern.h>

#include "globals.h"
#include "libgrx.h"
#include "usercord.h"

void grx_user_draw_filled_ellipse_with_pattern(int xc,int yc,int xa,int ya,GrxPattern *p)
{
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(xa,xa,CURC->x_max,CURC->user_width);
        SCALE(ya,ya,CURC->y_max,CURC->user_height);
        grx_draw_filled_ellipse_with_pattern(xc,yc,xa,ya,p);
}
