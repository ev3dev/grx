/**
 ** libwin32.h - GRX library Win32-API private include file
 **
 ** Author:        Gernot Graeff
 ** E-mail:        gernot.graeff@t-online.de
 ** Date:        13.11.98
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

#ifndef _LIBWIN32_H_
#define _LIBWIN32_H_

#include <windows.h>
#include "grx20.h"

typedef struct _EvQueue {
        GrMouseEvent        *pEvent;
        struct _EvQueue        *pNext;
} SEventQueue;

struct _ColorList {
        int               nIndex;
        COLORREF          color;
        struct _ColorList *pNext;
};
typedef struct _ColorList SColorList;

extern SEventQueue                *pEventQueue;
extern CRITICAL_SECTION                csEventQueue;
extern SColorList                *ColorList;
extern HBITMAP                        hBitmapScreen;
extern HWND                        hGRXWnd;
extern HDC                        hDCMem;

extern int GRXMain ( int, char**, char** );

#endif

