/*
 * curstest.c ---- test cursors
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
 *
 */

#include "test.h"

char p16d[] = {
    0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,
    1,2,1,0,0,0,0,0,0,0,0,1,2,2,1,0,
    1,2,2,1,0,0,0,0,0,0,1,2,0,0,2,1,
    1,2,2,2,1,0,0,0,0,0,1,2,0,0,2,1,
    1,2,2,2,2,1,0,0,0,0,0,1,2,2,1,0,
    1,2,2,2,2,2,1,0,0,0,0,0,1,1,0,0,
    1,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,
    1,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,
    1,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,
    1,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0,
    1,2,2,2,2,2,2,2,2,2,2,1,0,0,0,0,
    1,2,2,2,2,1,1,1,1,1,1,0,0,0,0,0,
    1,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,
    1,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,
    1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

TESTFUNC(cursortest)
{
        GrxColor bgc = grx_color_info_alloc_color(0,0,128);
        GrxColor fgc = grx_color_info_alloc_color(255,255,0);
        GrxColor msc[3];
        GrCursor *cur;
        int x,y;

        msc[0] = 2;
        msc[1] = grx_color_info_get_white();
        msc[2] = grx_color_info_alloc_color(255,0,0);
        cur = GrBuildCursor(p16d,16,16,16,1,1,msc);
        x = grx_get_screen_x() / 2;
        y = grx_get_screen_y() / 2;
        GrMoveCursor(cur,x,y);
        grx_clear_screen(bgc);
        grx_color_info_set_color((grx_color_info_n_colors() - 1),255,255,255);
        drawing(0,0,grx_get_width(),grx_get_height(),fgc,GRX_COLOR_NONE);
        grx_draw_filled_box(0,0,320,120,grx_color_info_alloc_color(0,255,255));
        grx_draw_text_xy( 10,90,"ANDmask",grx_color_info_get_black(),GRX_COLOR_NONE);
        grx_draw_text_xy( 90,90,"ORmask", grx_color_info_get_black(),GRX_COLOR_NONE);
        grx_draw_text_xy(170,90,"Save",   grx_color_info_get_black(),GRX_COLOR_NONE);
        grx_draw_text_xy(250,90,"Work",   grx_color_info_get_black(),GRX_COLOR_NONE);
        GrDisplayCursor(cur);
        for( ; ; ) {
            grx_bit_blt(
                10,10,
                &cur->work,cur->xwork/2,0,cur->xwork/2+cur->xsize-1,cur->ysize-1,
                GRX_COLOR_MODE_WRITE
            );
            grx_bit_blt(
                90,10,
                &cur->work,0,0,cur->xsize-1,cur->ysize-1,
                GRX_COLOR_MODE_WRITE
            );
            grx_bit_blt(
                170,10,
                &cur->work,0,cur->ysize,cur->xwork-1,cur->ysize+cur->ywork-1,
                GRX_COLOR_MODE_WRITE
            );
            grx_bit_blt(
                250,10,
                &cur->work,0,cur->ysize+cur->ywork,cur->xwork-1,cur->ysize+2*cur->ywork-1,
                GRX_COLOR_MODE_WRITE
            );
            grx_draw_text_xy(0,grx_get_max_y()-20,"Type u d l r U D L R or q to quit",grx_color_info_get_white(),GRX_COLOR_NONE);
            switch(GrKeyRead()) {
                case 'u': y--; break;
                case 'd': y++; break;
                case 'l': x--; break;
                case 'r': x++; break;
                case 'U': y -= 10; break;
                case 'D': y += 10; break;
                case 'L': x -= 10; break;
                case 'R': x += 10; break;
                case 'q': return;
                default:  continue;
            }
            if(x < 0) x = 0;
            if(x > grx_get_screen_x()) x = grx_get_screen_x();
            if(y < 100) y = 100;
            if(y > grx_get_screen_y()) y = grx_get_screen_y();
            GrMoveCursor(cur,x,y);
        }
}
