/*
 * cliptest.c ---- test clipping
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
 *
 * This is a test/demo file of the GRX graphics library.
 * You can use GRX test/demo files as you want.
 *
 * The GRX graphics library is free software; you can redistribute it
 * and/or modify it under some conditions; see the "copying.grx" file
 * for details.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "test.h"
#include "rand.h"

TESTFUNC(cliptest)
{
        long delay;
        int x = grx_get_width();
        int y = grx_get_height();
        int ww = (x * 2) / 3;
        int wh = (y * 2) / 3;
        GrxColor c;

        c = grx_color_get(200,100,100);
        grx_draw_box(ww/4-1,wh/4-1,ww/4+ww+1,wh/4+wh+1,GRX_COLOR_WHITE);
        grx_set_clip_box(ww/4,wh/4,ww/4+ww,wh/4+wh);

        drawing(0,0,ww,wh,c,GRX_COLOR_BLACK);
        GrKeyRead();

        while(!GrKeyPressed()) {
            grx_draw_filled_box(0,0,x,y,GRX_COLOR_BLACK);
            drawing(-(RND()%(2*ww))+ww/2,
                -(RND()%(2*wh))+wh/2,
                RND()%(3*ww)+10,
                RND()%(3*wh)+10,
                c,
                GRX_COLOR_NONE
            );
            for(delay = 200000L; delay > 0L; delay--);
        }
        GrKeyRead();
}
