/**
 ** rgbtest.c ---- show 256 color RGB palette 
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

TESTFUNC(rgbtest)
{
    int x, y, ww, wh, rww, rwh;
    int ii, jj;
    GrEvent ev;
    GrColor c;

    x = GrSizeX();
    y = GrSizeY();
    ww = x / 32;
    wh = y / 8;
    rww = (x % 32) / 2;
    rwh = (y % 8) / 2;
    
    if (GrCurrentVideoMode()->bpp < 8) {
        sprintf(exit_message, "RGB mode not supported for videomodes with bpp < 8");
        return;
    }
    
    GrSetRGBcolorMode();
    
    for (ii = 0; ii < 8; ii++) {
        for (jj = 0; jj < 32; jj++) {
            c = GrAllocColor(ii*32+jj, 0, 0);
            GrFilledBox(rww+jj*ww, rwh+ii*wh, rww+jj*ww+ww-1, rwh+ii*wh+wh-1, c);
        }
    }
    GrEventWaitKeyOrClick(&ev);
    
    for (ii = 0; ii < 8; ii++) {
        for (jj = 0; jj < 32; jj++) {
            c = GrAllocColor(0, ii*32+jj, 0);
            GrFilledBox(rww+jj*ww, rwh+ii*wh, rww+jj*ww+ww-1, rwh+ii*wh+wh-1, c);
        }
    }
    GrEventWaitKeyOrClick(&ev);
    
    for (ii = 0; ii < 8; ii++) {
        for (jj = 0; jj < 32; jj++) {
            c = GrAllocColor(0, 0, ii*32+jj);
            GrFilledBox(rww+jj*ww, rwh+ii*wh, rww+jj*ww+ww-1, rwh+ii*wh+wh-1, c);
        }
    }
    GrEventWaitKeyOrClick(&ev);
    
    for (ii = 0; ii < 8; ii++) {
        for (jj = 0; jj < 32; jj++) {
            c = GrAllocColor(ii*32+jj, ii*32+jj, ii*32+jj);
            GrFilledBox(rww+jj*ww, rwh+ii*wh, rww+jj*ww+ww-1, rwh+ii*wh+wh-1, c);
        }
    }
    GrEventWaitKeyOrClick(&ev);
}

