/**
 ** fd_win32.c - the Win32-API color frame driver
 **
 ** Author:     Gernot Graeff
 ** E-mail:     gernot.graeff@t-online.de
 ** Date:       21.08.98
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
 **   - Some bugs fixed
 **   - All the pixel reads (GetPixel) are done with in-memory
 **     bitmaps, because when done with on-screen bitmaps, and if the
 **     window is partially hidden or out of screen, then GetPixel fails.
 **   - Loops optimized in bitmap operations
 **   - Color handling optimized. By storing the last color used
 **     (index, rgb value) if the next color is the same, there is
 **     not need to call _GetColor, wich is time consuming.
 **   - 24 bpp driver.
 **   - WARNINGS: - The colors returned by GetPixel have to be the SAME that
 **                 colors in the ColorList. Unfortunately, Windows doesn't
 **                 use the RGB color you asked for, but an aproximation. Then
 **                 when a pixel is read, its color index it's not found
 **                 (_GetColIndex). It is the responsability of the function
 **                 that loads colors in the ColorList to guarantee that
 **                 those colors will actually be used by Windows 'as-is'.
 **               - The driver for 4 bits per pixel doesn't work.
 **   - TO DO: Support for logical operations in drawpixel, drawblock
 **            and putscanline
 **            Implement getiscanline for 8 and 24 bpp
 **            Implement bltv2r and bltr2v for 24 bpp
 **/

#include <string.h>
#include <assert.h>
#include "libgrx.h"
#include "libwin32.h"
#include "grdriver.h"

extern HWND hGRXWnd;
extern HDC hDCMem;

static COLORREF (*fGetColor)(unsigned int nIndex) = NULL;
static long (*fGetColIndex)(COLORREF color) = NULL;

static COLORREF _GetColor_palette ( unsigned int nIndex )
{
        COLORREF        color;
        SColorList        *cl;

        color = RGB ( 0, 0, 0 );
        cl = ColorList;
        while ( cl != NULL ) {
                if ( cl -> nIndex == nIndex ) {
                        color = cl -> color;
                        break;
                }
                cl = cl -> pNext;
        }

        return color;
}


static long _GetColIndex_palette ( COLORREF color )
{
        long                lRet = -1;
        SColorList        *cl;

        cl = ColorList;
        while ( cl != NULL ) {
                if ( cl -> color == color ) {
                        lRet = cl -> nIndex;
                        break;
                }
                cl = cl -> pNext;
        }

        return lRet;
}

static COLORREF _GetColor_RGB ( unsigned int nIndex )
{
        return((COLORREF) nIndex);
}

static long _GetColIndex_RGB ( COLORREF color )
{
        return((long)color);
}

static GrColor i_readpixel(GrFrame *c,int x, int y)
{
        COLORREF color;

        color = GetPixel ( hDCMem, x, y );
        return fGetColIndex ( color );
}

static void i_drawpixel(int x,int y,GrColor c)
{
        HDC hDC;
        COLORREF color;

        color = fGetColor ( GrColorValue(c) );
        hDC = GetDC ( hGRXWnd );
        SetPixelV ( hDC, x, y, color );
        SetPixelV ( hDCMem, x, y, color );
        ReleaseDC ( hGRXWnd, hDC );
}

// NOTE: Only handles GrWRITE and GrIMAGE
static void i_putscanline(int x,int y,int w,
                 const GrColor *scl, GrColor op )
{
        GrColor  skipc;
        HDC      hDC;
        COLORREF color;
        int      xx, xend;
        
        GRX_ENTER();
        skipc = op ^ GrIMAGE;
        op &= GrCMODEMASK;

        hDC = GetDC ( hGRXWnd );

        xend = x + w;
        for ( xx = x; xx < xend; xx++)
        {
                GrColor c = *(scl++);
                if (c != skipc)
                {
                        color = fGetColor ( c );
// Lot more speed if only draw the in-memory bitmap, and then
// do an BitBlt from memory to screen.
//                        SetPixelV ( hDC, x, y, color );
                        SetPixelV ( hDCMem, xx, y, color );
                }
        }
        BitBlt ( hDC, x, y, w, 1, hDCMem, x, y, SRCCOPY );
        ReleaseDC ( hGRXWnd, hDC );
   
        GRX_LEAVE();
}

