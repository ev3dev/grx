/**
 ** SOLIDFIL.C ---- wrapper for solid filling
 **
 ** Copyright (c) 1998 Hartmut Schirmer
 */

#include "libgrx.h"
#include "shapes.h"

static void pixel(int x,int y,GrFillArg fval) {
  GRX_ENTER();
  FDRV->drawpixel(x,y,fval.color);
  GRX_LEAVE();
}

static void line(int x,int y,int dx,int dy,GrFillArg fval) {
  GRX_ENTER();
  FDRV->drawline(x,y,dx,dy,fval.color);
  GRX_LEAVE();
}

static void scan(int x,int y,int w,GrFillArg fval) {
  GRX_ENTER();
  FDRV->drawhline(x,y,w,fval.color);
  GRX_LEAVE();
}

GrFiller _GrSolidFiller = {
    pixel, line, scan
};

