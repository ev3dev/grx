/**
 ** grguip.h ---- Mini GUI for MGRX, private header 
 **
 ** Copyright (C) 2002,2005,2019 Mariano Alvarez Fernandez
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

#ifndef __GRGUIP_H_INCLUDED__
#define __GRGUIP_H_INCLUDED__

#include "grgui.h"

/**
 ** EVENTS (GREV_PRIVGUI subtypes)
 **/

#define GPEV_MENUCANCEL    1
#define GPEV_ACTMENUBAR    2

/**
 ** SETUP
 **/

extern int _GUIUseDB;
extern GrContext *_GUIGlobCtx;

void _GUISuspendHooks(void);
void _GUIRestartHooks(void);
int _GUIGetNoHookNow(void);

/**
 ** GUI CONTEXTS
 **/

/**
 ** KEY SHORT CUTS
 **/

void _GUIKeyShortCutInit(void);
void _GUIKeyShortCutEnd(void);
int _GUIKeyShortCutHookEvent(GrEvent *ev);
void _GUIKeyShortCutReset(void);
int _GUIKeyShortCutAdd(int key, int type, long p1, long p2, long p3);

/**
 ** MENUS
 **/

extern int _menuchrtype;
extern GrFont *_menufont;
extern int _menufontloaded;
extern GrColor _menubgcolor;
extern GrColor _menufgcolor;
extern GrColor _menubgscolor;
extern GrColor _menufgscolor;
extern GrColor _menufgnacolor;
extern GrColor _menufgsnacolor;

void _GUIMenuInit(void);
void _GUIMenuEnd(void);
void _GUIMenuSetKeyShortCut(int idmenu);
int _GUIMenuRun(int idmenu, int x, int y, int minwidth);

/**
 ** MENUBAR
 **/

void _GUIMenuBarInit(void);
void _GUIMenuBarEnd(void);
int _GUIMenuBarHookEvent(GrEvent *ev);
int _GUIMenuBarInUse(void);

/**
 ** SCROLLBARS
 **/

#define GUI_SB_DEFAULT_THICK 12

extern GrColor _scbbgcolor;
extern GrColor _scbliftcolor;

void _GUIScrollbarInit(void);
void _GUIScrollbarEnd(void);

/**
 ** TILES
 **/

extern GrColor _tileslcolor;
extern GrColor _tilesbcolor;
extern GrColor _tilessbcolor;

void _GUITilesInit(void);
void _GUITilesEnd(void);
void _GUITilesDisable(void);
void _GUITilesEnable(void);
int _GUITilesHookEvent(GrEvent *ev);

/**
 ** DIALOGS
 **/

extern int _dlgtitlechrtype;
extern GrFont *_dlgtitlefont;
extern int _dlgtitlefontloaded;
extern GrColor _dlglinecolor;
extern GrColor _dlgbordercolor;
extern GrColor _dlgfgcolor;
extern GrColor _dlgbgcolor;

void _GUIDialogInit(void);
void _GUIDialogEnd(void);
int _GUIDialogRunning(void);

/* Common dialogs */

extern int _cdlgchrtype;
extern GrFont *_cdlgfont;
extern int _dlgfontloaded;
extern GrColor _cdlgbtbgcolor;
extern GrColor _cdlgbtfgcolor;

void _GUICDialogInit(void);
void _GUICDialogEnd(void);

/**
 ** OBJECTS
 **/

extern int _objectchrtype;
extern GrFont *_objectfont;
extern int _objectfontloaded;
extern GrColor _objectlcolor;
extern GrColor _objectshcolor1;
extern GrColor _objectshcolor2;
extern GrTextOption _objectgenopt; // generic
extern GrTextOption _objectbutopt; // buttons
extern GrTextOption _objectentopt; // entries

void _GUIObjectInit(void);
void _GUIObjectEnd(void);
void _GUIObjectPaint(GUIObject *o, int x, int y);
int _GUIObjectProcessEvent(GUIGroup *g, GUIObject *o, GrEvent *ev);
void _GUIObjectSetOn(GUIGroup *g, GUIObject *o, int paint);
void _GUIObjectDestroy(GUIObject *o);
void *_GUIObjectGetText(GUIObject *o, int chrtype);
void _GUIObjectSetText(GUIObject *o, void *newtext);

void _GUIOLabelPaint(GUIObject *o, int x, int y);
void _GUIOTextDestroy(GUIObject *o);
void _GUIOTextPaint(GUIObject *o, int x, int y);
void _GUIOLightPaint(GUIObject *o, int x, int y);
void _GUIOLightSetOn(GUIGroup *g, GUIObject *o, int paint);
void _GUIOButtonPaint(GUIObject *o, int x, int y);
int _GUIOButtonProcessEvent(GUIGroup *g, GUIObject *o, GrEvent *ev);
void _GUIOButtonSetOn(GUIGroup *g, GUIObject *o, int paint);
void _GUIOEntryDestroy(GUIObject *o);
void *_GUIOEntryGetText(GUIObject *o, int chrtype);
void _GUIOEntrySetText(GUIObject *o, void *newtext);
void _GUIOEntryPaint(GUIObject *o, int x, int y);
int _GUIOEntryProcessEvent(GUIGroup *g, GUIObject *o, GrEvent *ev);
void _GUIOListDestroy(GUIObject *o);
void _GUIOListPaint(GUIObject *o, int x, int y);
int _GUIOListProcessEvent(GUIGroup *g, GUIObject *o, GrEvent *ev);

/**
 ** GROUPS of objects
 **/

#endif /*__GRGUIP_H_INCLUDED__*/
