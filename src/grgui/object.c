/**
 ** object.c ---- Mini GUI for MGRX, objects (labels, buttons, etc)
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

int _objectchrtype = GR_BYTE_TEXT;
GrFont *_objectfont = NULL;
int _objectfontloaded = 0;

GrColor _objectlcolor;
GrColor _objectshcolor1;
GrColor _objectshcolor2;

GrTextOption _objectgenopt; // generic
GrTextOption _objectbutopt; // buttons
GrTextOption _objectentopt; // entries

/***************************/

void _GUIObjectInit(void)
{
    _objectfont = GrGetDefaultFont();
    _objectlcolor = GrBlack();
    _objectshcolor1 = GrWhite();
    _objectshcolor2 = GrWhite();

    _objectchrtype = GrGetChrtypeForUserEncoding();
    
    _objectgenopt.txo_font = _objectfont;
    _objectgenopt.txo_fgcolor = GrWhite();
    _objectgenopt.txo_bgcolor = GrNOCOLOR;
    _objectgenopt.txo_chrtype = _objectchrtype;
    _objectgenopt.txo_direct = GR_TEXT_RIGHT;
    _objectgenopt.txo_xalign = GR_ALIGN_LEFT;
    _objectgenopt.txo_yalign = GR_ALIGN_CENTER;

    _objectbutopt = _objectgenopt;
    _objectbutopt.txo_xalign = GR_ALIGN_CENTER;
    _objectbutopt.txo_yalign = GR_ALIGN_CENTER;

    _objectentopt = _objectgenopt;
    _objectentopt.txo_chrtype = GR_WORD_TEXT;
    _objectentopt.txo_yalign = GR_ALIGN_CENTER;
}

/***************************/

void _GUIObjectEnd(void)
{
    if (_objectfontloaded) GrUnloadFont(_objectfont);
    _objectfont = NULL;
    _objectfontloaded = 0;
}

/***************************/

void GUIObjectsSetChrType(int chrtype)
{
    _objectchrtype = chrtype;
    _objectgenopt.txo_chrtype = _objectchrtype;
    _objectbutopt.txo_chrtype = _objectchrtype;
}

/***************************/

void GUIObjectsSetFont(GrFont *fnt)
{
    if (_objectfontloaded) {
        GrUnloadFont(_objectfont);
        _objectfontloaded = 0;
    }
    _objectfont = fnt;
    _objectgenopt.txo_font = _objectfont;
    _objectbutopt.txo_font = _objectfont;
    _objectentopt.txo_font = _objectfont;
}

/***************************/

void GUIObjectsSetFontByName(char *fntname)
{
    if (_objectfontloaded) {
        GrUnloadFont(_objectfont);
        _objectfontloaded = 0;
    }
    _objectfont = GrLoadFont(fntname);
    if (_objectfont == NULL )
        _objectfont = &GrFont_PC8x16;
    else
        _objectfontloaded = 1; 
    _objectgenopt.txo_font = _objectfont;
    _objectbutopt.txo_font = _objectfont;
    _objectentopt.txo_font = _objectfont;
}

/***************************/

void GUIObjectsSetColors(GrColor l, GrColor sh1, GrColor sh2)
{
    _objectlcolor = l;
    _objectshcolor1 = sh1;
    _objectshcolor2 = sh2;
}

/***************************/

void _GUIObjectPaint(GUIObject *o, int dx, int dy)
{
    switch (o->type) {
        case GUIOBJTYPE_LABEL:
            _GUIOLabelPaint(o, dx, dy);
            break;
        case GUIOBJTYPE_TEXT:
            _GUIOTextPaint(o, dx, dy);
            break;
        case GUIOBJTYPE_LIGHT:
            _GUIOLightPaint(o, dx, dy);
            break;
        case GUIOBJTYPE_BUTTON:
            _GUIOButtonPaint(o, dx, dy);
            break;
        case GUIOBJTYPE_ENTRY:
            _GUIOEntryPaint(o, dx, dy);
            break;
        case GUIOBJTYPE_LIST:
            _GUIOListPaint(o, dx, dy);
            break;
        case GUIOBJTYPE_DLIST:
            _GUIODListPaint(o, dx, dy);
            break;
        case GUIOBJTYPE_REGLIST:
            _GUIORegListPaint(o, dx, dy);
            break;
    }
}

