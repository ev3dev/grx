/**
 ** fd_xwin.c - the X Window color frame driver
 **
 ** Author:     Ulrich Leodolter
 ** E-mail:     ulrich@lab1.psy.univie.ac.at
 ** Date:       Thu Sep 28 10:31:08 1995
 ** RCSId:      $Id: fd_xwin.c 1.1 1995/11/19 17:42:55 ulrich Exp $
 **
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 **/

#include <string.h>
#include "libxwin.h"
#include "libgrx.h"
#include "grdriver.h"

#if defined(XLibSpecificationRelease) && (XLibSpecificationRelease<6)
#error X11 driver needs X11R6 or newer !
#endif

static inline
void _XGrSetForeColor (long color)
{
  color &= GrCVALUEMASK;
  if (color != _XGrForeColor)
    {
      _XGrForeColor = color;
      XSetForeground (_XGrDisplay, _XGrGC, _XGrForeColor);
    }
}

static inline
void _XGrSetBackColor (long color)
{
  color &= GrCVALUEMASK;
  if (color != _XGrBackColor)
    {
      _XGrBackColor = color;
      XSetBackground (_XGrDisplay, _XGrGC, _XGrBackColor);
    }
}

static inline
void _XGrSetColorOper (long color)
{
  static int _GXtable[4] = {
    GXcopy,     /* C_WRITE */
    GXxor,      /* C_XOR */
    GXor,       /* C_OR */
    GXand,      /* C_AND */
  };
  unsigned int coper = C_OPER(color) & 0x03;

  if (coper != _XGrColorOper)
    {
      _XGrColorOper = coper;
      XSetFunction (_XGrDisplay, _XGrGC, _GXtable[_XGrColorOper]);
    }
}

static XImage * _XGrPixelCacheImage = NULL;
static Drawable _XGrPixelCacheDrawable = None;
static int      _XGrPixelCacheXmin;
static int      _XGrPixelCacheYmin;
static int      _XGrPixelCacheXmax;
static int      _XGrPixelCacheYmax;

#define DOT_INSIDE_PIXEL_CACHE(x,y)     \
        (   (x) >= _XGrPixelCacheXmin   \
         && (x) <  _XGrPixelCacheXmax   \
         && (y) >= _XGrPixelCacheYmin   \
         && (y) <  _XGrPixelCacheYmax)

#define INVALIDATE_PIXEL_CACHE do {     \
        _XGrPixelCacheDrawable = None;  \
        } while (0)

/* BUG: every drawing operation should check for drawings inside
   pixel cache, and if so invalidate pixel chache drawable */

static
long readpixel(GrFrame *c,int x, int y)
{
  if (_XGrPixelCacheDrawable != (Drawable) c->gf_baseaddr[0]
      || !DOT_INSIDE_PIXEL_CACHE(x,y))
    {
      Window dummy_root;
      unsigned int dummy_x, dummy_y, dummy_border_width, dummy_depth;
      unsigned int width, height;

      if (_XGrPixelCacheImage)
        XDestroyImage (_XGrPixelCacheImage);

      _XGrPixelCacheXmin = x;
      _XGrPixelCacheYmin = y;
      _XGrPixelCacheXmax = x + 32;
      _XGrPixelCacheYmax = y + 32;
      _XGrPixelCacheDrawable = (Drawable) c->gf_baseaddr[0];
      XGetGeometry (_XGrDisplay,
                    _XGrPixelCacheDrawable,
                    &dummy_root,
                    &dummy_x,
                    &dummy_y,
                    &width,
                    &height,
                    &dummy_border_width,
                    &dummy_depth);
      if (_XGrPixelCacheXmax > width)
        _XGrPixelCacheXmax = width;
      if (_XGrPixelCacheYmax > height)
        _XGrPixelCacheYmax = height;

      _XGrPixelCacheImage =
        XGetImage (_XGrDisplay,
                   _XGrPixelCacheDrawable,
                   _XGrPixelCacheXmin,
                   _XGrPixelCacheYmin,
                   _XGrPixelCacheXmax - _XGrPixelCacheXmin,
                   _XGrPixelCacheYmax - _XGrPixelCacheYmin,
                   AllPlanes,
                   ZPixmap);
    }
  return XGetPixel (_XGrPixelCacheImage,
                    x - _XGrPixelCacheXmin,
                    y - _XGrPixelCacheYmin);
}


static
inline
void drawpixel(int x,int y,long c)
{
  _XGrSetForeColor (c);
  _XGrSetColorOper (c);
  XDrawPoint (_XGrDisplay,
              (Drawable) CURC->gc_baseaddr[0],
              _XGrGC,
              x,
              y);
}


