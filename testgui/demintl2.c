/**
 ** demintl2.c ---- MGRX Test intl support and GrGUI (full)
 **
 ** Copyright (C) 2019 Mariano Alvarez Fernandez
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "mgrx.h"
//#include "mgrxkeys.h"
#include "grgui.h"
#include "mgrxcolr.h"

/* commands generate by menus or buttons */

#define COMMAND_RES_640x480x8       1
#define COMMAND_RES_800x600x8       2
#define COMMAND_RES_1024x768x8      3
#define COMMAND_RES_1920x1080x8     4
#define COMMAND_RES_4000x4000x8     5
#define COMMAND_RES_640x480x24     11
#define COMMAND_RES_800x600x24     12
#define COMMAND_RES_1024x768x24    13
#define COMMAND_RES_1920x1080x24   14
#define COMMAND_RES_4000x4000x24   15
#define COMMAND_EXIT              100

#define COMMAND_FONT_CP437        201
#define COMMAND_FONT_CP850        202
#define COMMAND_FONT_CP1252       203
#define COMMAND_FONT_ISO88591     204
#define COMMAND_FONT_MGRX         205
#define COMMAND_UENC_CP437        211
#define COMMAND_UENC_CP850        212
#define COMMAND_UENC_CP1252       213
#define COMMAND_UENC_ISO88591     214
#define COMMAND_UENC_UTF8         215
#define COMMAND_UENC_UCS2         216
#define COMMAND_TEST1             221
#define COMMAND_TEST2             222
#define COMMAND_TEST3             223
#define COMMAND_TEST4             224

#define COMMAND_TA_CHARCOLOR      230
#define COMMAND_TA_CLEAR          231
#define COMMAND_TA_SAVE           232

#define COMMAND_DLG_ABOUT         240
#define COMMAND_DLG_YN            241
#define COMMAND_DLG_YNC           242
#define COMMAND_TEST_OBJ1         243
#define COMMAND_TEST_OBJ2         244
#define COMMAND_USE_DB            245
#define COMMAND_MANAGE_EXP        246

#define COMMAND_LOADFONT_FIRST    300
#define COMMAND_LOADFONT_CP437    300
#define COMMAND_LOADFONT_CP850    320
#define COMMAND_LOADFONT_ISO59    340
#define COMMAND_LOADFONT_MGRX     400
#define COMMAND_LOADFONT_LAST     499

/* tiles ids */

#define IDT_INFO    1
#define IDT_BUTTONS 2
#define IDT_EDIT    3
#define IDT_STATUS  4

/* global variables */

static int globalw = 800;
static int globalh = 600;
static int globalbpp = 8;

static int usedoublebuffer = 0;
static int manageexposeevents = 0;

static GrTextOption grt_info, grt_status, grt_status2;

static GUITile *tile1, *tile2, *tile3, *tile4;

static GUIGroup *grp1;

static GUITextArea *textarea1;
static GrColor textarea1fgcolor;

static GrFont *grf_CP437 = NULL;
static GrFont *grf_CP850 = NULL;
static GrFont *grf_CP1252 = NULL;
static GrFont *grf_ISO88591 = NULL;
static GrFont *grf_MGRX = NULL;
static GrFont *texta_font = NULL;

static char *font_name_CP437 = "pc8x16.fnt";
static char *font_name_CP850 = "pc850-19.fnt";
static char *font_name_CP1252 = "ter-114b.res";
static char *font_name_ISO88591 = "ncen22b.fnt";
static char *font_name_MGRX = "tmgrx22b.fnt";
static char *stext_font = NULL;

char *abouttext[3] = {
    "Demintl2 tests the MGRX international text encoding",
    "capabilities and the GrGUI mini GUI, MGRX is a small C",
    "2D graphics library, mgrx.fgrim.com for more info"};

char *listopt[5] = {
    "Primera opción", 
    "Segunda opción", 
    "Tercera opción",
    "Cuarta opción",
    "Quinta opción"};

static char bdtline1[81], bdtline2[81], bdtline3[81], bdtline4[81];
static char *buf_dlg_test[4] = {bdtline1, bdtline2, bdtline3, bdtline4};

/* function declarations */

static void setup(void);
static void setup_fonts(void);
static void setup_menus(void);
static void setup_tiles(void);
static void setup_groups(void);
static void terminate(void);
static void disaster(char *s);
static void set_desired_resolution(int dgw, int dgh, int dbpp);
static int process_command(GrEvent *ev);
static int process_fchange(GrEvent *ev);
static int process_loadfont(long nfont);
static void restart_paint(int rbut);
static void restart_groups(void);
static void save_content_to_file(void);
static void dotest(int n);
static void draw_string_collection(void);
static void file_listing(void);
static void print_dlg_result( char *s, int d);
static void print_test_dlg_result(void);

void paint_panel1(void *udata);
void paint_panel2(void *udata);
void paint_panel3(void *udata);
void paint_panel4(void *udata);
int processev_panel2(void *udata, GrEvent *ev);
int processev_panel3(void *udata, GrEvent *ev);

static void dlg_ta_color(void);
static void dlg_test_obj1(void);
static void dlg_test_obj2(void);

/* main program begin */

int main()
{
    GrEvent ev;
    
    while (1) {
        setup();
        GUIMenuBarShow();
        GUITilePaint(IDT_INFO);
        GUITilePaint(IDT_BUTTONS);
        GUITilePaint(IDT_EDIT);
        GUITilePaint(IDT_STATUS);
        GUITAShowCursor(textarea1);
        while (1) {
            GrEventRead(&ev);
            if (GUITilesProcessEvent(&ev)) continue;
            if (ev.type == GREV_KEY && ev.p1 == GrKey_Escape) {
                globalw = -1;
                break;
            }
            if (ev.type == GREV_COMMAND) {
                if (process_command(&ev)) break;
            }
            if (ev.type == GREV_FCHANGE) {
                if (process_fchange(&ev)) break;
            }
        }
        terminate();
        if (globalw < 0) break;
    }

    return 0;
}

/**************************/

static void setup(void)
{
    static char *wintitle = "MGRX+GrGUI 1.3.0, the graphics library";

    GrSetMode(GR_width_height_bpp_graphics, globalw, globalh, globalbpp);
    GrGenEgaColorTable();
    GrSetFontPath("../fonts/;./");
    GUIInit(1, usedoublebuffer);
    GUIManageExposeEvents(manageexposeevents);
    //GrEventGenExpose(GR_GEN_EXPOSE_YES);
    GrSetWindowTitle(wintitle);
    //GrClearScreen(EGAC_BROWN);

    setup_fonts();
    setup_menus();
    setup_tiles();
    setup_groups();
}

/**************************/

static void setup_fonts(void)
{
    char aux[81];

    if (grf_CP437 == NULL) grf_CP437 = GrLoadFont(font_name_CP437);
    if (grf_CP437 == NULL) {
        sprintf(aux,"%s not found", font_name_CP437);
        disaster(aux);
    }
    //GrFontSetEncoding(grf_CP437, GR_FONTENC_CP437);

    if (grf_CP850 == NULL) grf_CP850 = GrLoadFont(font_name_CP850);
    if (grf_CP850 == NULL) {
        sprintf(aux,"%s not found", font_name_CP850);
        disaster(aux);
    }
    //GrFontSetEncoding(grf_CP850, GR_FONTENC_CP850);

    if (grf_CP1252 == NULL) grf_CP1252 = GrLoadFont(font_name_CP1252);
    if (grf_CP1252 == NULL) {
        sprintf(aux,"%s not found", font_name_CP1252);
        disaster(aux);
    }
    GrFontSetEncoding(grf_CP1252, GR_FONTENC_CP1252);

    if (grf_ISO88591 == NULL) grf_ISO88591 = GrLoadFont(font_name_ISO88591);
    if (grf_ISO88591 == NULL) {
        sprintf(aux,"%s not found", font_name_ISO88591);
        disaster(aux);
    }
    //GrFontSetEncoding(grf_ISO88591, GR_FONTENC_ISO_8859_1);

    if (grf_MGRX == NULL) grf_MGRX = GrLoadFont(font_name_MGRX);
    if (grf_MGRX == NULL) {
        sprintf(aux,"%s not found", font_name_MGRX);
        disaster(aux);
    }
    //GrFontSetEncoding(grf_MGRX, GR_FONTENC_MGRX512);

    if (texta_font == NULL) {
        texta_font = grf_CP437;
        stext_font = font_name_CP437;
    }
}

/**************************/

