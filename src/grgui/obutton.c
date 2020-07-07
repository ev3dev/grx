/**
 ** obutton.c ---- Mini GUI for MGRX, specific funtions for buttons
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grguip.h"

static void adjust_sgid(GUIGroup *g, int sgid, int id, int paint);
static void dboton(int x, int y, int an, int al, GrColor c, GrColor ct,
                   void *s, int sgid, int selec, int pulsd, int on);

/***************************/

void GUIObjectSetButton(GUIObject *o, int id, int x, int y, int width, int height,
                        GrColor bg, GrColor fg, void *text, int cid, int sgid, int on)
{
    o->type = GUIOBJTYPE_BUTTON;
    o->id = id;
    o->x = x;
    o->y = y;
    o->width = width;
    o->height = height;
    o->bg = bg;
    o->fg = fg;
    o->text = text;
    o->cid = cid;
    o->sgid = sgid;
    o->on = on;
    
    o->maxlen = 0;
    o->pressed = 0;
    o->selected = 0;
    o->data = NULL;
    o->visible = 1;
}

/***************************/

void _GUIOButtonPaint(GUIObject *o, int dx, int dy)
{
    dboton(dx+o->x, dy+o->y, o->width, o->height, o->bg, o->fg,
           o->text, o->sgid, o->selected, o->pressed, o->on);
}

/***************************/

void _GUIOButtonRePaint(GUIGroup *g, GUIObject *o)
{
    dboton(g->x+o->x, g->y+o->y, o->width, o->height, o->bg, o->fg,
           o->text, o->sgid, o->selected, o->pressed, o->on);
    GUIDBCurCtxBltRectToScreen(g->x+o->x, g->y+o->y, g->x+o->x+o->width-1,
                             g->y+o->y+o->height-1);
}

/***************************/

int _GUIOButtonProcessEvent(GUIGroup *g, GUIObject *o, GrEvent *ev)
{
    if (ev->type == GREV_MOUSE) {
        if (ev->p1 == GRMOUSE_LB_PRESSED) {
            o->selected = 1;
            o->pressed = 1;
            GrEventParEnqueue(GREV_SELECT, o->cid, o->id, 0, 0);
            _GUIOButtonRePaint(g, o);
            return 1;
        } else if (ev->p1 == GRMOUSE_LB_RELEASED) {
            o->pressed = 0;
            if (GrCheckCoordInto(ev->p2, ev->p3, g->x+o->x, g->y+o->y,
                o->width, o->height)) {
                GrEventParEnqueue(GREV_COMMAND, o->cid, o->id, 0, 0 );
                if (o->sgid == 1) {
                    o->on ^= 1;
                } else if (o->sgid > 100) {
                    o->on = 1;
                    adjust_sgid(g, o->sgid, o->id, 1);
                }
            }
            _GUIOButtonRePaint(g, o);
            return 1;
        }
    } else if (ev->type == GREV_KEY) {
        if (ev->p1 == GrKey_Return) {
            o->pressed = 1;
            _GUIOButtonRePaint(g, o);
            GrSleep(100);
            if (o->sgid == 1) {
                o->on ^= 1;
            } else if (o->sgid > 100) {
                o->on = 1;
                adjust_sgid(g, o->sgid, o->id, 1);
            }
            o->pressed = 0;
            GrEventParEnqueue(GREV_COMMAND, o->cid, o->id, 0, 0 );
            _GUIOButtonRePaint(g, o);
            return 1;
        }
    }
        
    return 0;
}

/***************************/

void _GUIOButtonSetOn(GUIGroup *g, GUIObject *o, int paint)
{
    if (o->sgid == 1) {
        o->on ^= 1;
    } else if (o->sgid > 100) {
        o->on = 1;
        adjust_sgid(g, o->sgid, o->id, paint);
    }
    if (paint) _GUIOButtonRePaint(g, o);
}

/***************************/

void _GUIOButtonSetText(GUIObject *o, void *newtext)
{
    o->text = newtext;
}

/***************************/

