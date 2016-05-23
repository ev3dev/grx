/*
 * demogrx.c ---- GRX Test programs launcher
 *
 * Copyright (C) 2000,2001 Mariano Alvarez Fernandez
 * [e-mail: malfer@teleline.es]
 *
 * This is a test/demo file of the GRX graphics library.
 * You can use GRX test/demo files as you want.
 *
 * The GRX graphics library is free software; you can redistribute it
 * and/or modify it under some conditions; see the "copying.grx" file
 * for details.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grx-3.0.h"
#include "grxkeys.h"
#include "gfaz.h"
#include "drawing.h"

/* default mode */

static int gwidth = 640;
static int gheight = 480;
static int gbpp = 16;

char *wintitle =
    "GRX 2.4.9, the graphics library";

char *animatedtext =
    "GRX 2.4.9, the graphics library for DJGPPv2, Linux, X11 and Win32";

#define NDEMOS 33

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
#define ID_LINETEST 13
#define ID_MOUSETST 14
#define ID_PCIRCTST 15
#define ID_PNMTEST  16
#define ID_PNGTEST  17
#define ID_POLYTEST 18
#define ID_RGBTEST  19
#define ID_SCROLTST 20
#define ID_SBCTEST  21
#define ID_SPEEDTST 22
#define ID_TEXTPATT 23
#define ID_WINCLIP  24
#define ID_WINTEST  25
#define ID_FNTDEMO1 26
#define ID_FNTDEMO2 27
#define ID_FNTDEMO3 28
#define ID_FNTDEMO4 29
#define ID_MODETEST 50
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
    {ID_BB1TEST, "bb1test", "bb1test.c -> test grx_bit_blt_1bpp routine"},
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
    {ID_MODETEST, "modetest", "modetest.c -> test all available graphics modes"},
    {ID_PAGE1, "", "Change to page 1"},
    {ID_PAGE2, "", "Change to page 2"},
    {ID_EXIT, "", "Exit GRX test programs launcher"}
};

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

static Button bp1[NBUTTONSP1] = {
    {PX0, PY0, 100, 40, IND_BLUE, IND_YELLOW, "ArcTest", BSTATUS_SELECTED, ID_ARCTEST},
    {PX0, PY1, 100, 40, IND_BLUE, IND_YELLOW, "Bb1Test", 0, ID_BB1TEST},
    {PX0, PY2, 100, 40, IND_BLUE, IND_YELLOW, "BlitTest", 0, ID_BLITTEST},
    {PX0, PY3, 100, 40, IND_BLUE, IND_YELLOW, "CircTest", 0, ID_CIRCTEST},
    {PX0, PY4, 100, 40, IND_BLUE, IND_YELLOW, "ClipTest", 0, ID_CLIPTEST},
    {PX0, PY5, 100, 40, IND_BLUE, IND_YELLOW, "Colorops", 0, ID_COLOROPS},
    {PX0, PY6, 100, 40, IND_BLUE, IND_YELLOW, "CursTest", 0, ID_CURSTEST},
    {PX0, PY7, 100, 40, IND_BLUE, IND_YELLOW, "ImgTest", 0, ID_IMGTEST},
    {PX0, PY8, 100, 40, IND_BLUE, IND_YELLOW, "JpgTest", 0, ID_JPGTEST},
    {PX1, PY0, 100, 40, IND_BLUE, IND_YELLOW, "Keys", 0, ID_KEYS},
    {PX1, PY1, 100, 40, IND_BLUE, IND_YELLOW, "Life", 0, ID_LIFE},
    {PX1, PY2, 100, 40, IND_BLUE, IND_YELLOW, "LineTest", 0, ID_LINETEST},
    {PX1, PY3, 100, 40, IND_BLUE, IND_YELLOW, "MouseTst", 0, ID_MOUSETST},
    {PX1, PY4, 100, 40, IND_BLUE, IND_YELLOW, "PcircTst", 0, ID_PCIRCTST},
    {PX1, PY5, 100, 40, IND_BLUE, IND_YELLOW, "PnmTest", 0, ID_PNMTEST},
    {PX1, PY6, 100, 40, IND_BLUE, IND_YELLOW, "PngTest", 0, ID_PNGTEST},
    {PX1, PY7, 100, 40, IND_BLUE, IND_YELLOW, "PolyTest", 0, ID_POLYTEST},
    {PX1, PY8, 100, 40, IND_BLUE, IND_YELLOW, "RgbTest", 0, ID_RGBTEST},
    {PX2, PY0, 100, 40, IND_BLUE, IND_YELLOW, "SbcTest", 0, ID_SBCTEST},
    {PX2, PY1, 100, 40, IND_BLUE, IND_YELLOW, "ScrolTst", 0, ID_SCROLTST},
    {PX2, PY2, 100, 40, IND_BLUE, IND_YELLOW, "SpeedTst", 0, ID_SPEEDTST},
    {PX2, PY3, 100, 40, IND_BLUE, IND_YELLOW, "WinClip", 0, ID_WINCLIP},
    {PX2, PY4, 100, 40, IND_BLUE, IND_YELLOW, "WinTest", 0, ID_WINTEST},
    {PX2, PY6, 100, 40, IND_GREEN, IND_WHITE, "Page 2", 0, ID_PAGE2},
    {PX2, PY7, 100, 40, IND_BROWN, IND_WHITE, "ModeTest", 0, ID_MODETEST},
    {PX2, PY8, 100, 40, IND_RED, IND_WHITE, "Exit", 0, ID_EXIT}
};

