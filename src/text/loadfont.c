/*
 * loadfont.c - load a font from a disk file
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

#include <fontconfig/fontconfig.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TYPES_H

#include "text.h"

G_DEFINE_QUARK(ft-error-quark, ft_error)
#define FT_ERROR_QUARK ft_error_quark()

static gboolean freetype_initalized;
static FT_Library library;

FT_Library grx_get_global_freetype_library(GError **err)
{
    if (!freetype_initalized) {
        FT_Error ret = FT_Init_FreeType(&library);
        if (ret) {
            g_set_error_literal(err, FT_ERROR_QUARK, ret, "Error initializing freetype");
            return NULL;
        }
        freetype_initalized = TRUE;
    }
    return library;
}

/**
 * grx_font_load_from_file:
 * @filename: (type filename): the file path
 * @err: Pointer to hold an error
 *
 * Loads a font from a file. Only loads the first font face in the file.
 *
 * Returns: (transfer full) (nullable): the font or %NULL if there was an error
 * such as the font was not found.
 */
GrxFont *grx_font_load_from_file(const gchar *name, GError **err)
{
    FT_Library library;
    GrxFont *font;
    FT_Error ret;

    g_return_val_if_fail(name != NULL, NULL);

    library = grx_get_global_freetype_library(err);
    if (!library) {
        return NULL;
    }

    font = g_malloc0(sizeof(*font));

    ret = FT_New_Face(library, name, 0, &font->face);
    if (ret) {
        g_set_error_literal(err, FT_ERROR_QUARK, ret, "Error getting font face");
        g_free(font);
        return NULL;
    }
    if (font->face->num_fixed_sizes == 0) {
        g_set_error_literal(err, FT_ERROR_QUARK, 0, "Only bitmap fonts are supported");
        g_free(font);
        return NULL;
    }
    // TODO: are there fonts with more than one available_sizes?
    ret = FT_Set_Pixel_Sizes(font->face, font->face->available_sizes[0].x_ppem >> 6,
        font->face->available_sizes[0].y_ppem >> 6);
    if (ret) {
        g_set_error_literal(err, FT_ERROR_QUARK, ret, "Error setting size");
        g_free(font);
        return NULL;
    }

    ret = FT_Select_Charmap(font->face, FT_ENCODING_UNICODE);
    if (ret) {
        g_set_error_literal(err, FT_ERROR_QUARK, ret, "Only unicode fonts are supported");
        g_free(font);
        return NULL;
    }

    g_debug("Loaded: %s", name);
    g_debug("num_faces: %ld", font->face->num_faces);
    g_debug("face_index: %ld", font->face->face_index);
    g_debug("face_flags: 0x%08lx", font->face->face_flags);
    g_debug("style_flags: 0x%08lx", font->face->style_flags);
    g_debug("num_glyphs: %ld", font->face->num_glyphs);
    g_debug("family_name: %s", font->face->family_name);
    g_debug("style_name: %s", font->face->style_name);
    g_debug("num_fixed_sizes: %d", font->face->num_fixed_sizes);
    g_debug("available_sizes:");
    for (int i = 0; i < font->face->num_fixed_sizes; i++) {
        FT_Bitmap_Size *size = &font->face->available_sizes[i];
        g_debug("\t" "height: %d, width: %d, size: %ld-%ld/64, "
                "x_ppem: %ld-%ld/64, y_ppem: %ld-%ld/64",
            size->height, size->width,
            size->size >> 6, size->size & 0x3f,
            size->x_ppem >> 6, size->x_ppem & 0x3f,
            size->y_ppem >> 6, size->y_ppem & 0x3f);
    }
    g_debug("num_charmaps: %d", font->face->num_charmaps);
    g_debug("charmaps:");
    for (int i = 0; i < font->face->num_charmaps; i++) {
        FT_CharMap map = font->face->charmaps[i];
        // FIXME: string gets cut off is map->encoding == FT_ENCODING_NONE
        g_debug("\t" "encoding: %c%c%c%c, platform_id: %d, encoding_id: %d",
            map->encoding >> 24, (map->encoding >> 16) & 0xff,
            (map->encoding >> 8) & 0xff, map->encoding & 0xff,
            map->platform_id, map->encoding_id);
    }
    g_debug("units_per_EM: %d", font->face->units_per_EM);
    g_debug("ascender: %d", font->face->ascender);
    g_debug("descender: %d", font->face->descender);
    g_debug("height: %d", font->face->height);
    g_debug("max_advance_width: %d", font->face->max_advance_width);
    g_debug("max_advance_height: %d", font->face->max_advance_height);
    g_debug("underline_position: %d", font->face->underline_position);
    g_debug("underline_thickness: %d", font->face->underline_thickness);
    g_debug("size->metrics.x_ppem: %d", font->face->size->metrics.x_ppem);
    g_debug("size->metrics.y_ppem: %d", font->face->size->metrics.y_ppem);
    g_debug("size->metrics.x_scale: %ld-%ld/256",
        font->face->size->metrics.x_scale >> 16,
        font->face->size->metrics.x_scale & 0xff);
    g_debug("size->metrics.y_scale: %ld-%ld/256",
        font->face->size->metrics.y_scale >> 16,
        font->face->size->metrics.y_scale & 0xff);
    g_debug("size->metrics.ascender: %ld-%ld/64",
        font->face->size->metrics.ascender >> 6,
        font->face->size->metrics.ascender & 0x3f);
    g_debug("size->metrics.descender: %ld-%ld/64",
        font->face->size->metrics.descender >> 6,
        font->face->size->metrics.descender & 0x3f);
    g_debug("size->metrics.height: %ld-%ld/64",
        font->face->size->metrics.height >> 6,
        font->face->size->metrics.height & 0x3f);
    g_debug("size->metrics.max_advance: %ld-%ld/64",
        font->face->size->metrics.max_advance >> 6,
        font->face->size->metrics.max_advance & 0x3f);

    return grx_font_ref(font);
}

