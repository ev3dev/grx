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

typedef struct {
    void **text;          // string array
    int nlin;             // num of lines
    char xalign;          // txo_xalign
    GrFont *f;            // if NULL use the object font
} TextData;

/***************************/

void GUIObjectSetText(GUIObject *o, int id, int x, int y, int width, int height,
                      GrColor bg, GrColor fg, void **text, int nlin, char xalign,
                      GrFont *f)
{
    TextData *data;

    o->type = GUIOBJTYPE_TEXT;
    o->id = id;
    o->x = x;
    o->y = y;
    o->width = width;
    o->height = height;
    o->bg = bg;
    o->fg = fg;

    o->text = NULL;
    o->cid = 0;
    o->sgid = 0;
    o->on = 0;
    o->maxlen = 0;
    o->pressed = -1;
    o->selected = -1;

    data = malloc(sizeof(TextData));
    if (data == NULL) {
        o->type = GUIOBJTYPE_NONE;
        return;
    }
    
    data->text = text;
    data->nlin = nlin;
    data->xalign = xalign;
    data->f = f;
    o->data = (void *)data;
}

/***************************/

void _GUIOTextDestroy(GUIObject *o)
{
    TextData *data;
    
    data = (TextData *)(o->data);

    free(data);
}

/***************************/

void _GUIOTextPaint(GUIObject *o, int x, int y)
{
    GrTextOption grt;
    TextData *data;
    int i, h;

    data = (TextData *)(o->data);
    grt = _objectgenopt;
    grt.txo_xalign = data->xalign;
    grt.txo_yalign = GR_ALIGN_TOP;
    grt.txo_fgcolor = o->fg;
    if (data->f) grt.txo_font = data->f;
    h = grt.txo_font->h.height;

    x += o->x;
    y += o->y;
    
    GrFilledBox(x, y, x+o->width-1, y+o->height-1, o->bg);
    GrSetClipBox(x, y, x+o->width-1, y+o->height-1);

    if (data->xalign == GR_ALIGN_LEFT) x += 2;
    else if (data->xalign == GR_ALIGN_RIGHT) x += o->width - 3;
    else x += o->width / 2;
    y += 2;
    
    for (i=0; i<data->nlin; i++) {
        GrDrawString(data->text[i], 0, x, y, &grt);
        y += h;
    }

    GrResetClipBox();
}
