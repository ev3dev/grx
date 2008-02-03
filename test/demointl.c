/**
 ** demointl.c ---- MGRX Test intl support
 **
 ** Copyright (C) 2008 Mariano Alvarez Fernandez
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
#include "mgrx.h"
#include "mgrxkeys.h"
#include "gfaz.h"
#include "drawing.h"

/* default mode */

static int gwidth = 640;
static int gheight = 480;
static int gbpp = 16;

char *wintitle = "MGRX 0.9, the graphics library";

#define ID_FCP437      1
#define ID_FCP850      2
#define ID_FCP1252     3
#define ID_FISO88591   4
#define ID_SCP437      11
#define ID_SCP850      12
#define ID_SCP1252     13
#define ID_SISO88591   14
#define ID_SUTF8       15
#define ID_SUCS2       16
#define ID_EXIT        99


#define PX0 0
#define PX1 80
#define PY0 0
#define INCRY 42

#define NBUTTONSP1 11

static Button bp1[NBUTTONSP1] = {
    {PX0, PY0, 80, 40, IND_BLUE, IND_YELLOW, "CP437", BSTATUS_SELECTED, ID_FCP437},
    {PX1, PY0, 80, 40, IND_BLUE, IND_YELLOW, "CP850", 0, ID_FCP850},
    {PX0, PY0+INCRY*1, 80, 40, IND_BLUE, IND_YELLOW, "CP1252", 0, ID_FCP1252},
    {PX1, PY0+INCRY*1, 80, 40, IND_BLUE, IND_YELLOW, "8859-1", 0, ID_FISO88591},
    {PX0, PY0+INCRY*3, 80, 40, IND_GREEN, IND_YELLOW, "CP437", 0, ID_SCP437},
    {PX1, PY0+INCRY*3, 80, 40, IND_GREEN, IND_YELLOW, "CP850", 0, ID_SCP850},
    {PX0, PY0+INCRY*4, 80, 40, IND_GREEN, IND_YELLOW, "CP1252", 0, ID_SCP1252},
    {PX1, PY0+INCRY*4, 80, 40, IND_GREEN, IND_YELLOW, "8859-1", 0, ID_SISO88591},
    {PX0, PY0+INCRY*5, 80, 40, IND_GREEN, IND_YELLOW, "UTF-8", 0, ID_SUTF8},
    {PX1, PY0+INCRY*5, 80, 40, IND_GREEN, IND_YELLOW, "UCS-2", 0, ID_SUCS2},
    {PX0, PY0+INCRY*7, 160, 40, IND_RED, IND_WHITE, "Exit", 0, ID_EXIT}
};

static Button_Group bgp1 = { 10, 120, bp1, NBUTTONSP1, 0, 0 };

static Board brd =
    { 0, 0, 640, 480, IND_BLACK, IND_CYAN, IND_DARKGRAY, 1 };
static Board brdtxt =
    { 180, 80, 450, 390, IND_BLACK, IND_CYAN, IND_DARKGRAY, 1 };

static GrFont *grf_CP437;
static GrFont *grf_CP850;
static GrFont *grf_CP1252;
static GrFont *grf_ISO88591;
GrTextOption grt_text;
GrTextOption grt_info;

#define FONT_CP437 "pc8x16.fnt"
#define FONT_CP850 "pc8x16.fnt"  /* we need a real CP850 font */
#define FONT_CP1252 "ter-114b.res"
#define FONT_ISO88591 "ncen22b.fnt"

static char *stext_font = NULL;

static GrContext *grcglob = NULL;
static GrContext *grctext = NULL;
static int worg = 0, horg = 0;

static int xpos = 0, ypos = 0;
static int xdelta = 0, ydelta = 0;

/* Internal routines */

static void ini_graphics(void);
static void ini_objects(void);
static void paint_screen(void);
static void the_info(int x, int y);
static int pev_command(GrEvent * ev);
static void disaster(char *s);
static void drawchar(long ch);

/************************************************************************/

int main(int argc, char **argv)
{
    GrEvent ev;
    int i;

    if (argc >= 4) {
        gwidth = atoi(argv[1]);
        gheight = atoi(argv[2]);
        gbpp = atoi(argv[3]);
    }

    ini_graphics();
    GrSetWindowTitle(wintitle);
    ini_objects();
    paint_screen();

    while (1) {
        GrEventRead(&ev);
        if (ev.type == GREV_MOUSE) {
            ev.p2 -= worg;
            ev.p3 -= horg;
        }
        if (ev.type == GREV_END)
            break;
        if ((ev.type == GREV_KEY) && (ev.p1 == GrKey_Escape))
            break;
        if (pev_button_group(&ev, &bgp1))
            continue;
        if (pev_command(&ev))
            continue;
        if ((ev.type == GREV_KEY) && (ev.p1 == GrKey_Alt_A)) {
            if (GrGetKbEncoding() == GRENC_UTF_8)
              for (i=0; i<256; i++) drawchar(GrUCS2ToUTF8(i));
            else
              for (i=0; i<256; i++) drawchar(i);
            continue;
        }
        if ((ev.type == GREV_KEY) && (ev.p2 != GRKEY_KEYCODE)) {
            drawchar(ev.p1);
            continue;
        }
    }

    gfaz_fin();
    return 0;
}

