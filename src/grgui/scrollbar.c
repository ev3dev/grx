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

static long next_aid = 1;

static void send_event(GUIScrollbar *scb, int first, int pos);

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
    scb->dx = 0;
    scb->dy = 0;
    scb->aid = next_aid++;

    GUIScrollbarSetLimits(scb, 0, 10, 0, 10);

    return scb;
}

/***************************/

long GUIScrollbarGetAid(GUIScrollbar *scb)
{
    return scb->aid;
}

/***************************/

void GUIScrollbarSetLimits(GUIScrollbar *scb, int minvalue, int maxvalue,
                           int inivalue, int endvalue)
{
    long p1, p2;

    // some protection checks
    if (maxvalue <= minvalue) maxvalue = minvalue + 1; // no divide by 0
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

void GUIScrollbarPaint(GUIScrollbar *scb, int dx, int dy, int blt)
{
    int x, y, fx, fy;

    scb->dx = dx;
    scb->dy = dy;

    x = scb->x + scb->dx;
    y = scb->y + scb->dy;
    fx = scb->liftx + scb->dx;
    fy = scb->lifty + scb->dy;

    GrFilledBox(x, y, x+scb->width-1, y+scb->height-1, _scbbgcolor);
    GrFilledBox(fx, fy, fx+scb->liftw-1, fy+scb->lifth-1, _scbliftcolor);
    if (blt) {
        GUIDBCurCtxBltRectToScreen(x, y, x+scb->width-1, y+scb->height-1);
    }
}


/***************************/

int GUIScrollbarProcessEvent(GUIScrollbar *scb, GrEvent *ev)
{
    int x, y;

    if (ev->type == GREV_MOUSE) {
        x = ev->p2 - scb->dx;
        y = ev->p3 - scb->dy;
        if (ev->p1 == GRMOUSE_LB_PRESSED) {
            if (GrCheckCoordInto(x, y, scb->x, scb->y, scb->width, scb->height)) {
                scb->pressed = 1;
                scb->lasttimesend = ev->time;
                scb->intv = 500;
                if (scb->type == GUI_TSB_VERTICAL) {
                    scb->pressed_pos = y;
                    if (y < scb->lifty)
                        scb->pressedmode = 1;
                    else if (y >= scb->lifty+scb->lifth)
                        scb->pressedmode = -1;
                    else
                        scb->pressedmode = 0;
                } else {
                    scb->pressed_pos = x;
                    if (x < scb->liftx)
                        scb->pressedmode = 1;
                    else if (x >= scb->liftx+scb->liftw)
                        scb->pressedmode = -1;
                    else
                        scb->pressedmode = 0;
                }
                send_event(scb, 1, 0);
                return 1;
            }
        } else if (ev->p1 == GRMOUSE_LB_RELEASED) {
            if (scb->pressed) {
                scb->pressed = 0;
                return 1;
            }
        }
    } else if (ev->type == GREV_MMOVE) {
        if (ev->p1 == GRMOUSE_LB_PRESSED) {
            if (scb->pressed && (scb->pressedmode == 0)) {
                if (scb->type == GUI_TSB_VERTICAL)
                    send_event(scb, 0, ev->p3 - scb->dy);
                else
                    send_event(scb, 0, ev->p2 - scb->dx);
                return 1;
            }
        }
    } else if (ev->type == GREV_NULL) {
        if (scb->pressed && (scb->pressedmode != 0) &&
            (ev->time > scb->lasttimesend + scb->intv)) {
            send_event(scb, 0, 0);
            scb->lasttimesend = ev->time;
            scb->intv = 50;
            return 1;
        }
    }
    
    return 0;
}

/***************************/

static void send_event(GUIScrollbar *scb, int first, int pos)
{
    GrEvent ev;
    int liftlen;
    int dime, dist, value;

    ev.type = (scb->type == GUI_TSB_VERTICAL) ? GREV_SCBVERT : GREV_SCBHORZ;
    ev.p1 = ev.p3 = ev.p4 = 0;
    ev.p2 = scb->aid;

    liftlen = scb->endvalue - scb->inivalue;

    if (scb->pressedmode == 1) { // up
        if (first) scb->lastvaluesend = scb->inivalue;
        scb->lastvaluesend -= liftlen;
        if (scb->lastvaluesend < scb->minvalue) scb->lastvaluesend = scb->minvalue;
        ev.p1 = scb->lastvaluesend;
        GrEventEnqueue(&ev);
    } else if (scb->pressedmode == -1) { // down
        if (first) scb->lastvaluesend = scb->inivalue;
        scb->lastvaluesend += liftlen;
        if (scb->lastvaluesend > (scb->maxvalue - liftlen))
            scb->lastvaluesend = scb->maxvalue - liftlen;
        ev.p1 = scb->lastvaluesend;
        GrEventEnqueue(&ev);
    } else { // move
        if (first) {
            scb->lastvaluesend = scb->inivalue;
        } else {
            dime = (scb->type == GUI_TSB_VERTICAL) ? scb->height : scb->width;
            dist = (pos - scb->pressed_pos) *
                   (scb->maxvalue - scb->minvalue) / dime;
            value = scb->lastvaluesend + dist;
            if (value < scb->minvalue) value = scb->minvalue;
            if (value > (scb->maxvalue - liftlen)) value = scb->maxvalue - liftlen;
            ev.p1 = value;
            GrEventEnqueue(&ev);
        }
    }
}
