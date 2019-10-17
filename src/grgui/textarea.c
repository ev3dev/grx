/**
 ** textarea.c ---- Mini GUI for MGRX, textarea (mini editor)
 **
 ** Copyright (C) 2017,2019 Mariano Alvarez Fernandez
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
#include <stdio.h>
#include <string.h>
#include "grguip.h"

#define INITIAL_LINE_SIZE 10
#define INCR_LINE_SIZE 50

#define BLINK_TIME 500

typedef struct {
    unsigned short c;      // glyph
    unsigned short u;      // UCS2 char
    GrColor fg, bg;        // colors
    int x;                 // x-pos
    int w;                 // width
} TAChar;

typedef struct {
    TAChar *cl;            // char array
    int maxlen;            // max chars (auto extended if necesary)
    int len;               // actual number of chars
    long y;                // y-pos
    int nextx;             // next char x-pos
} TALine;

typedef struct _guiTextArea {
    GUIPanel *p;           // panel container
    GrColor bgcolor;       // bg color
    GrTextOption grt;      // actual GrTextOption
    TALine *l;             // line array
    int maxlines;          // max lines
    int nlines;            // actual number of lines
    int flscr;             // first line on screen
    int nlscr;             // lines on screen
    int xorg;              // x origen value
    int xbits, ybits;      // max x-bits, y-bits per char
    int tcstatus;          // text cursor estatus (showed if >= 0)
    GrColor tccolor;       // text cursor color
    int tclpos;            // text cursor line pos
    int tccpos;            // text cursor char pos
    int tcblink;           // text cursor blinking
    long tctime;           // text cursor blink time
    int full;              // full, no more chars or lines can be added
} GUITextArea;

static void show_tcursor(GUITextArea *ta);
static void hide_tcursor(GUITextArea *ta);
static void draw_tcursor(GUITextArea *ta, int show);
static void blink_tcursor(GUITextArea *ta);
static void redraw(GUITextArea *ta);
//static void redraw_line(GUITextArea *ta, int line);
static void redraw_line_from(GUITextArea *ta, int line, int c);
static void redraw_char(GUITextArea *ta, int line, int c);
static void scroll_up(GUITextArea *ta);
static void scroll_down(GUITextArea *ta);
static void scroll_toshow_tcline(GUITextArea *ta);
static void scroll_nlines_up(GUITextArea *ta, int nlines);
static void scroll_nlines_down(GUITextArea *ta, int nlines);
static void junta_linea(GUITextArea *ta);
static void salta_linea(GUITextArea *ta);
static void add_char(GUITextArea *ta, long ch, int chrtype);
static void delete_char(GUITextArea *ta);
static void erase_char(GUITextArea *ta);
static int calc_line_col_ckick(GUITextArea *ta, int x, int y, int *l, int *c);
static int calc_maxx_in_screen(GUITextArea *ta);
static void redraw_scbs(GUITextArea *ta);
static void select_char_rel(GUITextArea *ta, int incrc);

/**************************************************************************/

GUITextArea * GUITACreate(GUIPanel *p, GrFont *f, int maxlines)
{
    GUITextArea *ta;
    int i;
    
    if (maxlines < 1) return NULL;
    ta = malloc(sizeof(GUITextArea));
    if (ta == NULL) return NULL;
    ta->l = malloc(sizeof(TALine)*maxlines);
    if (ta->l == NULL) {
        free(ta);
        return NULL;
    }
    ta->l[0].cl = malloc(sizeof(TAChar)*INITIAL_LINE_SIZE);
    if (ta->l[0].cl == NULL) {
        free(ta->l);
        free(ta);
        return NULL;
    }
    ta->p = p;
    ta->bgcolor = GrBlack();
    ta->grt.txo_font = (f) ? f : GrGetDefaultFont();
    ta->grt.txo_fgcolor = GrWhite();
    ta->grt.txo_bgcolor = GrBlack();
    ta->grt.txo_direct = GR_TEXT_RIGHT;
    ta->grt.txo_xalign = GR_ALIGN_LEFT;
    ta->grt.txo_yalign = GR_ALIGN_BOTTOM;
    ta->grt.txo_chrtype = GR_WORD_TEXT;
    ta->maxlines = maxlines;
    ta->nlines = 1;
    ta->flscr = 0;
    //ta->xbits = GrCharWidth('M', &(ta->grt));
    //ta->ybits = GrCharHeight('M', &(ta->grt));
    ta->xbits = ta->grt.txo_font->maxwidth;
    ta->ybits = ta->grt.txo_font->h.height;
    //ta->nlscr = (ta->p->cl->gc_ymax+1) / ta->ybits;
    ta->nlscr = (ta->p->cl->gc_ymax) / ta->ybits;
    ta->xorg = 0;
    ta->tcstatus = -1;
    ta->tccolor = GrWhite();
    ta->tclpos = 0;
    ta->tccpos = 0;
    ta->tcblink = 1;
    ta->tctime = 0;
    ta->full = 0;
    ta->l[0].len = 0;
    ta->l[0].maxlen = INITIAL_LINE_SIZE;
    ta->l[0].y = ta->ybits;
    ta->l[0].nextx = 0;
    for (i=1; i<ta->maxlines; i++) {
        ta->l[i].cl = NULL;
        ta->l[i].len = 0;
        ta->l[i].maxlen = 0;
        ta->l[i].y = ta->ybits * (i + 1);
        ta->l[i].nextx = 0;
    }
    
    return ta;
}

