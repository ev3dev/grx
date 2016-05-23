/*
 * epatstrg.c ---- extended patterned draw character string
 *
 * Copyright (c) 1998 Hartmut Schirmer
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
 *
 * This file is part of the GRX graphics library.
 *
 * The GRX graphics library is free software; you can redistribute it
 * and/or modify it under some conditions; see the "copying.grx" file
 * for details.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <grx/extents.h>

#include "libgrx.h"
#include "clipping.h"
#include "shapes.h"
#include "text.h"

static void ExtPatternFilledBmp(int x,int y,int w,int h,int ox, int oy,
                                char *bmp,int pitch,int start,
                                GrxColor fg,GrxColor bg,GrxPattern *p)
{
  GRX_ENTER();
  _GrFillBitmapPatternExt(x,y,w,h,ox,oy,bmp,pitch,start,p,bg);
  GRX_LEAVE();
}

void grx_draw_string_with_pattern_ext(void *text,int length,int x,int y,
                               const GrxTextOptions *opt,GrxPattern *p)
{
  GRX_ENTER();
  _GrDrawString(text,length,x,y,opt,p,ExtPatternFilledBmp);
  GRX_LEAVE();
}
