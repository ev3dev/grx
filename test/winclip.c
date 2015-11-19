/**
 ** winclip.c ---- clip a drawing to various windows (contexts)
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu]
 **
 ** This is a test/demo file of the GRX graphics library.
 ** You can use GRX test/demo files as you want.
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

#include "test.h"

TESTFUNC(winclip)
{
        int  x = grx_get_size_x();
        int  y = grx_get_size_y();
        int  ww = (x / 2) - 10;
        int  wh = (y / 2) - 10;
        GrxColor c;
        GrxContext *w1 = grx_context_create_subcontext(5,5,ww+4,wh+4,NULL,NULL);
        GrxContext *w2 = grx_context_create_subcontext(15+ww,5,ww+ww+14,wh+4,NULL,NULL);
        GrxContext *w3 = grx_context_create_subcontext(5,15+wh,ww+4,wh+wh+14,NULL,NULL);
        GrxContext *w4 = grx_context_create_subcontext(15+ww,15+wh,ww+ww+14,wh+wh+14,NULL,NULL);

        grx_context_set_current(w1);
        c = grx_color_info_alloc_color(200,100,100);
        drawing(0,0,ww,wh,c,grx_color_info_get_black());
        GrBox(0,0,ww-1,wh-1,c);

        grx_context_set_current(w2);
        c = grx_color_info_alloc_color(100,200,200);
        drawing(-ww/4,ww/3,ww,wh,c,grx_color_info_get_black());
        GrBox(0,0,ww-1,wh-1,c);

        grx_context_set_current(w3);
        c = grx_color_info_alloc_color(200,200,0);
        drawing(ww/2,-wh/2,ww,wh,c,grx_color_info_get_black());
        GrBox(0,0,ww-1,wh-1,c);

        grx_context_set_current(w4);
        c = grx_color_info_alloc_color(0,100,200);
        drawing(-ww/2,-wh/2,ww*2,wh*2,c,grx_color_info_get_black());
        GrBox(0,0,ww-1,wh-1,c);

        GrKeyRead();
}

