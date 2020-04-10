/**
 ** panels.c ---- Mini GUI for MGRX, panels
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
#include <stdio.h>
#include <string.h>
#include "grguip.h"

static void PaintBorder(GUIPanel *p, GrColor lcolor, GrColor bcolor);
void PaintCorner(GUIPanel *p);

/***************************/

GUIPanel * GUIPanelCreate(int xo, int yo, int width, int height, int capbs,
                            int wborder, int htitle)
{
    GUIPanel *p;
    int su, both, lenght;
    int x, y, w, h;

    p = (GUIPanel *)malloc(sizeof(GUIPanel));
    if (p == NULL) return NULL;

    su = capbs & GUI_PCAPB_SU;
    p->gc = GUIContextCreate(xo, yo, width, height, su);
    if (p->gc == NULL) {
        free(p);
        return NULL;
    }
    
    p->wborder = wborder;
    x = wborder;
    y = wborder;
    w = width - wborder * 2;
    h = height - wborder * 2;
    
    p->htitle = htitle;
    p->ct = NULL;
    if (p->htitle > 0) {
        p->ct = GrCreateSubContext(x, y, x+w-1, y+p->htitle-1, p->gc->c, NULL);
        if (p->ct == NULL) p->htitle = 0;
    }
    y += p->htitle;
    h -= p->htitle;

    p->vscb = NULL;
    p->hscb = NULL;

    both = ((capbs & GUI_PCAPB_VSCB) && (capbs & GUI_PCAPB_HSCB)) ? 1 : 0;
    if (capbs & GUI_PCAPB_VSCB) {
        lenght = h - both * GUI_SB_DEFAULT_THICK;
        p->vscb = GUIScrollbarCreate(GUI_TSB_VERTICAL, x+w-GUI_SB_DEFAULT_THICK,
                                     y, GUI_SB_DEFAULT_THICK, lenght);
    }
    if (capbs & GUI_PCAPB_HSCB) {
        lenght = w - both * GUI_SB_DEFAULT_THICK;
        p->hscb = GUIScrollbarCreate(GUI_TSB_HORIZONTAL, x, y+h-GUI_SB_DEFAULT_THICK,
                                     GUI_SB_DEFAULT_THICK, lenght);
    }
    if (p->vscb != NULL) w -= GUI_SB_DEFAULT_THICK;
    if (p->hscb != NULL) h -= GUI_SB_DEFAULT_THICK;

    p->cl = GrCreateSubContext(x, y, x+w-1, y+h-1, p->gc->c, NULL);
    if (p->cl == NULL) {
        if (p->ct) GrDestroyContext(p->ct);
        GUIContextDestroy(p->gc);
        free(p);
        return NULL;
    }
    p->xcl = xo + x;
    p->ycl = yo + y;
    
    p->udata = NULL;
    p->paintcl = NULL;
    p->painttl = NULL;
    p->proevcl = NULL;
    
    return p;
}

/***************************/

void GUIPanelSetClCallBacks(GUIPanel *p, void (*paintcl)(void *),
                             int (*proevcl)(void *, GrEvent *))
{
    p->paintcl = paintcl;
    p->proevcl = proevcl;
}

/***************************/

void GUIPanelSetTlCallBack(GUIPanel *p, void (*painttl)(void *))
{
    p->painttl = painttl;
}

/***************************/

void GUIPanelSetUserData(GUIPanel *p, void *udata)
{
    p->udata = udata;
}

/***************************/

void * GUIPanelGetUserData(GUIPanel *p)
{
    return p->udata;
}

/***************************/

void GUIPanelPaint(GUIPanel *p, GrColor lcolor, GrColor bcolor)
{
    GrContext grcaux;

    GrSaveContext(&grcaux);

    GrSetContext(p->gc->c);
    if (p->wborder > 0) PaintBorder(p, lcolor, bcolor);
    if (p->vscb != NULL) GUIScrollbarPaint(p->vscb, 0, 0, 0);
    if (p->hscb != NULL) GUIScrollbarPaint(p->hscb, 0, 0, 0);
    if ((p->vscb != NULL) && (p->hscb != NULL)) PaintCorner(p);

    if (p->ct && p->painttl) {
        GrSetContext(p->ct);
        (p->painttl)(p->udata);
    }

    if (p->paintcl) {
        GrSetContext(p->cl);
        (p->paintcl)(p->udata);
    }

    GrSetContext(p->gc->c);
    GUIDBCurCtxBltToScreen();
    GrSetContext(&grcaux);
}