static void setup_menus(void)
{
    static GUIMenuItem itemsm1[13] = {
        {GUI_MI_OPER, 1, "&A 640x480x8", 'A', NULL, 0, COMMAND_RES_640x480x8, 0}, 
        {GUI_MI_OPER, 1, "&B 800x600x8", 'B', NULL, 0, COMMAND_RES_800x600x8, 0}, 
        {GUI_MI_OPER, 1, "&C 1024x768x8", 'C', NULL, 0, COMMAND_RES_1024x768x8, 0}, 
        {GUI_MI_OPER, 1, "&D 1920x1080x8", 'D', NULL, 0, COMMAND_RES_1920x1080x8, 0}, 
        {GUI_MI_OPER, 1, "&E 4000x4000x8", 'E', NULL, 0, COMMAND_RES_4000x4000x8, 0}, 
        {GUI_MI_SEP, 1, "", 0, NULL, 0, 0, 0}, 
        {GUI_MI_OPER, 1, "&F 640x480x24", 'F', NULL, 0, COMMAND_RES_640x480x24, 0}, 
        {GUI_MI_OPER, 1, "&G 800x600x24", 'G', NULL, 0, COMMAND_RES_800x600x24, 0}, 
        {GUI_MI_OPER, 1, "&H 1024x768x24", 'H', NULL, 0, COMMAND_RES_1024x768x24, 0}, 
        {GUI_MI_OPER, 1, "&I 1920x1080x24", 'I', NULL, 0, COMMAND_RES_1920x1080x24, 0}, 
        {GUI_MI_OPER, 1, "&J 4000x4000x24", 'J', NULL, 0, COMMAND_RES_4000x4000x24, 0}, 
        {GUI_MI_SEP, 1, "", 0, NULL, 0, 0, 0}, 
        {GUI_MI_OPER, 1, "E&xit", 'X', "Ctrl+X", GrKey_Control_X, COMMAND_EXIT, 0}};

    static GUIMenu menu1 = {1, 13, 0, itemsm1};

    static GUIMenuItem itemsm2[10] = {
        {GUI_MI_MENU, 1, "&1 Select CP437 font", '1', NULL, 0, 21, 0}, 
        {GUI_MI_MENU, 1, "&2 Select CP850 font", '2', NULL, 0, 22, 0}, 
        {GUI_MI_MENU, 0, "&3 Select CP1252 font", '3', NULL, 0, 23, 0}, 
        {GUI_MI_MENU, 1, "&4 Select ISO8859-1 font", '4', NULL, 0, 24, 0}, 
        {GUI_MI_MENU, 1, "&5 Select MGRX font", '5', NULL, 0, 25, 0}, 
        {GUI_MI_SEP, 1, "", 0, NULL, 0, 0, 0}, 
        {GUI_MI_OPER, 1, "&Save content to demintl2.out", 'S', NULL, 0, COMMAND_TA_SAVE, 0},
        {GUI_MI_SEP, 1, "", 0, NULL, 0, 0, 0}, 
        {GUI_MI_OPER, 1, "&Char color", 'C', "Alt+C", GrKey_Alt_C, COMMAND_TA_CHARCOLOR, 0},
        {GUI_MI_OPER, 1, "C&lear text area", 'L', "Alt+L", GrKey_Alt_L, COMMAND_TA_CLEAR, 0}};

    static GUIMenu menu2 = {2, 10, 0, itemsm2};

    static GUIMenuItem itemsm21[7] = {
        {GUI_MI_OPER, 1, "&A pc6x8.fnt", 'A', NULL, 0, COMMAND_LOADFONT_CP437+1, 0}, 
        {GUI_MI_OPER, 1, "&B pc6x14.fnt", 'B', NULL, 0, COMMAND_LOADFONT_CP437+2, 0}, 
        {GUI_MI_OPER, 1, "&C pc8x8.fnt", 'C', NULL, 0, COMMAND_LOADFONT_CP437+3, 0}, 
        {GUI_MI_OPER, 1, "&D pc8x8t.fnt", 'D', NULL, 0, COMMAND_LOADFONT_CP437+4, 0}, 
        {GUI_MI_OPER, 1, "&E pc8x14.fnt", 'E', NULL, 0, COMMAND_LOADFONT_CP437+5, 0}, 
        {GUI_MI_OPER, 1, "&F pc8x14t.fnt", 'F', NULL, 0, COMMAND_LOADFONT_CP437+6, 0}, 
        {GUI_MI_OPER, 1, "&G pc8x16.fnt", 'G', NULL, 0, COMMAND_LOADFONT_CP437+7, 0}};

    static GUIMenu menu21 = {21, 7, 0, itemsm21};

    static GUIMenuItem itemsm22[3] = {
        {GUI_MI_OPER, 1, "&A pc850-14.fnt", 'A', NULL, 0, COMMAND_LOADFONT_CP850+1, 0}, 
        {GUI_MI_OPER, 1, "&B pc850-16.fnt", 'B', NULL, 0, COMMAND_LOADFONT_CP850+2, 0}, 
        {GUI_MI_OPER, 1, "&C pc850-19.fnt", 'C', NULL, 0, COMMAND_LOADFONT_CP850+3, 0}};

    static GUIMenu menu22 = {22, 3, 0, itemsm22};

    static GUIMenuItem itemsm24[20] = {
        {GUI_MI_OPER, 1, "&A char23i.fnt", 'A', NULL, 0, COMMAND_LOADFONT_ISO59+1, 0}, 
        {GUI_MI_OPER, 1, "&B char23b.fnt", 'B', NULL, 0, COMMAND_LOADFONT_ISO59+2, 0}, 
        {GUI_MI_OPER, 1, "&C char30i.fnt", 'C', NULL, 0, COMMAND_LOADFONT_ISO59+3, 0}, 
        {GUI_MI_OPER, 1, "&D char30b.fnt", 'D', NULL, 0, COMMAND_LOADFONT_ISO59+4, 0}, 
        {GUI_MI_OPER, 1, "&E lucb21i.fnt", 'E', NULL, 0, COMMAND_LOADFONT_ISO59+5, 0}, 
        {GUI_MI_OPER, 1, "&F lucb21b.fnt", 'F', NULL, 0, COMMAND_LOADFONT_ISO59+6, 0}, 
        {GUI_MI_OPER, 1, "&G lucb27i.fnt", 'G', NULL, 0, COMMAND_LOADFONT_ISO59+7, 0}, 
        {GUI_MI_OPER, 1, "&H lucb27b.fnt", 'H', NULL, 0, COMMAND_LOADFONT_ISO59+8, 0}, 
        {GUI_MI_OPER, 1, "&I lucs21i.fnt", 'I', NULL, 0, COMMAND_LOADFONT_ISO59+9, 0}, 
        {GUI_MI_OPER, 1, "&J lucs21b.fnt", 'J', NULL, 0, COMMAND_LOADFONT_ISO59+10, 0}, 
        {GUI_MI_OPER, 1, "&K lucs27i.fnt", 'K', NULL, 0, COMMAND_LOADFONT_ISO59+11, 0}, 
        {GUI_MI_OPER, 1, "&L lucs27b.fnt", 'L', NULL, 0, COMMAND_LOADFONT_ISO59+12, 0}, 
        {GUI_MI_OPER, 1, "&M luct19.fnt",  'M', NULL, 0, COMMAND_LOADFONT_ISO59+13, 0}, 
        {GUI_MI_OPER, 1, "&N luct19b.fnt", 'N', NULL, 0, COMMAND_LOADFONT_ISO59+14, 0}, 
        {GUI_MI_OPER, 1, "&O luct27.fnt",  'O', NULL, 0, COMMAND_LOADFONT_ISO59+15, 0}, 
        {GUI_MI_OPER, 1, "&P luct27b.fnt", 'P', NULL, 0, COMMAND_LOADFONT_ISO59+16, 0}, 
        {GUI_MI_OPER, 1, "&Q ncen22i.fnt", 'Q', NULL, 0, COMMAND_LOADFONT_ISO59+17, 0}, 
        {GUI_MI_OPER, 1, "&R ncen22b.fnt", 'R', NULL, 0, COMMAND_LOADFONT_ISO59+18, 0}, 
        {GUI_MI_OPER, 1, "&S ncen29i.fnt", 'S', NULL, 0, COMMAND_LOADFONT_ISO59+19, 0},
        {GUI_MI_OPER, 1, "&T ncen29b.fnt", 'T', NULL, 0, COMMAND_LOADFONT_ISO59+20, 0}};

    static GUIMenu menu24 = {24, 20, 0, itemsm24};

    static GUIMenuItem itemsm25[17] = {
        {GUI_MI_OPER, 1, "&A tmgrx12n.fnt", 'A', NULL, 0, COMMAND_LOADFONT_MGRX+1, 0}, 
        {GUI_MI_OPER, 1, "&B tmgrx14b.fnt", 'B', NULL, 0, COMMAND_LOADFONT_MGRX+2, 0}, 
        {GUI_MI_OPER, 1, "&C tmgrx14n.fnt", 'C', NULL, 0, COMMAND_LOADFONT_MGRX+3, 0}, 
        {GUI_MI_OPER, 1, "&D tmgrx16b.fnt", 'D', NULL, 0, COMMAND_LOADFONT_MGRX+4, 0}, 
        {GUI_MI_OPER, 1, "&E tmgrx16n.fnt", 'E', NULL, 0, COMMAND_LOADFONT_MGRX+5, 0}, 
        {GUI_MI_OPER, 1, "&F tmgrx18b.fnt", 'F', NULL, 0, COMMAND_LOADFONT_MGRX+6, 0}, 
        {GUI_MI_OPER, 1, "&G tmgrx18n.fnt", 'G', NULL, 0, COMMAND_LOADFONT_MGRX+7, 0}, 
        {GUI_MI_OPER, 1, "&H tmgrx20b.fnt", 'H', NULL, 0, COMMAND_LOADFONT_MGRX+8, 0}, 
        {GUI_MI_OPER, 1, "&I tmgrx20n.fnt", 'I', NULL, 0, COMMAND_LOADFONT_MGRX+9, 0}, 
        {GUI_MI_OPER, 1, "&J tmgrx22b.fnt", 'J', NULL, 0, COMMAND_LOADFONT_MGRX+10, 0}, 
        {GUI_MI_OPER, 1, "&K tmgrx22n.fnt", 'K', NULL, 0, COMMAND_LOADFONT_MGRX+11, 0}, 
        {GUI_MI_OPER, 1, "&L tmgrx24b.fnt", 'L', NULL, 0, COMMAND_LOADFONT_MGRX+12, 0}, 
        {GUI_MI_OPER, 1, "&M tmgrx24n.fnt", 'M', NULL, 0, COMMAND_LOADFONT_MGRX+13, 0}, 
        {GUI_MI_OPER, 1, "&N tmgrx28b.fnt", 'N', NULL, 0, COMMAND_LOADFONT_MGRX+14, 0}, 
        {GUI_MI_OPER, 1, "&O tmgrx28n.fnt", 'O', NULL, 0, COMMAND_LOADFONT_MGRX+15, 0},
        {GUI_MI_OPER, 1, "&P tmgrx32b.fnt", 'P', NULL, 0, COMMAND_LOADFONT_MGRX+16, 0}, 
        {GUI_MI_OPER, 1, "&Q tmgrx32n.fnt", 'Q', NULL, 0, COMMAND_LOADFONT_MGRX+17, 0}};

    static GUIMenu menu25 = {25, 17, 0, itemsm25};

    static GUIMenuItem itemsm3[10] = {
        {GUI_MI_OPER, 1, "&About", 'A', NULL, 0, COMMAND_DLG_ABOUT, 0},
        {GUI_MI_SEP, 1, "", 0, NULL, 0, 0, 0}, 
        {GUI_MI_OPER, 1, "&Test Yes/No dialog", 'T', NULL, 0, COMMAND_DLG_YN, 0},
        {GUI_MI_OPER, 1, "T&est Yes/No/Cancel dialog", 'E', NULL, 0, COMMAND_DLG_YNC, 0},
        {GUI_MI_SEP, 1, "", 0, NULL, 0, 0, 0}, 
        {GUI_MI_OPER, 1, "Test &Objects 1", 'O', NULL, 0, COMMAND_TEST_OBJ1, 0},
        {GUI_MI_OPER, 1, "Test O&bjects 2", 'B', NULL, 0, COMMAND_TEST_OBJ2, 0},
        {GUI_MI_SEP, 1, "", 0, NULL, 0, 0, 0}, 
        {GUI_MI_OPER, 1, "&Use DoubleBuffer", 'U', NULL, 0, COMMAND_USE_DB, 0},
        {GUI_MI_OPER, 0, "&Manage Expose Events", 'M', NULL, 0, COMMAND_MANAGE_EXP, 0}};

    static GUIMenu menu3 = {3, 10, 0, itemsm3};

    static GUIMenuBarItem mbitems[3] = {
        {"&Resolution", 1, GrKey_Alt_R, 1}, 
        {"&TextArea", 1, GrKey_Alt_T, 2},
        {"Test&GUI", 1, GrKey_Alt_G, 3}};

    static GUIMenuBar menubar = {3 ,0, mbitems};

    GUIMenusSetChrType(GR_UTF8_TEXT); // this source is UTF8 coded
//    GUIMenusSetFontByName("univ12b.fnt");
//    GUIMenusSetFontByName("anti10.fnt");
//    GUIMenusSetFontByName("ncen22.fnt");
//    GUIMenusSetFontByName("ter-mgrx20n.psf");
    GUIMenusSetFontByName("tmgrx18b.fnt");
//    GUIMenuSetFontByName("tmgrx12n.fnt");
//    GUIMenuSetFontByName("c:/grxfonts/univ09.fnt");

    GUIMenusSetColors(EGAC_WHITE, EGAC_RED, EGAC_BLACK,
                      EGAC_YELLOW, EGAC_LIGHTGRAY, EGAC_DARKGRAY);

    GUIMenuRegister(&menu1);
    GUIMenuRegister(&menu2);
    GUIMenuRegister(&menu21);
    GUIMenuRegister(&menu22);
    GUIMenuRegister(&menu24);
    GUIMenuRegister(&menu25);
    GUIMenuRegister(&menu3);
    GUIMenuBarSet(&menubar);
}

