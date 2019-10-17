/**
 ** frecode.c ---- recode font functions
 **
 ** Copyright (c) 2017 Mariano Alvarez Fernandez
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
 **/

#include "libgrx.h"
#include "text/text.h"
 
void GrFontSetEncoding(GrFont *font,int fontencoding)
{
  if (font == NULL) return;
  if (fontencoding >= 0 && fontencoding <= GR_FONTENC_LASTENC)
    font->h.encoding = fontencoding;
}

int GrFontNeedRecode(const GrFont *font,int chrtype)
{
  if (font == NULL) return 0;
  if (font->h.encoding == GR_FONTENC_UNKNOWN) return 0;
  if (chrtype == GR_BYTE_TEXT) return 0;
  if (chrtype == GR_WORD_TEXT) return 0;
  if (font->h.encoding >= GR_FONTENC_CP437 &&
      font->h.encoding <= GR_FONTENC_ISO_8859_1 &&
      font->h.encoding == chrtype - 1) return 0;
  if (font->h.encoding == GR_FONTENC_UNICODE &&
      (chrtype == GR_UTF8_TEXT || chrtype == GR_UCS2_TEXT)) return 0;
  return 1;
}

static unsigned int _GrCharRecode(int fontenc,long chr,int chrtype)
{
  long aux, aux2;
  unsigned char caux;
  
  aux = '?';
  switch (chrtype) {
  case GR_CP437_TEXT:
    _GrRecode_CP437_UCS2((unsigned char)chr, &aux);
    break;
  case GR_CP850_TEXT:
    _GrRecode_CP850_UCS2((unsigned char)chr, &aux);
    break;
  case GR_CP1252_TEXT:
    _GrRecode_CP1252_UCS2((unsigned char)chr, &aux);
    break;
  case GR_ISO_8859_1_TEXT:
    _GrRecode_ISO88591_UCS2((unsigned char)chr, &aux);
    break;
  case GR_UTF8_TEXT:
  case GR_UCS2_TEXT:
    aux = chr;
    break;
  }

  caux = '?';
  aux2 = '?';
  switch (fontenc) {
  case GR_FONTENC_CP437:
    _GrRecode_UCS2_CP437(aux, &caux);
    return caux;
  case GR_FONTENC_CP850:
    _GrRecode_UCS2_CP850(aux, &caux);
    return caux;
  case GR_FONTENC_CP1252:
    _GrRecode_UCS2_CP1252(aux, &caux);
    return caux;
  case GR_FONTENC_ISO_8859_1:
    _GrRecode_UCS2_ISO88591(aux, &caux);
    return caux;
  case GR_FONTENC_MGRX512:
    _GrRecode_UCS2_mgrx512(aux, &aux2);
    return aux2;
  case GR_FONTENC_UNICODE:
    return aux;
  }
  
  return chr;
}

unsigned int GrFontCharRecode(const GrFont *font,long chr,int chrtype)
{
  long aux;
  
  if (chrtype == GR_UTF8_TEXT) {
    aux = GrUTF8ToUCS2((unsigned char *)(&chr));
    chr = aux;
//    chrtype = GR_UCS2_TEXT;
  }

  if (!GrFontNeedRecode(font, chrtype)) return chr;
  
  return _GrCharRecode(font->h.encoding, chr, chrtype);
}

unsigned short *GrFontTextRecode(const GrFont *font,const void *text,int length,int chrtype)
{
  unsigned short *buf, *buf2;
  int i, len2;

  if (length <= 0) length = 0;

  buf = NULL;
  switch (chrtype) {
  case GR_BYTE_TEXT:
  case GR_CP437_TEXT:
  case GR_CP850_TEXT:
  case GR_CP1252_TEXT:
  case GR_ISO_8859_1_TEXT:
    buf = calloc(length+1, sizeof(unsigned short));
    if (buf == NULL) return NULL;
    for (i=0; i<length; i++) buf[i] = ((unsigned char *)text)[i];
    break;
  case GR_WORD_TEXT:
  case GR_UCS2_TEXT:
    buf = calloc(length+1, sizeof(unsigned short));
    if (buf == NULL) return NULL;
    for (i=0; i<length; i++) buf[i] = ((unsigned short *)text)[i];
    break;
  case GR_UTF8_TEXT:
    if (length == 0) {
        buf = calloc(1, sizeof(unsigned short));
        break;
    }
    buf = GrUTF8StrToUCS2Str((unsigned char *)text, &len2, length);
    if (buf == NULL) return NULL;
    if (len2 < length) { /* better that return NULL we add '?' */
                         /* so programmer can see he has a bug */
      buf2 = realloc(buf, (length+1) * sizeof(unsigned short));
      if (buf2 == NULL) {
          free(buf);
          return NULL;
      }
      buf = buf2;
      for (i=len2; i<length; i++) buf[i] = '?';
      buf[length] = 0;
    }
    break;
  }
  
  if (!GrFontNeedRecode(font, chrtype)) return buf;

  for (i=0; i<length; i++) {
    buf[i] = _GrCharRecode(font->h.encoding, buf[i], chrtype);
  }
  
  return buf;
}

static long _GrCharReverseRecode(int fontenc,unsigned int chr,int chrtype)
{
  long aux, aux2;
  unsigned char caux;
  
  aux = '?';
  switch (fontenc) {
  case GR_FONTENC_CP437:
    _GrRecode_CP437_UCS2(chr, &aux);
    break;
  case GR_FONTENC_CP850:
    _GrRecode_CP850_UCS2(chr, &aux);
    break;
  case GR_FONTENC_CP1252:
    _GrRecode_CP1252_UCS2(chr, &aux);
    break;
  case GR_FONTENC_ISO_8859_1:
    _GrRecode_ISO88591_UCS2(chr, &aux);
    break;
  case GR_FONTENC_MGRX512:
    _GrRecode_mgrx512_UCS2(chr, &aux);
    break;
  case GR_FONTENC_UNICODE:
    aux = chr;
    break;
  }

  caux = '?';
  switch (chrtype) {
  case GR_CP437_TEXT:
    _GrRecode_UCS2_CP437(aux, &caux);
    return caux;
  case GR_CP850_TEXT:
    _GrRecode_UCS2_CP850(aux, &caux);
    return caux;
  case GR_CP1252_TEXT:
    _GrRecode_UCS2_CP1252(aux, &caux);
    return caux;
  case GR_ISO_8859_1_TEXT:
    _GrRecode_UCS2_ISO88591(aux, &caux);
    return caux;
  case GR_UTF8_TEXT:
    _GrRecode_UCS2_UTF8(aux, (unsigned char *)&aux2);
    return aux2;
  case GR_UCS2_TEXT:
    return aux;
  }
  
  return chr;
}

long GrFontCharReverseRecode(const GrFont *font,unsigned int chr,int chrtype)
{
  if (!GrFontNeedRecode(font, chrtype)) return chr;
  
  return _GrCharReverseRecode(font->h.encoding, chr, chrtype);
}

