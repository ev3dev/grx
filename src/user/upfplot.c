/*
 * upfplot.c
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

#include <grx/pixmap.h>

#include "globals.h"
#include "libgrx.h"
#include "usercord.h"

/**
 * grx_user_draw_pixel_with_pixmap:
 * @x: the X coordinate
 * @y: the Y coordinate
 * @p: the pixmap
 *
 * Draw a single pixel on the current context at the specified coordinates.
 */
void grx_user_draw_pixel_with_pixmap(int x,int y,GrxPixmap *p)
{
        U2SX(x,CURC);
        U2SY(y,CURC);
        grx_draw_pixel_with_pixmap(x,y,p);
}
