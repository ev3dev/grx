/*
 * majorln4.c ---- lines parallel with the Y axis (no clip)
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
 *
 */

#include "globals.h"
#include "libgrx.h"
#include "clipping.h"

/**
 * grx_fast_draw_vline:
 * @x: X coordinate
 * @y1: starting Y coordinate
 * @y2: ending Y coordinate
 * @c: the color
 *
 * Draws a vertical line on the current context from the starting coordinates
 * to the ending coordinates using the specified color.
 *
 * This is more efficient that using grx_fast_draw_line().
 *
 * This function does not perform clipping. If you cannot ensure that the
 * endpoints are within the bounds of the current context, then use
 * grx_draw_hline() instead.
 */
void grx_fast_draw_vline(int xx,int y1,int y2,GrxColor c)
{
        isort(y1,y2);
        (*FDRV->drawvline)(
            xx + CURC->x_offset,
            y1 + CURC->y_offset,
            y2 - y1 + 1,
            c
        );
}
