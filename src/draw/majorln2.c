/**
 ** majorln2.c ---- lines parallel with the Y axis
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu].
 **
 ** This file is part of the GRX graphics library.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 **/

#include "libgrx.h"
#include "clipping.h"

void grx_draw_vline(int xx,int y1,int y2,GrxColor c)
{
        clip_vline(CURC,xx,y1,y2);
        mouse_block(CURC,xx,y1,xx,y2);
        (*FDRV->drawvline)(
            xx + CURC->x_offset,
            y1 + CURC->y_offset,
            y2 - y1 + 1,
            c
        );
        mouse_unblock();
}
