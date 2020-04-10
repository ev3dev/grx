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

static int gwidth = 800;
static int gheight = 600;
static int gbpp = 16;

#define XGUI 800
#define YGUI 600

char *wintitle = "MGRX 1.3.3, the graphics library";

#define ID_FCP437      1
#define ID_FCP850      2
#define ID_FCP1252     3
#define ID_FISO88591   4
#define ID_FMGRX       5
#define ID_SCP437      11
#define ID_SCP850      12
#define ID_SCP1252     13
#define ID_SISO88591   14
#define ID_SUTF8       15
#define ID_SUCS2       16
#define ID_TEST1       21
#define ID_TEST2       22
#define ID_TEST3       23
#define ID_EXIT        99

#define PX0 0
#define PX1 80
#define PY0 0
#define INCRY 42

#define NBUTTONSP1 15

static Button bp1[NBUTTONSP1] = {
    {PX0, PY0, 80, 40, IND_BLUE, IND_YELLOW, "CP437", BSTATUS_SELECTED, ID_FCP437},
    {PX1, PY0, 80, 40, IND_BLUE, IND_YELLOW, "CP850", 0, ID_FCP850},
    {PX0, PY0+INCRY*1, 80, 40, IND_BLUE, IND_YELLOW, "CP1252", 0, ID_FCP1252},
    {PX1, PY0+INCRY*1, 80, 40, IND_BLUE, IND_YELLOW, "8859-1", 0, ID_FISO88591},
    {PX0, PY0+INCRY*2, 80, 40, IND_BLUE, IND_YELLOW, "MGRX", 0, ID_FMGRX},
    {PX0, PY0+INCRY*4, 80, 40, IND_GREEN, IND_YELLOW, "CP437", 0, ID_SCP437},
    {PX1, PY0+INCRY*4, 80, 40, IND_GREEN, IND_YELLOW, "CP850", 0, ID_SCP850},
    {PX0, PY0+INCRY*5, 80, 40, IND_GREEN, IND_YELLOW, "CP1252", 0, ID_SCP1252},
    {PX1, PY0+INCRY*5, 80, 40, IND_GREEN, IND_YELLOW, "8859-1", 0, ID_SISO88591},
    {PX0, PY0+INCRY*6, 80, 40, IND_GREEN, IND_YELLOW, "UTF-8", 0, ID_SUTF8},
    {PX1, PY0+INCRY*6, 80, 40, IND_GREEN, IND_YELLOW, "UCS-2", 0, ID_SUCS2},
    {PX0, PY0+INCRY*7.5, 80, 40, IND_BROWN, IND_YELLOW, "Glyphs", 0, ID_TEST1},
    {PX1, PY0+INCRY*7.5, 80, 40, IND_BROWN, IND_YELLOW, "Chars", 0, ID_TEST2},
    {PX0, PY0+INCRY*8.5, 80, 40, IND_BROWN, IND_YELLOW, "Strings", 0, ID_TEST3},
    {PX0, PY0+INCRY*10, 160, 40, IND_RED, IND_WHITE, "Exit", 0, ID_EXIT}
};

static Button_Group bgp1 = { 10, 120, bp1, NBUTTONSP1, 0, 0 };

static Board brd =
    { 0, 0, XGUI, YGUI, IND_BLACK, IND_CYAN, IND_DARKGRAY, 1 };
static Board brdtxt =
    { 180, 80, XGUI-190, YGUI-90, IND_BLACK, IND_CYAN, IND_BLACK, 1 };

static GrFont *grf_CP437;
static GrFont *grf_CP850;
static GrFont *grf_CP1252;
static GrFont *grf_ISO88591;
static GrFont *grf_MGRX;
GrTextOption grt_text;
GrTextOption grt_info;

#define FONT_CP437 "pc8x16.fnt"
#define FONT_CP850 "pc850-19.fnt"
#define FONT_CP1252 "ter-114b.res"
#define FONT_ISO88591 "ncen22b.fnt"
#define FONT_MGRX "tmgrx22b.fnt"

static char *stext_font = NULL;

static GrContext *grcglob = NULL;
static GrContext *grctext = NULL;
static int worg = 0, horg = 0;

static int xpos = 0, ypos = 0;
static int xdelta = 0, ydelta = 0;

static int tcursor_status = -1;

/* Internal routines */

static void ini_graphics(void);
static void ini_objects(void);
static void paint_screen(void);
static void the_info(int x, int y);
static void draw_string_collection(void);
static int pev_command(GrEvent * ev);
static void disaster(char *s);
static void salta_linea(void);
static void drawchar(long ch);
void show_tcursor(void);
void hide_tcursor(void);
void dotest(int n);