/**************************************************************************/

void GUITASetFont(GUITextArea *ta, GrFont *f)
{
    int i, j;
    long ybits;

    if (ta && f) {
        ta->grt.txo_font = f;
        //ta->xbits = GrCharWidth('M', &(ta->grt));
        //ta->ybits = GrCharHeight('M', &(ta->grt));
        ta->xbits = ta->grt.txo_font->maxwidth;
        ta->ybits = ta->grt.txo_font->h.height;
        //ta->nlscr = (ta->p->cl->gc_ymax+1) / ta->ybits;
        ta->nlscr = (ta->p->cl->gc_ymax) / ta->ybits;
        //GUITAClear(ta);

        ta->l[ta->flscr].y = ta->ybits;
        ybits = 0;
        for (i=ta->flscr-1; i>=0; i--) {
            ta->l[i].y = ybits;
            ybits -= ta->ybits;
        }
        ybits = ta->ybits * 2;
        for (i=ta->flscr+1; i<ta->maxlines; i++) {
            ta->l[i].y = ybits;
            ybits += ta->ybits;
        }
        for (i=0; i<ta->nlines; i++) {
            ta->l[i].nextx = 0;
            for (j=0; j<ta->l[i].len; j++) {
                ta->l[i].cl[j].x = ta->l[i].nextx;
                ta->l[i].cl[j].c =
                    GrFontCharRecode(ta->grt.txo_font, ta->l[i].cl[j].u,
                                     GR_UCS2_TEXT);
                ta->l[i].cl[j].w =
                    GrFontCharWidth(ta->grt.txo_font, ta->l[i].cl[j].c);
                ta->l[i].nextx += ta->l[i].cl[j].w;
            }
        }
    }
}

/**************************************************************************/

void GUITASetBgColor(GUITextArea *ta, GrColor c)
{
    if (ta == NULL) return;

    ta->bgcolor = c;
}

/**************************************************************************/

void GUITASetTextColors(GUITextArea *ta, GrColor fg, GrColor bg)
{
    if (ta == NULL) return;

    ta->grt.txo_fgcolor = fg;
    ta->grt.txo_bgcolor = bg;
}

/**************************************************************************/

void GUITADestroy(GUITextArea *ta)
{
    int i;
    
    if (ta == NULL) return;

    for (i=0; i<ta->maxlines; i++) {
        free(ta->l[i].cl);
    }
    free(ta->l);
    free(ta);
}

/**************************************************************************/

void GUITAGetStatus(GUITextArea *ta, GUITAStatus *tast)
{
    if (ta == NULL) return;
    
    tast->nlines = ta->nlines;
    tast->tclpos = ta->tclpos;
    tast->tccpos = ta->tccpos;
    tast->ncscr = (ta->p->cl->gc_xmax+1) / ta->xbits;
    tast->full = ta->full;
}

/**************************************************************************/

void *GUITAGetString(GUITextArea *ta, int nline, int chrtype)
{
    unsigned char *s;
    unsigned short *h;
    long laux;
    unsigned char *cutf8;
    int i, j, len;
    
    if (ta == NULL) return NULL;
    if (nline < 0 || nline >= ta->nlines) return NULL;
    
    len = ta->l[nline].len;

    switch (chrtype) {
    case GR_BYTE_TEXT:
    case GR_CP437_TEXT:
    case GR_CP850_TEXT:
    case GR_CP1252_TEXT:
    case GR_ISO_8859_1_TEXT:
        s = malloc(len+1);
        if (s == NULL) return NULL;
        for (i=0; i<len; i++)
            //s[i] = GrFontCharReverseRecode(ta->grt.txo_font,
            //                               ta->l[nline].cl[i].c, chrtype);
            s[i] = GrCharRecodeFromUCS2(ta->l[nline].cl[i].u, chrtype);
        s[len] = '\0';
        return s;
    case GR_UTF8_TEXT:
        s = malloc((len*4)+1);
        if (s == NULL) return NULL;
        j = 0;
        cutf8 = (unsigned char *)&laux;
        for (i=0; i<len; i++) {
            //l = GrFontCharReverseRecode(ta->grt.txo_font,
            //                            ta->l[nline].cl[i].c, chrtype);
            laux = GrCharRecodeFromUCS2(ta->l[nline].cl[i].u, chrtype);
            s[j++] = cutf8[0];
            if (cutf8[1] != '\0') s[j++] = cutf8[1];
            if (cutf8[2] != '\0') s[j++] = cutf8[2];
            if (cutf8[3] != '\0') s[j++] = cutf8[3];
        }
        s[j] = '\0';
        return s;
    case GR_WORD_TEXT:
    case GR_UCS2_TEXT:
        h = malloc((len+1)*sizeof(unsigned short));
        if (h == NULL) return NULL;
        for (i=0; i<len; i++)
            //h[i] = GrFontCharReverseRecode(ta->grt.txo_font,
            //                               ta->l[nline].cl[i].c, chrtype);
            h[i] = ta->l[nline].cl[i].u;
        h[len] = '\0';
        return h;
    }

    return NULL;
}

/**************************************************************************/

