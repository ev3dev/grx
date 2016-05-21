/*
 * drawtext.c ---- draw a character string with the default font
 *
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

#include <grx/text.h>

#include <string.h>
#include "libgrx.h"

void grx_draw_text_xy(int x,int y,char *text,GrxColor fg,GrxColor bg)
{
        GrxTextOptions opt;
        opt.txo_font      = &grx_font_default;
        opt.txo_fgcolor.v = fg;
        opt.txo_bgcolor.v = bg;
        opt.txo_chrtype   = GRX_CHAR_TYPE_BYTE;
        opt.txo_direct    = GRX_TEXT_DIRECTION_RIGHT;
        opt.txo_xalign    = GRX_TEXT_ALIGN_LEFT;
        opt.txo_yalign    = GRX_TEXT_VALIGN_TOP;
        grx_draw_string_with_text_options(text,(int)strlen(text),x,y,&opt);
}
