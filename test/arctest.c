/*
 * arctest.c ---- test arc outline and filled arc drawing
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
#include "test.h"

TESTFUNC(arctest)
{
        char buff[300];
        int  xc,yc,rx,ry,start,end;
        FILE *fp;
        GrxColor red   = grx_color_get(255,0,0);
        GrxColor green = grx_color_get(0,255,0);
        GrxColor blue  = grx_color_get(0,0,255);

        fp = fopen("arctest.dat","r");
        if(fp == NULL) return;
        while(fgets(buff,299,fp) != NULL) {
            int len = strlen(buff);
            while(--len >= 0) {
                if(buff[len] == '\n') { buff[len] = '\0'; continue; }
                if(buff[len] == '\r') { buff[len] = '\0'; continue; }
                break;
            }
            if(sscanf(buff,
                      "arc xc=%d yc=%d rx=%d ry=%d start=%d end=%d",
                      &xc,&yc,&rx,&ry,&start,&end) == 6) {
                grx_clear_screen(GRX_COLOR_BLACK);
                grx_draw_ellipse(xc,yc,rx,ry,red);
                grx_draw_filled_ellipse(xc,yc,rx,ry,blue);
                grx_draw_ellipse_arc(xc,yc,rx,ry,start,end,GRX_ARC_STYLE_CLOSED_RADIUS,GRX_COLOR_WHITE);
                grx_draw_text(buff,0,0,white_text);
                grx_draw_text("press any key to continue",0,20,white_text);
                GrKeyRead();
                grx_clear_screen(GRX_COLOR_BLACK);
                grx_draw_ellipse_arc(xc,yc,rx,ry,start,end,GRX_ARC_STYLE_CLOSED_RADIUS,red);
                grx_draw_filled_ellipse_arc(xc,yc,rx,ry,start,end,GRX_ARC_STYLE_CLOSED_RADIUS,green);
                grx_draw_text(buff,0,0,white_text);
                grx_draw_text("press any key to continue",0,20,white_text);
                GrKeyRead();
            }
        }
        fclose(fp);
}
