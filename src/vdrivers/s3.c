/**
 ** S3.C ---- the GRX 2.0 S3 driver
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 **/

#include <string.h>

#include "grdriver.h"
#include "libgrx.h"
#include "memcopy.h"
#include "ioport.h"
#include "highlow.h"

extern int _GrVidDrvVESAbanksft;

/* -------------------------------------------------------------------- */
/*  direct register access banking. Adopted from code written by  Finn  */
/*  Thoegersen (finth@datashopper.dk) and taken from whatvga v2.20      */

#define CRTC 0x3d4

#define WriteIndexed(pt,inx,val) outport_w((pt),highlow((val),(inx)))

static
#ifdef __GNUC__
inline
#endif
uchar ReadIndexed(ushort pt, uchar inx) {
  /* read register PT index INX */
  outport_b(pt,inx);
  return (uchar) inport_b(pt+1);
}

/* In register PT index INX sets the bits set in VAL */
#define SetIndexed(pt,inx,val) do {             \
    uchar _temp_val_ = ReadIndexed((pt),(inx)); \
    _temp_val_ |= (val);                        \
    WriteIndexed((pt), (inx), _temp_val_);      \
  } while (0)

/* In register PT index INX sets
** the bits in MASK as in NWV the other are left unchanged */
#define ModifyIndexed(pt,inx,mask,nwv) do {     \
    uchar _temp_val_ = ReadIndexed((pt),(inx)); \
    _temp_val_ &= ~(mask);                      \
    _temp_val_ |= (nwv) & (mask);               \
    WriteIndexed((pt), (inx), _temp_val_);      \
  } while (0)

static int TestIndexed(ushort pt, uchar rg, uchar msk)
{ /* Returns TRUE if the bits in MSK of register PT index RG are read/writable */
  uchar old,nw1,nw2;

  old = ReadIndexed(pt,rg);
  WriteIndexed(pt,rg,old & ~msk);
  nw1 = ReadIndexed(pt,rg) & msk;
  WriteIndexed(pt,rg,old | msk);
  nw2 = ReadIndexed(pt,rg) & msk;
  WriteIndexed(pt,rg,old);
  return (nw1==0) && (nw2==msk);
}

static uchar s3_revision(void) {
  uchar res = 0;
  WriteIndexed(CRTC,0x38,0);
  if (!TestIndexed(CRTC,0x35,0x0F)) {
    WriteIndexed(CRTC,0x38,0x48);
    if (TestIndexed(CRTC,0x35,0x0F)) {
      res = ReadIndexed(CRTC,0x30);
    }
  }
  return res;
}


/* general bank shift */
static void setbank_864(int bk) {
  WriteIndexed(CRTC,0x39,0xA5);
  WriteIndexed(CRTC,0x6A, bk << _GrVidDrvVESAbanksft);
  WriteIndexed(CRTC,0x39,0x5A);
}
/* no shift required */
static void setbank_864_0(int bk) {
  WriteIndexed(CRTC,0x39,0xA5);
  WriteIndexed(CRTC,0x6A, bk);
  WriteIndexed(CRTC,0x39,0x5A);
}

static void setbank_801(int bk) {
  bk <<= _GrVidDrvVESAbanksft;
  WriteIndexed(CRTC,0x39,0xA5);
  WriteIndexed(CRTC,0x38,0x48);
  SetIndexed(CRTC,0x31,9);
  ModifyIndexed(CRTC,0x35,0x0F,bk);
  ModifyIndexed(CRTC,0x51,0x0C,bk>>2);
  WriteIndexed(CRTC,0x38,0);
  WriteIndexed(CRTC,0x39,0x5A);
}

static void setbank_911(int bk) {
  WriteIndexed(CRTC,0x39,0xA5);
  WriteIndexed(CRTC,0x38,0x48);
  SetIndexed(CRTC,0x31,9);
  ModifyIndexed(CRTC,0x35,0x0F, bk << _GrVidDrvVESAbanksft);
  WriteIndexed(CRTC,0x38,0);
  WriteIndexed(CRTC,0x39,0x5A);
}

static int init(char *options) {
  int res, s3, i;
  void (*bank)(int bk) = NULL;

  res = _GrVideoDriverVESA.init(options);
  _GrVideoDriverS3.modes  = _GrVideoDriverVESA.modes;
  _GrVideoDriverS3.nmodes = _GrVideoDriverVESA.nmodes;
  s3 = s3_revision();
  if (s3 >= 0xc0) {
      /* 864 or newer */
      if (!_GrVidDrvVESAbanksft)
          bank = setbank_864_0; /* don't shift */
      else
          bank = setbank_864;   /* shift by _GrVidDrvVESAbanksft */
  } else
  if (s3 >= 0x90)
      bank = setbank_801; /* 801 or newer */
  else
  if (s3 >= 0x80)
      bank = setbank_911; /* 911 or newer */
  else {
    sttcopy(&_GrVideoDriverS3, &_GrVideoDriverVESA);
    return res;
  }

  /* step through all modes and select S3 banking */
  for (i=0; i < _GrVideoDriverS3.nmodes; ++i) {
    GrVideoMode *mp = &_GrVideoDriverS3.modes[i];
    if (mp->bpp >= 8 && mp->extinfo) {
      mp->extinfo->setbank    = bank;
      mp->extinfo->setrwbanks = NULL;
    }
  }
  return res;
}

static void reset(void) {
  _GrVideoDriverVESA.reset();
}

GrVideoDriver _GrVideoDriverS3 = {
    "S3",                               /* name */
    GR_VGA,                             /* adapter type */
    &_GrVideoDriverSTDVGA,              /* inherit modes from this driver */
    NULL,                               /* mode table */
    0,                                  /* # of modes */
    NULL,                               /* detection routine */
    init,                               /* initialization routine */
    reset                               /* reset routine */
};
