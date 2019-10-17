/**
 ** group.c ---- Mini GUI for MGRX, group of objects (labels, buttons, etc)
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

static int find_id(GUIGroup *g, int id);
static int select_next(GUIGroup *g, int dir);
static void repaint_object(GUIGroup *g, int ind);

/***************************/

GUIGroup * GUIGroupCreate(int nobj, int x, int y)
{
    GUIGroup *g;
    int i;

    g = (GUIGroup *)malloc(sizeof(GUIGroup));
    if (g == NULL) return NULL;

    g->o = (GUIObject *)malloc(sizeof(GUIObject)*nobj);
    if (g->o == NULL) {
        free(g);
        return NULL;
    }
    
    for (i=0; i<nobj; i++) {
        g->o[i].type = GUIOBJTYPE_NONE;
        g->o[i].pressed = -1;
        g->o[i].selected = -1;
    }
    
    g->x = x;
    g->y = y;
    g->nobj = nobj;
    g->selected = -1;
    g->p = NULL;
    
    return g;
}

/***************************/

void GUIGroupSetPanel(GUIGroup *g, GUIPanel *p)
{
    g->p = p;
}

/***************************/

void GUIGroupDestroy(GUIGroup *g)
{
    int i;

    for (i=0; i<g->nobj; i++) {
        _GUIObjectDestroy(&(g->o[i]));
    }

    free(g->o);
    free(g);
}

/***************************/

void GUIGroupSetSelected(GUIGroup *g, int id, int paint)
{
    int ind;
    
    ind = find_id(g, id);
    if (ind < 0) return;
    
    if (g->selected != ind) {
        if (g->selected >= 0) {
            g->o[g->selected].selected = 0;
            if (paint) repaint_object(g, g->selected);
        }
        g->selected = ind;
        g->o[g->selected].selected = 1;
        if (paint) repaint_object(g, g->selected);
    }
}

/***************************/

void GUIGroupPaint(GUIGroup *g)
{
    int i;

    for (i=0; i<g->nobj; i++) {
        _GUIObjectPaint(&(g->o[i]), g->x, g->y);
    }
}

/***************************/

void GUIGroupRePaintObject(GUIGroup *g, int id)
{
    int ind;
    
    ind = find_id(g, id);
    if (ind < 0) return;

    repaint_object(g, ind);
}

/***************************/

void *GUIGroupGetText(GUIGroup *g, int id, int chrtype)
{
    int ind;
    
    ind = find_id(g, id);
    if (ind < 0) return NULL;
    
    return _GUIObjectGetText(&(g->o[ind]), chrtype);
}

/***************************/

void GUIGroupSetOn(GUIGroup *g, int id, int paint)
{
    int ind;
    
    ind = find_id(g, id);
    if (ind < 0) return;
    
    _GUIObjectSetOn(g, &(g->o[ind]), paint);
}

/***************************/

int GUIGroupGetOn(GUIGroup *g, int id)
{
    int ind;
    
    ind = find_id(g, id);
    if (ind < 0) return 0;
    
    return g->o[ind].on;
}

/***************************/

int GUIGroupProcessEvent(GUIGroup *g, GrEvent *ev)
{
    int i, ret, next;
    
    if (ev->type == GREV_MOUSE) {
        if (ev->p1 == GRMOUSE_LB_PRESSED) {
            for (i=0; i<g->nobj; i++) {
                if ((g->o[i].selected >= 0) &&
                    (GrCheckCoordInto(ev->p2, ev->p3, g->x+g->o[i].x,
                    g->y+g->o[i].y, g->o[i].width, g->o[i].height))) {
                    if ((g->selected != i) && (g->selected >= 0)) {
                        g->o[g->selected].selected = 0;
                        repaint_object(g, g->selected);
                    }
                    g->selected = i;
                    return _GUIObjectProcessEvent(g, &(g->o[g->selected]), ev);
                }
            }
        } else if (ev->p1 == GRMOUSE_LB_RELEASED) {
            if (g->o[g->selected].pressed) {
                return _GUIObjectProcessEvent(g, &(g->o[g->selected]), ev);
            }
        }
    } else if (ev->type == GREV_KEY) {
        ret = _GUIObjectProcessEvent(g, &(g->o[g->selected]), ev);
        if (ret) return ret;
        next = -1;
        if (ev->p1 == GrKey_Right || ev->p1 == GrKey_Down || ev->p1 == GrKey_Tab) {
            next = select_next(g, 1);
        } else if (ev->p1 == GrKey_Left ||  ev->p1 == GrKey_Up || ev->p1 == GrKey_BackTab) {
            next = select_next(g, -1);
        }
        if (next >=0 ) {
            g->o[g->selected].selected = 0;
            repaint_object(g, g->selected);
            g->selected = next;
            g->o[g->selected].selected = 1;
            repaint_object(g, g->selected);
            return 1;
        }
    } else {
        ret = _GUIObjectProcessEvent(g, &(g->o[g->selected]), ev);
    }

    return 0;
}

/***************************/

void GUIGroupCalcMinDim(GUIGroup *g, int *width, int *height)
{
    int i, w, h;

    *width = 0;
    *height = 0;

    for (i=0; i<g->nobj; i++) {
        w = g->o[i].x + g->o[i].width;
        h = g->o[i].y + g->o[i].height;
        if (w > *width) *width = w;
        if (h > *height) *height = h;
    }
}

/***************************/

static int find_id(GUIGroup *g, int id)
{
    int i;
    
    for (i=0; i<g->nobj; i++) {
        if (g->o[i].id == id ) return i;
    }
    
    return -1;
}

/***************************/

static int select_next(GUIGroup *g, int dir)
{
    int i, next;
    
    if (g->selected < 0) return -1;
    
    next = g->selected + dir;
    for (i=0; i<g->nobj-1; i++) {
        if (next >= g->nobj) next = 0;
        if (next < 0) next = g->nobj - 1;
        if (g->o[next].selected >= 0) {
            return next;
        }
        next += dir;
    }
    
    return -1;
}

/***************************/

static void repaint_object(GUIGroup *g, int ind)
{
    _GUIObjectPaint(&(g->o[ind]), g->x, g->y);
    if (g->p) {
        GUIPanelBltRectClToScreen(g->p, g->x+g->o[ind].x, g->y+g->o[ind].y,
                                  g->o[ind].width, g->o[ind].height);
    }
}
