/**
 ** shortcut.c ---- Mini GUI for MGRX, key short cuts
 **
 ** Copyright (C) 2002,2006 Mariano Alvarez Fernandez
 ** [e-mail: malfer at telefonica.net]
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

#define MAX_KEYSHORTCUT 50

typedef struct {
    GrEvent ev;
    int key;
} KeyShortCut;

static KeyShortCut ksc[MAX_KEYSHORTCUT];
static int nksc = 0;

void _GUIKeyShortCutInit(void)
{
    _GUIKeyShortCutReset();
    GrEventAddHook(_GUIKeyShortCutHookEvent);
    nksc = 0;
}

void _GUIKeyShortCutEnd(void)
{
    GrEventDeleteHook(_GUIKeyShortCutHookEvent);
}

void _GUIKeyShortCutReset(void)
{
    nksc = 0;
}

int _GUIKeyShortCutAdd(int key, int type, long p1, long p2, long p3)
{
    if (nksc >= MAX_KEYSHORTCUT) return -1;

    ksc[nksc].key = key;
    ksc[nksc].ev.type = type;
    ksc[nksc].ev.p1 = p1;
    ksc[nksc].ev.p2 = p2;
    ksc[nksc].ev.p3 = p3;
    nksc++;

    return 0;
}

int _GUIKeyShortCutHookEvent(GrEvent *ev)
{
    int i;

    if (_GUIGetNoHookNow()) return 0;

    if (ev->type == GREV_KEY) {
        for (i = 0; i<nksc; i++) {
            if (ev->p1 == ksc[i].key) {
                *ev = ksc[i].ev;
                return 0;
            }
        }
    }
    return 0;
}