/**************************/

static void setup_tiles(void)
{
#define PAN1H 60
#define PAN2W 200

    int mbheight, stheight;

    grt_info.txo_font = &GrFont_PC8x16;
    grt_info.txo_fgcolor = EGAC_BLACK;
    grt_info.txo_bgcolor = GrNOCOLOR;
    grt_info.txo_direct = GR_TEXT_RIGHT;
    grt_info.txo_xalign = GR_ALIGN_CENTER;
    grt_info.txo_yalign = GR_ALIGN_TOP;
    grt_info.txo_chrtype = GR_UTF8_TEXT; /* this source file is UTF-8 coded!! */
    grt_status = grt_info;
    grt_status.txo_fgcolor = EGAC_WHITE;
    grt_status.txo_xalign = GR_ALIGN_RIGHT;
    grt_status2 = grt_info;
    grt_status2.txo_fgcolor = EGAC_LIGHTRED;
    grt_status2.txo_xalign = GR_ALIGN_LEFT;

    GUIScrollbarsSetColors(EGAC_LIGHTGRAY, EGAC_DARKGRAY);
    GUITilesSetColors(EGAC_BLACK, EGAC_LIGHTGRAY, EGAC_YELLOW);

    mbheight = GUIMenuBarGetHeight();
    stheight = grt_status.txo_font->h.height + 2;

    tile1 = GUITileCreate(IDT_INFO, GUI_TT_STATICBORDER, 0, mbheight,
                          GrSizeX(), PAN1H);
    tile2 = GUITileCreate(IDT_BUTTONS, GUI_TT_ACTIVEBORDER, 0, PAN1H+mbheight,
                          PAN2W, GrSizeY()-PAN1H-mbheight);
    tile3 = GUITileCreate(IDT_EDIT, GUI_TT_ACTIVEBWSCB, PAN2W, PAN1H+mbheight,
                          GrSizeX()-PAN2W, GrSizeY()-PAN1H-mbheight-stheight);
    tile4 = GUITileCreate(IDT_STATUS, GUI_TT_BORDERLESS, PAN2W, GrSizeY()-stheight,
                          GrSizeX()-PAN2W, stheight);
    
    GUIPanelSetClCallBacks(tile1->p, paint_panel1, NULL);
    GUIPanelSetClCallBacks(tile2->p, paint_panel2, processev_panel2);
    GUIPanelSetClCallBacks(tile3->p, paint_panel3, processev_panel3);
    GUIPanelSetClCallBacks(tile4->p, paint_panel4, NULL);

    GUITileRegister(tile1);
    GUITileRegister(tile2);
    GUITileRegister(tile3);
    GUITileRegister(tile4);
    
    textarea1 = GUITACreate(tile3->p, NULL, 10000);
    if (textarea1 == NULL) {
        disaster("No memory available for TextArea");
    }
    GUITASetBgColor(textarea1, EGAC_DARKGRAY);
    textarea1fgcolor = EGAC_WHITE;
    GUITASetTextColors(textarea1, textarea1fgcolor, EGAC_DARKGRAY);
    GUITASetCursorColor(textarea1, EGAC_YELLOW);
    GUITASetFont(textarea1, texta_font);
    GUITAClear(textarea1);

    GUIDialogsSetColors(EGAC_BLACK, EGAC_YELLOW, EGAC_BLUE, EGAC_WHITE);
    GUICDialogsSetColors(EGAC_GREEN, EGAC_WHITE);
    GUIDialogsSetTitleFontByName("tmgrx18n.fnt");
    GUICDialogsSetFontByName("tmgrx18n.fnt");

#undef PAN2W
#undef PAN1H
}

