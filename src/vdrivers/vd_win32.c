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
 **
 ** Contributions by M.Alvarez (malfer@teleline.es) 18/11/2001
 **   - Better keys handling using translation tables (w32keys.h).
 **
 ** Contributions by M.Alvarez (malfer@teleline.es) 02/01/2002
 **   - Go to full screen if the framemode dimensions are equal to
 **     the screen dimensions (setting the client start area at 0,0).
 **/

#include "libwin32.h"
#include "libgrx.h"
#include "grdriver.h"
#include "arith.h"
#include "grxkeys.h"
#include "w32keys.h"

#ifndef GRXWINDOW_TITLE
#define GRXWINDOW_TITLE "GRX"
#endif

HINSTANCE          hGlobInst;
HWND               hGRXWnd = NULL;
HDC                hDCMem = NULL;
SEventQueue        *pEventQueue;
CRITICAL_SECTION   csEventQueue;
SColorList         *ColorList;
HBITMAP            hBitmapScreen;

int _nkeysw32pool = 0;
int _keysw32pool[_MAXKEYSW32POOL];

static int maxScreenWidth, maxScreenHeight;
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
        int     inipos;

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
                inipos = 50;
                if( mp->width == maxScreenWidth &&
                    mp->height == maxScreenHeight ) inipos = 0;
                Rect.left = inipos;
                Rect.top = inipos;
                Rect.right = mp->width + inipos;
                Rect.bottom = mp->height + inipos;
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
        int i;
        
        maxScreenWidth = GetSystemMetrics(SM_CXSCREEN);
        for(i=1; i < itemsof(modes); i++)
        {
                if(modes[i].width > maxScreenWidth)
                        modes[i].present = FALSE;
        }
        maxScreenHeight = GetSystemMetrics(SM_CYSCREEN);
        for(i=1; i < itemsof(modes); i++)
        {
                if(modes[i].height > maxScreenHeight)
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
        WNDCLASSEX wndclass;
        DWORD      tid;
        MSG        msg;

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

static ULONG convertwin32keystate( void )
{
        ULONG fkbState = 0;

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
        return fkbState;
}

static GrKeyType stdkeytranslate( int winkey, ULONG fkbState )
{
        keytrans *k;
        int i;

        if ( fkbState & GR_KB_ALT )
                k = altstdkeys;
        else if ( fkbState & GR_KB_CTRL )
                k = controlstdkeys;
        else if ( fkbState & GR_KB_SHIFT )
                k = shiftstdkeys;
        else
                k = stdkeys;

        for ( i=0; i<NSTDKEYS; i++ ) {
                if ( winkey == k[i].winkey )
                        return k[i].grkey;
        }

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
                        fkbState = convertwin32keystate ();
                        if ( fkbState & GR_KB_ALT ) fInsert = FALSE;
                        if ( fInsert ) {
                                chKey = wParam;
                                pEv = malloc ( sizeof ( SEventQueue ) );
                                pEv -> pEvent = malloc ( sizeof ( GrMouseEvent ) );
                                pEv -> pEvent -> flags = GR_M_KEYPRESS;
                                pEv -> pEvent -> key = wParam;
                                pEv -> pEvent -> kbstat = fkbState;
                                pEv -> pNext = NULL;
                                pEvQ = pEventQueue;
                                while ( pEvQ -> pNext != NULL ) {
                                        pEvQ = pEvQ -> pNext;
                                }
                                pEvQ -> pNext = pEv;
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
                        fkbState = convertwin32keystate ();
                        chKey = wParam;
                        pEv = malloc ( sizeof ( SEventQueue ) );
                        pEv -> pEvent = malloc ( sizeof ( GrMouseEvent ) );
                        pEv -> pEvent -> flags = GR_M_KEYPRESS;
                        pEv -> pEvent -> key = wParam;
                        if ( fkbState & GR_KB_ALT ) {
                                if ( chKey >= 'a' && chKey <= 'z' ) {
                                        pEv -> pEvent -> key = altletters[chKey - 'a'];
                                        fInsert = TRUE;
                                }
                                if ( chKey >= 'A' && chKey <= 'Z' ) {
                                        pEv -> pEvent -> key = altletters[chKey - 'A'];
                                        fInsert = TRUE;
                                }
                                if ( chKey >= '0' && chKey <= '9' ) {
                                        pEv -> pEvent -> key = altnumbers[chKey - '0'];
                                        fInsert = TRUE;
                                }
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
        case WM_SYSKEYDOWN :
                {
                        SEventQueue        *pEvQ, *pEv;
                        ULONG                fkbState;
                        char                chKey;

                        EnterCriticalSection ( &csEventQueue );
                        fkbState = convertwin32keystate ();
                        chKey = wParam;
                        pEv = malloc ( sizeof ( SEventQueue ) );
                        pEv -> pEvent = malloc ( sizeof ( GrMouseEvent ) );
                        pEv -> pEvent -> flags = GR_M_KEYPRESS;
                        pEv -> pEvent -> key = stdkeytranslate( chKey,fkbState );
                        pEv -> pEvent -> kbstat = fkbState;
                        pEv -> pNext = NULL;
/*
                        if( pEv -> pEvent -> key == GrKey_Alt_F4 ) {
                                PostMessage ( hWnd, WM_DESTROY, 0, 0 );
                                pEv -> pEvent -> key = 0;
                        }
*/
                        if ( pEv -> pEvent -> key != 0) {
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

