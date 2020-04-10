/**
 ** demmgrx2.c ---- MGRX Test programs launcher using partially the new
 **                 grgui, only objects and dialogs, drawing and double
 **                 buffering managed directly, like the old demomgrx.c
 **
 ** Copyright (C) 2000,2001,2005,2008,2019 Mariano Alvarez Fernandez
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
//#include "mgrx.h"
//#include "mgrxkeys.h"
#include "grgui.h"
#include "mgrxcolr.h"

/* default mode */

static int gwidth = 640;
static int gheight = 480;
static int gbpp = 16;

char *wintitle =
    "MGRX+GrGUI 1.3.3, the graphics library";

char *animatedtext[2] = {
    "MGRX 1.3.3, the graphics library for DJGPPv2, Linux, X11 and Win32",
    "Hello world    Привет мир    Γειά σου Κόσμε    Hola mundo" };

#if defined(__XWIN__) || defined(__WIN32__)
static int need_restart = 0;
#else
static int need_restart = 1;
#endif

#if defined(__MSDOS__) || defined(__WIN32__)
#define PPMIMG1   "..\\testimg\\pnmtest.ppm"
#define PPMIMG2   "..\\testimg\\pnmtest2.ppm"
#define JPGIMGBG  "..\\testimg\\jpeg3.jpg"
#define PPMIMGOUT "..\\testimg\\demmgrx2.ppm"
#else
#define PPMIMG1   "../testimg/pnmtest.ppm"
#define PPMIMG2   "../testimg/pnmtest2.ppm"
#define JPGIMGBG  "../testimg/jpeg3.jpg"
#define PPMIMGOUT "../testimg/demmgrx2.ppm"
#endif

#define NDEMOS 51

#define ID_ARCTEST   1
#define ID_BB1TEST   2
#define ID_BLITTEST  3
#define ID_CIRCTEST  4
#define ID_CLIPTEST  5
#define ID_COLOROPS  6
#define ID_CURSTEST  7
#define ID_FONTTEST  8
#define ID_IMGTEST   9
#define ID_JPGTEST  10
#define ID_KEYS     11
#define ID_LIFE     12
#define ID_LIFEDB   13
#define ID_LINETEST 14
#define ID_MOUSETST 15
#define ID_PCIRCTST 16
#define ID_PNMTEST  17
#define ID_PNGTEST  18
#define ID_POLYTEST 19
#define ID_RGBTEST  20
#define ID_SCROLTST 21
#define ID_SBCTEST  22
#define ID_SPEEDTST 23
#define ID_TEXTPATT 24
#define ID_WINCLIP  25
#define ID_WINTEST  26
#define ID_FNTDEMO1 27
#define ID_FNTDEMO2 28
#define ID_FNTDEMO3 29
#define ID_FNTDEMO4 30
#define ID_DEMOINTL 31
#define ID_BIGGTEST 32
#define ID_CUSPTEST 33
#define ID_CLRTABLE 34
#define ID_DEMINTL2 40
#define ID_GUIEX01  41
#define ID_GUIEX02  42
#define ID_GUIEX03  43
#define ID_GUIEX04  44
#define ID_GUIEX05  45
#define ID_GUIEX06  46
#define ID_GUIEX07  47
#define ID_GUIEX08  48
#define ID_GUIEX09  49
#define ID_GUIEX10  50
#define ID_GUIEX11  51
#define ID_SHOWFNT2 52
#define ID_MODETEST 70
#define ID_PAGE1    81
#define ID_PAGE2    82
#define ID_EXIT     99

typedef struct {
    int cid;
    char *prog;
    char *text;
} ProgTable;