static void adjust_sgid(GUIGroup *g, int sgid, int id, int paint)
{
    int i;

    for (i=0; i<g->nobj; i++) {
        if (g->o[i].id != id && g->o[i].sgid == sgid) {
            if (g->o[i].on) {
                g->o[i].on = 0;
                if (paint) _GUIOButtonRePaint(g, &(g->o[i]));
            }
        }
    }
}

/***************************/

static void dboton(int x, int y, int an, int al, GrColor c, GrColor ct,
                   void *s, int sgid, int selec, int pulsd, int on)
{
    int pol[7][2], prof;
    GrLineOption glo;
    int mouseblock;
    GrColor caux, ctdim;
    long lctdim;
    int iaux;

    prof = (pulsd) ? 2 : 4;

    mouseblock = GrMouseBlock(NULL, x, y, x+an-1, y+al-1);
    GrBox(x, y, x+an-1, y+al-1, _objectlcolor);
    x = x + 1; y = y + 1;
    an = an - 2; al = al - 2;

    pol[0][0] = x;                 pol[0][1] = y;
    pol[1][0] = x + an - 1;        pol[1][1] = y;
    pol[2][0] = x + an - 2 - prof; pol[2][1] = y + 1 + prof;
    pol[3][0] = x + 1 + prof;      pol[3][1] = y + 1 + prof;
    pol[4][0] = x + 1 + prof;      pol[4][1] = y + al - 2 - prof;
    pol[5][0] = x;                 pol[5][1] = y + al - 1;
    pol[6][0] = pol[0][0];         pol[6][1] = pol[0][1];
    GrFilledPolygon(7, pol, pulsd ? _objectshcolor2 : _objectshcolor1);
    GrPolygon(7, pol, _objectlcolor);
    GrLine(pol[0][0], pol[0][1], pol[3][0], pol[3][1], _objectlcolor);
    pol[0][0] = x + an - 1;        pol[0][1] = y + al - 1;
    pol[3][0] = x + an - 2 - prof; pol[3][1] = y + al - 2 - prof;
    pol[6][0] = pol[0][0];         pol[6][1] = pol[0][1];
    GrFilledPolygon(7, pol, pulsd ? _objectshcolor1 : _objectshcolor2);
    GrPolygon(7, pol, _objectlcolor);
    GrLine(pol[0][0], pol[0][1], pol[3][0], pol[3][1], _objectlcolor);
    GrFilledBox(x+2+prof, y+2+prof, x+an-3-prof, y+al-3-prof, c);

    if (sgid > 0) {
        caux = on ? ct : _objectshcolor2;
        iaux = pulsd ? 1 : 0;
        GrLine(x+12+iaux, y+al-9+iaux, x+an-13+iaux, y+al-9+iaux, caux);
        GrLine(x+11+iaux, y+al-10+iaux, x+an-12+iaux, y+al-10+iaux, caux);
        GrLine(x+12+iaux, y+al-11+iaux, x+an-13+iaux, y+al-11+iaux, caux);
    }

    _objectbutopt.txo_fgcolor = ct;
    if (sgid > 100 && !on) {
        if (GrCurrentContext()->gc_driver->bits_per_pixel > 8) {
            GrQueryColor2(ct, &lctdim);
            lctdim &= 0xCFCFCF;
            ctdim = GrAllocColor2(lctdim);
            _objectbutopt.txo_fgcolor = ctdim;
        }
    }
    iaux = pulsd ? 1 : 0;
    GrDrawString(s, 0, x+an/2+iaux, y+al/2+iaux, &_objectbutopt);

    if (selec) {
        glo.lno_color = ct;
        glo.lno_width = 1;
        glo.lno_pattlen = 2;
        glo.lno_dashpat = (unsigned char *)"\2\1";
//      GrCustomBox(x+8, y+al/2-6, x+an-9, y+al/2+5, &glo);
        GrCustomBox(x+2+prof, y+2+prof, x+an-3-prof, y+al-3-prof, &glo);
    }

    GrMouseUnBlock(mouseblock);
}
