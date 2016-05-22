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

void grx_user_draw_filled_ellipse_with_pattern(int xc,int yc,int rx,int ry,GrxPattern *p)
{
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(rx,rx,CURC->x_max,CURC->user_width);
        SCALE(ry,ry,CURC->y_max,CURC->user_height);
        grx_draw_filled_ellipse_with_pattern(xc,yc,rx,ry,p);
}