/**
 * grx_font_load_full:
 * @family: (nullable): the font family name or %NULL
 * @style: (nullable): the font style or %NULL
 * @lang: (nullable): the language or %NULL
 * @size: the preferred size or -1
 * @dpi: the preferred dpi or -1
 * @err: pointer to hold an error
 *
 * Loads the font that best matches the parameters.
 *
 * Uses fontconfig for font matching.
 *
 * Returns: (transfer full) (nullable): the font or %NULL if there was an error
 */
GrxFont *grx_font_load_full(const gchar *family, gint size, const gchar *style,
                            const gchar *lang, gint dpi, GError **err)
{
    FcPattern *pattern, *match;
    FcResult result;
    GrxFont *font;
    FcChar8 *file;

    if (!FcInit()) {
        g_set_error_literal(err, FT_ERROR_QUARK, 0, "Failed to init fontconfig");
        return NULL;
    }

    pattern = FcPatternCreate();
    FcPatternAddBool(pattern, "scalable", FcFalse);
    if (family) {
        FcPatternAddString(pattern, "family", (FcChar8 *)family);
    }
    if (size >= 0) {
        FcPatternAddInteger(pattern, "size", size);
    }
    if (style) {
        FcPatternAddString(pattern, "style", (FcChar8 *)style);
    }
    if (lang) {
        FcPatternAddString(pattern, "lang", (FcChar8 *)lang);
    }
    if (dpi >= 0) {
        FcPatternAddInteger(pattern, "dpi", dpi);
    }
    g_debug("searching for pattern: %s", FcNameUnparse(pattern));

    FcConfigSubstitute(NULL, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);
    match = FcFontMatch(NULL, pattern, &result);

    if (!match) {
        // TODO: use result for better error
        g_set_error_literal(err, FT_ERROR_QUARK, 0, "Failed to find match");
        FcPatternDestroy(pattern);
        return NULL;
    }
    g_debug("found pattern: %s", FcNameUnparse(pattern));

    if (FcPatternGetString(match, FC_FILE, 0, &file) != FcResultMatch) {
        g_set_error_literal(err, FT_ERROR_QUARK, 0, "Failed to get file name");
        FcPatternDestroy(match);
        FcPatternDestroy(pattern);
        return NULL;
    }

    font = grx_font_load_from_file((gchar *)file, err);

    FcPatternDestroy(match);
    FcPatternDestroy(pattern);

    return font;
}

/**
 * grx_font_load:
 * @family: (nullable): the font family name or %NULL
 * @size: the preferred size or -1
 * @err: pointer to hold an error
 *
 * Loads the font that best matches the parameters.
 *
 * Uses fontconfig for font matching.
 *
 * Returns: (transfer full) (nullable): the font or %NULL if there was an error
 */
GrxFont *grx_font_load(const gchar *family, gint size, GError **err)
{
    return grx_font_load_full(family, size, NULL, NULL, -1, err);
}