void GUITAShowCursor(GUITextArea *ta)
{
    GrContext ctxsave;

    if (ta == NULL) return;

    ta->tcstatus++;
        
    if (ta->tcstatus == 0) {
        GrSaveContext(&ctxsave);
        GrSetContext(ta->p->cl);
        draw_tcursor(ta, 1);
        GrSetContext(&ctxsave);
    }
}

/**************************************************************************/

void GUITAHideCursor(GUITextArea *ta)
{
    GrContext ctxsave;

    if (ta == NULL) return;

    ta->tcstatus--;
        
    if (ta->tcstatus == -1) {
        GrSaveContext(&ctxsave);
        GrSetContext(ta->p->cl);
        draw_tcursor(ta, 0);
        GrSetContext(&ctxsave);
    }
}

/**************************************************************************/

void GUITAMoveCursor(GUITextArea *ta, int nline, int nchar)
{
    GrContext ctxsave;

    if (ta == NULL) return;

    if (nline < 0) nline = 0;
    if (nline >= ta->nlines) nline = ta->nlines - 1;
    
    if (nchar < 0) nchar = 0;
    if (nchar > ta->l[nline].len) nchar = ta->l[nline].len;

    GrSaveContext(&ctxsave);
    GrSetContext(ta->p->cl);
    hide_tcursor(ta);
    ta->tclpos = nline;
    ta->tccpos = nchar;
    show_tcursor(ta);
    GrSetContext(&ctxsave);
}

/**************************************************************************/

void GUITAMoveCursorRel(GUITextArea *ta, int incrl, int incrc)
{
    GrContext ctxsave;
    int nline, nchar;

    if (ta == NULL) return;

    nline = ta->tclpos + incrl;
    if (nline < 0) nline = 0;
    if (nline >= ta->nlines) nline = ta->nlines - 1;
    
    nchar = ta->tccpos + incrc;
    if (nchar < 0) nchar = 0;
    if (nchar > ta->l[nline].len) nchar = ta->l[nline].len;

    GrSaveContext(&ctxsave);
    GrSetContext(ta->p->cl);
    hide_tcursor(ta);
    ta->tclpos = nline;
    ta->tccpos = nchar;
    show_tcursor(ta);
    GrSetContext(&ctxsave);
}

/**************************************************************************/

void GUITASetCursorColor(GUITextArea *ta, GrColor c)
{
    if (ta == NULL) return;

    ta->tccolor = c;
}

/**************************************************************************/

void GUITAClear(GUITextArea *ta)
{
    //GrContext ctxsave;
    int i;
    
    if (ta == NULL) return;

    ta->flscr = 0;
    ta->nlines = 1;
    for (i=0; i<ta->maxlines; i++) {
        ta->l[i].len = 0;
        ta->l[i].y = ta->ybits * (i + 1);
        ta->l[i].nextx = 0;
    }
    //GrSaveContext(&ctxsave);
    //GrSetContext(ta->p->cl);
    //GrClearContext(ta->bgcolor);
    //GrSetContext(&ctxsave);
    ta->xorg = 0;
    ta->tcstatus = -1;
    ta->tclpos = 0;
    ta->tccpos = 0;
    ta->tcblink = 1;
    ta->tctime = 0;
    ta->full = 0;
    
    //redraw_scbs(ta);
}

/**************************************************************************/

void GUITAReDraw(GUITextArea *ta)
{
    GrContext ctxsave;

    if (ta == NULL) return;

    GrSaveContext(&ctxsave);
    GrSetContext(ta->p->cl);
    hide_tcursor(ta);
    GrClearContext(ta->bgcolor);
    redraw(ta);
    show_tcursor(ta);
    GrSetContext(&ctxsave);
}

/**************************************************************************/

void GUITANewLine(GUITextArea *ta)
{
    GrContext ctxsave;

    if (ta == NULL) return;
    if (ta->full) return;

    GrSaveContext(&ctxsave);
    GrSetContext(ta->p->cl);
    hide_tcursor(ta);
    salta_linea(ta);
    show_tcursor(ta);
    GrSetContext(&ctxsave);
}

/**************************************************************************/

void GUITADrawChar(GUITextArea *ta, long ch, int chrtype)
{
    GrContext ctxsave;

    if (ta == NULL) return;
    if (ta->full) return;

    GrSaveContext(&ctxsave);
    GrSetContext(ta->p->cl);
    hide_tcursor(ta);
    add_char(ta, ch, chrtype);
    show_tcursor(ta);
    GrSetContext(&ctxsave);
}

/**************************************************************************/

void GUITADrawString(GUITextArea *ta, void *s, int len, int chrtype)
{
    GrContext ctxsave;
    unsigned short *text2;
    int i, newchrtype;

    if (ta == NULL) return;
    if (ta->full) return;

    if (len <= 0) len = GrStrLen(s, chrtype);
    //text2 = GrFontTextRecode(ta->grt.txo_font, s, len, chrtype);
    text2 = GrTextRecodeToUCS2(s, len, chrtype);
    if (text2 == NULL) return;

    GrSaveContext(&ctxsave);
    GrSetContext(ta->p->cl);
    hide_tcursor(ta);

    newchrtype = GR_UCS2_TEXT;
    if (chrtype == GR_BYTE_TEXT || chrtype == GR_WORD_TEXT)
        newchrtype = GR_WORD_TEXT; // so we can draw glyphs if necesary
    
    for (i=0; i<len; i++) {
        add_char(ta, text2[i], newchrtype);
        if (ta->full) break;
    }

    show_tcursor(ta);
    GrSetContext(&ctxsave);
    free(text2);
}

