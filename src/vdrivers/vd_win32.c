/**
 ** vd_win32.c ---- the standard Win32-API driver
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
 ** Contributions by Josu Onandia (jonandia@fagorautomation.es) 21/02/2001
 **   - The colors loaded in the ColorList are guaranteed to be actually used
 **     by Windows (GetNearestColor), for the GR_frameWin32 to work.
 **   - When the window is created, it gets the maximum size allowed by the
 **     current mode. Indeed this size is stored (maxWindowWidth/
 **     maxWindowHeight).
 **     When the window is going to be resized (WM_GETMINMAXINFO) it's not
 **     allowed to grow bigger than this maximum size (it makes nosense).
 **   - Added some modes for 24bpp colors.
 **   - When changed to text-mode, the graphics window is hidden. If the
 **     application has a console (linked with -mconsole) it can use
 **     printf/scanf and the like.
 **     When changed again into graphics mode, the window reappears.
 **   - Inter-task synchronization. In some cases the two threads are
 **     manipulating at the same time the main window, and the on-memory
 **     bitmap. I guess this is causing trouble, so in some cases the main
 **     thread suspends the worker thread, make its operation, and then
 **     resumes it.
 **   - The window title is selectable with a define, at compile time.
 **     If not defined, it defaults to "GRX".
 **/

#include "libwin32.h"
#include "libgrx.h"
#include "grdriver.h"
#include "arith.h"
#include "grxkeys.h"

#ifndef GRXWINDOW_TITLE
#define GRXWINDOW_TITLE "GRX"
#endif

HINSTANCE          hGlobInst;
HWND               hGRXWnd = NULL;
HDC                                        hDCMem = NULL;
SEventQueue        *pEventQueue;
CRITICAL_SECTION   csEventQueue;
SColorList         *ColorList;
HBITMAP            hBitmapScreen;

static int maxWindowWidth, maxWindowHeight;
// Identifier of the main thread
static DWORD mainThreadId;
// Handle of the worker thread, which is executing GRXMain
HANDLE hThread;

static LRESULT CALLBACK WndProc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

// NOTE: Adjusts the color to the real RGB used by Windows
static void loadcolor(int c,int r,int g,int b)
{
        SColorList *clTmp, *clPrev;
        HDC hdcW;
        COLORREF color;

        hdcW = GetDC ( hGRXWnd );
        color = RGB(r, g, b);
        color = GetNearestColor(hdcW, color);
        ReleaseDC ( hGRXWnd, hdcW );
        if ( ColorList == NULL ) {
                clTmp = malloc ( sizeof ( SColorList ) );
                clTmp -> nIndex = c;
                clTmp -> color = color;
                clTmp -> pNext = NULL;
                ColorList = clTmp;
                return;
        }
        else {
                clTmp = ColorList;
                do {
                        if ( clTmp -> nIndex == c ) {
                                clTmp -> color = color;
                                return;
                        }
                        clPrev = clTmp;
                        clTmp = clTmp -> pNext;
                } while ( clTmp );
                clPrev -> pNext = malloc ( sizeof ( SColorList ) );
                clTmp = clPrev -> pNext;
                clTmp -> nIndex = c;
                clTmp -> color = color;
                clTmp -> pNext = NULL;
                return;
        }
}

static void ColorList_destroy(void)
{
        SColorList *clTmp;

        while ( ColorList != NULL ) {
                clTmp = ColorList;
                ColorList = ColorList -> pNext;
                free ( clTmp );
        }
}

static int setmode(GrVideoMode *mp,int noclear)
{
        RECT    Rect;
        HDC     hDC;
        HBRUSH        hBrush;

        if ( mp -> extinfo -> mode != GR_frameText )
        {
                if(hGRXWnd == NULL)
                {
                        hGRXWnd = CreateWindow (
                                "GRXCLASS",
                                GRXWINDOW_TITLE,
                //                WS_OVERLAPPEDWINDOW,        We don't need MaximizeBox
                                WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,
                                0, 0,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                NULL,
                                NULL,
                                hGlobInst,
                                NULL
                                );
                }
                Rect.left = 50;
                Rect.top = 50;
                Rect.right = mp->width + 50;
                Rect.bottom = mp->height + 50;
                AdjustWindowRect ( &Rect, WS_OVERLAPPEDWINDOW, FALSE );
                maxWindowWidth = Rect.right - Rect.left;
                maxWindowHeight = Rect.bottom - Rect.top;
                SetWindowPos (
                        hGRXWnd, NULL,
                        Rect.left, Rect.top,
                        maxWindowWidth, maxWindowHeight,
                        SWP_DRAWFRAME | SWP_NOZORDER | SWP_SHOWWINDOW
                );

                if(hBitmapScreen != NULL)
                {
                        DeleteObject(hBitmapScreen);
                        hBitmapScreen = NULL;
                }
                hDC = GetDC ( hGRXWnd );
                if(hDCMem == NULL)
                        hDCMem = CreateCompatibleDC ( hDC );
                hBitmapScreen = CreateCompatibleBitmap ( hDC, mp->width, mp->height );
                SelectObject ( hDCMem, hBitmapScreen );
                SetRect ( &Rect, 0, 0, mp->width, mp->height );
                hBrush = CreateSolidBrush ( RGB ( 0, 0, 0 ) );
                FillRect ( hDCMem, &Rect, hBrush );
                ReleaseDC ( hGRXWnd, hDC );
                DeleteObject ( hBrush );

                UpdateWindow(hGRXWnd);

                ColorList_destroy();
        }
        else {
                // If changing to text-mode, hide the graphics window.
                if(hGRXWnd != NULL)
                        ShowWindow(hGRXWnd, SW_HIDE);
        }
        return (TRUE);
}


