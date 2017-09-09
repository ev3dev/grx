/**
 ** iniencod.c ---- Initialice user encooding
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

static int userencoding = -1;

void _GrIniUserEncoding(void) {
  if (userencoding < 0) {
#if MGRX_VERSION == MGRX_VERSION_GCC_386_DJGPP
    userencoding = GRENC_CP437;
#elif MGRX_VERSION == MGRX_VERSION_GCC_386_WIN32
    userencoding = GRENC_CP1252;
#else
    userencoding = GRENC_UTF_8;
#endif
  }
}

int GrGetUserEncoding(void) {
  return userencoding;
}

int GrSetUserEncoding(int enc) {
  if (enc >= 0 && enc <= GRENC_LASTENCODE) {
    userencoding = enc;
    return 1;
  }
  return 0;
}

/* This function take advantage of the fact that the corresponding
 * type is user encoding + 2 !! (see defines en mgrx.h of GRENC_xxxs
 * and GR_xxxx_TEXT */

char GrGetChrtypeForUserEncoding(void) {
  if (userencoding >= 0 && userencoding <= GRENC_LASTENCODE)
    return (userencoding + 2);
  else
    return GR_BYTE_TEXT;
}
