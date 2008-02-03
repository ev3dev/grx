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
 **
 **/

#include "libgrx.h"

int GrProportionalTextWidth(const GrFont *font,const void *text,int len,int type)
{
  char *txp;
  unsigned short *text2 = NULL;
  int len2;
  int wdt = 0;

  if (type == GR_UTF8_TEXT) {
    text2 = GrUTF8StrToUCS2Str((unsigned char *)text, &len2);
    if (text2 == NULL) return 0;
    type = GR_WORD_TEXT;
    if (len2 < len) len = len2;
    txp = (char *)text2;
  }
  else {
    text2 = NULL;
    txp = (char *)text;
  }

  while(--len >= 0) {
    wdt += GrFontCharWidth(font,GR_TEXTSTR_CODE(txp,type));
    txp += GR_TEXTCHR_SIZE(type);
  }

  if (text2) free(text2);

  return (wdt);
}

int GrCharWidth(long chr, const GrTextOption *opt)
{
  long chr2;
  int r;

  if (opt->txo_chrtype == GR_UTF8_TEXT) {
    chr2 = GrUTF8ToUCS2((unsigned char *)&chr);
    r = GR_TEXT_IS_VERTICAL(opt->txo_direct) ?                               \
        GrFontCharHeight(opt->txo_font,GR_TEXTCHR_CODE(chr2,GR_WORD_TEXT)) : \
        GrFontCharWidth(opt->txo_font,GR_TEXTCHR_CODE(chr2,GR_WORD_TEXT));
    return r;
  }

  r = GR_TEXT_IS_VERTICAL(opt->txo_direct) ?                                  \
      GrFontCharHeight(opt->txo_font,GR_TEXTCHR_CODE(chr,opt->txo_chrtype)) : \
      GrFontCharWidth(opt->txo_font,GR_TEXTCHR_CODE(chr,opt->txo_chrtype));
  return r;
}

int GrCharHeight(long chr, const GrTextOption *opt)
{
  long chr2;
  int r;

  if (opt->txo_chrtype == GR_UTF8_TEXT) {
    chr2 = GrUTF8ToUCS2((unsigned char *)&chr);
    r = GR_TEXT_IS_VERTICAL(opt->txo_direct) ?                               \
        GrFontCharWidth(opt->txo_font,GR_TEXTCHR_CODE(chr2,GR_WORD_TEXT)) :  \
        GrFontCharHeight(opt->txo_font,GR_TEXTCHR_CODE(chr2,GR_WORD_TEXT));
    return r;
  }

  r = GR_TEXT_IS_VERTICAL(opt->txo_direct) ?                                  \
      GrFontCharWidth(opt->txo_font,GR_TEXTCHR_CODE(chr,opt->txo_chrtype)) :  \
      GrFontCharHeight(opt->txo_font,GR_TEXTCHR_CODE(chr,opt->txo_chrtype));
  return r;
}

void GrCharSize(long chr, const GrTextOption *opt,int *w,int *h)
{
  long chr2;
  int tw, th;

  if (opt->txo_chrtype == GR_UTF8_TEXT) {
    chr2 = GrUTF8ToUCS2((unsigned char *)&chr);
    th = GrFontCharHeight(opt->txo_font,GR_TEXTCHR_CODE(chr2,GR_WORD_TEXT));
    tw = GrFontCharWidth(opt->txo_font,GR_TEXTCHR_CODE(chr2,GR_WORD_TEXT));
  }
  else {
    th = GrFontCharHeight(opt->txo_font,GR_TEXTCHR_CODE(chr,opt->txo_chrtype));
    tw = GrFontCharWidth(opt->txo_font,GR_TEXTCHR_CODE(chr,opt->txo_chrtype));
  }

  if (GR_TEXT_IS_VERTICAL(opt->txo_direct)) {
    *w = th;
    *h = tw;
  }
  else {
    *w = tw;
    *h = th;
  }
}