/**************************************************************************/

void GUITAPrintChar(GUITextArea *ta, long ch, int chrtype)
{
    GrContext ctxsave;

    if (ta == NULL) return;
    if (ta->full) return;

    GrSaveContext(&ctxsave);
    GrSetContext(ta->p->cl);
    hide_tcursor(ta);
    
    if (ch == '\r') {
        salta_linea(ta);
    } else if (ch == '\b') {
        delete_char(ta);
    } else if (ch == 0x7f) {
        erase_char(ta);
    } else {
        add_char(ta, ch, chrtype);
    }
    
    show_tcursor(ta);
    GrSetContext(&ctxsave);
}

/**************************************************************************/

int GUITAProcessEvent(GUITextArea *ta, GrEvent *ev)
{
    int l, c;

    if (ev->type == GREV_MOUSE) {
        if (ev->p1 == GRMOUSE_LB_PRESSED) {
            if (calc_line_col_ckick(ta, ev->p2, ev->p3, &l, &c)){
              GUITAMoveCursor(ta, l, c);
              scroll_toshow_tcline(ta);
            }
            return 1;
        } else if (ev->p1 == GRMOUSE_B4_RELEASED) {
            GUITAMoveCursorRel(ta, -3, 0);
            scroll_nlines_up(ta, 3);
            //scroll_toshow_tcline(ta);
            return 1;
        } else if (ev->p1 == GRMOUSE_B5_RELEASED) {
            GUITAMoveCursorRel(ta, 3, 0);
            scroll_nlines_down(ta, 3);
            //scroll_toshow_tcline(ta);
            return 1;
        }
    } else if ((ev->type == GREV_KEY) && (ev->p2 != GRKEY_KEYCODE)) {
        GUITAPrintChar(ta, ev->p1, GrGetChrtypeForUserEncoding());
        return 1;
    } else if ((ev->type == GREV_KEY) && (ev->p2 == GRKEY_KEYCODE)) {
        if (ev->p1 == GrKey_Delete) {
            GUITAPrintChar(ta, 0x7f, GrGetChrtypeForUserEncoding());
        } else if (ev->p1 == GrKey_Left) {
            GUITAMoveCursorRel(ta, 0, -1);
            return 1;
        } else if (ev->p1 == GrKey_Right) {
            GUITAMoveCursorRel(ta, 0, 1);
            return 1;
        } else if (ev->p1 == GrKey_Up) {
            GUITAMoveCursorRel(ta, -1, 0);
            scroll_toshow_tcline(ta);
            return 1;
        } else if (ev->p1 == GrKey_Down) {
            GUITAMoveCursorRel(ta, 1, 0);
            scroll_toshow_tcline(ta);
            return 1;
        } else if (ev->p1 == GrKey_Home) {
            GUITAMoveCursorRel(ta, 0, -ta->tccpos);
            scroll_toshow_tcline(ta);
            return 1;
        } else if (ev->p1 == GrKey_End) {
            GUITAMoveCursorRel(ta, 0, ta->l[ta->tclpos].len - ta->tccpos);
            scroll_toshow_tcline(ta);
            return 1;
        } else if (ev->p1 == GrKey_Control_Home) {
            GUITAMoveCursorRel(ta, -ta->tclpos, -ta->tccpos);
            scroll_toshow_tcline(ta);
            return 1;
        } else if (ev->p1 == GrKey_Control_End) {
            GUITAMoveCursorRel(ta, ta->nlines - ta->tclpos - 1,
                              ta->l[ta->nlines-1].len - ta->tccpos);
            scroll_toshow_tcline(ta);
            return 1;
        } else if (ev->p1 == GrKey_PageUp) {
            GUITAMoveCursorRel(ta, -(ta->nlscr-1), 0);
            scroll_nlines_up(ta, ta->nlscr-1);
            //scroll_toshow_tcline(ta);
            return 1;
        } else if (ev->p1 == GrKey_PageDown) {
            GUITAMoveCursorRel(ta, ta->nlscr-1, 0);
            scroll_nlines_down(ta, ta->nlscr-1);
            //scroll_toshow_tcline(ta);
            return 1;
        } else if (ev->p1 == GrKey_Shift_Left) {
            select_char_rel(ta, -1);
            return 1;
        } else if (ev->p1 == GrKey_Shift_Right) {
            select_char_rel(ta, 1);
            return 1;
        }
    } else if ((ev->type == GREV_NULL) && (ta->tcstatus >= 0)) {
        if (ta->tctime == 0) {
            ta->tctime = ev->time;
        } else {
            if (ev->time > ta->tctime + BLINK_TIME) {
              ta->tctime = ev->time;
              ta->tcblink ^= 1;
              blink_tcursor(ta);
            }
        }
    }
    
    return 0;
}

/**************************************************************************/

static void show_tcursor(GUITextArea *ta)
/* internal version, assume ta != NULL and context setted */
{
    ta->tcstatus++;
        
    if (ta->tcstatus == 0) {
        draw_tcursor(ta, 1);
    }
}

