/**
 ** VD_XWIN.C ---- the standard X Window driver
 **
 ** Author:        Ulrich Leodolter
 ** E-mail:        ulrich@lab1.psy.univie.ac.at
 ** Date:        Thu Sep 28 09:29:26 1995
 ** RCSId:        $Id: vd_xwin.c 1.2 1995/11/19 19:28:12 ulrich Exp $
 **/

#include "libxwin.h"
#include "libgrx.h"
#include "grdriver.h"
#include "arith.h"

Display *        _XGrDisplay;
int                _XGrScreen;
Window                _XGrWindow;
Pixmap                _XGrBitmap;
Pixmap                _XGrPattern;
Colormap        _XGrColormap;
GC                _XGrGC;
GC                _XGrBitmapGC;
GC                _XGrPatternGC;
long                _XGrForeColor;
long                _XGrBackColor;
unsigned int        _XGrColorOper;
unsigned int        _XGrDepth;
unsigned int        _XGrBitsPerPixel;

static void setbank(int bk)
{}

static void setrwbanks(int rb,int wb)
{}

static void loadcolor(int c,int r,int g,int b)
{
  if (_XGrDisplay != NULL && _XGrColormap != None) {
    XColor xcolor;
    xcolor.pixel = c;
    xcolor.red   = r * 257;
    xcolor.green = g * 257;
    xcolor.blue  = b * 257;
    xcolor.flags = DoRed|DoGreen|DoBlue;
    XStoreColor (_XGrDisplay, _XGrColormap, &xcolor);
  }
}

static int setmode(GrVideoMode *mp,int noclear)
{
  char name[100], *window_name, *icon_name;
  int nc;
  char *cu;

  if (_XGrWindow == None)
    return (FALSE);

  XUnmapWindow (_XGrDisplay, _XGrWindow);
  if (mp->extinfo->mode != GR_frameText) {
    XSizeHints hints;
      
    XResizeWindow (_XGrDisplay, _XGrWindow, mp->width, mp->height);

    cu = "";
    nc = 1 << mp->bpp;
    if ((nc % 1024) == 0) nc >>= 10, cu = "K";
    if ((nc % 1024) == 0) nc >>= 10, cu = "M";

    sprintf (name, "grx %dx%dx%d%s", mp->width, mp->height, nc, cu);
    window_name = name;
    icon_name = name;
      
    hints.min_width = mp->width;
    hints.max_width = mp->width;
    hints.min_height = mp->height;
    hints.max_height = mp->height;
    hints.flags = PMaxSize | PMinSize;

    XSetStandardProperties (_XGrDisplay,        /* display */
                            _XGrWindow,                /* w */
                            window_name,        /* window_name */
                            icon_name,                /* icon_name */
                            None,                /* icon_pixmap */
                            NULL,                /* argv */
                            0,                        /* argc */
                            &hints);                /* hints */

    XMapRaised (_XGrDisplay, _XGrWindow);
      
    /* Wait until window appears on screen */
    {
      XEvent xevent;
        
      while (1)        {
        XNextEvent (_XGrDisplay, &xevent);
        if (xevent.type == MapNotify)
          break;
      }
    }
  }
  return (TRUE);
}

/* Note: _GrViDrvEGAVGAtextModeExt is referenced in setup/drvinfo.c */
#define grtextext _GrViDrvEGAVGAtextModeExt

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

static GrVideoModeExt grxwinext = {
  GR_frameUndef,                        /* frame driver */
  NULL,                                        /* frame driver override */
  NULL,                                        /* frame buffer address */
  { 0, 0, 0 },                                /* color precisions */
  { 0, 0, 0 },                                /* color component bit positions */
  0,                                        /* mode flag bits */
  setmode,                                /* mode set */
  NULL,                                        /* virtual size set */
  NULL,                                        /* virtual scroll */
  setbank,                                /* bank set function */
  setrwbanks,                                /* double bank set function */
  loadcolor                                /* color loader */
};


static GrVideoMode modes[] = {
  /* pres.  bpp wdt   hgt   BIOS   scan  priv. &ext  */
  {  TRUE,  8,   80,   25,  0x00,    80, 1,    &grtextext  },
  { FALSE,  0,  320,  240,  0x00,     0, 0,    &grxwinext  },
  { FALSE,  0,  640,  480,  0x00,     0, 0,    &grxwinext  },
  { FALSE,  0,  800,  600,  0x00,     0, 0,    &grxwinext  },
  { FALSE,  0, 1024,  768,  0x00,     0, 0,    &grxwinext  },
  { FALSE,  0, 1280, 1024,  0x00,     0, 0,    &grxwinext  },
};

static int detect(void)
{
  return (getenv("DISPLAY") ? TRUE : FALSE);
}

