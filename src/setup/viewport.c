/*
 * viewport.c ---- set display start address for virtual screen
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
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

#include <grx/extents.h>

#include "arith.h"
#include "globals.h"
#include "libgrx.h"

/**
 * grx_set_viewport:
 * @x: the left X coordinate of the viewport
 * @y: the top Y coordinate of the viewport
 *
 * Sets the viewport on a virtual screen. The width and height of the viewport
 * are determined by the physical screen size (grx_get_screen_width() and
 * grx_get_screen_height()).
 *
 * Returns: %TRUE if setting the viewport was successful
 */
int grx_set_viewport(int x, int y)
{
    int res[2];
    if (!grx_is_screen_virtual())
        return FALSE;
    if (!DRVINFO->actmode.extended_info->scroll)
        return FALSE;
    x = imax(0, imin((grx_get_virtual_width() - grx_get_screen_width()), x));
    y = imax(0, imin((grx_get_virtual_height() - grx_get_screen_height()), y));
    if ((x == grx_get_viewport_x()) && (y == grx_get_viewport_y()))
        return TRUE;
    if ((*DRVINFO->actmode.extended_info->scroll)(&DRVINFO->actmode, x, y, res)) {
        DRVINFO->vposx = res[0];
        DRVINFO->vposy = res[1];
        return TRUE;
    }
    return FALSE;
}
