/*
 * upfbox.c
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
 * grx_user_draw_filled_box_with_pixmap:
 * @x1: the left X coordinate
 * @y1: the top Y coordinate
 * @x2: the right X coordinate
 * @y2: the bottom Y coordinate
 * @p: the pixmap
 *
 * Draws a filled rectangle on the current context using the specified
 * coordinates and pixmap.
 */
void grx_user_draw_filled_box_with_pixmap(int x1,int y1,int x2,int y2,GrxPixmap *p)
{
        U2SX(x1,CURC);
        U2SX(x2,CURC);
        U2SY(y1,CURC);
        U2SY(y2,CURC);
        grx_draw_filled_box_with_pixmap(x1,y1,x2,y2,p);
}
