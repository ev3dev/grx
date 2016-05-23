/*
 * blittest.c ---- test various bitblt-s
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

#include <stdlib.h>
#include <string.h>

#include "test.h"

#define BHH     (grx_get_screen_height() / 10)
int     BWW =   83;

void drbox(GrxContext *src,int x,int y)
{
        GrxColor c1 = grx_color_info_alloc_color(0,0,255);
        GrxColor c2 = grx_color_info_alloc_color(255,0,0);
        int  xx;

        grx_clear_screen(c1);
        grx_set_current_context(src);
        grx_set_clip_box(x-10,y-10,x+BWW-1+10,y+BHH-1+10);
        grx_clear_clip_box(c2);
        grx_set_clip_box(x,y,x+BWW-1,y+BHH-1);
        grx_clear_clip_box(grx_color_info_get_black());
        grx_draw_box(x,y,x+BWW-1,y+BHH-1,grx_color_info_get_white());
        for(xx = x; xx < x+BWW; xx += 5) {
            grx_draw_line(xx,y,xx+BHH,y+BHH,grx_color_info_get_white());
            grx_draw_line(xx,y,xx-BHH,y+BHH,grx_color_info_get_white());
        }
        grx_set_current_context(NULL);
        grx_reset_clip_box();
}

void doblits(GrxContext *src,int x,int y)
{
        int xx = (grx_get_width() - BWW)/ 2;
        int yy = 2;
        int ii;

        for(ii = 0; ii < 8; ii++) {
            grx_bit_blt(xx,yy,src,x,y,x+BWW-1,y+BHH-1,GRX_COLOR_MODE_WRITE);
            xx++;
            yy += (BHH + 2);
        }
/*
  {
        GrxColor xc = grx_color_info_alloc_color(255,255,255) | GRX_COLOR_MODE_XOR;
        GrKeyRead();
        xx = (grx_get_width() - BWW)/ 2;
        yy = 2;
        for(ii = 0; ii < 8; ii++) {
            grx_draw_filled_box(xx,yy,xx+BWW-1,yy+BHH-1,xc);
            xx++;
            yy += (BHH + 2);
        }
  }
*/
}

void bltest(GrxContext *src,int x,int y)
{
        int ii;

        for(ii = 0; ii < 8; ii++) {
            drbox(src,x,y);
            doblits(src,x,y);
            GrKeyRead();
            x++;
        }
}

void blxtest(void)
{
        GrxContext memc;
        int cw = (BWW + 28) & ~7;
        int ch = BHH + 20;

        bltest(NULL,grx_get_screen_width()-BWW-8,grx_get_screen_height()-BHH);
        bltest(NULL,0,0);
        grx_context_new(cw,ch,NULL,&memc);
        bltest(&memc,cw-BWW-8,ch-BHH);
}

