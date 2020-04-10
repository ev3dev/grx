/**
 ** showfnt2.c ---- show the MGRX font collection
 **
 ** Copyright (c) 2020 Mariano Alvarez Fernandez (malfer@telefonica.net)
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
#include <grgui.h>
#include "mgrxcolr.h"

#define MAX_FONTLIST 400

#define DIR_FILE1 "_fonts.dir"
#if defined(__MSDOS__) || defined(__WIN32__)
#define DIR_FILE2 "..\\fonts\\_fonts.dir"
#else
#define DIR_FILE2 "../fonts/_fonts.dir"
#endif

#define COMMAND_EXIT   100

#define IDT_INFO    1
#define IDT_SELEC   2
#define IDT_SHOW    3

typedef struct {
    GUITile *gt;
    GUIGroup *gr;
    GrColor fg;
    GrColor bg;
} UserData;

/* default mode */

static int gwidth = 1024;
static int gheight = 768;
static int gbpp = 4;

static GUITile *gt1, *gt2, *gt3;
static UserData ud1, ud2, ud3;
static GUIGroup *grp1;

static char *dirfnt[MAX_FONTLIST];
static int ndirfnt = 0;
static int actfnt = 0;
static GrFont *glbfnt = NULL;

void setup_tiles(void);
int loadfont(int n);
int loadfontlist(void);
void paint_tile1(void *data);
void paint_tile2(void *data);
void paint_tile3(void *data);
int process_tile2_event(void *data, GrEvent *ev);
int process_tile3_event(void *data, GrEvent *ev);

int main(int argc, char **argv)
{
    GrEvent ev;
    int ret;
    
    if (argc >= 4) {
        gwidth = atoi(argv[1]);
        gheight = atoi(argv[2]);
        gbpp = atoi(argv[3]);
    }

    if (loadfontlist() != 0 ) {
        printf("Error loading font list\n");
        return 1 ;
    }

    GrSetMode(GR_width_height_bpp_graphics, gwidth, gheight, gbpp);
    GrGenEgaColorTable();
    GrSetFontPath("../fonts/;./");
    GrSetDefaultFont(&GrFont_PX8x18);
    GUIInit(1, 0);

    setup_tiles();

    GUITilePaint(IDT_INFO);
    GUITilePaint(IDT_SELEC);
    GUITilePaint(IDT_SHOW);

    while(1) {
        GrEventRead(&ev);
        if ((ev.type == GREV_COMMAND) && (ev.p1 == COMMAND_EXIT))
            break;
        if ((ev.type == GREV_FCHANGE) && (ev.p1 == 0)) {
            loadfont(ev.p2);
            GUITilePaint(IDT_INFO);
            GUITilePaint(IDT_SHOW);
            continue;
        }
        ret = GUITilesProcessEvent(&ev);
        if (ret == -1) break;
    }
    
    GUIGroupDestroy(grp1);
    GUITilesDestroyAll();
    
    GUIEnd();
    GrSetMode(GR_default_text);

    return 0;
}

void setup_tiles(void)
{
#define TILE1H 30
#define TILE2W 192
    
    int h;

    GUIScrollbarsSetColors(EGAC_LIGHTGRAY, EGAC_DARKGRAY);
    GUITilesSetColors(EGAC_BLACK, EGAC_LIGHTGRAY, EGAC_YELLOW);
    GUIObjectsSetChrType(GR_UTF8_TEXT); // this source is UTF8 coded
    GUIObjectsSetColors(EGAC_BLACK, EGAC_LIGHTGRAY, EGAC_DARKGRAY);

    gt1 = GUITileCreate(IDT_INFO, GUI_TT_STATICBORDER, 0, 0,
                        GrSizeX(), TILE1H);
    gt2 = GUITileCreate(IDT_SELEC, GUI_TT_ACTIVEBORDER, 0, TILE1H,
                        TILE2W, GrSizeY()-TILE1H);
    gt3 = GUITileCreate(IDT_SHOW, GUI_TT_ACTIVEBORDER, TILE2W, TILE1H,
                        GrSizeX()-TILE2W, GrSizeY()-TILE1H);
    if (gt1 == NULL || gt2 == NULL || gt3 == NULL) exit(1);
    
    GUIPanelSetClCallBacks(gt1->p, paint_tile1, NULL);
    GUIPanelSetUserData(gt1->p, (void *)&ud1);
    ud1.gt = gt1;
    ud1.gr = NULL;
    ud1.fg = EGAC_WHITE;
    ud1.bg = EGAC_CYAN;
    GUIPanelSetClCallBacks(gt2->p, paint_tile2, process_tile2_event);
    GUIPanelSetUserData(gt2->p, (void *)&ud2);
    ud2.gt = gt2;
    ud2.gr = NULL;
    ud2.fg = EGAC_WHITE;
    ud2.bg = EGAC_GREEN;
    GUIPanelSetClCallBacks(gt3->p, paint_tile3, process_tile3_event);
    GUIPanelSetUserData(gt3->p, (void *)&ud3);
    ud3.gt = gt3;
    ud3.gr = NULL;
    ud3.fg = EGAC_BLACK;
    ud3.bg = EGAC_WHITE;

    GUITileRegister(gt1);
    GUITileRegister(gt2);
    GUITileRegister(gt3);

    grp1 = GUIGroupCreate(2, 12, 12);
    if (grp1 == NULL) exit(1);
    h = GrSizeY() - TILE1H - 32;
    //GUIObjectSetDList(&(grp1->o[0]), 0, 0, 0, 160, h-45, EGAC_WHITE, EGAC_BLACK, (void **)listopt, 10, 0);
    GUIObjectSetDList(&(grp1->o[0]), 0, 0, 0, 160, h-45, EGAC_WHITE, EGAC_BLACK, (void **)dirfnt, ndirfnt, 0);
    GUIObjectSetButton(&(grp1->o[1]), 1, 0, h-40, 160, 40, EGAC_RED, EGAC_WHITE, "Exit", COMMAND_EXIT, 0, 0);
    GUIGroupSetSelected(grp1, 1, 0);
    GUIGroupSetPanel(grp1, gt2->p);
    ud2.gr = grp1;
}

