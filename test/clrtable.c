/**
 ** clrtablel.c ---- test color tables
 **
 ** Copyright (C) 2019 Mariano Alvarez Fernandez
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

#include "mgrx.h"
#include "mgrxkeys.h"
#include "mgrxcolr.h"

/* default mode */

static int gwidth = 1024;
static int gheight = 768;
static int gbpp = 24;

int main(int argc,char **argv)
{
    GrEvent ev;
    int i, j, ind, wide, high;
    int egatableloaded = 0;
    int webtableloaded = 0;
    
    if (argc >= 4) {
        gwidth = atoi(argv[1]);
        gheight = atoi(argv[2]);
        gbpp = atoi(argv[3]);
    }

    GrSetMode(GR_width_height_bpp_graphics, gwidth, gheight, gbpp);

    GrEventInit();
    GrMouseDisplayCursor();
    GrSetDefaultFont(&GrFont_PC8x16);
    
    if (!GrGenEgaColorTable()) {
        egatableloaded = 1;
        GrClearScreen(EGAC_BLACK);
        GrTextXY(10, 10, "EGA COLOR TABLE", EGAC_WHITE, EGAC_BLACK);
        wide = (GrMaxX()-10) / 4;
        high = (GrMaxY()-52) / 4;
        ind = 0;
        for (i=0; i<4; i++) {
            for (j=0; j<4; j++) {
                if (ind < NUM_EGA_COLORS) {
                    GrFilledBox(j*wide+10, i*high+50, (j+1)*wide, (i+1)*high+50,
                                GrEgaColorTable[ind].color);
                    GrTextXY(j*wide+12, i*high+52, GrEgaColorTable[ind].name,
                             EGAC_WHITE, EGAC_BLACK);
                    ind++;
                }
            }
        }
        GrEventWaitKeyOrClick(&ev);
    }

    wide = GrMaxX() / 2;
    high = (GrMaxY()-52) / 2;

    GrClearScreen(GrAllocColor2(EGAR_BLACK));
    GrTextXY(10, 10, "EGA COLOR STATIC RGB DEFS", GrAllocColor2(EGAR_WHITE),
             GrAllocColor2(EGAR_BLACK));
    GrFilledBox(10, 50, wide-10, high+30, GrAllocColor2(EGAR_RED));
    GrFilledBox(wide+10, 50, wide*2-10, high+30, GrAllocColor2(EGAR_GREEN));
    GrFilledBox(10, high+50, wide-10, high*2+30, GrAllocColor2(EGAR_BLUE));
    GrFilledBox(wide+10, high+50, wide*2-10, high*2+30, GrAllocColor2(EGAR_YELLOW));
    GrEventWaitKeyOrClick(&ev);
    
    if (egatableloaded) {
        GrClearScreen(EGAC_BLACK);
        GrTextXY(10, 10, "EGA COLOR DYNAMIC DEFS", EGAC_WHITE, EGAC_BLACK);
        GrFilledBox(10, 50, wide-10, high+30, EGAC_RED);
        GrFilledBox(wide+10, 50, wide*2-10, high+30, EGAC_GREEN);
        GrFilledBox(10, high+50, wide-10, high*2+30, EGAC_BLUE);
        GrFilledBox(wide+10, high+50, wide*2-10, high*2+30, EGAC_YELLOW);
        GrEventWaitKeyOrClick(&ev);
    }

    if (egatableloaded) {
        GrClearScreen(EgaColor(EGAI_BLACK));
        GrTextXY(10, 10, "EGA COLOR INDEXES DEFS", EgaColor(EGAI_WHITE),
                 EgaColor(EGAI_BLACK));
        GrFilledBox(10, 50, wide-10, high+30, EgaColor(EGAI_RED));
        GrFilledBox(wide+10, 50, wide*2-10, high+30, EgaColor(EGAI_GREEN));
        GrFilledBox(10, high+50, wide-10, high*2+30, EgaColor(EGAI_BLUE));
        GrFilledBox(wide+10, high+50, wide*2-10, high*2+30, EgaColor(EGAI_YELLOW));
        GrEventWaitKeyOrClick(&ev);
    }

    if (!GrGenWebColorTable()) {
        webtableloaded = 1;
        GrClearScreen(WEBC_BLACK);
        GrTextXY(10, 10, "WEB COLOR TABLE", WEBC_WHITE, WEBC_BLACK);
        wide = (GrMaxX()-10) / 4;
        high = (GrMaxY()-52) / 35;
        ind = 0;
        for (i=0; i<35; i++) {
            for (j=0; j<4; j++) {
                if (ind < NUM_WEB_COLORS) {
                    GrFilledBox(j*wide+10, i*high+50, (j+1)*wide, (i+1)*high+50,
                                GrWebColorTable[ind].color);
                    GrTextXY(j*wide+12, i*high+52, GrWebColorTable[ind].name,
                             WEBC_WHITE, WEBC_BLACK);
                    ind++;
                }
            }
        }
        GrEventWaitKeyOrClick(&ev);
    }

    wide = GrMaxX() / 2;
    high = (GrMaxY()-52) / 2;

    GrClearScreen(GrAllocColor2(WEBC_BLACK));
    GrTextXY(10, 10, "WEB COLOR STATIC RGB DEFS", GrAllocColor2(WEBR_WHITE), GrAllocColor2(WEBR_BLACK));
    wide = GrMaxX() / 2;
    GrFilledBox(10, 50, wide-10, high+30, GrAllocColor2(WEBR_ORANGE));
    GrFilledBox(wide+10, 50, wide*2-10, high+30, GrAllocColor2(WEBR_TURQUOISE));
    GrFilledBox(10, high+50, wide-10, high*2+30, GrAllocColor2(WEBR_INDIGO));
    GrFilledBox(wide+10, high+50, wide*2-10, high*2+30, GrAllocColor2(WEBR_YELLOWGREEN));
    GrEventWaitKeyOrClick(&ev);
    
    if (webtableloaded) {
        GrClearScreen(WEBC_BLACK);
        GrTextXY(10, 10, "WEB COLOR DYNAMIC DEFS", WEBC_WHITE, WEBC_BLACK);
        wide = GrMaxX() / 2;
        GrFilledBox(10, 50, wide-10, high+30, WEBC_ORANGE);
        GrFilledBox(wide+10, 50, wide*2-10, high+30, WEBC_TURQUOISE);
        GrFilledBox(10, high+50, wide-10, high*2+30, WEBC_INDIGO);
        GrFilledBox(wide+10, high+50, wide*2-10, high*2+30, WEBC_YELLOWGREEN);
        GrEventWaitKeyOrClick(&ev);
    }

    if (webtableloaded) {
        GrClearScreen(WebColor(WEBI_BLACK));
        GrTextXY(10, 10, "WEB COLOR INDEXES DEFS", WebColor(WEBI_WHITE), WebColor(WEBI_BLACK));
        wide = GrMaxX() / 2;
        GrFilledBox(10, 50, wide-10, high+30, WebColor(WEBI_ORANGE));
        GrFilledBox(wide+10, 50, wide*2-10, high+30, WebColor(WEBI_TURQUOISE));
        GrFilledBox(10, high+50, wide-10, high*2+30, WebColor(WEBI_INDIGO));
        GrFilledBox(wide+10, high+50, wide*2-10, high*2+30, WebColor(WEBI_YELLOWGREEN));
        GrEventWaitKeyOrClick(&ev);
    }

    GrEventUnInit();
    GrSetMode(GR_default_text);
    return 0;
}
