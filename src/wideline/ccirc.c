/*
 * ccirc.c ---- draw dashed and/or wide circles
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
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

#include <grx/wideline.h>

#include "libgrx.h"
#include "shapes.h"

/**
 * grx_draw_circle_with_options:
 * @xc: the X coordinate of the center of the circle
 * @yc: the Y coordinate of the center of the circle
 * @r: the radius of the circle
 * @o: the options
 *
 * Draws a circle on the current context centered at the specified coordinates
 * with the specified radius and options.
 */
void grx_draw_circle_with_options(int xc,int yc,int r,const GrxLineOptions *o)
{
        grx_draw_ellipse_with_options(xc,yc,r,r,o);
}
