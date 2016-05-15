/**
 ** sbctest.c ---- test subcontext
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
 **
 **/

#include <string.h>
#include "test.h"

static void drawpf( int border, GrxPattern *pat );
static void drawp( int border, GrxLinePattern *grlp );

TESTFUNC(sbctest)
{
  char bits[] = {0, 76, 50, 0, 0, 76, 60, 0};
  GrxContext *grc, *grsc;
  GrxPattern *pat1, *pat2;
  GrxLineOptions grl;
  GrxLinePattern grlp;
  GrxFont *grf;
  GrxTextOption grt;

  grc = grx_context_create( 300,300,NULL,NULL );
  if( grc == NULL ) return;
  grsc = grx_context_create_subcontext( 10,10,290,290,grc,NULL );
  if( grsc == NULL ) return;
  pat1 = grx_pattern_create_pixmap_from_bits( bits,8,8,grx_color_info_get_white(),grx_color_info_get_black() );
  if( pat1 == NULL ) return;
  pat2 = grx_pattern_create_pixmap_from_bits( bits,8,8,grx_color_info_get_black(),grx_color_info_get_white() );
  if( pat2 == NULL ) return;
  grf = grx_font_load( "lucb40.fnt" );
  if( grf == NULL ){
    grf = grx_font_load( "../fonts/lucb40.fnt" );
    if( grf == NULL ) return;
    }

  grx_draw_box( 19,19,320,320,grx_color_info_get_white() );

  grx_draw_text_xy( 0,0,"White drawing on context       ",grx_color_info_get_white(),grx_color_info_get_black() );
  grx_context_set_current( grc );
  grx_clear_context( grx_color_info_get_black() );
  drawing( 10,10,280,280,grx_color_info_get_white(),GRX_COLOR_NONE );
  grx_context_set_current( NULL );
  grx_bit_blt( NULL,20,20,grc,0,0,299,299,GRX_COLOR_MODE_WRITE );
  GrKeyRead();

  grx_draw_text_xy( 0,0,"Black drawing on subcontext    ",grx_color_info_get_white(),grx_color_info_get_black() );
  grx_context_set_current( grsc );
  drawing( 0,0,280,280,grx_color_info_get_black(),GRX_COLOR_NONE );
  grx_context_set_current( NULL );
  grx_bit_blt( NULL,20,20,grc,0,0,299,299,GRX_COLOR_MODE_WRITE );
  GrKeyRead();

  grx_draw_text_xy( 0,0,"Pattern drawing on context     ",grx_color_info_get_white(),grx_color_info_get_black() );
  grx_context_set_current( grc );
  grx_clear_context( grx_color_info_get_black() );
  drawpf( 10,pat1 );
  grx_context_set_current( NULL );
  grx_bit_blt( NULL,20,20,grc,0,0,299,299,GRX_COLOR_MODE_WRITE );
  GrKeyRead();

  grx_draw_text_xy( 0,0,"Pattern drawing on subcontext  ",grx_color_info_get_white(),grx_color_info_get_black() );
  grx_context_set_current( grsc );
  grx_clear_context( grx_color_info_get_black() );
  drawpf( 0,pat2 );
  grx_context_set_current( NULL );
  grx_bit_blt( NULL,20,20,grc,0,0,299,299,GRX_COLOR_MODE_XOR );
  GrKeyRead();

  grl.color = grx_color_info_get_white();
  grl.width = 3;
  grl.n_dash_patterns = 0;
  grlp.pattern = pat1;
  grlp.options = &grl;

  grx_draw_text_xy( 0,0,"Patterned drawing on context   ",grx_color_info_get_white(),grx_color_info_get_black() );
  grx_context_set_current( grc );
  grx_clear_context( grx_color_info_get_black() );
  grlp.pattern = pat1;
  drawp( 10,&grlp );
  grx_context_set_current( NULL );
  grx_bit_blt( NULL,20,20,grc,0,0,299,299,GRX_COLOR_MODE_WRITE );
  GrKeyRead();

  grx_draw_text_xy( 0,0,"Patterned drawing on subcontext",grx_color_info_get_white(),grx_color_info_get_black() );
  grx_context_set_current( grsc );
  grx_clear_context( grx_color_info_get_black() );
  grlp.pattern = pat2;
  drawp( 0,&grlp );
  grx_context_set_current( NULL );
  grx_bit_blt( NULL,20,20,grc,0,0,299,299,GRX_COLOR_MODE_XOR );
  GrKeyRead();

  grt.txo_fgcolor.v = grx_color_info_get_white();
  grt.txo_bgcolor.v = grx_color_info_get_black() | GRX_COLOR_MODE_OR;
  grt.txo_font = grf;
  grt.txo_direct = GRX_TEXT_DIRECTION_RIGHT;
  grt.txo_xalign = GRX_TEXT_ALIGN_LEFT;
  grt.txo_yalign = GRX_TEXT_VALIGN_MIDDLE;
  grt.txo_chrtype = GRX_CHAR_TYPE_BYTE;

  grx_draw_text_xy( 0,0,"Patterned text on context      ",grx_color_info_get_white(),grx_color_info_get_black() );
  grx_context_set_current( grc );
  grx_clear_context( grx_color_info_get_black() );
  grx_draw_string_with_pattern( "Hello all",9,20,60,&grt,pat1 );
  grx_draw_char_with_pattern( 'G',20,120,&grt,pat1 );
  grx_draw_string_with_pattern_ext( "Hola a todos",12,20,180,&grt,pat1 );
  grx_context_set_current( NULL );
  grx_bit_blt( NULL,20,20,grc,0,0,299,299,GRX_COLOR_MODE_WRITE );
  GrKeyRead();

  grx_draw_text_xy( 0,0,"Patterned text on subcontext   ",grx_color_info_get_white(),grx_color_info_get_black() );
  grx_context_set_current( grsc );
  grx_clear_context( grx_color_info_get_black() );
  grx_draw_string_with_pattern( "Hello all",9,10,50,&grt,pat2 );
  grx_draw_char_with_pattern( 'G',10,110,&grt,pat2 );
  grx_draw_string_with_pattern_ext( "Hola a todos",12,10,170,&grt,pat2 );
  grx_context_set_current( NULL );
  grx_bit_blt( NULL,20,20,grc,0,0,299,299,GRX_COLOR_MODE_XOR );
  GrKeyRead();

  grx_font_unref( grf );
  grx_pattern_free( pat2 );
  grx_pattern_free( pat1 );
  grx_context_free( grsc );
  grx_context_free( grc );
}

