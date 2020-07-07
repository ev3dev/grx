/**
 ** gradtest.c ---- test gradients
 **
 ** Copyright (C) 2020 Mariano Alvarez Fernandez
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
 **
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "mgrx.h"

GrFont *glbfont;
GrTextOption glbopt;

/* default mode */

static int gwidth = 1024;
static int gheight = 768;
static int gbpp = 24;

void test(GrPattern *gradient)
{
    int pol[7][2] = {{20,730}, {880,730}, {880,520}, {500,600},
                     {400,520}, {200,600}, {20,520}};
    GrEvent ev;
    GrContext *sbctx, csave;
    int i, j;

    GrPatternFilledBox(0, 0, GrMaxX(), GrMaxY(), gradient);
    GrEventWaitKeyOrClick(&ev);

    GrClearScreen(GrBlack());
    GrPatternFilledBox(20, 20, 320, 220, gradient);
    GrPatternFilledCircle(170, 370, 150, gradient);
    GrPatternFilledBox(320, 220, 520, 520, gradient);
    GrPatternFilledEllipse(700, 370, 180, 150, gradient);
    GrPatternFilledPolygon(7, pol, gradient);
    GrPatternDrawString(" Hello world. MGRX gradients!! ", 0, 350, 50, &glbopt, gradient);
    GrPatternDrawStringExt(" Hello world. MGRX gradients!! ", 0, 360, 150, &glbopt, gradient);
    GrPatternFilledLine(350, 40, 880, 30, gradient);
    GrPatternFilledLine(360, 140, 900, 130, gradient);
    GrEventWaitKeyOrClick(&ev);

    GrClearScreen(GrBlack());
    GrPatAlignFilledBox(20, 20, 20, 20, 320, 220, gradient);
    for (i=0; i<20; i++)
        GrPatAlignFilledLine(20, 230, 20, 230+i, 319, 230+i, gradient);
    for (i=0; i<20; i++)
        for (j=0; j<300; j++)
            GrPatAlignFilledPlot(20, 250, 20+j, 250+i, gradient);
    GrSaveContext(&csave);
    sbctx = GrCreateSubContext(330, 230, 630, 500, NULL, NULL);
    GrSetContext(sbctx);
    GrPatAlignFilledBox(0, 0, 0, 0, 300, 200, gradient);
    for (i=0; i<20; i++)
        GrPatAlignFilledLine(0, 210, 0, 210+i, 299, 210+i, gradient);
    for (i=0; i<20; i++)
        for (j=0; j<300; j++)
            GrPatAlignFilledPlot(0, 230, 0+j, 230+i, gradient);
    GrSetContext(&csave);
    GrDestroyContext(sbctx);
    GrEventWaitKeyOrClick(&ev);
}

int main(int argc, char **argv)
{
    GrPattern *gradient;
    GrEvent ev;

    if (argc >= 4) {
        gwidth = atoi(argv[1]);
        gheight = atoi(argv[2]);
        gbpp = atoi(argv[3]);
    }

    GrSetMode(GR_width_height_bpp_graphics, gwidth, gheight, gbpp);
    GrSetRGBcolorMode();
    GrEventInit();
    GrMouseDisplayCursor();
    if (GrNumColors() < 256) {
        GrTextXY(20, 20, "8bpp or more is needed to run this test",
                 GrWhite(), GrBlack());
        GrEventWaitKeyOrClick(&ev);
        goto end;
    }

    glbfont = GrLoadFont("lucs40b.fnt");
    if (!glbfont) glbfont = GrLoadFont("../fonts/lucs40b.fnt");
    if (!glbfont) return 1;
    glbopt.txo_font   = glbfont;
    glbopt.txo_xalign = 0;
    glbopt.txo_yalign = 0;
    glbopt.txo_direct = GR_TEXT_RIGHT;
    glbopt.txo_fgcolor = GrNOCOLOR;
    glbopt.txo_bgcolor = GrNOCOLOR;

    gradient = GrCreateLinGradient(0, 0, GrMaxX(), GrMaxY());
    GrAddGradientStop(gradient, 0, GrAllocColor(255, 0, 0));
    GrAddGradientStop(gradient, 255, GrAllocColor(0, 255, 0));
    GrGenGradientColorTbl(gradient);
    test(gradient);
    GrDestroyPattern(gradient);

    gradient = GrCreateLinGradient(0, GrMaxY(), GrMaxX(), GrMaxY()/2);
    GrAddGradientStop(gradient, 0, GrAllocColor(255, 0, 0));
    GrAddGradientStop(gradient, 100, GrAllocColor(0, 255, 0));
    GrAddGradientStop(gradient, 255, GrAllocColor(0, 0, 255));
    GrGenGradientColorTbl(gradient);
    test(gradient);
    GrDestroyPattern(gradient);

    gradient = GrCreateLinGradient(0, GrMaxY(), GrMaxX()/2, 0);
    GrAddGradientStop(gradient, 10, GrAllocColor(255, 0, 0));
    GrAddGradientStop(gradient, 80, GrAllocColor(0, 255, 0));
    GrAddGradientStop(gradient, 100, GrAllocColor(0, 255, 0));
    GrAddGradientStop(gradient, 200, GrAllocColor(0, 0, 255));
    GrAddGradientStop(gradient, 240, GrAllocColor(0, 255, 255));
    GrGenGradientColorTbl(gradient);
    test(gradient);
    GrDestroyPattern(gradient);

    gradient = GrCreateRadGradient(GrMaxX()/4, GrMaxY()/4, GrMaxX());
    GrAddGradientStop(gradient, 0, GrAllocColor(255, 0, 0));
    GrAddGradientStop(gradient, 255, GrAllocColor(0, 255, 0));
    GrGenGradientColorTbl(gradient);
    test(gradient);
    GrDestroyPattern(gradient);

    gradient = GrCreateRadGradient(GrMaxX()/2, GrMaxY()/2, GrMaxX()/2);
    GrAddGradientStop(gradient, 0, GrAllocColor(255, 0, 0));
    GrAddGradientStop(gradient, 128, GrAllocColor(0, 0, 255));
    GrAddGradientStop(gradient, 255, GrAllocColor(0, 255, 0));
    GrGenGradientColorTbl(gradient);
    test(gradient);
    GrDestroyPattern(gradient);

end:
    GrEventUnInit();
    GrSetMode(GR_default_text);
    return 0;
}
