/*
 * textpatt.c
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
#include <string.h>
#include "grx-3.0.h"
#include "grxkeys.h"

#define FONT "../fonts/tms38b.fnt"

int main(void)
{
  char bits[] = {0, 76, 50, 0, 0, 76, 60, 0};
  GrxPattern *p1, *p2;
  GrxFont *font;
  GrxTextOptions opt;
  GError *error = NULL;

  if (!grx_set_mode(GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_COLOR, &error, 320, 200, (GrxColor)256)) {
    g_error("%s", error->message);
  }
  p1 = grx_pattern_new_pixmap_from_bits(bits, 8, 8, 11,  3);
  p2 = grx_pattern_new_pixmap_from_bits(bits, 8, 8,  3, 11);
  font = grx_font_load(FONT);
  if (font && p1 && p2) {
    memset(&opt, 0, sizeof(GrxTextOptions));
    opt.txo_font   = font;
    opt.txo_xalign = 0;
    opt.txo_yalign = 0;
    opt.txo_direct = GRX_TEXT_DIRECTION_RIGHT;
    opt.txo_fgcolor.v = GRX_COLOR_NONE;
    opt.txo_bgcolor.v = GRX_COLOR_NONE;
    grx_draw_filled_box_with_pattern(0, 0, grx_get_max_x(), grx_get_max_y(), p1);
    GrKeyRead();
    grx_draw_string_with_pattern(" Hello world !", 15, 40, 10, &opt, p1);
    grx_draw_string_with_pattern(" Hello world !", 15, 44, 50, &opt, p2);
    grx_draw_string_with_pattern_ext(" Hello world !!", 16, 40, 100, &opt, p1);
    grx_draw_string_with_pattern_ext(" Hello world !!", 16, 44, 140, &opt, p2);
    GrKeyRead();
    opt.txo_bgcolor.v = GRX_COLOR_BLACK;
    grx_draw_string_with_pattern(" Hello world !", 15, 40, 10, &opt, p1);
    grx_draw_string_with_pattern(" Hello world !", 15, 44, 50, &opt, p2);
    grx_draw_string_with_pattern_ext(" Hello world !!", 16, 40, 100, &opt, p1);
    grx_draw_string_with_pattern_ext(" Hello world !!", 16, 44, 140, &opt, p2);
    GrKeyRead();
  }
  if (p1)   grx_pattern_free(p1);
  if (p2)   grx_pattern_free(p2);
  if (font) grx_font_unref(font);
  grx_set_mode(GRX_GRAPHICS_MODE_TEXT_DEFAULT, NULL);
  if (!p1) fprintf(stderr, "Couldn't create first pattern\n");
  if (!p2) fprintf(stderr, "Couldn't create second pattern\n");
  if (!font) fprintf(stderr, "Couldn't load font %s\n", FONT);

  return 0;
}
