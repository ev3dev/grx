/**
 ** pngtest.c ---- test the ctx2png routines
 **
 ** Copyright (c) 2001 Mariano Alvarez Fernandez
 ** [e-mail: malfer@teleline.es]
 **
 ** This is a test/demo file of the GRX graphics library.
 ** You can use GRX test/demo files as you want.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **/

#include <stdio.h>
#include <stdlib.h>
#include "grx-3.0.h"
#include "grxkeys.h"

void imagen( char *nf )
{
  GrxContext *grc;
  int wide, high;
  char s[81];
  int w, h;

  grx_check_png_file( nf,&w,&h );
  sprintf( s,"%s %dx%d",nf,w,h );
  wide = (w > 300) ? 300 : w;
  high = (h > 400) ? 400 : h;
  grx_clear_screen( grx_color_info_alloc_color( 0,0,200 ) );

  grx_draw_box( 10,40,10+wide+1,40+high+1,grx_color_info_get_white() );
  grc = grx_context_create_subcontext( 11,41,11+wide-1,41+high-1,NULL,NULL );
  grx_context_load_from_png( grc,nf,0 );
  grx_context_free( grc );

  grx_draw_box( 320,40,320+wide+1,40+high+1,grx_color_info_get_white() );
  grc = grx_context_create_subcontext( 321,41,321+wide-1,41+high-1,NULL,NULL );
  grx_context_load_from_png( grc,nf,1 );
  grx_context_free( grc );

  grx_draw_text_xy( 10,10,s,grx_color_info_get_black(),grx_color_info_get_white() );
  grx_draw_text_xy( 10,50+high,"Press any key to continue",grx_color_info_get_black(),grx_color_info_get_white() );
  GrKeyRead();
}

void nopngsupport( void )
{
  char *s[6] = {
    "Warning!",
    "You need libpng (http://www.libpng.org/pub/png/libpng.html)",
    "and enable png support in the GRX lib (edit makedefs.grx)",
    "to run this demo",
    " ",
    "Press any key to continue..." };
  int i;

  grx_clear_screen( grx_color_info_alloc_color( 0,0,100 ) );
  for( i=0; i<6; i++ )
    grx_draw_text_xy( 90,160+i*18,s[i],grx_color_info_get_white(),GRX_COLOR_NONE );
  GrKeyRead();
}


int main()
{
  GrxContext *grc;

  grx_set_mode( GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_BPP,640,480,24 );

  if( !grx_is_png_supported() ){
    nopngsupport();
    grx_set_mode(GRX_GRAPHICS_MODE_TEXT_DEFAULT);
    exit( 1 );
    }

  imagen( "pngcompo.png" );
  imagen( "pngowl.png" );
  imagen( "pngred.png" );

  grx_clear_screen( grx_color_info_alloc_color( 0,100,0 ) );
  grc = grx_context_create_subcontext( 191,121,191+256-1,121+240-1,NULL,NULL );
  grx_context_load_from_png( grc,"pngred.png",1 );
  grx_context_free( grc );
  grc = grx_context_create_subcontext( 181,241,181+289-1,241+80-1,NULL,NULL );
  grx_context_load_from_png( grc,"pngcompo.png",1 );
  grx_context_free( grc );

  grx_draw_text_xy( 10,10,"Press any key to save screen",grx_color_info_get_black(),grx_color_info_get_white() );
  GrKeyRead();
  grx_context_save_to_png( NULL,"output.png" );

  grx_clear_screen( grx_color_info_get_black() );
  grx_draw_text_xy( 10,10,"Press any key to reload screen",grx_color_info_get_black(),grx_color_info_get_white() );
  GrKeyRead();
  grx_context_load_from_png( NULL,"output.png",0 );

  grx_draw_text_xy( 10,10,"Press any key to end          ",grx_color_info_get_black(),grx_color_info_get_white() );
  GrKeyRead();
  grx_set_mode(GRX_GRAPHICS_MODE_TEXT_DEFAULT);
  return 0;
}
