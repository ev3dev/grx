/*
 * uframbox.c
 *
 * Copyright (C) 1992, Csaba Biegl
 *   820 Stirrup Dr, Nashville, TN, 37221
 *   csaba@vuse.vanderbilt.edu
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

#include "globals.h"
#include "libgrx.h"
#include "arith.h"
#include "usercord.h"

/**
 * grx_user_draw_framed_box:
 * @x1: the left X coordinate
 * @y1: the top Y coordinate
 * @x2: the right X coordinate
 * @y2: the bottom Y coordinate
 * @width: the width of the frame
 * @c: the colors
 *
 * Draws a framed rectangle on the current context using the specified
 * coordinates and colors. The coordinates specify the interior box. The border
 * extends outside of the box using the given @width.
 */
void grx_user_draw_framed_box(int x1,int y1,int x2,int y2,int wdt,GrxFramedBoxColors *c)
{
        int w1,w2;

        U2SX(x1,CURC);
        U2SY(y1,CURC);
        U2SX(x2,CURC);
        U2SY(y2,CURC);
        SCALE(w1,wdt,CURC->x_max,CURC->user_width);
        SCALE(w2,wdt,CURC->y_max,CURC->user_height);
        wdt = (iabs((int)w1) + iabs((int)w2)) >> 1;
        if(wdt == 0)
            grx_draw_filled_box(x1,y1,x2,y2,c->background);
        else grx_draw_framed_box(x1,y1,x2,y2,wdt,c);
}
