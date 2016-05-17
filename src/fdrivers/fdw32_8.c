/**
 ** w32dib24.c ---- the 256 color Win32 linear frame buffer driver using DIB
 **
 ** Copyright (c) 2003 Josu Onandia
 ** [e-mail: jonandia@fagorautomation.es]
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

#include "libwin32.h"
#include "driver8.h"

static void w32_drawpixel(int x, int y, GrxColor color)
{
    HDC hDC;
    COLORREF c;

    GRX_ENTER();
    drawpixel(x, y, color);

    c = GetPixel ( hDCMem, x, y );
    hDC = GetDC ( hGRXWnd );
    SetPixelV ( hDC, x, y, c );
    ReleaseDC ( hGRXWnd, hDC );

    GRX_LEAVE();
}

static void w32_drawline(int x, int y, int dx, int dy, GrxColor c)
{
    RECT r;

    GRX_ENTER();
    drawline(x, y, dx, dy, c);
    if (dx > 0 ) {
        r.left = x;
        r.right = x + dx + 1;
    } else {
        r.left = x + dx;
        r.right = x + 1;
    }
    if (dy > 0 ) {
        r.top = y;
        r.bottom = y + dy + 1;
    } else {
        r.top = y + dy;
        r.bottom = y + 1;
    }
    InvalidateRect(hGRXWnd, &r, FALSE);        
    GRX_LEAVE();
}

static void w32_drawhline(int x, int y, int w, GrxColor c)
{
    RECT r;

    GRX_ENTER();
    drawhline(x, y, w, c);
    r.left = x;
    r.top = y;
    r.right = x + w;
    r.bottom = y + 1;
    InvalidateRect(hGRXWnd, &r, FALSE);        
    GRX_LEAVE();
}

static void w32_drawvline(int x, int y, int h, GrxColor c)
{
    RECT r;

    GRX_ENTER();
    drawvline(x, y, h, c);
    r.left = x;
    r.top = y;
    r.right = x + 1;
    r.bottom = y + h;
    InvalidateRect(hGRXWnd, &r, FALSE);        
    GRX_LEAVE();
}

static void w32_drawpattern(int x, int y, int w, unsigned char patt,
                            GrxColor fg, GrxColor bg)
{
    RECT r;

    GRX_ENTER();
    drawpattern(x, y, w, patt, fg, bg);
    r.left = x;
    r.top = y;
    r.right = x + w;
    r.bottom = y + 1;
    InvalidateRect(hGRXWnd, &r, FALSE);        
    GRX_LEAVE();
}

static void w32_putscanline(int x, int y, int w,
                            const GrxColor *scl, GrxColor op)
{
    RECT r;

    GRX_ENTER();
    putscanline(x, y, w, scl, op);
    r.left = x;
    r.top = y;
    r.right = x + w;
    r.bottom = y + 1;
    InvalidateRect(hGRXWnd, &r, FALSE);
    GRX_LEAVE();
}

static void w32_drawblock(int x, int y, int w, int h, GrxColor c)
{
    RECT r;

    GRX_ENTER();
    drawblock(x, y, w, h, c);
    r.left = x;
    r.top = y;
    r.right = x + w;
    r.bottom = y + h;
    InvalidateRect(hGRXWnd, &r, FALSE);    
    GRX_LEAVE();
}

static void w32_drawbitmap(int x, int y, int w, int h, unsigned char *bmp,
                           int pitch, int start, GrxColor fg, GrxColor bg)
{
    RECT r;

    GRX_ENTER();
    drawbitmap(x, y, w, h, bmp, pitch, start, fg, bg);
    r.left = x;
    r.top = y;
    r.right = x + w;
    r.bottom = y + h;
    InvalidateRect(hGRXWnd, &r, FALSE);    
    GRX_LEAVE();
}

static void w32_bitblit(GrxFrame *dst, int dx, int dy, GrxFrame *src,
                        int sx, int sy, int w, int h, GrxColor op)
{
    RECT r;

    GRX_ENTER();
    bitblit(dst, dx, dy, src, sx, sy, w, h, op);
    r.left = dx;
    r.top = dy;
    r.right = dx + w;
    r.bottom = dy + h;
    InvalidateRect(hGRXWnd, &r, FALSE);
    GRX_LEAVE();
}

/* -------------------------------------------------------------------- */

GrxFrameDriver _GrFrameDriverWIN32_8 = {
    .mode               = GR_frameWIN32_8,         /* frame mode */
    .rmode              = GRX_FRAME_MODE_RAM_8BPP, /* compatible RAM frame mode */
    .is_video           = TRUE,                    /* onscreen */
    .row_align          = 4,                       /* line width alignment */
    .num_planes         = 1,                       /* number of planes */
    .bits_per_pixel     = 8,                       /* bits per pixel */
    .max_plane_size     = 8 * 16 * 1024L * 1024L,  /* max plane size the code can handle */
    .init               = NULL,
    .readpixel          = readpixel,
    .drawpixel          = w32_drawpixel,
    .drawline           = w32_drawline,
    .drawhline          = w32_drawhline,
    .drawvline          = w32_drawvline,
    .drawblock          = w32_drawblock,
    .drawbitmap         = w32_drawbitmap,
    .drawpattern        = w32_drawpattern,
    .bitblt             = w32_bitblit,
    .bltv2r             = bitblit,
    .bltr2v             = w32_bitblit,
    .getindexedscanline = getindexedscanline,
    .putscanline        = w32_putscanline,
};