/***************************/

void GUIPanelRePaintScb(GUIPanel *p)
{
    GrContext grcaux;

    GrSaveContext(&grcaux);
    GrSetContext(p->gc->c);

    if (p->vscb != NULL) {
        GUIScrollbarPaint(p->vscb, 0, 0, 1);
    }
    if (p->hscb != NULL) {
        GUIScrollbarPaint(p->hscb, 0, 0, 1);
    }

    GrSetContext(&grcaux);
}

/***************************/

void GUIPanelRePaintBorder(GUIPanel *p, GrColor lcolor, GrColor bcolor)
{
    GrContext grcaux;

    if (p->wborder <= 0) return;
    
    GrSaveContext(&grcaux);

    GrSetContext(p->gc->c);
    PaintBorder(p, lcolor, bcolor);
    
    if (_GUIUseDB) { // we repaint instead bitblt in this case (by now)
        GrSetContext(p->gc->sc);
        PaintBorder(p, lcolor, bcolor);
    }
        
    GrSetContext(&grcaux);
}

/***************************/

int GUIPanelProcessEvent(GUIPanel *p, GrEvent *ev)
{
    GrContext grcaux;
    GrEvent evaux;
    int ret = 0;
    
    if (p->proevcl == NULL) return 0;
    
    GrSaveContext(&grcaux);

    GrSetContext(p->gc->c);
    evaux = *ev;
    GUIContextTransMouseEv(p->gc, &evaux);
    if (p->vscb != NULL)
        ret = GUIScrollbarProcessEvent(p->vscb, &evaux);
    if (!ret) {
        if (p->hscb != NULL)
            ret = GUIScrollbarProcessEvent(p->hscb, &evaux);
    }

    if (!ret) {
        GrSetContext(p->cl);
        if (ev->type == GREV_MOUSE || ev->type == GREV_MMOVE) {
            //evaux.p2 -= p->cl->gc_xoffset - p->gc->c->gc_xoffset;
            //evaux.p3 -= p->cl->gc_yoffset - p->gc->c->gc_yoffset;
            evaux.p2 = ev->p2 - p->xcl;
            evaux.p3 = ev->p3 - p->ycl;
        }
        ret = (p->proevcl)(p->udata, &evaux);
    }

    GrSetContext(&grcaux);
    return ret;
}

/***************************/

void GUIPanelDestroy(GUIPanel *p)
{
    if (p == NULL) return;
    if (p->ct) GrDestroyContext(p->ct);
    GrDestroyContext(p->cl);
    if (p->vscb != NULL) free(p->vscb);
    if (p->hscb != NULL) free(p->hscb);
    GUIContextDestroy(p->gc);
    free(p);
}

/***************************/

static void PaintBorder(GUIPanel *p, GrColor lcolor, GrColor bcolor)
{
    //we expect the context set
    GrDrawBorder(0, 0, p->gc->width-1, p->gc->height-1, p->wborder, lcolor, bcolor);
}

/***************************/

void PaintCorner(GUIPanel *p)
{
    //we expect the context set
    GrFilledBox(GrMaxX()-GUI_SB_DEFAULT_THICK-p->wborder+1,
                GrMaxY()-GUI_SB_DEFAULT_THICK-p->wborder+1,
                GrMaxX()-p->wborder, GrMaxY()-p->wborder, _scbliftcolor);
    GrFilledBox(GrMaxX()-GUI_SB_DEFAULT_THICK-p->wborder+4,
                GrMaxY()-GUI_SB_DEFAULT_THICK-p->wborder+4,
                GrMaxX()-p->wborder-2, GrMaxY()-p->wborder-2, _scbbgcolor);
}
