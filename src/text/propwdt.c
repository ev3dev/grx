/*
 * propwdt.c ---- calculate the width of a string using a proportional font
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

#include "libgrx.h"

int grx_font_get_proportional_text_width(const GrxFont *font,const void *text,int len,GrxCharType type)
{
        char *txp = (char *)text;
        int   wdt = 0;
        while(--len >= 0) {
            wdt += grx_font_get_char_width(font,GRX_CHAR_TYPE_GET_CODE_STR(type, txp));
            txp += GRX_CHAR_TYPE_GET_SIZE(type);
        }
        return(wdt);
}
