/*
 * w32dib24.c ---- the 16M color Win32 linear frame buffer driver using DIB
 *
 * Copyright (c) 2003 Mariano Alvarez Fernandez
 * [e-mail: malfer@telefonica.net]
 *
 * This file is part of the GRX graphics library.
 *
 * The GRX graphics library is free software; you can redistribute it
 * and/or modify it under some conditions; see the "copying.grx" file
 * for details.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "libwin32.h"
#include "driver24.h"

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

static void w32_drawline(int x, int y, int dx, int dy, GrxColor color)
{
    RECT Rect;

    GRX_ENTER();
    drawline(x, y, dx, dy, color);
    if (dx > 0 ) {
        Rect.left = x;
        Rect.right = x + dx + 1;
    } else {
        Rect.left = x + dx;
        Rect.right = x + 1;
    }
    if (dy > 0 ) {
        Rect.top = y;
        Rect.bottom = y + dy + 1;
    } else {
        Rect.top = y + dy;
        Rect.bottom = y + 1;
    }
    InvalidateRect(hGRXWnd, &Rect, FALSE);
    GRX_LEAVE();
}

static void w32_drawhline(int x, int y, int w, GrxColor color)
{
    RECT Rect;

    GRX_ENTER();
    drawhline(x, y, w, color);
    Rect.left = x;
    Rect.top = y;
    Rect.right = x + w;
    Rect.bottom = y + 1;
    InvalidateRect(hGRXWnd, &Rect, FALSE);
    GRX_LEAVE();
}

static void w32_drawvline(int x, int y, int h, GrxColor color)
{
    RECT Rect;

    GRX_ENTER();
    drawvline(x, y, h, color);
    Rect.left = x;
    Rect.top = y;
    Rect.right = x + 1;
    Rect.bottom = y + h;
    InvalidateRect(hGRXWnd, &Rect, FALSE);
    GRX_LEAVE();
}

static void w32_drawblock(int x, int y, int w, int h, GrxColor color)
{
    RECT Rect;

    GRX_ENTER();
    drawblock(x, y, w, h, color);
    Rect.left = x;
    Rect.top = y;
    Rect.right = x + w;
    Rect.bottom = y + h;
    InvalidateRect(hGRXWnd, &Rect, FALSE);
    GRX_LEAVE();
}

static void w32_drawbitmap(int x, int y, int w, int h, unsigned char *bmp,
                           int pitch, int start, GrxColor fg, GrxColor bg)
{
    RECT Rect;

    GRX_ENTER();
    drawbitmap(x, y, w, h, bmp, pitch, start, fg, bg);
    Rect.left = x;
    Rect.top = y;
    Rect.right = x + w;
    Rect.bottom = y + h;
    InvalidateRect(hGRXWnd, &Rect, FALSE);
    GRX_LEAVE();
}

static void w32_drawpattern(int x, int y, int w, unsigned char patt,
                            GrxColor fg, GrxColor bg)
{
    RECT Rect;

    GRX_ENTER();
    drawpattern(x, y, w, patt, fg, bg);
    Rect.left = x;
    Rect.top = y;
    Rect.right = x + w;
    Rect.bottom = y + 1;
    InvalidateRect(hGRXWnd, &Rect, FALSE);
    GRX_LEAVE();
}

static void w32_bitblt(GrxFrame *dst, int dx, int dy, GrxFrame *src,
                       int sx, int sy, int w, int h, GrxColor op)
{
    RECT Rect;

    GRX_ENTER();
    bitblt(dst, dx, dy, src, sx, sy, w, h, op);
    Rect.left = dx;
    Rect.top = dy;
    Rect.right = dx + w;
    Rect.bottom = dy + h;
    InvalidateRect(hGRXWnd, &Rect, FALSE);
    GRX_LEAVE();
}

void w32_putscanline(int x, int y, int w,
                     const GrxColor *scl, GrxColor op )
{
    GrxColor skipc;
    RECT Rect;

    GRX_ENTER();
    Rect.left = x;
    Rect.top = y;
    Rect.right = x + w;
    Rect.bottom = y + 1;

    skipc = op ^ GRX_COLOR_MODE_IMAGE;
    op &= GRX_COLOR_MODE_MASK;
    for ( w += x; x < w; ++x) {
        GrxColor c = *(scl++);
        if (c != skipc) drawpixel(x, y, (c|op));
    }

    InvalidateRect(hGRXWnd, &Rect, FALSE);
    GRX_LEAVE();
}

/* -------------------------------------------------------------------- */

GrxFrameDriver _GrFrameDriverWIN32_24 = {
    .mode               = GR_frameWIN32_24,         /* frame mode */
    .rmode              = GRX_FRAME_MODE_RAM_24BPP, /* compatible RAM frame mode */
    .is_video           = TRUE,                     /* onscreen */
    .row_align          = 4,                        /* line width alignment */
    .bits_per_pixel     = 24,                       /* bits per pixel */
    .max_plane_size     = 24 * 16 * 1024L * 1024L,  /* max plane size the code can handle */
    .init               = NULL,
    .readpixel          = readpixel,
    .drawpixel          = w32_drawpixel,
    .drawline           = w32_drawline,
    .drawhline          = w32_drawhline,
    .drawvline          = w32_drawvline,
    .drawblock          = w32_drawblock,
    .drawbitmap         = w32_drawbitmap,
    .drawpattern        = w32_drawpattern,
    .bitblt             = w32_bitblt,
    .bltv2r             = bitblt,
    .bltr2v             = w32_bitblt,
    .getindexedscanline = _GrFrDrvGenericGetIndexedScanline,
    .putscanline        = w32_putscanline,
};
