/**
 ** drawstrg.c ---- low level character string output
 **
 ** Copyright (c) 1998 Hartmut Schirmer
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu]
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
 ** Contributions by:
 ** 080125 M.Alvarez, UTF-8 support
 ** 170706 M.Alvarez, rewrite for font encoding functionality
 *= 200620 M.Alvarez, solved an old bug
 **
 **/

#include "libgrx.h"
#include "clipping.h"
#include "text/text.h"

static void _GrDrawWordText(const unsigned short *text,int length,int x,int y,
                   const GrTextOption *opt, GrPattern *p, TextDrawBitmapFunc dbm)
{
  GrFont *f;
  int    x1;
  GRX_ENTER();

  if (((f = opt->txo_font) != NULL) &&
      ((x1 = _GrFontWordTextWidth(f,text,length)) != 0)) {
    GrColor fgcv  = opt->txo_fgcolor;
    GrColor bgcv  = opt->txo_bgcolor;
    int     undl  = (fgcv & GR_UNDERLINE_TEXT) ? 1 : 0;
    int     rotat = GR_TEXT_IS_VERTICAL(opt->txo_direct) ? ~0 : 0;
    int     dxpre = 0;
    int     dypre = 0;
    int     dxpost= 0;
    int     dypost= 0;
    int     y1    = f->h.height;
    int     ww    = (x1 & ~rotat) | (y1 &  rotat);
    int     hh    = (x1 &  rotat) | (y1 & ~rotat);
    int     x2, y2;
    int     oldx, oldy;
    switch(opt->txo_xalign) {
      case GR_ALIGN_RIGHT:
        x -= ww - 1;
        break;
      case GR_ALIGN_CENTER:
        x -= (ww >> 1);
        break;
    }
    switch(opt->txo_yalign) {
      case GR_ALIGN_BASELINE:
        if(opt->txo_direct == GR_TEXT_DEFAULT) y -= f->h.baseline;
        break;
      case GR_ALIGN_BOTTOM:
        y -= hh - 1;
        break;
      case GR_ALIGN_CENTER:
        y -= (hh >> 1);
        break;
    }
    mouse_block(CURC,x,y,(x + ww - 1),(y + hh - 1));
    oldx = x + CURC->gc_xoffset;
    oldy = y + CURC->gc_yoffset;
    switch(opt->txo_direct) {
      case GR_TEXT_DOWN:
        dypost = ~0;
        break;
      case GR_TEXT_LEFT:
        dxpre = ~0;
        x += ww;
        break;
      case GR_TEXT_UP:
        dypre = ~0;
        y += hh;
        break;
      default:
        dxpost = ~0;
        break;
    }
    while(--length >= 0) {
      unsigned short chr;
      int xx,yy,cw,ch;
      char *bmp;
      chr = text[0];
      text = text + 1;
      x1 = GrFontCharWidth(f,chr);
      y1 = f->h.height;
      cw = (x1 & ~rotat) | (y1 &  rotat);
      ch = (x1 &  rotat) | (y1 & ~rotat);
      x1 = (x -= (cw & dxpre));
      y1 = (y -= (ch & dypre));
      x2 = (xx = x1) + cw - 1;
      y2 = (yy = y1) + ch - 1;
      x += (cw & dxpost);
      y += (ch & dypost);
      clip_ordbox_(CURC,x1,y1,x2,y2,continue,CLIP_EMPTY_MACRO_ARG);
      bmp = GrFontCharAuxBmp(f,chr,opt->txo_direct,undl);
      if(bmp) (*dbm)(
        (x1 + CURC->gc_xoffset),
        (y1 + CURC->gc_yoffset),
        (x2 - x1 + 1),
        (y2 - y1 + 1),
        oldx, oldy,
        bmp,
        ((cw + 7) >> 3),
        ((x1 - xx) + ((y1 - yy) * ((cw + 7) & ~7))),
        fgcv,bgcv,
        p
        );
      else (*FDRV->drawblock)(
        (x1 + CURC->gc_xoffset),
        (y1 + CURC->gc_yoffset),
        (x2 - x1 + 1),
        (y2 - y1 + 1),
        bgcv
        );
    }
    mouse_unblock();
  }
  GRX_LEAVE();
}

void _GrDrawString(const void *text, int length, int x, int y,
                   const GrTextOption *opt, GrPattern *p, TextDrawBitmapFunc dbm)
{
  unsigned short *text2;

  GRX_ENTER();
  if (text == NULL) return;
  if (length <= 0) length = GrStrLen(text, opt->txo_chrtype);
  text2 = GrFontTextRecode(opt->txo_font, text, length, opt->txo_chrtype);
  if (text2 == NULL) return;
  _GrDrawWordText(text2, length, x, y, opt, p, dbm);
  free(text2);
}

void _GrDrawChar(long chr, int x, int y,
                 const GrTextOption *opt, GrPattern *p, TextDrawBitmapFunc dbm)
{
  unsigned short schr;

  GRX_ENTER();
  schr = GrFontCharRecode(opt->txo_font, chr, opt->txo_chrtype);
  _GrDrawWordText(&schr, 1, x, y, opt, p, dbm);
}
