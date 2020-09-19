/*
 * putscl.c ---- put scanline pixels
 *
 * Copyright (c) 1998 Hartmut Schirmer
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

#include "clipping.h"
#include "globals.h"
#include "libgrx.h"
#include "mouse.h"

/**
 * grx_put_scanline:
 * @x1: the starting X coordinate
 * @x2: the ending X coordinate
 * @y: the Y coordinate
 * @scan_line: (array): the data to write
 * @op: the color operation
 *
 * Writes @scan_data to the current context using the @op operation.
 *
 * Data values in @scan_data must fit #GRX_COLOR_VALUE_MASK otherwise the
 * results are implementation dependent. So you can't supply operation code
 * with the pixel data!
 */
void grx_put_scanline(int x1, int x2, int yy, const GrxColor *c, GrxColor op)
{
    int xs;
    isort(x1, x2);
    xs = x1;
    clip_hline(CURC, x1, x2, yy);
    mouse_block(CURC, x1, yy, x2, yy);
    (*FDRV->putscanline)(x1 + CURC->x_offset, yy + CURC->y_offset, x2 - x1 + 1,
        &c[x1 - xs], /* adjust pixel pointer when clipped */
        op);
    mouse_unblock();
}
