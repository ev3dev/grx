/**
 ** LIBXWIN.H - GRX library X Windows private include file
 **
 ** Author:        Ulrich Leodolter
 ** E-mail:        ulrich@lab1.psy.univie.ac.at
 ** Date:        Thu Sep 28 09:46:21 1995
 ** RCSId:        $Id$
 **/
#ifndef _LIBXWIN_H_
#define _LIBXWIN_H_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>
#include <unistd.h>

extern Display *        _XGrDisplay;
extern int                _XGrScreen;
extern Colormap                _XGrColormap;
extern Window                _XGrWindow;
extern Pixmap                _XGrBitmap;
extern Pixmap                _XGrPattern;
extern GC                _XGrGC;
extern GC                _XGrBitmapGC;
extern GC                _XGrPatternGC;
extern long                _XGrForeColor;
extern long                _XGrBackColor;
extern unsigned int        _XGrColorOper;
extern unsigned int        _XGrDepth;
extern unsigned int        _XGrBitsPerPixel;

extern int                _XGrKeyboardHit(void);
extern int                _XGrKeyboardGetKey(void);
extern int                _XGrKeyboardGetState(void);

#endif
