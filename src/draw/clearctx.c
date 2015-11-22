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
 ** 081105 Richard, grx_context_clear
 **
 **/

#include "libgrx.h"

void grx_clear_context(GrxColor bg)
{
  mouse_block(CURC,0,0,CURC->x_max,CURC->y_max);
  grx_draw_filled_box_nc(0,0,CURC->x_max,CURC->y_max,bg);
  mouse_unblock();
}

void grx_context_clear(GrxContext *ctx, GrxColor bg)
{
  GrxContext s;
  grx_context_save(&s);
  grx_context_set_current(ctx);
  grx_clear_context(bg);
  grx_context_set_current(&s);
}
