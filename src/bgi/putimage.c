/**
 ** BCC2GRX  -  Interfacing Borland based graphics programs to LIBGRX
 ** Copyright (C) 1993-97 by Hartmut Schirmer
 **
 **
 ** Contact :                Hartmut Schirmer
 **                          Feldstrasse 118
 **                  D-24105 Kiel
 **                          Germany
 **
 ** e-mail : hsc@techfak.uni-kiel.de
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

#include "bccgrx00.h"

/* ----------------------------------------------------------------- */
void putimage(int left, int top, void *bitmap, int op)
{
  GrContext *gc;

  _DO_INIT_CHECK;
  GrSetContext(NULL);
  GrResetClipBox();
  gc = bitmap;
  switch (op) {
    case XOR_PUT  : op = GrXorModeColor(0);   break;
    case OR_PUT   : op = GrOrModeColor(0);    break;
    case AND_PUT  : op = GrAndModeColor(0);   break;
    default       : op = GrWriteModeColor(0); break;
  }
  GrBitBlt( NULL, left+VL, top+VT+PY, gc, 0, 0, gc->gc_xmax, gc->gc_ymax, op);
  __gr_Reset_ClipBox();
}

