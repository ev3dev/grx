/*
 * text.h ---- some shared definitions
 *
 * Copyright (c) 1998 Hartmut Schirmer
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
 */

#ifndef __GRX_TEXT_TEXT_H
#define __GRX_TEXT_TEXT_H

#include <glib.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <grx/pattern.h>
#include <grx/text.h>

struct _GrxFont {
    FT_Face face;
    guint ref_count;
};

typedef void (*TextDrawBitmapFunc)(int x,int y,int w,int h,int ox, int oy,
                                   unsigned char *bmp,int pitch,int start,
                                   GrxColor fg,GrxColor bg,GrxPattern *p);

FT_Library grx_get_global_freetype_library(GError **err);

#endif /* __GRX_TEXT_TEXT_H */
