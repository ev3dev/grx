/**
 ** pnmtest.c ---- test the ctx2pnm routines
 **
 ** Copyright (c) 2000 Mariano Alvarez Fernandez
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
 
#include <stdlib.h>
#include <stdio.h>
#include "grx-3.0.h"
#include "grxkeys.h"

#define FIMAGEPPM "pnmtest.ppm"
#define FIMAGEPBM "pnmtest.pbm"

#define FIMAGEPGM "prueba.pgm"
#define FIMAGEPBM2 "prueba.pbm"
#define FSCREEN "output.ppm"

int main(void)
{
  GrxContext *grc;
  int wide, high, maxval;
  char s[81];
  
/* grx_set_mode( GRX_GRAPHICS_MODE_GRAPHICS_DEFAULT ); */
  grx_set_mode( GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_COLOR,640,480,32768 );
  GrQueryPnm( FIMAGEPPM, &wide, &high, &maxval );
  sprintf( s,"%s %d x %d pixels",FIMAGEPPM,wide,high );
  GrTextXY( 10,20,s,grx_color_info_get_black(),grx_color_info_get_white() );
  grx_draw_box( 10,40,10+wide+1,40+high+1,grx_color_info_get_white() );
  grc = grx_context_create_subcontext( 11,41,11+wide-1,41+high-1,NULL,NULL );
  GrLoadContextFromPnm( grc,FIMAGEPPM );
  GrSaveContextToPgm( grc,FIMAGEPGM,"TestPnm" );
  grx_context_free( grc );
  GrTextXY( 10,50+high,"Press any key to continue",grx_color_info_get_black(),grx_color_info_get_white() );
  GrKeyRead();

  grx_clear_screen( grx_color_info_get_black() );
  GrQueryPnm( FIMAGEPGM, &wide, &high, &maxval );
  sprintf( s,"%s %d x %d pixels",FIMAGEPGM,wide,high );
  GrTextXY( 10,20,s,grx_color_info_get_black(),grx_color_info_get_white() );
  grx_draw_box( 10,40,10+wide+1,40+high+1,grx_color_info_get_white() );
  grc = grx_context_create_subcontext( 11,41,11+wide-1,41+high-1,NULL,NULL );
  GrLoadContextFromPnm( grc,FIMAGEPGM );
  grx_context_free( grc );
  GrTextXY( 10,50+high,"Press any key to continue",grx_color_info_get_black(),grx_color_info_get_white() );
  GrKeyRead();

  grx_clear_screen( grx_color_info_get_black() );
  GrQueryPnm( FIMAGEPBM, &wide, &high, &maxval );
  sprintf( s,"%s %d x %d pixels",FIMAGEPBM,wide,high );
  GrTextXY( 10,20,s,grx_color_info_get_black(),grx_color_info_get_white() );
  grx_draw_box( 10,40,10+wide+1,40+high+1,grx_color_info_get_white() );
  grc = grx_context_create_subcontext( 11,41,11+wide-1,41+high-1,NULL,NULL );
  GrLoadContextFromPnm( grc,FIMAGEPBM );
  GrSaveContextToPbm( grc,FIMAGEPBM2,"TestPnm" );
  grx_context_free( grc );
  GrTextXY( 10,50+high,"Press any key to continue",grx_color_info_get_black(),grx_color_info_get_white() );
  GrKeyRead();

  grx_clear_screen( grx_color_info_get_black() );
  GrQueryPnm( FIMAGEPPM, &wide, &high, &maxval );
  grx_draw_box( 10,40,10+wide+1,40+high+1,grx_color_info_get_white() );
  grc = grx_context_create_subcontext( 11,41,11+wide-1,41+high-1,NULL,NULL );
  GrLoadContextFromPnm( grc,FIMAGEPPM );
  grx_context_free( grc );
  GrQueryPnm( FIMAGEPGM, &wide, &high, &maxval );
  grx_draw_box( 110,140,110+wide+1,140+high+1,grx_color_info_get_white() );
  grc = grx_context_create_subcontext( 111,141,111+wide-1,141+high-1,NULL,NULL );
  GrLoadContextFromPnm( grc,FIMAGEPGM );
  grx_context_free( grc );
  GrQueryPnm( FIMAGEPBM, &wide, &high, &maxval );
  grx_draw_box( 210,240,210+wide+1,240+high+1,grx_color_info_get_white() );
  grc = grx_context_create_subcontext( 211,241,211+wide-1,241+high-1,NULL,NULL );
  GrLoadContextFromPnm( grc,FIMAGEPBM2 );
  grx_context_free( grc );
  GrTextXY( 10,20,"Press any key to save screen",grx_color_info_get_black(),grx_color_info_get_white() );
  GrKeyRead();

  GrSaveContextToPpm( NULL,FSCREEN,"TestPnm" );
  grx_clear_screen( grx_color_info_get_white() );
  GrTextXY( 10,20,"Press any key to reload screen",grx_color_info_get_white(),grx_color_info_get_black() );
  GrKeyRead();

  GrLoadContextFromPnm( NULL,FSCREEN );
  GrTextXY( 10,20,"Press any key to end        ",grx_color_info_get_black(),grx_color_info_get_white() );
  GrKeyRead();

  grx_set_mode(GRX_GRAPHICS_MODE_TEXT_DEFAULT);
  return 0;
}
