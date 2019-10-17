/**
 ** memtest.c ---- test the memory driver
 **
 ** Copyright (C) 2001 Mariano Alvarez Fernandez
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

#include <stdlib.h>
#include <stdio.h>
#include "mgrx.h"
#include "drawing.h"

#if defined(__MSDOS__) || defined(__WIN32__)
#define PPMIMGOUT "..\\testimg\\memtest.ppm"
#else
#define PPMIMGOUT "../testimg/memtest.ppm"
#endif

int main(int argc, char **argv)
{
    int gwidth = 400;
    int gheight = 400;
    int gbpp = 8;
    int x, y, ww, wh;
    GrColor c;
    GrContext *w1, *w2, *w3, *w4;
    char drspec[81], *sncolors;

    if (argc >= 4) {
        gwidth = atoi(argv[1]);
        gheight = atoi(argv[2]);
        gbpp = atoi(argv[3]);
    }
    sncolors = "256"; /* default 8bpp */
    if (gbpp == 1) sncolors = "2";
    if (gbpp == 4) sncolors = "16";
    if (gbpp == 16) sncolors = "64K";
    if (gbpp >= 24) sncolors = "16M";
    
    sprintf(drspec, "memory gw %d gh %d nc %s", gwidth, gheight, sncolors);
    printf("Set driver specs to \"%s\"\n", drspec);
    
    GrSetDriver( drspec );
    GrSetMode( GR_default_graphics );
    printf("FrameDriver: %s\n", GrFrameDriverName(GrScreenFrameMode()));

    x = GrSizeX();
    y = GrSizeY();
    ww = (x / 2) - 10;
    wh = (y / 2) - 10;
    w1 = GrCreateSubContext(5,5,ww+4,wh+4,NULL,NULL);
    w2 = GrCreateSubContext(15+ww,5,ww+ww+14,wh+4,NULL,NULL);
    w3 = GrCreateSubContext(5,15+wh,ww+4,wh+wh+14,NULL,NULL);
    w4 = GrCreateSubContext(15+ww,15+wh,ww+ww+14,wh+wh+14,NULL,NULL);

    GrSetContext(w1);
    c = GrAllocColor(200,100,100);
    //printf("color %8lx\n", (long)c);
    drawing(0,0,ww,wh,c,GrBlack());
    GrBox(0,0,ww-1,wh-1,c);

    GrSetContext(w2);
    c = GrAllocColor(100,200,200);
    //printf("color %8lx\n", (long)c);
    drawing(0,0,ww,wh,c,GrBlack());
    GrBox(0,0,ww-1,wh-1,c);

    GrSetContext(w3);
    c = GrAllocColor(200,200,0);
    //printf("color %8lx\n", (long)c);
    drawing(0,0,ww,wh,c,GrBlack());
    GrBox(0,0,ww-1,wh-1,c);

    GrSetContext(w4);
    c = GrAllocColor(0,100,200);
    //printf("color %8lx\n", (long)c);
    drawing(0,0,ww,wh,c,GrBlack());
    GrBox(0,0,ww-1,wh-1,c);

    GrSetContext( NULL );
    GrSaveContextToPpm( NULL,PPMIMGOUT,"GRX MemTest" );
    printf("Saved output in: %s\n", PPMIMGOUT);

    return 0;
}