static ProgTable ptable[NDEMOS] = {
    {ID_ARCTEST, "arctest", "arctest.c -> test arc outline and filled arc drawing"},
    {ID_BB1TEST, "bb1test", "bb1test.c -> test GrBitBlt1bpp routine"},
    {ID_BLITTEST, "blittest", "blittest.c -> test various bitblt-s"},
    {ID_CIRCTEST, "circtest", "circtest.c -> test circle and ellipse rendering"},
    {ID_CLIPTEST, "cliptest", "cliptest.c -> test clipping"},
    {ID_COLOROPS, "colorops", "colorops.c -> test WRITE, XOR, OR, and AND draw modes"},
    {ID_CURSTEST, "curstest", "curstest.c -> test cursors"},
    {ID_FONTTEST, "fonttest", "fonttest.c -> test text drawing"},
    {ID_IMGTEST, "imgtest", "imgtest.c -> test image functions mapping"},
    {ID_JPGTEST, "jpgtest", "jpgtext.c -> text context to jpeg functions"},
    {ID_KEYS, "keys", "keys.c -> test keyboard input"},
    {ID_LIFE, "life", "life.c -> Conway's life program"},
    {ID_LIFEDB, "life_db", "life_db.c -> Double buffer version of life"},
    {ID_LINETEST, "linetest", "linetest.c -> test wide and patterned lines"},
    {ID_MOUSETST, "mousetst", "mousetst.c -> test mouse cursor and mouse/keyboard input"},
    {ID_PCIRCTST, "pcirctst", "pcirctst.c -> test custom circle and ellipse rendering"},
    {ID_PNMTEST, "pnmtest", "pnmtext.c -> text context to pnm functions"},
    {ID_PNGTEST, "pngtest", "pngtext.c -> text context to png functions"},
    {ID_POLYTEST, "polytest", "polytest.c -> test polygon rendering"},
    {ID_RGBTEST, "rgbtest", "rgbtest.c -> show 256 color RGB palette"},
    {ID_SBCTEST, "sbctest", "sbctest.c -> test subcontext operations"},
    {ID_SCROLTST, "scroltst", "scroltst.c -> test virtual screen set/scroll"},
    {ID_SPEEDTST, "speedtst", "speedtst.c -> check all available frame drivers speed"},
    {ID_TEXTPATT, "textpatt", "textpatt.c -> test patterned text"},
    {ID_WINCLIP, "winclip", "winclip.c -> clip a drawing to various windows (contexts)"},
    {ID_WINTEST, "wintest", "wintest.c -> test window (context) mapping"},
    {ID_FNTDEMO1, "fontdemo ncen22b.fnt", "fontdemo ncen22b.fnt -> test a GRX font"},
    {ID_FNTDEMO2, "fontdemo ter-114b.res", "fontdemo ter-114b.res -> test a RES font"},
    {ID_FNTDEMO3, "fontdemo ter-114n.fna", "fontdemo ter-114n.fna -> test a FNA font"},
    {ID_FNTDEMO4, "fontdemo ter-114v.psf", "fontdemo ter-114v.psf -> test a PSF font"},
    {ID_DEMOINTL, "demointl", "demointl.c -> test intl support"},
    {ID_BIGGTEST, "biggtest", "biggtest.c -> test GR_biggest_graphics"},
    {ID_CUSPTEST, "cusptest", "cusptest.c -> test wide line functions"},
    {ID_CLRTABLE, "clrtable", "clrtable.c -> test color tables"},
    {ID_DEMINTL2, "demintl2", "demintl2.c -> test intl support and GrGUI"},
    {ID_GUIEX01, "grgui01", "grgui01.c -> GrGUI example 01"},
    {ID_GUIEX02, "grgui02", "grgui02.c -> GrGUI example 02"},
    {ID_GUIEX03, "grgui03", "grgui03.c -> GrGUI example 03"},
    {ID_GUIEX04, "grgui04", "grgui04.c -> GrGUI example 04"},
    {ID_GUIEX05, "grgui05", "grgui05.c -> GrGUI example 05"},
    {ID_GUIEX06, "grgui06", "grgui06.c -> GrGUI example 06"},
    {ID_GUIEX07, "grgui07", "grgui07.c -> GrGUI example 07"},
    {ID_GUIEX08, "grgui08", "grgui08.c -> GrGUI example 08"},
    {ID_GUIEX09, "grgui09", "grgui09.c -> GrGUI example 09"},
    {ID_GUIEX10, "grgui10", "grgui10.c -> GrGUI example 10"},
    {ID_GUIEX11, "grgui11", "grgui11.c -> GrGUI example 11"},
    {ID_SHOWFNT2, "showfnt2", "showfnt2.c -> show MGRX font collection"},
    {ID_MODETEST, "modetest", "modetest.c -> test all available graphics modes"},
    {ID_PAGE1, "", "Change to page 1"},
    {ID_PAGE2, "", "Change to page 2"},
    {ID_EXIT, "", "Exit MGRX test programs launcher"}
};

