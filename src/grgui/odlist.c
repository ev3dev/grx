/**
 ** odlist.c ---- Mini GUI for MGRX, specific funtions for deployed lists
 **
 ** Copyright (C) 2020 Mariano Alvarez Fernandez
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
    void **reg;           // string list
    int nreg;             // num of strings in list
    int sreg;             // selected reg
    GUIRawDataList *rdl;  // raw data list
} DListData;

/***************************/

void GUIObjectSetDList(GUIObject *o, int id, int x, int y, int width, int height,
                       GrColor bg, GrColor fg, void **reg, int nreg, int sreg)
{
    DListData *data;
    
    o->type = GUIOBJTYPE_DLIST;
    o->id = id;
    o->x = x;
    o->y = y;
    o->width = width;
    o->height = height;
    o->bg = bg;
    o->fg = fg;

    o->maxlen = 0;
    o->text = NULL;
    o->cid = 0;
    o->sgid = 0;
    o->on = 0;
    o->pressed = 0;
    o->selected = 0;
    o->visible = 1;

    data = malloc(sizeof(DListData));
    if (data == NULL) goto error;

    data->rdl = NULL;
    data->reg = reg;
    data->nreg = nreg;
    data->sreg = sreg;
    if (data->sreg > data->nreg) data->sreg = 0;
    data->rdl = GUIRDLCreate(GUIRDLTYPE_SIMPLE, o->x, o->y, o->width, o->height,
                             o->bg, o->fg, reg, data->nreg, data->sreg);
    if (data->rdl == NULL) goto error;

    o->data = (void *)data;
    return;

error:
    if (data) free(data);
    o->type = GUIOBJTYPE_NONE;
    o->pressed = -1;
    o->selected = -1;
    o->visible = 0;
}

/***************************/

void _GUIODListDestroy(GUIObject *o)
{
    DListData *data;
    
    data = (DListData *)(o->data);

    GUIRDLDestroy(data->rdl);
    free(data);
}

/***************************/

void _GUIODListPaint(GUIObject *o, int dx, int dy)
{
    DListData *data;
    int mouseblock;
    int x, y;
    
    data = (DListData *)(o->data);

    x = o->x + dx;
    y = o->y + dy;

    mouseblock = GrMouseBlock(NULL, x, y, x+o->width-1, y+o->height-1);

    GUIRDLSetSelected(data->rdl, o->selected, 0, 0);
    GUIRDLPaint(data->rdl, dx, dy, 0);

    GrMouseUnBlock(mouseblock);
}

/***************************/

void _GUIODListRePaint(GUIGroup *g, GUIObject *o)
{
    int x, y;

    x = o->x + g->x;
    y = o->y + g->y;

    _GUIODListPaint(o, g->x, g->y);
    GUIDBCurCtxBltRectToScreen(x, y, x+o->width-1, y+o->height-1);
}

/***************************/

int _GUIODListProcessEvent(GUIGroup *g, GUIObject *o, GrEvent *ev)
{
    DListData *data;
    int ret;
    
    data = (DListData *)(o->data);

    if (ev->type == GREV_MOUSE) {
        if (ev->p1 == GRMOUSE_LB_PRESSED) {
            if (!o->selected) {
                o->selected = 1;
                _GUIODListRePaint(g, o);
            }
            o->pressed = 1;
        } else if (ev->p1 == GRMOUSE_LB_RELEASED) {
            o->pressed = 0;
        }
    }

    ret = GUIRDLProcessEvent(data->rdl, ev);
    if (ret == 2) {
        data->sreg = GUIRDLGetSreg(data->rdl);
        GrEventParEnqueue(GREV_FCHANGE, o->id, data->sreg, 0, 0);
        ret = 1;
    }
    
    return ret;
}
