/**
 ** SHAPES.H ---- declarations and global data for generating complex shapes
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#ifndef __SHAPES_H_INCLUDED__
#define __SHAPES_H_INCLUDED__

typedef union _GR_fillArg {
    long    color;
    struct _GR_bitmap *bmp;
    struct _GR_pixmap *pxp;
    GrPattern *p;
} GrFillArg;
/*
typedef union _GR_fillArg {
    long    color;
    struct _GR_bitmap *bmp;
    struct _GR_pixmap *pxp;
} GrFillArg;
*/

typedef void (*PixelFillFunc)(int x,int y,GrFillArg fval);
typedef void (*LineFillFunc)(int x,int y,int dx,int dy,GrFillArg fval);
typedef void (*ScanFillFunc)(int x,int y,int w,GrFillArg fval);

/*
 * The order of the three functions in this struct MUST match the order
 * of the corresponding three functions in the frame driver structure.
 */
typedef struct _GR_filler {
    PixelFillFunc pixel;
    LineFillFunc  line;
    ScanFillFunc  scan;
} GrFiller;

#define         _GrSolidFiller          (*((GrFiller *)(&FDRV->drawpixel)))
extern GrFiller _GrBitmapFiller;
extern GrFiller _GrPixmapFiller;

void _GrDrawPolygon(int n,int pt[][2],GrFiller *f,GrFillArg c,int doClose);
void _GrDrawCustomPolygon(int n,int pt[][2],GrLineOption *lp,GrFiller *f,GrFillArg c,int doClose,int circle);
void _GrScanConvexPoly(int n,int pt[][2],GrFiller *f,GrFillArg c);
void _GrScanPolygon(int n,int pt[][2],GrFiller *f,GrFillArg c);
void _GrScanEllipse(int xc,int yc,int xa,int ya,GrFiller *f,GrFillArg c,int filled);

/* --- */
#define _GrDrawPatternedPixel ((PixelFillFunc)GrPatternFilledPlot)
#define _GrDrawPatternedLine ((LineFillFunc)_GrPatternFilledLine)
void _GrFillPatternedScanLine(int x,int y,int w,GrFillArg arg);

#endif
