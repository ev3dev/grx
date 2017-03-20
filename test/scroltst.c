/*
 * scroltst.c ---- test virtual screen set/scroll
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

TESTFUNC(scrolltest)
{
        GrxFont *font = grx_text_options_get_font(white_text);
        int  wdt = grx_get_screen_width();
        int  hgt = grx_get_screen_height();
        GrxColor nc  = grx_color_info_n_colors();
        int  txh = grx_font_get_height(font) + 2;
        for( ; ; ) {
            char buff[100];
            char *l1 = "Screen resolution: %dx%d";
            char *l2 = "Virtual resolution: %dx%d";
            char *l3 = "Current screen start: x=%-4d y=%-4d";
            char *l4 = "Commands: q -- exit program,";
            char *l5 = "w W h H -- shrink/GROW screen width or height,";
            char *l6 = "x X y Y -- decrease/INCREASE screen start position";
            GrxColor bgc = grx_color_alloc(0,0,128);
            GrxColor fgc = grx_color_alloc(200,200,0);
            GrxColor txc = grx_color_alloc(255,0,255);
            int vw = grx_get_virtual_width();
            int vh = grx_get_virtual_height();
            int vx = grx_get_viewport_x();
            int vy = grx_get_viewport_y();
            int x  = (vw / 3) - (strlen(l6) * grx_font_get_width(font) / 2);
            int y  = (vh / 3) - (3 * txh);
            GrxTextOptions *text_opt = grx_text_options_new_full(font, txc, bgc,
                GRX_TEXT_HALIGN_LEFT, GRX_TEXT_VALIGN_TOP);
            grx_clear_screen(bgc);
            drawing(0,0,vw,vh,fgc,bgc);
            sprintf(buff,l1,wdt,hgt); grx_draw_text(buff,x,y,text_opt); y += txh;
            sprintf(buff,l2,vw, vh ); grx_draw_text(buff,x,y,text_opt); y += txh;
            for( ; ; grx_set_viewport(vx,vy)) {
                int yy = y;
                vx = grx_get_viewport_x();
                vy = grx_get_viewport_y();
                sprintf(buff,l3,vx,vy); grx_draw_text(buff,x,yy,text_opt); yy += txh;
                grx_draw_text(l4,x,yy,text_opt); yy += txh;
                grx_draw_text(l5,x,yy,text_opt); yy += txh;
                grx_draw_text(l6,x,yy,text_opt); yy += txh;
                switch(GrKeyRead()) {
                    case 'w': vw -= 8; break;
                    case 'W': vw += 8; break;
                    case 'h': vh -= 8; break;
                    case 'H': vh += 8; break;
                    case 'x': vx--; continue;
                    case 'X': vx++; continue;
                    case 'y': vy--; continue;
                    case 'Y': vy++; continue;
                    // FIXME: return does not unref text options
                    case 'q': return;
                    case 'Q': return;
                    default:  continue;
                }
                grx_set_mode(GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM,NULL,wdt,hgt,nc,vw,vh);
                break;
            }
            grx_text_options_unref(text_opt);
        }
}
