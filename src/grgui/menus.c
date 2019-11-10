/**
 ** menus.c ---- Mini GUI for MGRX, menus
 **
 ** Copyright (C) 2002,2006,2019 Mariano Alvarez Fernandez
 ** [e-mail: malfer at telefonica dot net]
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
#include <ctype.h>
#include "grguip.h"

typedef struct{
    int width;
    int height;
    int linewidth;
    int lineheight;
    int startx;
    int starty;
    int titlestartx;
    int fktitlestartx;
    int marrowstartx;
} GUIMenuDims;

#define MAX_MENUSREG 200

static GUIMenu *menusreg[MAX_MENUSREG];
static int nmenusreg = 0;

int _menuchrtype = GR_BYTE_TEXT;
GrFont *_menufont = NULL;
int _menufontloaded = 0;

GrColor _menubgcolor;
GrColor _menufgcolor;
GrColor _menubgscolor;
GrColor _menufgscolor;
GrColor _menufgnacolor;
GrColor _menufgsnacolor;

static GrTextOption grtopt;

static GUIMenu *search_menu_reg(int idm);
static void calculate_menu_dims(GUIMenu *m, GUIMenuDims *md, int minwidth);
static void show_menu_item(GUIMenu *m, GUIMenuDims *md, int ind, int selected);
static void blt_menu_item(GUIContext *gctx, GUIMenuDims *md, int ind);
static int proccess_menu_event(GUIMenu *m, GUIMenuDims *md,
                               GUIContext *gctx, GrEvent *ev);
static void set_key_short_cut(int idmenu, int level);

void _GUIMenuInit(void)
{
    nmenusreg = 0;

    _menubgcolor = GrWhite();
    _menufgcolor = GrBlack();
    _menubgscolor = GrBlack();
    _menufgscolor = GrWhite();
    _menufgnacolor = GrWhite();
    _menufgsnacolor = GrBlack();

    _menufont = GrGetDefaultFont();
    _menufontloaded = 0;
    _menuchrtype = GrGetChrtypeForUserEncoding();
    
    grtopt.txo_font = _menufont;
    grtopt.txo_fgcolor = _menufgcolor;
    grtopt.txo_bgcolor = GrNOCOLOR;
    grtopt.txo_chrtype = GR_WORD_TEXT;
    grtopt.txo_direct = GR_TEXT_RIGHT;
    grtopt.txo_xalign = GR_ALIGN_LEFT;
    grtopt.txo_yalign = GR_ALIGN_TOP;
}

void _GUIMenuEnd(void)
{
    if (_menufontloaded) GrUnloadFont(_menufont);
    _menufont = NULL;
    _menufontloaded = 0;
}

void _GUIMenuSetKeyShortCut(int idmenu)
{
    set_key_short_cut(idmenu, 0);
}

void GUIMenusSetChrType(int chrtype)
{
    _menuchrtype = chrtype;
}

void GUIMenusSetFont(GrFont *fnt)
{
    if (_menufontloaded) {
        GrUnloadFont(_menufont);
        _menufontloaded = 0;
    }
    _menufont = fnt;
    grtopt.txo_font = _menufont;
}

void GUIMenusSetFontByName(char *fntname)
{
    if (_menufontloaded) {
        GrUnloadFont(_menufont);
        _menufontloaded = 0;
    }
    _menufont = GrLoadFont(fntname);
    if (_menufont == NULL )
        _menufont = &GrFont_PC8x16;
    else
        _menufontloaded = 1; 
    grtopt.txo_font = _menufont;
}

void GUIMenusSetColors(GrColor bg, GrColor bgs, GrColor fg,
                       GrColor fgs, GrColor fgna, GrColor fgsna)
{
    _menubgcolor = bg;
    _menufgcolor = fg;
    _menubgscolor = bgs;
    _menufgscolor = fgs;
    _menufgnacolor = fgna;
    _menufgsnacolor = fgsna;
}

int GUIMenuRegister(GUIMenu *m)
{
    int i, ind;

    ind = -1;
    for (i=0; i<nmenusreg; i++) {
        if (menusreg[i]->idm == m->idm) {
            ind = i;
             break;
         }
    }

    if (ind < 0) {
        if (nmenusreg >= MAX_MENUSREG) return -1;
        menusreg[nmenusreg] = m;
        ind = nmenusreg++;
    }

    return 0;
}

int GUIMenuSetTag(int idmenu, int idop, int tag)
{
    GUIMenu *m;
    int i;

    m = search_menu_reg(idmenu);
    if (m == NULL) return -1;

    for (i=0; i<m->nitems; i++) {
        if ((m->i[i].type == GUI_MI_OPER) && (m->i[i].id == idop)) {
            m->i[i].tag = tag;
            return 0;
        }
    }

    return -1;
}

int GUIMenuSetUniqueTag(int idmenu, int idop, int tag)
{
    GUIMenu *m;
    int i, r = -1;

    m = search_menu_reg(idmenu);
    if (m == NULL) return -1;

    for (i=0; i<m->nitems; i++) {
        if (m->i[i].type == GUI_MI_OPER) {
            if (m->i[i].id == idop) {
                m->i[i].tag = tag;
                r = 0;
            }
            else
                m->i[i].tag = 0;
        }
    }

    return r;
}

int GUIMenuSetEnable(int idmenu, int type, int id, int enable)
{
    GUIMenu *m;
    int i;

    m = search_menu_reg(idmenu);
    if (m == NULL) return -1;

    for (i=0; i<m->nitems; i++) {
        if ((m->i[i].type == type) && (m->i[i].id == id)) {
            m->i[i].enabled = enable;
            return 0;
        }
    }

    return -1;
}

char *GUIMenuGetTitle(int idmenu, int type, int id)
{
    GUIMenu *m;
    int i;

    m = search_menu_reg(idmenu);
    if (m == NULL) return NULL;

    for (i=0; i<m->nitems; i++) {
        if ((m->i[i].type == type) && (m->i[i].id == id)) {
            return m->i[i].title;
        }
    }

    return NULL;
}

int GUIMenuGetTag(int idmenu, int idop)
{
    GUIMenu *m;
    int i;

    m = search_menu_reg(idmenu);
    if (m == NULL) return -1;

    for (i=0; i<m->nitems; i++) {
        if ((m->i[i].type == GUI_MI_OPER) && (m->i[i].id == idop)) {
            return m->i[i].tag;
        }
    }

    return -1;
}

int GUIMenuGetEnable(int idmenu,int type, int id)
{
    GUIMenu *m;
    int i;

    m = search_menu_reg(idmenu);
    if (m == NULL) return -1;

    for (i=0; i<m->nitems; i++) {
        if ((m->i[i].type == type) && (m->i[i].id == id)) {
            return m->i[i].enabled;
        }
    }

    return -1;
}

int GUIMenuRun(int idmenu, int x, int y, int minwidth)
{
    int ret;

    _GUISuspendHooks();
    ret = _GUIMenuRun(idmenu, x, y, minwidth);
    _GUIRestartHooks();

    return ret;
}

int _GUIMenuRun(int idmenu, int x, int y, int minwidth)
/* returns -1=error, 0=continue, 1=operation enqueued, 2=cancel*/
{
    GUIMenu *m;
    GUIMenuDims md;
    GUIContext *gctx;
    GrEvent ev;
    int i, r;
    int res = 0;
    GrContext grcaux;

    m = search_menu_reg(idmenu);
    if (m == NULL) return 0;

    calculate_menu_dims(m, &md, minwidth);
    gctx = GUIContextCreate(x, y, md.width, md.height, 1);
    if (gctx == NULL) return -1;

    GrSaveContext(&grcaux);
    GUIContextSaveUnder(gctx);
    GrSetContext(gctx->c);
    GrClearContext(_menubgcolor);
    GrBox(0, 0, md.width-1, md.height-1, _menufgcolor);

    for (i=0; i<m->nitems; i++)
        show_menu_item(m, &md, i, (i == m->select));
    GUIContextBltToScreen(gctx);

    while (1) {
        GrEventWait(&ev);
        if (ev.type == GREV_PRIVGUI && ev.p1 == GPEV_MENUCANCEL) {
            res = 2;
            break;
        }
        r = proccess_menu_event(m, &md, gctx, &ev);
        if (r < 0) break;
        if (r == 1) {
            if (m->i[m->select].type == GUI_MI_MENU) {
                res = _GUIMenuRun(m->i[m->select].id, x+30,
                                  y+md.lineheight*(m->select+1)+2, md.width);
                GrSetContext(gctx->c);
            }
            else if (m->i[m->select].type == GUI_MI_OPER) {
                ev.type = GREV_COMMAND;
                ev.p1 = m->i[m->select].id;
                ev.p2 = 0;
                ev.p3 = 0;
                GrEventEnqueue(&ev);
                res = 1;
            }
            if (res != 0) break;
        }
    }

    GUIContextRestoreUnder(gctx);
    GrSetContext(&grcaux);
    GUIContextDestroy(gctx);

    return res;
}