static GUIGroup *bgrp1, *bgrp2, *bgrpact;

static GrFont *grf_std;
static GrFont *grf_big;
static GrFont *grf_gfaz;
static GrFont *grf_intl;
GrTextOption grt_centered;
GrTextOption grt_left;

static GrContext *grcglob = NULL;
static GrContext *grcglobdb = NULL;
static int worg = 0, horg = 0;

/* Internal routines */

static void ini_graphics(void);
static void end_graphics(void);
static GrFont *load_font(char *fname);
static void ini_objects(void);
static void ini_grgui(void);
static void ini_groups(void);
static void paint_screen(void);
static void the_title(int x, int y);
static void the_info(int x, int y);
static int pev_command(GrEvent * ev);
static int pev_select(GrEvent * ev);
static void paint_foot(char *s);
static void paint_animation(void);
static void disaster(char *s);

/************************************************************************/

int main(int argc, char **argv)
{
    char *infoexit[1] = {"Do you want to exit?"};
    GrEvent ev;
    long oldtime = 0;
    int ret;

    if (argc >= 4) {
        gwidth = atoi(argv[1]);
        gheight = atoi(argv[2]);
        gbpp = atoi(argv[3]);
    }

    ini_graphics();
    GrSetWindowTitle(wintitle);
    ini_objects();
    ini_grgui();
    ini_groups();
    paint_screen();

    while (1) {
        GrEventRead(&ev);
        if (ev.type == GREV_MOUSE) {
            ev.p2 -= worg;
            ev.p3 -= horg;
        }
        if (ev.type == GREV_END)
            break;
        if (((ev.type == GREV_KEY) && (ev.p1 == GrKey_Escape)) ||
             (ev.type == GREV_WMEND)) {
            ret = GUICDialogYesNo("Exit", (void **)infoexit, 1, "Yes", "No");
            if (ret == 1) break;
        }
        if ((ev.type == GREV_KEY) && (ev.p1 == 's')) {
            GrSaveContextToPpm(NULL, PPMIMGOUT, "DemMGRX2");
            continue;
        }
        if (GUIGroupProcessEvent(bgrpact, &ev))
            continue;
        if (pev_command(&ev))
            continue;
        if (pev_select(&ev))
            continue;
        if (ev.type == GREV_MOUSE) {
            if (ev.p1 == GRMOUSE_LB_PRESSED)
                paint_foot("over a button, please");
            else if (ev.p1 == GRMOUSE_LB_RELEASED)
                paint_foot("Hold down left mouse buttom to see a comment");
            continue;
        }
        if (ev.type == GREV_EXPOSE) {
            //printf("expose %ld %ld %ld %ld %d\n",
            //       ev.p1, ev.p2, ev.p3, ev.p4, ev.kbstat);
            if (ev.kbstat == 0) {// no more EXPOSE events follow
                if (gwidth > 640 || gheight > 480)
                    GrClearScreen(GrAllocColor(120, 90, 60));
                paint_screen();
            }
            continue;
        }
        if (ev.type == GREV_NULL) {
            if (ev.time > (oldtime + 9)) {
                paint_animation();
                oldtime = ev.time;
            }
        }
    }

    end_graphics();
    return 0;
}

/************************************************************************/

static void ini_graphics(void)
{
    GrSetMode(GR_width_height_bpp_graphics, gwidth, gheight, gbpp);
    GrGenEgaColorTable();
    GrSetFontPath("../fonts/;./");
    GUIInit(1, 0);

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
    grcglobdb = GrCreateContext(640, 480, NULL, NULL);
    GrEventGenExpose(GR_GEN_EXPOSE_YES);
    GrEventGenWMEnd(GR_GEN_WMEND_YES);
}

/************************************************************************/