/***************************/

int _GUIObjectProcessEvent(GUIGroup *g, GUIObject *o, GrEvent *ev)
{
    switch (o->type) {
        case GUIOBJTYPE_LABEL:
            return 0;
        case GUIOBJTYPE_TEXT:
            return 0;
        case GUIOBJTYPE_LIGHT:
            return 0;
        case GUIOBJTYPE_BUTTON:
            return _GUIOButtonProcessEvent(g, o, ev);
        case GUIOBJTYPE_ENTRY:
            return _GUIOEntryProcessEvent(g, o, ev);
        case GUIOBJTYPE_LIST:
            return _GUIOListProcessEvent(g, o, ev);
        case GUIOBJTYPE_DLIST:
            return _GUIODListProcessEvent(g, o, ev);
        case GUIOBJTYPE_REGLIST:
            return _GUIORegListProcessEvent(g, o, ev);
        default:
            break;
    }
    
    return 0;
}

/***************************/

void _GUIObjectSetOn(GUIGroup *g, GUIObject *o, int paint)
{
    switch (o->type) {
        case GUIOBJTYPE_LABEL:
            break;
        case GUIOBJTYPE_TEXT:
            break;
        case GUIOBJTYPE_LIGHT:
            _GUIOLightSetOn(g, o, paint);
            break;
        case GUIOBJTYPE_BUTTON:
            _GUIOButtonSetOn(g, o, paint);
            break;
        case GUIOBJTYPE_ENTRY:
            break;
        case GUIOBJTYPE_LIST:
            break;
        case GUIOBJTYPE_DLIST:
            break;
        case GUIOBJTYPE_REGLIST:
            break;
    }
}

/***************************/

void _GUIObjectDestroy(GUIObject *o)
{
    switch (o->type) {
        case GUIOBJTYPE_LABEL:
            break;
        case GUIOBJTYPE_TEXT:
            _GUIOTextDestroy(o);
            break;
        case GUIOBJTYPE_LIGHT:
            break;
        case GUIOBJTYPE_BUTTON:
            break;
        case GUIOBJTYPE_ENTRY:
            _GUIOEntryDestroy(o);
            break;
        case GUIOBJTYPE_LIST:
            _GUIOListDestroy(o);
            break;
        case GUIOBJTYPE_DLIST:
            _GUIODListDestroy(o);
            break;
        case GUIOBJTYPE_REGLIST:
            _GUIORegListDestroy(o);
            break;
    }
}

/***************************/

void *_GUIObjectGetText(GUIObject *o, int chrtype)
{
    switch (o->type) {
        case GUIOBJTYPE_LABEL:
            return NULL;
        case GUIOBJTYPE_TEXT:
            return NULL;
        case GUIOBJTYPE_LIGHT:
            return NULL;
        case GUIOBJTYPE_BUTTON:
            return NULL;
        case GUIOBJTYPE_ENTRY:
            return _GUIOEntryGetText(o, chrtype);
        case GUIOBJTYPE_LIST:
            return NULL;
        case GUIOBJTYPE_DLIST:
            return NULL;
        case GUIOBJTYPE_REGLIST:
            return NULL;
        default:
            break;
    }
    
    return NULL;
}

/***************************/

void _GUIObjectSetText(GUIObject *o, void *newtext)
{
    switch (o->type) {
        case GUIOBJTYPE_LABEL:
            _GUIOLabelSetText(o, newtext);
            break;
        case GUIOBJTYPE_TEXT:
            break;
        case GUIOBJTYPE_LIGHT:
            _GUIOLightSetText(o, newtext);
            break;
        case GUIOBJTYPE_BUTTON:
            _GUIOButtonSetText(o, newtext);
            break;
        case GUIOBJTYPE_ENTRY:
            _GUIOEntrySetText(o, newtext);
            break;
        case GUIOBJTYPE_LIST:
            break;
        case GUIOBJTYPE_DLIST:
            break;
        case GUIOBJTYPE_REGLIST:
            break;
    }
}