#define NBUTTONSP2  9

static Button bp2[NBUTTONSP2] = {
    {PX0, PY0, 100, 40, IND_BLUE, IND_YELLOW, "FontTest", BSTATUS_SELECTED, ID_FONTTEST},
    {PX0, PY1, 100, 40, IND_BLUE, IND_YELLOW, "TextPatt", 0, ID_TEXTPATT},
    {PX0, PY2, 100, 40, IND_BLUE, IND_YELLOW, "FontDemo1", 0, ID_FNTDEMO1},
    {PX0, PY3, 100, 40, IND_BLUE, IND_YELLOW, "FontDemo2", 0, ID_FNTDEMO2},
    {PX0, PY4, 100, 40, IND_BLUE, IND_YELLOW, "FontDemo3", 0, ID_FNTDEMO3},
    {PX0, PY5, 100, 40, IND_BLUE, IND_YELLOW, "FontDemo4", 0, ID_FNTDEMO4},
    {PX2, PY6, 100, 40, IND_GREEN, IND_WHITE, "Page 1", 0, ID_PAGE1},
    {PX2, PY7, 100, 40, IND_BROWN, IND_WHITE, "ModeTest", 0, ID_MODETEST},
    {PX2, PY8, 100, 40, IND_RED, IND_WHITE, "Exit", 0, ID_EXIT}
};

static Button_Group bgp1 = { 20, 30, bp1, NBUTTONSP1, 0, 0 };
static Button_Group bgp2 = { 20, 30, bp2, NBUTTONSP2, 0, 0 };
static Button_Group *bgact = &bgp1;

static Board brd =
    { 0, 0, 640, 480, IND_BLACK, IND_CYAN, IND_DARKGRAY, 1 };
static Board brdimg =
    { 384, 46, 235, 157, IND_BLACK, IND_CYAN, IND_DARKGRAY, 1 };

static GrxFont *grf_std;
static GrxFont *grf_big;
GrxTextOptions grt_centered;
GrxTextOptions grt_left;

static GrxContext *grcglob = NULL;
static int worg = 0, horg = 0;

/* Internal routines */

static void ini_graphics(void);
static void ini_objects(void);
static void paint_screen(void);
static void the_title(int x, int y);
static void the_info(int x, int y);
static int pev_command(Event * ev);
static int pev_select(Event * ev);
static void paint_foot(char *s);
static void paint_animation(void);
static void disaster(char *s);

/************************************************************************/

