/**
 ** propwdt.c ---- calculate the width of a string using a proportional font
 **
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
 **
 **/

#include "libgrx.h"

int GrCharWidth(long chr, const GrTextOption *opt)
{
  long chr2;
  int r;

  chr2 = GrFontCharRecode(opt->txo_font, chr, opt->txo_chrtype);

  r = GR_TEXT_IS_VERTICAL(opt->txo_direct) ?
    GrFontCharHeight(opt->txo_font,chr2) : GrFontCharWidth(opt->txo_font,chr2);
      
  return r;
}

int GrCharHeight(long chr, const GrTextOption *opt)
{
  long chr2;
  int r;

  chr2 = GrFontCharRecode(opt->txo_font, chr, opt->txo_chrtype);

  r = GR_TEXT_IS_VERTICAL(opt->txo_direct) ?
    GrFontCharWidth(opt->txo_font,chr2) : GrFontCharHeight(opt->txo_font,chr2);

  return r;
}

void GrCharSize(long chr, const GrTextOption *opt,int *w,int *h)
{
  long chr2;
  int tw, th;

  chr2 = GrFontCharRecode(opt->txo_font, chr, opt->txo_chrtype);

  th = GrFontCharHeight(opt->txo_font,chr2);
  tw = GrFontCharWidth(opt->txo_font,chr2);

  if (GR_TEXT_IS_VERTICAL(opt->txo_direct)) {
    *w = th;
    *h = tw;
  }
  else {
    *w = tw;
    *h = th;
  }
}

int _GrFontWordTextWidth(const GrFont *font,const unsigned short *text,int len)
{
  int wdt = 0;
  int i;

  if (!font->h.proportional) return (font->h.width * len);
  
  for (i=0; i<len; i++)
    wdt += GrFontCharWidth(font,text[i]);
      
  return wdt;
}

int GrFontStringWidth(const GrFont *font,const void *text,int len,int chrtype)
{
  unsigned short *text2;
  int wdt = 0;
  int i;

  if (len <= 0) len = GrStrLen(text, chrtype);

  if (!font->h.proportional) return (font->h.width * len);

  text2 = GrFontTextRecode(font, text, len, chrtype);
  
  if (text2 != NULL) {
    for (i=0; i<len; i++)
      wdt += GrFontCharWidth(font,text2[i]);

    free(text2);
  }
      
  return wdt;
}

int GrFontStringHeight(const GrFont *font,const void *text,int len,int chrtype)
{
  return font->h.height;
}

int GrStringWidth(const void *text,int length,const GrTextOption *opt)
{
  return (GR_TEXT_IS_VERTICAL(opt->txo_direct) ?
          GrFontStringHeight(opt->txo_font,text,length,opt->txo_chrtype) :
          GrFontStringWidth(opt->txo_font,text,length,opt->txo_chrtype));
}

int GrStringHeight(const void *text,int length,const GrTextOption *opt)
{
  return (GR_TEXT_IS_VERTICAL(opt->txo_direct) ?
          GrFontStringWidth(opt->txo_font,text,length,opt->txo_chrtype) :
          GrFontStringHeight(opt->txo_font,text,length,opt->txo_chrtype));
}

void GrStringSize(const void *text,int length,const GrTextOption *opt,int *w,int *h)
{
  *w = GrStringWidth(text,length,opt);
  *h = GrStringHeight(text,length,opt);
}
