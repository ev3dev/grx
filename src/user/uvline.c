/*
 * uvline.c
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
 * grx_user_draw_vline:
 * @x: X coordinate
 * @y1: starting Y coordinate
 * @y2: ending Y coordinate
 * @c: the color
 *
 * Draws a vertical line on the current context from the starting coordinates
 * to the ending coordinates using the specified color.
 *
 * This is more efficient that using grx_user_draw_line().
 */
void grx_user_draw_vline(int x, int y1, int y2, GrxColor c)
{
    U2SX(x, CURC);
    U2SY(y1, CURC);
    U2SY(y2, CURC);
    grx_draw_vline(x, y1, y2, c);
}
