/**
 ** libxwin.h - GRX library X Windows private include file
 **
 ** Author:     Ulrich Leodolter
 ** E-mail:     ulrich@lab1.psy.univie.ac.at
 ** Date:       Thu Sep 28 09:46:21 1995
 ** RCSId:      $Id$
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

#ifndef _LIBXWIN_H_
#define _LIBXWIN_H_

#include <X11/Xproto.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>


/*
  Invoke pre-X11R6 ICCCM routines if XlibSpecificationRelease is not 6.
*/
#if XlibSpecificationRelease-0 < 6
#define PRE_R6_ICCCM
#endif
/*
  Invoke pre-X11R5 ICCCM routines if XlibSpecificationRelease is not defined.
*/
#if !defined(XlibSpecificationRelease)
#define PRE_R5_ICCCM
#endif
/*
  Invoke pre-X11R4 ICCCM routines if PWinGravity is not defined.
*/
#if !defined(PWinGravity)
#define PRE_R4_ICCCM
#endif

#define Export
#include "prex11r6.h"

extern Display *        _XGrDisplay;
extern int              _XGrScreen;
extern Colormap         _XGrColormap;
extern Window           _XGrWindow;
extern Pixmap           _XGrBitmap;
extern Pixmap           _XGrPattern;
extern GC               _XGrGC;
extern GC               _XGrBitmapGC;
extern GC               _XGrPatternGC;
extern unsigned long    _XGrForeColor;
extern unsigned long    _XGrBackColor;
extern unsigned int     _XGrColorOper;
extern unsigned int     _XGrDepth;
extern unsigned int     _XGrBitsPerPixel;

extern unsigned long   _XGrColorPlanes[8];
extern unsigned int    _XGrColorNumPlanes;
extern unsigned long   _XGrColorPixels[2];
extern unsigned int    _XGrColorNumPixels;

extern int              _XGrKeyboardHit(void);
extern int              _XGrKeyboardGetKey(void);
extern int              _XGrKeyboardGetState(void);

#endif