static
void drawhline(int x,int y,int w,long c)
{
  _XGrSetForeColor (c);
  _XGrSetColorOper (c);
  XDrawLine (_XGrDisplay,
             (Drawable) CURC->gc_baseaddr[0],
             _XGrGC,
             x,
             y,
             x + w - 1,
             y);
}


static
void drawvline(int x,int y,int h,long c)
{
  _XGrSetForeColor (c);
  _XGrSetColorOper (c);
  XDrawLine (_XGrDisplay,
             (Drawable) CURC->gc_baseaddr[0],
             _XGrGC,
             x,
             y,
             x,
             y + h - 1);
}


static
void drawblock(int x,int y,int w,int h,long c)
{
  _XGrSetForeColor (c);
  _XGrSetColorOper (c);
  XFillRectangle (_XGrDisplay,
                  (Drawable) CURC->gc_baseaddr[0],
                  _XGrGC,
                  x,
                  y,
                  w,
                  h);
}


static
void drawline(int x,int y,int dx,int dy,long c)
{
  _XGrSetForeColor (c);
  _XGrSetColorOper (c);
  XDrawLine (_XGrDisplay,
             (Drawable) CURC->gc_baseaddr[0],
             _XGrGC,
             x,
             y,
             x + dx,
             y + dy);
}


static
void drawbitmap(int x,int y,int w,int h,char far *bmp,int pitch,int start,long fg,long bg)
{
  XImage ximage;

  bmp += (uint)start >> 3;
  start &= 7;

  ximage.width          = w;
  ximage.height         = h;
  ximage.xoffset        = start;
  ximage.format         = XYBitmap;
  ximage.data           = bmp;
  ximage.byte_order     = LSBFirst;
  ximage.bitmap_unit    = 8;
  ximage.bitmap_bit_order = MSBFirst;
  ximage.bitmap_pad     = 8;
  ximage.depth          = 1;
  ximage.bytes_per_line = pitch;
  ximage.bits_per_pixel = 1;
  ximage.red_mask       = 0L;
  ximage.green_mask     = 0L;
  ximage.blue_mask      = 0L;
  ximage.obdata         = NULL;
  memchr(&ximage.f, 0, sizeof(ximage.f));

  if (XInitImage (&ximage) == 0)
    return;

  if ((C_OPER(fg) == C_OPER(bg)) && (fg != GrNOCOLOR)) {
    _XGrSetForeColor (fg);
    _XGrSetBackColor (bg);
    _XGrSetColorOper (fg);
    XPutImage (_XGrDisplay,
               (Drawable) CURC->gc_baseaddr[0],
               _XGrGC,
               &ximage,
               0,
               0,
               x,
               y,
               w,
               h);
  }
  else {
    if (fg != GrNOCOLOR) {
      XSetForeground (_XGrDisplay, _XGrBitmapGC, 1);
      XSetBackground (_XGrDisplay, _XGrBitmapGC, 0);
      XPutImage (_XGrDisplay,
                 _XGrBitmap,
                 _XGrBitmapGC,
                 &ximage,
                 0,
                 0,
                 0,
                 0,
                 w,
                 h);
      XSetStipple (_XGrDisplay, _XGrGC, _XGrBitmap);
      XSetTSOrigin (_XGrDisplay, _XGrGC, x, y);
      XSetFillStyle (_XGrDisplay, _XGrGC, FillStippled);
      _XGrSetForeColor (fg);
      _XGrSetColorOper (fg);
      XFillRectangle (_XGrDisplay,
                      (Drawable) CURC->gc_baseaddr[0],
                      _XGrGC,
                      x,
                      y,
                      w,
                      h);
    }
    if (bg != GrNOCOLOR) {
      XSetForeground (_XGrDisplay, _XGrBitmapGC, 0);
      XSetBackground (_XGrDisplay, _XGrBitmapGC, 1);
      XPutImage (_XGrDisplay,
                 _XGrBitmap,
                 _XGrBitmapGC,
                 &ximage,
                 0,
                 0,
                 0,
                 0,
                 w,
                 h);
      XSetStipple (_XGrDisplay, _XGrGC, _XGrBitmap);
      XSetTSOrigin (_XGrDisplay, _XGrGC, x, y);
      XSetFillStyle (_XGrDisplay, _XGrGC, FillStippled);
      _XGrSetForeColor (bg);
      _XGrSetColorOper (bg);
      XFillRectangle (_XGrDisplay,
                      (Drawable) CURC->gc_baseaddr[0],
                      _XGrGC,
                      x,
                      y,
                      w,
                      h);
    }
    XSetFillStyle (_XGrDisplay, _XGrGC, FillSolid);
  }
}


