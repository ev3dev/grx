/*
 * usetwin.c
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

#include "globals.h"
#include "libgrx.h"

/**
 * grx_user_set_window:
 * @x1: the left X coordinate
 * @y1: the top Y coordinate
 * @x2: the right X coordinate
 * @y2: the bottom Y coordinate
 *
 * Specifies the user window.
 *
 * A call to this function it in fact specifies the virtual coordinate limits
 * which will be mapped onto the current context regardless of the size of the
 * context. For example, the call:
 * |[<!-- language="C" -->
 * GrSetUserWindow(0,0,11999,8999);
 * ]|
 * tells the library that the program will perform its drawing operations in a
 * coordinate system X:0...11999 (width = 12000) and Y:0...8999 (height = 9000).
 * This coordinate range will be mapped onto the total area of the current
 * context. The virtual coordinate system can also be shifted. For example:
 * |[<!-- language="C" -->
 * GrSetUserWindow(5000,2000,16999,10999);
 * ]|
 * The user coordinates can even be used to turn the usual left-handed
 * coordinate system (0:0 corresponds to the upper left corner) to a right
 * handed one (0:0 corresponds to the bottom left corner) by calling:
 * |[<!-- language="C" -->
 * GrSetUserWindow(0,8999,11999,0);
 * ]|
 */
void grx_user_set_window(int x1, int y1, int x2, int y2)
{
    CURC->user_x_base = x1;
    CURC->user_y_base = y1;
    CURC->user_width = x2 - x1;
    CURC->user_height = y2 - y1;
}
