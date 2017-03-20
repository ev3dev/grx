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
 */

#include <glib.h>
#include <gio/gio.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <jpeglib.h>

#include <grx/context.h>
#include <grx/draw.h>
#include <grx/error.h>
#include <grx/extents.h>

static gboolean readjpeg(FILE *f, GrxContext *grc, int scale);
static gboolean queryjpeg(FILE *f, int *w, int *h);

/**
 * grx_context_load_from_jpeg:
 * @context: (nullable): Context to be loaded or %NULL to use the global context
 * @filename: (type filename): Name of jpeg file
 * @scale: scale the image to 1/scale, only 1, 2, 4 and 8 are supported
 * @error: pointer to hold an error or %NULL to ignore
 *
 * Load a context from a JPEG file.
 *
 * If context dimensions are less than jpeg dimensions, the function loads as
 * much of the image as it can.
 *
 * If color mode is not in RGB mode, the functions allocates as many colors as
 * it can.
 *
 * Returns: %TRUE on success, otherwise %FALSE
 */
gboolean grx_context_load_from_jpeg(GrxContext *grc, const char *jpegfn, int scale, GError **error)
{
  GrxContext grcaux;
  FILE *f;
  gboolean r;

  f = fopen( jpegfn,"rb" );
  if (f == NULL) {
    g_set_error(error, G_IO_ERROR, g_io_error_from_errno(errno),
      "Failed to open '%s'", jpegfn);
    return FALSE;
  }

  grx_save_current_context( &grcaux );
  if( grc != NULL ) grx_set_current_context( grc );
  r = readjpeg( f,grc,scale );
  grx_set_current_context( &grcaux );

  fclose( f );

  if (!r) {
    g_set_error(error, GRX_ERROR, GRX_ERROR_JPEG_ERROR,
      "Error while reading '%s'", jpegfn);
  }

  return r;
}

/**
 * grx_query_jpeg_file:
 * @filename: (type filename): Name of jpeg file
 * @width: (out): return image width
 * @height: (out): return image height
 *
 * Query width and height data from a JPEG file
 *
 * Returns: %TRUE on success, otherwise %FALSE
 */
gboolean grx_query_jpeg_file(const char *jpegfn, int *width, int *height)
{
  FILE *f;
  int r;
  
  f = fopen( jpegfn,"rb" );
  if( f == NULL ) return FALSE;

  r = queryjpeg( f,width,height );

  fclose( f );

  return r;
}

struct my_error_mgr{
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr *my_error_ptr;

METHODDEF(void) my_error_exit( j_common_ptr cinfo )
{
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /*(*cinfo->err_>output_message)( cinfo );*/

  longjmp( myerr->setjmp_buffer,1 );
}

static gboolean readjpeg(FILE *f, GrxContext *grc, int scale)
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
    return FALSE;
  }

  jpeg_create_decompress( &cinfo );
  jpeg_stdio_src( &cinfo,f );
  jpeg_read_header( &cinfo,TRUE );

  cinfo.scale_denom = scale;
  
  jpeg_start_decompress( &cinfo );

  row_stride = cinfo.output_width * cinfo.output_components;

  buffer = (*cinfo.mem->alloc_sarray)
           ( (j_common_ptr)&cinfo,JPOOL_IMAGE,row_stride,1 );

  maxwidth = (cinfo.output_width > grx_get_width()) ?
             grx_get_width() : cinfo.output_width;
  maxheight = (cinfo.output_height > grx_get_height()) ?
             grx_get_height() : cinfo.output_height;
  pColors = malloc( maxwidth * sizeof(GrxColor) );
  if( pColors == NULL ) longjmp( jerr.setjmp_buffer,1 );

  for( y=0; y<maxheight; y++ ){
    jpeg_read_scanlines( &cinfo,buffer,1 );
    pix_ptr = buffer[0];
    if( cinfo.output_components == 1 ){
      for( x=0; x<maxwidth; x++ ){
        r = *pix_ptr++;
        pColors[x] = grx_color_alloc( r,r,r );
        }
      }
    else{
      for( x=0; x<maxwidth; x++ ){
        r = *pix_ptr++;
        g = *pix_ptr++;
        b = *pix_ptr++;
        pColors[x] = grx_color_alloc( r,g,b );
        }
      }
    grx_put_scanline( 0,maxwidth-1,y,pColors,GRX_COLOR_MODE_WRITE );
    }

  jpeg_finish_decompress( &cinfo );
  jpeg_destroy_decompress( &cinfo );
  
  return TRUE;
}

static gboolean queryjpeg( FILE *f, int *w, int *h )
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;

  cinfo.err = jpeg_std_error( &jerr.pub );
  jerr.pub.error_exit = my_error_exit;
  if( setjmp( jerr.setjmp_buffer ) ) {
    jpeg_destroy_decompress( &cinfo );
    return FALSE;
  }

  jpeg_create_decompress( &cinfo );
  jpeg_stdio_src( &cinfo,f );
  jpeg_read_header( &cinfo,TRUE );

  *w = cinfo.image_width;
  *h = cinfo.image_height;

  jpeg_destroy_decompress( &cinfo );
  
  return TRUE;
}
