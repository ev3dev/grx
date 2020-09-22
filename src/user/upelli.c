/*
 * upelli.c
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
 * grx_user_draw_ellipse_with_pixmap:
 * @xc: the X coordinate of the center of the ellipse
 * @yc: the Y coordinate of the center of the ellipse
 * @rx: the radius in the X direction
 * @ry: the radius in the Y direction
 * @o: the line options
 * @p: the pixmap
 *
 * Draws an ellipse on the current context using the specified line options and
 * pattern.
 *
 * The ellipse can only draw ellipses with its major axis parallel with either
 * the X or Y coordinate axis
 */
void grx_user_draw_ellipse_with_pixmap(
    int xc, int yc, int rx, int ry, const GrxLineOptions *o, GrxPixmap *p)
{
    U2SX(xc, CURC);
    U2SY(yc, CURC);
    SCALE(rx, rx, CURC->x_max, CURC->user_width);
    SCALE(ry, ry, CURC->y_max, CURC->user_height);
    grx_draw_ellipse_with_pixmap(xc, yc, rx, ry, o, p);
}
