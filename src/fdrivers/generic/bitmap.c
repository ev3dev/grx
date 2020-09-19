/*
 * generic/bitmap.c ---- generic (=slow) font or bitmap draw routine
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

static void drawbitmap(int x, int y, int w, int h, unsigned char *bmp, int pitch,
    int start, GrxColor fg, GrxColor bg)
{
    GRX_ENTER();
    w += x;
    h += y;
    bmp += (unsigned int)start >> 3;
    start &= 7;
    do {
        unsigned char *bitp = bmp;
        unsigned char bits = *bitp;
        unsigned char mask = 0x80 >> start;
        int xx = x;
        do {
            drawpixel(xx, y, (bits & mask) ? fg : bg);
            if ((mask >>= 1) == 0)
                bits = *++bitp, mask = 0x80;
        } while (++xx != w);
        bmp += pitch;
    } while (++y != h);
    GRX_LEAVE();
}