static void i_drawline(int x,int y,int dx,int dy,GrColor c)
{
        HDC     hDC;
        HPEN        hPen;
        HGDIOBJ oldPen;
        int     r2mode;

        if(c == GrNOCOLOR) return;
        hDC = GetDC ( hGRXWnd );
        hPen = CreatePen ( PS_SOLID, 1, fGetColor ( GrColorValue(c) ) );
        switch(C_OPER(c)) {
          case C_XOR: r2mode = R2_XORPEN; break;
          case C_OR: r2mode = R2_MERGEPEN; break;
          case C_AND: r2mode = R2_MASKPEN; break;
          default: r2mode = R2_COPYPEN; break;
        }

        oldPen = SelectObject ( hDC, hPen );
        SetROP2(hDC, r2mode);
        MoveToEx ( hDC, x, y, NULL );
        LineTo ( hDC, x + dx, y + dy );
        SelectObject ( hDC, oldPen );
        SetROP2(hDC, R2_COPYPEN);
        ReleaseDC ( hGRXWnd, hDC );

        oldPen = SelectObject ( hDCMem, hPen );
        SetROP2(hDCMem, r2mode);
        MoveToEx ( hDCMem, x, y, NULL );
        LineTo ( hDCMem, x + dx, y + dy );
        SelectObject ( hDCMem, oldPen );
        DeleteObject(hPen);
}
static void i_drawhline(int x,int y,int w,GrColor c)
{
        i_drawline(x, y, w, 0, c);
}

static void i_drawvline(int x,int y,int h,GrColor c)
{
        i_drawline ( x, y, 0, h, c );
}

static void i_drawblock(int x,int y,int w,int h,GrColor c)
{
        HBRUSH        hBrush;
        HDC     hDC;
        RECT        rect;

        hDC = GetDC ( hGRXWnd );
        SetRect ( &rect, x, y, x+w, y+h );
        hBrush = CreateSolidBrush ( fGetColor ( GrColorValue(c) ) );
        FillRect ( hDC, &rect, hBrush );
        FillRect ( hDCMem, &rect, hBrush );
        ReleaseDC ( hGRXWnd, hDC );
        DeleteObject ( hBrush );
}

static void i_drawbitmap(int x,int y,int w,int h,char far *bmp,int pitch,
                         int start,GrColor fg,GrColor bg)
{
        HDC      hDC;
        int      i, j;
        COLORREF fgColor=0, bgColor=0;

        hDC = GetDC ( hGRXWnd );
        if(fg != GrNOCOLOR)
                fgColor = fGetColor ( GrColorValue(fg) );
        if(bg != GrNOCOLOR)
                bgColor = fGetColor ( GrColorValue(bg) );

        bmp += start >> 3;
        start &= 7;
        for ( i = 0; i < h; i++ )
        {
            unsigned char *bitp = (unsigned char *)bmp;
            unsigned char bits = *bitp;
            unsigned char mask = 0x80 >> start;
                for ( j = 0; j < w; j++ )
                {
                        if(bits & mask)
                        {
                                if(fg != GrNOCOLOR)
                                        SetPixelV ( hDCMem, x+j, y+i, fgColor );
                        }
                        else
                        {
                                if(bg != GrNOCOLOR)
                                        SetPixelV ( hDCMem, x+j, y+i, bgColor );
                        }
                        if((mask >>= 1) == 0) { bits = *++bitp; mask = 0x80;}
                }
                bmp += pitch;
        }

        BitBlt ( hDC, x, y, w, h, hDCMem, x, y, SRCCOPY );
        ReleaseDC ( hGRXWnd, hDC );
}

static void drawpattern(int x,int y,int w,char patt,GrColor fg,GrColor bg)
{
}

