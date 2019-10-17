/**
 ** scrollbar.c ---- Mini GUI for MGRX, scrollbars
 **
 ** Copyright (C) 2019 Mariano Alvarez Fernandez
 ** [e-mail: malfer at telefonica dot net]
 **
 ** This file is part of the GRX graphics library.
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
#include <string.h>
#include "grguip.h"

GrColor _scbbgcolor;
GrColor _scbliftcolor;

/***************************/

void _GUIScrollbarInit(void)
{
    _scbbgcolor = GrBlack();
    _scbliftcolor = GrWhite();
}

/***************************/

void _GUIScrollbarEnd(void)
{
}

/***************************/

void GUIScrollbarsSetColors(GrColor bg, GrColor lift)
{
    _scbbgcolor = bg;
    _scbliftcolor = lift;
}

/***************************/

GUIScrollbar * GUIScrollbarCreate(int type, int x, int y, int thick, int lenght)
{
    GUIScrollbar *scb;
    
    scb = (GUIScrollbar *)malloc(sizeof(GUIScrollbar));
    if (scb == NULL) return NULL;

    if (type == GUI_TSB_VERTICAL) {
        scb->type = GUI_TSB_VERTICAL;
        scb->x = x;
        scb->width = thick;
        scb->y = y;
        scb->height = lenght;
    } else {
        scb->type = GUI_TSB_HORIZONTAL;
        scb->x = x;
        scb->width = lenght;
        scb->y = y;
        scb->height = thick;
    }

    scb->pressed = 0;

    GUIScrollbarSetLimits(scb, 0, 10, 0, 10);

    return scb;
}

/***************************/

void GUIScrollbarSetLimits(GUIScrollbar *scb, int minvalue, int maxvalue,
                           int inivalue, int endvalue)
{
    long p1, p2;

    // some protection checks
    if (maxvalue <= minvalue) maxvalue = minvalue + 1; // not want divide by 0
    if (endvalue > maxvalue) endvalue = maxvalue;
    if (inivalue < minvalue) inivalue = minvalue;
    if (inivalue >= endvalue) {
        inivalue = minvalue;
        endvalue = maxvalue;
    }

    //printf("%4d %4d   %4d %4d    ", minvalue, maxvalue, inivalue,endvalue);
    scb->minvalue = minvalue;
    scb->inivalue = inivalue;
    scb->maxvalue = maxvalue;
    scb->endvalue = endvalue;

    if (scb->type == GUI_TSB_VERTICAL) {
        p1 = ((long)(scb->inivalue - scb->minvalue) * (scb->height - 4))
             / (scb->maxvalue - scb->minvalue);
        p2 = ((long)(scb->endvalue - scb->minvalue) * (scb->height - 4))
             / (scb->maxvalue - scb->minvalue);
        //printf("%4d    %4ld %4ld\n", scb->height - 4, p1, p2);
        scb->liftx = scb->x + 2;
        scb->liftw = scb->width - 4;
        scb->lifty = scb->y + p1 + 2;
        scb->lifth = p2 - p1;
    } else {
        p1 = ((long)(scb->inivalue - scb->minvalue) * (scb->width - 4))
             / (scb->maxvalue - scb->minvalue);
        p2 = ((long)(scb->endvalue - scb->minvalue) * (scb->width - 4))
             / (scb->maxvalue - scb->minvalue);
        scb->liftx = scb->x + p1 + 2;
        scb->liftw = p2 - p1;
        scb->lifty = scb->y + 2;
        scb->lifth = scb->height - 4;
    }
}

/***************************/

void GUIScrollbarPaint(GUIScrollbar *scb)
{
    GrFilledBox(scb->x, scb->y, scb->x+scb->width-1,
                scb->y+scb->height-1, _scbbgcolor);
    GrFilledBox(scb->liftx, scb->lifty, scb->liftx+scb->liftw-1,
                scb->lifty+scb->lifth-1, _scbliftcolor);
}

/***************************/

void GUIScrollbarBltToScreen(GUIContext *gctx, GUIScrollbar *scb)
{
    GUIContextBltRectToScreen(gctx, scb->x, scb->y, scb->x+scb->width-1,
                              scb->y+scb->height-1);
}

/***************************/

int GUIScrollbarProcessEvent(GUIScrollbar *scb, GrEvent *ev)
{
    int x, y;

    if (ev->type == GREV_MOUSE) {
        x = ev->p2;
        y = ev->p3;
        if (ev->p1 == GRMOUSE_LB_PRESSED) {
            if (GrCheckCoordInto(x, y, scb->x, scb->y, scb->width, scb->height)) {
                scb->pressed = 1;
                scb->lastkeysend = GrKey_NoKey;
                scb->lasttimesend = ev->time;
                scb->intv = 500;
                if (scb->type == GUI_TSB_VERTICAL) {
                    if (y < scb->lifty)
                        scb->lastkeysend = GrKey_PageUp;
                    if (y >= scb->lifty+scb->lifth)
                        scb->lastkeysend = GrKey_PageDown;
                } else {
                    if (x < scb->liftx)
                        scb->lastkeysend = GrKey_Home;
                    if (x >= scb->liftx+scb->liftw)
                        scb->lastkeysend = GrKey_End;
                }
                if (scb->lastkeysend != GrKey_NoKey)
                    GrEventParEnqueue(GREV_KEY, scb->lastkeysend, GRKEY_KEYCODE, 0, 0);
                return 1;
            }
        } else if (ev->p1 == GRMOUSE_LB_RELEASED) {
            if (scb->pressed) {
                scb->pressed = 0;
                return 1;
            }
        }
    }
    if (ev->type == GREV_NULL) {
        if (scb->pressed && (scb->lastkeysend != GrKey_NoKey) &&
            (ev->time > scb->lasttimesend + scb->intv)) {
            GrEventParEnqueue(GREV_KEY, scb->lastkeysend, GRKEY_KEYCODE, 0, 0);
            scb->lasttimesend = ev->time;
            scb->intv = 50;
            return 1;
        }
    }
    
    return 0;
}
