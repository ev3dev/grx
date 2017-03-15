/*
 * fswwtest.c ---- test programmed switching
 *                 between full screen and windowed modes
 *                 for the sdl driver
 *
 * Copyright (C) 2007 Maurice Lombardi
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
#include "grx-3.0.h"
#include "grxkeys.h"
#include "drawing.h"

int main()
{
  GrxFont *font;
  GrxTextOptions *text_opt;
  int x, y, ww, wh, i = 0;
  GrxColor c;
  GrxContext *w1, *w2, *w3, *w4;
  GError *error = NULL;

  font = grx_font_load(NULL, -1, &error);
  if (!font) {
    g_error("%s", error->message);
  }

  text_opt = grx_text_options_new(font,GRX_COLOR_WHITE,GRX_COLOR_BLACK,
                                  GRX_TEXT_HALIGN_LEFT,GRX_TEXT_VALIGN_TOP);

  do {

    if( i ) grx_set_driver("sdl::ww gw 600 gh 600 nc 256", NULL);
    else grx_set_driver("sdl::fs gw 600 gh 600 nc 256", NULL);
    i = ! i;

    if (!grx_set_mode(GRX_GRAPHICS_MODE_GRAPHICS_DEFAULT, &error)) {
      g_error("%s", error->message);
    }

    x = grx_get_width();
    y = grx_get_height();
    ww = (x / 2) - 10;
    wh = (y / 2) - 10;
    w1 = grx_context_new_subcontext(5,5,ww+4,wh+4,NULL,NULL);
    w2 = grx_context_new_subcontext(15+ww,5,ww+ww+14,wh+4,NULL,NULL);
    w3 = grx_context_new_subcontext(5,15+wh,ww+4,wh+wh+14,NULL,NULL);
    w4 = grx_context_new_subcontext(15+ww,15+wh,ww+ww+14,wh+wh+14,NULL,NULL);

    grx_set_current_context(w1);
    c = grx_color_info_alloc_color(200,100,100);
    drawing(0,0,ww,wh,c,GRX_COLOR_BLACK);
    grx_draw_box(0,0,ww-1,wh-1,c);

    grx_set_current_context(w2);
    c = grx_color_info_alloc_color(100,200,200);
    drawing(0,0,ww,wh,c,GRX_COLOR_BLACK);
    grx_draw_box(0,0,ww-1,wh-1,c);

    grx_set_current_context(w3);
    c = grx_color_info_alloc_color(200,200,0);
    drawing(0,0,ww,wh,c,GRX_COLOR_BLACK);
    grx_draw_box(0,0,ww-1,wh-1,c);

    grx_set_current_context(w4);
    c = grx_color_info_alloc_color(0,100,200);
    drawing(0,0,ww,wh,c,GRX_COLOR_BLACK);
    grx_draw_box(0,0,ww-1,wh-1,c);

    grx_set_current_context( NULL );

    grx_draw_text("press any key to toggle full screen / windowed modes, escape to end",
        10,wh,text_opt);

  } while ( GrKeyRead() != GrKey_Escape );

  grx_text_options_unref(text_opt);
  grx_font_unref(font);

  return 0;
}
