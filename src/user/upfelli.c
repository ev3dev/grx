/*
 * upfelli.c
 *
 * Copyright (C) 1997, Michael Goffioul
 * [goffioul@emic.ucl.ac.be]
 */

#include <grx/pattern.h>

#include "globals.h"
#include "libgrx.h"
#include "usercord.h"

/**
 * grx_user_draw_filled_ellipse_with_pattern:
 * @xc: the X coordinate of the center of the ellipse
 * @yc: the Y coordinate of the center of the ellipse
 * @rx: the radius in the X direction
 * @ry: the radius in the Y direction
 * @p: the pattern
 *
 * Draws a filled ellipse on the current context using the specified pattern.
 *
 * The ellipse can only draw ellipses with its major axis parallel with either
 * the X or Y coordinate axis
 */
void grx_user_draw_filled_ellipse_with_pattern(int xc,int yc,int rx,int ry,GrxPattern *p)
{
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(rx,rx,CURC->x_max,CURC->user_width);
        SCALE(ry,ry,CURC->y_max,CURC->user_height);
        grx_draw_filled_ellipse_with_pattern(xc,yc,rx,ry,p);
}