/**************************************************************************/

static void hide_tcursor(GUITextArea *ta)
/* internal version, assume ta != NULL and context setted */
{
    ta->tcstatus--;
        
    if (ta->tcstatus == -1) {
        draw_tcursor(ta, 0);
    }
}

/**************************************************************************/

static void draw_tcursor(GUITextArea *ta, int show)
{
    TALine *l;
    int xpos_org, xpos, ypos;
    int need_redraw = 0;

    l = &(ta->l[ta->tclpos]);
    ypos = l->y;
    if (ypos > GrMaxY()) return;
    xpos_org = (ta->tccpos >= l->len) ? l->nextx : l->cl[ta->tccpos].x;
    xpos = xpos_org - ta->xorg;
    
    if (show) {
        while (xpos < 0) {
            ta->xorg -= ta->xbits;
            need_redraw = 1;
            xpos = xpos_org - ta->xorg;
        }
        while (xpos > GrMaxX()) {
            ta->xorg += ta->xbits;
            need_redraw = 1;
            xpos = xpos_org - ta->xorg;
        }
        if (need_redraw) redraw(ta);
        GrVLine(xpos, ypos, ypos-ta->ybits+1, ta->tccolor);
        GrVLine(xpos+1, ypos, ypos-ta->ybits+1, ta->tccolor);
        GUIPanelBltRectClToScreen(ta->p, xpos, ypos-ta->ybits+1, 2, ta->ybits);
        ta->tctime = 0; // restet blik
    } else if (ta->tccpos >= l->len) {
        GrVLine(xpos, ypos, ypos-ta->ybits+1, ta->bgcolor);
        GrVLine(xpos+1, ypos, ypos-ta->ybits+1, ta->bgcolor);
        GUIPanelBltRectClToScreen(ta->p, xpos, ypos-ta->ybits+1, 2, ta->ybits);
    } else {
        redraw_char(ta, ta->tclpos, ta->tccpos);
    }
}

/**************************************************************************/

static void blink_tcursor(GUITextArea *ta)
{
    // routine used "only" by the blink code when tc is showed
    TALine *l;
    int xpos_org, xpos, ypos;

    l = &(ta->l[ta->tclpos]);
    ypos = l->y;
    if (ypos > GrMaxY()) return;
    xpos_org = (ta->tccpos >= l->len) ? l->nextx : l->cl[ta->tccpos].x;
    xpos = xpos_org - ta->xorg;
    
    if (ta->tcblink) {
        GrVLine(xpos, ypos, ypos-ta->ybits+1, ta->tccolor);
        GrVLine(xpos+1, ypos, ypos-ta->ybits+1, ta->tccolor);
        GUIPanelBltRectClToScreen(ta->p, xpos, ypos-ta->ybits+1, 2, ta->ybits);
    } else if (ta->tccpos >= l->len) {
        GrVLine(xpos, ypos, ypos-ta->ybits+1, ta->bgcolor);
        GrVLine(xpos+1, ypos, ypos-ta->ybits+1, ta->bgcolor);
        GUIPanelBltRectClToScreen(ta->p, xpos, ypos-ta->ybits+1, 2, ta->ybits);
    } else {
        //GrVLine(xpos, ypos, ypos-ta->ybits+1, ta->bgcolor);
        //GrVLine(xpos+1, ypos, ypos-ta->ybits+1, ta->bgcolor);
        //GUIPanelBltRectClToScreen(ta->p, xpos, ypos-ta->ybits+1, 2, ta->ybits);
        redraw_char(ta, ta->tclpos, ta->tccpos);
    }
}

/**************************************************************************/

static void redraw(GUITextArea *ta)
{
    GrTextOption grt2;
    int i, j, cl;
    
    cl = 0;
    grt2 = ta->grt;
    for (i=ta->flscr; i<ta->nlines; i++) {
        GrFilledBox(0, ta->l[i].y-ta->ybits+1, GrMaxX(), ta->l[i].y, ta->bgcolor);
        for (j=0; j<ta->l[i].len; j++) {
            grt2.txo_fgcolor = ta->l[i].cl[j].fg;
            grt2.txo_bgcolor = ta->l[i].cl[j].bg;
            GrDrawChar(ta->l[i].cl[j].c, ta->l[i].cl[j].x-ta->xorg,
                       ta->l[i].y, &(grt2));
        }
        cl++;
        if (ta->l[i].y > GrMaxY()) break;
    }
    
    i = ta->nlscr + 1 - cl;
    if (i > 0)
        GrFilledBox(0, (cl*ta->ybits)+1, GrMaxX(), (cl+i)*ta->ybits, ta->bgcolor);
    
    GUIPanelBltRectClToScreen(ta->p, 0, 0, GrSizeX(), GrSizeY());
    redraw_scbs(ta);
}