/**************************/

static void setup_groups(void)
{
#define PX0 0
#define PX1 80
#define PY0 0
#define INCRY 42
#define NOBJECTSP1 19

    GUIObjectsSetChrType(GR_UTF8_TEXT); // this source is UTF8 coded
    GUIObjectsSetFontByName("tmgrx16b.fnt");
    GUIObjectsSetColors(EGAC_BLACK, EGAC_LIGHTGRAY, EGAC_DARKGRAY);

    grp1 = GUIGroupCreate(NOBJECTSP1, 16, 32);
    GUIObjectSetLabel(&(grp1->o[0]), 0, PX0, PY0-20, 160, 20, GrNOCOLOR, EGAC_BLACK, "Set FontEncoding");
    GUIObjectSetButton(&(grp1->o[1]), 1, PX0, PY0, 80, 40, EGAC_BLUE, EGAC_YELLOW, "CP437", COMMAND_FONT_CP437, 101, 1);
    GUIObjectSetButton(&(grp1->o[2]), 2, PX1, PY0, 80, 40, EGAC_BLUE, EGAC_YELLOW, "CP850", COMMAND_FONT_CP850, 101, 0);
    GUIObjectSetButton(&(grp1->o[3]), 3, PX0, PY0+INCRY*1, 80, 40, EGAC_BLUE, EGAC_YELLOW, "CP1252", COMMAND_FONT_CP1252, 101, 0);
    GUIObjectSetButton(&(grp1->o[4]), 4, PX1, PY0+INCRY*1, 80, 40, EGAC_BLUE, EGAC_YELLOW, "8859-1", COMMAND_FONT_ISO88591, 101, 0);
    GUIObjectSetButton(&(grp1->o[5]), 5, PX0, PY0+INCRY*2, 80, 40, EGAC_BLUE, EGAC_YELLOW, "MGRX", COMMAND_FONT_MGRX, 101, 0);
    GUIObjectSetLabel(&(grp1->o[6]), 6, PX0, PY0+INCRY*3.5-20, 160, 20, GrNOCOLOR, EGAC_BLACK, "Set UserEncoding");
    GUIObjectSetButton(&(grp1->o[7]), 7, PX0, PY0+INCRY*3.5, 80, 40, EGAC_GREEN, EGAC_YELLOW, "CP437", COMMAND_UENC_CP437, 102, 1);
    GUIObjectSetButton(&(grp1->o[8]), 8, PX1, PY0+INCRY*3.5, 80, 40, EGAC_GREEN, EGAC_YELLOW, "CP850", COMMAND_UENC_CP850, 102, 0);
    GUIObjectSetButton(&(grp1->o[9]), 9, PX0, PY0+INCRY*4.5, 80, 40, EGAC_GREEN, EGAC_YELLOW, "CP1252", COMMAND_UENC_CP1252, 102, 0);
    GUIObjectSetButton(&(grp1->o[10]), 10, PX1, PY0+INCRY*4.5, 80, 40, EGAC_GREEN, EGAC_YELLOW, "8859-1", COMMAND_UENC_ISO88591, 102, 0);
    GUIObjectSetButton(&(grp1->o[11]), 11, PX0, PY0+INCRY*5.5, 80, 40, EGAC_GREEN, EGAC_YELLOW, "UTF-8", COMMAND_UENC_UTF8, 102, 0);
    GUIObjectSetButton(&(grp1->o[12]), 12, PX1, PY0+INCRY*5.5, 80, 40, EGAC_GREEN, EGAC_YELLOW, "UCS-2", COMMAND_UENC_UCS2, 102, 0);
    GUIObjectSetButton(&(grp1->o[13]), 13, PX0, PY0+INCRY*7, 80, 40, EGAC_BROWN, EGAC_WHITE, "Glyphs", COMMAND_TEST1, 0, 0);
    GUIObjectSetButton(&(grp1->o[14]), 14, PX1, PY0+INCRY*7, 80, 40, EGAC_BROWN, EGAC_WHITE, "Chars", COMMAND_TEST2, 0, 0);
    GUIObjectSetButton(&(grp1->o[15]), 15, PX0, PY0+INCRY*8, 80, 40, EGAC_BROWN, EGAC_WHITE, "Strings", COMMAND_TEST3, 0, 0);
    GUIObjectSetButton(&(grp1->o[16]), 16, PX1, PY0+INCRY*8, 80, 40, EGAC_BROWN, EGAC_WHITE, "LoadFile", COMMAND_TEST4, 0, 0);
    GUIObjectSetEntry(&(grp1->o[17]), 17, PX0, PY0+INCRY*9.1, 160, 30, EGAC_WHITE, EGAC_BLACK, 30, "demintl2.c");
    GUIObjectSetButton(&(grp1->o[18]), 18, PX0, PY0+INCRY*10, 160, 40, EGAC_RED, EGAC_WHITE, "Exit", COMMAND_EXIT, 0, 0);
    GUIGroupSetSelected(grp1, 1, 0);
    GUIGroupSetPanel(grp1, tile2->p);
}

/**************************/

static void terminate(void)
{
    GUIGroupDestroy(grp1);
    GUITADestroy(textarea1);
    GUITilesDestroyAll();
    GUIEnd();
    GrSetMode(GR_default_text);
}

/**************************/

static void disaster(char *s)
{
    void _GrCloseVideoDriver(void);

    char aux[81];

    GUIEnd();
    _GrCloseVideoDriver();
    printf("Demointl: %s\n", s);
    printf("press Return to continue\n");
    fgets(aux, 80, stdin);
    exit(1);
}

/**************************/

static void set_desired_resolution(int dgw, int dgh, int dbpp)
{
    globalw = dgw;
    globalh = dgh;
    globalbpp = dbpp;
}

/**************************/

