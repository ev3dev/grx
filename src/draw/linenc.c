/*
 * linenc.c ---- line drawing (no clip)
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu].
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
#include "clipping.h"

/**
 * grx_fast_draw_line:
 * @x1: starting X coordinate
 * @y1: starting Y coordinate
 * @x2: ending X coordinate
 * @y2: ending Y coordinate
 * @c: the color
 *
 * Draws a line on the current context from the starting coordinates to the
 * ending coordinates using the specified color.
 *
 * For horizontal and vertical lines, see grx_fast_draw_hline() and
 * grx_fast_draw_vline().
 *
 * This function does not perform clipping. If you cannot ensure that the
 * endpoints are within the bounds of the current context, then use
 * grx_draw_line() instead.
 */
void grx_fast_draw_line(int x1,int y1,int x2,int y2,GrxColor c)
{
        (*FDRV->drawline)(
            x1 + CURC->x_offset,
            y1 + CURC->y_offset,
            x2 - x1,
            y2 - y1,
            c
        );
}