static void end_graphics(void)
{
    if (grcglobdb) GrDestroyContext(grcglobdb);
    if (grcglob) GrDestroyContext(grcglob);
    GUIEnd();
    GrSetMode(GR_default_text);
}

/************************************************************************/

static GrFont *load_font(char *fname)
{
    GrFont *gf;
    char s[81];
    
    gf = GrLoadFont(fname);
    if (gf == NULL) {
        snprintf(s, 80, "../fonts/%s", fname);
        gf = GrLoadFont(s);
        if (gf == NULL) {
            snprintf(s, 80, "%s not found in ./ or ../fonts/", fname);
            disaster(s);
        }
    }
    
    return gf;
}

/************************************************************************/

static void ini_objects(void)
{
    grf_std = load_font("lucb21.fnt");
    grf_big = load_font("lucb40.fnt");
    grf_gfaz = load_font("tmgrx14b.fnt");
    grf_intl = load_font("tmgrx18b.fnt");

    grt_centered.txo_bgcolor = GrNOCOLOR;
    grt_centered.txo_direct = GR_TEXT_RIGHT;
    grt_centered.txo_xalign = GR_ALIGN_CENTER;
    grt_centered.txo_yalign = GR_ALIGN_CENTER;
    grt_centered.txo_chrtype = GR_UTF8_TEXT;

    grt_left.txo_bgcolor = GrNOCOLOR;
    grt_left.txo_direct = GR_TEXT_RIGHT;
    grt_left.txo_xalign = GR_ALIGN_LEFT;
    grt_left.txo_yalign = GR_ALIGN_CENTER;
    grt_left.txo_chrtype = GR_UTF8_TEXT;
}

/************************************************************************/

static void ini_grgui(void)
{
    GUIObjectsSetChrType(GR_UTF8_TEXT); // this source is UTF8 coded
    GUIObjectsSetFont(grf_gfaz);
    GUIObjectsSetColors(EGAC_BLACK, EGAC_LIGHTGRAY, EGAC_DARKGRAY);

    GUIDialogsSetTitleFont(grf_gfaz);
    GUIDialogsSetColors(EGAC_BLACK, EGAC_LIGHTCYAN, EGAC_WHITE, EGAC_BLACK);
    GUIDialogsSetTitleFont(grf_intl);
    GUICDialogsSetFont(grf_gfaz);
    GUICDialogsSetColors(EGAC_CYAN, EGAC_WHITE);
}

/************************************************************************/