/**************************************************************************/
/*
static void redraw_line(GUITextArea *ta, int line)
{
    GrTextOption grt2;
    int j;
    
    if (ta->l[line].y > GrMaxY()) return;
    GrFilledBox(0, ta->l[line].y-ta->ybits+1, GrMaxX(), ta->l[line].y, ta->bgcolor);
    grt2 = ta->grt;
    for (j=0; j<ta->l[line].len; j++) {
        grt2.txo_fgcolor = ta->l[line].cl[j].fg;
        grt2.txo_bgcolor = ta->l[line].cl[j].bg;
        GrDrawChar(ta->l[line].cl[j].c, ta->l[line].cl[j].x-ta->xorg,
                   ta->l[line].y, &(grt2));
    }
    GUIPanelBltRectClToScreen(ta->p, 0, ta->l[line].y-ta->ybits+1,
                              GrSizeX(), ta->ybits);
}
*/
/**************************************************************************/

static void redraw_line_from(GUITextArea *ta, int line, int c)
{
    GrTextOption grt2;
    int j, x1;
    
    if (ta->l[line].y > GrMaxY()) return;
    grt2 = ta->grt;
    for (j=c; j<ta->l[line].len; j++) {
        grt2.txo_fgcolor = ta->l[line].cl[j].fg;
        grt2.txo_bgcolor = ta->l[line].cl[j].bg;
        GrDrawChar(ta->l[line].cl[j].c, ta->l[line].cl[j].x-ta->xorg,
                   ta->l[line].y, &(grt2));
    }
    x1 = ta->l[line].nextx - ta->xorg;
    if (x1 < 0) x1 = 0;
    if (x1 < GrSizeX())
        GrFilledBox(x1, ta->l[line].y-ta->ybits+1, GrMaxX(), ta->l[line].y,
                    ta->bgcolor);
    x1 = (c >= ta->l[line].len) ? ta->l[line].nextx - ta->xorg : 
                                  ta->l[line].cl[c].x - ta->xorg;
    if (x1 < 0) x1 = 0;
    if (x1 < GrSizeX())
        GUIPanelBltRectClToScreen(ta->p, x1, ta->l[line].y-ta->ybits+1,
                                  GrSizeX(), ta->ybits);
}

/**************************************************************************/

static void redraw_char(GUITextArea *ta, int line, int c)
{
    GrTextOption grt2;
    
    if (ta->l[line].y > GrMaxY()) return;
    if (c >= ta->l[line].len) return;
    grt2 = ta->grt;
    grt2.txo_fgcolor = ta->l[line].cl[c].fg;
    grt2.txo_bgcolor = ta->l[line].cl[c].bg;
    GrDrawChar(ta->l[line].cl[c].c, ta->l[line].cl[c].x-ta->xorg,
               ta->l[line].y, &(grt2));

    GUIPanelBltRectClToScreen(ta->p, ta->l[line].cl[c].x-ta->xorg,
                              ta->l[line].y-ta->ybits+1, ta->l[line].cl[c].w,
                              ta->ybits);
}

/**************************************************************************/

static void scroll_up(GUITextArea *ta)
{
    int i;
    
    ta->flscr += 1;
    for (i=0; i<ta->nlines; i++) {
        ta->l[i].y -= ta->ybits;
    }
}

/**************************************************************************/

static void scroll_down(GUITextArea *ta)
{
    int i;
    
    if (ta->flscr == 0) return;
    ta->flscr -= 1;
    for (i=0; i<ta->nlines; i++) {
        ta->l[i].y += ta->ybits;
    }
}

/**************************************************************************/

static void scroll_toshow_tcline(GUITextArea *ta)
{
    GrContext ctxsave;
    int need_redraw = 0;

    if (ta == NULL) return;

    while (ta->l[ta->tclpos].y > GrMaxY()) {
        scroll_up(ta);
        need_redraw = 1;
    }
    
    while (ta->l[ta->tclpos].y < ta->ybits) {
        scroll_down(ta);
        need_redraw = 1;
    }

    if (need_redraw) {
        GrSaveContext(&ctxsave);
        GrSetContext(ta->p->cl);
        hide_tcursor(ta);
        redraw(ta);
        show_tcursor(ta);
        GrSetContext(&ctxsave);
    }
}

/**************************************************************************/

static void scroll_nlines_up(GUITextArea *ta, int nlines)
{
    GrContext ctxsave;
    int i;

    if (ta == NULL) return;

    if (ta->flscr - nlines < 0) nlines = ta->flscr;
    
    if (nlines <= 0) return;
    
    GrSaveContext(&ctxsave);
    GrSetContext(ta->p->cl);
    hide_tcursor(ta);

    ta->flscr -= nlines;
    for (i=0; i<ta->nlines; i++) {
        ta->l[i].y += ta->ybits * nlines;
    }
            
    redraw(ta);

    show_tcursor(ta);
    GrSetContext(&ctxsave);
}

/**************************************************************************/

static void scroll_nlines_down(GUITextArea *ta, int nlines)
{
    GrContext ctxsave;
    int i, llscr;

    if (ta == NULL) return;

    llscr = ta->flscr + ta->nlscr - 1;
    if (llscr + nlines >= ta->nlines) {
        nlines = ta->nlines - 1 - llscr;
    }
    
    if (nlines <= 0) return;
    
    GrSaveContext(&ctxsave);
    GrSetContext(ta->p->cl);
    hide_tcursor(ta);

    ta->flscr += nlines;
    for (i=0; i<ta->nlines; i++) {
        ta->l[i].y -= ta->ybits * nlines;
    }
            
    redraw(ta);

    show_tcursor(ta);
    GrSetContext(&ctxsave);
}

/**************************************************************************/