/************************************************************************/

static void drawchar(long ch)
{
    GrContext ctxsave;

    GrSaveContext(&ctxsave);
    GrSetContext(grctext);
    GrDrawChar(ch, xpos, ypos, &grt_text);
//    GrDrawString((void *)&ch, 1, xpos, ypos, &grt_text);
    xpos += GrCharWidth(ch, &grt_text);
    if (xpos > brdtxt.wide-xdelta-8) {
        xpos = 0;
        if (ypos > brdtxt.high-2*ydelta-8 ) {
            GrBitBlt(NULL, 0, 0, NULL, 0, ydelta, GrMaxX(), GrMaxY(), GrWRITE);
            GrFilledBox(0, GrMaxY()-ydelta, GrMaxX(), GrMaxY(), DARKGRAY);
        }
        else
            ypos += ydelta;
    }
    GrSetContext(&ctxsave);
}

/************************************************************************/

static void ini_graphics(void)
{
    gfaz_ini(gwidth, gheight, gbpp);
    gwidth = GrScreenX();
    gheight = GrScreenY();
    grcglob = NULL;
    if (gwidth > 640 || gheight > 480) {
        GrClearScreen(GrAllocColor(120, 90, 60));
        worg = (gwidth - 640) / 2;
        horg = (gheight - 480) / 2;
        grcglob = GrCreateSubContext(worg, horg, worg + 639, horg + 479,
                                     NULL, NULL);
        GrSetContext(grcglob);
    }
    grctext = GrCreateSubContext(brdtxt.x + 4, brdtxt.y + 4,
                             brdtxt.x + brdtxt.wide - 5,
                             brdtxt.y + brdtxt.high - 5, grcglob, NULL);
}

/************************************************************************/

static void ini_objects(void)
{
    char aux[81];

    grf_CP437 = GrLoadFont(FONT_CP437);
    if (grf_CP437 == NULL) {
        sprintf(aux,"../fonts/%s", FONT_CP437);
        grf_CP437 = GrLoadFont(aux);
        if (grf_CP437 == NULL) {
            sprintf(aux,"%s not found", FONT_CP437);
            disaster(aux);
        }
    }

    grf_CP850 = grf_CP437;

    grf_CP1252 = GrLoadFont(FONT_CP1252);
    if (grf_CP1252 == NULL) {
        sprintf(aux,"../fonts/%s", FONT_CP1252);
        grf_CP1252 = GrLoadFont(aux);
        if (grf_CP1252 == NULL) {
            sprintf(aux,"%s not found", FONT_CP1252);
            disaster(aux);
        }
    }

    grf_ISO88591 = GrLoadFont(FONT_ISO88591);
    if (grf_ISO88591 == NULL) {
        sprintf(aux,"../fonts/%s", FONT_ISO88591);
        grf_ISO88591 = GrLoadFont(aux);
        if (grf_ISO88591 == NULL) {
            sprintf(aux,"%s not found", FONT_ISO88591);
            disaster(aux);
        }
    }

    stext_font = FONT_CP437;

    grt_text.txo_font = grf_CP437;
    grt_text.txo_fgcolor.v = WHITE;
    grt_text.txo_bgcolor.v = GrNOCOLOR;
    grt_text.txo_direct = GR_TEXT_RIGHT;
    grt_text.txo_xalign = GR_ALIGN_LEFT;
    grt_text.txo_yalign = GR_ALIGN_TOP;
    grt_text.txo_chrtype = GR_BYTE_TEXT;

    xdelta = GrCharWidth('M', &grt_text);
    ydelta = GrCharHeight('M', &grt_text);

    grt_info.txo_font = grf_CP437;
    grt_info.txo_fgcolor.v = CYAN;
    grt_info.txo_bgcolor.v = GrNOCOLOR;
    grt_info.txo_direct = GR_TEXT_RIGHT;
    grt_info.txo_xalign = GR_ALIGN_LEFT;
    grt_info.txo_yalign = GR_ALIGN_TOP;
    grt_info.txo_chrtype = GR_BYTE_TEXT;
}

/************************************************************************/

static void paint_screen(void)
{
    char aux[81];

    paint_board(&brd);

    sprintf(aux, "Set Test Font");
    GrDrawString(aux, strlen(aux), 10, 100, &grt_info);
    sprintf(aux, "Set KbEncoding");
    GrDrawString(aux, strlen(aux), 10, 220, &grt_info);

    paint_button_group(&bgp1);
    paint_board(&brdtxt);
    the_info(10, 6);
}

/************************************************************************/

