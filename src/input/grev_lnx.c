/**
 ** grev_lnx.c ---- MGRX events, linux console input
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
 ** 190803 M.Alvarez, added support for imps2 mouse protocol (we have the wheel)
 ** 190804 M.Alvarez, changed termio by termios, solve problems with control keys
 **
 **/

#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>

#include "libgrx.h"
#include "ninput.h"
#include "arith.h"
#include "memcopy.h"
#include "memfill.h"

#include "mgrxkeys.h"

static struct termios kbd_nsetup;
static struct termios kbd_nreset;
static int kbd_initted = FALSE;
static int kbd_isatty;
static int kbd_lastchr;
static int kbd_filedsc;
static enum { normal, test, wait } kbd_mode;

#define PS2_LEFTBUTTON   1
#define PS2_MIDDLEBUTTON 4
#define PS2_RIGHTBUTTON  2

static int mou_filedsc;
static int mou_buttons;

extern int _lnxfb_waiting_to_switch_console;
extern void _LnxfbSwitchConsoleAndWait(void);

static int kbd_lastmod = 0;
static int kbsysencoding = 0;

static void kbd_restore(void);
static void kbd_init(void);
static int getByte(int remove);
static unsigned short keytrans(unsigned short k, const unsigned short * t);
static int inkey(void);
static int validKey(int key, int valid);
static int _CheckKeyboardHit(void);
static int _ReadCharFromKeyboard(void);
static int _TryToSetImps2_Mode(void);
static int _ReadPS2MouseData(int *mb, int *mx, int *my, int *wh);

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

    kbd_init();
    if (GrMouseDetect()) {
        mou_buttons = 0;
    }

    s = getenv("LANG");
    if (strstr(s,"UTF-8"))
      kbsysencoding = GRENC_UTF_8;
    else {
      s = getenv("MGRXKBSYSENCODING");
      if (s != NULL) kbsysencoding = GrFindEncoding(s);
      if (kbsysencoding < 0) kbsysencoding = GRENC_ISO_8859_1;
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
    kbd_restore();
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
    int mb = 0, mx = 0, my = 0, wh = 0;

    if (_lnxfb_waiting_to_switch_console)
        _LnxfbSwitchConsoleAndWait();

    if (MOUINFO->msstatus == 2) {
        if (_ReadPS2MouseData(&mb, &mx, &my, &wh)) {
            update_coord(x, mx);
            update_coord(y, my);
            /** NOTE we know PS2_LEFTBUTTON == GRMOUSE_LB_STATUS, etc */
            MOUINFO->bstatus = mb;
            GrMouseUpdateCursor();
            MOUINFO->moved  = TRUE;
            if (mb != mou_buttons || wh != 0) {
                evaux.type = GREV_MOUSE;
                evaux.kbstat = kbd_lastmod;
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
                if (wh < 0) {
                    evaux.p1 = GRMOUSE_B4_PRESSED;
                    GrEventEnqueue(&evaux);
                    evaux.p1 = GRMOUSE_B4_RELEASED;
                    GrEventEnqueue(&evaux);
                    nev += 2;
                }
                if (wh > 0) {
                    evaux.p1 = GRMOUSE_B5_PRESSED;
                    GrEventEnqueue(&evaux);
                    evaux.p1 = GRMOUSE_B5_RELEASED;
                    GrEventEnqueue(&evaux);
                    nev += 2;
                }
                mou_buttons = mb;
                MOUINFO->moved = FALSE;
            }
            else if ((MOUINFO->genmmove == GR_GEN_MMOVE_ALWAYS) ||
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
        }
    }

    if (kbd_isatty && _CheckKeyboardHit()) {
        unsigned int key = _ReadCharFromKeyboard();
        evaux.type = GREV_PREKEY;
        evaux.kbstat = kbd_lastmod;
        evaux.p3 = 0;
        if (key > 0xff) {
            evaux.p1 = key;
            evaux.p2 = GRKEY_KEYCODE;
        }
        else if ((key >= 0x80) && (kbsysencoding == GRENC_UTF_8)) {
            evaux.p1 = 0;
            if ((key & 0xe0) == 0xc0) {  /* two bytes */
                evaux.cp1[0] = key;
                evaux.cp1[1] = _ReadCharFromKeyboard();
                evaux.p2 = 2;
            }
            else if ((key & 0xf0) == 0xe0) {  /* three bytes */
                evaux.cp1[0] = key;
                evaux.cp1[1] = _ReadCharFromKeyboard();
                evaux.cp1[2] = _ReadCharFromKeyboard();
                evaux.p2 = 3;
            }
            else {  /* four bytes */
                evaux.cp1[0] = key;
                evaux.cp1[1] = _ReadCharFromKeyboard();
                evaux.cp1[2] = _ReadCharFromKeyboard();
                evaux.cp1[3] = _ReadCharFromKeyboard();
                evaux.p2 = 3;
            }
        }
        else {
            evaux.p1 = key;
            evaux.p2 = 1;
        }
        GrEventEnqueue(&evaux);
        MOUINFO->moved = FALSE;
        nev++;
    }

    if (nev == 0) {
        usleep(1000L);   // wait 1 ms to not eat 100% cpu
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
    MOUINFO->msstatus = (-1);        /* assume missing */
    mou_filedsc = open("/dev/input/mice", O_RDWR | O_NDELAY);
    if (mou_filedsc < 0)
        mou_filedsc = open("/dev/psaux", O_RDWR | O_NDELAY);
    if (mou_filedsc >= 0) {
        MOUINFO->msstatus = 1;        /* present, but not initted */
        _TryToSetImps2_Mode();
        // although now we know if we have an imps2 mouse, let _ReadPS2MouseData
        // to adapt itself to the packet size (3 for ps2, 4 for imps2)
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

static void kbd_restore(void)
{
    if (kbd_initted && kbd_isatty && (kbd_mode != normal)) {
        tcsetattr(kbd_filedsc, 0, &kbd_nreset);
        kbd_mode = normal;
    }
}

static void kbd_init(void)
{
    if (!kbd_initted) {
        kbd_initted = TRUE;
        kbd_lastchr = EOF;
        kbd_filedsc = fileno(stdin);
        kbd_isatty = isatty(kbd_filedsc);
        if (kbd_isatty) {
            tcgetattr(kbd_filedsc, &kbd_nreset);
            tcgetattr(kbd_filedsc, &kbd_nsetup);
            kbd_nsetup.c_lflag &= ~((tcflag_t)(ICANON | ISIG | ECHO | ECHOCTL));
            kbd_nsetup.c_iflag = 0;
            atexit(kbd_restore);
            kbd_mode = normal;
        }
    }
}

#define CHRBUFSIZE 30

static int chrspending = 0;
static int getByte(int remove)
{
    static unsigned char keybuf[CHRBUFSIZE];
    int ch, f;

    if (chrspending < 0) chrspending = 0;
    
    if (chrspending == 0) {
        f = read(kbd_filedsc, keybuf, CHRBUFSIZE);
        if (f > 0) {
            chrspending += f;
        }
    }

    if (chrspending <= 0) return EOF;

    ch = keybuf[0];
    if (remove) {
        --chrspending;
        memmove(&keybuf[0], &keybuf[1], chrspending * sizeof(unsigned char));
    }
    return ch;
}

static const unsigned short fnk[] = {
    GrKey_F6, GrKey_F7, GrKey_F8, GrKey_F9, GrKey_F10, GrKey_Escape,
    /* shift F1 == F11, shift F2 == F12  What code should be returned ?? */
    /* M.Alvarez 190807 we return F11, F12 now all kb has 12 function keys */
    GrKey_F11, GrKey_F12,
    GrKey_Shift_F1, GrKey_Shift_F2,
    GrKey_Escape, GrKey_Shift_F3, GrKey_Shift_F4, GrKey_Escape,
    GrKey_Shift_F5, GrKey_Shift_F6, GrKey_Shift_F7, GrKey_Shift_F8
};

static const unsigned short alt[] = {
    GrKey_Escape, GrKey_Alt_Escape,
    0x7f, GrKey_Alt_Backspace,
    'q', GrKey_Alt_Q,
    'w', GrKey_Alt_W,
    'e', GrKey_Alt_E,
    'r', GrKey_Alt_R,
    't', GrKey_Alt_T,
    'y', GrKey_Alt_Y,
    'u', GrKey_Alt_U,
    'i', GrKey_Alt_I,
    'o', GrKey_Alt_O,
    'p', GrKey_Alt_P,
    '[', GrKey_Alt_LBracket,
    ']', GrKey_Alt_RBracket,
    GrKey_Return, GrKey_Alt_Return,
    'a', GrKey_Alt_A,
    's', GrKey_Alt_S,
    'd', GrKey_Alt_D,
    'f', GrKey_Alt_F,
    'g', GrKey_Alt_G,
    'h', GrKey_Alt_H,
    'j', GrKey_Alt_J,
    'k', GrKey_Alt_K,
    'l', GrKey_Alt_L,
    ';', GrKey_Alt_Semicolon,
    '\'', GrKey_Alt_Quote,
    '\x60', GrKey_Alt_Backquote,
    '\\', GrKey_Alt_Backslash,
    'z', GrKey_Alt_Z,
    'x', GrKey_Alt_X,
    'c', GrKey_Alt_C,
    'v', GrKey_Alt_V,
    'b', GrKey_Alt_B,
    'n', GrKey_Alt_N,
    'm', GrKey_Alt_M,
    ',', GrKey_Alt_Comma,
    '.', GrKey_Alt_Period,
    '/', GrKey_Alt_Slash,
/*                GrKey_Alt_F1 */
/*                GrKey_Alt_F2 */
/*                GrKey_Alt_F3 */
/*                GrKey_Alt_F4 */
/*                GrKey_Alt_F5 */
/*                GrKey_Alt_F6 */
/*                GrKey_Alt_F7 */
/*                GrKey_Alt_F8 */
/*                GrKey_Alt_F9 */
/*                GrKey_Alt_F10 */
    '1', GrKey_Alt_1,
    '2', GrKey_Alt_2,
    '3', GrKey_Alt_3,
    '4', GrKey_Alt_4,
    '5', GrKey_Alt_5,
    '6', GrKey_Alt_6,
    '7', GrKey_Alt_7,
    '8', GrKey_Alt_8,
    '9', GrKey_Alt_9,
    '0', GrKey_Alt_0,
    '-', GrKey_Alt_Dash,
    '=', GrKey_Alt_Equals,
/*                GrKey_Alt_F11 */
/*                GrKey_Alt_F12 */
/*                GrKey_Alt_KPSlash */
    GrKey_Tab, GrKey_Alt_Tab,
/*                GrKey_Alt_Enter */
    '<', GrKey_Alt_LAngle,
    '>', GrKey_Alt_RAngle,
    '@', GrKey_Alt_At,
    '{', GrKey_Alt_LBrace,
    '|', GrKey_Alt_Pipe,
    '}', GrKey_Alt_RBrace,
    0x0000, GrKey_NoKey
};

static unsigned short keytrans(unsigned short k, const unsigned short * t)
{
    while (t[0] != 0x0000 && t[1] != GrKey_NoKey) {
        if (k == t[0])
            return t[1];
        t += 2;
    }
    return GrKey_OutsideValidRange;
}

static int inkey(void)
{
    int Key, Key2;

  Restart:
    Key = getByte(1);
    if (Key != GrKey_Escape) {
        if (Key == 0x7f)
            return (GrKey_BackSpace);
        return (Key);
    }
    if (chrspending == 0)
        return (GrKey_Escape);
    /* We have ^[ and something more after that */

    Key = getByte(1);
    if (Key == EOF)
        return (GrKey_Escape);
    if (Key != '[')
        return (keytrans(Key, alt));        /* Check for Alt+Key */

    /* We have ^[[ */
    if (chrspending == 0)
        return (GrKey_Alt_LBracket);

    Key = getByte(1);
    if (Key == EOF)
        return (GrKey_Alt_LBracket);;
    switch (Key) {
    case 'A':
        return (GrKey_Up);
    case 'B':
        return (GrKey_Down);
    case 'C':
        return (GrKey_Right);
    case 'D':
        return (GrKey_Left);
    case 'G':
        return (GrKey_Center);
    case '[':
        Key = getByte(1);
        switch (Key) {
        case 'A':
            return (GrKey_F1);
        case 'B':
            return (GrKey_F2);
        case 'C':
            return (GrKey_F3);
        case 'D':
            return (GrKey_F4);
        case 'E':
            return (GrKey_F5);
        default:
            goto Restart;
        }
    default:
        if (Key >= '1' && Key <= '6') {
            Key2 = getByte(1);
            if (Key2 == '~')
                switch (Key) {
                case '1':
                    return (GrKey_Home);
                case '2':
                    return (GrKey_Insert);
                case '3':
                    return (GrKey_Delete);
                case '4':
                    return (GrKey_End);
                case '5':
                    return (GrKey_PageUp);
                case '6':
                    return (GrKey_PageDown);
            } else if (Key2 >= '0' && Key2 <= '9') {
                int index = 10 * (Key - '0') + (Key2 - '0');
                if (getByte(1) != '~')
                    goto Restart;
                if (17 <= index && index <= 34)
                    return (fnk[index - 17]);
                goto Restart;
            }
        }
        goto Restart;
    }
}

static int validKey(int key, int valid)
{
    if (key < 0 || key > GrKey_LastDefinedKeycode || key == EOF)
        return valid;
    return key;
}

static int _CheckKeyboardHit(void)
{
    if (_lnxfb_waiting_to_switch_console)
        _LnxfbSwitchConsoleAndWait();

    if (!kbd_initted) {
        kbd_init();
    }
    if (!kbd_isatty) {
        return (TRUE);
    }
    if (kbd_lastchr != EOF) {
        return (TRUE);
    }
    if (kbd_mode != test) {
        kbd_nsetup.c_cc[VMIN] = 0;
        kbd_nsetup.c_cc[VTIME] = 0;
        if (tcsetattr(kbd_filedsc, TCSADRAIN, &kbd_nsetup) == -1)
            return (FALSE);
        kbd_mode = test;
    }
    kbd_lastchr = validKey(inkey(), EOF);
    return (kbd_lastchr != EOF);
}

static int _ReadCharFromKeyboard(void)
{
    int key = EOF;

    if (!kbd_initted) {
        kbd_init();
    }
    if (!kbd_isatty) {
        return (getc(stdin));
    }
    do {
        if (_lnxfb_waiting_to_switch_console)
            _LnxfbSwitchConsoleAndWait();

        if (kbd_lastchr != EOF) {
            key = kbd_lastchr;
            kbd_lastchr = EOF;
            break;
        }
        if (kbd_mode == test) {
            /* we're in test mode. Look for key without mode switch ... */
            _CheckKeyboardHit();
            if (kbd_lastchr != EOF) {
                key = kbd_lastchr;
                kbd_lastchr = EOF;
                break;
            }
        }
        /* no key till now. Wait of it ... */
        if (kbd_mode != wait) {
            kbd_nsetup.c_cc[VMIN] = 0;
            kbd_nsetup.c_cc[VTIME] = 1;
            if (tcsetattr(kbd_filedsc, TCSADRAIN, &kbd_nsetup) == -1)
                return (EOF);
            kbd_mode = wait;
        }
        if ((key = inkey()) != EOF) break;
    } while (1);
    return validKey(key, GrKey_OutsideValidRange);
}

/** Internal mouse functions **/

static int _TryToSetImps2_Mode(void)
{
    unsigned char buf_imps2[6] = {0xf3, 0xc8, 0xf3, 0x64, 0xf3, 0x50}; 
    unsigned char bufw[2], bufr[256] = {0};
    int ret;

    // reset the mouse
    bufw[0] = 0xff;
    write(mou_filedsc, bufw, 1);
    usleep( 10000L );
    while (read(mou_filedsc, bufr, 1) == 1);
    
    // send magic string to set imps2 mode (to have wheell events)
    write(mou_filedsc, buf_imps2, 6);
    usleep( 10000L );
    while (read(mou_filedsc, bufr, 1) == 1);

    // ask mouse ID (0=normal, 2=imps2)
    bufw[0] = 0xf2;
    write(mou_filedsc, bufw, 1);
    usleep( 10000L );
    read(mou_filedsc, bufr, 1);
    read(mou_filedsc, bufr+1, 1);
    ret = bufr[1];

    // enable mouse
    bufw[0] = 0xf4;
    write(mou_filedsc, bufw, 1);
    usleep( 10000L );
    read(mou_filedsc, bufr, 1);

    return ret;
}
    
static int _ReadPS2MouseData(int *mb, int *mx, int *my, int *wh)
{
    static int imps2 = 0;
    static int count_to_imps2 = 0;
    static int last_packed_ok = 0;
    unsigned char buf[4];
    int evr = 0;

    *mx = *my = 0;
    *wh = 0;
    do {
        if (read(mou_filedsc, buf, 1) < 1)
            return evr;
        if (!(buf[0] & 0x08)) {        /* sync lost */
            if (!imps2) {
                if (last_packed_ok) {        /* an IMPS2 mouse? */
                    count_to_imps2++;
                    if (count_to_imps2 > 20)
                        imps2 = 1;
                } else
                    count_to_imps2 = 0;
                last_packed_ok = 0;
            }
            return evr;
        } else if ((buf[0] & 0xF8) == 0xF8) { /* mmmm, can be the wheel packet */
            return evr;                       /* and sync is lost */
        }
        while (read(mou_filedsc, buf + 1, 1) != 1);
        while (read(mou_filedsc, buf + 2, 1) != 1);
        if (imps2) {/* the IMPS2 protocol is 4 bytes wide */
            while (read(mou_filedsc, buf + 3, 1) != 1);
            *wh = (buf[3] & 7) * ((buf[3] & 8)? -1 : 1);
        }
        *mb = buf[0] & 0x7;
        *mx += (buf[0] & 0x10) ? buf[1] - 256 : buf[1];
        *my += (buf[0] & 0x20) ? -(buf[2] - 256) : -buf[2];
        last_packed_ok = 1;
        evr = 1;
    } while (*mb == mou_buttons);
    return evr;
}