static void ini_groups(void)
{
#define PX0 10
#define PX1 115
#define PX2 220
#define PY0 10
#define PY1 54
#define PY2 98
#define PY3 142
#define PY4 186
#define PY5 230
#define PY6 274
#define PY7 318
#define PY8 362

#define NBUTTONSP1 26
#define NBUTTONSP2 27

    bgrp1 = GUIGroupCreate(NBUTTONSP1, 16, 30);
    GUIObjectSetButton(&(bgrp1->o[0]),   0, PX0, PY0, 100, 40, EGAC_DARKGRAY, EGAC_LIGHTGRAY, "ArcTest", ID_ARCTEST, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[1]),   1, PX0, PY1, 100, 40, EGAC_BLUE, EGAC_YELLOW, "Bb1Test", ID_BB1TEST, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[2]),   2, PX0, PY2, 100, 40, EGAC_BLUE, EGAC_YELLOW, "BlitTest", ID_BLITTEST, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[3]),   3, PX0, PY3, 100, 40, EGAC_BLUE, EGAC_YELLOW, "CircTest", ID_CIRCTEST, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[4]),   4, PX0, PY4, 100, 40, EGAC_BLUE, EGAC_YELLOW, "ClipTest", ID_CLIPTEST, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[5]),   5, PX0, PY5, 100, 40, EGAC_BLUE, EGAC_YELLOW, "Colorops", ID_COLOROPS, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[6]),   6, PX0, PY6, 100, 40, EGAC_BLUE, EGAC_YELLOW, "CursTest", ID_CURSTEST, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[7]),   7, PX0, PY7, 100, 40, EGAC_BLUE, EGAC_YELLOW, "ImgTest", ID_IMGTEST, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[8]),   8, PX0, PY8, 100, 40, EGAC_BLUE, EGAC_YELLOW, "JpgTest", ID_JPGTEST, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[9]),   9, PX1, PY0, 100, 40, EGAC_BLUE, EGAC_YELLOW, "Keys", ID_KEYS, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[10]), 10, PX1, PY1, 100, 40, EGAC_BLUE, EGAC_YELLOW, "Life", ID_LIFE, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[11]), 11, PX1, PY2, 100, 40, EGAC_BLUE, EGAC_YELLOW, "Life_db", ID_LIFEDB, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[12]), 12, PX1, PY3, 100, 40, EGAC_BLUE, EGAC_YELLOW, "LineTest", ID_LINETEST, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[13]), 13, PX1, PY4, 100, 40, EGAC_BLUE, EGAC_YELLOW, "MouseTst", ID_MOUSETST, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[14]), 14, PX1, PY5, 100, 40, EGAC_BLUE, EGAC_YELLOW, "PcircTst", ID_PCIRCTST, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[15]), 15, PX1, PY6, 100, 40, EGAC_BLUE, EGAC_YELLOW, "PnmTest", ID_PNMTEST, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[16]), 16, PX1, PY7, 100, 40, EGAC_BLUE, EGAC_YELLOW, "PngTest", ID_PNGTEST, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[17]), 17, PX1, PY8, 100, 40, EGAC_DARKGRAY, EGAC_LIGHTGRAY, "PolyTest", ID_POLYTEST, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[18]), 18, PX2, PY0, 100, 40, EGAC_BLUE, EGAC_YELLOW, "RgbTest", ID_RGBTEST, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[19]), 19, PX2, PY1, 100, 40, EGAC_BLUE, EGAC_YELLOW, "SbcTest", ID_SBCTEST, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[20]), 20, PX2, PY2, 100, 40, EGAC_BLUE, EGAC_YELLOW, "ScrolTst", ID_SCROLTST, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[21]), 21, PX2, PY3, 100, 40, EGAC_BLUE, EGAC_YELLOW, "WinClip", ID_WINCLIP, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[22]), 22, PX2, PY4, 100, 40, EGAC_BLUE, EGAC_YELLOW, "WinTest", ID_WINTEST, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[23]), 23, PX2, PY6, 100, 40, EGAC_GREEN, EGAC_WHITE, "Page 2", ID_PAGE2, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[24]), 24, PX2, PY7, 100, 40, EGAC_BROWN, EGAC_WHITE, "ModeTest", ID_MODETEST, 0, 0);
    GUIObjectSetButton(&(bgrp1->o[25]), 25, PX2, PY8, 100, 40, EGAC_RED, EGAC_WHITE, "Exit", ID_EXIT, 0, 0);
    GUIGroupSetSelected(bgrp1, 0, 0);

    bgrp2= GUIGroupCreate(NBUTTONSP2, 16, 30);
    GUIObjectSetButton(&(bgrp2->o[0]),   0, PX0, PY0, 100, 40, EGAC_BLUE, EGAC_YELLOW, "FontTest", ID_FONTTEST, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[1]),   1, PX0, PY1, 100, 40, EGAC_BLUE, EGAC_YELLOW, "TextPatt", ID_TEXTPATT, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[2]),   2, PX0, PY2, 100, 40, EGAC_BLUE, EGAC_YELLOW, "FontDemo1", ID_FNTDEMO1, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[3]),   3, PX0, PY3, 100, 40, EGAC_BLUE, EGAC_YELLOW, "FontDemo2", ID_FNTDEMO2, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[4]),   4, PX0, PY4, 100, 40, EGAC_BLUE, EGAC_YELLOW, "FontDemo3", ID_FNTDEMO3, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[5]),   5, PX0, PY5, 100, 40, EGAC_BLUE, EGAC_YELLOW, "FontDemo4", ID_FNTDEMO4, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[6]),   6, PX0, PY6, 100, 40, EGAC_BLUE, EGAC_YELLOW, "DemoIntl", ID_DEMOINTL, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[7]),   7, PX0, PY7, 100, 40, EGAC_BLUE, EGAC_YELLOW, "BigGTest", ID_BIGGTEST, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[8]),   8, PX0, PY8, 100, 40, EGAC_BLUE, EGAC_YELLOW, "CusPTest", ID_CUSPTEST, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[9]),   9, PX1, PY0, 100, 40, EGAC_BLUE, EGAC_YELLOW, "ClrTable", ID_CLRTABLE, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[10]), 10, PX1, PY1, 100, 40, EGAC_DARKGRAY, EGAC_LIGHTGRAY, "SpeedTst", ID_SPEEDTST, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[11]), 11, PX1, PY2, 100, 40, EGAC_CYAN, EGAC_YELLOW, "DemIntl2", ID_DEMINTL2, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[12]), 12, PX1, PY3, 100, 40, EGAC_CYAN, EGAC_YELLOW, "GrGui01", ID_GUIEX01, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[13]), 15, PX1, PY4, 100, 40, EGAC_CYAN, EGAC_YELLOW, "GrGui02", ID_GUIEX02, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[14]), 14, PX1, PY5, 100, 40, EGAC_CYAN, EGAC_YELLOW, "GrGui03", ID_GUIEX03, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[15]), 15, PX1, PY6, 100, 40, EGAC_CYAN, EGAC_YELLOW, "GrGui04", ID_GUIEX04, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[16]), 16, PX1, PY7, 100, 40, EGAC_CYAN, EGAC_YELLOW, "GrGui05", ID_GUIEX05, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[17]), 17, PX1, PY8, 100, 40, EGAC_CYAN, EGAC_YELLOW, "GrGui06", ID_GUIEX06, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[18]), 18, PX2, PY0, 100, 40, EGAC_CYAN, EGAC_YELLOW, "GrGui07", ID_GUIEX07, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[19]), 19, PX2, PY1, 100, 40, EGAC_CYAN, EGAC_YELLOW, "GrGui08", ID_GUIEX08, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[20]), 20, PX2, PY2, 100, 40, EGAC_CYAN, EGAC_YELLOW, "GrGui09", ID_GUIEX09, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[21]), 21, PX2, PY3, 100, 40, EGAC_CYAN, EGAC_YELLOW, "GrGui10", ID_GUIEX10, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[22]), 22, PX2, PY4, 100, 40, EGAC_CYAN, EGAC_YELLOW, "GrGui11", ID_GUIEX11, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[23]), 23, PX2, PY5, 100, 40, EGAC_CYAN, EGAC_YELLOW, "ShowFnt2", ID_SHOWFNT2, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[24]), 24, PX2, PY6, 100, 40, EGAC_GREEN, EGAC_WHITE, "Page 1", ID_PAGE1, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[25]), 25, PX2, PY7, 100, 40, EGAC_BROWN, EGAC_WHITE, "ModeTest", ID_MODETEST, 0, 0);
    GUIObjectSetButton(&(bgrp2->o[26]), 26, PX2, PY8, 100, 40, EGAC_RED, EGAC_WHITE, "Exit", ID_EXIT, 0, 0);
    GUIGroupSetSelected(bgrp2, 0, 0);

    bgrpact = bgrp1;
}

