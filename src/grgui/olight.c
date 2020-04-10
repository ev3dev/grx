/**
 ** olight.c ---- Mini GUI for MGRX, specific funtions for lights
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

/***************************/

void GUIObjectSetLight(GUIObject *o, int id, int x, int y, int width, int height,
                       GrColor bg, GrColor fg, void *text, int on)
{
    o->type = GUIOBJTYPE_LIGHT;
    o->id = id;
    o->x = x;
    o->y = y;
    o->width = width;
    o->height = height;
    o->bg = bg;
    o->fg = fg;
    o->text = text;
    o->on = on;
    
    o->cid = 0;
    o->sgid = 0;
    o->maxlen = 0;
    o->pressed = -1;
    o->selected = -1;
    o->data = NULL;
    o->visible = 1;
}

/***************************/

void _GUIOLightPaint(GUIObject *o, int dx, int dy)
{
    GrColor c1, c2;
    int x, y;
    
    x = o->x + dx;
    y = o->y + dy;
    
    if (o->on) {
        c1 = o->bg;
        c2 = o->fg;
    } else {
        c1 = _objectshcolor1;
        c2 = _objectshcolor2;
    }

    GrBox(x, y, x+o->width-1, y+o->height-1, _objectlcolor);
    GrBox(x+1, y+1, x+o->width-2, y+o->height-2, c1);
    GrBox(x+2, y+2, x+o->width-3, y+o->height-3, _objectlcolor);
    _objectbutopt.txo_fgcolor = c2;
    GrFilledBox(x+3, y+3, x+o->width-4, y+o->height-4, c1);
    GrDrawString(o->text, 0, x+o->width/2, y+o->height/2, &_objectbutopt);
}

/***************************/

void _GUIOLightRePaint(GUIGroup *g, GUIObject *o)
{
    _GUIOLightPaint(o, g->x, g->y);
    GUIDBCurCtxBltRectToScreen(g->x+o->x, g->y+o->y, g->x+o->x+o->width-1,
                             g->y+o->y+o->height-1);
}

/***************************/

void _GUIOLightSetOn(GUIGroup *g, GUIObject *o, int paint)
{
    o->on ^= 1;
    if (paint) _GUIOLightRePaint(g, o);
}

/***************************/

void _GUIOLightSetText(GUIObject *o, void *newtext)
{
    o->text = newtext;
}
