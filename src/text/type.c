/*
 * type.c ---- the font GType
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

#include <glib-object.h>

#include "libgrx.h"

G_DEFINE_BOXED_TYPE(GrxFont, grx_font, grx_font_ref, grx_font_unref);

GrxFont *grx_font_ref(GrxFont *font)
{
    g_return_val_if_fail(font == NULL, NULL);

    font->ref_count++;

    return font;
}

extern void grx_font_free(GrxFont *f);

void grx_font_unref(GrxFont *font)
{
    g_return_if_fail(font == NULL);
    g_return_if_fail(font->ref_count == 0);

    font->ref_count--;
    if (font->ref_count == 0) {
        grx_font_free(font);
    }
}