/************************************************************************/

static void paint_screen(void)
{
    // use a double buffer to avoid flickering
    GrContext *grc, *grcf;

    grcf = grcglob;
    if (grcglobdb != NULL) { 
        GrSetContext(grcglobdb);
        grcf = grcglobdb;
    }

    GrDrawBorder(0, 0, GrMaxX(), GrMaxY(), 4, EGAC_BLACK, EGAC_CYAN);
    GrFilledBox(4, 4, GrMaxX()-4, GrMaxY()-4, EGAC_DARKGRAY);
    if (GrNumColors() > 256 && GrJpegSupport()) {
        GrSetClipBox(4,4,GrMaxX()-4,GrMaxY()-4);
        GrLoadContextFromJpeg(NULL, JPGIMGBG, 1);
        GrResetClipBox();
    }

    GUIGroupPaint(bgrpact);

    GrDrawBorder(379, 46, 613, 202, 4, EGAC_BLACK, EGAC_CYAN);
    grc = GrCreateSubContext(383, 50, 609, 198, grcf, NULL);
    if (bgrpact == bgrp1)
        GrLoadContextFromPnm(grc, PPMIMG1);
    else
        GrLoadContextFromPnm(grc, PPMIMG2);
    GrDestroyContext(grc);

    the_info(495, 225);
    the_title(495, 370);
    paint_foot("Hold down left mouse buttom to see a comment");
    GrSetContext(grcglob);
    GrBitBlt(NULL, 0, 0, grcglobdb, 0, 0, 639, 479, GrWRITE);
}

