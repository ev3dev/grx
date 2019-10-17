/**
 ** oentry.c ---- Mini GUI for MGRX, specific funtions for entries
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grguip.h"

#define BLINK_TIME 500

typedef struct {
    unsigned short *c;    // char array
    unsigned short *x;    // x-pos array
    unsigned short *w;    // char width
    int len;              // actual number of chars
    int tcpos;            // text cursor char pos
    int xbits, ybits;     // biggest char dimensions
    int xorg;             // x-pos difference to show cursor
    int blink, save;      // blink: 1=on, 0=off, save used only by blink code
    long time;            // cursor blink time
    int changed;          // entry edited
} EntryData;

static void add_char(GUIObject *o, long ch, int chrtype);
static void delete_char(GUIObject *o);
static void erase_char(GUIObject *o);
static void calc_tcpos_click(GUIObject *o, int x);

/***************************/

void GUIObjectSetEntry(GUIObject *o, int id, int x, int y, int width, int height,
                       GrColor bg, GrColor fg, int maxlen, void *initext)
{
    EntryData *data;
    unsigned short *text2;
    int i, len;
    
    o->type = GUIOBJTYPE_ENTRY;
    o->id = id;
    o->x = x;
    o->y = y;
    o->width = width;
    o->height = height;
    o->bg = bg;
    o->fg = fg;
    o->maxlen = maxlen;
    o->text = initext;
    
    o->cid = 0;
    o->sgid = 0;
    o->on = 0;
    o->pressed = 0;
    o->selected = 0;

    data = malloc(sizeof(EntryData));
    if (data != NULL) {
        data->c = malloc(sizeof(unsigned short)*maxlen);
        data->x = malloc(sizeof(unsigned short)*maxlen);
        data->w = malloc(sizeof(unsigned short)*maxlen);
    }
    if (!data || !data->c || !data->x || !data->w) {
        if (data && data->c) free(data->c);
        if (data && data->x) free(data->x);
        if (data && data->w) free(data->w);
        if (data) free(data);
        o->type = GUIOBJTYPE_NONE;
        o->pressed = -1;
        o->selected = -1;
        return;
    }
    data->len = 0;
    data->tcpos = 0;
    data->xbits = _objectfont->maxwidth;
    data->ybits = _objectfont->h.height;
    data->xorg = 0;
    data->blink = 1;
    data->save = 0;
    data->time = 0;
    o->data = (void *)data;

    len = GrStrLen(initext, _objectchrtype);
    text2 = GrFontTextRecode(_objectfont, initext, len, _objectchrtype);
    if (text2 == NULL) return;
    
    for (i = 0; i<len; i++) {
        add_char(o, text2[i], GR_WORD_TEXT);
    }
    data->changed = 0;
    free(text2);
}

/***************************/

void _GUIOEntryDestroy(GUIObject *o)
{
    EntryData *data;
    
    data = (EntryData *)(o->data);

    free(data->c);
    free(data->x);
    free(data->w);
    free(data);
}

/***************************/

void *_GUIOEntryGetText(GUIObject *o, int chrtype)
{
    EntryData *data;
    unsigned char *s;
    unsigned short *h;
    long l;
    unsigned char *cutf8;
    int i, j;
    
    data = (EntryData *)(o->data);
    
    switch (chrtype) {
        case GR_BYTE_TEXT:
        case GR_CP437_TEXT:
        case GR_CP850_TEXT:
        case GR_CP1252_TEXT:
        case GR_ISO_8859_1_TEXT:
            s = malloc(data->len+1);
            if (s == NULL) return NULL;
            for (i=0; i<data->len; i++)
                s[i] = GrFontCharReverseRecode(_objectfont, data->c[i], chrtype);
            s[data->len] = '\0';
            return s;
        case GR_UTF8_TEXT:
            s = malloc((data->len*4)+1);
            if (s == NULL) return NULL;
            j = 0;
            cutf8 = (unsigned char *)&l;
            for (i=0; i<data->len; i++) {
                l = GrFontCharReverseRecode(_objectfont, data->c[i], chrtype);
                s[j++] = cutf8[0];
                if (cutf8[1] != '\0') s[j++] = cutf8[1];
                if (cutf8[2] != '\0') s[j++] = cutf8[2];
                if (cutf8[3] != '\0') s[j++] = cutf8[3];
            }
            s[j] = '\0';
            return s;
        case GR_WORD_TEXT:
        case GR_UCS2_TEXT:
            h = malloc((data->len+1)*sizeof(unsigned short));
            if (h == NULL) return NULL;
            for (i=0; i<data->len; i++)
                h[i] = GrFontCharReverseRecode(_objectfont, data->c[i], chrtype);
            h[data->len] = '\0';
            return h;
    }

    return NULL;
}

