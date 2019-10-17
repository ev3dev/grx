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
}

/***************************/

void _GUIOLightPaint(GUIObject *o, int x, int y)
{
    GrColor c1, c2;
    
    if (o->on) {
        c1 = o->bg;
        c2 = o->fg;
    } else {
        c1 = _objectshcolor1;
        c2 = _objectshcolor2;
    }

    GrBox(x+o->x, y+o->y, x+o->x+o->width-1, y+o->y+o->height-1, _objectlcolor);
    GrBox(x+o->x+1, y+o->y+1, x+o->x+o->width-2, y+o->y+o->height-2, c1);
    GrBox(x+o->x+2, y+o->y+2, x+o->x+o->width-3, y+o->y+o->height-3, _objectlcolor);
    _objectbutopt.txo_fgcolor = c2;
    GrFilledBox(x+o->x+3, y+o->y+3, x+o->x+o->width-4, y+o->y+o->height-4, c1);
    GrDrawString(o->text, 0, x+o->x+o->width/2, y+o->y+o->height/2, &_objectbutopt);
}

/***************************/

void _GUIOLightRePaint(GUIGroup *g, GUIObject *o)
{
    _GUIOLightPaint(o, g->x, g->y);
    if (g->p) {
        GUIPanelBltRectClToScreen(g->p, g->x+o->x, g->y+o->y,
                                  o->width, o->height);
    }
}

/***************************/

void _GUIOLightSetOn(GUIGroup *g, GUIObject *o, int paint)
{
    o->on ^= 1;
    if (paint) _GUIOLightRePaint(g, o);
}
