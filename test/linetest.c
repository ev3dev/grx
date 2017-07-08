/*
 * linetest.c ---- test wide and patterned lines
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

TESTFUNC(test1)
{
        GrxLineOptions o1,o2,o3,o4;
        int i;
        for(i = 0; i < 2; i++) {
            o1.color   = grx_color_get(255,0,0);
            o1.width   = 1;
            o1.n_dash_patterns = 4 * i;
            o1.dash_patterns = "\5\5\24\24";
            o2.color   = grx_color_get(255,255,0);
            o2.width   = 2;
            o2.n_dash_patterns = 6 * i;
            o2.dash_patterns = "\5\5\24\24\2\2";
            o3.color   = grx_color_get(0,255,255);
            o3.width   = 30;
            o3.n_dash_patterns = 8 * i;
            o3.dash_patterns = "\5\5\24\24\2\2\40\40";
            o4.color   = grx_color_get(255,0,255);
            o4.width   = 4;
            o4.n_dash_patterns = 6 * i;
            o4.dash_patterns = "\2\2\2\2\10\10";
            grx_clear_screen(GRX_COLOR_BLACK);
            grx_draw_line_with_options(10,10,100,100,&o1);
            grx_draw_line_with_options(10,50,100,140,&o1);
            grx_draw_line_with_options(10,90,100,180,&o1);
            grx_draw_line_with_options(110,10,200,100,&o2);
            grx_draw_line_with_options(110,50,200,140,&o2);
            grx_draw_line_with_options(110,90,200,180,&o2);
            grx_draw_line_with_options(210,10,300,100,&o3);
            grx_draw_line_with_options(210,50,300,140,&o3);
            grx_draw_line_with_options(210,90,300,180,&o3);
            grx_draw_line_with_options(20,300,600,300,&o4);
            grx_draw_line_with_options(20,320,600,340,&o4);
            grx_draw_line_with_options(20,380,600,360,&o4);
            grx_draw_line_with_options(400,100,400,300,&o4);
            grx_draw_line_with_options(420,100,440,300,&o4);
            grx_draw_line_with_options(480,100,460,300,&o4);
            grx_draw_line_with_options(600,200,500,300,&o4);
            GrKeyRead();
            grx_clear_screen(GRX_COLOR_BLACK);
            grx_draw_box_with_options(50,50,550,350,&o3);
            grx_draw_circle_with_options(300,200,50,&o2);
            GrKeyRead();
        }
}