/***************************/

void _GUIOEntryPaint(GUIObject *o, int x, int y)
{
    GrLineOption glo;
    EntryData *data;
    int tcx_org, tcx, tcy;
    int entry_maxx;
    
    data = (EntryData *)(o->data);
    tcx = 0;

    _objectentopt.txo_fgcolor = o->fg;
    GrBox(x+o->x, y+o->y, x+o->x+o->width-1, y+o->y+o->height-1, _objectlcolor);
    GrFilledBox(x+o->x+1, y+o->y+1, x+o->x+o->width-2, y+o->y+o->height-2, o->bg);
    entry_maxx = o->width - 7;

    if (o->selected) {
        glo.lno_color = o->fg;
        glo.lno_width = 1;
        glo.lno_pattlen = 2;
        glo.lno_dashpat = (unsigned char *)"\2\1";
        GrCustomBox(x+o->x+2, y+o->y+2, x+o->x+o->width-3, y+o->y+o->height-3, &glo);
        if (data->tcpos == 0)
            tcx_org = 0;
        else if (data->tcpos >= data->len)
            tcx_org = data->x[data->tcpos-1] + data->w[data->tcpos-1];
        else
            tcx_org = data->x[data->tcpos];
        tcx = tcx_org - data->xorg;
        while (tcx < 0) {
            data->xorg -= data->xbits;
            tcx = tcx_org - data->xorg;
        }
        while (tcx > entry_maxx) {
            data->xorg += data->xbits;
            tcx = tcx_org - data->xorg;
        }
        //printf("tcpos %d  len %d  xorg %d\n", data->tcpos, data->len, data->xorg);
    }

    GrSetClipBox(x+o->x+3, y+o->y+1, x+o->x+o->width-4, y+o->y+o->height-2);

    if (data->len > 0) {
        GrDrawString(data->c, data->len, x+o->x+3-data->xorg, y+o->y+o->height/2,
                     &_objectentopt);
    }

    if (!data->save) {
        data->blink = 1;
        data->time = 0;
    }

    if (o->selected && data->blink) {
        tcx += x + o->x + 3;
        tcy = y + o->y + o->height/2 - data->ybits/2 - 1;
        GrVLine(tcx, tcy, tcy+data->ybits, o->fg);
        GrVLine(tcx+1, tcy, tcy+data->ybits, o->fg);
    }

    GrResetClipBox();

    if (!o->selected && data->changed) {
        GrEventParEnqueue(GREV_FCHANGE, o->id, 0, 0, 0);
        data->changed = 0;
    }
}

/***************************/

void _GUIOEntryRePaint(GUIGroup *g, GUIObject *o)
{
    _GUIOEntryPaint(o, g->x, g->y);
    if (g->p) {
        GUIPanelBltRectClToScreen(g->p, g->x+o->x, g->y+o->y,
                                  o->width, o->height);
    }
}

/***************************/

