/**
 ** cdialogs.c ---- Mini GUI for MGRX, common dialogs
 **
 ** Copyright (C) 2019 Mariano Alvarez Fernandez
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
#include "grguip.h"

int _cdlgchrtype = GR_BYTE_TEXT;
GrFont *_cdlgfont = NULL;
int _dlgfontloaded = 0;

GrColor _cdlgbtbgcolor;
GrColor _cdlgbtfgcolor;

static GrTextOption grtopt;

#define MINWIDTH 100
#define BTWIDTH 80
#define BTNHEIGHT 40
#define MINSPACE 30
#define FILLTITLE 4
#define FILLSPACE 10
#define FILLSPACEX2 20

#define COMMAND_OK 1
#define COMMAND_NO 2
#define COMMAND_CANCEL 3

static int genericdlg(void *title, void **text, int nlin, int nb,
                      void *b1, void *b2, void *b3);
static GUIGroup *creatego(void **text, int nlin, int nb,
                          void *b1, void *b2, void *b3);
static void CalcDimensions(void *title, GUIGroup *g, int *dlgx, int *dlgy,
                           int *dlgwidth, int *dlgheight);
static void paint_genericdlg(void *udata);
static int processev_genericdlg(void *udata, GrEvent *ev);

void _GUICDialogInit(void)
{
    _cdlgfont = GrGetDefaultFont();
    _cdlgbtbgcolor = GrWhite();
    _cdlgbtfgcolor = GrBlack();

    _cdlgchrtype = GrGetChrtypeForUserEncoding();
    
    grtopt.txo_font = _cdlgfont;
    grtopt.txo_fgcolor = _dlgfgcolor;
    grtopt.txo_bgcolor = GrNOCOLOR;
    grtopt.txo_chrtype = _cdlgchrtype;
    grtopt.txo_direct = GR_TEXT_RIGHT;
    grtopt.txo_xalign = GR_ALIGN_CENTER;
    grtopt.txo_yalign = GR_ALIGN_TOP;
}

void _GUICDialogEnd(void)
{
    if (_dlgfontloaded) GrUnloadFont(_cdlgfont);
    _cdlgfont = NULL;
}

void GUICDialogsSetChrType(int chrtype)
{
    _cdlgchrtype = chrtype;
    grtopt.txo_chrtype = _cdlgchrtype;
}

void GUICDialogsSetFont(GrFont *fnt)
{
    if (_dlgfontloaded) {
        GrUnloadFont(_cdlgfont);
        _dlgfontloaded = 0;
    }
    _cdlgfont = fnt;
    grtopt.txo_font = _cdlgfont;
}

void GUICDialogsSetFontByName(char *fntname)
{
    if (_dlgfontloaded) {
        GrUnloadFont(_cdlgfont);
        _dlgfontloaded = 0;
    }
    _cdlgfont = GrLoadFont(fntname);
    if (_cdlgfont == NULL )
        _cdlgfont = &GrFont_PC8x16;
    else
        _dlgfontloaded = 1; 
    grtopt.txo_font = _cdlgfont;
}

void GUICDialogsSetColors(GrColor btbg, GrColor btfg)
{
    _cdlgbtbgcolor = btbg;
    _cdlgbtfgcolor = btfg;
    grtopt.txo_fgcolor = _dlgfgcolor;
}

void GUICDialogInfo(void *title, void **text, int nlin, void *ok)
{
    genericdlg(title, text, nlin, 1, ok, "", "");
}

int GUICDialogYesNo(void *title, void **text, int nlin, void *yes, void *no)
{
    return genericdlg(title, text, nlin, 2, yes, no, "");
}

int GUICDialogYesNoCancel(void *title, void **text, int nlin,
                          void *yes, void *no, void *cancel)
{
    return genericdlg(title, text, nlin, 3, yes, no, cancel);
}

GUIDialog * GUIGroupDialogCreate(void *title, GUIGroup *go,
                                 int (*proevfn)(void *, GrEvent *))
{
    GUIDialog *d;
    int dlgx, dlgy, dlgwidth, dlgheight;

    CalcDimensions(title, go, &dlgx, &dlgy, &dlgwidth, &dlgheight);
    d = GUIDialogCreate(title, dlgx, dlgy, dlgwidth, dlgheight);
    if (d == NULL) {
        return NULL;
    }
    d->exdata = (void *)go;
    d->result = -1;
    GUIGroupSetPanel(go, d->p);

    GUIPanelSetClCallBacks(d->p, paint_genericdlg, proevfn);
    
    return d;
}

static int genericdlg(void *title, void **text, int nlin, int nb,
                      void *b1, void *b2, void *b3)
{
    GUIDialog *d;
    GUIGroup *go;
    int dlgx, dlgy, dlgwidth, dlgheight;
    int result;

    go = creatego(text, nlin, nb, b1, b2, b3);
    if (go == NULL) return -1;

    CalcDimensions(title, go, &dlgx, &dlgy, &dlgwidth, &dlgheight);
    d = GUIDialogCreate(title, dlgx, dlgy, dlgwidth, dlgheight);
    if (d == NULL) {
        GUIGroupDestroy(go);
        return -1;
    }
    d->exdata = (void *)go;
    d->result = -1;
    GUIGroupSetPanel(go, d->p);

    GUIPanelSetClCallBacks(d->p, paint_genericdlg, processev_genericdlg);
    GUIDialogRun(d);
    result = d->result;
    GUIGroupDestroy(go);
    GUIDialogDestroy(d);
    
    return result;
}

static GUIGroup *creatego(void **text, int nlin, int nb,
                          void *b1, void *b2, void *b3)
{
    GUIGroup *go;
    int i, l, twidth, theight, bwidth, torig, borig, x, y;
    
    go = GUIGroupCreate(nb+1, 0, 0);
    if (go == NULL) return NULL;
    
    twidth = 0;
    for (i=0; i<nlin; i++) {
        l = GrFontStringWidth(_cdlgfont, text[i], 0, _cdlgchrtype);
        if (l > twidth) twidth = l;
    }
    theight = _cdlgfont->h.height * nlin + 4;

    bwidth = BTWIDTH * nb + FILLSPACE * (nb - 1);
    if (twidth > bwidth) {
        torig = 0;
        borig = (twidth - bwidth) / 2;
    } else {
        borig = 0;
        torig = (bwidth - twidth) / 2;
    }

    GUIObjectSetText(&(go->o[0]), 0, torig, 0, twidth, theight, GrNOCOLOR,
                     _dlgfgcolor, text, nlin, GR_ALIGN_CENTER, _cdlgfont);
    x = borig;
    y = theight + FILLSPACE;
    GUIObjectSetButton(&(go->o[1]), 1, x, y, BTWIDTH, BTNHEIGHT,
                       _cdlgbtbgcolor, _cdlgbtfgcolor, b1, COMMAND_OK, 0, 0);
    if (nb > 1) {
        x += BTWIDTH + FILLSPACE;
        GUIObjectSetButton(&(go->o[2]), 2, x, y, BTWIDTH, BTNHEIGHT,
                           _cdlgbtbgcolor, _cdlgbtfgcolor, b2, COMMAND_NO, 0, 0);
    }
    if (nb > 2) {
        x += BTWIDTH + FILLSPACE;
        GUIObjectSetButton(&(go->o[3]), 3, x, y, BTWIDTH, BTNHEIGHT, _cdlgbtbgcolor,
                           _cdlgbtfgcolor, b3, COMMAND_CANCEL, 0, 0);
    }
    GUIGroupSetSelected(go, nb, 0);

    return go;
}

static void CalcDimensions(void *title, GUIGroup *g, int *dlgx, int *dlgy,
                           int *dlgwidth, int *dlgheight)
{
    int th, b, gw, gh;

    GUIDialogsGetDims(&th, &b);
    GUIGroupCalcMinDim(g, &gw, &gh);

    *dlgwidth = gw +  b*2 + FILLSPACEX2;
    if (*dlgwidth > GrScreenX() - MINSPACE) *dlgwidth = GrScreenX() - MINSPACE;
    *dlgx = (GrScreenX() - *dlgwidth) / 2;
    
    *dlgheight = gh + b*2 + FILLSPACEX2;
    if (title) *dlgheight += th;
    if (*dlgheight > GrScreenY() - MINSPACE) *dlgheight = GrScreenY() - MINSPACE;
    *dlgy = (GrScreenY() - *dlgheight) / 2;
}

static void paint_genericdlg(void *udata)
{
    GUIDialog *d = (GUIDialog *)udata;
    GUIGroup *go = (GUIGroup *)(d->exdata);

    GrClearContext(_dlgbgcolor);
    go->x = FILLSPACE;
    go->y = FILLSPACE;
    GUIGroupPaint(go);
}

static int processev_genericdlg(void *udata, GrEvent *ev)
{
    GUIDialog *d = (GUIDialog *)udata;
    GUIGroup *go = (GUIGroup *)(d->exdata);

    if (ev->type == GREV_COMMAND && ev->p1 == COMMAND_OK) {
        d->result = 1;
        return -1;
    } else if (ev->type == GREV_COMMAND && ev->p1 == COMMAND_NO) {
        d->result = 0;
        return -1;
    } else if (ev->type == GREV_COMMAND && ev->p1 == COMMAND_CANCEL) {
        d->result = -1;
        return -1;
    }
    
    return GUIGroupProcessEvent(go, ev);
}
