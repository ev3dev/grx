/*
 * ugetwin.c
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
 * grx_user_get_window:
 * @x1: (out): the left X coordinate
 * @y1: (out): the top Y coordinate
 * @x2: (out): the right X coordinate
 * @y2: (out): the bottom Y coordinate
 *
 * Gets the user window of the current context.
 */
void grx_user_get_window(int *x1, int *y1, int *x2, int *y2)
{
    *x1 = CURC->user_x_base;
    *y1 = CURC->user_y_base;
    *x2 = CURC->user_x_base + CURC->user_width;
    *y2 = CURC->user_y_base + CURC->user_height;
}
