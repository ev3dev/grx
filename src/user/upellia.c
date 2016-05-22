/*
 * upellia.c
 *
 * Copyright (C) 1997, Michael Goffioul
 * [goffioul@emic.ucl.ac.be]
 *
 * This file is part of the GRX graphics library.
 *
 * The GRX graphics library is free software; you can redistribute it
 * and/or modify it under some conditions; see the "copying.grx" file
 * for details.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <grx/draw.h>
#include <grx/pattern.h>

#include "globals.h"
#include "libgrx.h"
#include "usercord.h"


void grx_user_draw_ellipse_arc_with_pattern(int xc,int yc,int rx,int ry,int start,int end,GrxArcStyle style,GrxLinePattern *lp)
{
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(rx,rx,CURC->x_max,CURC->user_width);
        SCALE(ry,ry,CURC->y_max,CURC->user_height);
        grx_draw_ellipse_arc_with_pattern(xc,yc,rx,ry,start,end,style,lp);
}
