/*
 * ucirc.c
 *
 * Copyright (C) 1992, Csaba Biegl
 *  820 Stirrup Dr, Nashville, TN, 37221
 *  csaba@vuse.vanderbilt.edu
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
 * Note : compiling the library with -DUSR_KEEP_SHAPE makes a circle
 *        looks like a circle on the screen
 *
 */

#ifdef USR_KEEP_SHAPE
#include <grx/draw.h>
#else
#include <grx/user.h>
#endif

#include "globals.h"
#include "libgrx.h"
#include "usercord.h"

/**
 * grx_user_draw_circle:
 * @xc: the X coordinate of the center of the circle
 * @yc: the Y coordinate of the center of the circle
 * @r: the radius of the circle
 * @c: the color
 *
 * Draws a circle on the current context centered at the specified coordinates
 * with the specified radius and color.
 */
void grx_user_draw_circle(int xc,int yc,int r,GrxColor c)
{
#ifdef USR_KEEP_SHAPE
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(r,r,CURC->x_max,CURC->user_width);
        grx_draw_circle(xc,yc,r,c);
#else
        grx_user_draw_ellipse(xc,yc,r,r,c);
#endif
}
