/*
 * options.c - the text options GType
 *
 * Copyright (c) 2017 David Lechner <david@lechnology.com>
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

#include <glib-object.h>

#include <grx/text.h>

#include "text.h"

G_DEFINE_BOXED_TYPE(GrxTextOptions, grx_text_options, grx_text_options_ref, grx_text_options_unref);

/**
 * grx_text_options_new:
 * @font: the font
 * @fg: the forground color
 *
 * Allocate a text options structure with background color set to #GRX_COLOR_NONE,
 * horizontal alignment of #GRX_TEXT_HALIGN_LEFT and vertival alignment of
 * #GRX_TEXT_VALIGN_TOP.
 *
 * This structure is passed to text drawing functions to provide information on
 * how to draw the text.
 *
 * Returns: (transfer full): the new #GrxTextOptions structure
 */
GrxTextOptions *grx_text_options_new(GrxFont *font, GrxColor fg)
{
    return grx_text_options_new_full(font, fg, GRX_COLOR_NONE, GRX_TEXT_HALIGN_LEFT, GRX_TEXT_VALIGN_TOP);
}

/**
 * grx_text_options_new_full_full:
 * @font: the font
 * @fg: the forground color
 * @bg: the background color
 * @h_align: the horizontal alignment
 * @v_align: the vertical alignment
 *
 * Allocate a text options structure.
 *
 * This structure is passed to text drawing functions to provide information on
 * how to draw the text.
 *
 * Returns: (transfer full): the new #GrxTextOptions structure
 */
GrxTextOptions *grx_text_options_new_full(GrxFont *font, GrxColor fg, GrxColor bg, GrxTextHAlign h_align, GrxTextVAlign v_align)
{
    GrxTextOptions *options;

    g_return_val_if_fail(font != NULL, NULL);

    options = g_malloc(sizeof(*options));
    options->font = grx_font_ref(font);
    options->fg_color = fg;
    options->bg_color = bg;
    options->h_align = h_align;
    options->v_align = v_align;
    options->ref_count = 0;

    return grx_text_options_ref(options);
}

/**
 * grx_text_options_ref:
 * @options: the options
 *
 * Increases the reference count to @options.
 *
 * Returns: (transfer full): @options
 */
GrxTextOptions *grx_text_options_ref(GrxTextOptions *options)
{
    g_return_val_if_fail(options != NULL, NULL);

    options->ref_count++;

    return options;
}

/**
 * grx_text_options_unref:
 * @options: (transfer full): the options
 *
 * Decreases the reference count on @options.
 *
 * When the reference count reaches 0, @options is freed.
 */
void grx_text_options_unref(GrxTextOptions *options)
{
    g_return_if_fail(options != NULL);
    g_return_if_fail(options->ref_count != 0);

    options->ref_count--;
    if (options->ref_count == 0) {
        grx_font_unref(options->font);
        g_free(options);
    }
}

/**
 * grx_text_options_get_font:
 * @options: the options
 *
 * Gets the font.
 *
 * Returns: (transfer none): the font
 */
GrxFont *grx_text_options_get_font(GrxTextOptions *options)
{
    g_return_val_if_fail(options != NULL, NULL);

    return options->font;
}

/**
 * grx_text_options_set_font:
 * @options: the options
 * @font: the font
 *
 * Sets the font.
 */
void grx_text_options_set_font(GrxTextOptions *options, GrxFont *font)
{
    g_return_if_fail(options != NULL);
    g_return_if_fail(font != NULL);

    grx_font_unref(options->font);
    options->font = grx_font_ref(font);
}

/**
 * grx_text_options_get_fg_color:
 * @options: the options
 *
 * Gets the forground color.
 *
 * Returns: the color
 */
GrxColor grx_text_options_get_fg_color(GrxTextOptions *options)
{
    g_return_val_if_fail(options != NULL, 0);

    return options->fg_color;
}

/**
 * grx_text_options_set_fg_color:
 * @options: the options
 * @fg: the color
 *
 * Sets the forground color.
 */
void grx_text_options_set_fg_color(GrxTextOptions *options, GrxColor fg)
{
    g_return_if_fail(options != NULL);

    options->fg_color = fg;
}

/**
 * grx_text_options_get_bg_color:
 * @options: the options
 *
 * Gets the background color.
 *
 * Returns: the color
 */
GrxColor grx_text_options_get_bg_color(GrxTextOptions *options)
{
    g_return_val_if_fail(options != NULL, 0);

    return options->bg_color;
}

/**
 * grx_text_options_set_bg_color:
 * @options: the options
 * @bg: the color
 *
 * Sets the background color.
 */
void grx_text_options_set_bg_color(GrxTextOptions *options, GrxColor bg)
{
    g_return_if_fail(options != NULL);

    options->bg_color = bg;
}

/**
 * grx_text_options_get_h_align:
 * @options: the options
 *
 * Gets the horizontal alignment.
 *
 * Returns: the alignment
 */
GrxTextHAlign grx_text_options_get_h_align(GrxTextOptions *options)
{
    g_return_val_if_fail(options != NULL, 0);

    return options->h_align;
}

/**
 * grx_text_options_set_h_align:
 * @options: the options
 * @h_align: the alignment
 *
 * Sets the horizontal alignment.
 */
void grx_text_options_set_h_align(GrxTextOptions *options, GrxTextHAlign h_align)
{
    g_return_if_fail(options != NULL);

    options->h_align = h_align;
}

/**
 * grx_text_options_get_v_align:
 * @options: the options
 *
 * Gets the vertical alignment.
 *
 * Returns: the alignment
 */
GrxTextVAlign grx_text_options_get_v_align(GrxTextOptions *options)
{
    g_return_val_if_fail(options != NULL, 0);

    return options->v_align;
}

/**
 * grx_text_options_set_v_align:
 * @options: the options
 * @v_align: the alignment
 *
 * Sets the vertical alignment.
 */
void grx_text_options_set_v_align(GrxTextOptions *options, GrxTextVAlign v_align)
{
    g_return_if_fail(options != NULL);

    options->v_align = v_align;
}
