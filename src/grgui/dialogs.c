/**
 ** dialogs.c ---- Mini GUI for MGRX, raw dialogs
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

#define BORDERW 4
#define FILLTITLE 4

int _dlgtitlechrtype = GR_BYTE_TEXT;
GrFont *_dlgtitlefont = NULL;
int _dlgtitlefontloaded = 0;

GrColor _dlglinecolor;
GrColor _dlgbordercolor;
GrColor _dlgfgcolor;
GrColor _dlgbgcolor;

static GrTextOption tgrtopt;

static int dialog_running = 0;

static void paint_titledlg(void *udata);

void _GUIDialogInit(void)
{
    dialog_running = 0;

    _dlglinecolor = GrBlack();
    _dlgbordercolor = GrWhite();
    _dlgfgcolor = GrBlack();
    _dlgbgcolor = GrWhite();

    _dlgtitlefont = GrGetDefaultFont();
    _dlgtitlefontloaded = 0;
    _dlgtitlechrtype = GrGetChrtypeForUserEncoding();
    
    tgrtopt.txo_font = _dlgtitlefont;
    tgrtopt.txo_fgcolor = _dlgbgcolor;
    tgrtopt.txo_bgcolor = GrNOCOLOR;
    tgrtopt.txo_chrtype = _dlgtitlechrtype;
    tgrtopt.txo_direct = GR_TEXT_RIGHT;
    tgrtopt.txo_xalign = GR_ALIGN_CENTER;
    tgrtopt.txo_yalign = GR_ALIGN_TOP;
}

void _GUIDialogEnd(void)
{
    if (_dlgtitlefontloaded) GrUnloadFont(_dlgtitlefont);
    _dlgtitlefont = NULL;
}

int _GUIDialogRunning(void)
{
    return dialog_running;
}

void GUIDialogsSetTitleChrType(int chrtype)
{
    _dlgtitlechrtype = chrtype;
    tgrtopt.txo_chrtype = _dlgtitlechrtype;
}

void GUIDialogsSetTitleFont(GrFont *fnt)
{
    if (_dlgtitlefontloaded) {
        GrUnloadFont(_dlgtitlefont);
        _dlgtitlefontloaded = 0;
    }
    _dlgtitlefont = fnt;
    tgrtopt.txo_font = _dlgtitlefont;
}

void GUIDialogsSetTitleFontByName(char *fntname)
{
    if (_dlgtitlefontloaded) {
        GrUnloadFont(_dlgtitlefont);
        _dlgtitlefontloaded = 0;
    }
    _dlgtitlefont = GrLoadFont(fntname);
    if (_dlgtitlefont == NULL )
        _dlgtitlefont = GrGetDefaultFont();
    else
        _dlgtitlefontloaded = 1; 
    tgrtopt.txo_font = _dlgtitlefont;
}

void GUIDialogsSetColors(GrColor line, GrColor border, GrColor bg, GrColor fg)
{
    _dlglinecolor = line;
    _dlgbordercolor = border;
    _dlgfgcolor = fg;
    _dlgbgcolor = bg;
    tgrtopt.txo_fgcolor = bg;
}

void GUIDialogsGetDims(int *titleheight, int *borderwidth)
{
    *titleheight = _dlgtitlefont->h.height + FILLTITLE;
    *borderwidth = BORDERW;
}

GUIDialog * GUIDialogCreate(void *title, int x, int y, int width, int height)
{
    GUIDialog *d;

    d = (GUIDialog *)malloc(sizeof(GUIDialog));
    if (d == NULL) return NULL;

    d->title = NULL;
    d->theight = 0;
    if (title) {
        d->title = title;
        d->theight = _dlgtitlefont->h.height + FILLTITLE;
    }
    
    d->p = GUIPanelCreate(x, y, width, height, GUI_PCAPB_SU, BORDERW, d->theight);
    if (d->p == NULL) {
        free(d);
        return NULL;
    }
    
    if (d->title) GUIPanelSetTlCallBack(d->p, paint_titledlg);
    GUIPanelSetUserData(d->p, (void *)d);

    d->running = 0;
    d->result = 0;
    d->exdata = NULL;
    d->fg = _dlgfgcolor;
    d->bg = _dlgbgcolor;
    
    return d;
}

void GUIDialogDestroy(GUIDialog *d)
{
    if (d == NULL) return;
    GUIPanelDestroy(d->p);
    free(d);
}

int GUIDialogRun(GUIDialog *d)
{
    GrEvent ev;
    int ret = 0;
    
    if (d == NULL) return -1;

    dialog_running++;
    if (dialog_running == 1) {
        _GUISuspendHooks();
        _GUITilesDisable();
    }

    d->running = 1;
    GUIContextSaveUnder(d->p->gc);
    GUIPanelPaint(d->p, _dlglinecolor, _dlgbordercolor);

    while (1) {
        GrEventRead(&ev);
        if (ev.type == GREV_KEY && ev.p1 == GrKey_Escape) {
            ret = -1;
            break;
        }
        if (GUIPanelProcessEvent(d->p, &ev) < 0) break;
    }
    
    GUIContextRestoreUnder(d->p->gc);
    d->running = 0;

    if (dialog_running == 1) {
        _GUITilesEnable();
        _GUIRestartHooks();
    }
    dialog_running--;
    
    return ret;
}

static void paint_titledlg(void *udata)
{
    GUIDialog *d = (GUIDialog *)udata;

    GrClearContext(_dlgfgcolor);
    if (d->title) {
        GrDrawString(d->title, 0, GrSizeX()/2, FILLTITLE/2, &tgrtopt);
    }
}
