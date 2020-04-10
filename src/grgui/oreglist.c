/**
 ** oreglist.c ---- Mini GUI for MGRX, specific funtions for reg lists
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
    GUIRDL2Intf f;          // interfaz data
    int sreg;               // selected reg
    GUIRawDataList2 *rdl2;  // raw data list2
} RegListData;

/***************************/

void GUIObjectSetRegList(GUIObject *o, int id, int x, int y, int width, int height,
                         GrColor bg, GrColor fg, int nreg, int regheight, int sreg,
                         void (*paint_reg)(int x, int y, int ind, int selected,
                         GrColor bg, GrColor fg))
{
    RegListData *data;
    
    o->type = GUIOBJTYPE_REGLIST;
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

    data = malloc(sizeof(RegListData));
    if (data == NULL) goto error;

    data->rdl2 = NULL;
    data->f.type = GUIRDLTYPE_SIMPLE;
    data->f.nreg = nreg;
    data->f.x = o->x;
    data->f.y = o->y;
    data->f.width = o->width;
    data->f.height = o->height;
    data->f.bg = o->bg;
    data->f.fg = o->fg;
    data->f.regheight = regheight;
    data->f.paint_reg = paint_reg;
    
    data->sreg = sreg;
    if (data->sreg > data->f.nreg) data->sreg = 0;
    data->rdl2 = GUIRDL2Create(&(data->f), data->sreg);
    if (data->rdl2 == NULL) goto error;

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

void _GUIORegListDestroy(GUIObject *o)
{
    RegListData *data;
    
    data = (RegListData *)(o->data);

    GUIRDL2Destroy(data->rdl2);
    free(data);
}

/***************************/

void _GUIORegListPaint(GUIObject *o, int dx, int dy)
{
    RegListData *data;
    int mouseblock;
    int x, y;
    
    data = (RegListData *)(o->data);

    x = o->x + dx;
    y = o->y + dy;

    mouseblock = GrMouseBlock(NULL, x, y, x+o->width-1, y+o->height-1);

    GUIRDL2SetSelected(data->rdl2, o->selected, 0, 0);
    GUIRDL2Paint(data->rdl2, dx, dy, 0);

    GrMouseUnBlock(mouseblock);
}

/***************************/

void _GUIORegListRePaint(GUIGroup *g, GUIObject *o)
{
    int x, y;

    x = o->x + g->x;
    y = o->y + g->y;

    _GUIORegListPaint(o, g->x, g->y);
    GUIDBCurCtxBltRectToScreen(x, y, x+o->width-1, y+o->height-1);
}

/***************************/

int _GUIORegListProcessEvent(GUIGroup *g, GUIObject *o, GrEvent *ev)
{
    RegListData *data;
    int ret;
    
    data = (RegListData *)(o->data);

    if (ev->type == GREV_MOUSE) {
        if (ev->p1 == GRMOUSE_LB_PRESSED) {
            if (!o->selected) {
                o->selected = 1;
                _GUIORegListRePaint(g, o);
            }
            o->pressed = 1;
        } else if (ev->p1 == GRMOUSE_LB_RELEASED) {
            o->pressed = 0;
        }
    }

    ret = GUIRD2LProcessEvent(data->rdl2, ev);
    if (ret == 2) {
        data->sreg = GUIRDL2GetSreg(data->rdl2);
        GrEventParEnqueue(GREV_FCHANGE, o->id, data->sreg, 0, 0);
        ret = 1;
    }
    
    return ret;
}