static int init(char *options)
{
  Window root;
  XSetWindowAttributes attr;
  unsigned long mask;
  Visual *visual;
  GrVideoMode *mp;
  unsigned int depth, bpp;
  
  _XGrDisplay = XOpenDisplay ("");
  if (_XGrDisplay == NULL)
    return (FALSE);

  _XGrScreen = DefaultScreen (_XGrDisplay);
  _XGrDepth = depth = DefaultDepth (_XGrDisplay, _XGrScreen);

  visual = DefaultVisual (_XGrDisplay, _XGrScreen);
  root = RootWindow (_XGrDisplay, _XGrScreen);
  
  if (visual->class == PseudoColor
      && (depth == 4 || depth == 8)) {
    _XGrColormap = XCreateColormap (_XGrDisplay, root, visual, True);
    grxwinext.cprec[0] = visual->bits_per_rgb;
    grxwinext.cprec[1] = visual->bits_per_rgb;
    grxwinext.cprec[2] = visual->bits_per_rgb;
    grxwinext.cpos[0] = 0;
    grxwinext.cpos[1] = 0;
    grxwinext.cpos[2] = 0;
    /* loadcolor (0, 0, 0, 0); */        /* load black */
  }
  else if (visual->class == TrueColor
           && (depth == 8 || depth == 15 || depth == 16 || depth == 24)) {
    int i, pos, prec;
    unsigned long mask[3];

    _XGrColormap = (Colormap) 0;

    mask[0] = visual->red_mask;
    mask[1] = visual->green_mask;
    mask[2] = visual->blue_mask;
    for (i = 0; i < 3; i++) {
      for (pos = 0; !(mask[i] & 1); pos++, mask[i]>>=1);
      for (prec = 0; (mask[i] & 1); prec++, mask[i]>>=1);
      grxwinext.cprec[i] = prec;
      grxwinext.cpos[i] = pos;
    }
  }
  else {
    static char *class_names[] = {
      "StaticGray",
      "GrayScale", 
      "StaticColor",
      "PseudoColor",
      "TrueColor",
      "DirectColor"
    };
    XCloseDisplay (_XGrDisplay);
    fprintf (stderr, "GRX init: Visual class=%s depth=%d not supported\n",
             class_names[visual->class], depth);
    exit (1);
  }
  _XGrForeColor = GrNOCOLOR;        /* force XSetForeground */
  _XGrBackColor = GrNOCOLOR;        /* force XSetBackground */
  _XGrColorOper = C_WRITE; 

  mask = 0L;
  attr.backing_store = Always;
  mask |= CWBackingStore;
  if (_XGrColormap) {
    attr.colormap = _XGrColormap;
    mask |= CWColormap;
  }
  attr.background_pixel = 0L;
  mask |= CWBackPixel;
  attr.border_pixel = 0L;
  mask |= CWBorderPixel;
  attr.event_mask = (StructureNotifyMask
                     | KeyPressMask
                     | KeyReleaseMask
                     | ButtonPressMask
                     | ButtonReleaseMask
                     | ButtonMotionMask
                     | PointerMotionMask);
  mask |= CWEventMask;
  _XGrWindow = XCreateWindow (_XGrDisplay,
                              root,                /* parent */
                              0,                /* x */
                              0,                /* y */
                              1,                /* width */
                              1,                /* height */
                              0,                /* border_width */
                              depth,                /* depth */
                              InputOutput,        /* class */
                              visual,                /* visual */
                              mask,                /* valuemask */
                              &attr);                /* attributes */
  _XGrGC = XCreateGC (_XGrDisplay, _XGrWindow, 0L, NULL);
  _XGrBitmap = XCreatePixmap (_XGrDisplay, root, 128, 128, 1);
  _XGrBitmapGC = XCreateGC (_XGrDisplay, _XGrBitmap, 0L, NULL);
  _XGrPattern = XCreatePixmap (_XGrDisplay, root, 8, 1, 1);
  _XGrPatternGC = XCreateGC (_XGrDisplay, _XGrPattern, 0L, NULL);

  grxwinext.frame = (char far *) _XGrWindow;

  bpp = depth;
  switch (depth) {
  case  1: grxwinext.mode = GR_frameXWIN1; break;
  case  4: grxwinext.mode = GR_frameXWIN4; break;
  case  8: grxwinext.mode = GR_frameXWIN8; break;
  case 15: bpp++;
  case 16: grxwinext.mode = GR_frameXWIN16; break;
  case 24: grxwinext.mode = GR_frameXWIN24; break;
  }

  _XGrBitsPerPixel = bpp;

  for (mp = &modes[1]; mp < &modes[itemsof(modes)]; mp++) {
    mp->present = TRUE;
    mp->bpp = depth;
    mp->lineoffset = (mp->width * bpp) / 8;
  }
  return(TRUE);
}

static void reset(void)
{
  if (_XGrDisplay) XCloseDisplay (_XGrDisplay);
}

GrVideoDriver _GrVideoDriverXWIN = {
  "xwin",                                /* name */
  GR_XWIN,                                /* adapter type */
  NULL,                                        /* inherit modes from this driver */
  modes,                                /* mode table */
  itemsof(modes),                        /* # of modes */
  detect,                                /* detection routine */
  init,                                        /* initialization routine */
  reset                                        /* reset routine */
};

