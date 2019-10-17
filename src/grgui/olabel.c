/**
 ** olabel.c ---- Mini GUI for MGRX, specific funtions for labels
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

void GUIObjectSetLabel(GUIObject *o, int id, int x, int y, int width, int height,
                       GrColor bg, GrColor fg, void *text)
{
    o->type = GUIOBJTYPE_LABEL;
    o->id = id;
    o->x = x;
    o->y = y;
    o->width = width;
    o->height = height;
    o->bg = bg;
    o->fg = fg;
    o->text = text;
    
    o->cid = 0;
    o->sgid = 0;
    o->on = 0;
    o->maxlen = 0;
    o->pressed = -1;
    o->selected = -1;
    o->data = NULL;
}

/***************************/

void _GUIOLabelPaint(GUIObject *o, int x, int y)
{
    _objectgenopt.txo_fgcolor = o->fg;
    GrFilledBox(x+o->x, y+o->y, x+o->x+o->width-1, y+o->y+o->height-1, o->bg);
    GrSetClipBox(x+o->x, y+o->y, x+o->x+o->width-1, y+o->y+o->height-1);
    GrDrawString(o->text, 0, x+o->x+2, y+o->y+o->height/2, &_objectgenopt);
    GrResetClipBox();
}
