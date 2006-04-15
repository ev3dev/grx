/**
 ** grev_x11.c ---- MGRX events, X11 input
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
 **/

#include <stdlib.h>
#include <string.h>
#include "libgrx.h"
#include "libxwin.h"
#include <X11/keysym.h>
#include "mgrxkeys.h"
#include "ninput.h"
#include "x11keys.h"
#include "arith.h"

static int kbd_lastmod = 0;

unsigned short _XKeyEventToGrKey(XKeyEvent *xkey);
unsigned int _XGrModifierKey(KeySym keysym, int type);

/**
 ** _GrEventInit - Initializes inputs
 **
 ** Returns  0 on success
 **         -1 on error
 **
 ** For internal use only
 **/

int _GrEventInit(void)
{
    static char cbits[8] = { 0,0,0,0,0,0,0,0, };
    Pixmap csource, cmask;
    XColor cfore, cback;
    Cursor curs;

    if (GrMouseDetect()) { // Define an invisible X cursor for _XGrWindow
        if(_XGrWindowedMode) {
            csource = cmask = XCreateBitmapFromData(
                              _XGrDisplay, _XGrWindow, cbits, 8, 8);
            cfore.red = cfore.green = cfore.blue = 0;
            cback.red = cback.green = cback.blue = 0;
            curs = XCreatePixmapCursor(
                   _XGrDisplay, csource, cmask, &cfore, &cback, 0, 0);
            XDefineCursor (_XGrDisplay, _XGrWindow, curs);
        }
    }
    return 1;
}

/**
 ** _GrEventUnInit - UnInitializes inputs
 **
 ** For internal use only
 **/

void _GrEventUnInit(void)
{
}

/**
 ** _GrReadInputs - Reads inputs and sets events
 **
 ** For internal use only
 **/

int _GrReadInputs(void)
{
    GrEvent evaux;
    int nev = 0;
    int count;
    XEvent xev;
    KeySym keysym;

    if (!_XGrDisplay) return 0;

    count = XEventsQueued(_XGrDisplay, QueuedAfterReading);
    if (count <= 0) {
        XFlush(_XGrDisplay);
        return 0;
    }

    while (--count >= 0) {
        XNextEvent(_XGrDisplay, &xev);
        switch (xev.type) {
        case MotionNotify:
            if (_XGrWindowedMode) {
                MOUINFO->xpos = xev.xmotion.x;
                MOUINFO->ypos = xev.xmotion.y;
            } else {
                MOUINFO->xpos += xev.xmotion.x;
                MOUINFO->ypos += xev.xmotion.y;
            }
            GrMouseUpdateCursor();
            MOUINFO->moved  = TRUE;
            if ((MOUINFO->genmmove == GR_GEN_MMOVE_ALWAYS) ||
                ((MOUINFO->genmmove == GR_GEN_MMOVE_IFBUT) &&
                 (MOUINFO->bstatus != 0))) {
                evaux.type = GREV_MMOVE;
                evaux.kbstat = kbd_lastmod;
                evaux.p1 = MOUINFO->bstatus;
                evaux.p2 = MOUINFO->xpos;
                evaux.p3 = MOUINFO->ypos;
                GrEventEnqueue(&evaux);
                MOUINFO->moved = FALSE;
                nev++;
            }
            break;

        case ButtonPress:
        case ButtonRelease:
            evaux.type = GREV_MOUSE;
            evaux.kbstat = kbd_lastmod;
            evaux.p2 = MOUINFO->xpos;
            evaux.p3 = MOUINFO->ypos;
            switch (xev.xbutton.type) {
            case ButtonPress:
                switch (xev.xbutton.button) {
                case Button1: evaux.p1 = GRMOUSE_LB_PRESSED;
                              MOUINFO->bstatus |= GRMOUSE_LB_STATUS;
                              break;
                case Button2: evaux.p1 = GRMOUSE_MB_PRESSED;
                              MOUINFO->bstatus |= GRMOUSE_MB_STATUS;
                              break;
                case Button3: evaux.p1 = GRMOUSE_RB_PRESSED;
                              MOUINFO->bstatus |= GRMOUSE_RB_STATUS;
                              break;
                }
                break;
            case ButtonRelease:
                switch (xev.xbutton.button) {
                case Button1: evaux.p1 = GRMOUSE_LB_RELEASED;
                              MOUINFO->bstatus &= ~GRMOUSE_LB_STATUS;
                              break;
                case Button2: evaux.p1 = GRMOUSE_MB_RELEASED;
                              MOUINFO->bstatus &= ~GRMOUSE_MB_STATUS;
                              break;
                case Button3: evaux.p1 = GRMOUSE_RB_RELEASED;
                              MOUINFO->bstatus &= ~GRMOUSE_RB_STATUS;
                              break;
                }
                break;
            }
            GrEventEnqueue(&evaux);
            MOUINFO->moved = FALSE;
            nev++;
            break;

      case KeyPress:
          keysym = XKeycodeToKeysym(_XGrDisplay, xev.xkey.keycode, 0);
          if (IsModifierKey(keysym)) {
              _XGrModifierKey(keysym, xev.type);
          } else {
              evaux.type = GREV_KEY;
              evaux.kbstat = kbd_lastmod;
              evaux.p1 = _XKeyEventToGrKey (&xev.xkey);
              evaux.p2 = 0;
              evaux.p3 = 0;
              GrEventEnqueue(&evaux);
              MOUINFO->moved = FALSE;
              nev++;
          }
          break;

      case KeyRelease:
          keysym = XKeycodeToKeysym(_XGrDisplay, xev.xkey.keycode, 0);
          if (IsModifierKey (keysym)) {
              _XGrModifierKey (keysym, xev.type);
          }
          break;
      }
    }

    return nev;
}

