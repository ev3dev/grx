/**
 ** grevent.c ---- MGRX events
 **
 ** Copyright (C) 2005 Mariano Alvarez Fernandez
 ** [e-mail: malfer@telefonica.net]
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
 ** Contributions by:
 ** 080113 M.Alvarez, intl support
 **
 **/

#include <stdlib.h>
#include <string.h>
#include "libgrx.h"
#include "ninput.h"

#define MAX_EVQUEUE 60

static GrEvent evqueue[MAX_EVQUEUE];
static int num_evqueue = 0;

static int kbsysencoding = 0;
static int kbusrencoding = 0;

#define MAX_HOOK_FUNCTIONS 10
static int (*hook_event[MAX_HOOK_FUNCTIONS]) (GrEvent *);

static int preproccess_event(GrEvent *ev);

/**
 ** GrEventInit - Initializes the input event queue
 **
 ** Returns  0 on success
 **         -1 on error
 **/

int GrEventInit(void)
{
    int i, ret;

    num_evqueue = 0;
    for (i=0; i<MAX_HOOK_FUNCTIONS; i++)
        hook_event[i] = NULL;
    if (GrMouseDetect()) {
        GrMouseSetSpeed(1, 1);
        GrMouseSetAccel(100, 1);
        GrMouseSetLimits(0, 0, SCRN->gc_xmax, SCRN->gc_ymax);
        GrMouseWarp((SCRN->gc_xmax >> 1), (SCRN->gc_ymax >> 1));
        _GrInitMouseCursor();
        MOUINFO->msstatus = 2;
    }
    ret = _GrEventInit();
    kbsysencoding = _GrGetKbSysEncoding();
    if (kbsysencoding == GRENC_UTF_8)
        kbusrencoding = GRENC_ISO_8859_1;
    else
        kbusrencoding = kbsysencoding;
    return ret;
}

/**
 ** GrEventUnInit - Ending the input event queue
 **
 **/

void GrEventUnInit(void)
{
    num_evqueue = 0;
    if (MOUINFO->msstatus > 1) MOUINFO->msstatus = 1;
    _GrEventUnInit();
}

/**
 ** GrGetKbSysEncoding - Get kb system encoding
 **
 **/

int GrGetKbSysEncoding(void)
{
    return kbsysencoding;
}

/**
 ** GrGetKbEncoding - Get kb user encoding
 **
 **/

int GrGetKbEncoding(void)
{
    return kbusrencoding;
}

/**
 ** GrSetKbEncoding - Set kb user encoding
 **
 **/

int GrSetKbEncoding(int enc)
{
    if (enc >= 0 && enc <= GRENC_LASTENCODE) {
        kbusrencoding = enc;
        return 1;
    }
    return 0;
}

/**
 ** GrEventCheck - Checks if a event is waiting
 **
 ** Returns  1 an event is waiting
 **          0 no events
 **/

int GrEventCheck(void)
{
    if (num_evqueue > 0 || _GrReadInputs()) return 1;
    return 0;
}

/**
 ** GrEventFlush - Empty the input event queue
 **
 **/

void GrEventFlush(void)
{
    while(_GrReadInputs());
    num_evqueue = 0;
}

/**
 ** GrEventRead - Reads an event
 **
 ** It doesn't wait, GREV_NULL is returned if no event is waiting
 **
 ** Arguments:
 **   ev: returns the event
 **/

void GrEventRead(GrEvent * ev)
{
    while (1) {
        if (num_evqueue > 0) {
            num_evqueue--;
            *ev = evqueue[num_evqueue];
            if (preproccess_event(ev)) continue;
            return;
        }
        if (_GrReadInputs()) {
            continue;
        }
        ev->type = GREV_NULL;
        ev->time = GrMsecTime();
        ev->kbstat = 0;
        ev->p1 = 0;
        ev->p2 = 0;
        ev->p3 = 0;
        return;
    }
}

/**
 ** GrEventWait - Waits for an event
 **
 ** Arguments:
 **   ev: returns the event
 **/


void GrEventWait(GrEvent * ev)
{
    while (1) {
        GrEventRead(ev);
        if (ev->type != GREV_NULL) {
            return;
        }
    }
}

/**
 ** GrEventWaitKeyOrClick - Waits for a kay event or a mouse click event
 **
 ** Arguments:
 **   ev: returns the event
 **/


void GrEventWaitKeyOrClick(GrEvent * ev)
{
    while (1) {
        GrEventRead(ev);
        if (ev->type == GREV_KEY)
           return;
        if (ev->type == GREV_MOUSE && ev->p1 == GRMOUSE_LB_RELEASED)
           return;
    }
}

/**
 ** GrEventEnqueue - Enqueues an event
 **
 ** Arguments:
 **   ev: returns the event
 **
 ** Returns  0 on success
 **         -1 on error
 **/

int GrEventEnqueue(GrEvent * ev)
{
    int i;

    ev->time = GrMsecTime();
    if (num_evqueue < MAX_EVQUEUE) {
        for (i=num_evqueue; i>0; i--)
            evqueue[i] = evqueue[i-1];
        evqueue[0] = *ev;
        num_evqueue++;
        return 0;
    }
    return -1;
}

/**
 ** GrEventAddHook - Add a function to hook events
 **
 ** Arguments:
 **   fn: hook funtion pointer
 **
 ** Returns true on succes
 **/

int GrEventAddHook(int (*fn) (GrEvent *))
{
    int i;

    for (i=0; i<MAX_HOOK_FUNCTIONS; i++) {
        if (hook_event[i] == NULL) {
            hook_event[i] = fn;
            return 1;
        }
    }

    return 0;
}

/**
 ** GrEventDeleteHook - Delete a function to hook events
 **
 ** Arguments:
 **   fn: hook funtion pointer
 **
 ** Returns true on succes
 **/

int GrEventDeleteHook(int (*fn) (GrEvent *))
{
    int i;

    for (i=0; i<MAX_HOOK_FUNCTIONS; i++) {
        if (hook_event[i] == fn) {
            hook_event[i] = NULL;
            return 1;
        }
    }

    return 0;
}

/* Internal functions */

static int preproccess_event(GrEvent *ev)
{
    int i;

    if (ev->type == GREV_PREKEY) {
        if ((ev->p2 != GRKEY_KEYCODE) &&
            (kbsysencoding != kbusrencoding)) {
          GrRecodeEvent(ev, kbsysencoding, kbusrencoding);
        }
        ev->type = GREV_KEY;
    }

    for ( i=0; i<MAX_HOOK_FUNCTIONS; i++) {
        if (hook_event[i] != NULL)
            if (hook_event[i](ev)) return 1;
    }

    return 0;
}

