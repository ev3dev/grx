/*
 * shapes.h ---- declarations and global data for generating complex shapes
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
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

#ifndef __SHAPES_H_INCLUDED__
#define __SHAPES_H_INCLUDED__

#include <glib.h>

#include <grx/pixmap.h>

#define USE_FDR_DRAWPATTERN 1

typedef union _GR_fillArg {
    GrxColor color;
    struct _GR_bitmap *bmp;
    struct _GR_pixmap *pxp;
    GrxPixmap *p;
} GrFillArg;

typedef void (*PixelFillFunc)(int x, int y, GrFillArg fval);
typedef void (*LineFillFunc)(int x, int y, int dx, int dy, GrFillArg fval);
typedef void (*ScanFillFunc)(int x, int y, int w, GrFillArg fval);

typedef struct _GR_filler {
    PixelFillFunc pixel;
    LineFillFunc line;
    ScanFillFunc scan;
} GrFiller;

G_GNUC_INTERNAL extern GrFiller _GrSolidFiller;
G_GNUC_INTERNAL extern GrFiller _GrPatternFiller;

G_GNUC_INTERNAL void _GrDrawPolygon(
    int n, const GrxPoint *pt, GrFiller *f, GrFillArg c, int doClose);
G_GNUC_INTERNAL void _GrDrawCustomPolygon(int n, const GrxPoint *pt,
    const GrxLineOptions *lp, GrFiller *f, GrFillArg c, int doClose, int circle);
G_GNUC_INTERNAL void _GrScanConvexPoly(
    int n, const GrxPoint *pt, GrFiller *f, GrFillArg c);
G_GNUC_INTERNAL void _GrScanPolygon(
    int n, const GrxPoint *pt, GrFiller *f, GrFillArg c);
G_GNUC_INTERNAL void _GrScanEllipse(
    int xc, int yc, int rx, int ry, GrFiller *f, GrFillArg c, int filled);

#define _GrDrawPatternedPixel ((PixelFillFunc)_GrPatternFilledPlot)
#define _GrDrawPatternedLine  ((LineFillFunc)_GrPatternFilledLine)
G_GNUC_INTERNAL void _GrFillPatternedScanLine(int x, int y, int w, GrFillArg arg);

G_GNUC_INTERNAL void _GrFloodFill(
    int x, int y, GrxColor border, GrFiller *f, GrFillArg fa);

G_GNUC_INTERNAL void _GrFillPattern(int x, int y, int width, GrxPixmap *p);
G_GNUC_INTERNAL void _GrFillPatternExt(
    int x, int y, int sx, int sy, int width, GrxPixmap *p);
G_GNUC_INTERNAL void _GrPatternFilledLine(int x1, int y1, int dx, int dy, GrxPixmap *p);
G_GNUC_INTERNAL void _GrPatternFilledPlot(int x, int y, GrxPixmap *p);

G_GNUC_INTERNAL void _GrFillBitmapPattern(int x, int y, int w, int h,
    unsigned char *bmp, int pitch, int start, GrxPixmap *p, GrxColor bg);
G_GNUC_INTERNAL void _GrFillBitmapPatternExt(int x, int y, int w, int h, int sx, int sy,
    unsigned char *bmp, int pitch, int start, GrxPixmap *p, GrxColor bg);

#endif