/* Note: drawpattern is not tested because it's not used in this GRX version */
static
void drawpattern(int x,int y,int w,char patt,long fg,long bg)
{
  XImage ximage;

  ximage.width          = 8;
  ximage.height         = 1;
  ximage.xoffset        = 0;
  ximage.format         = XYBitmap;
  ximage.data           = &patt;
  ximage.byte_order     = LSBFirst;
  ximage.bitmap_unit    = 8;
  ximage.bitmap_bit_order = MSBFirst;
  ximage.bitmap_pad     = 8;
  ximage.depth          = 1;
  ximage.bytes_per_line = 1;
  ximage.bits_per_pixel = 1;
  ximage.red_mask       = 0L;
  ximage.green_mask     = 0L;
  ximage.blue_mask      = 0L;
  ximage.obdata         = NULL;
  memchr(&ximage.f, 0, sizeof(ximage.f));

  if (XInitImage (&ximage) == 0)
    return;

  if ((C_OPER(fg) == C_OPER(bg)) && (fg != GrNOCOLOR)) {
    XSetForeground (_XGrDisplay, _XGrPatternGC, 1);
    XSetBackground (_XGrDisplay, _XGrPatternGC, 0);
    XPutImage (_XGrDisplay,
               _XGrPattern,
               _XGrPatternGC,
               &ximage,
               0,
               0,
               0,
               0,
               8,
               1);
    XSetStipple (_XGrDisplay, _XGrGC, _XGrPattern);
    XSetTSOrigin (_XGrDisplay, _XGrGC, x, y);
    XSetFillStyle (_XGrDisplay, _XGrGC, FillOpaqueStippled);
    _XGrSetForeColor (fg);
    _XGrSetBackColor (bg);
    _XGrSetColorOper (fg);
    XFillRectangle (_XGrDisplay,
                    (Drawable) CURC->gc_baseaddr[0],
                    _XGrGC,
                    x,
                    y,
                    w,
                    1);
  }
  else {
    if (fg != GrNOCOLOR) {
      XSetForeground (_XGrDisplay, _XGrPatternGC, 1);
      XSetBackground (_XGrDisplay, _XGrPatternGC, 0);
      XPutImage (_XGrDisplay,
                 _XGrPattern,
                 _XGrPatternGC,
                 &ximage,
                 0,
                 0,
                 0,
                 0,
                 8,
                 1);
      XSetStipple (_XGrDisplay, _XGrGC, _XGrPattern);
      XSetTSOrigin (_XGrDisplay, _XGrGC, x, y);
      XSetFillStyle (_XGrDisplay, _XGrGC, FillStippled);
      _XGrSetForeColor (fg);
      _XGrSetColorOper (fg);
      XFillRectangle (_XGrDisplay,
                      (Drawable) CURC->gc_baseaddr[0],
                      _XGrGC,
                      x,
                      y,
                      w,
                      1);
    }
    if (bg != GrNOCOLOR) {
      XSetForeground (_XGrDisplay, _XGrPatternGC, 0);
      XSetBackground (_XGrDisplay, _XGrPatternGC, 1);
      XPutImage (_XGrDisplay,
                 _XGrPattern,
                 _XGrPatternGC,
                 &ximage,
                 0,
                 0,
                 0,
                 0,
                 8,
                 1);
      XSetStipple (_XGrDisplay, _XGrGC, _XGrPattern);
      XSetTSOrigin (_XGrDisplay, _XGrGC, x, y);
      XSetFillStyle (_XGrDisplay, _XGrGC, FillStippled);
      _XGrSetForeColor (bg);
      _XGrSetColorOper (bg);
      XFillRectangle (_XGrDisplay,
                      (Drawable) CURC->gc_baseaddr[0],
                      _XGrGC,
                      x,
                      y,
                      w,
                      1);
    }
  }
  XSetFillStyle (_XGrDisplay, _XGrGC, FillSolid);
}


static
void bitblt(GrFrame *dst,int dx,int dy,GrFrame *src,int x,int y,int w,int h,long op)
{
  _XGrSetColorOper (op);
  XCopyArea (_XGrDisplay,
             (Drawable) src->gf_baseaddr[0],
             (Drawable) dst->gf_baseaddr[0],
             _XGrGC,
             x,
             y,
             w,
             h,
             dx,
             dy);
}

