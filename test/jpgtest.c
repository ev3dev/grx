/*
 * jpgtest.c ---- test the ctx2jpeg routines
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
#include "grx-3.0.h"
#include "grxkeys.h"

static GrxTextOptions *text_opt;

void imagen( char *nf, int scale )
{
  GrxContext *grc;
  int wide, high;
  char s[81];
  int w, h;

  grx_check_jpeg_file( nf,&w,&h );
  sprintf( s,"%s %dx%d scale 1/%d",nf,w,h,scale );
  wide = (w/scale > 600) ? 600 : w/scale;
  high = (h/scale > 400) ? 400 : h/scale;
  grx_clear_screen( grx_color_alloc( 0,0,200 ) );

  grx_draw_box( 10,40,10+wide+1,40+high+1,GRX_COLOR_WHITE );
  grc = grx_context_new_subcontext( 11,41,11+wide-1,41+high-1,NULL,NULL );
  grx_context_load_from_jpeg( grc,nf,scale,NULL );
  grx_context_unref( grc );

  grx_draw_text( s,10,10,text_opt );
  grx_draw_text( "Press any key to continue",10,50+high,text_opt );
  GrKeyRead();
}

void nojpegsupport( void )
{
  char *s[6] = {
    "Warning!",
    "You need libjpeg (http://www.ijg.org) and enable",
    "jpeg support in the GRX lib (edit makedefs.grx)",
    "to run this demo",
    " ",
    "Press any key to continue..." };
  int i;

  grx_clear_screen( grx_color_alloc( 0,0,100 ) );
  for( i=0; i<6; i++ )
    grx_draw_text( s[i],90,160+i*18,text_opt );
  GrKeyRead();
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

  if (!grx_set_mode(GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_BPP,&error,640,480,24)) {
    g_error("%s", error->message);
  }

  imagen( "jpeg1.jpg",1 );
  imagen( "jpeg1.jpg",2 );
  imagen( "jpeg1.jpg",4 );
  imagen( "jpeg1.jpg",8 );
  imagen( "jpeg2.jpg",1 );
  imagen( "jpeg2.jpg",2 );
  imagen( "jpeg2.jpg",4 );
  imagen( "jpeg2.jpg",8 );

  grx_clear_screen( grx_color_alloc( 0,100,0 ) );
  grc = grx_context_new_subcontext( 10,40,10+400-1,40+300-1,NULL,NULL );
  grx_context_load_from_jpeg( grc,"jpeg1.jpg",2,NULL );
  grx_context_unref( grc );
  grc = grx_context_new_subcontext( 210,150,210+400-1,150+300-1,NULL,NULL );
  grx_context_load_from_jpeg( grc,"jpeg2.jpg",2,NULL );
  grx_context_unref( grc );

  grx_draw_text( "Press any key to save color and gray screen",10,10,text_opt );
  GrKeyRead();

  grx_context_save_to_jpeg( NULL,"p.jpg",75,NULL );
  grx_context_save_to_jpeg_grayscale( NULL,"pgray.jpg",75,NULL );

  grx_clear_screen( GRX_COLOR_BLACK );
  grx_draw_text( "Press any key to reload color screen       ",10,10,text_opt );
  GrKeyRead();
  grx_context_load_from_jpeg( NULL,"p.jpg",1,NULL );

  grx_draw_text( "Press any key to reload gray screen        ",10,10,text_opt );
  GrKeyRead();
  grx_clear_screen( GRX_COLOR_BLACK );
  grx_context_load_from_jpeg( NULL,"pgray.jpg",1,NULL );

  grx_draw_text( "Press any key to end                       ",10,10,text_opt );
  GrKeyRead();

  grx_text_options_unref(text_opt);

  grx_set_mode(GRX_GRAPHICS_MODE_TEXT_DEFAULT, NULL);

  return 0;
}
