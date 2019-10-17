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
    int freg;             // first reg in screen
    int areg;             // actual reg (when selecting)
    int sx, sy;           // screen position
    int width, height;    // dimension
    int lineheight;       // height of a line
    unsigned short *c;    // char array
} ListData;

static int ListRun(GUIObject *o);
static void show_list_item(int ind, GUIObject *o);
static int proccess_list_event(GUIObject *o, GUIContext *gctx, GrEvent *ev);

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

    data = malloc(sizeof(ListData));
    if (data == NULL) {
        o->type = GUIOBJTYPE_NONE;
        o->pressed = -1;
        o->selected = -1;
        return;
    }

    data->reg = reg;
    data->nreg = nreg;
    data->vreg = vreg;
    if (data->vreg > data->nreg) data->vreg = data->nreg;
    data->sreg = sreg;
    if (data->sreg > data->nreg) data->sreg = 0;
    data->freg = 0;
    data->lineheight = _objectfont->h.height;
    data->sx = 0; // will be set later
    data->sy = 0; // idem
    data->width = width;
    data->height = vreg * data->lineheight + 4;
    o->data = (void *)data;
}

/***************************/

void _GUIOListDestroy(GUIObject *o)
{
    ListData *data;
    
    data = (ListData *)(o->data);

    free(data);
}

/***************************/
/*
void *_GUIOListGetText(GUIObject *o, int chrtype)
{
    EntryData *data;
    unsigned char *s;
    unsigned short *h;
    long l;
    unsigned char *cutf8;
    int i, j;
    
    data = (EntryData *)(o->data);
    
    switch (chrtype) {
        case GR_BYTE_TEXT:
        case GR_CP437_TEXT:
        case GR_CP850_TEXT:
        case GR_CP1252_TEXT:
        case GR_ISO_8859_1_TEXT:
            s = malloc(data->len+1);
            if (s == NULL) return NULL;
            for (i=0; i<data->len; i++)
                s[i] = GrFontCharReverseRecode(_objectfont, data->c[i], chrtype);
            s[data->len] = '\0';
            return s;
        case GR_UTF8_TEXT:
            s = malloc((data->len*4)+1);
            if (s == NULL) return NULL;
            j = 0;
            cutf8 = (unsigned char *)&l;
            for (i=0; i<data->len; i++) {
                l = GrFontCharReverseRecode(_objectfont, data->c[i], chrtype);
                s[j++] = cutf8[0];
                if (cutf8[1] != '\0') s[j++] = cutf8[1];
                if (cutf8[2] != '\0') s[j++] = cutf8[2];
                if (cutf8[3] != '\0') s[j++] = cutf8[3];
            }
            s[j] = '\0';
            return s;
        case GR_WORD_TEXT:
        case GR_UCS2_TEXT:
            h = malloc((data->len+1)*sizeof(unsigned short));
            if (h == NULL) return NULL;
            for (i=0; i<data->len; i++)
                h[i] = GrFontCharReverseRecode(_objectfont, data->c[i], chrtype);
            h[data->len] = '\0';
            return h;
    }

    return NULL;
}
*/
/***************************/

