/*
 * upixel.c
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

#include <grx/draw.h>

#include "globals.h"
#include "libgrx.h"
#include "usercord.h"

/**
 * grx_user_get_pixel_at:
 * @x: the X coordinate
 * @y: the Y coordinate
 *
 * Gets the color value of the pixel in the current context at the specified
 * coordinates.
 */
GrxColor grx_user_get_pixel_at(int x, int y)
{
    U2SX(x, CURC);
    U2SY(y, CURC);
    return grx_get_pixel_at(x, y);
}
