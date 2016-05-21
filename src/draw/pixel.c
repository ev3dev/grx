/*
 * PIXEL.C ---- pixel read routine
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
#include "clipping.h"

GrxColor grx_get_pixel_at(int x,int y)
{
        GrxColor retval;
        cxclip_dot_(CURC,x,y,return(GRX_COLOR_NONE));
        mouse_block(CURC,x,y,x,y);
        retval = (*FDRV->readpixel)(
            &CURC->frame,
            x + CURC->x_offset,
            y + CURC->y_offset
        );
        mouse_unblock();
        return(retval);
}
