/*
 * jpg2ctx.c ---- loads a context from a JPEG file
 *
 * Copyright (C) 2001 Mariano Alvarez Fernandez
 * [e-mail: malfer@teleline.es]
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
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <jpeglib.h>
#include "grx-3.0.h"

/*
** grx_is_jpeg_supported - Returns true
*/

int grx_is_jpeg_supported( void )
{
  return 1;
}

static int readjpeg( FILE *f, GrxContext *grc, int scale );
static int queryjpeg( FILE *f, int *w, int *h );

/*
** grx_context_load_from_jpeg - Load a context from a JPEG file
**
** If context dimensions are lesser than jpeg dimensions,
** the routine loads as much as it can
**
** If color mode is not in RGB mode, the routine allocates as
** much colors as it can
**
** Arguments:
**   grc:      Context to be loaded (NULL -> use current context)
**   jpegfn:   Name of jpeg file
**   scale:    scale the image to 1/scale, actually libjpeg support
**             1, 2, 4 and 8 noly
**
** Returns  0 on success
**         -1 on error
*/

int grx_context_load_from_jpeg( GrxContext *grc, char *jpegfn, int scale )
{
  GrxContext grcaux;
  FILE *f;
  int r;
  
  f = fopen( jpegfn,"rb" );
  if( f == NULL ) return -1;

  grx_context_save( &grcaux );
  if( grc != NULL ) grx_context_set_current( grc );
  r = readjpeg( f,grc,scale );
  grx_context_set_current( &grcaux );

  fclose( f );

  return r;
}

/*
** grx_check_jpeg_file - Query width and height data from a JPEG file
**
** Arguments:
**   jpegfn:  Name of jpeg file
**   width:   return pnm width
**   height:  return pnm height
**
** Returns  0 on success
**         -1 on error
*/

int grx_check_jpeg_file( char *jpegfn, int *width, int *height )
{
  FILE *f;
  int r;
  
  f = fopen( jpegfn,"rb" );
  if( f == NULL ) return -1;

  r = queryjpeg( f,width,height );

  fclose( f );

  return r;
}

/**/

struct my_error_mgr{
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
  };

typedef struct my_error_mgr *my_error_ptr;

/**/

METHODDEF(void) my_error_exit( j_common_ptr cinfo )
{
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /*(*cinfo->err_>output_message)( cinfo );*/

  longjmp( myerr->setjmp_buffer,1 );
}

/**/

static int readjpeg( FILE *f, GrxContext *grc, int scale )
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  JSAMPARRAY buffer;
  int row_stride;
  int maxwidth, maxheight;
  static GrxColor *pColors = NULL;
  unsigned char *pix_ptr;
  int x, y, r, g, b;

  cinfo.err = jpeg_std_error( &jerr.pub );
  jerr.pub.error_exit = my_error_exit;
  if( setjmp( jerr.setjmp_buffer ) ) {
    if( pColors) free( pColors );
    jpeg_destroy_decompress( &cinfo );
    return -1;
  }

  jpeg_create_decompress( &cinfo );
  jpeg_stdio_src( &cinfo,f );
  jpeg_read_header( &cinfo,TRUE );

  cinfo.scale_denom = scale;
  
  jpeg_start_decompress( &cinfo );

  row_stride = cinfo.output_width * cinfo.output_components;

  buffer = (*cinfo.mem->alloc_sarray)
           ( (j_common_ptr)&cinfo,JPOOL_IMAGE,row_stride,1 );

  maxwidth = (cinfo.output_width > grx_get_size_x()) ?
             grx_get_size_x() : cinfo.output_width;
  maxheight = (cinfo.output_height > grx_get_size_y()) ?
             grx_get_size_y() : cinfo.output_height;
  pColors = malloc( maxwidth * sizeof(GrxColor) );
  if( pColors == NULL ) longjmp( jerr.setjmp_buffer,1 );

  for( y=0; y<maxheight; y++ ){
    jpeg_read_scanlines( &cinfo,buffer,1 );
    pix_ptr = buffer[0];
    if( cinfo.output_components == 1 ){
      for( x=0; x<maxwidth; x++ ){
        r = *pix_ptr++;
        pColors[x] = grx_color_info_alloc_color( r,r,r );
        }
      }
    else{
      for( x=0; x<maxwidth; x++ ){
        r = *pix_ptr++;
        g = *pix_ptr++;
        b = *pix_ptr++;
        pColors[x] = grx_color_info_alloc_color( r,g,b );
        }
      }
    grx_put_scanline( 0,maxwidth-1,y,pColors,GRX_COLOR_MODE_WRITE );
    }

  jpeg_finish_decompress( &cinfo );
  jpeg_destroy_decompress( &cinfo );
  
  return 0;
}

/**/

static int queryjpeg( FILE *f, int *w, int *h )
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;

  cinfo.err = jpeg_std_error( &jerr.pub );
  jerr.pub.error_exit = my_error_exit;
  if( setjmp( jerr.setjmp_buffer ) ) {
    jpeg_destroy_decompress( &cinfo );
    return -1;
  }

  jpeg_create_decompress( &cinfo );
  jpeg_stdio_src( &cinfo,f );
  jpeg_read_header( &cinfo,TRUE );

  *w = cinfo.image_width;
  *h = cinfo.image_height;

  jpeg_destroy_decompress( &cinfo );
  
  return 0;
}