static int process_command(GrEvent *ev)
{
    int result;
    
    if (process_loadfont(ev->p1)) return 0;
        
    switch (ev->p1) {
        case COMMAND_RES_640x480x8 :
            set_desired_resolution(640, 480, 8);
            return 1;
        case COMMAND_RES_800x600x8 :
            set_desired_resolution(800, 600, 8);
            return 1;
        case COMMAND_RES_1024x768x8 :
            set_desired_resolution(1024, 768, 8);
            return 1;
        case COMMAND_RES_1920x1080x8 :
            set_desired_resolution(1920, 1080, 8);
            return 1;
        case COMMAND_RES_4000x4000x8 :
            set_desired_resolution(4000, 4000, 8);
            return 1;
        case COMMAND_RES_640x480x24 :
            set_desired_resolution(640, 480, 24);
            return 1;
        case COMMAND_RES_800x600x24 :
            set_desired_resolution(800, 600, 24);
            return 1;
        case COMMAND_RES_1024x768x24 :
            set_desired_resolution(1024, 768, 24);
            return 1;
        case COMMAND_RES_1920x1080x24 :
            set_desired_resolution(1920, 1080, 24);
            return 1;
        case COMMAND_RES_4000x4000x24 :
            set_desired_resolution(4000, 4000, 24);
            return 1;

        case COMMAND_FONT_CP437 :
            texta_font = grf_CP437;
            stext_font = font_name_CP437;
            restart_paint(0);
            return 0;
        case COMMAND_FONT_CP850 :
            texta_font = grf_CP850;
            stext_font = font_name_CP850;
            restart_paint(0);
            return 0;
        case COMMAND_FONT_CP1252 :
            texta_font = grf_CP1252;
            stext_font = font_name_CP1252;
            restart_paint(0);
            return 0;
        case COMMAND_FONT_ISO88591 :
            texta_font = grf_ISO88591;
            stext_font = font_name_ISO88591;
            restart_paint(0);
            return 0;
        case COMMAND_FONT_MGRX :
            texta_font = grf_MGRX;
            stext_font = font_name_MGRX;
            restart_paint(0);
            return 0;

        case COMMAND_UENC_CP437 :
            GrSetUserEncoding(GRENC_CP437);
            restart_paint(0);
            return 0;
        case COMMAND_UENC_CP850 :
            GrSetUserEncoding(GRENC_CP850);
            restart_paint(0);
            return 0;
        case COMMAND_UENC_CP1252 :
            GrSetUserEncoding(GRENC_CP1252);
            restart_paint(0);
            return 0;
        case COMMAND_UENC_ISO88591 :
            GrSetUserEncoding(GRENC_ISO_8859_1);
            restart_paint(0);
            return 0;
        case COMMAND_UENC_UTF8 :
            GrSetUserEncoding(GRENC_UTF_8);
            restart_paint(0);
            return 0;
        case COMMAND_UENC_UCS2 :
            GrSetUserEncoding(GRENC_UCS_2);
            restart_paint(0);
            return 0;

        case COMMAND_TEST1 :
            dotest(1);
            GUITilePaint(IDT_STATUS);
            return 0;
        case COMMAND_TEST2 :
            dotest(2);
            GUITilePaint(IDT_STATUS);
            return 0;
        case COMMAND_TEST3 :
            dotest(3);
            GUITilePaint(IDT_STATUS);
            return 0;
        case COMMAND_TEST4 :
            dotest(4);
            GUITilePaint(IDT_STATUS);
            return 0;

        case COMMAND_TA_SAVE :
            save_content_to_file();
            return 0;
        case COMMAND_TA_CLEAR :
            result = GUICDialogYesNo("Clear text area?", NULL, 0, "Yes", "No");
            if (result == 1) {
                GUITAClear(textarea1);
                GUITAShowCursor(textarea1);
                GUITilePaint(IDT_EDIT);
                GUITilePaint(IDT_STATUS);
            }
            return 0;
        case COMMAND_TA_CHARCOLOR :
            dlg_ta_color();
            return 0;

        case COMMAND_DLG_ABOUT :
            GUICDialogInfo("About", (void **)abouttext, 3, "Ok");
            return 0;
        case COMMAND_DLG_YN :
            result = GUICDialogYesNo("Test Yes/No", (void **)abouttext,
                                     3, "Yes", "No");
            print_dlg_result("Yes/No", result);
            GUITilePaint(IDT_STATUS);
            return 0;
        case COMMAND_DLG_YNC :
            result = GUICDialogYesNoCancel("Test Yes/No/Cancel", (void **)abouttext,
                                           3, "Yes", "No", "Cancel");
            print_dlg_result("Yes/No/Cancel", result);
            GUITilePaint(IDT_STATUS);
            return 0;

        case COMMAND_TEST_OBJ1 :
            dlg_test_obj1();
            print_test_dlg_result();
            return 0;
        case COMMAND_TEST_OBJ2 :
            dlg_test_obj2();
            return 0;
            
        case COMMAND_USE_DB :
            usedoublebuffer = (usedoublebuffer) ? 0 : 1;
            GUIMenuSetTag(3, COMMAND_USE_DB, usedoublebuffer);
            if (usedoublebuffer) {
                GUIMenuSetEnable(3, GUI_MI_OPER, COMMAND_MANAGE_EXP, 1);
            } else {
                manageexposeevents = 0;
                GUIMenuSetTag(3, COMMAND_MANAGE_EXP, 0);
                GUIMenuSetEnable(3, GUI_MI_OPER, COMMAND_MANAGE_EXP, 0);
            }
            return 1;
        case COMMAND_MANAGE_EXP :
            if (usedoublebuffer) {
                manageexposeevents = (manageexposeevents) ? 0 : 1;
                GUIMenuSetTag(3, COMMAND_MANAGE_EXP, manageexposeevents);
                GUIManageExposeEvents(manageexposeevents);
            }
            return !manageexposeevents;
            
        case COMMAND_EXIT :
            set_desired_resolution(-1, 0, 0);
            return 1;
    }
    return 0;
}

/**************************/

static int process_fchange(GrEvent *ev)
{
    char aux[81];
    
    sprintf(aux, "Field changed, p1=%ld, p2=%ld", ev->p1, ev->p2);
    GUITANewLine(textarea1);
    GUITADrawString(textarea1, aux, 0, GR_UTF8_TEXT);
    GUITANewLine(textarea1);
    GUITilePaint(IDT_STATUS);

    return 0;
}

/**************************/

static int process_loadfont(long nfont)
{
    static char* cp437_fonts[7] = {
        "pc6x8.fnt", "pc6x14.fnt", "pc8x8.fnt", "pc8x8t.fnt", "pc8x14.fnt",
        "pc8x14t.fnt", "pc8x16.fnt"};
    static char* cp850_fonts[3] = {
        "pc850-14.fnt", "pc850-16.fnt", "pc850-19.fnt"};
    static char* iso88591_fonts[20] = {
        "char23i.fnt", "char23b.fnt", "char30i.fnt", "char30b.fnt", "lucb21i.fnt",
        "lucb21b.fnt", "lucb27i.fnt", "lucb27b.fnt", "lucs21i.fnt", "lucs21b.fnt",
        "lucs27i.fnt", "lucs27b.fnt", "luct19.fnt", "luct19b.fnt", "luct27.fnt",
        "luct27b.fnt", "ncen22i.fnt", "ncen22b.fnt", "ncen29i.fnt", "ncen29b.fnt"};
    static char* mgrx_fonts[17] = {
        "tmgrx12n.fnt", "tmgrx14b.fnt", "tmgrx14n.fnt", "tmgrx16b.fnt", "tmgrx16n.fnt",
        "tmgrx18b.fnt", "tmgrx18n.fnt", "tmgrx20b.fnt", "tmgrx20n.fnt", "tmgrx22b.fnt",
        "tmgrx22n.fnt", "tmgrx24b.fnt", "tmgrx24n.fnt", "tmgrx28b.fnt", "tmgrx28n.fnt",
        "tmgrx32b.fnt", "tmgrx32n.fnt"};
    int selected;
    char aux[81];
    
    if ((nfont >= COMMAND_LOADFONT_CP437+1) && (nfont <= COMMAND_LOADFONT_CP437+7)) {
        selected = nfont - COMMAND_LOADFONT_CP437 - 1;
        GrUnloadFont(grf_CP437);
        font_name_CP437 = cp437_fonts[selected];
        grf_CP437 = GrLoadFont(font_name_CP437);
        if (grf_CP437 == NULL) {
            sprintf(aux,"%s not found", font_name_CP437);
            disaster(aux);
        }
        texta_font = grf_CP437;
        stext_font = font_name_CP437;
        GUIMenuSetUniqueTag(21, nfont, 2);
        restart_paint(1);
        return 1;
    }
    if ((nfont >= COMMAND_LOADFONT_CP850+1) && (nfont <= COMMAND_LOADFONT_CP850+3)) {
        selected = nfont - COMMAND_LOADFONT_CP850 - 1;
        GrUnloadFont(grf_CP850);
        font_name_CP850 = cp850_fonts[selected];
        grf_CP850 = GrLoadFont(font_name_CP850);
        if (grf_CP850 == NULL) {
            sprintf(aux,"%s not found", font_name_CP850);
            disaster(aux);
        }
        texta_font = grf_CP850;
        stext_font = font_name_CP850;
        GUIMenuSetUniqueTag(22, nfont, 2);
        restart_paint(1);
        return 1;
    }
    if ((nfont >= COMMAND_LOADFONT_ISO59+1) && (nfont <= COMMAND_LOADFONT_ISO59+20)) {
        selected = nfont - COMMAND_LOADFONT_ISO59 - 1;
        GrUnloadFont(grf_ISO88591);
        font_name_ISO88591 = iso88591_fonts[selected];
        grf_ISO88591 = GrLoadFont(font_name_ISO88591);
        if (grf_ISO88591 == NULL) {
            sprintf(aux,"%s not found", font_name_ISO88591);
            disaster(aux);
        }
        texta_font = grf_ISO88591;
        stext_font = font_name_ISO88591;
        GUIMenuSetUniqueTag(24, nfont, 2);
        restart_paint(1);
        return 1;
    }
    if ((nfont >= COMMAND_LOADFONT_MGRX+1) && (nfont <= COMMAND_LOADFONT_MGRX+17)) {
        selected = nfont - COMMAND_LOADFONT_MGRX - 1;
        GrUnloadFont(grf_MGRX);
        font_name_MGRX = mgrx_fonts[selected];
        grf_MGRX = GrLoadFont(font_name_MGRX);
        if (grf_MGRX == NULL) {
            sprintf(aux,"%s not found", font_name_MGRX);
            disaster(aux);
        }
        texta_font = grf_MGRX;
        stext_font = font_name_MGRX;
        GUIMenuSetUniqueTag(25, nfont, 2);
        restart_paint(1);
        return 1;
    }
    return 0;
}