int main(int argc, char **argv)
{
    Event ev;
    char buffer[100];

    if (argc >= 4) {
        gwidth = atoi(argv[1]);
        gheight = atoi(argv[2]);
        gbpp = atoi(argv[3]);
    }

    ini_graphics();
    ini_objects();
    paint_screen();

    while (1) {
        event_read(&ev);
        if (ev.type == EV_MOUSE) {
            ev.p2 -= worg;
            ev.p3 -= horg;
        }
        if (ev.type == EV_END)
            break;
        if ((ev.type == EV_KEY) && (ev.p1 == GrKey_Escape))
            break;
        if ((ev.type == EV_KEY) && (ev.p1 == 's')) {
            grx_save_current_context_to_ppm(NULL, "demogrx.ppm", "DemoGRX");
            continue;
        }
        if (pev_button_group(&ev, bgact))
            continue;
        if (pev_command(&ev))
            continue;
        if (pev_select(&ev))
            continue;
        if (ev.type == EV_MOUSE) {
            if (ev.p1 == MOUSE_LB_PRESSED) {
                sprintf(buffer, "%ld %ld over a button, please",ev.p2,ev.p3);
                paint_foot(buffer);
            }
            else if (ev.p1 == MOUSE_LB_RELEASED)
                paint_foot("Hold down left mouse buttom to see a comment");
        }
        if (ev.type == EV_NULL)
            paint_animation();
    }

    gfaz_fin();
    return 0;
}

/************************************************************************/

static void ini_graphics(void)
{
    gfaz_ini(gwidth, gheight, gbpp);
    gwidth = grx_get_screen_width();
    gheight = grx_get_screen_height();
    grcglob = NULL;
    if (gwidth > 640 || gheight > 480) {
        grx_clear_screen(grx_color_info_alloc_color(120, 90, 60));
        worg = (gwidth - 640) / 2;
        horg = (gheight - 480) / 2;
        grcglob = grx_context_new_subcontext(worg, horg, worg + 639, horg + 479,
                                     NULL, NULL);
        grx_set_current_context(grcglob);
    }
}

/************************************************************************/

static void ini_objects(void)
{
    grf_std = grx_font_load("lucb21.fnt");
    if (grf_std == NULL) {
#if defined(__WIN32__)
        grf_std = grx_font_load("..\\fonts\\lucb21.fnt");
#else
        grf_std = grx_font_load("../fonts/lucb21.fnt");
#endif
        if (grf_std == NULL)
            disaster("lucb21.fnt not found");
    }

    grf_big = grx_font_load("lucb40b.fnt");
    if (grf_big == NULL) {
#if defined(__WIN32__)
        grf_big = grx_font_load("..\\fonts\\lucb40b.fnt");
#else
        grf_big = grx_font_load("../fonts/lucb40b.fnt");
#endif
        if (grf_big == NULL)
            disaster("lucb40b.fnt not found");
    }

    grt_centered.txo_bgcolor.v = GRX_COLOR_NONE;
    grt_centered.txo_direct = GRX_TEXT_DIRECTION_RIGHT;
    grt_centered.txo_xalign = GRX_TEXT_ALIGN_CENTER;
    grt_centered.txo_yalign = GRX_TEXT_VALIGN_MIDDLE;
    grt_centered.txo_chrtype = GRX_CHAR_TYPE_BYTE;

    grt_left.txo_bgcolor.v = GRX_COLOR_NONE;
    grt_left.txo_direct = GRX_TEXT_DIRECTION_RIGHT;
    grt_left.txo_xalign = GRX_TEXT_ALIGN_LEFT;
    grt_left.txo_yalign = GRX_TEXT_VALIGN_MIDDLE;
    grt_left.txo_chrtype = GRX_CHAR_TYPE_BYTE;
}

/************************************************************************/

static void paint_screen(void)
{
    GrxContext *grc;

    paint_board(&brd);
    paint_button_group(bgact);
    paint_board(&brdimg);
    grc = grx_context_new_subcontext(brdimg.x + 4, brdimg.y + 4,
                             brdimg.x + brdimg.wide - 5,
                             brdimg.y + brdimg.high - 5, grcglob, NULL);
    if (bgact == &bgp1)
        grx_context_load_from_pnm(grc, "pnmtest.ppm");
    else
        grx_context_load_from_pnm(grc, "pnmtest2.ppm");
    grx_context_unref(grc);
    the_info(500, 215);
    drawing(400, 280, 200, 150, BROWN, DARKGRAY);
    the_title(500, 330);
    paint_foot("Hold down left mouse buttom to see a comment");
}

/************************************************************************/

