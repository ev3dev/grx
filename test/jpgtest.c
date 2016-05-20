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
  grx_clear_screen( grx_color_info_alloc_color( 0,0,200 ) );

  grx_draw_box( 10,40,10+wide+1,40+high+1,grx_color_info_get_white() );
  grc = grx_context_create_subcontext( 11,41,11+wide-1,41+high-1,NULL,NULL );
  grx_context_load_from_jpeg( grc,nf,scale );
  grx_context_unref( grc );

  grx_draw_text_xy( 10,10,s,grx_color_info_get_black(),grx_color_info_get_white() );
  grx_draw_text_xy( 10,50+high,"Press any key to continue",grx_color_info_get_black(),grx_color_info_get_white() );
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

  grx_clear_screen( grx_color_info_alloc_color( 0,0,100 ) );
  for( i=0; i<6; i++ )
    grx_draw_text_xy( 90,160+i*18,s[i],grx_color_info_get_white(),GRX_COLOR_NONE );
  GrKeyRead();
}

int main()
{
  GrxContext *grc;

  grx_set_mode( GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_BPP,640,480,24 );

  if( !grx_is_jpeg_supported() ){
    nojpegsupport();
    grx_set_mode(GRX_GRAPHICS_MODE_TEXT_DEFAULT);
    exit( 1 );
    }

  imagen( "jpeg1.jpg",1 );
  imagen( "jpeg1.jpg",2 );
  imagen( "jpeg1.jpg",4 );
  imagen( "jpeg1.jpg",8 );
  imagen( "jpeg2.jpg",1 );
  imagen( "jpeg2.jpg",2 );
  imagen( "jpeg2.jpg",4 );
  imagen( "jpeg2.jpg",8 );

  grx_clear_screen( grx_color_info_alloc_color( 0,100,0 ) );
  grc = grx_context_create_subcontext( 10,40,10+400-1,40+300-1,NULL,NULL );
  grx_context_load_from_jpeg( grc,"jpeg1.jpg",2 );
  grx_context_unref( grc );
  grc = grx_context_create_subcontext( 210,150,210+400-1,150+300-1,NULL,NULL );
  grx_context_load_from_jpeg( grc,"jpeg2.jpg",2 );
  grx_context_unref( grc );

  grx_draw_text_xy( 10,10,"Press any key to save color and gray screen",
    grx_color_info_get_black(),grx_color_info_get_white() );
  GrKeyRead();

  grx_context_save_to_jpeg( NULL,"p.jpg",75 );
  grx_context_save_to_jpeg_grayscale( NULL,"pgray.jpg",75 );

  grx_clear_screen( grx_color_info_get_black() );
  grx_draw_text_xy( 10,10,"Press any key to reload color screen       ",
    grx_color_info_get_black(),grx_color_info_get_white() );
  GrKeyRead();
  grx_context_load_from_jpeg( NULL,"p.jpg",1 );

  grx_draw_text_xy( 10,10,"Press any key to reload gray screen        ",
    grx_color_info_get_black(),grx_color_info_get_white() );
  GrKeyRead();
  grx_clear_screen( grx_color_info_get_black() );
  grx_context_load_from_jpeg( NULL,"pgray.jpg",1 );

  grx_draw_text_xy( 10,10,"Press any key to end                       ",
    grx_color_info_get_black(),grx_color_info_get_white() );
  GrKeyRead();

  grx_set_mode(GRX_GRAPHICS_MODE_TEXT_DEFAULT);
  return 0;
}