static int detect(void)
{
        return TRUE;
}

static void setbank_dummy(int bk) { bk=bk; }


GrVideoModeExt grtextext = {
    GR_frameText,                        /* frame driver */
    NULL,                                /* frame driver override */
    NULL,                                /* frame buffer address */
    { 0, 0, 0 },                        /* color precisions */
    { 0, 0, 0 },                        /* color component bit positions */
    0,                                        /* mode flag bits */
    setmode,                                /* mode set */
    NULL,                                /* virtual size set */
    NULL,                                /* virtual scroll */
    NULL,                                /* bank set function */
    NULL,                                /* double bank set function */
    NULL                                /* color loader */
};

static GrVideoModeExt grxwinext8 = {
  GR_frameWIN32_8,                        /* frame driver */
  NULL,                                        /* frame driver override */
  NULL,                                        /* frame buffer address */
  { 8, 8, 8 },                                /* color precisions */
  { 0, 8, 16 },                                /* color component bit positions */
  0,                                        /* mode flag bits */
  setmode,                                /* mode set */
  NULL,                                        /* virtual size set */
  NULL,                                        /* virtual scroll */
  setbank_dummy,                        /* bank set function */
  NULL,                                 /* double bank set function */
  loadcolor                                /* color loader */
};

static GrVideoModeExt grxwinext24 = {
  GR_frameWIN32_24,                        /* frame driver */
  NULL,                                        /* frame driver override */
  NULL,                                        /* frame buffer address */
  { 8, 8, 8 },                                /* color precisions */
  { 0, 8, 16 },                                /* color component bit positions */
  0,                                        /* mode flag bits */
  setmode,                                /* mode set */
  NULL,                                        /* virtual size set */
  NULL,                                        /* virtual scroll */
  setbank_dummy,                        /* bank set function */
  NULL,                                 /* double bank set function */
  NULL                                  /* color loader */
};


static GrVideoMode modes[] = {
  /* pres.  bpp wdt   hgt   BIOS   scan  priv. &ext  */
  {  TRUE,  8,   80,   25,  0x00,    80, 1,    &grtextext  },

  {  TRUE,  8,  320,  240,  0x00,   320, 0,    &grxwinext8  },
  {  TRUE,  8,  640,  480,  0x00,   640, 0,    &grxwinext8  },
  {  TRUE,  8,  800,  600,  0x00,   800, 0,    &grxwinext8  },
  {  TRUE,  8, 1024,  768,  0x00,  1024, 0,    &grxwinext8  },
  {  TRUE,  8, 1280, 1024,  0x00,  1280, 0,    &grxwinext8  },

  {  TRUE,  24,  640,  480,  0x00,    0, 0,    &grxwinext24  },
  {  TRUE,  24,  800,  600,  0x00,    0, 0,    &grxwinext24  },
  {  TRUE,  24, 1024,  768,  0x00,    0, 0,    &grxwinext24  },
  {  TRUE,  24, 1280, 1024,  0x00,    0, 0,    &grxwinext24  },
};

// The modes not compatible width the configuration of Windows are
// made 'non-present'
static int init(char *options)
{
        int size, i;
        
        size = GetSystemMetrics(SM_CXSCREEN);
        for(i=1; i < itemsof(modes); i++)
        {
                if(modes[i].width > size)
                        modes[i].present = FALSE;
        }
        size = GetSystemMetrics(SM_CYSCREEN);
        for(i=1; i < itemsof(modes); i++)
        {
                if(modes[i].height > size)
                        modes[i].present = FALSE;
        }
        return TRUE;
}


static void reset(void)
{
        ColorList_destroy();
        if(hGRXWnd != NULL)
                ShowWindow(hGRXWnd, SW_HIDE);
}


/* Note: _GrViDrvEGAVGAtextModeExt is referenced in setup/drvinfo.c */
#define grtextext _GrViDrvEGAVGAtextModeExt

