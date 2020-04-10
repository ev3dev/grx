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
    int tcclast;           // last text cursor char pos
    int tcblink;           // text cursor blinking
    long tctime;           // text cursor blink time
    int pressed;           // left mouse pressed
    int full;              // full, no more chars or lines can be added
    int fmline, fmch;      // line&col first char of marked area
    int lmline, lmch;      // line&col last char of marked area
    int pvmline, pvmch;    // line&col pivot char to extend marked area
} GUITextArea;

static void move_tcursor(GUITextArea *ta, int nline, int nchar, int setmark);
static void move_tcursor_rel(GUITextArea *ta, int incrl, int incrc, int setmark);
static void show_tcursor(GUITextArea *ta);
static void hide_tcursor(GUITextArea *ta);
static void draw_tcursor(GUITextArea *ta, int show);
static void blink_tcursor(GUITextArea *ta);
static void redraw(GUITextArea *ta);
static void redraw_line_from(GUITextArea *ta, int line, int c, int blit);
static void redraw_char(GUITextArea *ta, int line, int c, int blit);

static void scroll_up(GUITextArea *ta);
static void scroll_down(GUITextArea *ta);
static void scroll_toshow_tc(GUITextArea *ta);
static void scroll_nlines_up(GUITextArea *ta, int nlines);
static void scroll_nlines_down(GUITextArea *ta, int nlines);
static void scroll_xorg(GUITextArea *ta, int newxorg);

static int calc_line_col_ckick(GUITextArea *ta, int x, int y,
                               int *l, int *c, int restrictxy);
static int calc_maxx_in_screen(GUITextArea *ta);
static void redraw_scbs(GUITextArea *ta);

static void print_char(GUITextArea *ta, long ch, int chrtype, int draw);
static void new_line(GUITextArea *ta, int draw);
static void add_char(GUITextArea *ta, long ch, int chrtype, int draw);
static void add_string(GUITextArea *ta, void *s, int len, int chrtype, int draw);
static void join_lines(GUITextArea *ta, int draw);
static void delete_char(GUITextArea *ta, int draw);
static void erase_char(GUITextArea *ta, int draw);

static int in_marqued_area(GUITextArea *ta, int line, int c);
static void reset_marqued_area(GUITextArea *ta, int draw);
static void expand_marqued_area(GUITextArea *ta, int draw);
static int delete_marqued_area(GUITextArea *ta, int draw);
static void paste_marqued_area(GUITextArea *ta, int draw);
static void copy_marqued_area(GUITextArea *ta, int clear, int draw);

static void nd_del_line(GUITextArea *ta, int line);
static void nd_del_char_from(GUITextArea *ta, int line, int c);
static void nd_del_char_to(GUITextArea *ta, int line, int c);
static void nd_del_char_from_to(GUITextArea *ta, int line, int fc, int lc);
static int nd_join_lines(GUITextArea *ta, int line);
static unsigned short * nd_get_ucs2text(GUITextArea *ta, int fline, int fc,
                                        int lline, int lc, int *len);
static void nd_put_ucs2text(GUITextArea *ta, unsigned short *buf,
                            int len, int line, int c);
static int nd_add_char(GUITextArea *ta, int line, int c, long ch, int chrtype);
static int nd_new_line(GUITextArea *ta, int line, int c);

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
    ta->xbits = ta->grt.txo_font->maxwidth;
    ta->ybits = ta->grt.txo_font->h.height;
    //ta->nlscr = (ta->p->cl->gc_ymax+1) / ta->ybits;
    ta->nlscr = (ta->p->cl->gc_ymax) / ta->ybits;
    ta->xorg = 0;
    ta->tcstatus = -1;
    ta->tccolor = GrWhite();
    ta->tclpos = 0;
    ta->tccpos = 0;
    ta->tcclast = 0;
    ta->tcblink = 1;
    ta->tctime = 0;
    ta->pressed = 0;
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
    reset_marqued_area(ta, 0);

    return ta;
}

/**************************************************************************/

