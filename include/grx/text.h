/*
 * text.h
 *
 * Copyright (c) 2015,2017 David Lechner <david@lechnology.com>
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
 * The library supports loadable fonts. When in memory they are bitmapped (i.e.
 * not scalable!) fonts.
 *
 * Under the hood, it uses fontconfig and freetype2. This means than fontconfig
 * must be configured to allow using bitmap fonts, otherwise grx_font_load()
 * will not be able to find any fonts. You can also use the fontconfig command
 * line tools to help find availble fonts. For example, `fc-list ":scalable=False"`
 * will list all bitmap fonts.
 *
 * To bypass fontconfig, use grx_font_load_from_file() instead.
 */

/**
 * GrxFontWeight:
 * GRX_FONT_WEIGHT_REGULAR: regular font
 * GRX_FONT_WEIGHT_BOLD: bold font
 *
 * Describes the weight of the font.
 */
typedef enum {
    GRX_FONT_WEIGHT_REGULAR,
    GRX_FONT_WEIGHT_BOLD,
} GrxFontWeight;

/**
 * GrxFontSlant:
 * GRX_FONT_SLANT_REGULAR: regular font
 * GRX_FONT_SLANT_ITALIC: italic font
 *
 * Describes the slant of the font.
 */
typedef enum {
    GRX_FONT_SLANT_REGULAR,
    GRX_FONT_SLANT_ITALIC,
} GrxFontSlant;

/**
 * GrxFontWidth:
 * GRX_FONT_WIDTH_NARROW: narrow font
 * GRX_FONT_WIDTH_REGULAR: regular font
 * GRX_FONT_WIDTH_WIDE: wide font
 *
 * Describes the slant of the font.
 */
typedef enum {
    GRX_FONT_WIDTH_NARROW,
    GRX_FONT_WIDTH_REGULAR,
    GRX_FONT_WIDTH_WIDE,
} GrxFontWidth;

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
typedef struct _GrxTextOptions GrxTextOptions;

GType grx_font_get_type(void);

GrxFont *grx_font_load_from_file(const gchar *filename, GError **err);
GrxFont *grx_font_load_full(const gchar *family, gint size, gint dpi, GrxFontWeight weight,
    GrxFontSlant slant, GrxFontWidth width, gboolean monospace, const gchar *lang,
    const gchar *script, GError **err);
GrxFont *grx_font_load(const gchar *family, gint size, GError **err);

GrxFont *grx_font_ref(GrxFont *font);
void grx_font_unref(GrxFont *font);

const gchar *grx_font_get_family(GrxFont *font);
const gchar *grx_font_get_style(GrxFont *font);
gint grx_font_get_height(GrxFont *font);
gint grx_font_get_width(GrxFont *font);

gint grx_font_dump(GrxFont *font, GrxContext *context, gint start, GrxColor fg, GrxColor bg);

gint grx_font_get_char_width(GrxFont *font, gunichar c);
gint grx_font_get_char_height(GrxFont *font, gunichar c);
gint grx_font_get_text_width(GrxFont *font, const gchar *text);
gint grx_font_get_text_height(GrxFont *font, const gchar *text);

void grx_draw_text(const gchar *text, gint x, gint y, GrxTextOptions *options);

GType grx_text_options_get_type(void);
GrxTextOptions *grx_text_options_new(GrxFont *font, GrxColor fg);
GrxTextOptions *grx_text_options_new_full(GrxFont *font, GrxColor fg, GrxColor bg,
                                          GrxTextHAlign h_align, GrxTextVAlign v_align);
GrxTextOptions *grx_text_options_ref(GrxTextOptions *options);
void grx_text_options_unref(GrxTextOptions *options);
GrxFont *grx_text_options_get_font(GrxTextOptions *options);
void grx_text_options_set_font(GrxTextOptions *options, GrxFont *font);
GrxColor grx_text_options_get_fg_color(GrxTextOptions *options);
void grx_text_options_set_fg_color(GrxTextOptions *options, GrxColor fg);
GrxColor grx_text_options_get_bg_color(GrxTextOptions *options);
void grx_text_options_set_bg_color(GrxTextOptions *options, GrxColor bg);
GrxTextHAlign grx_text_options_get_h_align(GrxTextOptions *options);
void grx_text_options_set_h_align(GrxTextOptions *options, GrxTextHAlign h_align);
GrxTextVAlign grx_text_options_get_v_align(GrxTextOptions *options);
void grx_text_options_set_v_align(GrxTextOptions *options, GrxTextVAlign v_align);

#endif /* __GRX_TEXT_H__ */
