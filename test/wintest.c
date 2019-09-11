/**
 ** wintest.c ---- test window (context) mapping and OP colors
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
 ** 20190908 Test OP colors (M.Alvarez)
 **
 **/

#include "test.h"

void dodraw(GrColor op, int d)
{
    int  x = GrSizeX();
    int  y = GrSizeY();
    int  ww = (x / 2) - 10;
    int  wh = (y / 2) - 10;
    GrColor c1, c2, c3, c4;
    GrContext *w1 = GrCreateSubContext(5,5,ww+4,wh+4,NULL,NULL);
    GrContext *w2 = GrCreateSubContext(15+ww,5,ww+ww+14,wh+4,NULL,NULL);
    GrContext *w3 = GrCreateSubContext(5,15+wh,ww+4,wh+wh+14,NULL,NULL);
    GrContext *w4 = GrCreateSubContext(15+ww,15+wh,ww+ww+14,wh+wh+14,NULL,NULL);

    if (op == GrWhite()) {
        c1 = c2 = c3 = c4 = GrWhite();
    } else {
        c1 = GrAllocColor(200,100,100) | op;
        c2 = GrAllocColor(100,200,200) | op;
        c3 = GrAllocColor(200,200,0) | op;
        c4 = GrAllocColor(0,100,200) | op;
    }

    GrSetContext(w1);
    drawing(0+d,0+d,ww,wh,c1,GrNOCOLOR);
    GrBox(0,0,ww-1,wh-1,c1);

    GrSetContext(w2);
    drawing(0+d,0+d,ww,wh,c2,GrNOCOLOR);
    GrBox(0,0,ww-1,wh-1,c2);

    GrSetContext(w3);
    drawing(0+d,0+d,ww,wh,c3,GrNOCOLOR);
    GrBox(0,0,ww-1,wh-1,c3);

    GrSetContext(w4);
    drawing(0+d,0+d,ww,wh,c4,GrNOCOLOR);
    GrBox(0,0,ww-1,wh-1,c4);
    
    GrSetContext(NULL);
    GrDestroyContext(w1);
    GrDestroyContext(w2);
    GrDestroyContext(w3);
    GrDestroyContext(w4);
}

void PrintCentered(char *s)
{
    int x, y;

    x = (GrMaxX() -
        GrFontStringWidth(GrGetDefaultFont(), s, 0, GR_BYTE_TEXT)) / 2;
    y = (GrMaxY() -
        GrFontStringHeight(GrGetDefaultFont(), s, 0, GR_BYTE_TEXT)) / 2;
    GrTextXY(x, y, s, GrBlack(), GrWhite());
}

TESTFUNC(wintest)
{
    GrEvent ev;

    GrSetDefaultFont(&GrFont_PC8x16);
    
    GrClearScreen(GrBlack());
    dodraw(GrWRITE, 0);
    PrintCentered(" GrWRITE only. Press any key to continue ");
    GrEventWaitKeyOrClick(&ev);

    GrClearScreen(GrBlack());
    dodraw(GrWRITE, -5);
    dodraw(GrXOR, 5);
    PrintCentered(" GrWRITE+GrXOR. Press any key to continue ");
    GrEventWaitKeyOrClick(&ev);

    GrClearScreen(GrBlack());
    dodraw(GrWhite(), -5);
    dodraw(GrXOR, 5);
    PrintCentered(" GrWhite+GrXOR. Press any key to continue ");
    GrEventWaitKeyOrClick(&ev);

    GrClearScreen(GrBlack());
    dodraw(GrWhite(), -5);
    dodraw(GrAND, 5);
    PrintCentered(" GrWhite+GrAND. Press any key to continue ");
    GrEventWaitKeyOrClick(&ev);

    GrClearScreen(GrBlack());
    dodraw(GrWhite(), -5);
    dodraw(GrOR, 5);
    PrintCentered(" GrWRITE+GrOR. Press any key to finish ");
    GrEventWaitKeyOrClick(&ev);
}

