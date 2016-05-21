/*
 * plot.c ---- pixel draw routines
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
#include "mouse.h"
#include "libgrx.h"
#include "arith.h"
#include "clipping.h"

void grx_draw_pixel(int x,int y,GrxColor c)
{
        clip_dot(CURC,x,y);
        mouse_block(CURC,x,y,x,y);
        (*FDRV->drawpixel)(
            x + CURC->x_offset,
            y + CURC->y_offset,
            c
        );
        mouse_unblock();
}