/**************************/

static void restart_paint(int rbut)
{
    GUITASetFont(textarea1, texta_font);
    //GUITAClear(textarea1);

    GUITilePaint(IDT_INFO);
    if (rbut) GUITilePaint(IDT_BUTTONS);
    GUITilePaint(IDT_EDIT);
    GUITilePaint(IDT_STATUS);
    //GUITAShowCursor(textarea1);
}

/**************************/

static void restart_groups(void)
{
    int i, command_usrenc = 0, command_fontenc = 0;

    switch (GrGetUserEncoding()) {
        case GRENC_CP437:
            command_usrenc = COMMAND_UENC_CP437;
            break;
        case GRENC_CP850:
            command_usrenc = COMMAND_UENC_CP850;
            break;
        case GRENC_CP1252:
            command_usrenc = COMMAND_UENC_CP1252;
            break;
        case GRENC_ISO_8859_1:
            command_usrenc = COMMAND_UENC_ISO88591;
            break;
        case GRENC_UTF_8:
            command_usrenc = COMMAND_UENC_UTF8;
            break;
        case GRENC_UCS_2:
            command_usrenc = COMMAND_UENC_UCS2;
            break;
    }
    
    switch (texta_font->h.encoding) {
        case GR_FONTENC_CP437:
            command_fontenc = COMMAND_FONT_CP437;
            break;
        case GR_FONTENC_CP850:
            command_fontenc = COMMAND_FONT_CP850;
            break;
        case GR_FONTENC_CP1252:
            command_fontenc = COMMAND_FONT_CP1252;
            break;
        case GR_FONTENC_ISO_8859_1:
            command_fontenc = COMMAND_FONT_ISO88591;
            break;
        case GR_FONTENC_MGRX512:
            command_fontenc = COMMAND_FONT_MGRX;
            break;
    }
        
    for (i=0; i<grp1->nobj; i++) {
        if (grp1->o[i].type == GUIOBJTYPE_LABEL) continue;
        if (grp1->o[i].type == GUIOBJTYPE_TEXT) continue;
        if (grp1->o[i].type == GUIOBJTYPE_LIGHT) continue;
        if (grp1->o[i].sgid == 101) {
            if (grp1->o[i].cid == command_fontenc)
                grp1->o[i].on = 1;
            else
                grp1->o[i].on = 0;
        }
        if (grp1->o[i].sgid == 102) {
            if (grp1->o[i].cid == command_usrenc)
                grp1->o[i].on = 1;
            else
                grp1->o[i].on = 0;
        }
        if (i == grp1->selected)
            grp1->o[i].selected = 1;
        else
            grp1->o[i].selected = 0;
    }
}

/**************************/

static void save_content_to_file(void)
{
    FILE *fout;
    GUITAStatus tast;
    int i;
    char *s;
    
    fout = fopen("demintl2.out", "w");
    if (fout == NULL) return;
    
    GUITAGetStatus(textarea1, &tast);

    for (i=0; i<tast.nlines; i++) {
        s = GUITAGetString(textarea1, i, GR_UTF8_TEXT);
        if (s == NULL) break;
        fputs(s, fout);
        putc( '\n', fout);
        free(s);
    }
    
    fclose(fout);
}

/**************************/

static void dotest(int n)
{
    GUITAStatus tast;
    int i, count;
    
    GUITAHideCursor(textarea1);
    GUITAGetStatus(textarea1, &tast);
    switch (n) {
        case 1:
            GUITANewLine(textarea1);
            GUITADrawString(textarea1, "Font glyphs:", 0, GR_UTF8_TEXT);
            GUITANewLine(textarea1);
            //GUITASetTextColors(textarea1, EGAC_YELLOW, EGAC_DARKGRAY);
            count = 0;
            for (i=texta_font->h.minchar; i<(texta_font->h.minchar +
                texta_font->h.numchars); i++) {
                //if ((i-texta_font->h.minchar) % tast.ncscr == 0) GUITANewLine(textarea1);
                GUITADrawChar(textarea1, i, GR_BYTE_TEXT);
                count++;
                if (count >= 32) {
                    count = 0;
                    GUITANewLine(textarea1);
                }
            }
            if (count > 0) GUITANewLine(textarea1);
            break;
        case 2:
            GUITANewLine(textarea1);
            GUITADrawString(textarea1, "First 256 user encoding chars:",
                            0, GR_UTF8_TEXT);
            GUITANewLine(textarea1);
            //GUITASetTextColors(textarea1, EGAC_LIGHTGREEN, EGAC_DARKGRAY);
            count = 0;
            for (i=0; i<256; i++) {
                //if (i % tast.ncscr == 0) GUITANewLine(textarea1);
                if (GrGetUserEncoding() == GRENC_UTF_8)
                    GUITADrawChar(textarea1, GrUCS2ToUTF8(i),
                                 GrGetChrtypeForUserEncoding());
                else
                  GUITADrawChar(textarea1, i, GrGetChrtypeForUserEncoding());
                count++;
                if (count >= 32) {
                    count = 0;
                    GUITANewLine(textarea1);
                }
            }
            if (count > 0) GUITANewLine(textarea1);
            break;
        case 3:
            GUITANewLine(textarea1);
            GUITADrawString(textarea1, "Strings with different chrtypes:",
                           0, GR_UTF8_TEXT);
            //GUITASetTextColors(textarea1, EGAC_LIGHTCYAN, EGAC_DARKGRAY);
            draw_string_collection();
            GUITANewLine(textarea1);
            break;
        case 4:
            file_listing();
            break;
    }
    GUITASetTextColors(textarea1, textarea1fgcolor, EGAC_DARKGRAY);
    GUITAShowCursor(textarea1);
}

/**************************/

static void draw_string_collection(void)
{
    unsigned char *scp437 =
      (unsigned char *)"CP437 string (\xa5\xa4 -\xa0\x90\x82-\xa1-\xa2-\xa3 - \xc3\xc4\xb4 \x01\x02)";
    unsigned char *scp850 =
      (unsigned char *)"CP850 string (\xa5\xa4 \xb5\xa0\x90\x82\xd6\xa1\xe0\xa2\xe9\xa3 - \xc3\xc4\xb4)";
    unsigned char *scp1252 =
      (unsigned char *)"CP1252 string (\xd1\xf1 \xc1\xe1\xc9\xe9\xcd\xed\xd3\xf3\xda\xfa \x80 ---)";
    unsigned char *siso88591 =
      (unsigned char *)"ISO-8859-1 string (\xd1\xf1 \xc1\xe1\xc9\xe9\xcd\xed\xd3\xf3\xda\xfa - ---)";
    unsigned char *sutf8 =
      (unsigned char *)"UTF-8 string (Ññ ÁáÉéÍíÓóÚú € ├─┤ ☺☻)";
    unsigned char *sutf8c1 =
      (unsigned char *)"⌠ ⎛ ⎞ ⎡ ⎤ ⎧ ⎫   ⌠ ⎛ ⎞ ⎡ ⎤ ⎧ ⎫";
    unsigned char *sutf8c2 =
      (unsigned char *)"⎮ ⎜ ⎟ ⎢ ⎥ ⎨ ⎬   ⌡ ⎝ ⎠ ⎣ ⎦ ⎩ ⎭";
    unsigned char *sutf8c3 =
      (unsigned char *)"⌡ ⎝ ⎠ ⎣ ⎦ ⎩ ⎭   ←↑→↓↔↕↵⇐⇑⇒⇓⇔⇕";
    
    GUITANewLine(textarea1);
    GUITADrawString(textarea1, scp437, 0, GR_CP437_TEXT);

    GUITANewLine(textarea1);
    GUITADrawString(textarea1, scp850, 0, GR_CP850_TEXT);
    
    GUITANewLine(textarea1);
    GUITADrawString(textarea1, scp1252, 0, GR_CP1252_TEXT);

    GUITANewLine(textarea1);
    GUITADrawString(textarea1, siso88591, 0, GR_ISO_8859_1_TEXT);
    
    GUITANewLine(textarea1);
    GUITADrawString(textarea1, sutf8, 0, GR_UTF8_TEXT);
    GUITANewLine(textarea1);
    GUITADrawString(textarea1, sutf8c1, 0, GR_UTF8_TEXT);
    GUITANewLine(textarea1);
    GUITADrawString(textarea1, sutf8c2, 0, GR_UTF8_TEXT);
    GUITANewLine(textarea1);
    GUITADrawString(textarea1, sutf8c3, 0, GR_UTF8_TEXT);

}