static void the_info(int x, int y)
{
    char aux[81];
    char sys[4] = "?";
    int nsys;
    int nsysenc, nusrenc;

    nsys = GrGetLibrarySystem();
    if (nsys == MGRX_VERSION_GCC_386_DJGPP)
        strcpy(sys, "DJ2");
    else if (nsys == MGRX_VERSION_GCC_386_LINUX)
        strcpy(sys, "LNX");
    else if (nsys == MGRX_VERSION_GCC_386_X11)
        strcpy(sys, "X11");
    else if (nsys == MGRX_VERSION_GCC_X86_64_LINUX)
        strcpy(sys, "L64");
    else if (nsys == MGRX_VERSION_GCC_X86_64_X11)
        strcpy(sys, "X64");
    else if (nsys == MGRX_VERSION_GCC_386_WIN32)
        strcpy(sys, "W32");

    nsysenc = GrGetKbSysEncoding();
    nusrenc = GrGetKbEncoding();

    sprintf(aux, "Intl-test MGRX Version:%x System:%s",
            GrGetLibraryVersion(), sys);
    GrDrawString(aux, strlen(aux), 0 + x, 0 + y, &grt_info);

    sprintf(aux, "KbSysEncoding:%s KbEncoding:%s TextFont:%s",
            GrStrEncoding(nsysenc), GrStrEncoding(nusrenc), stext_font);
    GrDrawString(aux, strlen(aux), 0 + x, 20 + y, &grt_info);

    sprintf(aux, "VideoDriver: %s Mode: %dx%d %d bpp",
            GrCurrentVideoDriver()->name, GrCurrentVideoMode()->width,
            GrCurrentVideoMode()->height, GrCurrentVideoMode()->bpp);
    GrDrawString(aux, strlen(aux), 0 + x, 40 + y, &grt_info);
}

/************************************************************************/

static int pev_command(GrEvent * ev)
{
    if (ev->type == GREV_COMMAND) {
        if (ev->p1 == ID_EXIT) {
            par_event_queue(GREV_END, 0, 0, 0);
            return 1;
        }
        if (ev->p1 == ID_FCP437) {
            grt_text.txo_font = grf_CP437;
            stext_font = FONT_CP437;
            xpos = ypos = 0;
            xdelta = GrCharWidth('M', &grt_text);
            ydelta = GrCharHeight('M', &grt_text);
            paint_screen();
            return 1;
        }
        if (ev->p1 == ID_FCP850) {
            grt_text.txo_font = grf_CP850;
            stext_font = FONT_CP850;
            xpos = ypos = 0;
            xdelta = GrCharWidth('M', &grt_text);
            ydelta = GrCharHeight('M', &grt_text);
            paint_screen();
            return 1;
        }
        if (ev->p1 == ID_FCP1252) {
            grt_text.txo_font = grf_CP1252;
            stext_font = FONT_CP1252;
            xpos = ypos = 0;
            xdelta = GrCharWidth('M', &grt_text);
            ydelta = GrCharHeight('M', &grt_text);
            paint_screen();
            return 1;
        }
        if (ev->p1 == ID_FISO88591) {
            grt_text.txo_font = grf_ISO88591;
            stext_font = FONT_ISO88591;
            xpos = ypos = 0;
            xdelta = GrCharWidth('M', &grt_text);
            ydelta = GrCharHeight('M', &grt_text);
            paint_screen();
            return 1;
        }
        if (ev->p1 == ID_SCP437) {
            GrSetKbEncoding(GRENC_CP437);
            grt_text.txo_chrtype = GR_BYTE_TEXT;
            xpos = ypos = 0;
            paint_screen();
            return 1;
        }
        if (ev->p1 == ID_SCP850) {
            GrSetKbEncoding(GRENC_CP850);
            grt_text.txo_chrtype = GR_BYTE_TEXT;
            xpos = ypos = 0;
            paint_screen();
            return 1;
        }
        if (ev->p1 == ID_SCP1252) {
            GrSetKbEncoding(GRENC_CP1252);
            grt_text.txo_chrtype = GR_BYTE_TEXT;
            xpos = ypos = 0;
            paint_screen();
            return 1;
        }
        if (ev->p1 == ID_SISO88591) {
            GrSetKbEncoding(GRENC_ISO_8859_1);
            grt_text.txo_chrtype = GR_BYTE_TEXT;
            xpos = ypos = 0;
            paint_screen();
            return 1;
        }
        if (ev->p1 == ID_SUTF8) {
            GrSetKbEncoding(GRENC_UTF_8);
            grt_text.txo_chrtype = GR_UTF8_TEXT; /* will be GR_UTF8_TEXT */
            xpos = ypos = 0;
            paint_screen();
            return 1;
        }
        if (ev->p1 == ID_SUCS2) {
            GrSetKbEncoding(GRENC_UCS_2);
            grt_text.txo_chrtype = GR_WORD_TEXT;
            xpos = ypos = 0;
            paint_screen();
            return 1;
        }
    }
    return 0;
}

/************************************************************************/

static void disaster(char *s)
{
    void _GrCloseVideoDriver(void);

    char aux[81];

    gfaz_fin();
    _GrCloseVideoDriver();
    printf("Demointl: %s\n", s);
    printf("press Return to continue\n");
    fgets(aux, 80, stdin);
    exit(1);
}
