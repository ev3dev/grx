/*
 * ptcirca.c
 *
 * Copyright (C) 1997, Michael Goffioul
 * [e-mail : goffioul@emic.ucl.ac.be]
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
 *
 *  Copyright (C) 1992, Csaba Biegl
 *    820 Stirrup Dr, Nashville, TN, 37221
 *    csaba@vuse.vanderbilt.edu
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

#include "libgrx.h"

/**
 * grx_draw_circle_arc_with_pattern:
 * @xc: the X coordinate of the center of the arc
 * @yc: the Y coordinate of the center of the arc
 * @r: the radius of the arc
 * @start: the starting angle in 1/10ths of degrees
 * @end: the ending angle in 1/10ths of degrees
 * @style: the arc style
 * @line_pattern: the line pattern
 *
 * Draws an arc on the current context centered at the specified coordinates
 * from the starting angle to the ending angle with the specified radius,
 * arc style and line pattern.
 */
void grx_draw_circle_arc_with_pattern(int xc,int yc,int r,int start,int end,GrxArcStyle style,GrxLinePattern *lp)
{
        grx_draw_ellipse_arc_with_pattern(xc,yc,r,r,start,end,style,lp);
}
