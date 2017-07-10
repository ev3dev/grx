/*
 * upellia.c
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
 */

#include <grx/draw.h>
#include <grx/pixmap.h>

#include "globals.h"
#include "libgrx.h"
#include "usercord.h"

/**
 * grx_user_draw_ellipse_arc_with_pixmap:
 * @xc: the X coordinate of the center of the arc
 * @yc: the Y coordinate of the center of the arc
 * @rx: the radius in the X direction
 * @ry: the radius in the Y direction
 * @start: the starting angle in 1/10ths of degrees
 * @end: the ending angle in 1/10ths of degrees
 * @style: the arc style
 * @o: the line options
 * @p: the pixmap
 *
 * Draws an arc on the current context centered at the specified coordinates
 * from the starting angle to the ending angle with the specified radii,
 * arc style and line options and pixmap.
 */
void grx_user_draw_ellipse_arc_with_pixmap (int xc, int yc, int rx, int ry, int start, int end, GrxArcStyle style, GrxLineOptions *o, GrxPixmap *p)
{
    U2SX (xc, CURC);
    U2SY (yc, CURC);
    SCALE (rx, rx, CURC->x_max, CURC->user_width);
    SCALE (ry, ry, CURC->y_max, CURC->user_height);
    grx_draw_ellipse_arc_with_pixmap (xc, yc, rx, ry, start, end, style, o, p);
}