/***/

static void drawpf( int border, GrxPattern *pat )
{
  int pt1[4][2] = {{130,200},{140,240},{150,250},{160,180}};
  int pt2[4][2] = {{230,200},{235,240},{246,250},{258,180}};
  int ptaux[4][2];
  int i,j;
  
  grx_draw_filled_box_with_pattern( 0+border,0+border,93+border,93+border,pat );
  grx_draw_filled_circle_with_pattern( 139+border,46+border,45,pat );
  grx_draw_filled_ellipse_with_pattern( 232+border,46+border,45,35,pat );
  grx_draw_filled_circle_arc_with_pattern( 46+border,139+border,45,-300,600,
                            GRX_ARC_STYLE_CLOSE2,pat );
  grx_draw_filled_ellipse_arc_with_pattern( 139+border,139+border,45,35,-700,400,
                             GRX_ARC_STYLE_CLOSE2,pat );
  grx_draw_filled_line_with_pattern( 188+border,139+border,278+border,139+border,pat );
  grx_draw_filled_point_with_pattern( 47+border,228+border,pat );
  grx_draw_filled_point_with_pattern( 47+border,229+border,pat );
  grx_draw_filled_point_with_pattern( 47+border,230+border,pat );
  grx_draw_filled_point_with_pattern( 47+border,231+border,pat );
  grx_draw_filled_point_with_pattern( 47+border,232+border,pat );
  for( i=0; i<4; i++ )
    for( j=0; j<2; j++ )
      ptaux[i][j] = pt1[i][j] + border;
  grx_draw_filled_polygon_with_pattern( 4,ptaux,pat );
  for( i=0; i<4; i++ )
    for( j=0; j<2; j++ )
      ptaux[i][j] = pt2[i][j] + border;
  grx_draw_filled_convex_polygon_with_pattern( 4,ptaux,pat );
}

/***/

static void drawp( int border, GrxLinePattern *grlp )
{
  int pt1[4][2] = {{130,200},{140,240},{150,250},{160,180}};
  int pt2[4][2] = {{230,200},{235,240},{246,250},{258,180}};
  int ptaux[4][2];
  int i,j;
  
  grx_draw_box_with_pattern( 0+border,0+border,93+border,93+border,grlp );
  grx_draw_circle_with_pattern( 139+border,46+border,45,grlp );
  grx_draw_ellipse_with_pattern( 232+border,46+border,45,35,grlp );
  grx_draw_circle_arc_with_pattern( 46+border,139+border,45,-300,600,
                        GRX_ARC_STYLE_CLOSE2,grlp );
  grx_draw_ellipse_arc_with_pattern( 139+border,139+border,45,35,-700,400,
                         GRX_ARC_STYLE_CLOSE2,grlp );
  grx_draw_line_with_pattern( 188+border,139+border,278+border,139+border,grlp );
  for( i=0; i<4; i++ )
    for( j=0; j<2; j++ )
      ptaux[i][j] = pt1[i][j] + border;
  grx_draw_polygon_with_pattern( 4,ptaux,grlp );
  for( i=0; i<4; i++ )
    for( j=0; j<2; j++ )
      ptaux[i][j] = pt2[i][j] + border;
  grx_draw_polyline_with_pattern( 4,ptaux,grlp );
}

