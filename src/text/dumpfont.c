/*
 * dumpfont.c - Draw all glyphs of a font
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

#include <glib.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <grx/context.h>
#include <grx/draw.h>
#include <grx/frame_mode.h>
#include <grx/text.h>

#include "text.h"

/**
 * grx_font_dump:
 * @font: the font
 * @context: the drawing context
 * @start: the starting glyph index
 * @fg: the forground color
 * @bg: the background color
 *
 * Draws as many glyphs as possible in the clip box of the specified context.
 *
 * If all of the glyphs do no fit, an index will be returned. This index can
 * be passed to @start to draw the next "page" of glyphs.
 *
 * Returns: the index of the first glyph that was not drawn or -1 if all of the
 * glyphs were drawn
 */
gint grx_font_dump(const GrxFont *font, GrxContext *context, gint start, GrxColor fg, GrxColor bg)
{
    FT_Size_Metrics metrics;
    FT_GlyphSlot slot;
    FT_Error ret;
    GrxContext ctx;
    GrxFrameMemory mem;
    gint i, x, y, x0, y0, x1, y1, height, width, ascender;

    g_return_val_if_fail(font != NULL, 0);
    g_return_val_if_fail(context != NULL, 0);

    metrics = font->face->size->metrics;
    ascender = metrics.ascender >> 6;

    grx_context_get_clip_box(context, &x0, &y0, &x1, &y1);
    for (i = start, x = 0, y = 0; i < font->face->num_glyphs; i++) {
        height = metrics.height >> 6;
        width = metrics.max_advance >> 6;
        if (x + width > x1) {
            x = 0;
            y += height;
        }
        if (y + height > y1) {
            // we ran out of room
            return i;
        }
        ret = FT_Load_Glyph(font->face, i, FT_LOAD_DEFAULT);
        if (ret) {
            continue;
        }
        slot = font->face->glyph;
        if (slot->format != FT_GLYPH_FORMAT_BITMAP) {
            continue;
        }
        if (slot->bitmap.pixel_mode != FT_PIXEL_MODE_MONO) {
            continue;
        }
        mem.plane0 = slot->bitmap.buffer;
        grx_context_new_full(GRX_FRAME_MODE_RAM_1BPP,
            slot->bitmap.pitch * 8, slot->bitmap.rows, &mem, &ctx);
        grx_context_bit_blt_1bpp(context, x + slot->bitmap_left,
            y - slot->bitmap_top + ascender, &ctx, 0, 0,
            slot->bitmap.width, slot->bitmap.rows, fg, bg);
        x += slot->advance.x >> 6;
        y += slot->advance.y >> 6;
    }

    return -1;
}