static void junta_linea(GUITextArea *ta)
{
    int i, x, olen1, ncc, csize, asize;
    void *ptr;
    TALine *tali1, *tali2;
    
    if (ta->tclpos < 1) return;
    
    tali1 = &(ta->l[ta->tclpos-1]);
    tali2 = &(ta->l[ta->tclpos]);
    
    if (tali1->cl == NULL) {
        tali1->cl = malloc(sizeof(TAChar)*INITIAL_LINE_SIZE);
        if (tali1->cl == NULL) {
            ta->full = 1;
            return;
        }
        tali1->maxlen = INITIAL_LINE_SIZE;
    }
    
    olen1 = tali1->len;
    csize = tali1->maxlen - tali1->len;
    asize = 0;
    ncc = tali2->len;
    while (csize+asize < ncc) asize += INCR_LINE_SIZE;
    
    if (asize > 0) {
        ptr = realloc(tali1->cl, sizeof(TAChar) * (tali1->maxlen + asize));
        if (ptr == NULL) {
            ta->full = 1;
            return;
        }
        tali1->cl = ptr;
        tali1->maxlen += asize;
    }

    x = tali1->nextx;
    for (i=0; i<ncc; i++) {
        tali1->cl[olen1+i] = tali2->cl[i];
        tali1->cl[olen1+i].x = x;
        x += tali1->cl[olen1+i].w;
    }
    tali1->len += ncc;
    tali1->nextx = x;

    free(tali2->cl);
    ta->nlines -= 1;
    for (i=ta->tclpos; i<ta->nlines; i++) {
        ta->l[i] = ta->l[i+1];
        ta->l[i].y -= ta->ybits;
    }
    
    ta->tclpos -= 1;
    ta->tccpos = olen1;
    if (tali1->y < ta->ybits) scroll_down(ta);
    redraw(ta);
}

/**************************************************************************/

static void salta_linea(GUITextArea *ta)
{
    int i, j, x, ncc, size;
    
    if (ta->nlines >= ta->maxlines-1) {
        ta->full = 1;
        return;
    }

    ta->nlines += 1;
    for (i=ta->nlines-1; i>ta->tclpos; i--) {
        ta->l[i] = ta->l[i-1];
        ta->l[i].y += ta->ybits;
    }
    ta->tclpos += 1;
    ta->l[ta->tclpos].cl = NULL;
    ta->l[ta->tclpos].len = 0;
    ta->l[ta->tclpos].maxlen = 0;
    ta->l[ta->tclpos].y = ta->l[ta->tclpos-1].y + ta->ybits;
    ta->l[ta->tclpos].nextx = 0;

    ncc = ta->l[ta->tclpos-1].len - ta->tccpos;
    if (ncc > 0) {
        size = INITIAL_LINE_SIZE;
        while (size < ncc) size += INCR_LINE_SIZE;
        ta->l[ta->tclpos].cl = malloc(sizeof(TAChar)*size);
        if (ta->l[ta->tclpos].cl != NULL) {
            ta->l[ta->tclpos].maxlen = size;
            x = 0;
            j = ta->tccpos;
            for (i=0; i<ncc; i++) {
                ta->l[ta->tclpos].cl[i] = ta->l[ta->tclpos-1].cl[j];
                ta->l[ta->tclpos].cl[i].x = x;
                x += ta->l[ta->tclpos].cl[i].w;
                j++;
            }
            ta->l[ta->tclpos].nextx = x;
            ta->l[ta->tclpos-1].nextx -= x;
            ta->l[ta->tclpos].len = ncc;
            ta->l[ta->tclpos-1].len -= ncc;
        } else {
            ta->full = 1;
        }
    }
            
    ta->tccpos = 0;
    if (ta->l[ta->tclpos].y > GrMaxY()) {
        scroll_up(ta);
        redraw(ta);
    } else if (ncc != 0 || ta->tclpos != ta->nlines-1)
        redraw(ta);
}

/**************************************************************************/

static void add_char(GUITextArea *ta, long ch, int chrtype) 
{
    void *ptr;
    unsigned int chr, chu;
    TALine *tali;
    TAChar *tach;
    int i, w;

    tali = &(ta->l[ta->tclpos]);
    if (tali->cl == NULL) {
        tali->cl = malloc(sizeof(TAChar)*INITIAL_LINE_SIZE);
        if (tali->cl == NULL) {
            ta->full = 1;
            return;
        }
        tali->maxlen = INITIAL_LINE_SIZE;
    }
    if (tali->len >= tali->maxlen) {
        ptr = realloc(tali->cl, sizeof(TAChar) *
            (tali->maxlen + INCR_LINE_SIZE));
        if (ptr == NULL) {
            ta->full = 1;
            return;
        }
        tali->cl = ptr;
        tali->maxlen += INCR_LINE_SIZE;
    }
    
    chr = GrFontCharRecode(ta->grt.txo_font, ch, chrtype);
    w = GrFontCharWidth(ta->grt.txo_font, chr);
    // we do this to show glyps if needed using GR_BYTE_TEXT or GR_WORD_TEXT
    chu = GrFontCharReverseRecode(ta->grt.txo_font, chr, GR_UCS2_TEXT);

    tali->len += 1;

    for (i=tali->len-1; i>ta->tccpos; i--) {
        tali->cl[i] = tali->cl[i-1];
        tali->cl[i].x += w;
    }

    tach = &(tali->cl[ta->tccpos]);
    tach->c = chr;
    tach->u = chu;
    tach->fg = ta->grt.txo_fgcolor;
    tach->bg = ta->grt.txo_bgcolor;
    if (ta->tccpos >= tali->len-1) tach->x = tali->nextx;
    tach->w = w;
    
    tali->nextx += tach->w;

    hide_tcursor(ta);
    //for (i=ta->tccpos; i<tali->len; i++)
    //  redraw_char(ta, ta->tclpos, i);
    redraw_line_from(ta, ta->tclpos, ta->tccpos);
 
    ta->tccpos += 1;
    show_tcursor(ta);
}