/**************************/

static void file_listing(void)
{
    FILE *f;
    char aux[251];
    char *s;
    int l;
    
    s = GUIGroupGetText(grp1, 17, GR_UTF8_TEXT);
    if (s == NULL) return;

    //f = fopen("grgui.h", "r");
    f = fopen(s, "r");
    if (f == NULL) {
        GUITANewLine(textarea1);
        sprintf(aux, "Error opening %s", s);
        GUITADrawString(textarea1, aux, 0, GR_UTF8_TEXT);
        GUITANewLine(textarea1);
        free(s);
        return;
    }
    
    GUITANewLine(textarea1);
    sprintf(aux, "Listing %s", s);
    GUITADrawString(textarea1, aux, 0, GR_UTF8_TEXT);
    //GUITASetTextColors(textarea1, EGAC_GREEN, EGAC_DARKGRAY);
    GUITANewLine(textarea1);
    
    while (fgets(aux, 250, f) != NULL) {
        l = strlen(aux);
        if (l>0 && aux[l-1]=='\n') aux[l-1] = '\0';
        GUITADrawString(textarea1, aux, 0, GR_UTF8_TEXT);
        GUITANewLine(textarea1);
    }
    
    fclose(f);
    free(s);
}

/**************************/

static void print_dlg_result( char *s, int d)
{
    char aux[81];

    GUITANewLine(textarea1);
    sprintf(aux, "Dialog %s returned %d", s, d);
    GUITADrawString(textarea1, aux, 0, GR_UTF8_TEXT);
    GUITANewLine(textarea1);
}
    
/**************************/

static void print_test_dlg_result(void)
{
    int i;

    GUITANewLine(textarea1);
    GUITADrawString(textarea1, "Test dialog returned:", 0, GR_UTF8_TEXT);
    GUITANewLine(textarea1);
    
    for (i=0; i<4; i++) {
        GUITADrawString(textarea1, buf_dlg_test[i], 0, GR_UTF8_TEXT);
        GUITANewLine(textarea1);
    }
}

/**************************/

void paint_panel1(void *udata)
{
    char aux[101];
    char sys[4] = "?";
    int nsys, x, y;
    int nsysenc, nusrenc;

    GrClearContext(EGAC_LIGHTCYAN);

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

    nsysenc = GrGetKbSysEncoding();
    nusrenc = GrGetUserEncoding();

    x = GrSizeX() / 2;
    y = 6;
    
    sprintf(aux, "INTL_TEST  MGRX Version: %x  System: %s  "
            "VideoDriver: %s  Mode: %dx%d %d bpp", 
            GrGetLibraryVersion(), sys,
            GrCurrentVideoDriver()->name, GrCurrentVideoMode()->width,
            GrCurrentVideoMode()->height, GrCurrentVideoMode()->bpp);
    GrDrawString(aux, 0, x, 4 + y, &grt_info);

    sprintf(aux, "KbSysEncoding: %s  UserEncoding: %s  "
            "TextFont: %s  FontEncoding: %s",
            GrStrEncoding(nsysenc), GrStrEncoding(nusrenc),
            stext_font, GrStrFontEncoding(texta_font->h.encoding));
    GrDrawString(aux, 0, x, 24 + y, &grt_info);
}

/**************************/

void paint_panel2(void *udata)
{
    GrClearContext(EGAC_GREEN);
    restart_groups();
    GUIGroupPaint(grp1);
}

/**************************/

void paint_panel3(void *udata)
{
    GUITAReDraw(textarea1);
}

/**************************/

void paint_panel4(void *udata)
{
    char s[81];
    GUITAStatus tast;
    
    GUITAGetStatus(textarea1, &tast);
    snprintf(s, 80, "Line %d of %d, col %d",tast.tclpos+1,
             tast.nlines, tast.tccpos+1);
    GrClearContext(EGAC_DARKGRAY);
    GrDrawString(s, 0, GrMaxX()-12, 1, &grt_status);
    
    if (tast.full) {
        GrDrawString("FULL", 0, 2, 1, &grt_status2);
    }
}

/**************************/

int processev_panel2(void *udata, GrEvent *ev)
{
    return GUIGroupProcessEvent(grp1, ev);
}

/**************************/

int processev_panel3(void *udata, GrEvent *ev)
{
    int ret;
    ret = GUITAProcessEvent(textarea1, ev);
    if (ret) GUITilePaint(IDT_STATUS);
    return ret;
}

#define DLG_COMMAND_OK           1
#define DLG_COMMAND_SWITCH_L1    2
#define DLG_COMMAND_SWITCH_L2    3
#define DLG_COMMAND_SWITCH_L3    4
#define DLG_COMMAND_SWITCH_L4    5
#define DLG_COMMAND_GET_DATA     6
#define DLG_COMMAND_TA_COLOR     7

/**************************/

int processev_dlg_ta_color(void *udata, GrEvent *ev)
{
    GUIDialog *d = (GUIDialog *)udata;
    GUIGroup *go = (GUIGroup *)(d->exdata);

    if (ev->type == GREV_COMMAND && ev->p1 == DLG_COMMAND_TA_COLOR) {
        textarea1fgcolor = ev->p2;
        GUITASetTextColors(textarea1, textarea1fgcolor, EGAC_DARKGRAY);
        return -1;
    }
    if (ev->type == GREV_COMMAND && ev->p1 == DLG_COMMAND_OK) {
        return -1;
    }

    return GUIGroupProcessEvent(go, ev);
}

/**************************/

static void dlg_ta_color(void)
{
    GUIGroup *go;
    GUIDialog *d;

    go = GUIGroupCreate(13, 0, 0);
    if (go == NULL) disaster("creating group in dlg_ta_color");
    GUIObjectSetButton(&(go->o[0]), EGAC_BLUE,    0,   0, 80, 40, EGAC_DARKGRAY, EGAC_BLUE, "Blue", DLG_COMMAND_TA_COLOR, 0, 0);
    GUIObjectSetButton(&(go->o[1]), EGAC_GREEN,  80,   0, 80, 40, EGAC_DARKGRAY, EGAC_GREEN, "Green", DLG_COMMAND_TA_COLOR, 0, 0);
    GUIObjectSetButton(&(go->o[2]), EGAC_CYAN,  160,   0, 80, 40, EGAC_DARKGRAY, EGAC_CYAN, "Cyan", DLG_COMMAND_TA_COLOR, 0, 0);
    GUIObjectSetButton(&(go->o[3]), EGAC_RED,   240,   0, 80, 40, EGAC_DARKGRAY, EGAC_RED, "Red", DLG_COMMAND_TA_COLOR, 0, 0);

    GUIObjectSetButton(&(go->o[4]), EGAC_LIGHTGRAY,     0,  40, 80, 40, EGAC_DARKGRAY, EGAC_LIGHTGRAY, "LGray", DLG_COMMAND_TA_COLOR, 0, 0);
    GUIObjectSetButton(&(go->o[5]), EGAC_LIGHTBLUE,    80,  40, 80, 40, EGAC_DARKGRAY, EGAC_LIGHTBLUE, "LBlue", DLG_COMMAND_TA_COLOR, 0, 0);
    GUIObjectSetButton(&(go->o[6]), EGAC_LIGHTGREEN,  160,  40, 80, 40, EGAC_DARKGRAY, EGAC_LIGHTGREEN, "LGreen", DLG_COMMAND_TA_COLOR, 0, 0);
    GUIObjectSetButton(&(go->o[7]), EGAC_LIGHTCYAN,   240,  40, 80, 40, EGAC_DARKGRAY, EGAC_LIGHTCYAN, "LCyan", DLG_COMMAND_TA_COLOR, 0, 0);

    GUIObjectSetButton(&(go->o[8]),  EGAC_LIGHTRED,       0,  80, 80, 40, EGAC_DARKGRAY, EGAC_LIGHTRED, "LRed", DLG_COMMAND_TA_COLOR, 0, 0);
    GUIObjectSetButton(&(go->o[9]),  EGAC_LIGHTMAGENTA,  80,  80, 80, 40, EGAC_DARKGRAY, EGAC_LIGHTMAGENTA, "LMagenta", DLG_COMMAND_TA_COLOR, 0, 0);
    GUIObjectSetButton(&(go->o[10]), EGAC_YELLOW,       160,  80, 80, 40, EGAC_DARKGRAY, EGAC_YELLOW, "Yellow", DLG_COMMAND_TA_COLOR, 0, 0);
    GUIObjectSetButton(&(go->o[11]), EGAC_WHITE,        240,  80, 80, 40, EGAC_DARKGRAY, EGAC_WHITE, "White", DLG_COMMAND_TA_COLOR, 0, 0);

    GUIObjectSetButton(&(go->o[12]), 999, 80, 140, 160, 40, EGAC_BROWN, EGAC_WHITE, "Cancel", DLG_COMMAND_OK, 0, 0);
    GUIGroupSetSelected(go, 999, 0);

    d = GUIGroupDialogCreate("Text Area Char color", go, processev_dlg_ta_color);
    if (d == NULL) disaster("creating dialog in dlg_ta_color");

    GUIDialogRun(d);

    GUIDialogDestroy(d);
    GUIGroupDestroy(go);
}
/**************************/

