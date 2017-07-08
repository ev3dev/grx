/*
 * polytest.c ---- test polygon rendering
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

#include <string.h>
#include <stdio.h>

#include "test.h"

static GrxColor *EGA;
#define black EGA[0]
#define red   EGA[12]
#define blue  EGA[1]
#define white EGA[15]

static void testpoly(int n,GrxPoint *points,int convex)
{
        grx_clear_screen(black);
        grx_draw_polygon(n,points,white);
        grx_draw_filled_polygon(n,points,(red | GRX_COLOR_MODE_XOR));
        GrKeyRead();
        if(convex || (n <= 3)) {
            grx_clear_screen(black);
            grx_draw_filled_polygon(n,points,white);
            grx_draw_filled_convex_polygon(n,points,(red | GRX_COLOR_MODE_XOR));
            GrKeyRead();
        }
}

static void speedtest(void)
{
        GrxPoint pts[4];
        int ww = grx_get_width() / 10;
        int hh = grx_get_height() / 10;
        int sx = (grx_get_width() - 2*ww) / 32;
        int sy = (grx_get_height() - 2*hh) / 32;
        int  ii,jj;
        GrxColor color;
        gint64 t1,t2,t3;
        char msg[81];

        grx_clear_screen(black);
        t1 = g_get_monotonic_time();
        pts[0].y = 0;
        pts[1].y = hh;
        pts[2].y = 2*hh;
        pts[3].y = hh;
        color = 0;
        for(ii = 0; ii < 32; ii++) {
            pts[0].x = ww;
            pts[1].x = 2*ww;
            pts[2].x = ww;
            pts[3].x = 0;
            for(jj = 0; jj < 32; jj++) {
                grx_draw_filled_polygon(4,pts, EGA[color] | GRX_COLOR_MODE_XOR);
                color = (color + 1) & 15;
                pts[0].x += sx;
                pts[1].x += sx;
                pts[2].x += sx;
                pts[3].x += sx;
            }
            pts[0].y += sy;
            pts[1].y += sy;
            pts[2].y += sy;
            pts[3].y += sy;
        }
        t2 = g_get_monotonic_time();
        pts[0].y = 0;
        pts[1].y = hh;
        pts[2].y = 2*hh;
        pts[3].y = hh;
        color = 0;
        for(ii = 0; ii < 32; ii++) {
            pts[0].x = ww;
            pts[1].x = 2*ww;
            pts[2].x = ww;
            pts[3].x = 0;
            for(jj = 0; jj < 32; jj++) {
                grx_draw_filled_convex_polygon(4,pts, EGA[color] | GRX_COLOR_MODE_XOR);
                color = (color + 1) & 15;
                pts[0].x += sx;
                pts[1].x += sx;
                pts[2].x += sx;
                pts[3].x += sx;
            }
            pts[0].y += sy;
            pts[1].y += sy;
            pts[2].y += sy;
            pts[3].y += sy;
        }
        t3 = g_get_monotonic_time();
        grx_draw_text("Times to scan 1024 polygons", 0, 0, white_text_black_bg);
        sprintf(msg, "   with 'grx_draw_filled_polygon': %.2f (s)",
               (double)(t2 - t1) / (double)1000000);
        grx_draw_text(msg, 0, 18, white_text_black_bg);
        sprintf(msg, "   with 'grx_draw_filled_convex_polygon': %.2f (s)",
               (double)(t3 - t2) / (double)1000000);
        grx_draw_text(msg, 0, 36, white_text_black_bg);
}

TESTFUNC(ptest)
{
        char buff[300];
        GrxPoint pts[300];
        int  ii,collect;
        int  convex;
        FILE *fp;

        fp = fopen("polytest.dat","r");
        if(fp == NULL) return;
        EGA = grx_color_get_ega_colors();
        ii  = collect = convex = 0;
        while(fgets(buff,299,fp) != NULL) {
            if(!collect) {
                if(strncmp(buff,"begin",5) == 0) {
                    convex  = (buff[5] == 'c');
                    collect = 1;
                    ii      = 0;
                }
                continue;
            }
            if(strncmp(buff,"end",3) == 0) {
                testpoly(ii,pts,convex);
                collect = 0;
                continue;
            }
            if(sscanf(buff,"%d %d",&pts[ii].x,&pts[ii].y) == 2) ii++;
        }
        fclose(fp);
        speedtest();
        GrKeyRead();
}
