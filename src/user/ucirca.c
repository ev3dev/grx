/*
 * ucirca.c
 *
 * Copyright (C), Michael Goffioul
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
 * Note : compiling the library with -DUSR_KEEP_SHAPE makes a circle
 *        looks like a circle on the screen
 *
 */

#include "libgrx.h"
#include "usercord.h"

void grx_user_draw_circle_arc(int xc,int yc,int r,int start,int end,GrxArcStyle style,GrxColor c)
{
#ifdef USR_KEEP_SHAPE
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(r,r,CURC->x_max,CURC->user_width);
        grx_draw_circle_arc(xc,yc,r,start,end,style,c);
#else
        grx_user_draw_ellipse_arc(xc,yc,r,r,start,end,style,c);
#endif /* USR_KEEP_SHAPE */
}
