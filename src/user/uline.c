/*
 * uline.c
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
 */

#include <grx/draw.h>

#include "globals.h"
#include "libgrx.h"
#include "usercord.h"

/**
 * grx_user_draw_line:
 * @x1: starting X coordinate
 * @y1: starting Y coordinate
 * @x2: ending X coordinate
 * @y2: ending Y coordinate
 * @c: the color
 *
 * Draws a line on the current context from the starting coordinates to the
 * ending coordinates using the specified color.
 *
 * For horizontal and vertical lines, see grx_user_draw_hline() and
 * grx_user_draw_vline().
 */
void grx_user_draw_line(int x1,int y1,int x2,int y2,GrxColor c)
{
        U2SX(x1,CURC);
        U2SY(y1,CURC);
        U2SX(x2,CURC);
        U2SY(y2,CURC);
        grx_draw_line(x1,y1,x2,y2,c);
}
