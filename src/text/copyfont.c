/*
 * copyfont.c ---- font copy function
 *
 * Copyright (c) 2015 David Lechner <david@lechnology.com>
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

GrxFont *grx_font_copy(GrxFont *font)
{
    if (!font) {
        return NULL;
    }
    return grx_font_build_converted(font, GRX_FONT_CONV_FLAG_NONE, font->h.width,
                                    font->h.height, font->h.minchar,
                                    font->h.minchar + font->h.numchars - 1);
}