/**
 ** _GrMouseDetect - Returns true if a mouse is detected
 **
 ** For internal use only
 **/

int _GrMouseDetect(void)
{
    if (_XGrDisplay) {
        MOUINFO->msstatus = 1;
        return 1;
    }
    return 0;
}

/**
 ** GrMouseSetSpeed
 **
 **/

void GrMouseSetSpeed(int spmult, int spdiv)
{
    MOUINFO->spmult = umin(16, umax(1, spmult));
    MOUINFO->spdiv  = umin(16, umax(1, spdiv));
}

/**
 ** GrMouseSetAccel
 **
 **/

void GrMouseSetAccel(int thresh, int accel)
{
    MOUINFO->thresh = umin(64, umax(1, thresh));
    MOUINFO->accel  = umin(16, umax(1, accel));
}

/**
 ** GrMouseSetLimits
 **
 **/

void GrMouseSetLimits(int x1, int y1, int x2, int y2)
{
    isort(x1, x2);
    isort(y1, y2);
    MOUINFO->xmin = imax(0, imin(x1, SCRN->gc_xmax));
    MOUINFO->ymin = imax(0, imin(y1, SCRN->gc_ymax));
    MOUINFO->xmax = imax(0, imin(x2, SCRN->gc_xmax));
    MOUINFO->ymax = imax(0, imin(y2, SCRN->gc_ymax));
}

/**
 ** GrMouseWarp
 **
 **/

void GrMouseWarp(int x, int y)
{
    MOUINFO->xpos = imax(MOUINFO->xmin, imin(MOUINFO->xmax, x));
    MOUINFO->ypos = imax(MOUINFO->ymin, imin(MOUINFO->ymax, y));
    if (_XGrDisplay) {
        GrMouseUpdateCursor();
        XWarpPointer(_XGrDisplay, _XGrWindow, _XGrWindow, 0, 0,
                     GrScreenX(), GrScreenY(), MOUINFO->xpos, MOUINFO->ypos);
    }
}

/** Internal functions **/

unsigned short _XKeyEventToGrKey(XKeyEvent *xkey)
{
    KeyEntry *kp;
    unsigned int state;
    char buffer[20];
    KeySym keysym;
    int count;

    state = xkey->state & (ShiftMask | ControlMask | Mod1Mask);
    count = XLookupString(xkey, buffer, sizeof(buffer),
                          &keysym, (XComposeStatus *)NULL);

    if ((count == 1) && ((state & Mod1Mask) == 0))
        return (unsigned char) buffer[0];

    for (kp = _KeyTable;
         kp < &_KeyTable[sizeof(_KeyTable)/sizeof(_KeyTable[0])];
         kp = kp + 1) {
        if (keysym == kp->keysym && state == kp->state)
            return kp->key;
    }

    return EOF;
}

unsigned int _XGrModifierKey(KeySym keysym, int type)
{
    if (type == KeyPress) {
        switch (keysym) {
        case XK_Shift_L:    kbd_lastmod |= GRKBS_LEFTSHIFT; break;
        case XK_Shift_R:    kbd_lastmod |= GRKBS_RIGHTSHIFT; break;
        case XK_Control_L:
        case XK_Control_R:  kbd_lastmod |= GRKBS_CTRL; break;
        case XK_Alt_L:
        case XK_Alt_R:
        case XK_Meta_L:
        case XK_Meta_R:     kbd_lastmod |= GRKBS_ALT; break;
        case XK_Num_Lock:   kbd_lastmod |= GRKBS_NUMLOCK; break;
        case XK_Caps_Lock:  kbd_lastmod |= GRKBS_CAPSLOCK; break;
        case XK_Insert:     kbd_lastmod |= GRKBS_INSERT; break;
        }
    }
    if (type == KeyRelease) {
        switch (keysym) {
        case XK_Shift_L:    kbd_lastmod &= ~GRKBS_LEFTSHIFT; break;
        case XK_Shift_R:    kbd_lastmod &= ~GRKBS_RIGHTSHIFT; break;
        case XK_Control_L:
        case XK_Control_R:  kbd_lastmod &= ~GRKBS_CTRL; break;
        case XK_Alt_L:
        case XK_Alt_R:
        case XK_Meta_L:
        case XK_Meta_R:     kbd_lastmod &= ~GRKBS_ALT; break;
        case XK_Num_Lock:   kbd_lastmod &= ~GRKBS_NUMLOCK; break;
        case XK_Caps_Lock:  kbd_lastmod &= ~GRKBS_CAPSLOCK; break;
        case XK_Insert:     kbd_lastmod &= ~GRKBS_INSERT; break;
        }
    }
    return kbd_lastmod;
}
