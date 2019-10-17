/**
 ** pnmtest.c ---- test the ctx2pnm routines
 **
 ** Copyright (c) 2000 Mariano Alvarez Fernandez
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
 
#include <stdlib.h>
#include <stdio.h>
#include "mgrx.h"
#include "mgrxkeys.h"

#if defined(__MSDOS__) || defined(__WIN32__)
#define FIMAGEPPM "..\\testimg\\pnmtest.ppm"
#define FIMAGEPBM "..\\testimg\\pnmtest.pbm"
#define FIMAGEPGM "..\\testimg\\prueba.pgm"
#define FIMAGEPBM2 "..\\testimg\\prueba.pbm"
#define FSCREEN "..\\testimg\\output.ppm"
#else
#define FIMAGEPPM "../testimg/pnmtest.ppm"
#define FIMAGEPBM "../testimg/pnmtest.pbm"
#define FIMAGEPGM "../testimg/prueba.pgm"
#define FIMAGEPBM2 "../testimg/prueba.pbm"
#define FSCREEN "../testimg/output.ppm"
#endif

/* default mode */

static int gwidth = 640;
static int gheight = 480;
static int gbpp = 24;

int main(int argc, char **argv)
{
    GrContext *grc;
    int wide, high, maxval;
    char s[81];
    GrEvent ev;

    if (argc >= 4) {
        gwidth = atoi(argv[1]);
        gheight = atoi(argv[2]);
        gbpp = atoi(argv[3]);
    }

    GrSetMode(GR_width_height_bpp_graphics, gwidth, gheight, gbpp);
    GrEventInit();
    GrMouseDisplayCursor();
    GrQueryPnm(FIMAGEPPM, &wide, &high, &maxval);
    sprintf(s, "%s %d x %d pixels", FIMAGEPPM, wide, high);
    GrTextXY(10, 20, s, GrBlack(), GrWhite());
    GrBox(10, 40, 10+wide+1, 40+high+1, GrWhite());
    grc = GrCreateSubContext(11, 41, 11+wide-1, 41+high-1, NULL, NULL);
    GrLoadContextFromPnm(grc, FIMAGEPPM);
    GrSaveContextToPgm(grc, FIMAGEPGM, "TestPnm");
    GrDestroyContext(grc);
    GrTextXY(10, 50+high, "Press RETURN to continue", GrBlack(), GrWhite());
    GrEventWaitKeyOrClick(&ev);

    GrClearScreen(GrBlack());
    GrQueryPnm(FIMAGEPGM, &wide, &high, &maxval);
    sprintf(s, "%s %d x %d pixels", FIMAGEPGM, wide, high);
    GrTextXY(10, 20, s, GrBlack(), GrWhite());
    GrBox(10, 40, 10+wide+1, 40+high+1, GrWhite());
    grc = GrCreateSubContext(11, 41, 11+wide-1, 41+high-1, NULL, NULL);
    GrLoadContextFromPnm(grc,FIMAGEPGM);
    GrDestroyContext(grc);
    GrTextXY(10, 50+high, "Press RETURN to continue", GrBlack(), GrWhite());
    GrEventWaitKeyOrClick(&ev);

    GrClearScreen(GrBlack());
    GrQueryPnm(FIMAGEPBM, &wide, &high, &maxval);
    sprintf(s, "%s %d x %d pixels", FIMAGEPBM, wide, high);
    GrTextXY(10, 20, s, GrBlack(), GrWhite());
    GrBox(10, 40, 10+wide+1, 40+high+1, GrWhite());
    grc = GrCreateSubContext(11, 41, 11+wide-1, 41+high-1, NULL, NULL);
    GrLoadContextFromPnm(grc,FIMAGEPBM);
    GrSaveContextToPbm(grc, FIMAGEPBM2, "TestPnm");
    GrDestroyContext(grc);
    GrTextXY(10, 50+high, "Press RETURN to continue", GrBlack(), GrWhite());
    GrEventWaitKeyOrClick(&ev);

    GrClearScreen(GrBlack());
    GrQueryPnm(FIMAGEPPM, &wide, &high, &maxval);
    GrBox(10, 40, 10+wide+1, 40+high+1, GrWhite());
    grc = GrCreateSubContext(11, 41, 11+wide-1, 41+high-1, NULL, NULL);
    GrLoadContextFromPnm(grc,FIMAGEPPM);
    GrDestroyContext(grc);
    GrQueryPnm(FIMAGEPGM, &wide, &high, &maxval);
    GrBox(110, 140, 110+wide+1, 140+high+1, GrWhite());
    grc = GrCreateSubContext(111, 141, 111+wide-1, 141+high-1, NULL, NULL);
    GrLoadContextFromPnm(grc, FIMAGEPGM);
    GrDestroyContext(grc);
    GrQueryPnm(FIMAGEPBM, &wide, &high, &maxval);
    GrBox(210, 240, 210+wide+1, 240+high+1, GrWhite());
    grc = GrCreateSubContext(211, 241, 211+wide-1, 241+high-1, NULL, NULL);
    GrLoadContextFromPnm(grc,FIMAGEPBM2);
    GrDestroyContext(grc);
    GrTextXY(10, 20, "Press RETURN to save screen", GrBlack(), GrWhite());
    GrEventWaitKeyOrClick(&ev);

    GrSaveContextToPpm(NULL, FSCREEN, "TestPnm");
    GrClearScreen(GrWhite());
    GrTextXY(10, 20, "Press RETURN to reload screen", GrWhite(), GrBlack());
    GrEventWaitKeyOrClick(&ev);

    GrLoadContextFromPnm(NULL, FSCREEN);
    GrTextXY(10, 20, "Press RETURN to end        ", GrBlack(), GrWhite());
    GrEventWaitKeyOrClick(&ev);

    GrEventUnInit();
    GrSetMode(GR_default_text);
    return 0;
}