GrVideoDriver _GrVideoDriverWIN32 = {
  "win32",                                /* name */
  GR_WIN32,                                /* adapter type */
  NULL,                                        /* inherit modes from this driver */
  modes,                                /* mode table */
  itemsof(modes),                        /* # of modes */
  detect,                                /* detection routine */
  init,                                        /* initialization routine */
  reset,                                /* reset routine */
  _gr_selectmode,
  0
};



DWORD WINAPI GRXLocalMain ( LPVOID pCmdLine )
{
        int     argc;
        char        **argv;
        char        **envp;
        char        *pszCmdLine;
        char        *psz;
        char        **ppargv;
        int     i;

        pszCmdLine = GetCommandLine ();
        psz = pszCmdLine;
        argc = 1;
        while ( *psz ) {
                if ( *psz == ' ' ) {
                        argc++;
                        while ( *psz == ' ' ) {
                                *psz = 0x00;
                                psz++;
                        }
                        if ( *psz == 0x00 ) {
                                argc--;
                                break;
                        }
                        psz--;
                }
                psz++;
        }
        psz = pszCmdLine;
        argv = malloc ( argc * sizeof ( char* ) );
        ppargv = argv;
        for ( i = 0; i < argc; i++ ) {
                *ppargv = _strdup ( psz );
                ppargv++;
                psz += strlen ( psz );
                while ( *psz == 0x00 ) {
                        psz++;
                }
        }
        envp = ( char** ) GetEnvironmentStrings ();
        Sleep ( 1000 );
        // Execute the main program
        GRXMain ( argc, argv, envp );
        // Send a message to the main thread
        PostThreadMessage(mainThreadId, WM_QUIT, 0, 0);
        return 0;
}


int WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                                        PSTR szCmdLine, int nCmdShow )
{
        WNDCLASSEX        wndclass;
        DWORD                tid;
        MSG                        msg;

        hGlobInst = hInstance;
        tid = 0;
        mainThreadId = GetCurrentThreadId();
        ColorList = NULL;
        hBitmapScreen = NULL;
        InitializeCriticalSection ( &csEventQueue );

        wndclass.cbSize = sizeof ( wndclass );
        wndclass.style = CS_HREDRAW | CS_VREDRAW;
        wndclass.lpfnWndProc = WndProc;
        wndclass.cbClsExtra = 0;
        wndclass.cbWndExtra = 0;
        wndclass.hInstance = hGlobInst;
        wndclass.hIcon = LoadIcon ( NULL, IDI_APPLICATION );
        wndclass.hCursor = LoadCursor ( NULL, IDC_ARROW );
        wndclass.hbrBackground = ( HBRUSH ) GetStockObject ( BLACK_BRUSH );
        wndclass.lpszMenuName = NULL;
        wndclass.lpszClassName = "GRXCLASS";
        wndclass.hIconSm = LoadIcon ( NULL, IDI_APPLICATION );
        RegisterClassEx ( &wndclass );


        pEventQueue = malloc ( sizeof ( SEventQueue ) );
        pEventQueue -> pEvent = NULL;
        pEventQueue -> pNext = NULL;

        GrSetMode ( GR_default_graphics );

        hThread = CreateThread ( NULL, 0, ( LPTHREAD_START_ROUTINE ) GRXLocalMain, 
                                                        ( LPVOID ) szCmdLine, 0, &tid );
        while ( GetMessage ( &msg, NULL, 0, 0 ) ) {
                TranslateMessage ( &msg );
                DispatchMessage ( &msg );
        }
        DeleteCriticalSection ( &csEventQueue );
        // Before terminating, stop the worker thread. This way it wont access
        // the bitmap or the window while they are being destroyed.
        SuspendThread(hThread);
        
        return 0;
}


