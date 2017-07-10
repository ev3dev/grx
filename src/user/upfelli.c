/*
 * upfelli.c
 *
 * Copyright (C) 1997, Michael Goffioul
 * [goffioul@emic.ucl.ac.be]
 */

#include <grx/pixmap.h>

#include "globals.h"
#include "libgrx.h"
#include "usercord.h"

/**
 * grx_user_draw_filled_ellipse_with_pixmap:
 * @xc: the X coordinate of the center of the ellipse
 * @yc: the Y coordinate of the center of the ellipse
 * @rx: the radius in the X direction
 * @ry: the radius in the Y direction
 * @p: the pixmap
 *
 * Draws a filled ellipse on the current context using the specified pixmap.
 *
 * The ellipse can only draw ellipses with its major axis parallel with either
 * the X or Y coordinate axis
 */
void grx_user_draw_filled_ellipse_with_pixmap(int xc,int yc,int rx,int ry,GrxPixmap *p)
{
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(rx,rx,CURC->x_max,CURC->user_width);
        SCALE(ry,ry,CURC->y_max,CURC->user_height);
        grx_draw_filled_ellipse_with_pixmap(xc,yc,rx,ry,p);
}
