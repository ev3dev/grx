/*
 * majorln1.c ---- lines parallel with the X axis
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

#include "libgrx.h"
#include "clipping.h"

void grx_draw_hline(int x1,int x2,int yy,GrxColor c)
{
        clip_hline(CURC,x1,x2,yy);
        mouse_block(CURC,x1,yy,x2,yy);
        (*FDRV->drawhline)(
            x1 + CURC->x_offset,
            yy + CURC->y_offset,
            x2 - x1 + 1,
            c
        );
        mouse_unblock();
}
