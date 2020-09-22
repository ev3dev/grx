/*
 * generic/bitblt.c ---- generic (slow) bitblt routine
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

#include <string.h>

#include <grx/color.h>

#include "globals.h"
#include "libgrx.h"

static void bitblt(const GrxFrame *dst, int dx, int dy, const GrxFrame *src, int x,
    int y, int w, int h, GrxColor op)
{
    GrxFrame csave;
    GrxColor skipc;
    int step;
    GRX_ENTER();
    skipc = op ^ GRX_COLOR_MODE_IMAGE;
    step = 1;
    op &= GRX_COLOR_MODE_MASK;
    memcpy(&csave, &CURC->frame, sizeof(csave));
    memcpy(&CURC->frame, dst, sizeof(CURC->frame));
    if ((dy > y) || ((dy == y) && (dx > x))) {
        x += (w - 1);
        dx += (w - 1);
        y += (h - 1);
        dy += (h - 1);
        step = (-1);
    }
    do {
        int dxx = dx, xx = x, ww = w;
        do {
            GrxColor c = readpixel(src, xx, y);
            if (c != skipc)
                drawpixel(dxx, dy, (c | op));
            dxx += step;
            xx += step;
        } while (--ww > 0);
        dy += step;
        y += step;
    } while (--h > 0);
    memcpy(&CURC->frame, &csave, sizeof(CURC->frame));
    GRX_LEAVE();
}