/***/

static GUIMenu *search_menu_reg(int idm)
{
    int i;

    for (i=0; i<nmenusreg; i++) {
        if (menusreg[i]->idm == idm) return menusreg[i];
    }

    return NULL;
}

static void calculate_menu_dims(GUIMenu *m, GUIMenuDims *md, int minwidth)
{
    int i, l, lt, lfkt;
    int fillw = 0;

    md->lineheight = _menufont->h.height;
    md->height = md->lineheight * m->nitems + 4;
    lt = 0;
    lfkt = 0;
    for (i=0; i<m->nitems; i++) {
        if (m->i[i].title) {
            l = GrFontStringWidth(_menufont, m->i[i].title, 0, _menuchrtype);
            if (l > lt) lt = l;
        }
        if (m->i[i].fktitle) {
            l = GrFontStringWidth(_menufont, m->i[i].fktitle, 0, _menuchrtype);
            if (l > lfkt) lfkt = l;
        }
    }
    md->linewidth = lt;
    if (lfkt) md->linewidth += 8 + lfkt;
    md->linewidth += 20; //16
    md->width = md->linewidth + 4;
    if (md->width < minwidth) {
        fillw = minwidth - md->width;
        md->width += fillw;
        md->linewidth += fillw;
    }

    md->startx = 2;
    md->starty = 2;
    md->titlestartx = 2 + 12; //+8
    md->fktitlestartx = md->titlestartx + lt + fillw;
    if (lfkt) md->fktitlestartx += 8;
    md->marrowstartx = md->fktitlestartx + lfkt;
}