static
void bltv2r(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,long op)
{
  if(GrColorMode(op) == GrIMAGE)
    _GrFrDrvGenericBitBlt(dst,dx,dy,
                          src,sx,sy,
                          w,h,
                          op
                          );
  else {
    XImage *ximage;

    ximage = XGetImage (_XGrDisplay,
                        (Drawable) src->gf_baseaddr[0],
                        sx,
                        sy,
                        w,
                        h,
                        AllPlanes,
                        ZPixmap);
    if (ximage) {
      int bytes_per_pixel = _XGrBitsPerPixel >> 3;
      GrFrame tmp = *dst;

      tmp.gf_baseaddr[0] =
        tmp.gf_baseaddr[1] =
          tmp.gf_baseaddr[2] =
            tmp.gf_baseaddr[3] = ximage->data;
      tmp.gf_lineoffset = ximage->bytes_per_line;

      _GrFrDrvPackedBitBltR2R(dst, (dx * bytes_per_pixel), dy,
                              &tmp, 0, 0,
                              (w * bytes_per_pixel), h,
                              op
                              );
      XDestroyImage (ximage);
    }
  }
}

static void bltr2v(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,long op)
{
  if(GrColorMode(op) == GrIMAGE)
    _GrFrDrvGenericBitBlt(dst,dx,dy,
                          src,sx,sy,
                          w,h,
                          op
                          );
  else {
    XImage ximage;

    ximage.width        = src->gf_lineoffset;
    ximage.height       = sy + h;
    ximage.xoffset      = 0;
    ximage.format       = ZPixmap;
    ximage.data         = src->gf_baseaddr[0];
    ximage.byte_order   = LSBFirst;
    ximage.bitmap_unit  = _XGrBitsPerPixel;
    ximage.bitmap_bit_order = MSBFirst;
    ximage.bitmap_pad   = _XGrBitsPerPixel;
    ximage.depth        = _XGrDepth;
    ximage.bytes_per_line = src->gf_lineoffset;
    ximage.bits_per_pixel = _XGrBitsPerPixel;
    ximage.red_mask     = 0L;
    ximage.green_mask   = 0L;
    ximage.blue_mask    = 0L;
    ximage.obdata       = NULL;
    memchr(&ximage.f, 0, sizeof(ximage.f));

    if (XInitImage (&ximage) != 0) {
      _XGrSetColorOper (op);
      XPutImage (_XGrDisplay,
                 (Drawable) dst->gf_baseaddr[0],
                 _XGrGC,
                 &ximage,
                 sx,
                 sy,
                 dx,
                 dy,
                 w,
                 h);
    }
  }
}


GrFrameDriver _GrFrameDriverXWIN8 = {
  GR_frameXWIN8,                /* frame mode */
  GR_frameRAM8,                 /* compatible RAM frame mode */
  TRUE,                         /* onscreen */
  4,                            /* line width alignment */
  1,                            /* number of planes */
  8,                            /* bits per pixel */
  8*16*1024L*1024L,             /* max plane size the code can handle */
  NULL,
  readpixel,
  drawpixel,
  drawline,
  drawhline,
  drawvline,
  drawblock,
  drawbitmap,
  drawpattern,
  bitblt,
  bltv2r,
  bltr2v,
};

GrFrameDriver _GrFrameDriverXWIN16 = {
  GR_frameXWIN16,               /* frame mode */
  GR_frameRAM16,                /* compatible RAM frame mode */
  TRUE,                         /* onscreen */
  4,                            /* line width alignment */
  1,                            /* number of planes */
  16,                           /* bits per pixel */
  16*16*1024L*1024L,            /* max plane size the code can handle */
  NULL,
  readpixel,
  drawpixel,
  drawline,
  drawhline,
  drawvline,
  drawblock,
  drawbitmap,
  drawpattern,
  bitblt,
  bltv2r,
  bltr2v,
};

GrFrameDriver _GrFrameDriverXWIN24 = {
  GR_frameXWIN24,               /* frame mode */
  GR_frameRAM24,                /* compatible RAM frame mode */
  TRUE,                         /* onscreen */
  4,                            /* line width alignment */
  1,                            /* number of planes */
  24,                           /* bits per pixel */
  24*16*1024L*1024L,            /* max plane size the code can handle */
  NULL,
  readpixel,
  drawpixel,
  drawline,
  drawhline,
  drawvline,
  drawblock,
  drawbitmap,
  drawpattern,
  bitblt,
  bltv2r,
  bltr2v,
};
