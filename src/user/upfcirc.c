/*
 * upfcirc.c
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
 */

#include <grx/pattern.h>

#include "libgrx.h"
#include "usercord.h"

/**
 * grx_user_draw_filled_circle_with_pattern:
 * @xc: the X coordinate of the center of the circle
 * @yc: the Y coordinate of the center of the circle
 * @r: the radius of the circle
 * @p: the pattern
 *
 * Draws a filled circle on the current context centered at the specified
 * coordinates with the specified radius and pattern.
 */
void grx_user_draw_filled_circle_with_pattern(int xc,int yc,int r,GrxPattern *p)
{
#ifdef USR_KEEP_SHAPE
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(r,r,CURC->x_max,CURC->user_width);
        grx_draw_filled_circle_with_pattern(xc,yc,r,p);
#else
        grx_user_draw_filled_ellipse_with_pattern(xc,yc,r,r,p);
#endif /* USR_KEEP_SHAPE */
}
