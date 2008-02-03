/**
 ** mousetst.c ---- test mouse cursor and mouse/keyboard input
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

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "test.h"

TESTFUNC(mousetest)
{
    GrEvent evt;
    GrColor bgc = GrAllocColor(0,0,128);
    GrColor fgc = GrAllocColor(255,255,0);
    int  ii,mode;
    char lmr[4] = "lmr";
    int px = 0;
    int py = 0;
    int genmmove = 0;

    GrClearScreen(bgc);

    if (!GrMouseDetect()) {
        GrTextXY((GrScreenX()/3), (GrScreenY() - 20),
                 "Sorry, no mouse found !", GrWhite(), bgc);
        GrEventWaitKeyOrClick(&evt);
        return;
    }

    GrMouseEraseCursor();
    GrMouseSetInternalCursor(GR_MCUR_TYPE_CROSS, GrAllocColor(255, 0, 0), GrBlack());
    GrMouseDisplayCursor();
    ii = 0;
    mode = GR_M_CUR_NORMAL;
    GrTextXY(10, (GrScreenY()-20),
             "Commands: 'N' next mouse mode, 'G' next genmove mode, 'Q' exit",
             GrWhite(), bgc);

    for ( ; ; ) {
        char msg[200];
        drawing(ii, ii, (GrSizeX()-20), (GrSizeY()-20), ((fgc^bgc)|GrXOR), GrNOCOLOR);
        if ((ii += 7) > 20) ii -= 20;
        GrEventRead(&evt);
//        if (evt.type == GREV_NULL) {
//          GrSleep(1);
//        }
//        else {
        if (evt.type != GREV_NULL) {
            strcpy(msg,"Got event(s): ");
#           define mend (&msg[strlen(msg)])
            if (evt.type == GREV_MOUSE) {
                px = evt.p2;
                py = evt.p3;
                if (evt.p1 == GRMOUSE_LB_PRESSED) {
                    strcpy(mend,"[left down] ");
                    lmr[0] = 'L';
                }
                if (evt.p1 == GRMOUSE_MB_PRESSED) {
                    strcpy(mend,"[middle down] ");
                    lmr[1] = 'M';
                }
                if (evt.p1 == GRMOUSE_RB_PRESSED) {
                    strcpy(mend,"[right down] ");
                    lmr[2] = 'R';
                }
                if (evt.p1 == GRMOUSE_B4_PRESSED) {
                    strcpy(mend,"[b4 down] ");
                }
                if (evt.p1 == GRMOUSE_B5_PRESSED) {
                    strcpy(mend,"[b5 down] ");
                }
                if (evt.p1 == GRMOUSE_LB_RELEASED) {
                    strcpy(mend,"[left up] ");
                    lmr[0] = 'l';
                }
                if (evt.p1 == GRMOUSE_MB_RELEASED) {
                    strcpy(mend,"[middle up] ");
                    lmr[1] = 'm';
                }
                if (evt.p1 == GRMOUSE_RB_RELEASED) {
                    strcpy( mend,"[right up] ");
                    lmr[2] = 'r';
                }
                if (evt.p1 == GRMOUSE_B4_RELEASED) {
                    strcpy( mend,"[b4 up] ");
                }
                if (evt.p1 == GRMOUSE_B5_RELEASED) {
                    strcpy( mend,"[b5 up] ");
                }
                sprintf(mend,"at X=%ld, Y=%ld, ", evt.p2, evt.p3);
            }
            if (evt.type == GREV_MMOVE) {
                px = evt.p2;
                py = evt.p3;
                sprintf(mend,"[moved (0x%03lx)] at X=%ld, Y=%ld, ", evt.p1, evt.p2, evt.p3);
            }
            if (evt.type == GREV_KEY) {
                sprintf(mend,"[key (0x%03lx)] ", evt.p1);
            }
            sprintf(mend, "buttons=%s", lmr);
            strcpy (mend, "                         ");
            GrTextXY(10, (GrScreenY()-40), msg, GrWhite(), bgc);

            if (evt.type == GREV_KEY) {
                int key = evt.p1;
                if ((key == 'Q') || (key == 'q')) break;
                if ((key == 'N') || (key == 'n')) {
                    GrMouseEraseCursor();
                    switch (mode = (mode + 1) & 3) {
                    case GR_M_CUR_RUBBER:
                        GrMouseSetCursorMode(GR_M_CUR_RUBBER, px, py, GrWhite()^bgc);
                        break;
                    case GR_M_CUR_LINE:
                        GrMouseSetCursorMode(GR_M_CUR_LINE, px, py, GrWhite()^bgc);
                        break;
                    case GR_M_CUR_BOX:
                        GrMouseSetCursorMode(GR_M_CUR_BOX, -20, -10, 20, 10, GrWhite()^bgc);
                        break;
                    default:
                        GrMouseSetCursorMode(GR_M_CUR_NORMAL);
                        break;
                    }
                    GrMouseDisplayCursor();
                }
                if ((key == 'G') || (key == 'g')) {
                    genmmove++;
                    if (genmmove > 2) genmmove = 0;
                    GrEventGenMmove(genmmove);
                }
            }
        }
    }
}