void add_line_to_buf_dlg_test(char *s)
{
    int i;
    
    for (i=0; i<3; i++)
        memcpy(buf_dlg_test[i], buf_dlg_test[i+1], 81);
    
    strncpy(buf_dlg_test[3], s, 80);
    buf_dlg_test[3][80] = '\0';
}

/**************************/

int processev_dlg_test_obj1(void *udata, GrEvent *ev)
{
    GUIDialog *d = (GUIDialog *)udata;
    GUIGroup *go = (GUIGroup *)(d->exdata);
    char aux[81];
    char *s, *sonoff[4];
    char *son = "On";
    char *soff = "Off";
    int i, status;

    if (ev->type == GREV_COMMAND) {
        switch (ev->p1) {
            case DLG_COMMAND_OK :
                return -1;
            case DLG_COMMAND_SWITCH_L1 :
                GUIGroupSetOn(go, 0, 1);
                return 1;
            case DLG_COMMAND_SWITCH_L2 :
                GUIGroupSetOn(go, 1, 1);
                return 1;
            case DLG_COMMAND_SWITCH_L3 :
                GUIGroupSetOn(go, 2, 1);
                return 1;
            case DLG_COMMAND_SWITCH_L4 :
                GUIGroupSetOn(go, 3, 1);
                return 1;
            case DLG_COMMAND_GET_DATA :
                for (i=0; i<4; i++) {
                    status = GUIGroupGetOn(go, i);
                    sonoff[i] = status ? son : soff;
                }
                sprintf(aux, "Lights status 1:%s 2:%s 3:%s 4:%s",
                        sonoff[0], sonoff[1], sonoff[2], sonoff[3]);
                add_line_to_buf_dlg_test(aux);
                s = GUIGroupGetText(go, 5, GR_UTF8_TEXT);
                sprintf(aux, "Entry: %s", s);
                add_line_to_buf_dlg_test(aux);
                free(s);
                GUIGroupRePaintObject(go, 15);
                return 1;
        }
    }
    if (ev->type == GREV_FCHANGE) {
        sprintf(aux, "Field changed, p1=%ld, p2=%ld", ev->p1, ev->p2);
        add_line_to_buf_dlg_test(aux);
        GUIGroupRePaintObject(go, 15);
    }

    return GUIGroupProcessEvent(go, ev);
}

/**************************/

static void dlg_test_obj1(void)
{
    GUIGroup *go;
    GUIDialog *d;

    go = GUIGroupCreate(17, 0, 0);
    if (go == NULL) disaster("creating group in dlg_test_obj1");
    GUIObjectSetLight(&(go->o[0]), 0,   0, 0, 80, 32, EGAC_LIGHTGREEN, EGAC_BLACK, "Light 1", 0);
    GUIObjectSetLight(&(go->o[1]), 1,  80, 0, 80, 32, EGAC_LIGHTGREEN, EGAC_BLACK, "Light 2", 1);
    GUIObjectSetLight(&(go->o[2]), 2, 160, 0, 80, 32, EGAC_LIGHTRED, EGAC_BLACK, "Light 3", 0);
    GUIObjectSetLight(&(go->o[3]), 3, 240, 0, 80, 32, EGAC_LIGHTCYAN, EGAC_BLACK, "Light 4", 1);
    GUIObjectSetLabel(&(go->o[4]), 4,   0, 40, 160, 30, GrNOCOLOR, EGAC_WHITE, "Editable field");
    GUIObjectSetEntry(&(go->o[5]), 5, 160, 40, 160, 30, EGAC_WHITE, EGAC_BLACK, 30, "entry field");
    GUIObjectSetLabel(&(go->o[6]), 6,   0, 80, 160, 30, GrNOCOLOR, EGAC_WHITE, "List field #1");
    GUIObjectSetList(&(go->o[7]),  7, 160, 80, 160, 30, EGAC_WHITE, EGAC_BLACK, (void **)listopt, 5, 3, 1);
    GUIObjectSetLabel(&(go->o[8]), 8,   0, 120, 160, 30, GrNOCOLOR, EGAC_WHITE, "List field #2");
    GUIObjectSetList(&(go->o[9]),  9, 160, 120, 160, 30, EGAC_WHITE, EGAC_BLACK, (void **)listopt, 5, 5, 2);
    GUIObjectSetButton(&(go->o[10]), 10,   0, 160, 40, 40, EGAC_CYAN, EGAC_WHITE, "L1", DLG_COMMAND_SWITCH_L1, 0, 0);
    GUIObjectSetButton(&(go->o[11]), 11,  40, 160, 40, 40, EGAC_CYAN, EGAC_WHITE, "L2", DLG_COMMAND_SWITCH_L2, 0, 0);
    GUIObjectSetButton(&(go->o[12]), 12,  80, 160, 40, 40, EGAC_CYAN, EGAC_WHITE, "L3", DLG_COMMAND_SWITCH_L3, 0, 0);
    GUIObjectSetButton(&(go->o[13]), 13, 120, 160, 40, 40, EGAC_CYAN, EGAC_WHITE, "L4", DLG_COMMAND_SWITCH_L4, 0, 0);
    GUIObjectSetButton(&(go->o[14]), 14, 160, 160, 160, 40, EGAC_CYAN, EGAC_WHITE, "Get data", DLG_COMMAND_GET_DATA, 0, 0);
    GUIObjectSetText(&(go->o[15]), 15, 0, 210, 320, 80, EGAC_LIGHTGRAY, EGAC_BLACK, (void **)buf_dlg_test, 4, GR_ALIGN_LEFT, NULL);
    GUIObjectSetButton(&(go->o[16]), 16, 80, 300, 160, 40, EGAC_GREEN, EGAC_WHITE, "Ok", DLG_COMMAND_OK, 0, 0);
    GUIGroupSetSelected(go, 16, 0);

    d = GUIGroupDialogCreate("Test various objects", go, processev_dlg_test_obj1);
    if (d == NULL) disaster("creating dialog in dlg_test_obj1");

    GUIDialogRun(d);

    GUIDialogDestroy(d);
    GUIGroupDestroy(go);
}

/**************************/

int processev_dlg_test_obj2(void *udata, GrEvent *ev)
{
    GUIDialog *d = (GUIDialog *)udata;
    GUIGroup *go = (GUIGroup *)(d->exdata);

    if (ev->type == GREV_COMMAND && ev->p1 == DLG_COMMAND_OK) {
        return -1;
    }

    return GUIGroupProcessEvent(go, ev);
}

/**************************/

static void dlg_test_obj2(void)
{
    GUIGroup *go;
    GUIDialog *d;

    go = GUIGroupCreate(4, 0, 0);
    if (go == NULL) disaster("creating group in dlg_test_obj2");
    GUIObjectSetText(&(go->o[0]), 0, 0,   0, 480, 80, EGAC_WHITE, EGAC_BLACK, (void **)abouttext, 3, GR_ALIGN_LEFT, NULL);
    GUIObjectSetText(&(go->o[1]), 1, 0,  84, 480, 80, EGAC_WHITE, EGAC_BLACK, (void **)abouttext, 3, GR_ALIGN_CENTER, texta_font);
    GUIObjectSetText(&(go->o[2]), 2, 0, 168, 480, 80, EGAC_WHITE, EGAC_BLACK, (void **)abouttext, 3, GR_ALIGN_RIGHT, NULL);
    GUIObjectSetButton(&(go->o[3]), 3, 160, 260, 160, 40, EGAC_GREEN, EGAC_WHITE, "Ok", DLG_COMMAND_OK, 0, 0);
    GUIGroupSetSelected(go, 3, 0);

    d = GUIGroupDialogCreate("Test text-objects alignment", go, processev_dlg_test_obj2);
    if (d == NULL) disaster("creating dialog in dlg_test_obj2");

    GUIDialogRun(d);

    GUIDialogDestroy(d);
    GUIGroupDestroy(go);
}