int _GUIOEntryProcessEvent(GUIGroup *g, GUIObject *o, GrEvent *ev)
{
    EntryData *data;
    
    data = (EntryData *)(o->data);
    
    if (ev->type == GREV_MOUSE) {
        if (ev->p1 == GRMOUSE_LB_PRESSED) {
            o->selected = 1;
            o->pressed = 1;
            calc_tcpos_click(o, ev->p2-g->x-o->x);
            _GUIOEntryRePaint(g, o);
            return 1;
        } else if (ev->p1 == GRMOUSE_LB_RELEASED) {
            o->pressed = 0;
            return 1;
        }
    } else if ((ev->type == GREV_KEY) && (ev->p2 == GRKEY_KEYCODE)) {
        if (ev->p1 == GrKey_Delete) {
            erase_char(o);
        } else if (ev->p1 == GrKey_Left) {
            if (data->tcpos > 0) {
                data->tcpos -= 1;
                _GUIOEntryRePaint(g, o);
            }
            return 1;
        } else if (ev->p1 == GrKey_Right) {
            if (data->tcpos < data->len) {
                data->tcpos += 1;
                _GUIOEntryRePaint(g, o);
            }
            return 1;
        } else if (ev->p1 == GrKey_Home) {
            if (data->tcpos > 0) {
                data->tcpos = 0;
                _GUIOEntryRePaint(g, o);
            }
            return 1;
        } else if (ev->p1 == GrKey_End) {
            if (data->tcpos < data->len) {
                data->tcpos = data->len;
                _GUIOEntryRePaint(g, o);
            }
            return 1;
        }
    } else if ((ev->type == GREV_KEY) && (ev->p2 != GRKEY_KEYCODE)) {
        if (ev->p1 == '\r') {
            if (data->changed) {
                GrEventParEnqueue(GREV_FCHANGE, o->id, 0, 0, 0);
                data->changed = 0;
            }
        } else if (ev->p1 == '\b') {
            delete_char(o);
        } else if (ev->p1 == 0x7f) {
            erase_char(o);
        } else if (ev->p1 >= ' ') {
            add_char(o, ev->p1, GrGetChrtypeForUserEncoding());
        }
        _GUIOEntryRePaint(g, o);
        return 1;
    } else if (ev->type == GREV_NULL) {
        if (data->time == 0) {
            data->time = ev->time;
        } else {
            if (ev->time > data->time + BLINK_TIME) {
                data->time = ev->time;
                data->blink ^= 1;
                data->save = 1;
                _GUIOEntryRePaint(g, o);
                data->save = 0;
            }
        }
    }
    
    return 0;
}

/***************************/

static void add_char(GUIObject *o, long ch, int chrtype)
{
    EntryData *data;
    long chr;
    unsigned short w;
    int i;
    
    data = (EntryData *)(o->data);
    
    if (data->len >= o->maxlen) return;
    
    chr = GrFontCharRecode(_objectfont, ch, chrtype);
    w = GrFontCharWidth(_objectfont, chr);

    for (i=data->len; i>data->tcpos; i--) {
        data->c[i] = data->c[i-1];
        data->x[i] = data->x[i-1] + w;
        data->w[i] = data->w[i-1];
    }
        
    data->c[data->tcpos] = chr;
    data->w[data->tcpos] = w;
    if (data->tcpos == 0) 
        data->x[data->tcpos] = 0;
    else
        data->x[data->tcpos] = data->x[data->tcpos-1] + data->w[data->tcpos-1];
    
    data->len += 1;
    data->tcpos += 1;
    data->changed = 1;
}

/***************************/

static void delete_char(GUIObject *o)
{
    EntryData *data;
    unsigned short w;
    int i;
    
    data = (EntryData *)(o->data);

    if (data->tcpos < 1) return;
    if (data->len < 1) return;

    w = data->w[data->tcpos-1];
    
    for (i=data->tcpos-1; i<data->len-1; i++) {
        data->c[i] = data->c[i+1];
        data->x[i] = data->x[i+1] - w;
        data->w[i] = data->w[i+1];
    }

    data->len -= 1;
    data->tcpos -= 1;
    data->changed = 1;
}

/***************************/

static void erase_char(GUIObject *o)
{
    EntryData *data;
    unsigned short w;
    int i;
    
    data = (EntryData *)(o->data);

    if (data->tcpos >= data->len) return;

    w = data->w[data->tcpos];
    
    for (i=data->tcpos; i<data->len-1; i++) {
        data->c[i] = data->c[i+1];
        data->x[i] = data->x[i+1] - w;
        data->w[i] = data->w[i+1];
    }

    data->len -= 1;
    data->changed = 1;
}

/***************************/

static void calc_tcpos_click(GUIObject *o, int x)
{
    EntryData *data;
    int i, pos;
    
    data = (EntryData *)(o->data);

    x += data->xorg - 3;
    pos = 0;
    for (i=data->len-1; i>=0; i--) {
        if (x > data->x[i]+data->w[i]/2) {
            pos = i+1;
            break;
        }
    }
    data->tcpos = pos;
}
