/*
 * fonttest.c ---- test text drawing
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

#include <string.h>
#include "test.h"

int  cx;
int  cy;
GrxColor c1;
GrxColor c2;
GrxColor c3;
GrxColor c4;

char test_text[] = {
    "QUICK BROWN FOX JUMPS OVER THE LAZY DOG, "
    "quick brown fox jumps over the lazy dog !@#$%^&*()1234567890"
};

void displayfont(GrxFont *font,char *text,int len)
{
        GrxTextOptions opt;
        int ww,hh;
        int bx,by;
        int bw,bh;

        memset(&opt,0,sizeof(opt));
        opt.txo_font   = font;
        opt.txo_xalign = GRX_TEXT_ALIGN_LEFT;
        opt.txo_yalign = GRX_TEXT_VALIGN_TOP;
        grx_draw_filled_box(0,0,grx_get_width(),grx_get_height(),grx_color_info_get_black());
        opt.txo_direct    = GRX_TEXT_DIRECTION_RIGHT;
        opt.txo_fgcolor.v = grx_color_info_get_black();
        opt.txo_bgcolor.v = c1;
        ww = grx_text_option_get_string_width(&opt,text,len);
        hh = grx_text_option_get_string_height(&opt,text,len);
        bw = ww+2*hh;
        bh = ww;
        bx = cx - bw/2;
        by = cy - bh/2;
        grx_draw_string_with_text_options(text,len,bx+hh,by,&opt);
        opt.txo_direct    = GRX_TEXT_DIRECTION_DOWN;
        opt.txo_bgcolor.v = c2;
        grx_draw_string_with_text_options(text,len,bx+bw-hh,by,&opt);
        opt.txo_direct    = GRX_TEXT_DIRECTION_LEFT;
        opt.txo_bgcolor.v = c3;
        grx_draw_string_with_text_options(text,len,bx+bw-ww-hh,by+bh-hh,&opt);
        opt.txo_direct    = GRX_TEXT_DIRECTION_UP;
        opt.txo_bgcolor.v = c4;
        grx_draw_string_with_text_options(text,len,bx,by+bh-ww,&opt);
        GrKeyRead();
        grx_clear_clip_box(grx_color_info_get_black());
        opt.txo_direct    = GRX_TEXT_DIRECTION_RIGHT;
        opt.txo_fgcolor.v = c1;
        opt.txo_bgcolor.v = grx_color_info_get_black();
        bx = grx_get_width() / 16;
        by = grx_get_height() / 16;
        bx = (bx + 7) & ~7;
        while(by < grx_get_height()) {
            grx_draw_string_with_text_options(test_text,strlen(test_text),bx,by,&opt);
            opt.txo_fgcolor.v ^= GRX_UNDERLINE_TEXT;
            by += hh;
        }
        GrKeyRead();
}

TESTFUNC(fonttest)
{
        GrxFont *f;
        int i;
        char buff[100];
        cx = grx_get_width() / 2;
        cy = grx_get_height() / 2;
        c1 = grx_color_info_alloc_color(100,200,100);
        c2 = grx_color_info_alloc_color(150,150,100);
        c3 = grx_color_info_alloc_color(100,100,200);
        c4 = grx_color_info_alloc_color(100,180,180);
        grx_draw_box(grx_get_width()/16 - 2,
            grx_get_height()/16 - 2,
            grx_get_width() - grx_get_width()/16 + 1,
            grx_get_height() - grx_get_height()/16 + 1,
            grx_color_info_alloc_color(250,100,100)
        );
        grx_set_clip_box(grx_get_width()/16,
            grx_get_height()/16,
            grx_get_width() - grx_get_width()/16 - 1,
            grx_get_height() - grx_get_height()/16 - 1
        );
        strcpy(buff,"Default GRX font");
        displayfont(&grx_font_default,buff,strlen(buff));
        strcpy(buff,"Default font scaled to 6x10");
        displayfont(
            grx_font_build_converted(
                &grx_font_default,
                (GRX_FONT_CONV_FLAG_SKIP_CHARS | GRX_FONT_CONV_FLAG_RESIZE),
                6,
                10,
                ' ',
                'z'
            ),
            buff,
            strlen(buff)
        );
        strcpy(buff,"Default font scaled to 12x24");
        displayfont(
            grx_font_build_converted(
                &grx_font_default,
                (GRX_FONT_CONV_FLAG_SKIP_CHARS | GRX_FONT_CONV_FLAG_RESIZE),
                12,
                24,
                ' ',
                'z'
            ),
            buff,
            strlen(buff)
        );
        strcpy(buff,"Default font scaled to 18x36");
        displayfont(
            grx_font_build_converted(
                &grx_font_default,
                (GRX_FONT_CONV_FLAG_SKIP_CHARS | GRX_FONT_CONV_FLAG_RESIZE),
                18,
                36,
                ' ',
                'z'
            ),
            buff,
            strlen(buff)
        );
        strcpy(buff,"Default font scaled to 10x20 proportional");
        displayfont(
            grx_font_build_converted(
                &grx_font_default,
                (GRX_FONT_CONV_FLAG_SKIP_CHARS | GRX_FONT_CONV_FLAG_RESIZE | GRX_FONT_CONV_FLAG_PROPORTION),
                10,
                20,
                ' ',
                'z'
            ),
            buff,
            strlen(buff)
        );
        strcpy(buff,"Default font scaled to 10x20 bold");
        displayfont(
            grx_font_build_converted(
                &grx_font_default,
                (GRX_FONT_CONV_FLAG_SKIP_CHARS | GRX_FONT_CONV_FLAG_RESIZE | GRX_FONT_CONV_FLAG_BOLDIFY),
                10,
                20,
                ' ',
                'z'
            ),
            buff,
            strlen(buff)
        );
        strcpy(buff,"Default font scaled to 10x20 italic");
        displayfont(
            grx_font_build_converted(
                &grx_font_default,
                (GRX_FONT_CONV_FLAG_SKIP_CHARS | GRX_FONT_CONV_FLAG_RESIZE | GRX_FONT_CONV_FLAG_ITALICIZE),
                10,
                20,
                ' ',
                'z'
            ),
            buff,
            strlen(buff)
        );
        for(i = 0; i < Argc; i++) {
            f = grx_font_load(Argv[i]);
            if(f) {
                sprintf(buff,"This is font %s",Argv[i]);
                displayfont(f,buff,strlen(buff));
            }
        }
}