static void bitblt(GrFrame *dst,int dx,int dy,GrFrame *src,int x,int y,int w,
                                int h,GrColor op)
{
        HDC     hDC;
        DWORD        dwRop;

        switch ( op ) {

        case GrWRITE :
        case GrIMAGE :
                dwRop = SRCCOPY;
                break;
        case GrXOR :
                dwRop = SRCINVERT;
                break;
        case GrOR :
                dwRop = SRCPAINT;
                break;
        case GrAND :
                dwRop = SRCAND;        //SRCERASE;
                break;
        default:
                dwRop = SRCCOPY;
                break;
        }
        hDC = GetDC ( hGRXWnd );
        BitBlt ( hDC, dx, dy, w, h, hDCMem, x, y, dwRop );
        BitBlt ( hDCMem, dx, dy, w, h, hDCMem, x, y, dwRop );
        ReleaseDC ( hGRXWnd, hDC );
}

static void bltv2r_8(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,
                                        int w,int h,GrColor op)
{
        HDC             hDC, hDCFrom, hDCAux = 0;
        int             y, x;
        COLORREF                color;
        DWORD                        dwRop;
        HGDIOBJ         oldBitmapAux = 0, hBitmapAux = 0;
        unsigned char   *pdst;
        int             lastIndex;
        COLORREF        lastColor;

        switch ( op ) {

        case GrWRITE :
        case GrIMAGE :
                dwRop = SRCCOPY;
                break;
        case GrXOR :
                dwRop = SRCINVERT;
                break;
        case GrOR :
                dwRop = SRCPAINT;
                break;
        case GrAND :
                dwRop = SRCAND;        //SRCERASE;
                break;
        default:
                dwRop = SRCCOPY;
                break;
        }
        hDC = GetDC ( hGRXWnd );

        if(dwRop != SRCCOPY)
        {
                hDCAux = CreateCompatibleDC ( hDC );
                hBitmapAux = CreateCompatibleBitmap ( hDC, w, h );
                oldBitmapAux = SelectObject ( hDCAux, hBitmapAux );
                BitBlt ( hDCAux, 0, 0, w, h, hDCMem, sx, sy, dwRop );
                sx = sy = 0;
                hDCFrom = hDCAux;
        }
        else hDCFrom = hDCMem;
        ReleaseDC ( hGRXWnd, hDC );

        pdst = (unsigned char *)dst->gf_baseaddr[0];
        pdst += dx + (dy * dst->gf_lineoffset);
        lastIndex = 0;
        lastColor = _GetColor_palette(lastIndex);
        for ( y = 0; y < h; y++ )
        {
                unsigned char *pbmpdst = pdst;
                for ( x = 0; x < w; x++ )
                {
                        color = GetPixel ( hDCFrom, sx+x, sy+y );
                        if(color != lastColor)
                        {
                                lastColor = color;
                                lastIndex = _GetColIndex_palette(color);
                        }
                        *pbmpdst++ = (unsigned char)lastIndex;
                }
                pdst += dst->gf_lineoffset;
        }

        if(dwRop != SRCCOPY)
        {
                SelectObject ( hDCAux, oldBitmapAux );
                DeleteDC ( hDCAux );
                DeleteObject ( hBitmapAux );
        }
}