static void the_title(int x, int y)
{
    char *t1 = "GRX 2.4.9";
    char *t2 = "test programs launcher";

    grt_centered.txo_fgcolor.v = LIGHTGREEN;

    grt_centered.txo_font = grf_big;
    grx_draw_string_with_text_options(t1, strlen(t1), 0 + x, 0 + y, &grt_centered);

    grt_centered.txo_font = grf_std;
    grx_draw_string_with_text_options(t2, strlen(t2), 0 + x, 40 + y, &grt_centered);
}

/************************************************************************/

static void the_info(int x, int y)
{
    char aux[81];

    grt_centered.txo_fgcolor.v = CYAN;
    grt_centered.txo_font = grf_std;

    sprintf(aux, "VideoDriver: %s", grx_get_current_video_driver()->name);
    grx_draw_string_with_text_options(aux, strlen(aux), 0 + x, 25 + y, &grt_centered);

    sprintf(aux, "Mode: %dx%d %d bpp", grx_get_current_video_mode()->width,
            grx_get_current_video_mode()->height, grx_get_current_video_mode()->bpp);
    grx_draw_string_with_text_options(aux, strlen(aux), 0 + x, 50 + y, &grt_centered);
}

/************************************************************************/

static int pev_command(Event * ev)
{
    int i;
    char nprog[81];

    if (ev->type == EV_COMMAND) {
        if (ev->p1 == ID_EXIT) {
            par_event_queue(EV_END, 0, 0, 0);
            return 1;
        }
        if (ev->p1 == ID_PAGE1) {
            bgact = &bgp1;
            paint_screen();
            return 1;
        }
        if (ev->p1 == ID_PAGE2) {
            bgact = &bgp2;
            paint_screen();
            return 1;
        }
        for (i = 0; i < NDEMOS; i++) {
            if (ev->p1 == ptable[i].cid) {
                gfaz_fin();
#if defined(__WIN32__)
                if (ev->p1 == ID_MODETEST)
                    strcpy(nprog, "..\\bin\\");
                else
                    strcpy(nprog, ".\\");
#else
                if (ev->p1 == ID_MODETEST)
                    strcpy(nprog, "../bin/");
                else
                    strcpy(nprog, "./");
#endif
#if defined(__XWIN__)
                strcat(nprog, "x");
#endif
                strcat(nprog, ptable[i].prog);
                system(nprog);
                ini_graphics();
                paint_screen();
                return 1;
            }
        }
    }
    return 0;
}

/************************************************************************/

static int pev_select(Event * ev)
{
    int i;

    if (ev->type == EV_SELECT) {
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
    grt_centered.txo_fgcolor.v = LIGHTGREEN;
    grt_centered.txo_font = grf_std;

    grx_set_clip_box(10, 440, 630, 470);
    grx_clear_clip_box(CYAN);
    grx_draw_string_with_text_options(s, strlen(s), 320, 455, &grt_centered);
    grx_reset_clip_box();
}

/************************************************************************/

static void paint_animation(void)
{
    static int pos = 620;
    static int ini = 0;
    static GrxContext *grc;
    int ltext, wtext;

    if (!ini) {
        grc = grx_context_new(620, 30, NULL, NULL);
        if (grc == NULL)
            return;
        ini = 1;
    }

    grt_left.txo_fgcolor.v = CYAN;
    grt_left.txo_font = grf_std;
    ltext = strlen(animatedtext);
    wtext = grx_text_option_get_string_width(&grt_left, animatedtext, ltext);

    grx_set_current_context(grc);
    grx_clear_context(DARKGRAY);
    grx_draw_string_with_text_options(animatedtext, ltext, pos, 15, &grt_left);
    grx_set_current_context(grcglob);
    grx_bit_blt(10, 8, grc, 0, 0, 629, 29, GRX_COLOR_MODE_WRITE);

    pos -= 1;
    if (pos <= -wtext)
        pos = 620;
}

/************************************************************************/

static void disaster(char *s)
{
    void _GrCloseVideoDriver(void);

    char aux[81];

    gfaz_fin();
    _GrCloseVideoDriver();
    printf("DemoGRX: %s\n", s);
    printf("press Return to continue\n");
    fgets(aux, 80, stdin);
    exit(1);
}
