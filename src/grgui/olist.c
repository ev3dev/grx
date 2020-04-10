/**
 ** olist.c ---- Mini GUI for MGRX, specific funtions for lists
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

#define SELECTWIDTH 20

typedef struct {
    void **reg;           // string list
    int nreg;             // num of strings in list
    int vreg;             // max num of strings visible
    int sreg;             // selected reg
    GUIRawDataList *rdl;  // raw data list
    int sx, sy;           // rdl screen position
    int height;           // rdl height
} ListData;

static int ListRun(GUIObject *o);

/***************************/

void GUIObjectSetList(GUIObject *o, int id, int x, int y, int width, int height,
                      GrColor bg, GrColor fg, void **reg, int nreg, int vreg, int sreg)
{
    ListData *data;
    
    o->type = GUIOBJTYPE_LIST;
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

    data = malloc(sizeof(ListData));
    if (data == NULL) goto error;

    data->rdl = NULL;
    data->reg = reg;
    data->nreg = nreg;
    data->vreg = vreg;
    if (data->vreg > data->nreg) data->vreg = data->nreg;
    data->sreg = sreg;
    if (data->sreg > data->nreg) data->sreg = 0;
    data->sx = 0; // will be set later
    data->sy = 0; // idem
    data->height = data->vreg * _objectfont->h.height + 6;
    data->rdl = GUIRDLCreate(GUIRDLTYPE_SIMPLE, 0, 0, o->width, data->height,
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

void _GUIOListDestroy(GUIObject *o)
{
    ListData *data;
    
    data = (ListData *)(o->data);

    GUIRDLDestroy(data->rdl);
    free(data);
}

/***************************/

void _GUIOListPaint(GUIObject *o, int dx, int dy)
{
    ListData *data;
    GrLineOption glo;
    int midx, midy, tri[3][2];
    int mouseblock;
    int x, y;
    
    data = (ListData *)(o->data);
    x = o->x + dx;
    y = o->y + dy;

    mouseblock = GrMouseBlock(NULL, x, y, x+o->width-1, y+o->height-1);

    GrBox(x, y, x+o->width-1-SELECTWIDTH, y+o->height-1, _objectlcolor);
    GrFilledBox(x+1, y+1, x+o->width-2-SELECTWIDTH, y+o->height-2, o->bg);
    
    midx = x + o->width - (SELECTWIDTH + 3) / 2;
    midy = y + o->height / 2;
    tri[0][0] = midx - 6;
    tri[0][1] = midy - 5;
    tri[1][0] = midx + 6;
    tri[1][1] = midy - 5;
    tri[2][0] = midx;
    tri[2][1] = midy + 5;

    GrBox(x+o->width-1-SELECTWIDTH, y, x+o->width-1,
          y+o->height-1, _objectlcolor);

    if (o->pressed) {
        GrFilledBox(x+o->width-SELECTWIDTH, y+1, x+o->width-2,
                    y+o->height-2, _objectshcolor2);
        GrFilledPolygon(3, tri, _objectshcolor1);
        GrPolygon(3, tri, _objectlcolor);
    } else {
        GrFilledBox(x+o->width-SELECTWIDTH, y+1, x+o->width-2,
                    y+o->height-2, _objectshcolor1);
        GrFilledPolygon(3, tri, _objectshcolor2);
        GrPolygon(3, tri, _objectlcolor);
    }
    
    if (o->selected) {
        glo.lno_color = o->fg;
        glo.lno_width = 1;
        glo.lno_pattlen = 2;
        glo.lno_dashpat = (unsigned char *)"\2\1";
        GrCustomBox(x+2, y+2, x+o->width-3-SELECTWIDTH, y+o->height-3, &glo);
    }

    GrSetClipBox(x+3, y+1, x+o->width-4-SELECTWIDTH, y+o->height-2);
    //printf("%d %s\n", data->sreg, (char *)data->reg[data->sreg]);
    _objectgenopt.txo_fgcolor = o->fg;
    GrDrawString(data->reg[data->sreg], 0, x+3, y+o->height/2, &_objectgenopt);
    GrResetClipBox();
 
    GrMouseUnBlock(mouseblock);
}

/***************************/

void _GUIOListRePaint(GUIGroup *g, GUIObject *o)
{
    _GUIOListPaint(o, g->x, g->y);
    GUIDBCurCtxBltRectToScreen(g->x+o->x, g->y+o->y, g->x+o->x+o->width-1,
                             g->y+o->y+o->height-1);
}

/***************************/

int _GUIOListProcessEvent(GUIGroup *g, GUIObject *o, GrEvent *ev)
{
    ListData *data;
    GrContext *gc;
    
    data = (ListData *)(o->data);

    if (data->sx == 0) { // need to calculate real sx, sy for the list
        data->sx = g->x + o->x;
        data->sy = g->y + o->y + o->height;
        gc = GrCurrentContext();
        data->sx += gc->gc_xoffset;
        data->sy += gc->gc_yoffset;
        if (data->sy + data->height >= GrScreenY())
            data->sy = g->y + o->y - data->height + gc->gc_yoffset;
    }
    
    if (ev->type == GREV_MOUSE) {
        if (ev->p1 == GRMOUSE_LB_PRESSED) {
            o->selected = 1;
            if (GrCheckCoordInto(ev->p2, ev->p3, g->x+o->x+o->width-1-SELECTWIDTH,
                g->y+o->y, SELECTWIDTH, o->height)) {
                o->pressed = 1;
            }
            _GUIOListRePaint(g, o);
            return 1;
        } else if (ev->p1 == GRMOUSE_LB_RELEASED) {
            o->pressed = 0;
            if (GrCheckCoordInto(ev->p2, ev->p3, g->x+o->x+o->width-1-SELECTWIDTH,
                g->y+o->y, SELECTWIDTH, o->height)) {
                _GUIOListRePaint(g, o);
                if (ListRun(o) == 2) {
                    _GUIOListRePaint(g, o);
                    GrEventParEnqueue(GREV_FCHANGE, o->id, data->sreg, 0, 0);
                }
            } else {
                _GUIOListRePaint(g, o);
            }
            return 1;
        }
    } else if (ev->type == GREV_KEY) {
        if (ev->p1 == GrKey_Return) {
            o->pressed = 1;
            _GUIOListRePaint(g, o);
            GrSleep(100);
            o->pressed = 0;
            _GUIOListRePaint(g, o);
            if (ListRun(o) == 2) {
                _GUIOListRePaint(g, o);
                GrEventParEnqueue(GREV_FCHANGE, o->id, data->sreg, 0, 0);
            }
            return 1;
        }
    }
    
    return 0;
}

/***************************/

static int ListRun(GUIObject *o)
{
// returns 2=changed, else=no changed
    GUIContext *gctx;
    GrContext grcaux;
    GrEvent ev;
    ListData *data;
    int ret;
    
    data = (ListData *)(o->data);

    gctx = GUIContextCreate(data->sx, data->sy, o->width, data->height, 1);
    if (gctx == NULL) return -1;

    _GUISuspendHooks();
    GrSaveContext(&grcaux);
    GUIContextSaveUnder(gctx);
    GrSetContext(gctx->c);
    
    GUIRDLSetSreg(data->rdl, data->sreg);
    GUIRDLPaint(data->rdl, 0, 0, 1);

    while (1) {
        GrEventWait(&ev);

        if ((ev.type == GREV_KEY) && (ev.p1 == GrKey_Escape)) {
            ret = -1;
            break;
        }
        if (ev.type == GREV_WMEND) {
            ret = -1;
            break;
        }
        if ((ev.type == GREV_MOUSE) && (ev.p1 == GRMOUSE_LB_PRESSED)) {
            if (!GrCheckCoordInto(ev.p2, ev.p3, data->sx, data->sy,
                o->width, data->height)) {
                GrEventEnqueueFirst(&ev);
                ret = -1;
                break;
            }
        }
        GUIContextTransMouseEv(gctx, &ev);
        ret = GUIRDLProcessEvent(data->rdl, &ev);
        if (ret == 2) break;
    }

    GUIContextRestoreUnder(gctx);
    GUIContextDestroy(gctx);
    GrSetContext(&grcaux);
    _GUIRestartHooks();
    
    if (ret == 2) {
        data->sreg = GUIRDLGetSreg(data->rdl);
    }

    return ret;
}