static LRESULT CALLBACK WndProc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
        static int cursorOn = 1;
        switch ( uMsg ) {

        case WM_NCHITTEST:
                {
                        LRESULT res = DefWindowProc ( hWnd, uMsg, wParam, lParam );
                        if(res == HTCLIENT)
                        {
                                if(cursorOn)
                                {
                                        ShowCursor(FALSE);
                                        cursorOn = 0;
                                }
                        }
                        else
                        {
                                if(!cursorOn)
                                {
                                        ShowCursor(TRUE);
                                        cursorOn = 1;
                                }
                        }
                        return res;
                }
        case WM_CLOSE:
                // While terminating stop the worker thread
                SuspendThread(hThread);
                hGRXWnd = NULL;
                PostQuitMessage(0);
                break;
        case WM_DESTROY:
                if(hBitmapScreen != NULL)
                {
                        DeleteObject(hBitmapScreen);
                        hBitmapScreen = NULL;
                }
                DeleteDC ( hDCMem );
                break;
        case WM_GETMINMAXINFO :
                {
                        LPMINMAXINFO lpmmi = (LPMINMAXINFO) lParam;

                        lpmmi->ptMaxSize.x =
                        lpmmi->ptMaxTrackSize.x = maxWindowWidth;
                        
                        lpmmi->ptMaxSize.y =
                        lpmmi->ptMaxTrackSize.y = maxWindowHeight;
                }
                return(0);
                
        case WM_CHAR :
                {
                        SEventQueue        *pEvQ, *pEv;
                        ULONG                fkbState;
                        char                chKey;
                        BOOL                fInsert = TRUE;

                        EnterCriticalSection ( &csEventQueue );
                        chKey = wParam;
                        pEv = malloc ( sizeof ( SEventQueue ) );
                        pEv -> pEvent = malloc ( sizeof ( GrMouseEvent ) );
                        pEv -> pEvent -> flags = GR_M_KEYPRESS;
                        pEv -> pEvent -> key = wParam;
                        fkbState = 0;
                        if ( GetKeyState ( VK_SHIFT ) < 0 ) {
                                fkbState |= GR_KB_SHIFT;
                        }
                        if ( GetKeyState ( VK_CONTROL ) < 0 ) {
                                fkbState |= GR_KB_CTRL;
                        }
                        if ( GetKeyState ( VK_MENU ) < 0 ) {
                                fkbState |= GR_KB_ALT;
                                fInsert = FALSE;
                        }
                        if ( GetKeyState ( VK_SCROLL ) < 0 ) {
                                fkbState |= GR_KB_SCROLLOCK;
                        }
                        if ( GetKeyState ( VK_NUMLOCK ) < 0 ) {
                                fkbState |= GR_KB_NUMLOCK;
                        }
                        if ( GetKeyState ( VK_CAPITAL ) < 0 ) {
                                fkbState |= GR_KB_CAPSLOCK;
                        }
                        if ( GetKeyState ( VK_INSERT ) < 0 ) {
                                fkbState |= GR_KB_INSERT;
                        }
                        pEv -> pEvent -> kbstat = fkbState;
                        pEv -> pNext = NULL;
                        if ( fInsert ) {
                                pEvQ = pEventQueue;
                                while ( pEvQ -> pNext != NULL ) {
                                        pEvQ = pEvQ -> pNext;
                                }
                                pEvQ -> pNext = pEv;
                        }
                        else {
                                free ( pEv -> pEvent );
                                free ( pEv );
                        }
                        LeaveCriticalSection ( &csEventQueue );
                }
                return 0;
        
        case WM_SYSCHAR :
                {
                        SEventQueue        *pEvQ, *pEv;
                        ULONG                fkbState;
                        char                chKey;
                        BOOL                fInsert = FALSE;

                        EnterCriticalSection ( &csEventQueue );
                        chKey = wParam;
                        pEv = malloc ( sizeof ( SEventQueue ) );
                        pEv -> pEvent = malloc ( sizeof ( GrMouseEvent ) );
                        pEv -> pEvent -> flags = GR_M_KEYPRESS;
                        pEv -> pEvent -> key = wParam;
                        fkbState = 0;
                        if ( GetKeyState ( VK_SHIFT ) < 0 ) {
                                fkbState |= GR_KB_SHIFT;
                        }
                        if ( GetKeyState ( VK_CONTROL ) < 0 ) {
                                fkbState |= GR_KB_CTRL;
                        }
                        if ( GetKeyState ( VK_MENU ) < 0 ) {
                                fkbState |= GR_KB_ALT;
                                switch ( chKey ) {
                                        
                                case 0x61 :
                                        pEv -> pEvent -> key = GrKey_Alt_A;
                                        fInsert = TRUE;
                                        break;

                                case 0x62 :
                                        pEv -> pEvent -> key = GrKey_Alt_B;
                                        fInsert = TRUE;
                                        break;

                                case 0x63 :
                                        pEv -> pEvent -> key = GrKey_Alt_C;
                                        fInsert = TRUE;
                                        break;

                                case 0x64 :
                                        pEv -> pEvent -> key = GrKey_Alt_D;
                                        fInsert = TRUE;
                                        break;

                                case 0x65 :
                                        pEv -> pEvent -> key = GrKey_Alt_E;
                                        fInsert = TRUE;
                                        break;

                                case 0x66 :
                                        pEv -> pEvent -> key = GrKey_Alt_F;
                                        fInsert = TRUE;
                                        break;

                                case 0x67 :
                                        pEv -> pEvent -> key = GrKey_Alt_G;
                                        fInsert = TRUE;
                                        break;

                                case 0x68 :
                                        pEv -> pEvent -> key = GrKey_Alt_H;
                                        fInsert = TRUE;
                                        break;

                                case 0x69 :
                                        pEv -> pEvent -> key = GrKey_Alt_I;
                                        fInsert = TRUE;
                                        break;

                                case 0x6a :
                                        pEv -> pEvent -> key = GrKey_Alt_J;
                                        fInsert = TRUE;
                                        break;

                                case 0x6b :
                                        pEv -> pEvent -> key = GrKey_Alt_K;
                                        fInsert = TRUE;
                                        break;

                                case 0x6c :
                                        pEv -> pEvent -> key = GrKey_Alt_L;
                                        fInsert = TRUE;
                                        break;

                                case 0x6d :
                                        pEv -> pEvent -> key = GrKey_Alt_M;
                                        fInsert = TRUE;
                                        break;

                                case 0x6e :
                                        pEv -> pEvent -> key = GrKey_Alt_N;
                                        fInsert = TRUE;
                                        break;

                                case 0x6f :
                                        pEv -> pEvent -> key = GrKey_Alt_O;
                                        fInsert = TRUE;
                                        break;

                                case 0x70 :
                                        pEv -> pEvent -> key = GrKey_Alt_P;
                                        fInsert = TRUE;
                                        break;

                                case 0x71 :
                                        pEv -> pEvent -> key = GrKey_Alt_Q;
                                        fInsert = TRUE;
                                        break;

                                case 0x72 :
                                        pEv -> pEvent -> key = GrKey_Alt_R;
                                        fInsert = TRUE;
                                        break;

                                case 0x73 :
                                        pEv -> pEvent -> key = GrKey_Alt_S;
                                        fInsert = TRUE;
                                        break;

                                case 0x74 :
                                        pEv -> pEvent -> key = GrKey_Alt_T;
                                        fInsert = TRUE;
                                        break;

                                case 0x75 :
                                        pEv -> pEvent -> key = GrKey_Alt_U;
                                        fInsert = TRUE;
                                        break;

                                case 0x76 :
                                        pEv -> pEvent -> key = GrKey_Alt_V;
                                        fInsert = TRUE;
                                        break;

                                case 0x77 :
                                        pEv -> pEvent -> key = GrKey_Alt_W;
                                        fInsert = TRUE;
                                        break;

                                case 0x78 :
                                        pEv -> pEvent -> key = GrKey_Alt_X;
                                        fInsert = TRUE;
                                        break;

                                case 0x79 :
                                        pEv -> pEvent -> key = GrKey_Alt_Y;
                                        fInsert = TRUE;
                                        break;

                                case 0x7a :
                                        pEv -> pEvent -> key = GrKey_Alt_Z;
                                        fInsert = TRUE;
                                        break;

                                }
                        }
                        if ( GetKeyState ( VK_SCROLL ) < 0 ) {
                                fkbState |= GR_KB_SCROLLOCK;
                        }
                        if ( GetKeyState ( VK_NUMLOCK ) < 0 ) {
                                fkbState |= GR_KB_NUMLOCK;
                        }
                        if ( GetKeyState ( VK_CAPITAL ) < 0 ) {
                                fkbState |= GR_KB_CAPSLOCK;
                        }
                        if ( GetKeyState ( VK_INSERT ) < 0 ) {
                                fkbState |= GR_KB_INSERT;
                        }
                        pEv -> pEvent -> kbstat = fkbState;
                        pEv -> pNext = NULL;
                        if ( fInsert ) {
                                pEvQ = pEventQueue;
                                while ( pEvQ -> pNext != NULL ) {
                                        pEvQ = pEvQ -> pNext;
                                }
                                pEvQ -> pNext = pEv;
                        }
                        else {
                                free ( pEv -> pEvent );
                                free ( pEv );
                        }
                        LeaveCriticalSection ( &csEventQueue );
                }
                return 0;
        
        case WM_KEYDOWN :
                {
                        SEventQueue        *pEvQ, *pEv;
                        ULONG                fkbState;
                        char                chKey;
                        BOOL                fInsert = FALSE;

                        EnterCriticalSection ( &csEventQueue );
                        chKey = wParam;
                        pEv = malloc ( sizeof ( SEventQueue ) );
                        pEv -> pEvent = malloc ( sizeof ( GrMouseEvent ) );
                        pEv -> pEvent -> flags = GR_M_KEYPRESS;
                        switch ( chKey ) {
/*
                        case VK_SHIFT :
                                pEv -> pEvent -> key = GrKey_Shift;
                                fInsert = TRUE;
                                break;
*/
                        case VK_LEFT :
                                pEv -> pEvent -> key = GrKey_Left;
                                fInsert = TRUE;
                                break;

                        case VK_RIGHT :
                                pEv -> pEvent -> key = GrKey_Right;
                                fInsert = TRUE;
                                break;

                        case VK_UP :
                                pEv -> pEvent -> key = GrKey_Up;
                                fInsert = TRUE;
                                break;

                        case VK_DOWN :
                                pEv -> pEvent -> key = GrKey_Down;
                                fInsert = TRUE;
                                break;

                        case VK_F1 :
                                pEv -> pEvent -> key = GrKey_F1;
                                fInsert = TRUE;
                                break;

                        case VK_F2 :
                                pEv -> pEvent -> key = GrKey_F2;
                                fInsert = TRUE;
                                break;

                        case VK_F3 :
                                pEv -> pEvent -> key = GrKey_F3;
                                fInsert = TRUE;
                                break;

                        case VK_F4 :
                                pEv -> pEvent -> key = GrKey_F4;
                                fInsert = TRUE;
                                break;

                        case VK_F5 :
                                pEv -> pEvent -> key = GrKey_F5;
                                fInsert = TRUE;
                                break;

                        case VK_F6 :
                                pEv -> pEvent -> key = GrKey_F6;
                                fInsert = TRUE;
                                break;

                        case VK_F7 :
                                pEv -> pEvent -> key = GrKey_F7;
                                fInsert = TRUE;
                                break;

                        case VK_F8 :
                                pEv -> pEvent -> key = GrKey_F8;
                                fInsert = TRUE;
                                break;

                        case VK_F9 :
                                pEv -> pEvent -> key = GrKey_F9;
                                fInsert = TRUE;
                                break;

                        }
                        fkbState = 0;
                        if ( GetKeyState ( VK_SHIFT ) < 0 ) {
                                fkbState |= GR_KB_SHIFT;
                        }
                        if ( GetKeyState ( VK_CONTROL ) < 0 ) {
                                fkbState |= GR_KB_CTRL;
                        }
                        if ( GetKeyState ( VK_MENU ) < 0 ) {
                                fkbState |= GR_KB_ALT;
                        }
                        if ( GetKeyState ( VK_SCROLL ) < 0 ) {
                                fkbState |= GR_KB_SCROLLOCK;
                        }
                        if ( GetKeyState ( VK_NUMLOCK ) < 0 ) {
                                fkbState |= GR_KB_NUMLOCK;
                        }
                        if ( GetKeyState ( VK_CAPITAL ) < 0 ) {
                                fkbState |= GR_KB_CAPSLOCK;
                        }
                        if ( GetKeyState ( VK_INSERT ) < 0 ) {
                                fkbState |= GR_KB_INSERT;
                        }
                        pEv -> pEvent -> kbstat = fkbState;
                        pEv -> pNext = NULL;
                        if ( fInsert ) {
                                pEvQ = pEventQueue;
                                while ( pEvQ -> pNext != NULL ) {
                                        pEvQ = pEvQ -> pNext;
                                }
                                pEvQ -> pNext = pEv;
                        }
                        else {
                                free ( pEv -> pEvent );
                                free ( pEv );
                        }
                        LeaveCriticalSection ( &csEventQueue );
                }
                return 0;
        
        case WM_SYSKEYDOWN :
                {
                        SEventQueue        *pEvQ, *pEv;
                        ULONG                fkbState;
                        char                chKey;
                        BOOL                fInsert = FALSE;

                        EnterCriticalSection ( &csEventQueue );
                        chKey = wParam;
                        pEv = malloc ( sizeof ( SEventQueue ) );
                        pEv -> pEvent = malloc ( sizeof ( GrMouseEvent ) );
                        pEv -> pEvent -> flags = GR_M_KEYPRESS;
                        switch ( chKey ) {

                        case VK_F4 :
                                pEv -> pEvent -> key = GrKey_F4;
                                if ( GetKeyState ( VK_MENU ) < 0 ) {
                                        PostMessage ( hWnd, WM_DESTROY, 0, 0 );
                                }
                                break;

                        case VK_F10 :
                                pEv -> pEvent -> key = GrKey_F10;
                                fInsert = TRUE;
                                break;

                        }
                        fkbState = 0;
                        if ( GetKeyState ( VK_SHIFT ) < 0 ) {
                                fkbState |= GR_KB_SHIFT;
                        }
                        if ( GetKeyState ( VK_CONTROL ) < 0 ) {
                                fkbState |= GR_KB_CTRL;
                        }
                        if ( GetKeyState ( VK_MENU ) < 0 ) {
                                fkbState |= GR_KB_ALT;
                        }
                        if ( GetKeyState ( VK_SCROLL ) < 0 ) {
                                fkbState |= GR_KB_SCROLLOCK;
                        }
                        if ( GetKeyState ( VK_NUMLOCK ) < 0 ) {
                                fkbState |= GR_KB_NUMLOCK;
                        }
                        if ( GetKeyState ( VK_CAPITAL ) < 0 ) {
                                fkbState |= GR_KB_CAPSLOCK;
                        }
                        if ( GetKeyState ( VK_INSERT ) < 0 ) {
                                fkbState |= GR_KB_INSERT;
                        }
                        pEv -> pEvent -> kbstat = fkbState;
                        pEv -> pNext = NULL;
                        if ( fInsert ) {
                                pEvQ = pEventQueue;
                                while ( pEvQ -> pNext != NULL ) {
                                        pEvQ = pEvQ -> pNext;
                                }
                                pEvQ -> pNext = pEv;
                        }
                        else {
                                free ( pEv -> pEvent );
                                free ( pEv );
                        }
                        LeaveCriticalSection ( &csEventQueue );
                }
                return 0;
        
        case WM_LBUTTONDOWN :
                {
                        SEventQueue        *pEvQ, *pEv;
                        
                        EnterCriticalSection ( &csEventQueue );
                        pEv = malloc ( sizeof ( SEventQueue ) );
                        pEv -> pEvent = malloc ( sizeof ( GrMouseEvent ) );
                        pEv -> pEvent -> flags = GR_M_LEFT_DOWN;
                        pEv -> pEvent -> x = LOWORD ( lParam );
                        pEv -> pEvent -> y = HIWORD ( lParam );
                        pEv -> pNext = NULL;
                        pEvQ = pEventQueue;
                        while ( pEvQ -> pNext != NULL ) {
                                pEvQ = pEvQ -> pNext;
                        }
                        pEvQ -> pNext = pEv;
                        LeaveCriticalSection ( &csEventQueue );
                }
                return 0;

        case WM_MBUTTONDOWN :
                {
                        SEventQueue        *pEvQ, *pEv;

                        EnterCriticalSection ( &csEventQueue );
                        pEv = malloc ( sizeof ( SEventQueue ) );
                        pEv -> pEvent = malloc ( sizeof ( GrMouseEvent ) );
                        pEv -> pEvent -> flags = GR_M_MIDDLE_DOWN;
                        pEv -> pEvent -> x = LOWORD ( lParam );
                        pEv -> pEvent -> y = HIWORD ( lParam );
                        pEv -> pNext = NULL;
                        pEvQ = pEventQueue;
                        while ( pEvQ -> pNext != NULL ) {
                                pEvQ = pEvQ -> pNext;
                        }
                        pEvQ -> pNext = pEv;
                        LeaveCriticalSection ( &csEventQueue );
                }
                return 0;

        case WM_RBUTTONDOWN :
                {
                        SEventQueue        *pEvQ, *pEv;

                        EnterCriticalSection ( &csEventQueue );
                        pEv = malloc ( sizeof ( SEventQueue ) );
                        pEv -> pEvent = malloc ( sizeof ( GrMouseEvent ) );
                        pEv -> pEvent -> flags = GR_M_RIGHT_DOWN;
                        pEv -> pEvent -> x = LOWORD ( lParam );
                        pEv -> pEvent -> y = HIWORD ( lParam );
                        pEv -> pNext = NULL;
                        pEvQ = pEventQueue;
                        while ( pEvQ -> pNext != NULL ) {
                                pEvQ = pEvQ -> pNext;
                        }
                        pEvQ -> pNext = pEv;
                        LeaveCriticalSection ( &csEventQueue );
                }
                return 0;

        case WM_LBUTTONUP :
                {
                        SEventQueue        *pEvQ, *pEv;

                        EnterCriticalSection ( &csEventQueue );
                        pEv = malloc ( sizeof ( SEventQueue ) );
                        pEv -> pEvent = malloc ( sizeof ( GrMouseEvent ) );
                        pEv -> pEvent -> flags = GR_M_LEFT_UP;
                        pEv -> pEvent -> x = LOWORD ( lParam );
                        pEv -> pEvent -> y = HIWORD ( lParam );
                        pEv -> pNext = NULL;
                        pEvQ = pEventQueue;
                        while ( pEvQ -> pNext != NULL ) {
                                pEvQ = pEvQ -> pNext;
                        }
                        pEvQ -> pNext = pEv;
                        LeaveCriticalSection ( &csEventQueue );
                }
                return 0;

        case WM_MBUTTONUP :
                {
                        SEventQueue        *pEvQ, *pEv;

                        EnterCriticalSection ( &csEventQueue );
                        pEv = malloc ( sizeof ( SEventQueue ) );
                        pEv -> pEvent = malloc ( sizeof ( GrMouseEvent ) );
                        pEv -> pEvent -> flags = GR_M_MIDDLE_UP;
                        pEv -> pEvent -> x = LOWORD ( lParam );
                        pEv -> pEvent -> y = HIWORD ( lParam );
                        pEv -> pNext = NULL;
                        pEvQ = pEventQueue;
                        while ( pEvQ -> pNext != NULL ) {
                                pEvQ = pEvQ -> pNext;
                        }
                        pEvQ -> pNext = pEv;
                        LeaveCriticalSection ( &csEventQueue );
                }
                return 0;

        case WM_RBUTTONUP :
                {
                        SEventQueue        *pEvQ, *pEv;

                        EnterCriticalSection ( &csEventQueue );
                        pEv = malloc ( sizeof ( SEventQueue ) );
                        pEv -> pEvent = malloc ( sizeof ( GrMouseEvent ) );
                        pEv -> pEvent -> flags = GR_M_RIGHT_UP;
                        pEv -> pEvent -> x = LOWORD ( lParam );
                        pEv -> pEvent -> y = HIWORD ( lParam );
                        pEv -> pNext = NULL;
                        pEvQ = pEventQueue;
                        while ( pEvQ -> pNext != NULL ) {
                                pEvQ = pEvQ -> pNext;
                        }
                        pEvQ -> pNext = pEv;
                        LeaveCriticalSection ( &csEventQueue );
                }
                return 0;

        case WM_MOUSEMOVE :
                {
                        SEventQueue        *pEvQ, *pEv;
                        
                        EnterCriticalSection ( &csEventQueue );
                        pEv = malloc ( sizeof ( SEventQueue ) );
                        pEv -> pEvent = malloc ( sizeof ( GrMouseEvent ) );
                        pEv -> pEvent -> flags = GR_M_MOTION;
                        pEv -> pEvent -> x = LOWORD ( lParam );
                        pEv -> pEvent -> y = HIWORD ( lParam );
                        pEv -> pNext = NULL;
                        pEvQ = pEventQueue;
                        while ( pEvQ -> pNext != NULL ) {
                                pEvQ = pEvQ -> pNext;
                        }
                        pEvQ -> pNext = pEv;
                        LeaveCriticalSection ( &csEventQueue );
                }
                return 0;
        case WM_PAINT :
                {
                        RECT                UpdateRect;
                        HDC                        hDC;
                        PAINTSTRUCT        ps;

                        SuspendThread(hThread);
                        if ( GetUpdateRect ( hWnd, &UpdateRect, FALSE ) ) {
                                BeginPaint ( hWnd, &ps );
                                hDC = GetDC ( hWnd );
                                BitBlt (
                                        hDC,
                                        UpdateRect.left, UpdateRect.top,
                                        UpdateRect.right - UpdateRect.left + 1, 
                                        UpdateRect.bottom - UpdateRect.top + 1,
                                        hDCMem,
                                        UpdateRect.left, UpdateRect.top,
                                        SRCCOPY
                                        );
                                ReleaseDC ( hWnd, hDC );
                                EndPaint ( hWnd, &ps );
                        }
                        ResumeThread(hThread);
                }
                return 0;

/*
        case WM_SYSCOMMAND :
                return 0;
*/

        }
        switch ( uMsg ) {

        case WM_GETMINMAXINFO :
        case WM_NCCREATE :
        case WM_NCPAINT :
        case WM_NCHITTEST :
        case WM_NCMOUSEMOVE :
        case WM_PALETTEISCHANGING :
        case WM_ACTIVATEAPP :
        case WM_NCCALCSIZE :
        case WM_ACTIVATE :
        case WM_NCACTIVATE :
        case WM_SHOWWINDOW :
        case WM_WINDOWPOSCHANGING :
        case WM_GETTEXT :
        case WM_SETFOCUS :
        case WM_KILLFOCUS :
        case WM_GETICON :
        case WM_ERASEBKGND :
        case WM_QUERYNEWPALETTE :
        case WM_WINDOWPOSCHANGED :
        case WM_GETDLGCODE :
        case WM_MOVE :
//        case WM_SIZE :
        case WM_SETCURSOR :
        case WM_HELP :
        case WM_KEYUP :
        case WM_SYSKEYUP :
                break;
/*
        case WM_SIZE :
                {
                        SEventQueue        *pEvQ, *pEv;

                        EnterCriticalSection ( &csEventQueue );
                        pEv = malloc ( sizeof ( SEventQueue ) );
                        pEv -> pEvent = malloc ( sizeof ( GrMouseEvent ) );
                        pEv -> pEvent -> flags = GR_SIZE_CHANGED;
                        pEv -> pEvent -> x = LOWORD ( lParam );
                        pEv -> pEvent -> y = HIWORD ( lParam );
                        pEv -> pNext = NULL;
                        pEvQ = pEventQueue;
                        while ( pEvQ -> pNext != NULL ) {
                                pEvQ = pEvQ -> pNext;
                        }
                        pEvQ -> pNext = pEv;
                        LeaveCriticalSection ( &csEventQueue );
                }
                return 0;        
*/
        default :
                {
//                        char szMsg [ 255 ];
//                        sprintf ( szMsg, "Msg %x, wParam %d, lParam %d", uMsg, wParam, lParam );
//                        MessageBox ( NULL, szMsg, "Msg", MB_OK );
                }
                break;
        
        }
        
        return DefWindowProc ( hWnd, uMsg, wParam, lParam );
}

