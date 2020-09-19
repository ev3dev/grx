/*
 * upfcirca.c
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

#include <grx/draw.h>
#include <grx/pixmap.h>

#include "globals.h"
#include "libgrx.h"
#include "usercord.h"

/**
 * grx_user_draw_filled_circle_arc_with_pixmap:
 * @xc: the X coordinate of the center of the arc
 * @yc: the Y coordinate of the center of the arc
 * @r: the radius of the arc
 * @start: the starting angle in 1/10ths of degrees
 * @end: the ending angle in 1/10ths of degrees
 * @style: the arc style
 * @p: the pixmap
 *
 * Draws a filled arc on the current context centered at the specified
 * coordinates from the starting angle to the ending angle with the specified
 * radius, arc style and pixmap.
 */
void grx_user_draw_filled_circle_arc_with_pixmap(
    int xc, int yc, int r, int start, int end, GrxArcStyle style, GrxPixmap *p)
{
#ifdef USR_KEEP_SHAPE
    U2SX(xc, CURC);
    U2SY(yc, CURC);
    SCALE(r, r, CURC->x_max, CURC->user_width);
    grx_draw_filled_circle_arc_with_pixmap(xc, yc, r, start, end, style, p);
#else
    grx_user_draw_filled_ellipse_arc_with_pixmap(xc, yc, r, r, start, end, style, p);
#endif /* USR_KEEP_SHAPE */
}
