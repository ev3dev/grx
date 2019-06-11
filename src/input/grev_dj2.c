/**
 ** grev_dj2.c ---- MGRX events, djgpp2 console input
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu]
 ** Copyright (C) 2006 Mariano Alvarez Fernandez
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
 ** 080120 M.Alvarez, intl support
 **
 **/

#include <pc.h>
#include <dos.h>
#include <string.h>
#include <dpmi.h>

#include "libgrx.h"
#include "ninput.h"
#include "arith.h"
#include "memcopy.h"
#include "memfill.h"
#include "int86.h"
#include "mgrxkeys.h"

static int mou_buttons;
static int kbsysencoding = -1;

static int _GetKey(void);
static int _GetKbstat(void);
static void _ReadMouseData(int *mb, int *mx, int *my);

#define PS2_LEFTBUTTON   1
#define PS2_MIDDLEBUTTON 4
#define PS2_RIGHTBUTTON  2

#define update_coord(WHICH,MICKEYS) do {                                    \
	static int fract = 0;                                               \
	int delta,ddelta,pos;                                               \
	delta   = (MICKEYS) * MOUINFO->spmult;                              \
	ddelta  = (delta + fract) / MOUINFO->spdiv;                         \
	fract   = (delta + fract) % MOUINFO->spdiv;                         \
	if(iabs(ddelta) >= MOUINFO->thresh) ddelta *= MOUINFO->accel;       \
	pos     = MOUINFO->WHICH##pos + ddelta;                             \
	if(pos  < MOUINFO->WHICH##min) pos = MOUINFO->WHICH##min;           \
	if(pos  > MOUINFO->WHICH##max) pos = MOUINFO->WHICH##max;           \
	if(pos != MOUINFO->WHICH##pos) {                                    \
	MOUINFO->WHICH##pos = pos;                                          \
	}                                                                   \
} while(0)

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
    char *s;

    if (GrMouseDetect()) {
        mou_buttons = 0;
    }
    s = getenv("MGRXKBSYSENCODING");
    if (s != NULL) kbsysencoding = GrFindEncoding(s);
    if (kbsysencoding < 0) kbsysencoding = GRENC_CP437;
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
 ** _GrGetKbSysEncoding - Get kb system encoding
 **
 **/

int _GrGetKbSysEncoding(void)
{
    return kbsysencoding;
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
    int mb, mx, my;

    if (MOUINFO->msstatus == 2) {
        _ReadMouseData(&mb, &mx, &my);
        /** NOTE we know PS2_LEFTBUTTON == GRMOUSE_LB_STATUS, etc */
        MOUINFO->bstatus = mb;
        if ((mx != 0) | (my != 0)) {
            update_coord(x, mx);
            update_coord(y, my);
            GrMouseUpdateCursor();
            MOUINFO->moved  = TRUE;
        }
        if (mb != mou_buttons) {
            evaux.type = GREV_MOUSE;
            evaux.kbstat = _GetKbstat();
            evaux.p2 = MOUINFO->xpos;
            evaux.p3 = MOUINFO->ypos;
            if ((mb & PS2_LEFTBUTTON) != (mou_buttons & PS2_LEFTBUTTON)) {
                if (mb & PS2_LEFTBUTTON)
                    evaux.p1 = GRMOUSE_LB_PRESSED;
                else
                    evaux.p1 = GRMOUSE_LB_RELEASED;
                GrEventEnqueue(&evaux);
                nev++;
            }
            if ((mb & PS2_MIDDLEBUTTON) != (mou_buttons & PS2_MIDDLEBUTTON)) {
                if (mb & PS2_MIDDLEBUTTON)
                    evaux.p1 = GRMOUSE_MB_PRESSED;
                else
                    evaux.p1 = GRMOUSE_MB_RELEASED;
                GrEventEnqueue(&evaux);
                nev++;
            }
            if ((mb & PS2_RIGHTBUTTON) != (mou_buttons & PS2_RIGHTBUTTON)) {
                if (mb & PS2_RIGHTBUTTON)
                    evaux.p1 = GRMOUSE_RB_PRESSED;
                else
                    evaux.p1 = GRMOUSE_RB_RELEASED;
                GrEventEnqueue(&evaux);
                nev++;
            }
            mou_buttons = mb;
            MOUINFO->moved = FALSE;
        }
        else if ((mx != 0) | (my != 0) &&
                 ((MOUINFO->genmmove == GR_GEN_MMOVE_ALWAYS) ||
                 ((MOUINFO->genmmove == GR_GEN_MMOVE_IFBUT) &&
                 (MOUINFO->bstatus != 0)))) {
            evaux.type = GREV_MMOVE;
            evaux.kbstat = _GetKbstat();
            evaux.p1 = MOUINFO->bstatus;
            evaux.p2 = MOUINFO->xpos;
            evaux.p3 = MOUINFO->ypos;
            GrEventEnqueue(&evaux);
            MOUINFO->moved = FALSE;
            nev++;
       }
    }

    if (kbhit()) {
        evaux.type = GREV_PREKEY;
        evaux.kbstat = _GetKbstat();
        evaux.p1 = _GetKey();
        if (evaux.p1 > 0xff)
            evaux.p2 = GRKEY_KEYCODE;
        else
            evaux.p2 = 1;
        evaux.p3 = 0;
        GrEventEnqueue(&evaux);
        MOUINFO->moved = FALSE;
        nev++;
    }

    if (nev == 0) delay(1); /* yield */
    return nev;
}

/**
 ** _GrMouseDetect - Returns true if a mouse is detected
 **
 ** For internal use only
 **/

int _GrMouseDetect(void)
{
    Int86Regs r;

    MOUINFO->msstatus = (-1);	/* assume missing */
    sttzero(&r);
    int33(&r);
    if(IREG_AX(r) != 0) {
        MOUINFO->msstatus = 1;	/* present, but not initted */
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
    GrMouseUpdateCursor();
}

/** Internal keyboard functions **/

static int _GetKey(void)
{
    unsigned short key;

    key = getkey();
    switch (key) {
        case 0x197: key = GrKey_Alt_Home;      break;
        case 0x198: key = GrKey_Alt_Up;        break;
        case 0x199: key = GrKey_Alt_PageUp;    break;
        case 0x19b: key = GrKey_Alt_Left;      break;
        case 0x19d: key = GrKey_Alt_Right;     break;
        case 0x19f: key = GrKey_Alt_End;       break;
        case 0x1a0: key = GrKey_Alt_Down;      break;
        case 0x1a1: key = GrKey_Alt_PageDown;  break;
        case 0x1a2: key = GrKey_Alt_Insert;    break;
        case 0x1a3: key = GrKey_Alt_Delete;    break;
    }
    return key;
}

static int _GetKbstat(void)
{
    Int86Regs r;

    sttzero(&r);
    IREG_AX(r) = (0x10 + 2) << 8;
    int16(&r);
    return(IREG_AL(r));
}

/** Internal mouse functions **/

static void _ReadMouseData(int *mb, int *mx, int *my)
{
    Int86Regs r;

    sttzero(&r);
    IREG_AX(r) = 11;                /* read mickey counters */
    int33(&r);
    *mx = (short)IREG_CX(r);
    *my = (short)IREG_DX(r);
    IREG_AX(r) = 3;                 /* read button state */
    int33(&r);
    *mb = IREG_BX(r);
}
