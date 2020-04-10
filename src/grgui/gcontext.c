/**
 ** gcontext.c ---- Mini GUI for MGRX, GUI contexts
 **
 ** Copyright (C) 2002,2005 Mariano Alvarez Fernandez
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
#include "grguip.h"

GUIContext *GUIContextCreate(int x, int y, int width, int height, int su)
{
    GUIContext *gctx;

    gctx = (GUIContext *)malloc(sizeof(GUIContext));
    if (gctx == NULL) return NULL;

    gctx->xorg = x;
    gctx->yorg = y;
    gctx->width = width;
    gctx->height = height;
    
    gctx->sc = GrCreateSubContext(x, y, x+width-1, y+height-1, GrScreenContext(), NULL);
    if (gctx->sc == NULL) {
        free(gctx);
        return NULL;
    }
    
    if (_GUIUseDB) {
        gctx->c = GrCreateSubContext(x, y, x+width-1, y+height-1, _GUIGlobCtx, NULL);
        if (gctx->c == NULL) {
            free(gctx->sc);
            free(gctx);
            return NULL;
        }
    } else {
        gctx->c = gctx->sc;
        gctx->sc = NULL;
    }
    
    gctx->su = NULL;
    if (su) {
        gctx->su = GrCreateContext(width, height, NULL, NULL);
    }

    return gctx;
}

void GUIContextSaveUnder(GUIContext *gctx)
{
    if (gctx->su) {
        GrBitBlt(gctx->su, 0, 0, _GUIGlobCtx, gctx->xorg, gctx->yorg,
                 gctx->xorg+gctx->width-1, gctx->yorg+gctx->height-1,
                 GrWRITE);
    }
}

void GUIContextRestoreUnder(GUIContext *gctx)
{
    if (gctx->su) {
        GrBitBlt(gctx->c, 0, 0, gctx->su, 0, 0,
                 gctx->width-1, gctx->height-1, GrWRITE);
        if (_GUIUseDB) {
            GrBitBlt(gctx->sc, 0, 0, gctx->c, 0, 0,
                     gctx->width-1, gctx->height-1, GrWRITE);
        }
    }
}

void GUIContextDestroy(GUIContext *gctx)
{
    if (gctx->su)
        GrDestroyContext(gctx->su);
    if (_GUIUseDB)
        GrDestroyContext(gctx->sc);
    GrDestroyContext(gctx->c);
    free(gctx);
}

int GUIContextTransMouseEvIfInCtx(GUIContext *gctx, GrEvent *ev)
{
    long p2, p3;

    if (ev->type != GREV_MOUSE && ev->type != GREV_MMOVE) return 0;

    p2 = ev->p2 - gctx->xorg;
    p3 = ev->p3 - gctx->yorg;

    if (p2 < 0 || p3 < 0) return 0;
    if (p2 >= gctx->width || p3 >= gctx->height) return 0;
    
    ev->p2 = p2;
    ev->p3 = p3;

    return 1;
}

void GUIContextTransMouseEv(GUIContext *gctx, GrEvent *ev)
{
    if (ev->type != GREV_MOUSE && ev->type != GREV_MMOVE) return;

    ev->p2 -= gctx->xorg;
    ev->p3 -= gctx->yorg;
}