static void bltr2v_8(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,
                                        int w,int h,GrColor op)
{
        HDC      hDC, hDCAux=0, hDCDest;
        int      y, x, offx, offy;
        long     color;
        DWORD    dwRop;
        HGDIOBJ  oldBitmapAux=0;
        HBITMAP  hBitmapAux=0;
        unsigned char *psrc;
        int      lastIndex;
        COLORREF lastColor;

        switch ( op ) {

        case GrWRITE :
        case GrIMAGE :
                dwRop = SRCCOPY;
                break;
        case GrXOR :
                dwRop = SRCINVERT;
                break;
        case GrOR :
                dwRop = SRCPAINT;
                break;
        case GrAND :
                dwRop = SRCAND;                //SRCERASE;
                break;
        default:
                dwRop = SRCCOPY;
                break;
        }
        hDC = GetDC ( hGRXWnd );
        
        if(dwRop != SRCCOPY)
        {
                hDCAux = CreateCompatibleDC ( hDC );
                hBitmapAux = CreateCompatibleBitmap ( hDC, w, h );
                oldBitmapAux = SelectObject ( hDCAux, hBitmapAux );
                offx = 0;
                offy = 0;
                hDCDest = hDCAux;
        }
        else
        {
                offx = dx;
                offy = dy;
                hDCDest = hDCMem;
        }
        
        psrc = (unsigned char *)src->gf_baseaddr[0];
        psrc += sx + (sy * src->gf_lineoffset);
        lastIndex = 0;
        lastColor = _GetColor_palette(lastIndex);
        for ( y = 0; y < h; y++ )
        {
                unsigned char *pbmpsrc = psrc;
                for ( x = 0; x < w; x++ )
                {
                        color = *pbmpsrc++;
                        if(color != lastIndex)
                        {
                                lastIndex = color;
                                lastColor = _GetColor_palette(color);
                        }
                        SetPixelV( hDCDest, offx+x, offy+y, lastColor);
                }
                psrc += src->gf_lineoffset;
        }
        BitBlt ( hDC, dx, dy, w, h, hDCDest, offx, offy, dwRop );
        ReleaseDC ( hGRXWnd, hDC );
        if(dwRop != SRCCOPY)
        {
                BitBlt ( hDCMem, dx, dy, w, h, hDCAux, 0, 0, dwRop );
                SelectObject ( hDCAux, oldBitmapAux );
                DeleteDC ( hDCAux );
                DeleteObject ( hBitmapAux );
        }
}

static int init_frame(GrVideoMode *md)
{
        if(md->bpp >= 24)
        {
                fGetColor = _GetColor_RGB;
                fGetColIndex = _GetColIndex_RGB;
        }
        else
        {
                fGetColor = _GetColor_palette;
                fGetColIndex = _GetColIndex_palette;
        }
        return 0;
}


GrFrameDriver _GrFrameDriverWIN32_4 = {
        GR_frameWIN32_4,                /* frame mode */
        GR_frameRAM8,                        /* compatible RAM frame mode */
        TRUE,                           /* onscreen */
        4,                              /* line width alignment */
        1,                              /* number of planes */
        8,                              /* bits per pixel */
        8 * 16 * 1024L * 1024L,         /* max plane size the code can handle */
        init_frame,
        i_readpixel,
        i_drawpixel,
        i_drawline,
        i_drawhline,
        i_drawvline,
        i_drawblock,
        i_drawbitmap,
        drawpattern,
        bitblt,
        bltv2r_8,
        bltr2v_8,
        _GrFrDrvGenericGetIndexedScanline,
        i_putscanline
};


GrFrameDriver _GrFrameDriverWIN32_8 = {
        GR_frameWIN32_8,                /* frame mode */
        GR_frameRAM8,                        /* compatible RAM frame mode */
        TRUE,                           /* onscreen */
        4,                              /* line width alignment */
        1,                              /* number of planes */
        8,                              /* bits per pixel */
        8 * 16 * 1024L * 1024L,         /* max plane size the code can handle */
        init_frame,
        i_readpixel,
        i_drawpixel,
        i_drawline,
        i_drawhline,
        i_drawvline,
        i_drawblock,
        i_drawbitmap,
        drawpattern,
        bitblt,
        bltv2r_8,
        bltr2v_8,
        _GrFrDrvGenericGetIndexedScanline,
        i_putscanline
};


GrFrameDriver _GrFrameDriverWIN32_24 = {
        GR_frameWIN32_24,                /* frame mode */
#ifdef GRX_USE_RAM3x8
        GR_frameRAM3x8,
#else
        GR_frameRAM24,                        /* compatible RAM frame mode */
#endif
        TRUE,                           /* onscreen */
        4,                              /* line width alignment */
        1,                              /* number of planes */
        24,                             /* bits per pixel */
        8 * 16 * 1024L * 1024L,         /* max plane size the code can handle */
        init_frame,
        i_readpixel,
        i_drawpixel,
        i_drawline,
        i_drawhline,
        i_drawvline,
        i_drawblock,
        i_drawbitmap,
        drawpattern,
        bitblt,
        _GrFrDrvGenericBitBlt,
        _GrFrDrvGenericBitBlt,
        _GrFrDrvGenericGetIndexedScanline,
        i_putscanline
};

