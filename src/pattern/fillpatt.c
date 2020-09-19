/*
 * fillpatt.c ---- draw a pattern filled horizontal line
 *
 * Copyright (C) 1997, Michael Goffioul
 * [e-mail : goffioul@emic.ucl.ac.be]
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
 * Contributions by: (See "doc/credits.doc" for details)
 * Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 */

#include "arith.h"
#include "globals.h"
#include "libgrx.h"
#include "shapes.h"

void _GrFillPatternExt(int x, int y, int sx, int sy, int width, GrxPixmap *p)
{
    GRX_ENTER();
    void (*bltfun)(GrxFrame *, int, int, GrxFrame *, int, int, int, int, GrxColor);
    int pattwdt = p->width;
    int xdest = x;
    int ydest = y;
    int ypatt = (y - sy) % p->height;
    int xpatt = (x - sx) % pattwdt;
    int cpysize = pattwdt - xpatt;
    GrxColor optype = p->mode;

    if (CURC->gc_is_on_screen) {
        bltfun = CURC->gc_driver->bltr2v;
    }
    else {
        bltfun = CURC->gc_driver->bitblt;
    }
    while (width > 0) {
        if (cpysize > width) {
            cpysize = width;
        }
        (*bltfun)(
            &CURC->frame, xdest, ydest, &p->source, xpatt, ypatt, cpysize, 1, optype);
        width -= cpysize;
        xpatt = 0;
        xdest += cpysize;
        cpysize = pattwdt;
    }
    GRX_LEAVE();
}

void _GrFillPattern(int x, int y, int width, GrxPixmap *p)
{
    GRX_ENTER();
    _GrFillPatternExt(x, y, 0, 0, width, p);
    GRX_LEAVE();
}

void _GrFillPatternedScanLine(int x, int y, int w, GrFillArg arg)
{
    GRX_ENTER();
    _GrFillPatternExt(x, y, 0, 0, w, arg.p);
    GRX_LEAVE();
}