/************************************************************************/

static void the_title(int x, int y)
{
    char *t1 = "MGRX 1.3.3";
    char *t2 = "test programs launcher";

    grt_centered.txo_fgcolor = EGAC_LIGHTGREEN;

    grt_centered.txo_font = grf_big;
    GrDrawString(t1, 0, 0 + x, 0 + y, &grt_centered);

    grt_centered.txo_font = grf_std;
    GrDrawString(t2, 0, 0 + x, 30 + y, &grt_centered);
}

/************************************************************************/

static void the_info(int x, int y)
{
    char aux[81];
    char sys[4] = "?";
    int nsys;
    char *kbsysencoding;
    int nenc;

    grt_centered.txo_fgcolor = EGAC_LIGHTCYAN;
    grt_centered.txo_font = grf_std;

    nsys = GrGetLibrarySystem();
    if (nsys == MGRX_VERSION_GCC_386_DJGPP)
        strcpy(sys, "DJ2");
    else if (nsys == MGRX_VERSION_GCC_386_LINUX)
        strcpy(sys, "L32");
    else if (nsys == MGRX_VERSION_GCC_386_X11)
        strcpy(sys, "X32");
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

    nenc = GrGetKbSysEncoding();
    kbsysencoding = GrStrEncoding(nenc);

    sprintf(aux, "Version: %x System: %s", GrGetLibraryVersion(), sys);
    GrDrawString(aux, 0, 0 + x, 0 + y, &grt_centered);

    sprintf(aux, "KbSysEncoding: %s", kbsysencoding);
    GrDrawString(aux, 0, 0 + x, 20 + y, &grt_centered);

    sprintf(aux, "VideoDriver: %s", GrCurrentVideoDriver()->name);
    GrDrawString(aux, 0, 0 + x, 40 + y, &grt_centered);

    sprintf(aux, "ScreenFrameBuffer: %s", GrFrameDriverName(GrScreenFrameMode()));
    GrDrawString(aux, 0, 0 + x, 60 + y, &grt_centered);

    sprintf(aux, "RamFrameBuffer: %s", GrFrameDriverName(GrCoreFrameMode()));
    GrDrawString(aux, 0, 0 + x, 80 + y, &grt_centered);

    sprintf(aux, "Mode: %dx%d %d bpp", GrCurrentVideoMode()->width,
            GrCurrentVideoMode()->height, GrCurrentVideoMode()->bpp);
    GrDrawString(aux, 0, 0 + x, 100 + y, &grt_centered);
}

/************************************************************************/