void _GUIOListPaint(GUIObject *o, int x, int y)
{
    ListData *data;
    GrLineOption glo;
    int midx, midy, tri[3][2];
    int mouseblock;
    
    data = (ListData *)(o->data);

    mouseblock = GrMouseBlock(NULL, x+o->x, y+o->y, x+o->x+o->width-1,
                              y+o->y+o->height-1);

    GrBox(x+o->x, y+o->y, x+o->x+o->width-1-SELECTWIDTH,
          y+o->y+o->height-1, _objectlcolor);
    GrFilledBox(x+o->x+1, y+o->y+1, x+o->x+o->width-2-SELECTWIDTH,
                y+o->y+o->height-2, o->bg);
    
    midx = x + o->x + o->width - (SELECTWIDTH + 3) / 2;
    midy = y + o->y + o->height / 2;
    tri[0][0] = midx - 6;
    tri[0][1] = midy - 5;
    tri[1][0] = midx + 6;
    tri[1][1] = midy - 5;
    tri[2][0] = midx;
    tri[2][1] = midy + 5;

    GrBox(x+o->x+o->width-1-SELECTWIDTH, y+o->y, x+o->x+o->width-1,
          y+o->y+o->height-1, _objectlcolor);

    if (o->pressed) {
        GrFilledBox(x+o->x+o->width-SELECTWIDTH, y+o->y+1, x+o->x+o->width-2,
                    y+o->y+o->height-2, _objectshcolor2);
        GrFilledPolygon(3, tri, _objectshcolor1);
        GrPolygon(3, tri, _objectlcolor);
    } else {
        GrFilledBox(x+o->x+o->width-SELECTWIDTH, y+o->y+1, x+o->x+o->width-2,
                    y+o->y+o->height-2, _objectshcolor1);
        GrFilledPolygon(3, tri, _objectshcolor2);
        GrPolygon(3, tri, _objectlcolor);
    }
    
    if (o->selected) {
        glo.lno_color = o->fg;
        glo.lno_width = 1;
        glo.lno_pattlen = 2;
        glo.lno_dashpat = (unsigned char *)"\2\1";
        GrCustomBox(x+o->x+2, y+o->y+2, x+o->x+o->width-3-SELECTWIDTH,
                    y+o->y+o->height-3, &glo);
    }

    GrSetClipBox(x+o->x+3, y+o->y+1, x+o->x+o->width-4-SELECTWIDTH,
                 y+o->y+o->height-2);
    //printf("%d %s\n", data->sreg, (char *)data->reg[data->sreg]);
    _objectgenopt.txo_fgcolor = o->fg;
    GrDrawString(data->reg[data->sreg], 0, x+o->x+3, y+o->y+o->height/2,
                     &_objectgenopt);
    GrResetClipBox();
 
    GrMouseUnBlock(mouseblock);
}

/***************************/

void _GUIOListRePaint(GUIGroup *g, GUIObject *o)
{
    _GUIOListPaint(o, g->x, g->y);
    if (g->p) {
        GUIPanelBltRectClToScreen(g->p, g->x+o->x, g->y+o->y,
                                  o->width, o->height);
    }
}

/***************************/