/************************************************************************/

int main(int argc, char **argv)
{
    GrEvent ev;

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
            dotest(1);
            continue;
        }
        if ((ev.type == GREV_KEY) && (ev.p1 == GrKey_Alt_B)) {
            dotest(2);
            continue;
        }
        if ((ev.type == GREV_KEY) && (ev.p1 == GrKey_Alt_C)) {
            dotest(3);
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

static void salta_linea(void)
{
    GrContext ctxsave;

    GrSaveContext(&ctxsave);
    GrSetContext(grctext);

    xpos = 0;
    if (ypos > brdtxt.high-2*ydelta-8 ) {
        GrBitBlt(NULL, 0, 0, NULL, 0, ydelta, GrMaxX(), GrMaxY(), GrWRITE);
        GrFilledBox(0, GrMaxY()-ydelta, GrMaxX(), GrMaxY(), BLACK);
    }
    else
        ypos += ydelta;

    GrSetContext(&ctxsave);
}
/************************************************************************/

static void drawchar(long ch)
{
    GrContext ctxsave;

    GrSaveContext(&ctxsave);
    GrSetContext(grctext);
    hide_tcursor();
    GrDrawChar(ch, xpos, ypos, &grt_text);
    xpos += GrCharWidth(ch, &grt_text);
    if (xpos > brdtxt.wide-xdelta-8) salta_linea();
    show_tcursor();
    GrSetContext(&ctxsave);
}

/************************************************************************/

void draw_tcursor(GrColor fg)
{
    GrContext ctxsave;
    GrColor aux;

    GrSaveContext(&ctxsave);
    GrSetContext(grctext);
    aux = grt_text.txo_fgcolor;
    grt_text.txo_fgcolor = fg;
    GrDrawChar('_', xpos, ypos, &grt_text);
    grt_text.txo_fgcolor = aux;
    GrSetContext(&ctxsave);
}

void show_tcursor(void)
{
    tcursor_status++;
    
    if (tcursor_status == 0) {
      draw_tcursor(YELLOW);
    }
}

void hide_tcursor(void)
{
    tcursor_status--;
    
    if (tcursor_status == -1) {
      draw_tcursor(BLACK);
    }
}

/************************************************************************/

static void ini_graphics(void)
{
    gfaz_ini(gwidth, gheight, gbpp);
    gwidth = GrScreenX();
    gheight = GrScreenY();
    grcglob = NULL;
    if (gwidth > XGUI || gheight > YGUI) {
        GrClearScreen(GrAllocColor(120, 90, 60));
        worg = (gwidth - XGUI) / 2;
        horg = (gheight - YGUI) / 2;
        grcglob = GrCreateSubContext(worg, horg, worg + XGUI-1, horg + YGUI-1,
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
    GrFontSetEncoding(grf_CP437, GR_FONTENC_CP437);

//    grf_CP850 = grf_CP437;
    grf_CP850 = GrLoadFont(FONT_CP850);
    if (grf_CP850 == NULL) {
        sprintf(aux,"../fonts/%s", FONT_CP850);
        grf_CP850 = GrLoadFont(aux);
        if (grf_CP850 == NULL) {
            sprintf(aux,"%s not found", FONT_CP850);
            disaster(aux);
        }
    }
    GrFontSetEncoding(grf_CP850, GR_FONTENC_CP850);

    grf_CP1252 = GrLoadFont(FONT_CP1252);
    if (grf_CP1252 == NULL) {
        sprintf(aux,"../fonts/%s", FONT_CP1252);
        grf_CP1252 = GrLoadFont(aux);
        if (grf_CP1252 == NULL) {
            sprintf(aux,"%s not found", FONT_CP1252);
            disaster(aux);
        }
    }
    GrFontSetEncoding(grf_CP1252, GR_FONTENC_CP1252);

    grf_ISO88591 = GrLoadFont(FONT_ISO88591);
    if (grf_ISO88591 == NULL) {
        sprintf(aux,"../fonts/%s", FONT_ISO88591);
        grf_ISO88591 = GrLoadFont(aux);
        if (grf_ISO88591 == NULL) {
            sprintf(aux,"%s not found", FONT_ISO88591);
            disaster(aux);
        }
    }
    GrFontSetEncoding(grf_ISO88591, GR_FONTENC_ISO_8859_1);

    grf_MGRX = GrLoadFont(FONT_MGRX);
    if (grf_MGRX == NULL) {
        sprintf(aux,"../fonts/%s", FONT_MGRX);
        grf_MGRX = GrLoadFont(aux);
        if (grf_MGRX == NULL) {
            sprintf(aux,"%s not found", FONT_MGRX);
            disaster(aux);
        }
    }
    GrFontSetEncoding(grf_MGRX, GR_FONTENC_MGRX512);

    stext_font = FONT_CP437;

    grt_text.txo_font = grf_CP437;
    grt_text.txo_fgcolor = WHITE;
    grt_text.txo_bgcolor = GrNOCOLOR;
    grt_text.txo_direct = GR_TEXT_RIGHT;
    grt_text.txo_xalign = GR_ALIGN_LEFT;
    grt_text.txo_yalign = GR_ALIGN_TOP;
    grt_text.txo_chrtype = GrGetChrtypeForUserEncoding();

    xdelta = GrCharWidth('M', &grt_text);
    ydelta = GrCharHeight('M', &grt_text);

    grt_info.txo_font = grf_CP437;
    grt_info.txo_fgcolor = LIGHTCYAN;
    grt_info.txo_bgcolor = GrNOCOLOR;
    grt_info.txo_direct = GR_TEXT_RIGHT;
    grt_info.txo_xalign = GR_ALIGN_LEFT;
    grt_info.txo_yalign = GR_ALIGN_TOP;
//    grt_info.txo_chrtype = GrGetChrtypeForUserEncoding();
    grt_info.txo_chrtype = GR_UTF8_TEXT; /* this source file is UTF-8 coded!! */
}

/************************************************************************/

static void paint_screen(void)
{
    char aux[81];

    paint_board(&brd);

    sprintf(aux, "Set Test Font");
    GrDrawString(aux, strlen(aux), 10, 100, &grt_info);
    sprintf(aux, "Set UserEncoding");
    GrDrawString(aux, strlen(aux), 10, 268, &grt_info);

    paint_button_group(&bgp1);
    paint_board(&brdtxt);
    the_info(10, 6);
    
    tcursor_status = -1;
    show_tcursor();
}

/************************************************************************/

static void the_info(int x, int y)
{
    char aux[101];
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
    else if (nsys == MGRX_VERSION_GCC_ARM_LINUX)
        strcpy(sys, "LAR");
    else if (nsys == MGRX_VERSION_GCC_ARM_X11)
        strcpy(sys, "XAR");

    nsysenc = GrGetKbSysEncoding();
    nusrenc = GrGetUserEncoding();

    sprintf(aux, "Intl-test MGRX Version: %x  System: %s  KbSysEncoding: %s",
            GrGetLibraryVersion(), sys, GrStrEncoding(nsysenc));
    GrDrawString(aux, strlen(aux), 0 + x, 0 + y, &grt_info);

    sprintf(aux, "UserEncoding: %s  TextFont: %s  FontEncoding: %d (%s)",
            GrStrEncoding(nusrenc), stext_font, grt_text.txo_font->h.encoding,
            GrStrFontEncoding(grt_text.txo_font->h.encoding));
    GrDrawString(aux, strlen(aux), 0 + x, 20 + y, &grt_info);

    sprintf(aux, "VideoDriver: %s  Mode: %dx%d %d bpp",
            GrCurrentVideoDriver()->name, GrCurrentVideoMode()->width,
            GrCurrentVideoMode()->height, GrCurrentVideoMode()->bpp);
    GrDrawString(aux, strlen(aux), 0 + x, 40 + y, &grt_info);
}

/************************************************************************/

static void draw_string_collection(void)
{
    unsigned char *scp437 =
      (unsigned char *)"CP437 string (\xa5\xa4 -\xa0\x90\x82-\xa1-\xa2-\xa3 - \xc3\xc4\xb4)";
    unsigned char *scp850 =
      (unsigned char *)"CP850 string (\xa5\xa4 \xb5\xa0\x90\x82\xd6\xa1\xe0\xa2\xe9\xa3 - \xc3\xc4\xb4)";
    unsigned char *scp1252 =
      (unsigned char *)"CP1252 string (\xd1\xf1 \xc1\xe1\xc9\xe9\xcd\xed\xd3\xf3\xda\xfa \x80 ---)";
    unsigned char *siso88591 =
      (unsigned char *)"ISO-8859-1 string (\xd1\xf1 \xc1\xe1\xc9\xe9\xcd\xed\xd3\xf3\xda\xfa - ---)";
    unsigned char *sutf8 =
      (unsigned char *)"UTF-8 string (Ññ ÁáÉéÍíÓóÚú € ├─┤)";
    int len, i, nb, aux;
    long ch;
    unsigned char *s;
    
    aux = grt_text.txo_chrtype;

    if (xpos > 0) salta_linea();
    grt_text.txo_chrtype = GR_CP437_TEXT;
    len = strlen((char *)scp437);
    for (i=0; i<len; i++) {
        drawchar(scp437[i]);
    }

    if (xpos > 0) salta_linea();
    grt_text.txo_chrtype = GR_CP850_TEXT;
    len = strlen((char *)scp850);
    for (i=0; i<len; i++) {
        drawchar(scp850[i]);
    }

    if (xpos > 0) salta_linea();
    grt_text.txo_chrtype = GR_CP1252_TEXT;
    len = strlen((char *)scp1252);
    for (i=0; i<len; i++) {
        drawchar(scp1252[i]);
    }

    if (xpos > 0) salta_linea();
    grt_text.txo_chrtype = GR_ISO_8859_1_TEXT;
    len = strlen((char *)siso88591);
    for (i=0; i<len; i++) {
        drawchar(siso88591[i]);
    }

    if (xpos > 0) salta_linea();
    grt_text.txo_chrtype = GR_UTF8_TEXT;
    len = GrUTF8StrLen((unsigned char *)sutf8);
    s = sutf8;
    for (i=0; i<len; i++) {
        ch = GrNextUTF8Char(s, &nb);
        drawchar(ch);
        s += nb;
    }
    
    grt_text.txo_chrtype = aux;
}

/************************************************************************/

static int pev_command(GrEvent * ev)
{
    if (ev->type == GREV_COMMAND) {
        if (ev->p1 == ID_EXIT) {
            GrEventParEnqueue(GREV_END, 0, 0, 0, 0);
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
        if (ev->p1 == ID_FMGRX) {
            grt_text.txo_font = grf_MGRX;
            stext_font = FONT_MGRX;
            xpos = ypos = 0;
            xdelta = GrCharWidth('M', &grt_text);
            ydelta = GrCharHeight('M', &grt_text);
            paint_screen();
            return 1;
        }
        if (ev->p1 == ID_SCP437) {
            GrSetUserEncoding(GRENC_CP437);
            grt_text.txo_chrtype = GR_CP437_TEXT;
            xpos = ypos = 0;
            paint_screen();
            return 1;
        }
        if (ev->p1 == ID_SCP850) {
            GrSetUserEncoding(GRENC_CP850);
            grt_text.txo_chrtype = GR_CP850_TEXT;
            xpos = ypos = 0;
            paint_screen();
            return 1;
        }
        if (ev->p1 == ID_SCP1252) {
            GrSetUserEncoding(GRENC_CP1252);
            grt_text.txo_chrtype = GR_CP1252_TEXT;
            xpos = ypos = 0;
            paint_screen();
            return 1;
        }
        if (ev->p1 == ID_SISO88591) {
            GrSetUserEncoding(GRENC_ISO_8859_1);
            grt_text.txo_chrtype = GR_ISO_8859_1_TEXT;
            xpos = ypos = 0;
            paint_screen();
            return 1;
        }
        if (ev->p1 == ID_SUTF8) {
            GrSetUserEncoding(GRENC_UTF_8);
            grt_text.txo_chrtype = GR_UTF8_TEXT;
            xpos = ypos = 0;
            paint_screen();
            return 1;
        }
        if (ev->p1 == ID_SUCS2) {
            GrSetUserEncoding(GRENC_UCS_2);
            grt_text.txo_chrtype = GR_UCS2_TEXT;
            xpos = ypos = 0;
            paint_screen();
            return 1;
        }
        if (ev->p1 == ID_TEST1) {
            dotest(1);
            return 1;
        }
        if (ev->p1 == ID_TEST2) {
            dotest(2);
            return 1;
        }
        if (ev->p1 == ID_TEST3) {
            dotest(3);
            return 1;
        }
    }
    return 0;
}

/************************************************************************/

void dotest(int n)
{
    int i, aux;
    
    hide_tcursor();
    
    switch (n) {
        case 1:
            if (xpos > 0) salta_linea();
            aux = grt_text.txo_chrtype;
            grt_text.txo_chrtype = GR_BYTE_TEXT;
            for (i=0; i<(grt_text.txo_font->h.minchar +
                grt_text.txo_font->h.numchars); i++) drawchar(i);
            grt_text.txo_chrtype = aux;
            break;
        case 2:
            if (xpos > 0) salta_linea();
            if (GrGetUserEncoding() == GRENC_UTF_8)
              for (i=0; i<256; i++) drawchar(GrUCS2ToUTF8(i));
            else
              for (i=0; i<256; i++) drawchar(i);
            break;
        case 3:
            draw_string_collection();
            break;
    }
    
    show_tcursor();
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