static void show_menu_item(GUIMenu *m, GUIMenuDims *md, int ind, int selected)
{
    int i, x, y, len, aps, incrx;
    unsigned short *wtext;

    x = md->startx;
    y = md->starty + ind * md->lineheight;
    GrFilledBox(x, y, x+md->linewidth-1, y+md->lineheight-1,
                selected ? _menubgscolor : _menubgcolor);
    if (m->i[ind].type == GUI_MI_SEP) {
        GrHLine(x, x+md->linewidth-1, y+md->lineheight/2, _menufgcolor);
    }
    else {
        if (m->i[ind].enabled ) 
            grtopt.txo_fgcolor = selected ? _menufgscolor : _menufgcolor;
        else
            grtopt.txo_fgcolor = selected ? _menufgsnacolor : _menufgnacolor;
        if (m->i[ind].tag == 1){
            GrLine(x+1, y+md->lineheight/2, x+4, y+md->lineheight-1, grtopt.txo_fgcolor);
            GrLine(x+4, y+md->lineheight-1, x+7, y+2,  grtopt.txo_fgcolor);
            GrLine(x+2, y+md->lineheight/2, x+5, y+md->lineheight-1, grtopt.txo_fgcolor);
            GrLine(x+5, y+md->lineheight-1, x+8, y+2, grtopt.txo_fgcolor);
        }
        if (m->i[ind].tag == 2){
            GrFilledBox(x+2, y+md->lineheight/2-3,
                        x+7, y+md->lineheight/2+3, grtopt.txo_fgcolor);
        }
        len = GrStrLen(m->i[ind].title, _menuchrtype);
        wtext = GrFontTextRecode(grtopt.txo_font, m->i[ind].title, len, _menuchrtype);
        if (wtext == NULL) return;
        aps = -1;
        for (i=0; i<len; i++) {
            if (wtext[i] == '&') aps = i;
        }
        x = md->titlestartx;
        if (aps >= 0){
            if (aps > 0) {
                GrDrawString(wtext, aps, x, y, &grtopt);
                incrx = GrStringWidth(wtext, aps, &grtopt);
            } else {
                incrx = 0;
            }
            grtopt.txo_fgcolor |= GR_UNDERLINE_TEXT;
            GrDrawString(&(wtext[aps+1]), 1, x+incrx, y, &grtopt);
            incrx += GrStringWidth(&(wtext[aps+1]), 1, &grtopt);
            grtopt.txo_fgcolor &= ~GR_UNDERLINE_TEXT;
            GrDrawString(&(wtext[aps+2]), len-aps-2, x+incrx, y, &grtopt);
        }
        else
            GrDrawString(wtext, len, x, y, &grtopt);
        free(wtext);
        
        if (m->i[ind].fktitle) {
            x = md->fktitlestartx;
            len = GrStrLen(m->i[ind].fktitle, _menuchrtype);
            wtext = GrFontTextRecode(grtopt.txo_font, m->i[ind].fktitle, len, _menuchrtype);
            if (wtext != NULL) {
                GrDrawString(wtext, len, x, y, &grtopt);
                free(wtext);
            }
        }
        if (m->i[ind].type == GUI_MI_MENU) {
            x = md->marrowstartx;
            GrDrawChar('>', x, y, &grtopt);
        }
    }
}

static void blt_menu_item(GUIContext *gctx, GUIMenuDims *md, int ind)
{
    int x, y;

    x = md->startx;
    y = md->starty + ind * md->lineheight;
    GUIContextBltRectToScreen(gctx, x, y, x+md->linewidth-1, y+md->lineheight-1);
}

