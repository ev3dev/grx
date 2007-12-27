/**
 ** jpgtest.c ---- test the ctx2jpeg routines
 **
 ** Copyright (c) 2001,2005 Mariano Alvarez Fernandez
 ** [e-mail: malfer@telefonica.net]
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
 **/

#include <stdio.h>
#include <stdlib.h>
#include "mgrx.h"
#include "mgrxkeys.h"

void imagen(char *nf, int scale)
{
    GrContext *grc;
    int wide, high;
    char s[81];
    int w, h;
    GrEvent ev;

    GrQueryJpeg(nf, &w, &h);
    sprintf(s, "%s %dx%d scale 1/%d", nf, w, h, scale);
    wide = (w/scale > 600) ? 600 : w/scale;
    high = (h/scale > 400) ? 400 : h/scale;
    GrClearScreen(GrAllocColor(0, 0, 200));

    GrBox(10, 40, 10+wide+1, 40+high+1, GrWhite());
    grc = GrCreateSubContext(11, 41, 11+wide-1, 41+high-1, NULL, NULL);
    GrLoadContextFromJpeg(grc, nf, scale);
    GrDestroyContext(grc);

    GrTextXY(10, 10, s, GrBlack(), GrWhite());
    GrTextXY(10, 50+high, "Press any key to continue", GrBlack(), GrWhite());
    GrEventWaitKeyOrClick(&ev);
}

void nojpegsupport(void)
{
    char *s[6] = {
    "Warning!",
    "You need libjpeg (http://www.ijg.org) and enable",
    "jpeg support in the GRX lib (edit makedefs.grx)",
    "to run this demo",
    " ",
    "Press any key to continue..." };
    int i;
    GrEvent ev;

    GrClearScreen(GrAllocColor(0, 0, 100));
    for (i=0; i<6; i++)
        GrTextXY(90, 160+i*18, s[i], GrWhite(), GrNOCOLOR);
    GrEventWaitKeyOrClick(&ev);
}

int main()
{
    GrContext *grc;
    GrEvent ev;

    GrSetMode(GR_width_height_bpp_graphics, 640, 480, 24);
    GrEventInit();
    GrMouseDisplayCursor();

    if (!GrJpegSupport()) {
        nojpegsupport();
        GrEventUnInit();
        GrSetMode(GR_default_text);
        exit(1);
    }

    imagen("jpeg1.jpg", 1);
    imagen("jpeg1.jpg", 2);
    imagen("jpeg1.jpg", 4);
    imagen("jpeg1.jpg", 8);
    imagen("jpeg2.jpg", 1);
    imagen("jpeg2.jpg", 2);
    imagen("jpeg2.jpg", 4);
    imagen("jpeg2.jpg", 8);

    GrClearScreen(GrAllocColor(0, 100, 0));
    grc = GrCreateSubContext(10, 40, 10+400-1, 40+300-1, NULL, NULL);
    GrLoadContextFromJpeg(grc, "jpeg1.jpg", 2);
    GrDestroyContext(grc);
    grc = GrCreateSubContext(210, 150, 210+400-1, 150+300-1, NULL, NULL);
    GrLoadContextFromJpeg(grc, "jpeg2.jpg", 2);
    GrDestroyContext(grc);

    GrTextXY(10, 10, "Press any key to save color and gray screen",
             GrBlack(), GrWhite());
    GrEventWaitKeyOrClick(&ev);

    GrSaveContextToJpeg(NULL, "output.jpg", 75);
    GrSaveContextToGrayJpeg(NULL, "outputg.jpg", 75);

    GrClearScreen(GrBlack());
    GrTextXY(10, 10," Press any key to reload color screen       ",
             GrBlack(), GrWhite());
    GrEventWaitKeyOrClick(&ev);
    GrLoadContextFromJpeg( NULL, "output.jpg", 1);

    GrTextXY(10, 10, "Press any key to reload gray screen        ",
             GrBlack(), GrWhite());
    GrEventWaitKeyOrClick(&ev);
    GrClearScreen(GrBlack());
    GrLoadContextFromJpeg(NULL, "outputg.jpg", 1);

    GrTextXY(10, 10, "Press any key to end                       ",
             GrBlack(), GrWhite());
    GrEventWaitKeyOrClick(&ev);

    GrEventUnInit();
    GrSetMode(GR_default_text);
    return 0;
}
