/*
 * gengiscl.c ---- generic, VERY SLOW getindexscanline routine
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

#include <grx/context.h>

#include "grdriver.h"
#include "libgrx.h"
#include "scanline_buf.h"

/* will return an array of pixel values pv[] read from frame   */
/*    if indx == NULL: pv[i=0..w-1] = readpixel(x+i,y)         */
/*    else             pv[i=0..w-1] = readpixel(x+indx[i],y)   */

GrxColor *_GrFrDrvGenericGetIndexedScanline(
    const GrxFrame *c, int x, int y, int w, int *indx)
{
    GrxColor *pixels;
    GrxColor *p;
    GRX_ENTER();

    DBGPRINTF(DBG_DRIVER, ("x=%d, y=%d, w=%d\n", x, y, w));

    p = pixels = _grx_get_scanline_buf(w);
    _GR_readPix readpix = c->driver->readpixel;

    if (indx) {
        int i, oldx = -1;
        GrxColor col = 0;

        for (i = 0; i < w; ++i) {
            int xx = x + indx[i];

            if (oldx != xx) {
                oldx = xx;
                col = (*readpix)(c, xx, y);
            }

            *(p++) = col;
        }
    }
    else {
        for (; w > 0; --w) {
            *(p++) = (*readpix)(c, x++, y);
        }
    }

    GRX_RETURN(pixels);
}
