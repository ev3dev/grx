/*
 * pcirctst.c ---- test custom circle and ellipse rendering
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
#include <math.h>

static int stop = 0;

static int widths[] = { 1, 2, 5, 10, 20, 50, 0 };

static GrxLineOptions Solid = { 0, 1, 0, NULL };  /* normal solid */

static GrxLineOptions *Patterns[] = {
  &Solid, NULL
};

void drawellip(int xc,int yc,int rx,int ry,GrxColor c1,GrxColor c2,GrxColor c3)
{
        double ddx = (double)rx;
        double ddy = (double)ry;
        double R2 = ddx*ddx*ddy*ddy;
        double SQ;
        int x1,x2,y1,y2;
        int dx,dy;
        int *wdt, idx;
        GrxLineOptions *l;

        for (idx = 0, l = *Patterns; l != NULL; l = Patterns[++idx])
            for (wdt=widths; *wdt != 0; ++wdt) {
                grx_clear_screen(grx_color_info_get_black());

                grx_draw_filled_box(xc-rx,yc-ry,xc+rx,yc+ry,c1);
                dx = rx;
                dy = 0;
                grx_draw_pixel(xc-dx,yc,c3);
                grx_draw_pixel(xc+dx,yc,c3);
                while(++dy <= ry) {
                    SQ = R2 - (double)dy * (double)dy * ddx * ddx;
                    dx = (int)(sqrt(SQ)/ddy + 0.5);
                    x1 = xc - dx;
                    x2 = xc + dx;
                    y1 = yc - dy;
                    y2 = yc + dy;
                    grx_draw_pixel(x1,y1,c3);
                    grx_draw_pixel(x2,y1,c3);
                    grx_draw_pixel(x1,y2,c3);
                    grx_draw_pixel(x2,y2,c3);
                }

                l->color = c2;
                l->width = *wdt;
                grx_draw_ellipse_with_options(xc,yc,rx,ry,l);
                if(GrKeyRead() == 'q') {
                  stop = 1;
                  return;
                }
            }
}

TESTFUNC(circtest)
{
        int  xc,yc;
        int  xr,yr;
        GrxColor c1,c2,c3;

        c1 = grx_color_info_alloc_color(64,64,255);
        c2 = grx_color_info_alloc_color(255,255,64);
        c3 = grx_color_info_alloc_color(255,64,64);
        xc = grx_get_width() / 2;
        yc = grx_get_height() / 2;
        xr = 1;
        yr = 1;
        while(!stop && ((xr < 1000) || (yr < 1000))) {
            drawellip(xc,yc,xr,yr,c1,c2,c3);
            xr += xr/4+1;
            yr += yr/4+1;
        }
        xr = 4;
        yr = 1;
        while(!stop && ((xr < 1000) || (yr < 1000))) {
            drawellip(xc,yc,xr,yr,c1,c2,c3);
            yr += yr/4+1;
            xr = yr * 4;
        }
        xr = 1;
        yr = 4;
        while(!stop && ((xr < 1000) || (yr < 1000))) {
            drawellip(xc,yc,xr,yr,c1,c2,c3);
            xr += xr/4+1;
            yr = xr * 4;
        }
}
