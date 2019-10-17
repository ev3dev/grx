/**
 ** setup.c ---- Mini GUI for MGRX, setup
 **
 ** Copyright (C) 2002,2006,2019 Mariano Alvarez Fernandez
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
#include "grguip.h"

static int guiinited = 0;
static int greventinited = 0;
static int nohookplease = 0;
static int manageexposeevents = 0;

int _GUIUseDB = 0;
GrContext *_GUIGlobCtx = NULL;

int _GUIExposeHookEvent(GrEvent *ev);

void GUIInit(int initgrevent, int doublebuffer)
{
    if (initgrevent) {
        GrEventInit();
        GrMouseSetInternalCursor(GR_MCUR_TYPE_ARROW, GrWhite(), GrBlack());
        GrMouseDisplayCursor();
        GrEventGenMmove(GR_GEN_MMOVE_IFBUT);
        //GrEventGenMmove(GR_GEN_MMOVE_ALWAYS);
        greventinited = 1;
    }

    if (doublebuffer) {
        _GUIGlobCtx = GrCreateContext(GrScreenX(), GrScreenY(), NULL, NULL);
        if (_GUIGlobCtx != NULL) _GUIUseDB = 1;
    }

    if (_GUIGlobCtx == NULL)
        _GUIGlobCtx = GrScreenContext();

    _GUIKeyShortCutInit();
    _GUIMenuInit();
    _GUIMenuBarInit();
    _GUIScrollbarInit();
    _GUITilesInit();
    _GUIDialogInit();
    _GUICDialogInit();
    _GUIObjectInit();

    GrEventAddHook(_GUIExposeHookEvent);
    
    guiinited = 1;
}

void GUIEnd(void)
{
    if (!guiinited) return;

    _GUIObjectEnd();
    _GUICDialogEnd();
    _GUIDialogEnd();
    _GUITilesEnd();
    _GUIScrollbarEnd();
    _GUIMenuBarEnd();
    _GUIMenuEnd();
    _GUIKeyShortCutEnd();

    if (greventinited) {
        GrMouseEraseCursor();
        GrEventUnInit();
        greventinited = 0;
    }

    if (_GUIUseDB) {
        GrDestroyContext(_GUIGlobCtx);
    }
    _GUIGlobCtx = NULL;
    _GUIUseDB = 0;

    manageexposeevents = 0;
    nohookplease = 0;
    guiinited = 0;
}

GrContext *GUIGetGlobalContext(void)
{
    return _GUIGlobCtx;
}

void GUIGlobalBltRectToScreen(int x1, int y1, int x2, int y2)
{
    if (_GUIUseDB) {
        GrBitBlt(GrScreenContext(), x1, y1,
                 _GUIGlobCtx, x1, y1, x2, y2, GrWRITE);
    }
}


int _GUIExposeHookEvent(GrEvent *ev)
{
    if (manageexposeevents && (ev->type == GREV_EXPOSE)) {
        //printf("expose %ld %ld %ld %ld %d\n",
        //        ev->p1, ev->p2, ev->p3, ev->p4, ev->kbstat);
        //if (ev->kbstat == 0) {// no more EXPOSE events follow
        //    GrBitBlt(GrScreenContext(), 0, 0, _GUIGlobCtx, 0, 0,
        //             GrScreenX()-1, GrScreenY()-1, GrWRITE);
        //}
        GrBitBlt(GrScreenContext(), ev->p1, ev->p2, _GUIGlobCtx,
                 ev->p1, ev->p2, ev->p1+ev->p3-1, ev->p2+ev->p4-1, GrWRITE);
        return 1;
    }
    
    return 0;
}

void GUIManageExposeEvents(int manage)
{
    if (manage && _GUIUseDB && !manageexposeevents) {
        manageexposeevents = 1;
        GrEventGenExpose(GR_GEN_EXPOSE_YES);
    } else if (manageexposeevents) {
        manageexposeevents = 0;
        GrEventGenExpose(GR_GEN_EXPOSE_YES);
    }
}

void _GUISuspendHooks(void)
{
    nohookplease++;
}

void _GUIRestartHooks(void)
{
    nohookplease--;
}

int _GUIGetNoHookNow(void)
{
    return (nohookplease > 0);
}
