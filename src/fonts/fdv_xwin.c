/**
 ** FDV_XWIN.C -- driver X Windows fonts
 **
 ** Author:     Ulrich Leodolter
 ** E-mail:     ulrich@lab1.psy.univie.ac.at
 ** Date:       Thu Sep 28 11:21:46 1995
 ** RCSId:      $Id$
 **/

#include <stdio.h>
#include <string.h>

#include "libgrx.h"
#include "libxwin.h"
#include "grfontdv.h"
#include "allocate.h"

static XFontStruct *    fontp = NULL;
static Pixmap           fontbmp = None;
static GC               fontgc = None;

static unsigned char    swap_byte[256];
static unsigned char    swap_byte_inited = 0;

static void init_swap_byte (void)
{
  if (!swap_byte_inited) {
    unsigned int i;
    for (i = 0; i < 256; i++) {
      swap_byte[i] = (  ((i&0x01)<<7) | ((i&0x02)<<5)
                      | ((i&0x04)<<3) | ((i&0x08)<<1)
                      | ((i&0x10)>>1) | ((i&0x20)>>3)
                      | ((i&0x40)>>5) | ((i&0x80)>>7));
    }
    swap_byte_inited = 1;
  }
}

static void cleanup(void)
{
  if (_XGrDisplay != NULL) {
    if (fontp != NULL) XFreeFont (_XGrDisplay, fontp);
    if (fontbmp != None) XFreePixmap (_XGrDisplay, fontbmp);
    if (fontgc != None) XFreeGC (_XGrDisplay, fontgc);
  }
  fontp = NULL;
  fontbmp = None;
  fontgc = None;
}

static int openfile(char *fname)
{
  int i, numchars;

  init_swap_byte();
  cleanup();
  if (_XGrDisplay != NULL) fontp = XLoadQueryFont (_XGrDisplay, fname);
  if (fontp == NULL)    return(cleanup(), FALSE);

  numchars = fontp->max_char_or_byte2 - fontp->min_char_or_byte2 + 1;
  fontbmp = XCreatePixmap (_XGrDisplay, _XGrWindow,
                           numchars * fontp->max_bounds.width,
                           fontp->ascent + fontp->descent, 1);
  if (fontbmp == None)  return(cleanup(), FALSE);
  fontgc = XCreateGC (_XGrDisplay, fontbmp, 0L, NULL);
  if (fontgc == None)   return(cleanup(), FALSE);
  XSetFont (_XGrDisplay, fontgc, fontp->fid);
  XSetForeground (_XGrDisplay, fontgc, 0);
  XFillRectangle (_XGrDisplay, fontbmp, fontgc, 0, 0,
                  numchars * fontp->max_bounds.width,
                  fontp->ascent + fontp->descent);
  XSetForeground (_XGrDisplay, fontgc, 1);
  XSetBackground (_XGrDisplay, fontgc, 0);
  for (i = 0; i < numchars; i++) {
    char c = fontp->min_char_or_byte2 + i;
    XDrawString (_XGrDisplay, fontbmp, fontgc,
                 i * fontp->max_bounds.width,
                 fontp->ascent, &c, 1);
  }
  return(TRUE);
}

static int header(GrFontHeader *hdr)
{
  if (fontp == NULL) return(FALSE);
  strcpy(hdr->name, "");
  strcpy(hdr->family, "xwin");
  hdr->proportional = (fontp->per_char == NULL) ? FALSE : TRUE;
  hdr->scalable   = FALSE;
  hdr->preloaded  = FALSE;
  hdr->modified   = GR_FONTCVT_NONE;
  hdr->width      = fontp->max_bounds.width;
  hdr->height     = fontp->ascent + fontp->descent;
  hdr->baseline   = fontp->ascent;
  hdr->ulpos      = fontp->ascent;
  hdr->ulheight   = fontp->descent;
  hdr->minchar    = fontp->min_char_or_byte2;
  hdr->numchars   = fontp->max_char_or_byte2 - fontp->min_char_or_byte2 + 1;
  return(TRUE);
}

static int charwdt(int chr)
{
  int width;
  if (fontp == NULL)    return(-1);
  if (chr < fontp->min_char_or_byte2)   return(-1);
  if (chr > fontp->max_char_or_byte2)   return(-1);
  if (fontp->per_char == NULL)          return(fontp->max_bounds.width);
  width = fontp->per_char[chr - fontp->min_char_or_byte2].width;
  if (width <= 0)
    return fontp->per_char[fontp->default_char - fontp->min_char_or_byte2].width;;
  return(width);
}

static int bitmap(int chr,int w,int h,char *buffer)
{
  XImage *img;
  int x, y, bpl;
  unsigned char *data;

  if (fontp == NULL || fontbmp == None) return(FALSE);
  if ((w <= 0) || (w != charwdt(chr)))  return(FALSE);
  if ((h <= 0) || (h != (fontp->ascent + fontp->descent)))      return(FALSE);
  if (_XGrDisplay == NULL)      return(FALSE);
  img = XGetImage (_XGrDisplay,
                   fontbmp,
                   (chr - fontp->min_char_or_byte2) * fontp->max_bounds.width,
                   0,
                   w,
                   h,
                   AllPlanes,
                   ZPixmap);
  if (img == NULL)              return(FALSE);
  data = (unsigned char *)(img->data);
  bpl = (w + 7) >> 3;
  for (y = 0; y < h; y++) {
    for (x = 0; x < bpl; x++) buffer[x] = swap_byte[data[x]];
    buffer += bpl;
    data += img->bytes_per_line;
  }
  XDestroyImage (img);
  return(TRUE);
}

GrFontDriver _GrFontDriverXWIN = {
    "XWIN",                             /* driver name (doc only) */
    "",                                 /* font file extension */
    FALSE,                              /* scalable */
    openfile,                           /* file open and check routine */
    header,                             /* font header reader routine */
    charwdt,                            /* character width reader routine */
    bitmap,                             /* character bitmap reader routine */
    cleanup                             /* cleanup routine */
};

