/*
 * text.h
 *
 * Copyright (c) 2015 David Lechner <david@lechnology.com>
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
 */

#ifndef __GRX_TEXT_H__
#define __GRX_TEXT_H__

#include <glib.h>
#include <glib-object.h>

#include <grx/color.h>
#include <grx/common.h>

/**
 * SECTION:text
 * @short_description: Fonts and text primitives
 * @title: Text Drawing
 * @section_id: text
 * @include: grx-3.0.h
 *
 * The library supports loadable fonts. When in memory they are bit-mapped (i.e.
 * not scalable!) fonts.
 */

/**
 * GrxTextHAlign:
 * @GRX_TEXT_HALIGN_LEFT: align text on the left
 * @GRX_TEXT_HALIGN_CENTER: align text in the center
 * @GRX_TEXT_HALIGN_RIGHT: align text on the right
 *
 * Horizontal text alignment
 */
typedef enum {
    GRX_TEXT_HALIGN_LEFT,
    GRX_TEXT_HALIGN_CENTER,
    GRX_TEXT_HALIGN_RIGHT,
} GrxTextHAlign;

/**
 * GrxTextVAlign:
 * @GRX_TEXT_VALIGN_TOP: align text on the top
 * @GRX_TEXT_VALIGN_MIDDLE: align text in the middle
 * @GRX_TEXT_VALIGN_BOTTOM: align text on the bottom
 * @GRX_TEXT_VALIGN_BASELINE: align text on the baseline
 *
 * Vertical text alignment
 */
typedef enum {
    GRX_TEXT_VALIGN_TOP,
    GRX_TEXT_VALIGN_MIDDLE,
    GRX_TEXT_VALIGN_BOTTOM,
    GRX_TEXT_VALIGN_BASELINE,
} GrxTextVAlign;

typedef struct _GrxFont GrxFont;

GType grx_font_get_type(void);

GrxFont *grx_font_load_from_file(const char *filename, GError **err);
GrxFont *grx_font_load_full(const gchar *family, gint size, const gchar *style, const gchar *lang, gint dpi, GError **err);
GrxFont *grx_font_load(const gchar *family, gint size, GError **err);

GrxFont *grx_font_ref(GrxFont *font);
void grx_font_unref(GrxFont *font);

const gchar *grx_font_get_family(const GrxFont *font);
const gchar *grx_font_get_style(const GrxFont *font);
gint grx_font_get_height(const GrxFont *font);
gint grx_font_get_width(const GrxFont *font);

gint grx_font_dump(const GrxFont *font, GrxContext *context, gint start, GrxColor fg, GrxColor bg);

gint grx_font_get_char_width(const GrxFont *font, gunichar c);
gint grx_font_get_char_height(const GrxFont *font, gunichar c);
gint grx_font_get_text_width(const GrxFont *font, const gchar *text);
gint grx_font_get_text_height(const GrxFont *font, const gchar *text);

void grx_draw_text(const gchar *text, gint x, gint y, const GrxFont *font, GrxColor fg, GrxColor bg, GrxTextHAlign halign, GrxTextVAlign valign);

#endif /* __GRX_TEXT_H__ */
