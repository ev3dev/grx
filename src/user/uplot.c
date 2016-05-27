/*
 * uplot.c
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
 * grx_user_draw_pixel:
 * @x: the X coordinate
 * @y: the Y coordinate
 * @c: the color (can include #GrxColorMode)
 *
 * Draw a single pixel on the current context at the specified user coordinates.
 */
void grx_user_draw_pixel(int x,int y,GrxColor c)
{
        U2SX(x,CURC);
        U2SY(y,CURC);
        grx_draw_pixel(x,y,c);
}
