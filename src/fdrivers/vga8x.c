/**
 ** VGA8X.C ---- the 256 color VGA mode X frame driver
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 **/

#include "grdriver.h"
#include "libgrx.h"
#include "arith.h"
#include "docolor.h"
#include "mempeek.h"
#include "memfill.h"
#include "ioport.h"
#include "vgaregs.h"
#include "highlow.h"

#define _SetVGAWritePlanes(planes) \
    outport_w(VGA_SEQUENCER_PORT,highlow((planes),VGA_WRT_PLANE_ENB_REG))

#define _SetVGAReadPlane(plane) \
    outport_w(VGA_GR_CTRL_PORT,highlow((plane),VGA_RD_PLANE_SEL_REG))

#define _SetVGAWritePlane(plane) _SetVGAWritePlanes(1<<(plane))

#define _SetVGAWriteAllPlanes()  _SetVGAWritePlanes(0x0f)

static int _GrPXLmaskTable[] = { 0, 0x0e, 0x0c, 0x08 };
static int _GrPXRmaskTable[] = { 0, 0x01, 0x03, 0x07 };

static
#ifdef __GNUC__
inline
#endif
long readpixel(GrFrame *c,int x,int y)
{
        _SetVGAReadPlane(x&3);
        setup_far_selector(c->gf_selector);
        return((uchar)peek_b_f(&c->gf_baseaddr[0][(y * c->gf_lineoffset) + (x >> 2)]));
}

static
#ifdef __GNUC__
inline
#endif
void drawpixel(int x,int y,long color)
{
        char far *ptr = &CURC->gc_baseaddr[0][(y * CURC->gc_lineoffset) + (x >> 2)];
        _SetVGAWritePlane(x&3);
        setup_far_selector(CURC->gc_selector);
        switch(C_OPER(color)) {
          case C_XOR:
            _SetVGAReadPlane(x&3);
            poke_b_f_xor(ptr,(uchar)color);
            break;
          case C_OR:
            _SetVGAReadPlane(x&3);
            poke_b_f_or( ptr,(uchar)color);
            break;
          case C_AND:
            _SetVGAReadPlane(x&3);
            poke_b_f_and(ptr,(uchar)color);
            break;
          default:
            poke_b_f(ptr,(uchar)color);
            break;
        }
}

static void drawhline(int x,int y,int w,long color) {
  uchar far *p;
  int   opr, cval;
  long  offs;

  opr = C_OPER(color);
  if (w == 0 || !DOCOLOR8(color,opr)) return;
  cval = freplicate_b2l((int)color);
  offs = (y * CURC->gc_lineoffset) + (x >> 2);
  p = (uchar far *)(&CURC->gc_baseaddr[0][(unsigned)offs]);
  setup_far_selector(CURC->gc_selector);
  if ((opr == C_WRITE) && (w >= 10)) {  /* 10 = 3(left) + 1*4 + 3(right) */
      int rmask = (x + w) & 3;
      int lmask =  x      & 3;
      if(lmask) {
        _SetVGAWritePlanes(_GrPXLmaskTable[lmask]);
        poke_b_f(p,(uchar)cval);
        ++p;
        w -= (4 - lmask);
      }
      w >>= 2;
      if(rmask) {
        _SetVGAWritePlanes(_GrPXRmaskTable[rmask]);
        poke_b_f(p+w,(uchar)cval);
      }
      _SetVGAWriteAllPlanes();
      repfill_b_f(p,cval,w);
  } else {
    int i, plane = x & 3;
    for (i=0; i < 4; ++i) {
      uchar far *pp = p;
      int ww = (w+3) >> 2;
      if ( !ww ) break;
      _SetVGAReadPlane(plane);
      _SetVGAWritePlane(plane);
      switch (opr) {
        case C_XOR: repfill_b_f_xor(pp,cval,ww); break;
        case C_OR : repfill_b_f_or( pp,cval,ww); break;
        case C_AND: repfill_b_f_and(pp,cval,ww); break;
        default   : repfill_b_f(    pp,cval,ww); break;
      }
      --w;
      if (++plane == 4) { plane = 0; ++p; }
    }
  }
}

static
#include "fdrivers/generic/vline.c"

static
#include "fdrivers/generic/block.c"

static
#include "fdrivers/generic/line.c"

static
#include "fdrivers/generic/bitmap.c"

static
#include "fdrivers/generic/pattern.c"

static
#include "fdrivers/generic/bitblt.c"

GrFrameDriver _GrFrameDriverVGA8X = {
    GR_frameVGA8X,              /* frame mode */
    GR_frameRAM8,               /* compatible RAM frame mode */
    TRUE,                       /* onscreen */
    2,                          /* scan line width alignment */
    4,                          /* number of planes */
    8,                          /* bits per pixel */
    64*1024L,                   /* max plane size the code can handle */
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
    _GrFrDrvGenericBitBlt,
    _GrFrDrvGenericBitBlt
};

