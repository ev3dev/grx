/*
 * drawtext.c - draw a character string
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

#include <ft2build.h>
#include <glib.h>
#include FT_FREETYPE_H

#include <string.h>

#include <grx/context.h>
#include <grx/draw.h>
#include <grx/frame_mode.h>
#include <grx/text.h>

#include "libgrx.h"
#include "text.h"

/**
 * grx_draw_text:
 * @text: The text to draw
 * @x: The x coordinate
 * @y: The y coordinate
 * @options: #GrxTextOptions to control how the text is drawn
 *
 * Draws the text at the specified coordinates using @options.
 *
 * The position of the text relative to the coordinates will depend on the
 * alignment given in @options.
 */
void grx_draw_text(const gchar *text, gint x, gint y, GrxTextOptions *options)
{
    FT_GlyphSlot slot;
    GrxContext ctx;
    gint x_offset, y_offset;
    gunichar c;
    FT_UInt index;
    FT_Error ret;

    g_return_if_fail(text != NULL);
    g_return_if_fail(options != NULL);
    g_return_if_fail(options->font != NULL);

    slot = options->font->face->glyph;

    // FIXME: we are iterating the text multiple times here
    switch (options->h_align) {
    case GRX_TEXT_HALIGN_LEFT:
        x_offset = 0;
        break;
    case GRX_TEXT_HALIGN_CENTER:
        x_offset = grx_font_get_text_width(options->font, text) / 2;
        break;
    case GRX_TEXT_HALIGN_RIGHT:
        x_offset = grx_font_get_text_width(options->font, text);
        break;
    default:
        g_return_if_reached();
    }

    switch (options->v_align) {
    case GRX_TEXT_VALIGN_TOP:
        y_offset = options->font->face->size->metrics.ascender >> 6;
        break;
    case GRX_TEXT_VALIGN_MIDDLE:
        y_offset = (options->font->face->size->metrics.height >> 6) / 2;
        break;
    case GRX_TEXT_VALIGN_BASELINE:
        y_offset = 0;
        break;
    case GRX_TEXT_VALIGN_BOTTOM:
        y_offset = options->font->face->size->metrics.descender >> 6;
        break;
    default:
        g_return_if_reached();
    }

    for (; (c = g_utf8_get_char(text)) != '\0'; text = g_utf8_next_char(text)) {
        index = FT_Get_Char_Index(options->font->face, c);
        ret = FT_Load_Glyph(options->font->face, index, FT_LOAD_DEFAULT);
        if (ret) {
            continue;
        }
        // ret = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
        // if (ret) {
        //     continue;
        // }
        if (slot->format != FT_GLYPH_FORMAT_BITMAP) {
            continue;
        }
        if (slot->bitmap.pixel_mode != FT_PIXEL_MODE_MONO) {
            continue;
        }
        grx_context_new_full(GRX_FRAME_MODE_RAM_1BPP, slot->bitmap.pitch * 8,
            slot->bitmap.rows, slot->bitmap.buffer, &ctx);
        grx_bit_blt_1bpp(x + slot->bitmap_left - x_offset,
            y - slot->bitmap_top + y_offset, &ctx, 0, 0, slot->bitmap.width,
            slot->bitmap.rows, options->fg_color, options->bg_color);
        x += slot->advance.x >> 6;
        y += slot->advance.y >> 6;
    }
}