/**************************************************************************/

static void delete_char(GUITextArea *ta)
{
    TALine *l;
    int i, w;
    
    if (ta->tccpos < 1) {
        junta_linea(ta);
        return;
    }

    l = &(ta->l[ta->tclpos]);
    if (l->len < 1) return;
    w = l->cl[ta->tccpos-1].w;
    for (i=ta->tccpos-1; i<l->len-1; i++) {
        l->cl[i] = l->cl[i+1];
        l->cl[i].x -= w;
    }
    l->len -= 1;
    l->nextx -= w;
    ta->tccpos -= 1;
    //redraw_line(ta, ta->tclpos);
    redraw_line_from(ta, ta->tclpos, ta->tccpos);
}

/**************************************************************************/

static void erase_char(GUITextArea *ta)
{
    TALine *l;
    int i, w;
    
    l = &(ta->l[ta->tclpos]);

    if (ta->tccpos >= l->len) {
        if (ta->tclpos < ta->nlines-1) {
            ta->tclpos += 1;
            ta->tccpos = 0;
            junta_linea(ta);
        }
        return;
    }

    w = l->cl[ta->tccpos].w;
    for (i=ta->tccpos; i<l->len-1; i++) {
        l->cl[i] = l->cl[i+1];
        l->cl[i].x -= w;
    }
    l->len -= 1;
    l->nextx -= w;
    //redraw_line(ta, ta->tclpos);
    redraw_line_from(ta, ta->tclpos, ta->tccpos);
}

/**************************************************************************/

static int calc_line_col_ckick(GUITextArea *ta, int x, int y, int *l, int *c)
{
    TALine *tali;
    int i;

    if (x < 0 || x > GrMaxX()) return 0;
    if (y < 0 || y > GrMaxY()) return 0;
    
    *l = y / ta->ybits;
    if (*l > ta->nlscr) *l = ta->nlscr;
    *l += ta->flscr;
    if (*l >= ta->nlines) *l = ta->nlines - 1;
    if (*l < 0) *l = 0;
    
    tali = &(ta->l[*l]);
    *c = 0;
    x += ta->xorg;
    if (x >= tali->nextx)
        *c = tali->len;
    else {
        for (i=tali->len-1; i>=0; i--) {
            if (x > tali->cl[i].x+tali->cl[i].w/2) {
                *c = i+1;
                break;
            }
        }
    }
    
    return 1;
}

/**************************************************************************/

static int calc_maxx_in_screen(GUITextArea *ta)
{
    int i, maxx;

    maxx = 0;

    for (i=0; i<ta->nlscr; i++) {
        if ((i+ta->flscr) > ta->nlines) break;
        if (ta->l[i+ta->flscr].nextx > maxx) maxx = ta->l[i+ta->flscr].nextx;
    }

    return maxx;
}

/**************************************************************************/

static void redraw_scbs(GUITextArea *ta)
{
    int max;

    if (ta->p->vscb) {
        max = ta->nlines;
        if (max < ta->nlscr) max = ta->nlscr;
        GUIScrollbarSetLimits(ta->p->vscb, 0, max,
                              ta->flscr, ta->flscr+ta->nlscr);
    }

    if (ta->p->hscb) {
        max = calc_maxx_in_screen(ta);
        if (max < GrMaxX()) max = GrMaxX();
        GUIScrollbarSetLimits(ta->p->hscb, 0, max,
                              ta->xorg, ta->xorg+GrMaxX());
    }

    GUIPanelRePaintScb(ta->p);
}

/**************************************************************************/

static void select_char_rel(GUITextArea *ta, int incrc)
{
    GrContext ctxsave;
    GrColor caux;

    if (ta == NULL) return;
    if (incrc != 1 && incrc != -1) return;
    if (ta->tccpos+incrc < 0) return;
    if (ta->tccpos+incrc > ta->l[ta->tclpos].len) return;

    GrSaveContext(&ctxsave);
    GrSetContext(ta->p->cl);
    hide_tcursor(ta);
    if (incrc < 0) ta->tccpos--;
    caux = ta->l[ta->tclpos].cl[ta->tccpos].fg;
    ta->l[ta->tclpos].cl[ta->tccpos].fg = ta->l[ta->tclpos].cl[ta->tccpos].bg;
    ta->l[ta->tclpos].cl[ta->tccpos].bg = caux;
    redraw_char(ta, ta->tclpos, ta->tccpos);
    if (incrc > 0) ta->tccpos++;      
    show_tcursor(ta);
    GrSetContext(&ctxsave);
}
