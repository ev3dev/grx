/*
 * framboxn.c ---- framed (possibly shaded) box primitive (no clip)
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

#include <grx/draw.h>
#include <grx/draw_nc.h>

#include "libgrx.h"
#include "clipping.h"

void grx_fast_draw_framed_box(int x1,int y1,int x2,int y2,int wdt,const GrxFramedBoxColors *c)
{
        isort(x1,x2);
        isort(y1,y2);
        if(wdt < 0) wdt = 0;
        if(c->background != GRX_COLOR_NONE) {
            grx_fast_draw_filled_box(x1,y1,x2,y2,c->background);
        }
        while(--wdt >= 0) {
            x1--; x2++;
            y1--; y2++;
            grx_fast_draw_hline(x1,x2,y1,c->border_top);
            grx_fast_draw_vline(x1,(y1 + 1),(y2 - 1),c->border_left);
            grx_fast_draw_vline(x2,(y1 + 1),(y2 - 1),c->border_right);
            grx_fast_draw_hline(x1,x2,y2,c->border_bottom);
        }
}