int _GUIOListProcessEvent(GUIGroup *g, GUIObject *o, GrEvent *ev)
{
    ListData *data;
    GrContext *gc;
    
    data = (ListData *)(o->data);

    if (data->sx == 0) { // calculate real sx, sy for the list
        data->sx = g->x + o->x;
        data->sy = g->y + o->y + o->height;
        if (g->p) { // can be double buffered
            data->sx += g->p->xcl;
            data->sy += g->p->ycl;
            if (data->sy + data->height >= GrScreenY())
                data->sy = g->y + o->y - data->height + g->p->ycl;
        } else {  // assume not
            gc = GrCurrentContext();
            data->sx += gc->gc_xoffset;
            data->sy += gc->gc_yoffset;
            if (data->sy + data->height >= GrScreenY())
                data->sy = g->y + o->y - data->height + gc->gc_yoffset;
        }
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
                if (ListRun(o) == 1) {
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
            if (ListRun(o) == 1) {
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
// returns 1=changed, else=no changed
    GUIContext *gctx;
    GrContext grcaux;
    GrEvent ev;
    ListData *data;
    int i ,ret;
    
    data = (ListData *)(o->data);

    //gc = GrCurrentContext();
    gctx = GUIContextCreate(data->sx, data->sy, data->width, data->height, 1);
    if (gctx == NULL) return -1;

    _GUISuspendHooks();
    GrSaveContext(&grcaux);
    GUIContextSaveUnder(gctx);
    GrSetContext(gctx->c);
    GrClearContext(o->bg);
    GrBox(0, 0, data->width-1, data->height-1, _objectlcolor);

    data->areg = data->sreg;
    data->freg = 0;
    while (data->areg-data->freg >= data->vreg) data->freg++; // change to calc

    for (i=0; i<data->vreg; i++)
        show_list_item(i, o);
    GUIContextBltToScreen(gctx);

    while (1) {
        GrEventWait(&ev);
        ret = proccess_list_event(o, gctx, &ev);
        if (ret != 0) break;
    }
    
    GUIContextRestoreUnder(gctx);
    GUIContextDestroy(gctx);
    GrSetContext(&grcaux);
    _GUIRestartHooks();
    
    if (ret == 1) {
        if (data->sreg != data->areg)
            data->sreg = data->areg;
        else
            ret = -1;
    }

    return ret;
}

/***************************/

static void show_list_item(int ind, GUIObject *o)
{
    ListData *data;
    int x, y, selected = 0;

    data = (ListData *)(o->data);

    if (ind >= data->vreg) return;
    
    if (ind+data->freg == data->areg) selected = 1;
    x = 2;
    y = 2 + ind * data->lineheight;

    GrFilledBox(x, y, x+data->width-5, y+data->lineheight-1,
                selected ? o->fg : o->bg);

    GrSetClipBox(x+1, y, x+data->width-6, y+data->lineheight-1);
    _objectgenopt.txo_fgcolor = selected ? o->bg : o->fg;
    GrDrawString(data->reg[ind+data->freg], 0, x+1, y+data->lineheight/2,
                     &_objectgenopt);
    GrResetClipBox();
}

/***************************/

static int proccess_list_event(GUIObject *o, GUIContext *gctx, GrEvent *ev)
// returns 1=select, 0=continue, -1=cancel
{
    ListData *data;
    GrEvent orgev;
    long c;
    int ind, old, pos;
    int change = 0;
    int move = 0;
    int res = 0;
    int j;

    data = (ListData *)(o->data);
    orgev = *ev;
    ind = data->areg - data->freg;

    switch (ev->type) {

        case GREV_MOUSE :
            c = ev->p1;
            switch (c) {
                case GRMOUSE_LB_PRESSED :
                    if (!GUIContextTransMouseEvIfInCtx(gctx, ev)) {
                        GrEventEnqueueFirst(&orgev);
                        return -1;
                    }
                    pos = ev->p3 / data->lineheight;
                    if ((pos >= 0) && (pos < data->vreg)) {
                        if (ind != pos) {
                            old = ind;
                            ind = pos;
                            change = 1;
                        }
                    }
                    break;
                case GRMOUSE_LB_RELEASED :
                    if (!GUIContextTransMouseEvIfInCtx(gctx, ev)) return 0;
                    pos = ev->p3 / data->lineheight;
                    if (pos == ind) return 1;
                    break;
                case GRMOUSE_B4_RELEASED :
                    GrEventParEnqueue(GREV_KEY, GrKey_Up, GRKEY_KEYCODE, 0, 0);
                    break;
                case GRMOUSE_B5_RELEASED :
                    GrEventParEnqueue(GREV_KEY, GrKey_Down, GRKEY_KEYCODE, 0, 0);
                    break;
            }
            break;

        case GREV_MMOVE :
            if (ev->p1 & GRMOUSE_LB_STATUS) {
                if (!GUIContextTransMouseEvIfInCtx(gctx, ev)) return 0;
                pos = ev->p3 / data->lineheight;
                if ((pos >= 0) && (pos < data->vreg)) {
                    if (ind != pos) {
                        old = ind;
                        ind = pos;
                        change = 1;
                    }
                }
            }
            break;

        case GREV_KEY :
            c = ev->p1;
            switch (c) {
                case GrKey_Up :
                    if (ind > 0) {
                        old = ind;
                        ind--;
                        change = 1;
                    } else if (data->freg > 0) {
                        data->freg--;
                        change = 1;
                        move = 1;
                    }
                    break;
                case GrKey_Down :
                    if (ind < data->vreg-1) {
                        old = ind;
                        ind++;
                        change = 1;
                    } else if (data->vreg+data->freg < data->nreg) {
                        data->freg++;
                        change = 1;
                        move = 1;
                    }
                    break;
                case GrKey_Escape :
                    return -1;
                case GrKey_Return :
                    return 1;
            }
            break;
    }

    if (change) {
        data->areg = ind + data->freg;
        if (move) {
            for(j=0; j<data->vreg; j++)
                show_list_item(j, o);
        }
        else {
            show_list_item(old, o);
            show_list_item(ind, o);
        }
        GUIContextBltToScreen(gctx);
    }

    return res;
}
