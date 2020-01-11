/**
 ** tiles.c ---- Mini GUI for MGRX, panels
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
#include <string.h>
#include "grguip.h"

#define MAXTILES 100

GrColor _tileslcolor;
GrColor _tilesbcolor;
GrColor _tilessbcolor;

static GUITile *regtile[MAXTILES];
static int numtiles = 0;
static int selectedtile = -1;
static int savedselected = -1;

static int FindIdt(int idt);
static void PaintBorder(GUITile *t);
static void TileSelect(int nt);
static void TileSelectNext(int dir);

void _GUITilesInit(void)
{
    numtiles = 0;
    selectedtile = -1;
    _tileslcolor = GrBlack();
    _tilesbcolor = GrBlack();
    _tilessbcolor = GrWhite();

    GrEventAddHook(_GUITilesHookEvent);
}

void _GUITilesEnd(void)
{
    GrEventDeleteHook(_GUITilesHookEvent);

    //GUITilesDestroyAll();

    numtiles = 0;
    selectedtile = -1;
}

GUITile * GUITileCreate(int idt, int type, int x, int y, int width, int height)
{
    GUITile *t;
    int wborder, capbs;

    t = (GUITile *)malloc(sizeof(GUITile));
    if (t == NULL) return NULL;
    
    t->idt = idt;
    t->type = type;
    t->active = 0;
    
    wborder = 0;
    capbs = 0;
    if (t->type == GUI_TT_STATICBORDER) {
        wborder = 1;
    } else if (t->type == GUI_TT_ACTIVEBORDER) {
        wborder = 4;
        t->active = 1;
    } else if (t->type == GUI_TT_ACTIVEBWSCB) {
        wborder = 4;
        t->active = 1;
        capbs = GUI_PCAPB_VSCB | GUI_PCAPB_HSCB;
    } else if (t->type == GUI_TT_ACTIVEBWVSCB) {
        wborder = 4;
        t->active = 1;
        capbs = GUI_PCAPB_VSCB;
    } else if (t->type == GUI_TT_ACTIVEBWHSCB) {
        wborder = 4;
        t->active = 1;
        capbs = GUI_PCAPB_HSCB;
    }

    t->p = GUIPanelCreate(x, y, width, height, capbs, wborder, 0);
    if (t->p == NULL) {
        free(t);
        return NULL;
    }
    
    return t;
}

int GUITileRegister(GUITile *t)
{
    int ind;

    if (t == NULL) return -1;
    if (numtiles >= MAXTILES) return -2;
    if (FindIdt(t->idt) >= 0) return -3;

    ind = numtiles;
    t->selected = 0;
    regtile[ind] = t;
    
    if (selectedtile < 0 && t->active) {
        t->selected = 1;
        selectedtile = ind;
    }

    numtiles++;
    
    return 0;
}

GUITile * GUITileUnRegister(int idt)
{
    GUITile *t;
    int ind, i;
    
    if ((ind = FindIdt(idt)) < 0) return NULL;
    
    t = regtile[ind];
    if (t->selected) {
        t->selected = 0;
        selectedtile = -1;
    }
    
    numtiles--;
    for (i=ind; i<numtiles; i++)
        regtile[i] = regtile[i+1];
    
    if (selectedtile > ind)
        selectedtile--;
    
    if (selectedtile < 0) {
        for (i=0; i<numtiles; i++) {
            if (regtile[i]->active) {
                regtile[i]->selected = 1;
                selectedtile = i;
            }
        }
    }
    
    return t;
}

void GUITileDestroy(GUITile *t)
{
    if (t == NULL) return;
    GUIPanelDestroy(t->p);
    free(t);
}

void GUITilePaint(int idt)
{
    int ind;
    GUITile *t;
    GrColor c;

    if ((ind = FindIdt(idt)) < 0) return;
    
    t = regtile[ind];
    c = t->selected ? _tilessbcolor : _tilesbcolor;
    GUIPanelPaint(t->p, _tileslcolor, c);
    return;
}

GUITile * GUITileGetPointer(int idt)
{
    int ind;

    if ((ind = FindIdt(idt)) < 0) return NULL;
    
    return regtile[ind];
}

void GUITilesSetColors(GrColor l, GrColor b, GrColor sb)
{
    _tileslcolor = l;
    _tilesbcolor = b;
    _tilessbcolor = sb;
}

int GUITilesProcessEvent(GrEvent *ev)
{
    GUITile *t;
    
    if (selectedtile < 0) return 0;
    
    t = regtile[selectedtile];
    return GUIPanelProcessEvent(t->p, ev);
}

int GUITilesProcessTileEvent(GrEvent *ev, int idt)
{
    GUITile *t;
    int ind;

    if ((ind = FindIdt(idt)) < 0) return 0;
    
    t = regtile[ind];
    if (!t->active) return 0;

    return GUIPanelProcessEvent(t->p, ev);
}

void GUITilesDestroyAll(void)
{
    int i;

    for (i=0; i<numtiles; i++)
        GUITileDestroy(regtile[i]);

    numtiles = 0;
    selectedtile = -1;
}

void _GUITilesDisable(void)
{
    savedselected = selectedtile;
    if (selectedtile >= 0) {
        regtile[selectedtile]->selected = 0;
        PaintBorder(regtile[selectedtile]);
        selectedtile = -1;
    }
}

void _GUITilesEnable(void)
{
    if (selectedtile >= 0) return;
    
    selectedtile = savedselected;
    if (selectedtile >= 0) {
        regtile[selectedtile]->selected = 1;
        PaintBorder(regtile[selectedtile]);
    }
}

int _GUITilesHookEvent(GrEvent *ev)
{
    GUIContext *gc;
    int i;
    
    if (_GUIMenuBarInUse()) return 0;
    //if (_GUIDialogRunning()) return 0;
    if (_GUIGetNoHookNow()) return 0;

    if (ev->type == GREV_KEY && ev->p2 == GRKEY_KEYCODE) {
        if (ev->p1 == GrKey_Alt_Left) {
            TileSelectNext(-1);
            return 1;
        } else if (ev->p1 == GrKey_Alt_Right) {
            TileSelectNext(1);
            return 1;
        } else if (ev->p1 == GrKey_F8) {
            TileSelectNext(1);
            return 1;
        }
    }
    
    if (ev->type == GREV_MOUSE) {
        if ((ev->p1 == GRMOUSE_LB_PRESSED) ||
            (ev->p1 == GRMOUSE_MB_PRESSED) ||
            (ev->p1 == GRMOUSE_RB_PRESSED)){
            for (i=0; i<numtiles; i++) {
                gc = regtile[i]->p->gc;
                if (GrCheckCoordInto(ev->p2, ev->p3, gc->xorg, gc->yorg,
                    gc->width, gc->height))
                    TileSelect(i);
            }
        }
    }
    return 0;
}

static int FindIdt(int idt)
{
    int i;
    
    for(i=0; i<numtiles; i++) {
        if (regtile[i]->idt == idt)
            return i;
    }
    
    return -1;
}

static void PaintBorder(GUITile *t)
{
    GrColor c;

    if (t->type == GUI_TT_BORDERLESS) return;
  
    if (t->type == GUI_TT_STATICBORDER) {
        GUIPanelRePaintBorder(t->p, _tileslcolor, _tileslcolor);
        return;
    }

    if (t->type == GUI_TT_ACTIVEBORDER || t->type == GUI_TT_ACTIVEBWSCB ||
        t->type == GUI_TT_ACTIVEBWVSCB ||t->type == GUI_TT_ACTIVEBWHSCB) {
        c = t->selected ? _tilessbcolor : _tilesbcolor;
        GUIPanelRePaintBorder(t->p, _tileslcolor, c);
        return;
    }

    return;
}

static void TileSelect(int nt)
{
    if (!regtile[nt]->active) return;
    if (nt == selectedtile) return;
    if (selectedtile < 0) return;
    regtile[selectedtile]->selected = 0;
    PaintBorder(regtile[selectedtile]);
    selectedtile = nt;
    regtile[selectedtile]->selected = 1;
    PaintBorder(regtile[selectedtile]);
}

static void TileSelectNext(int dir)
{
    int i, next;
    
    if (selectedtile < 0) return;
    
    next = selectedtile + dir;
    for (i=0; i<numtiles-1; i++) {
        if (next >= numtiles) next = 0;
        if (next < 0) next = numtiles - 1;
        if (regtile[next]->active) {
            TileSelect(next);
            break;
        }
        next += dir;
    }
}

