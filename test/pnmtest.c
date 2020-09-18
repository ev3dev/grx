/*
 * pnmtest.c ---- test the ctx2pnm routines
 *
 * Copyright (c) 2000 Mariano Alvarez Fernandez
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

#include "grx-3.0.h"

#include "loop.h"

#define FIMAGEPPM "pnmtest.ppm"
#define FIMAGEPBM "pnmtest.pbm"

#define FIMAGEPGM "prueba.pgm"
#define FIMAGEPBM2 "prueba.pbm"
#define FSCREEN "output.ppm"

int main(void)
{
  GrxTextOptions *text_opt;
  GrxContext *grc;
  GrxPnmFormat format;
  int wide, high, maxval;
  char s[81];
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

  if (!grx_set_mode(GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_COLOR,&error,640,480,32768)) {
    g_error("%s", error->message);
  }
  grx_query_pnm_file( FIMAGEPPM, &format, &wide, &high, &maxval );
  sprintf( s,"%s %d x %d pixels",FIMAGEPPM,wide,high );
  grx_draw_text(s, 10, 20, text_opt);
  grx_draw_box( 10,40,10+wide+1,40+high+1,GRX_COLOR_WHITE );
  grc = grx_context_new_subcontext( 11,41,11+wide-1,41+high-1,NULL,NULL );
  grx_context_load_from_pnm( grc,FIMAGEPPM,NULL );
  grx_context_save_to_pgm( grc,FIMAGEPGM,"TestPnm",NULL );
  grx_context_unref( grc );
  grx_draw_text("Press any key to continue", 10, 50+high, text_opt);
  run_main_loop_until_key_press();

  grx_clear_screen( GRX_COLOR_BLACK );
  grx_query_pnm_file( FIMAGEPGM, &format, &wide, &high, &maxval );
  sprintf( s,"%s %d x %d pixels",FIMAGEPGM,wide,high );
  grx_draw_text(s, 10, 20, text_opt);
  grx_draw_box( 10,40,10+wide+1,40+high+1,GRX_COLOR_WHITE );
  grc = grx_context_new_subcontext( 11,41,11+wide-1,41+high-1,NULL,NULL );
  grx_context_load_from_pnm( grc,FIMAGEPGM,NULL );
  grx_context_unref( grc );
  grx_draw_text("Press any key to continue", 10, 50+high, text_opt);
  run_main_loop_until_key_press();

  grx_clear_screen( GRX_COLOR_BLACK );
  grx_query_pnm_file( FIMAGEPBM, &format, &wide, &high, &maxval );
  sprintf( s,"%s %d x %d pixels",FIMAGEPBM,wide,high );
  grx_draw_text(s, 10, 20, text_opt);
  grx_draw_box( 10,40,10+wide+1,40+high+1,GRX_COLOR_WHITE );
  grc = grx_context_new_subcontext( 11,41,11+wide-1,41+high-1,NULL,NULL );
  grx_context_load_from_pnm( grc,FIMAGEPBM,NULL );
  grx_context_save_to_pbm( grc,FIMAGEPBM2,"TestPnm",NULL );
  grx_context_unref( grc );
  grx_draw_text("Press any key to continue", 10, 50+high, text_opt);
  run_main_loop_until_key_press();

  grx_clear_screen( GRX_COLOR_BLACK );
  grx_query_pnm_file( FIMAGEPPM, &format, &wide, &high, &maxval );
  grx_draw_box( 10,40,10+wide+1,40+high+1,GRX_COLOR_WHITE );
  grc = grx_context_new_subcontext( 11,41,11+wide-1,41+high-1,NULL,NULL );
  grx_context_load_from_pnm( grc,FIMAGEPPM,NULL );
  grx_context_unref( grc );
  grx_query_pnm_file( FIMAGEPGM, &format, &wide, &high, &maxval );
  grx_draw_box( 110,140,110+wide+1,140+high+1,GRX_COLOR_WHITE );
  grc = grx_context_new_subcontext( 111,141,111+wide-1,141+high-1,NULL,NULL );
  grx_context_load_from_pnm( grc,FIMAGEPGM,NULL );
  grx_context_unref( grc );
  grx_query_pnm_file( FIMAGEPBM, &format, &wide, &high, &maxval );
  grx_draw_box( 210,240,210+wide+1,240+high+1,GRX_COLOR_WHITE );
  grc = grx_context_new_subcontext( 211,241,211+wide-1,241+high-1,NULL,NULL );
  grx_context_load_from_pnm( grc,FIMAGEPBM2,NULL );
  grx_context_unref( grc );
  grx_draw_text("Press any key to save screen", 10, 20, text_opt);
  run_main_loop_until_key_press();

  grx_context_save_to_ppm( NULL,FSCREEN,"TestPnm",NULL );
  grx_clear_screen( GRX_COLOR_WHITE );
  grx_draw_text("Press any key to reload screen", 10, 20, text_opt);
  run_main_loop_until_key_press();

  grx_context_load_from_pnm( NULL,FSCREEN,NULL );
  grx_draw_text("Press any key to end        ", 10, 20, text_opt);
  run_main_loop_until_key_press();

  grx_set_mode(GRX_GRAPHICS_MODE_TEXT_DEFAULT, NULL);

  grx_text_options_unref(text_opt);

  return 0;
}
