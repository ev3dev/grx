/**
 ** clearctx.c ---- clear context
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu].
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
 ** 081105 Richard, GrClearContextC
 **
 **/

#include "libgrx.h"

void GrClearContext(GrxColor bg)
{
  mouse_block(CURC,0,0,CURC->x_max,CURC->y_max);
  GrFilledBoxNC(0,0,CURC->x_max,CURC->y_max,bg);
  mouse_unblock();
}

void GrClearContextC(GrxContext *ctx, GrxColor bg)
{
  GrxContext s;
  GrSaveContext(&s);
  GrSetContext(ctx);
  GrClearContext(bg);
  GrSetContext(&s);
}