static int pev_command(GrEvent * ev)
{
    int i, error;
    char nprog[121];
    char *failtext[2];
    char *infonotest[2];

    failtext[0] = "Error executing test program";
    infonotest[0] = "Please run this test in the 'test' subdir";
    infonotest[1] = "because it uses some data files there";

    if (ev->type == GREV_COMMAND) {
        if (ev->p1 == ID_EXIT) {
            GrEventParEnqueue(GREV_END, 0, 0, 0, 0);
            return 1;
        }
        if (ev->p1 == ID_PAGE1) {
            bgrpact = bgrp1;
            paint_screen();
            return 1;
        }
        if (ev->p1 == ID_PAGE2) {
            bgrpact = bgrp2;
            paint_screen();
            return 1;
        }
        for (i = 0; i < NDEMOS; i++) {
            if (ev->p1 == ptable[i].cid) {
                if ((ev->p1 == ID_ARCTEST) || (ev->p1 == ID_POLYTEST) ||
                    (ev->p1 == ID_SPEEDTST)) {
                    GUICDialogInfo("Warning", (void **)infonotest, 2, "Ok");
                    return 1;
                }
#if defined(__MSDOS__) || defined(__WIN32__)
                if (ev->p1 == ID_MODETEST)
                    strcpy(nprog, "..\\bin\\");
                else if (ev->p1 < ID_DEMINTL2)
                    strcpy(nprog, "..\\test\\");
                else
                    strcpy(nprog, ".\\");
#elif defined(__XWIN__)
                if (ev->p1 == ID_MODETEST)
                    strcpy(nprog, "xterm -e ../bin/x");
                else if (ev->p1 == ID_SPEEDTST)
                    strcpy(nprog, "xterm -e ../test/x");
                else if (ev->p1 < ID_DEMINTL2)
                    strcpy(nprog, "../test/x");
                else
                    strcpy(nprog, "./x");
#else
                if (ev->p1 == ID_MODETEST)
                    strcpy(nprog, "../bin/");
                else if (ev->p1 < ID_DEMINTL2)
                    strcpy(nprog, "../test/");
                else
                    strcpy(nprog, "./");
#endif
                strcat(nprog, ptable[i].prog);
                if (need_restart) {
                    end_graphics();
                }
                error = system(nprog);
                //printf("error %d\n", error);
                if (need_restart) {
                    ini_graphics();
                    ini_grgui();
                    GrSetWindowTitle(wintitle);
                    GrEventFlush();
                }
                paint_screen();
                paint_animation();
                if (error != 0) {
                    failtext[1] = nprog;
                    GUICDialogInfo("Error", (void **)failtext, 2, "Ok");
                }
                return 1;
            }
        }
    }
    return 0;
}

/************************************************************************/

static int pev_select(GrEvent * ev)
{
    int i;

    if (ev->type == GREV_SELECT) {
        for (i = 0; i < NDEMOS; i++) {
            if (ev->p1 == ptable[i].cid) {
                paint_foot(ptable[i].text);
                return 1;
            }
        }
    }
    return 0;
}

/************************************************************************/

static void paint_foot(char *s)
{
    grt_centered.txo_fgcolor = EGAC_LIGHTGREEN;
    grt_centered.txo_font = grf_std;

    GrSetClipBox(10, 440, 630, 470);
    GrClearClipBox(EGAC_CYAN);
    GrDrawString(s, 0, 320, 455, &grt_centered);
    GrResetClipBox();
}

/************************************************************************/

static void paint_animation(void)
{
    static int pos = 620;
    static int ini = 0;
    static int ind = 0;
    static GrContext *grc, *grcor;
    int ltext, wtext;

    if (!ini) {
        grcor = GrCreateContext(620, 30, NULL, NULL);
        if (grcor == NULL) return;
        grc = GrCreateContext(620, 30, NULL, NULL);
        if (grc == NULL) {
            GrDestroyContext(grcor);
            return;
        }
        GrBitBlt(grcor, 0, 0, NULL, 10, 8, 629, 37, GrWRITE);
        ini = 1;
    }

    if (ind > 1) ind = 0;
    grt_left.txo_fgcolor = EGAC_LIGHTCYAN;
    grt_left.txo_font = ((ind % 2) == 1) ? grf_intl : grf_std;
    ltext = GrUTF8StrLen((unsigned char *)animatedtext[ind]);
    wtext = GrStringWidth(animatedtext[ind], ltext, &grt_left);

    GrBitBlt(grc, 0, 0, grcor, 0, 0, 619, 29, GrWRITE);
    GrSetContext(grc);
    //GrClearContext(DARKGRAY);
    GrDrawString(animatedtext[ind], ltext, pos, 15, &grt_left);
    GrSetContext(grcglob);
    GrBitBlt(NULL, 10, 8, grc, 0, 0, 619, 29, GrWRITE);

    pos -= 1;
    if (pos <= -wtext) {
        pos = 620;
        ind++;
    }
}

/************************************************************************/

static void disaster(char *s)
{
    void _GrCloseVideoDriver(void);

    char aux[81];

    GUIEnd();
    _GrCloseVideoDriver();
    printf("DemoMGRX: %s\n", s);
    printf("press Return to continue\n");
    fgets(aux, 80, stdin);
    exit(1);
}