static int proccess_menu_event(GUIMenu *m, GUIMenuDims *md,
                               GUIContext *gctx, GrEvent *ev)
/* returns 1=execute, 0=continue, -1=abort */
{
    GrEvent orgev;
    long c;
    int i, k, pos;
    int change = 0;
    int res = 0;

    orgev = *ev;
    i = m->select;

    switch (ev->type) {

        case GREV_MOUSE :
            c = ev->p1;
            switch (c) {
                case GRMOUSE_LB_PRESSED :
                    if (!GUIContextTransMouseEvIfInCtx(gctx, ev)) {
                        GrEventEnqueueFirst(&orgev);
                        return -1;
                    }
                    pos = ev->p3 / md->lineheight;
                    if ((pos >= 0) && (pos < m->nitems) &&
                        (m->i[pos].type != GUI_MI_SEP)) {
                        if (i != pos) {
                            i = pos;
                            change = 1;
                        }
                    }
                    break;
                case GRMOUSE_LB_RELEASED :
                    if (!GUIContextTransMouseEvIfInCtx(gctx, ev)) return 0;
                    pos = ev->p3 / md->lineheight;
                    if ((pos == i) && (m->i[i].enabled)) return 1;
                    break;
                case GRMOUSE_RB_PRESSED :
                    if (!GUIContextTransMouseEvIfInCtx(gctx, ev)) {
                        GrEventEnqueueFirst(&orgev);
                        return -1;
                    }
                    break;
                case GRMOUSE_B4_RELEASED :
                    GrEventParEnqueue(GREV_KEY, GrKey_Up, GRKEY_KEYCODE, 0, 0);
                    break;
                case GRMOUSE_B5_RELEASED :
                    GrEventParEnqueue(GREV_KEY, GrKey_Down, GRKEY_KEYCODE, 0, 0);
                    break;
            }
            break;

        case GREV_MMOVE :
            if (ev->p1 & GRMOUSE_LB_STATUS) {
                if (!GUIContextTransMouseEvIfInCtx(gctx, ev)) return 0;
                pos = ev->p3 / md->lineheight;
                if ((pos >= 0) && (pos < m->nitems) &&
                    (m->i[pos].type != GUI_MI_SEP)) {
                    if (i != pos) {
                        i = pos;
                        change = 1;
                    }
                }
            }
            break;

        case GREV_KEY :
            c = ev->p1;
            switch (c) {
                case GrKey_Up :
                    if (i > 0) {
                        if (m->i[i-1].type != GUI_MI_SEP) {
                            i--;
                            change = 1;
                        }
                        else if ((i > 1) && (m->i[i-2].type != GUI_MI_SEP)) {
                            i -= 2;
                            change = 1;
                        }
                    }
                    break;
                case GrKey_Down :
                    if (i < m->nitems-1) {
                        if (m->i[i+1].type != GUI_MI_SEP) {
                            i++;
                            change = 1;
                        }
                        else if ((i < m->nitems-2) &&
                                 (m->i[i+2].type != GUI_MI_SEP)){
                            i += 2;
                            change = 1;
                        }
                    }
                    break;
                case GrKey_Escape :
                    return -1;
                case GrKey_Return :
                    if (m->i[i].enabled) return 1;
                    break;
                default :
                    if ((c > ' ') && (c < 256)) {
                        c = toupper(c);
                        for (k=0; k<m->nitems; k++) {
                            if (c == m->i[k].key) {
                                if (k != i) {
                                    i = k;
                                    change = 1;
                                }
                                if (m->i[i].enabled) res = 1;
                                break;
                            }
                        }
                    }
                    break;
            }
            break;
    }

    if (change) {
        show_menu_item(m, md, m->select, 0);
        blt_menu_item(gctx, md, m->select);
        m->select = i;
        show_menu_item(m, md, m->select, 1);
        blt_menu_item(gctx, md, m->select);
    }

    return res;
}

static void set_key_short_cut(int idmenu, int level)
{
    GUIMenu *m;
    int i;

    if (level > 10) return;  // protection for very anidated menues

    m = search_menu_reg(idmenu);
    if (m == NULL) return;

    for (i=0; i<m->nitems; i++) {
        if ((m->i[i].type == GUI_MI_OPER) &&
            (m->i[i].fastkey != 0) &&
            (m->i[i].enabled)) {
            _GUIKeyShortCutAdd(m->i[i].fastkey, GREV_COMMAND, m->i[i].id, 0, 0);
        }
        if ((m->i[i].type == GUI_MI_MENU) && (m->i[i].enabled)) {
            set_key_short_cut(m->i[i].id, level+1);
        }
    }
}


