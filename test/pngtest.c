/*
 * pngtest.c ---- test the ctx2png routines
 *
 * Copyright (c) 2001 Mariano Alvarez Fernandez
 * [e-mail: malfer@teleline.es]
 *
 * This is a test/demo file of the GRX graphics library.
 * You can use GRX test/demo files as you want.
 *
 * The GRX graphics library is free software; you can redistribute it
 * and/or modify it under some conditions; see the "copying.grx" file
 * for details.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <stdio.h>
#include <stdlib.h>

#include <grx-3.0.h>

#include "loop.h"

static GrxTextOptions *text_opt;

void imagen(char *nf)
{
    GrxContext *grc;
    int wide, high;
    char s[81];
    int w, h;

    grx_query_png_file(nf, &w, &h);
    sprintf(s, "%s %dx%d", nf, w, h);
    wide = (w > 300) ? 300 : w;
    high = (h > 400) ? 400 : h;
    grx_clear_screen(grx_color_get(0, 0, 200));

    grx_draw_box(10, 40, 10 + wide + 1, 40 + high + 1, GRX_COLOR_WHITE);
    grc = grx_context_new_subcontext(11, 41, 11 + wide - 1, 41 + high - 1, NULL, NULL);
    grx_context_load_from_png(grc, nf, 0, NULL);
    grx_context_unref(grc);

    grx_draw_box(320, 40, 320 + wide + 1, 40 + high + 1, GRX_COLOR_WHITE);
    grc =
        grx_context_new_subcontext(321, 41, 321 + wide - 1, 41 + high - 1, NULL, NULL);
    grx_context_load_from_png(grc, nf, 1, NULL);
    grx_context_unref(grc);

    grx_draw_text(s, 10, 10, text_opt);
    grx_draw_text("Press any key to continue", 10, 50 + high, text_opt);
    run_main_loop_until_key_press();
}

void nopngsupport(void)
{
    GrxTextOptions *text_opt2;
    char *s[6] = { "Warning!",
        "You need libpng (http://www.libpng.org/pub/png/libpng.html)",
        "and enable png support in the GRX lib (edit makedefs.grx)", "to run this demo",
        " ", "Press any key to continue..." };
    int i;

    grx_clear_screen(grx_color_get(0, 0, 100));
    text_opt2 =
        grx_text_options_new(grx_text_options_get_font(text_opt), GRX_COLOR_WHITE);
    for (i = 0; i < 6; i++)
        grx_draw_text(s[i], 90, 160 + i * 18, text_opt2);
    run_main_loop_until_key_press();
    grx_text_options_unref(text_opt2);
}

int main()
{
    GrxContext *grc;
    GError *error = NULL;

    {
        GrxFont *font;

        font = grx_font_load(NULL, -1, &error);
        if (!font) {
            g_error("%s", error->message);
        }
        text_opt = grx_text_options_new_full(font, GRX_COLOR_BLACK, GRX_COLOR_WHITE,
            GRX_TEXT_HALIGN_LEFT, GRX_TEXT_VALIGN_TOP);
        grx_font_unref(font);
    }

    if (!grx_set_mode(
            GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_BPP, &error, 640, 480, 24)) {
        g_error("%s", error->message);
    }

    imagen("pngcompo.png");
    imagen("pngowl.png");
    imagen("pngred.png");

    grx_clear_screen(grx_color_get(0, 100, 0));
    grc =
        grx_context_new_subcontext(191, 121, 191 + 256 - 1, 121 + 240 - 1, NULL, NULL);
    grx_context_load_from_png(grc, "pngred.png", 1, NULL);
    grx_context_unref(grc);
    grc = grx_context_new_subcontext(181, 241, 181 + 289 - 1, 241 + 80 - 1, NULL, NULL);
    grx_context_load_from_png(grc, "pngcompo.png", 1, NULL);
    grx_context_unref(grc);

    grx_draw_text("Press any key to save screen", 10, 10, text_opt);
    run_main_loop_until_key_press();
    grx_context_save_to_png(NULL, "output.png", NULL);

    grx_clear_screen(GRX_COLOR_BLACK);
    grx_draw_text("Press any key to reload screen", 10, 10, text_opt);
    run_main_loop_until_key_press();
    grx_context_load_from_png(NULL, "output.png", 0, NULL);

    grx_draw_text("Press any key to end          ", 10, 10, text_opt);
    run_main_loop_until_key_press();
    grx_set_mode(GRX_GRAPHICS_MODE_TEXT_DEFAULT, NULL);

    grx_text_options_unref(text_opt);

    return 0;
}