void GUITASetFont(GUITextArea *ta, GrFont *f)
{
    int i, j;
    long ybits;

    if (ta && f) {
        ta->grt.txo_font = f;
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

void GUITASetCursorColor(GUITextArea *ta, GrColor c)
{
    if (ta == NULL) return;

    ta->tccolor = c;
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

void GUITAMoveCursor(GUITextArea *ta, int nline, int nchar, int setmark)
{
    GrContext ctxsave;

    if (ta == NULL) return;

    GrSaveContext(&ctxsave);
    GrSetContext(ta->p->cl);
    move_tcursor(ta, nline, nchar, setmark);
    GrSetContext(&ctxsave);
}

/**************************************************************************/

void GUITAMoveCursorRel(GUITextArea *ta, int incrl, int incrc, int setmark)
{
    GrContext ctxsave;

    if (ta == NULL) return;

    GrSaveContext(&ctxsave);
    GrSetContext(ta->p->cl);
    move_tcursor_rel(ta, incrl, incrc, setmark);
    GrSetContext(&ctxsave);
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

void GUITAClear(GUITextArea *ta)
{
    int i;
    
    if (ta == NULL) return;

    ta->flscr = 0;
    ta->nlines = 1;
    for (i=0; i<ta->maxlines; i++) {
        ta->l[i].len = 0;
        ta->l[i].y = ta->ybits * (i + 1);
        ta->l[i].nextx = 0;
    }
    ta->xorg = 0;
    ta->tcstatus = -1;
    ta->tclpos = 0;
    ta->tccpos = 0;
    ta->tcclast = 0;
    ta->tcblink = 1;
    ta->tctime = 0;
    ta->pressed = 0;
    ta->full = 0;
    reset_marqued_area(ta, 0);
    
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
    new_line(ta, 1);
    scroll_toshow_tc(ta);
    show_tcursor(ta);
    GrSetContext(&ctxsave);
}

/**************************************************************************/

void GUITAPutChar(GUITextArea *ta, long ch, int chrtype)
{
    GrContext ctxsave;

    if (ta == NULL) return;
    if (ta->full) return;

    GrSaveContext(&ctxsave);
    GrSetContext(ta->p->cl);
    hide_tcursor(ta);
    add_char(ta, ch, chrtype, 1);
    //scroll_toshow_tc(ta);
    show_tcursor(ta);
    GrSetContext(&ctxsave);
}

/**************************************************************************/

void GUITAPutString(GUITextArea *ta, void *s, int len, int chrtype)
{
    GrContext ctxsave;

    if (ta == NULL) return;
    if (ta->full) return;

    GrSaveContext(&ctxsave);
    GrSetContext(ta->p->cl);
    hide_tcursor(ta);
    add_string(ta, s, len, chrtype, 1);
    //scroll_toshow_tc(ta);
    show_tcursor(ta);
    GrSetContext(&ctxsave);
}

/**************************************************************************/

void GUITAPrintChar(GUITextArea *ta, long ch, int chrtype)
{
    GrContext ctxsave;

    if (ta == NULL) return;
    if (ta->full) return;

    GrSaveContext(&ctxsave);
    GrSetContext(ta->p->cl);
    print_char(ta, ch, chrtype, 1);
    scroll_toshow_tc(ta);
    GrSetContext(&ctxsave);
}

/**************************************************************************/

void GUITAGetStatus(GUITextArea *ta, GUITAStatus *tast)
{
    if (ta == NULL) return;
    
    tast->nlines = ta->nlines;
    tast->tclpos = ta->tclpos;
    tast->tccpos = ta->tccpos;
    tast->ncscr = (ta->p->cl->gc_xmax+1) / ta->xbits;
    tast->fmline = ta->fmline;
    tast->fmch = ta->fmch;
    tast->lmline = ta->lmline;
    tast->lmch = ta->lmch;
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
            s[i] = GrCharRecodeFromUCS2(ta->l[nline].cl[i].u, chrtype);
        s[len] = '\0';
        return s;
    case GR_UTF8_TEXT:
        s = malloc((len*4)+1);
        if (s == NULL) return NULL;
        j = 0;
        cutf8 = (unsigned char *)&laux;
        for (i=0; i<len; i++) {
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
            h[i] = ta->l[nline].cl[i].u;
        h[len] = '\0';
        return h;
    }

    return NULL;
}

/**************************************************************************/

int GUITAProcessEvent(GUITextArea *ta, GrEvent *ev)
{
    // because called by panel, the context is expected to be set
    int l, c, range;

    if (ev->type == GREV_MOUSE) {
        if (ev->p1 == GRMOUSE_LB_PRESSED) {
            if (calc_line_col_ckick(ta, ev->p2, ev->p3, &l, &c, 1)){
                ta->pressed = 1;
                move_tcursor(ta, l, c, 1);
                ta->tcclast = ta->tccpos;
                scroll_toshow_tc(ta);
            }
            return 1;
        } else if (ev->p1 == GRMOUSE_B4_RELEASED) {
            //move_tcursor_rel(ta, -3, 0, 1);
            scroll_nlines_up(ta, 3);
            //scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GRMOUSE_B5_RELEASED) {
            //move_tcursor_rel(ta, 3, 0, 1);
            scroll_nlines_down(ta, 3);
            //scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GRMOUSE_LB_RELEASED) {
            ta->pressed = 0;
            return 1;
        }
    } else if (ev->type == GREV_MMOVE) {
        if ((ev->p1 == GRMOUSE_LB_PRESSED) && ta->pressed) {
            if (calc_line_col_ckick(ta, ev->p2, ev->p3, &l, &c, 0)){
                move_tcursor(ta, l, c, -1);
                ta->tcclast = ta->tccpos;
                scroll_toshow_tc(ta);
            }
            return 1;
        }
    } else if (ev->type == GREV_SCBVERT) {
        range = ta->flscr - ev->p1;
        if (range > 0) scroll_nlines_up(ta, range);
        if (range < 0) scroll_nlines_down(ta, -range);
        return 1;
    } else if (ev->type == GREV_SCBHORZ) {
        scroll_xorg(ta, ev->p1);
        return 1;
    } else if ((ev->type == GREV_KEY) && (ev->p2 != GRKEY_KEYCODE)) {
        if (ev->p1 == GrKey_Control_C) {
            copy_marqued_area(ta, 0, 0);
            return 1;
        } else if (ev->p1 == GrKey_Control_V) {
            paste_marqued_area(ta, 1);
            scroll_toshow_tc(ta);
        } else if (ev->p1 == GrKey_Control_X) {
            copy_marqued_area(ta, 1, 1);
            scroll_toshow_tc(ta);
        } else {
            print_char(ta, ev->p1, GrGetChrtypeForUserEncoding(), 1);
            scroll_toshow_tc(ta);
        }
        return 1;
    } else if ((ev->type == GREV_KEY) && (ev->p2 == GRKEY_KEYCODE)) {
        if (ev->p1 == GrKey_Delete) {
            print_char(ta, 0x7f, GrGetChrtypeForUserEncoding(), 1);
            scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GrKey_Left) {
            move_tcursor_rel(ta, 0, -1, 1);
            scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GrKey_Right) {
            move_tcursor_rel(ta, 0, 1, 1);
            scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GrKey_Up) {
            move_tcursor_rel(ta, -1, 0, 1);
            scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GrKey_Down) {
            move_tcursor_rel(ta, 1, 0, 1);
            scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GrKey_Home) {
            move_tcursor_rel(ta, 0, -ta->tccpos, 1);
            scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GrKey_End) {
            move_tcursor_rel(ta, 0, ta->l[ta->tclpos].len - ta->tccpos, 1);
            scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GrKey_Control_Home) {
            move_tcursor_rel(ta, -ta->tclpos, -ta->tccpos, 1);
            scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GrKey_Control_End) {
            move_tcursor_rel(ta, ta->nlines - ta->tclpos - 1,
                              ta->l[ta->nlines-1].len - ta->tccpos, 1);
            scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GrKey_PageUp) {
            move_tcursor_rel(ta, -(ta->nlscr-1), 0, 1);
            scroll_nlines_up(ta, ta->nlscr-1);
            scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GrKey_PageDown) {
            move_tcursor_rel(ta, ta->nlscr-1, 0, 1);
            scroll_nlines_down(ta, ta->nlscr-1);
            scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GrKey_Shift_Left) {
            move_tcursor_rel(ta, 0, -1, -1);
            scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GrKey_Shift_Right) {
            move_tcursor_rel(ta, 0, 1, -1);
            scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GrKey_Shift_Up) {
            move_tcursor_rel(ta, -1, 0, -1);
            scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GrKey_Shift_Down) {
            move_tcursor_rel(ta, 1, 0, -1);
            scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GrKey_Shift_Home) {
            move_tcursor_rel(ta, 0, -ta->tccpos, -1);
            scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GrKey_Shift_End) {
            move_tcursor_rel(ta, 0, ta->l[ta->tclpos].len - ta->tccpos, -1);
            scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GrKey_Shift_PageUp) {
            move_tcursor_rel(ta, -(ta->nlscr-1), 0, -1);
            scroll_nlines_up(ta, ta->nlscr-1);
            scroll_toshow_tc(ta);
            return 1;
        } else if (ev->p1 == GrKey_Shift_PageDown) {
            move_tcursor_rel(ta, ta->nlscr-1, 0, -1);
            scroll_nlines_down(ta, ta->nlscr-1);
            scroll_toshow_tc(ta);
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

static void move_tcursor(GUITextArea *ta, int nline, int nchar, int setmark)
{
/* internal version, assume ta != NULL and context setted */

    if (nline < 0) nline = 0;
    if (nline >= ta->nlines) nline = ta->nlines - 1;
    if (nchar < 0) nchar = 0;
    if (nchar > ta->l[nline].len) nchar = ta->l[nline].len;
    
    hide_tcursor(ta);
    ta->tclpos = nline;
    ta->tccpos = nchar;
    if (setmark == 1) reset_marqued_area(ta, 1);
    if (setmark == -1) expand_marqued_area(ta, 1);
    show_tcursor(ta);
}

/**************************************************************************/

static void move_tcursor_rel(GUITextArea *ta, int incrl, int incrc, int setmark)
{
    int nline, nchar;
    int save_tcclast = 0;
    
    nline = ta->tclpos + incrl;
    nchar = ta->tccpos + incrc;

    if (incrl == 0) {
        if ((nchar < 0) && (nline > 0)) {
            nline -= 1;
            nchar = ta->l[nline].len;
        }
        if ((nchar > ta->l[nline].len) && (nline < ta->nlines-1)) {
            nline += 1;
            nchar = 0;
        }
    }
    if (incrc == 0) {
        nchar = ta->tcclast;
    } else {
        save_tcclast = 1;
    }

    move_tcursor(ta, nline, nchar, setmark);

    if (save_tcclast) ta->tcclast = ta->tccpos;
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

    if (ta->tclpos < ta->flscr) return;
    l = &(ta->l[ta->tclpos]);
    ypos = l->y;
    if (ypos > GrMaxY()) return;
    xpos_org = (ta->tccpos >= l->len) ? l->nextx : l->cl[ta->tccpos].x;
    xpos = xpos_org - ta->xorg;
    if ((xpos < 0) || (xpos > GrMaxX())) return;
    
    if (show) {
        GrVLine(xpos, ypos, ypos-ta->ybits+1, ta->tccolor);
        GrVLine(xpos+1, ypos, ypos-ta->ybits+1, ta->tccolor);
        GUIDBCurCtxBltRectToScreen(xpos, ypos-ta->ybits+1, xpos+1, ypos);
        ta->tctime = 0; // reset blink
    } else if (ta->tccpos >= l->len) {
        GrVLine(xpos, ypos, ypos-ta->ybits+1, ta->bgcolor);
        GrVLine(xpos+1, ypos, ypos-ta->ybits+1, ta->bgcolor);
        GUIDBCurCtxBltRectToScreen(xpos, ypos-ta->ybits+1, xpos+1, ypos);
    } else {
        redraw_char(ta, ta->tclpos, ta->tccpos, 1);
    }
}

/**************************************************************************/

static void blink_tcursor(GUITextArea *ta)
{
    // routine used "only" by the blink code when tc is showed
    TALine *l;
    int xpos_org, xpos, ypos;

    if (ta->tclpos < ta->flscr) return;
    l = &(ta->l[ta->tclpos]);
    ypos = l->y;
    if (ypos > GrMaxY()) return;
    xpos_org = (ta->tccpos >= l->len) ? l->nextx : l->cl[ta->tccpos].x;
    xpos = xpos_org - ta->xorg;
    if ((xpos < 0) || (xpos > GrMaxX())) return;
    
    if (ta->tcblink) {
        GrVLine(xpos, ypos, ypos-ta->ybits+1, ta->tccolor);
        GrVLine(xpos+1, ypos, ypos-ta->ybits+1, ta->tccolor);
        GUIDBCurCtxBltRectToScreen(xpos, ypos-ta->ybits+1, xpos+1, ypos);
   } else if (ta->tccpos >= l->len) {
        GrVLine(xpos, ypos, ypos-ta->ybits+1, ta->bgcolor);
        GrVLine(xpos+1, ypos, ypos-ta->ybits+1, ta->bgcolor);
        GUIDBCurCtxBltRectToScreen(xpos, ypos-ta->ybits+1, xpos+1, ypos);
    } else {
        //GrVLine(xpos, ypos, ypos-ta->ybits+1, ta->bgcolor);
        //GrVLine(xpos+1, ypos, ypos-ta->ybits+1, ta->bgcolor);
        //GUIDBCurCtxBltRectToScreen(xpos, ypos-ta->ybits+1, xpos+1, ypos);
        redraw_char(ta, ta->tclpos, ta->tccpos, 1);
    }
}

/**************************************************************************/

static void redraw(GUITextArea *ta)
{
    int i, cl;
    
    cl = 0;
    for (i=ta->flscr; i<ta->nlines; i++) {
        redraw_line_from(ta, i, 0, 0);
        cl++;
        if (ta->l[i].y > GrMaxY()) break;
    }
    
    i = ta->nlscr + 1 - cl;
    if (i > 0)
        GrFilledBox(0, (cl*ta->ybits)+1, GrMaxX(), (cl+i)*ta->ybits, ta->bgcolor);
    
    GUIDBCurCtxBltToScreen();
    redraw_scbs(ta);
}

/**************************************************************************/

static void redraw_line_from(GUITextArea *ta, int line, int c, int blit)
{
    int j, x1;
    
    if (ta->l[line].y > (GrMaxY()+ta->ybits)) return;
    for (j=c; j<ta->l[line].len; j++) {
        redraw_char(ta, line, j, 0);
    }
    x1 = ta->l[line].nextx - ta->xorg;
    if (x1 < 0) x1 = 0;
    if (x1 < GrSizeX())
        GrFilledBox(x1, ta->l[line].y-ta->ybits+1, GrMaxX(), ta->l[line].y,
                    ta->bgcolor);
    x1 = (c >= ta->l[line].len) ? ta->l[line].nextx - ta->xorg : 
                                  ta->l[line].cl[c].x - ta->xorg;
    if (x1 < 0) x1 = 0;
    if (x1 < GrSizeX() && blit)
        GUIDBCurCtxBltRectToScreen(x1, ta->l[line].y-ta->ybits+1,
                                 GrMaxX(), ta->l[line].y);
}

/**************************************************************************/

static void redraw_char(GUITextArea *ta, int line, int c, int blit)
{
    GrTextOption grt2;
    int x1;
    
    if (ta->l[line].y > (GrMaxY()+ta->ybits)) return;
    if (c >= ta->l[line].len) return;
    grt2 = ta->grt;
    if (in_marqued_area(ta, line, c)) {
        grt2.txo_fgcolor = ta->l[line].cl[c].bg;
        grt2.txo_bgcolor = ta->l[line].cl[c].fg;
    } else {
        grt2.txo_fgcolor = ta->l[line].cl[c].fg;
        grt2.txo_bgcolor = ta->l[line].cl[c].bg;
    }
    x1 = ta->l[line].cl[c].x-ta->xorg;
    GrDrawChar(ta->l[line].cl[c].c, x1, ta->l[line].y, &grt2);

    if (blit)
        GUIDBCurCtxBltRectToScreen(x1, ta->l[line].y-ta->ybits+1,
                                 x1+ta->l[line].cl[c].w-1, ta->l[line].y);
                                 
}

/**************************************************************************/

static void scroll_up(GUITextArea *ta)
{
    int i;
    
    if (ta->flscr == 0) return;
    ta->flscr -= 1;
    for (i=0; i<ta->nlines; i++) {
        ta->l[i].y += ta->ybits;
    }
}

/**************************************************************************/

static void scroll_down(GUITextArea *ta)
{
    int i;
    
    ta->flscr += 1;
    for (i=0; i<ta->nlines; i++) {
        ta->l[i].y -= ta->ybits;
    }
}

/**************************************************************************/

static void scroll_toshow_tc(GUITextArea *ta)
{
    TALine *l;
    int xpos_org, xpos;
    int need_redraw = 0;

    l = &(ta->l[ta->tclpos]);

    while (l->y > GrMaxY()) {
        scroll_down(ta);
        need_redraw = 1;
    }
    while (l->y < ta->ybits) {
        scroll_up(ta);
        need_redraw = 1;
    }

    xpos_org = (ta->tccpos >= l->len) ? l->nextx : l->cl[ta->tccpos].x;
    xpos = xpos_org - ta->xorg;
    while (xpos < 0) {
        ta->xorg -= ta->xbits;
        if (ta->xorg < 0) ta->xorg = 0;
        need_redraw = 1;
        xpos = xpos_org - ta->xorg;
    }
    while (xpos > GrMaxX()) {
        ta->xorg += ta->xbits;
        need_redraw = 1;
        xpos = xpos_org - ta->xorg;
    }

    if (need_redraw) {
        hide_tcursor(ta);
        redraw(ta);
        show_tcursor(ta);
    }
}

/**************************************************************************/

static void scroll_nlines_up(GUITextArea *ta, int nlines)
{
    int i;

    if (ta->flscr - nlines < 0) nlines = ta->flscr;
    
    if (nlines <= 0) return;
    
    hide_tcursor(ta);

    ta->flscr -= nlines;
    for (i=0; i<ta->nlines; i++) {
        ta->l[i].y += ta->ybits * nlines;
    }
            
    redraw(ta);
    show_tcursor(ta);
}

/**************************************************************************/

static void scroll_nlines_down(GUITextArea *ta, int nlines)
{
    int i, llscr;

    llscr = ta->flscr + ta->nlscr - 1;
    if (llscr + nlines >= ta->nlines) {
        nlines = ta->nlines - 1 - llscr;
    }
    
    if (nlines <= 0) return;
    
    hide_tcursor(ta);

    ta->flscr += nlines;
    for (i=0; i<ta->nlines; i++) {
        ta->l[i].y -= ta->ybits * nlines;
    }
            
    redraw(ta);
    show_tcursor(ta);
}

/**************************************************************************/

static void scroll_xorg(GUITextArea *ta, int newxorg)
{
    if (newxorg < 0) newxorg = 0;
    if (ta->xorg != newxorg) {
        ta->xorg = newxorg;
        hide_tcursor(ta);
        redraw(ta);
        show_tcursor(ta);
    }
}

/**************************************************************************/

static int calc_line_col_ckick(GUITextArea *ta, int x, int y,
                               int *l, int *c, int restrictxy)
{
    TALine *tali;
    int i;

    if (restrictxy && (x < 0 || x > GrMaxX())) return 0;
    if (restrictxy && (y < 0 || y > GrMaxY())) return 0;
    
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
/* edit function from the text cursor position, and call redraw screen    */
/* they assume the context is set and the tex cursor hidden               */
/* excepting print_char                                                   */
/**************************************************************************/

static void print_char(GUITextArea *ta, long ch, int chrtype, int draw)
{
    if (ta->full) return;

    if (draw) hide_tcursor(ta);

    if (ch == '\r') {
        new_line(ta, draw);
    } else if (ch == '\b') {
        delete_char(ta, draw);
    } else if (ch == 0x7f) {
        erase_char(ta, draw);
    } else {
        add_char(ta, ch, chrtype, draw);
    }

    if (draw) show_tcursor(ta);
}

/**************************************************************************/

static void new_line(GUITextArea *ta, int draw)
{
    delete_marqued_area(ta, draw);

    if (!nd_new_line(ta, ta->tclpos, ta->tccpos)) return;

    ta->tclpos += 1;
    ta->tccpos = 0;
    ta->tcclast = ta->tccpos;
    reset_marqued_area(ta, 0);
    if (ta->l[ta->tclpos].y > GrMaxY()) scroll_down(ta);
    if (draw) redraw(ta);
}

/**************************************************************************/

static void add_char(GUITextArea *ta, long ch, int chrtype, int draw) 
{
    delete_marqued_area(ta, draw);

    if (!nd_add_char(ta, ta->tclpos, ta->tccpos, ch, chrtype)) return;

    ta->tccpos += 1;
    ta->tcclast = ta->tccpos;
    reset_marqued_area(ta, 0);
    if (draw) redraw_line_from(ta, ta->tclpos, ta->tccpos-1, 1);
}

/**************************************************************************/

static void add_string(GUITextArea *ta, void *s, int len, int chrtype, int draw)
{
    unsigned short *text2;
    int i, newchrtype, ind;

    if (len <= 0) len = GrStrLen(s, chrtype);
    text2 = GrTextRecodeToUCS2(s, len, chrtype);
    if (text2 == NULL) return;
    newchrtype = GR_UCS2_TEXT;
    if (chrtype == GR_BYTE_TEXT || chrtype == GR_WORD_TEXT)
        newchrtype = GR_WORD_TEXT; // so we can draw glyphs if necesary

    delete_marqued_area(ta, draw);

    ind = 0;
    for (i=0; i<len; i++) {
        if (!nd_add_char(ta, ta->tclpos, ta->tccpos+ind, text2[i], newchrtype))
            break;
        ind++;
    }

    free(text2);

    ta->tccpos += ind;
    ta->tcclast = ta->tccpos;
    reset_marqued_area(ta, 0);
    if (draw) redraw_line_from(ta, ta->tclpos, ta->tccpos-ind, 1);
}

/**************************************************************************/

static void join_lines(GUITextArea *ta, int draw)
{
    // used only by delete_char and erase_char
    // so do not need delete_marqued_area by now
    int olen1;
    
    if (ta->tclpos < 1) return;
    olen1 = ta->l[ta->tclpos-1].len;
    
    if (!nd_join_lines(ta, ta->tclpos)) return;

    ta->tclpos -= 1;
    ta->tccpos = olen1;
    ta->tcclast = ta->tccpos;
    if (ta->l[ta->tclpos].y < ta->ybits) scroll_up(ta);
    reset_marqued_area(ta, 0);
    if (draw) redraw(ta);
}

/**************************************************************************/

static void delete_char(GUITextArea *ta, int draw)
{
    TALine *l;
    int i, w;

    if (delete_marqued_area(ta, draw)) return;

    if (ta->tccpos < 1) {
        join_lines(ta, draw);
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
    ta->tcclast = ta->tccpos;
    reset_marqued_area(ta, 0);
    if (draw) redraw_line_from(ta, ta->tclpos, ta->tccpos, 1);
}

/**************************************************************************/

static void erase_char(GUITextArea *ta, int draw)
{
    TALine *l;
    int i, w;
    
    if (delete_marqued_area(ta, draw)) return;

    l = &(ta->l[ta->tclpos]);

    if (ta->tccpos >= l->len) {
        if (ta->tclpos < ta->nlines-1) {
            ta->tclpos += 1;
            ta->tccpos = 0;
            join_lines(ta, draw);
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
    ta->tcclast = ta->tccpos;
    reset_marqued_area(ta, 0);
    if (draw) redraw_line_from(ta, ta->tclpos, ta->tccpos, 1);
}

/**************************************************************************/
/* marqued area functions                                                 */
/**************************************************************************/

static int in_marqued_area(GUITextArea *ta, int line, int c)
{
    if ((line > ta->fmline) && (line < ta->lmline)) return 1;
    if ((line == ta->fmline) && (c < ta->fmch)) return 0;
    if ((line == ta->lmline) && (c >= ta->lmch)) return 0;
    if ((line == ta->fmline) || (line == ta->lmline)) return 1;
    return 0;
}

/**************************************************************************/

static void reset_marqued_area(GUITextArea *ta, int draw)
{
    int fmline, lmline, fmch, lmch;
    int i;
    
    fmline = ta->fmline;
    lmline = ta->lmline;
    fmch = ta->fmch;
    lmch = ta->lmch;

    ta->fmline = ta->lmline = ta->pvmline = ta->tclpos;
    ta->fmch = ta->lmch = ta->pvmch = ta->tccpos;

    if (draw) {
        if ((fmline == lmline) && (fmch == lmch)) return;
        hide_tcursor(ta);
        for (i=fmline; i<=lmline; i++) {
            if (i >= ta->flscr) redraw_line_from(ta, i, 0, 1);
        }
        show_tcursor(ta);
    }
}

/**************************************************************************/

static void expand_marqued_area(GUITextArea *ta, int draw)
{
    int fmline, lmline;
    int minline, maxline;
    int left, i;
    
    fmline = ta->fmline;
    lmline = ta->lmline;

    left = 0;
    if (ta->tclpos < ta->pvmline) left = 1;
    if ((ta->tclpos == ta->pvmline) && (ta->tccpos < ta->pvmch)) left = 1;
    if (left) {
        ta->fmline = ta->tclpos;
        ta->fmch = ta->tccpos;
        ta->lmline = ta->pvmline;
        ta->lmch = ta->pvmch;
    } else {
        ta->fmline = ta->pvmline;
        ta->fmch = ta->pvmch;
        ta->lmline = ta->tclpos;
        ta->lmch = ta->tccpos;
    }

    if (draw) {
        minline = (fmline < ta->fmline) ? fmline : ta->fmline;
        maxline = (lmline > ta->lmline) ? lmline : ta->lmline;
        hide_tcursor(ta);
        for (i=minline; i<=maxline; i++)
            if (i >= ta->flscr) redraw_line_from(ta, i, 0, 1);
        show_tcursor(ta);
    }
}

/**************************************************************************/

static int delete_marqued_area(GUITextArea *ta, int draw)
{
    int i;

    if ((ta->fmline == ta->lmline) && (ta->fmch == ta->lmch)) return 0;

    for (i=0; i<(ta->lmline - ta->fmline - 1); i++) {
        nd_del_line(ta, ta->fmline+1);
    }
    if (ta->lmline != ta->fmline) {
        nd_del_char_from(ta, ta->fmline, ta->fmch);
        nd_del_char_to(ta, ta->fmline+1, ta->lmch);
        nd_join_lines(ta, ta->fmline+1);
    } else {
        nd_del_char_from_to(ta, ta->fmline, ta->fmch, ta->lmch);
    }
    
    ta->tclpos = ta->fmline;
    ta->tccpos = ta->fmch;
    ta->tcclast = ta->tccpos;
    reset_marqued_area(ta, 0);

    if (draw) {
        hide_tcursor(ta);
        while (ta->l[ta->tclpos].y > GrMaxY()) scroll_down(ta);
        while (ta->l[ta->tclpos].y < ta->ybits) scroll_up(ta);
        redraw(ta);
        show_tcursor(ta);
    }

    return 1;
}

/**************************************************************************/

static void copy_marqued_area(GUITextArea *ta, int clear, int draw)
{
    unsigned short *buf;
    int len;
    
    if ((ta->fmline == ta->lmline) && (ta->fmch == ta->lmch)) return;

    buf = nd_get_ucs2text(ta, ta->fmline, ta->fmch,
                         ta->lmline, ta->lmch, &len);
    if (buf == NULL) return;

    _GUISetClipBoard(buf, len);
    free(buf);

    if (ta->full) return;

    if (clear) delete_marqued_area(ta, draw);
}

/**************************************************************************/

static void paste_marqued_area(GUITextArea *ta, int draw)
{
    unsigned short *buf;
    int len;

    if (ta->full) return;

    len = _GUIGetLenClipBoard();
    if (len <=0) return;

    buf = malloc((len+1)*sizeof(unsigned short));
    if (buf == NULL) return;

    delete_marqued_area(ta, 0);

    len = _GUIGetClipBoard(buf, len+1);
    nd_put_ucs2text(ta, buf, len, ta->tclpos, ta->tccpos);

    free(buf);

    if (draw) {
        hide_tcursor(ta);
        while (ta->l[ta->tclpos].y > GrMaxY()) scroll_down(ta);
        while (ta->l[ta->tclpos].y < ta->ybits) scroll_up(ta);
        redraw(ta);
        show_tcursor(ta);
    }
}

/**************************************************************************/
/* nd_ functions do their work without drawing anything and without       */
/* moving the text cursor                                                 */
/**************************************************************************/
 
static void nd_del_line(GUITextArea *ta, int line)
{
    int i;

    if ((line < 0) || (line >= ta->nlines)) return;
    ta->nlines -= 1;
    free(ta->l[line].cl);
    for (i=line; i<ta->nlines; i++) {
        ta->l[i] = ta->l[i+1];
        ta->l[i].y -= ta->ybits;
    }
    ta->l[ta->nlines].cl = NULL;
    ta->l[ta->nlines].len = 0;
    ta->l[ta->nlines].maxlen = 0;
    ta->l[ta->nlines].nextx = 0;
}

/**************************************************************************/

static void nd_del_char_from(GUITextArea *ta, int line, int c)
{
    if ((line < 0) || (line >= ta->nlines)) return;
    if ((c < 0) || (c >= ta->l[line].len-1)) return;
    ta->l[line].len = c;
    ta->l[line].nextx = ta->l[line].cl[c].x;
}

/**************************************************************************/

static void nd_del_char_to(GUITextArea *ta, int line, int c)
{
    int i, x, newlen;

    if ((line < 0) || (line >= ta->nlines)) return;
    if ((c < 1) || (c > ta->l[line].len)) return;
    newlen = ta->l[line].len - c;
    x = 0;
    for (i=0; i<newlen; i++) {
        ta->l[line].cl[i] = ta->l[line].cl[i+c];
        ta->l[line].cl[i].x = x;
        x += ta->l[line].cl[i].w;
    }
    ta->l[line].len = newlen;
    ta->l[line].nextx = x;
}

/**************************************************************************/

static void nd_del_char_from_to(GUITextArea *ta, int line, int fc, int lc)
{
    int i, x, newlen, delchars;

    if ((line < 0) || (line >= ta->nlines)) return;
    delchars = lc - fc;
    if ((fc < 0) || (delchars < 1)) return;
    if (lc > ta->l[line].len) return;
    newlen = ta->l[line].len - delchars;
    x = (fc == 0) ? 0 : ta->l[line].cl[fc].x;
    for (i=fc; i<newlen; i++) {
        ta->l[line].cl[i] = ta->l[line].cl[i+delchars];
        ta->l[line].cl[i].x = x;
        x += ta->l[line].cl[i].w;
    }
    ta->l[line].len = newlen;
    ta->l[line].nextx = x;
}

/**************************************************************************/

static int nd_join_lines(GUITextArea *ta, int line)
{
    // join line-1 with line
    int i, x, olen1, ncc, csize, asize;
    void *ptr;
    TALine *tali1, *tali2;
    
    if ((line < 1) || (line >= ta->nlines)) return 0;
    
    tali1 = &(ta->l[line-1]);
    tali2 = &(ta->l[line]);
    
    if (tali1->cl == NULL) {
        tali1->cl = malloc(sizeof(TAChar)*INITIAL_LINE_SIZE);
        if (tali1->cl == NULL) {
            ta->full = 1;
            return 0;
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
            return 0;
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

    nd_del_line(ta, line);
    
    return 1;
}

/**************************************************************************/

static unsigned short * nd_get_ucs2text(GUITextArea *ta, int fline, int fc,
                                        int lline, int lc, int *len)
{
    unsigned short *buf;
    int nchars, i, j, ind;

    if ((fline < 0) || (fline > lline) || (lline >= ta->nlines)) return NULL;
    if ((fline == lline) && (fc >= lc)) return NULL;
    if ((fc < 0) || (fc > ta->l[fline].len)) return NULL;
    if ((lc < 0) || (lc > ta->l[lline].len)) return NULL;

    nchars = 0;
    if (fline == lline) {
        nchars = lc - fc;
    } else {
        nchars = ta->l[fline].len - fc;
        nchars += 1; // for \n
        for (j=fline+1; j<lline; j++) {
            nchars += ta->l[j].len;
            nchars += 1; // for \n
        }
        nchars += lc;
    }
    nchars += 1; // for \0

    buf = (unsigned short *)malloc(nchars*sizeof(unsigned short));
    if (buf == NULL) return NULL;

    ind = 0;
    if (fline == lline) {
        for (i=fc; i<lc; i++) {
            buf[ind++] = ta->l[fline].cl[i].u;
        }
    } else {
        for (i=fc; i<ta->l[fline].len; i++) {
            buf[ind++] = ta->l[fline].cl[i].u;
        }
        buf[ind++] = '\n';
        for (j=fline+1; j<lline; j++) {
            for (i=0; i<ta->l[j].len; i++) {
                buf[ind++] = ta->l[j].cl[i].u;
            }
            buf[ind++] = '\n';
        }
        for (i=0; i<lc; i++) {
            buf[ind++] = ta->l[lline].cl[i].u;
        }
    }
    *len = ind;
    buf[ind] = 0;

    return buf;
}

/**************************************************************************/

static void nd_put_ucs2text(GUITextArea *ta, unsigned short *buf,
                            int len, int line, int c)
{
    int i;

    if ((line < 0) || (line >= ta->nlines)) return;
    if ((c < 0) || (c > ta->l[line].len)) return;

    for (i=0; i<len; i++) {
        if (buf[i] == 0) break;
        if (buf[i] == '\n') {
            if (!nd_new_line(ta, line, c)) break;
            line++;
            c = 0;
        } else {
            if (!nd_add_char(ta, line, c, buf[i], GR_UCS2_TEXT)) break;
            c++;
        }
    }
}

/**************************************************************************/

static int nd_add_char(GUITextArea *ta, int line, int c, long ch, int chrtype)
{
    void *ptr;
    unsigned int chr, chu;
    TALine *tali;
    TAChar *tach;
    int i, w;

    if ((line < 0) || (line >= ta->nlines)) return 0;
    if ((c < 0) || (c > ta->l[line].len)) return 0;

    tali = &(ta->l[line]);
    if (tali->cl == NULL) {
        tali->cl = malloc(sizeof(TAChar)*INITIAL_LINE_SIZE);
        if (tali->cl == NULL) {
            ta->full = 1;
            return 0;
        }
        tali->maxlen = INITIAL_LINE_SIZE;
    }
    if (tali->len >= tali->maxlen) {
        ptr = realloc(tali->cl, sizeof(TAChar) *
            (tali->maxlen + INCR_LINE_SIZE));
        if (ptr == NULL) {
            ta->full = 1;
            return 0;
        }
        tali->cl = ptr;
        tali->maxlen += INCR_LINE_SIZE;
    }

    chr = GrFontCharRecode(ta->grt.txo_font, ch, chrtype);
    w = GrFontCharWidth(ta->grt.txo_font, chr);
    // we do this to show glyps if needed using GR_BYTE_TEXT or GR_WORD_TEXT
    // and also retain the real UCS2 char
    if (chrtype == GR_BYTE_TEXT || chrtype == GR_WORD_TEXT)
        chu = GrFontCharReverseRecode(ta->grt.txo_font, chr, GR_UCS2_TEXT);
    else if (chrtype == GR_UCS2_TEXT)
        chu = ch;
    else
        chu = GrCharRecodeToUCS2(ch, chrtype);

    tali->len += 1;

    for (i=tali->len-1; i>c; i--) {
        tali->cl[i] = tali->cl[i-1];
        tali->cl[i].x += w;
    }

    tach = &(tali->cl[c]);
    tach->c = chr;
    tach->u = chu;
    tach->fg = ta->grt.txo_fgcolor;
    tach->bg = ta->grt.txo_bgcolor;
    if (c >= tali->len-1) tach->x = tali->nextx;
    tach->w = w;
    
    tali->nextx += tach->w;

    return 1;
}

/**************************************************************************/

static int nd_new_line(GUITextArea *ta, int line, int c)
{
    TAChar *tach = NULL;
    int i, j, x, ncc, size = 0;
    
    if ((line < 0) || (line >= ta->nlines)) return 0;
    if ((c < 0) || (c > ta->l[line].len)) return 0;

    if (ta->nlines >= ta->maxlines-1) {
        ta->full = 1;
        return 0;
    }

    ncc = ta->l[line].len - c;
    if (ncc > 0) {
        size = INITIAL_LINE_SIZE;
        while (size < ncc) size += INCR_LINE_SIZE;
        tach = malloc(sizeof(TAChar)*size);
        if (tach == NULL) {
            ta->full = 1;
            return 0;
        }
    }

    ta->nlines += 1;
    for (i=ta->nlines-1; i>line; i--) {
        ta->l[i] = ta->l[i-1];
        ta->l[i].y += ta->ybits;
    }
    line += 1;
    ta->l[line].cl = NULL;
    ta->l[line].len = 0;
    ta->l[line].maxlen = 0;
    ta->l[line].y = ta->l[line-1].y + ta->ybits;
    ta->l[line].nextx = 0;

    if (ncc > 0) {
        ta->l[line].cl = tach;
        ta->l[line].maxlen = size;
        x = 0;
        j = c;
        for (i=0; i<ncc; i++) {
            ta->l[line].cl[i] = ta->l[line-1].cl[j];
            ta->l[line].cl[i].x = x;
            x += ta->l[line].cl[i].w;
            j++;
        }
        ta->l[line].nextx = x;
        ta->l[line-1].nextx -= x;
        ta->l[line].len = ncc;
        ta->l[line-1].len -= ncc;
    }

    return 1;
}
