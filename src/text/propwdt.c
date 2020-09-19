/*
 * propwdt.c - calculate the width of a string using a proportional font
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

#include <grx/text.h>

#include "libgrx.h"
#include "text.h"

/**
 * grx_font_get_char_width:
 * @font: the font
 * @c: the character
 *
 * Gets the width of a character using the specified font.
 *
 * Returns: the width
 */
gint grx_font_get_char_width(GrxFont *font, gunichar c)
{
    FT_UInt index;
    FT_Error ret;

    g_return_val_if_fail(font != NULL, 0);

    index = FT_Get_Char_Index(font->face, c);
    ret = FT_Load_Glyph(font->face, index, FT_LOAD_DEFAULT);
    if (ret) {
        return 0;
    }

    return font->face->glyph->bitmap.width;
}

/**
 * grx_font_get_char_height:
 * @font: the font
 * @c: the character
 *
 * Gets the height of a character using the specified font.
 *
 * Returns: the height
 */
gint grx_font_get_char_height(GrxFont *font, gunichar c)
{
    FT_UInt index;
    FT_Error ret;

    g_return_val_if_fail(font != NULL, 0);

    index = FT_Get_Char_Index(font->face, c);
    ret = FT_Load_Glyph(font->face, index, FT_LOAD_DEFAULT);
    if (ret) {
        return 0;
    }

    return font->face->glyph->bitmap.rows;
}

/**
 * grx_font_get_text_width:
 * @font: the font
 * @text: (nullable): the text
 *
 * Gets the width of a string using the specified font.
 *
 * The string must contain valid UTF-8. Check using g_utf8_validate() first if
 * @text is from an unknown source.
 *
 * Returns: the width
 */
gint grx_font_get_text_width(GrxFont *font, const gchar *text)
{
    FT_UInt index;
    FT_Error ret;
    gunichar c;
    gint width = 0;

    g_return_val_if_fail(font != NULL, 0);

    if (!text) {
        return 0;
    }

    if (FT_IS_FIXED_WIDTH(font->face)) {
        return strlen(text) * (font->face->size->metrics.max_advance >> 6);
    }

    for (; (c = g_utf8_get_char(text)) != '\0'; text = g_utf8_next_char(text)) {
        index = FT_Get_Char_Index(font->face, c);
        ret = FT_Load_Glyph(font->face, index, FT_LOAD_DEFAULT);
        if (ret) {
            continue;
        }
        width += font->face->glyph->advance.x >> 6;
    }

    return width;
}

/**
 * grx_font_get_text_height:
 * @font: the font
 * @text: (nullable): the text
 *
 * Gets the height of a string using the specified font.
 *
 * The string must contain valid UTF-8. Check using g_utf8_validate() first if
 * @text is from an unknown source.
 *
 * Returns: the height
 */
gint grx_font_get_text_height(GrxFont *font, const gchar *text)
{
    g_return_val_if_fail(font != NULL, 0);

    if (!text) {
        return 0;
    }

    return font->face->size->metrics.height >> 6;

#if 0 // TODO: Handle vertical layout
    FT_UInt index;
    FT_Error ret;
    gunichar c;
    gint height = 0;

    for (; (c = g_utf8_get_char(text)) != '\0'; text = g_utf8_next_char(text)) {
        index = FT_Get_Char_Index(font->face, c);
        ret = FT_Load_Glyph(font->face, index, FT_LOAD_DEFAULT);
        if (ret) {
            continue;
        }
        height += slot->advance.y >> 6;
    }

    return height;
#endif
}
