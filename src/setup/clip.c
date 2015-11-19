/**
 ** clip.c ---- clip box setting utilities
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu]
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

void grx_set_clip_box(int x1,int y1,int x2,int y2)
{
        cxclip_box(CURC,x1,y1,x2,y2);
        CURC->x_clip_low = x1;
        CURC->y_clip_low = y1;
        CURC->x_clip_high = x2;
        CURC->y_clip_high = y2;
}

void grx_context_set_clip_box(GrxContext *c,int x1,int y1,int x2,int y2)
{
        cxclip_box(c,x1,y1,x2,y2);
        c->x_clip_low = x1;
        c->y_clip_low = y1;
        c->x_clip_high = x2;
        c->y_clip_high = y2;
}

void grx_reset_clip_box(void)
{
        CURC->x_clip_low = 0;
        CURC->y_clip_low = 0;
        CURC->x_clip_high = CURC->x_max;
        CURC->y_clip_high = CURC->y_max;
}

void grx_context_reset_clip_box(GrxContext *c)
{
        c->x_clip_low = 0;
        c->y_clip_low = 0;
        c->x_clip_high = c->x_max;
        c->y_clip_high = c->y_max;
}