int loadfont(int n)
{
    if (glbfnt) GrUnloadFont(glbfnt);
    glbfnt = GrLoadFont(dirfnt[n]);
    if (glbfnt == NULL) return -1;
    actfnt = n;

    return 0;
}

int loadfontlist(void)
{
    FILE *f;
    char s[251];
    char *p;

    ndirfnt = 0;
    f = fopen(DIR_FILE1, "rt");
    if (f == NULL) f = fopen(DIR_FILE2, "rt");
    if (f == NULL) return -1;

    while (fgets(s, 250, f)) {
        if (s[0] == '#') continue;
        p = strchr(s, ' ');
        if (p && ndirfnt < MAX_FONTLIST) {
            *p = '\0';
            dirfnt[ndirfnt++] = strdup(s);
        }
    }
    return 0;
}

void paint_tile1(void *data)
{
    UserData *ud;
    char s[241], *p;

    ud = (UserData *)data;
    GrClearContext(ud->bg);
    if (glbfnt == NULL) {
        GrTextXY(10, 6, "Showfnt2, no font loaded", ud->fg, ud->bg);
    } else {
        p = s;
        p    += sprintf(p, "Name: %s", glbfnt->h.name);
        if (glbfnt->h.proportional) {
            p += sprintf(p, " (%d-%d)x%d prop.", glbfnt->minwidth,
                         glbfnt->maxwidth, glbfnt->h.height);
        } else {
            p += sprintf(p, " %dx%d fixed", glbfnt->h.width, glbfnt->h.height);
        }
        p += sprintf(p, "  Glyphs: %d/%d ", glbfnt->h.minchar,
                     glbfnt->h.minchar + glbfnt->h.numchars - 1);
        p += sprintf(p, "  Encd: %d (%s) ", glbfnt->h.encoding,
                     GrStrFontEncoding(glbfnt->h.encoding));
        p += sprintf(p, "  File: %s ", dirfnt[actfnt]);
        GrTextXY(10, 6, s, ud->fg, ud->bg);
    }
}

void paint_tile2(void *data)
{
    UserData *ud;

    ud = (UserData *)data;
    GrClearContext(EGAC_GREEN);
    GUIGroupPaint(ud->gr);
}

void paint_tile3(void *data)
{
    UserData *ud;
    GrTextOption grt;
    int x, y, h, i, j;
    char message[121];

    ud = (UserData *)data;
    GrClearContext(ud->bg);
    if (glbfnt == NULL) {
        GrTextXY(10, 10, "Select a font in the left tile to show it", ud->fg, ud->bg);
        GrTextXY(10, 26, "When this tile is selected:", ud->fg, ud->bg);
        GrTextXY(10, 42, "  Press R to reverse colors", ud->fg, ud->bg);
        GrTextXY(10, 58, "  Esc to finish", ud->fg, ud->bg);
    } else {
        grt.txo_fgcolor = ud->fg;
        grt.txo_bgcolor = ud->bg;
        grt.txo_direct = GR_TEXT_RIGHT;
        grt.txo_xalign = GR_ALIGN_LEFT;
        grt.txo_yalign = GR_ALIGN_TOP;
        grt.txo_chrtype = GR_UTF8_TEXT;
        grt.txo_font = glbfnt;
        x = 2;
        y = 2;
        h = glbfnt->h.height + 2;
        
        sprintf(message, "%s %dx%d %s. Hello, MGRX world!", dirfnt[actfnt],
                glbfnt->h.width, glbfnt->h.height,
                (glbfnt->h.proportional) ? "proportional" : "fixed");
        GrDrawString(message, 0, x, y, &grt);
        y += 2*h;
        sprintf(message, "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG ÑOÑO ÁCÉNTÓS" );
        GrDrawString(message, 0, x, y, &grt);
        y += h;
        sprintf(message, "the quick brown fox jumps over the lazy dog ñoño ácéntós" );
        GrDrawString(message, 0, x, y, &grt);
        y += 2*h;

        j = 0;
        grt.txo_chrtype = GR_WORD_TEXT;
        for (i=glbfnt->h.minchar; i<glbfnt->h.minchar+glbfnt->h.numchars; i++) {
            GrDrawChar(i, x, y, &grt);
            x += GrCharWidth(i, &grt);
            j++;
            if (j >= 32) {
                j = 0;
                y += h;
                x = 2;
            }
        }
        grt.txo_chrtype = GR_UTF8_TEXT;
    }
}

int process_tile2_event(void *data, GrEvent *ev)
{
    UserData *ud;

    ud = (UserData *)data;

    if ((ev->type == GREV_KEY) && (ev->p1 == GrKey_Escape))
        return -1;

    return GUIGroupProcessEvent(ud->gr, ev);
}

int process_tile3_event(void *data, GrEvent *ev)
{
    UserData *ud;
    GrColor aux;

    ud = (UserData *)data;
    if (ev->type == GREV_KEY) {
        if (ev->p1 == 'r' || ev->p1 == 'R') {
            aux = ud->fg;
            ud->fg = ud->bg;
            ud->bg = aux;
            ud->gt->p->paintcl(data);
            return 1;
        }
        if (ev->p1 == GrKey_Escape) {
            return -1;
        }
    }
    return 0;
}

