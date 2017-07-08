/*
 * colorops.c ---- test WRITE, XOR, OR, and AND draw modes
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

TESTFUNC(colorops)
{
        GrxFramedBoxColors bcolors,ocolors,icolors;
        GrxColor bg,c;
        int x = grx_get_width();
        int y = grx_get_height();
        int ww = (x * 2) / 3;
        int wh = (y * 2) / 3;
        int ii,jj;
        int wdt = ww / 150;
        int bw = x / 16;
        int bh = y / 16;
        int bx,by;

        /* This won't work very well under X11 in pseudocolor
         * mode (256 colors or less) if not using a private
         * color map. The missing colors break RGB mode      */
        grx_color_info_set_palette_type_rgb();

        bcolors.background = grx_color_get(160,100,30);
        bcolors.border_top = grx_color_get(240,150,45);
        bcolors.border_left = grx_color_get(240,150,45);
        bcolors.border_right = grx_color_get(80,50,15);
        bcolors.border_bottom = grx_color_get(80,50,15);

        ocolors.background = grx_color_get(0,120,100);
        ocolors.border_top = grx_color_get(0,180,150);
        ocolors.border_left = grx_color_get(0,180,150);
        ocolors.border_right = grx_color_get(0,90,60);
        ocolors.border_bottom = grx_color_get(0,90,60);

        icolors.background = grx_color_get(30,30,30);
        icolors.border_bottom = grx_color_get(0,180,150);
        icolors.border_right = grx_color_get(0,180,150);
        icolors.border_left = grx_color_get(0,90,60);
        icolors.border_top = grx_color_get(0,90,60);

        c  = grx_color_get(250,250,0);
        bg = GRX_COLOR_NONE;

        for(ii = 0,by = -(bh / 3); ii < 17; ii++) {
            for(jj = 0,bx = (-bw / 2); jj < 17; jj++) {
                grx_draw_framed_box(bx+2*wdt,by+2*wdt,bx+bw-2*wdt-1,by+bh-2*wdt-1,2*wdt,&bcolors);
                bx += bw;
            }
            by += bh;
        }

        grx_draw_framed_box(ww/4-5*wdt-1,wh/4-5*wdt-1,ww/4+5*wdt+ww+1,wh/4+5*wdt+wh+1,wdt,&ocolors);
        grx_draw_framed_box(ww/4-1,wh/4-1,ww/4+ww+1,wh/4+wh+1,wdt,&icolors);

        grx_set_clip_box(ww/4,wh/4,ww/4+ww,wh/4+wh);

        drawing(ww/4,wh/4,ww,wh,c,bg);
        while(!GrKeyPressed()) {
            drawing(ww/4+(RND()%100),
                wh/4+(RND()%100),
                ww,
                wh,
                ((RND() / 16) & (grx_color_info_n_colors() - 1)),
                bg
            );
        }
        GrKeyRead();
        grx_draw_framed_box(ww/4-1,wh/4-1,ww/4+ww+1,wh/4+wh+1,wdt,&icolors);
        drawing(ww/4,wh/4,ww,wh,c,bg);
        while(!GrKeyPressed()) {
            drawing(ww/4+(RND()%100),
                wh/4+(RND()%100),
                ww,
                wh,
                ((RND() / 16) & (grx_color_info_n_colors() - 1)) | GRX_COLOR_MODE_XOR,
                bg
            );
        }
        GrKeyRead();
        grx_draw_framed_box(ww/4-1,wh/4-1,ww/4+ww+1,wh/4+wh+1,wdt,&icolors);
        drawing(ww/4,wh/4,ww,wh,c,bg);
        while(!GrKeyPressed()) {
            drawing(ww/4+(RND()%100),
                wh/4+(RND()%100),
                ww,
                wh,
                ((RND() / 16) & (grx_color_info_n_colors() - 1)) | GRX_COLOR_MODE_OR,
                bg
            );
        }
        GrKeyRead();
        grx_draw_framed_box(ww/4-1,wh/4-1,ww/4+ww+1,wh/4+wh+1,wdt,&icolors);
        drawing(ww/4,wh/4,ww,wh,c,bg);
        while(!GrKeyPressed()) {
            drawing(ww/4+(RND()%100),
                wh/4+(RND()%100),
                ww,
                wh,
                ((RND() / 16) & (grx_color_info_n_colors() - 1)) | GRX_COLOR_MODE_AND,
                bg
            );
        }
        GrKeyRead();
}
