/*
 * generic/pattern.c ---- generic (=slow) patterned scan line fill routine
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

#include <grx/color.h>

#include "libgrx.h"

static void drawpattern(
    int x, int y, int w, unsigned char patt, GrxColor fg, GrxColor bg)
{
    GR_int8u mask;
    GRX_ENTER();
    mask = 0x80;
    w += x;
    do {
        drawpixel(x, y, (patt & mask) ? fg : bg);
        if ((mask >>= 1) == 0)
            mask = 0x80;
    } while (++x != w);
    GRX_LEAVE();
}