TESTFUNC(blittest)
{
        GrxFramedBoxColors bcolors,ocolors,icolors;
        GrxColor c,bg;
        int  x = grx_get_width();
        int  y = grx_get_height();
        int  ww = (x * 2) / 3;
        int  wh = (y * 2) / 3;
        int  ii,jj;
        int  wdt = ww / 150;
        int  bw = x / 17;
        int  bh = y / 17;
        int  bx,by;
        int  cnt;

        GrxContext *save = grx_context_new_subcontext(0,0,grx_get_max_x(),grx_get_max_y(),NULL,NULL);
        GrxContext *tile = grx_context_new(bw,bh,NULL,NULL);

        blxtest();
        GrKeyRead();

        BWW = 3;
        blxtest();
        GrKeyRead();

        bcolors.background = grx_color_info_alloc_color(160,100,30);
        bcolors.border_top = grx_color_info_alloc_color(240,150,45);
        bcolors.border_left = grx_color_info_alloc_color(240,150,45);
        bcolors.border_right = grx_color_info_alloc_color(80,50,15);
        bcolors.border_bottom = grx_color_info_alloc_color(80,50,15);

        ocolors.background = grx_color_info_alloc_color(0,120,100);
        ocolors.border_top = grx_color_info_alloc_color(0,180,150);
        ocolors.border_left = grx_color_info_alloc_color(0,180,150);
        ocolors.border_right = grx_color_info_alloc_color(0,90,60);
        ocolors.border_bottom = grx_color_info_alloc_color(0,90,60);

        icolors.background = bg = grx_color_info_alloc_color(30,30,30);
        icolors.border_bottom = grx_color_info_alloc_color(0,180,150);
        icolors.border_right = grx_color_info_alloc_color(0,180,150);
        icolors.border_left = grx_color_info_alloc_color(0,90,60);
        icolors.border_top = grx_color_info_alloc_color(0,90,60);

        c = grx_color_info_alloc_color(250,250,0);

        for(ii = 0,by = -(bh/3); ii < 19; ii++) {
            for(jj = 0,bx = -(bw/2); jj < 19; jj++) {
                grx_draw_framed_box(bx+2*wdt,by+2*wdt,bx+bw-2*wdt-1,by+bh-2*wdt-1,2*wdt,&bcolors);
                bx += bw;
            }
            by += bh;
        }

        grx_draw_framed_box(ww/4-5*wdt-1,wh/4-5*wdt-1,ww/4+5*wdt+ww+1,wh/4+5*wdt+wh+1,wdt,&ocolors);
        grx_draw_framed_box(ww/4-1,wh/4-1,ww/4+ww+1,wh/4+wh+1,wdt,&icolors);

        grx_set_clip_box(ww/4,wh/4,ww/4+ww,wh/4+wh);
        drawing(ww/4,wh/4,ww,wh,c,bg);
        GrKeyRead();

        grx_clear_screen(0);
        grx_set_current_context(save);

        bx = -(bw/2) + 15*bw;
        by = -(bh/3) + 15*bh;

        grx_draw_framed_box(bx+2*wdt,by+2*wdt,bx+bw-2*wdt-1,by+bh-2*wdt-1,2*wdt,&bcolors);

        for (cnt=0; cnt<3; cnt++) {
          for(ii = 0,by = -(bh/3); ii < 19; ii++) {
            for(jj = 0,bx = -(bw/2); jj < 19; jj++) {
                if((ii != 15) || (jj != 15)) {
                    grx_context_bit_blt(save,
                        bx,by,
                        save,
                        -(bw/2) + 15*bw,
                        -(bh/3) + 15*bh,
                        -(bw/2) + 15*bw + bw - 1,
                        -(bh/3) + 15*bh + bh - 1,
                        cnt==1 ? GRX_COLOR_MODE_XOR : GRX_COLOR_MODE_WRITE
                    );
                }
                bx += bw;
            }
            by += bh;
          }
        }

        grx_draw_framed_box(ww/4-5*wdt-1,wh/4-5*wdt-1,ww/4+5*wdt+ww+1,wh/4+5*wdt+wh+1,wdt,&ocolors);
        grx_draw_framed_box(ww/4-1,wh/4-1,ww/4+ww+1,wh/4+wh+1,wdt,&icolors);

        grx_set_clip_box(ww/4,wh/4,ww/4+ww,wh/4+wh);
        drawing(ww/4,wh/4,ww,wh,c,bg);
        GrKeyRead();


        grx_context_bit_blt(tile,
            0,0,
            save,
            -(bw/2) + 15*bw,
            -(bh/3) + 15*bh,
            -(bw/2) + 15*bw + bw - 1,
            -(bh/3) + 15*bh + bh - 1,
            GRX_COLOR_MODE_WRITE
        );
        grx_set_current_context(tile);
        grx_draw_framed_box(2*wdt,2*wdt,bw-2*wdt-1,bh-2*wdt-1,2*wdt,&bcolors);

        grx_clear_screen(0);
        grx_set_current_context(save);

        for(ii = 0,by = -(bh/3); ii < 19; ii++) {
            for(jj = 0,bx = -(bw/2); jj < 19; jj++) {
                grx_context_bit_blt(save,
                    bx,by,
                    tile,
                    0,0,
                    bw-1,bh-1,
                    GRX_COLOR_MODE_WRITE
                );
                bx += bw;
            }
            by += bh;
        }

        grx_draw_framed_box(ww/4-5*wdt-1,wh/4-5*wdt-1,ww/4+5*wdt+ww+1,wh/4+5*wdt+wh+1,wdt,&ocolors);
        grx_draw_framed_box(ww/4-1,wh/4-1,ww/4+ww+1,wh/4+wh+1,wdt,&icolors);

        grx_set_clip_box(ww/4,wh/4,ww/4+ww,wh/4+wh);
        drawing(ww/4,wh/4,ww,wh,c,bg);

        GrKeyRead();
        grx_reset_clip_box();
        grx_bit_blt(
           60,60,
           NULL,
           20,20,
           grx_get_width() - 40,
           grx_get_height() - 40,
           GRX_COLOR_MODE_WRITE
        );

        GrKeyRead();

        grx_bit_blt(
           10,10,
           NULL,
           60,60,
           grx_get_width() - 40,
           grx_get_height() - 40,
           GRX_COLOR_MODE_WRITE
        );

        GrKeyRead();

        grx_set_current_context(tile);
        grx_clear_context(0);

        grx_context_bit_blt(tile,
            0,0,
            save,
            -(bw/2),
            -(bh/3),
            -(bw/2) + 15*bw + bw - 1,
            -(bh/3) + 15*bh + bh - 1,
            GRX_COLOR_MODE_WRITE
        );

        grx_set_current_context(save);
        grx_clear_screen(0);

        for(ii = 0,by = -(bh/3); ii < 18; ii++) {
            for(jj = 0,bx = -(bw/2); jj < 18; jj++) {
                grx_context_bit_blt(save,
                    bx,by,
                    tile,
                    0,0,
                    bw-1,bh-1,
                    GRX_COLOR_MODE_WRITE
                );
                bx += bw;
            }
            by += bh;
        }

        grx_draw_framed_box(ww/4-5*wdt-1,wh/4-5*wdt-1,ww/4+5*wdt+ww+1,wh/4+5*wdt+wh+1,wdt,&ocolors);
        grx_draw_framed_box(ww/4-1,wh/4-1,ww/4+ww+1,wh/4+wh+1,wdt,&icolors);

        grx_set_clip_box(ww/4,wh/4,ww/4+ww,wh/4+wh);
        drawing(ww/4,wh/4,ww,wh,c,bg);

        GrKeyRead();

}
