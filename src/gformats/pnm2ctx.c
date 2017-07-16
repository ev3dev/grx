/*
 * pnm2ctx.c ---- loads a context from a PNM file or PNM buffer
 *
 * Copyright (C) 2000,2001 Mariano Alvarez Fernandez
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
 * Contributions by Josu Onandia (jonandia@fagorautomation.es) 10/03/2001
 *   _GrLoadContextFromPpm optimized (applied to Pbm and Pgm too)
 */

#include <glib.h>
#include <gio/gio.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <grx/context.h>
#include <grx/draw.h>
#include <grx/error.h>
#include <grx/extents.h>
#include <grx/gformats.h>
#include <grx/mode.h>

typedef struct{
  int method;  /* 0=file, 1=buffer */
  FILE *file;
  const unsigned char *buffer;
  int bufferpointer;
} inputstruct;

static size_t inputread( void *buffer, size_t size, size_t number,
                         inputstruct *is )
{
  if( is->method == 0 )
    return fread( buffer,size,number,is->file );
  else{
    memcpy( buffer,&(is->buffer[is->bufferpointer]),size*number );
    is->bufferpointer += size * number;
    return number;
    }
}

static int inputgetc( inputstruct *is )
{
  if( is->method == 0 )
    return fgetc( is->file );
  else
    return is->buffer[is->bufferpointer++];
}

static int inputungetc( int c, inputstruct *is )
{
  if( is->method == 0 )
    return ungetc( c,is->file );
  else{
    is->bufferpointer--;
    return c;
    }
}

static gboolean skipspaces( inputstruct *is )
{
  int c;
  
  while( 1 ){
    if( (c = inputgetc( is )) == EOF ) return FALSE;
    if( c == '#' ) // it's a comment
      while( 1 ){
        if( (c = inputgetc( is )) == EOF ) return FALSE;
        if( c == '\n' ) break;
        }
    if( c != ' ' && c != '\t' && c != '\n' && c != '\r' ){
      inputungetc( c,is );
      return TRUE;
      }
    }
}

static int readnumber( inputstruct *is )
{
  char buf[81];
  int count;
  
  count = 0;
  while( 1 ){
    if( (buf[count] = inputgetc( is )) == EOF ) return -1;
    if( buf[count] == ' ' || buf[count] == '\t' || buf[count] == '\n' ){
      inputungetc( buf[count],is );
      break;
      }
    if( count > 80 ) break;
    count++;
    }
  buf[count] = '\0';
  return atoi( buf );
}

static gboolean loaddata(inputstruct *is, GrxPnmFormat *format, int *width, int *height, int *maxval)
{
    unsigned char buf[10];
    GrxPnmFormat r;

    if (inputread(buf,1,2,is) != 2) {
        return FALSE;
    }
    if (buf[0] != 'P') {
        return FALSE;
    }

    r = buf[1] - '0';
    if ((r < GRX_PNM_FORMAT_ASCII_PBM) || (r > GRX_PNM_FORMAT_BINARY_PPM)) {
        return FALSE;
    }
    if (!skipspaces(is)) {
        return FALSE;
    }
    if ((*width = readnumber(is)) < 0) {
        return FALSE;
    }
    if (!skipspaces(is)) {
        return FALSE;
    }
    if ((*height = readnumber(is)) < 0) {
        return FALSE;
    }
    if ((r == GRX_PNM_FORMAT_ASCII_PBM) || (r == GRX_PNM_FORMAT_BINARY_PBM)) {
        *maxval = 1;
    }
    else {
        if (!skipspaces(is)) {
            return FALSE;
        }
        if ((*maxval = readnumber(is)) < 0) {
            return FALSE;
        }
    }

    // skip \n
    inputgetc(is);

    *format = r;

    return TRUE;
}

static gboolean _GrLoadContextFromPbm( inputstruct *is, int width, int height )
{
  int x, y;
  int maxwidth, maxheight;
  GrxColor color;
  int currentbyte, isbyteread = 0;
  int currentbit = 0;
  GrxColor *pColors=NULL;
  gboolean res = TRUE;

  maxwidth = (width > grx_get_width()) ? grx_get_width() : width;
  maxheight = (height > grx_get_height()) ? grx_get_height() : height;

  pColors = malloc( maxwidth * sizeof(GrxColor) );
  if(pColors == NULL) { res = FALSE; goto out; }

  for( y=0; y<maxheight; y++ ){
    for( x=0; x<width; x++ ){
      if( !isbyteread ){
        if( inputread( &currentbyte,1,1,is ) != 1 ) { res = FALSE; goto out; }
        isbyteread = 1;
        currentbit = 7;
        }
      if( x < maxwidth ){
        color = currentbyte & (1 << currentbit) ? GRX_COLOR_BLACK : GRX_COLOR_WHITE;
        pColors[x] = color;
        }
      currentbit--;
      if( currentbit < 0 ) isbyteread = 0;
      }
    grx_put_scanline( 0,maxwidth-1,y,pColors,GRX_COLOR_MODE_WRITE );
    isbyteread = 0;
    }

out:
  if( pColors != NULL ) free( pColors );
  return res;
}

static gboolean _GrLoadContextFromPgm( inputstruct *is, int width,
                                       int height, int maxval )
{
  int x, y;
  int needcoloradjust = 0;
  int maxwidth, maxheight;
  double coloradjust = 255.0;
  GrxColor *pColors=NULL;
  unsigned char *pData=NULL, *pCursor;
  gboolean res = TRUE;

  maxwidth = (width > grx_get_width()) ? grx_get_width() : width;
  maxheight = (height > grx_get_height()) ? grx_get_height() : height;

  if( maxval < 255 ){
    needcoloradjust = 1;
    coloradjust = 255.0 / maxval;
    }

  pData = NULL;
  pColors = malloc( maxwidth * sizeof(GrxColor) );
  if(pColors == NULL) { res = FALSE; goto out; }
  pData = malloc( width * sizeof(char) );
  if(pData == NULL) { res = FALSE; goto out; }

  for( y=0; y<maxheight; y++ ){
    if( inputread( pData,1,width,is ) != width ) { res = FALSE; goto out; }
    pCursor = pData;
    for( x=0; x<maxwidth; x++ ){
      if( needcoloradjust )
        *pCursor *= coloradjust;
      pColors[x] = grx_color_get( *pCursor,*pCursor,*pCursor );
      pCursor += 1;
      }
    grx_put_scanline( 0,maxwidth-1,y,pColors,GRX_COLOR_MODE_WRITE );
    }

out:
  if( pColors != NULL ) free( pColors );
  if( pData != NULL ) free( pData );
  return res;
}

static gboolean _GrLoadContextFromPpm( inputstruct *is, int width,
                                       int height, int maxval )
{
  int x, y;
  int needcoloradjust = 0;
  int maxwidth, maxheight;
  double coloradjust = 255.0;
  GrxColor *pColors=NULL;
  unsigned char *pRGB=NULL, *pCursor;
  gboolean res = TRUE;

  maxwidth = (width > grx_get_width()) ? grx_get_width() : width;
  maxheight = (height > grx_get_height()) ? grx_get_height() : height;

  if( maxval < 255 ){
    needcoloradjust = 1;
    coloradjust = 255.0 / maxval;
    }

  pRGB = NULL;
  pColors = malloc( maxwidth * sizeof(GrxColor) );
  if(pColors == NULL) { res = FALSE; goto out; }
  pRGB = malloc( width * 3 * sizeof(char) );
  if(pRGB == NULL) { res = FALSE; goto out; }

  for( y=0; y<maxheight; y++ ){
    if( inputread( pRGB,3,width,is ) != width ) { res = FALSE; goto out; }
    pCursor = pRGB;
    for( x=0; x<maxwidth; x++ ){
      if( needcoloradjust ){
        pCursor[0] *= coloradjust;
        pCursor[1] *= coloradjust;
        pCursor[2] *= coloradjust;
        }
      pColors[x] = grx_color_get( pCursor[0],pCursor[1],pCursor[2] );
      pCursor += 3;
      }
    grx_put_scanline( 0,maxwidth-1,y,pColors,GRX_COLOR_MODE_WRITE );
    }

out:
  if( pColors != NULL ) free( pColors );
  if( pRGB != NULL ) free( pRGB );
  return res;
}

/**
 * grx_context_load_from_pnm:
 * @context: (nullable): Context to be loaded or %NULL to use the global context
 * @filename: (type filename): Name of pnm file
 * @error: pointer to hold an error or %NULL to ignore
 *
 * Load a context from a PNM file.
 *
 * Support only PBM, PGM and PPM binary files with maxval < 256
 *
 * If context dimensions are less than pnm dimensions,
 * the routine loads as much as it can
 *
 * If color mode is not in RGB mode, the routine allocates as
 * many colors as it can
 *
 * Returns: %TRUE on success, otherwise %FALSE
 */
gboolean grx_context_load_from_pnm(GrxContext *grc, const char *pnmfn, GError **error)
{
  inputstruct is = {0, NULL, NULL, 0};
  GrxContext grcaux;
  gboolean r;
  GrxPnmFormat format;
  int width, height, maxval;

  if ((is.file = fopen( pnmfn,"rb" )) == NULL) {
    g_set_error(error, G_IO_ERROR, g_io_error_from_errno(errno),
      "Failed to open '%s'", pnmfn);
    return FALSE;
  }

  grx_save_current_context( &grcaux );
  if( grc != NULL ) grx_set_current_context( grc );

  r = loaddata(&is, &format, &width, &height, &maxval);
  if (!r) {
    goto out;
  }
  r = FALSE;
  if( maxval > 255 ) {
    goto out;
  }
  if ((format < GRX_PNM_FORMAT_BINARY_PBM) || (format > GRX_PNM_FORMAT_BINARY_PPM)) {
    goto out;
  }

  switch( format ){
    case GRX_PNM_FORMAT_BINARY_PBM: r = _GrLoadContextFromPbm( &is,width,height );
                    break;
    case GRX_PNM_FORMAT_BINARY_PGM: r = _GrLoadContextFromPgm( &is,width,height,maxval );
                    break;
    case GRX_PNM_FORMAT_BINARY_PPM: r = _GrLoadContextFromPpm( &is,width,height,maxval );
                    break;
    default:
      break;
  }

out:
  grx_set_current_context( &grcaux );
  fclose( is.file );

  if (!r) {
    g_set_error(error, GRX_ERROR, GRX_ERROR_PNM_ERROR,
      "Problem with PNM data in '%s'", pnmfn);
  }

  return r;
}

/**
 * grx_query_pnm_file:
 * @filename: (type filename): Name of pnm file
 * @format: (out): the #GrxPnmFormat of the file
 * @width: (out): return pnm width
 * @height: (out): return pnm height
 * @maxval: (out): max color component value
 *
 * Query format, width and height data from a PNM file
 *
 * Returns: %TRUE on success, otherwise %FALSE
 */
gboolean grx_query_pnm_file(const char *pnmfn, GrxPnmFormat *format, int *width, int *height, int *maxval)
{
  inputstruct is = {0, NULL, NULL, 0};
  GrxPnmFormat r;

  if( (is.file = fopen( pnmfn,"rb" )) == NULL ) return FALSE;

  r = loaddata( &is,format,width,height,maxval );

  fclose( is.file );

  return r;
}

/**
 * grx_context_load_from_pnm_data:
 * @context: (nullable): Context to be loaded or %NULL to use the global context
 * @buffer: Buffer that holds data
 *
 * Load a context from a PNM buffer
 *
 * Support only PBM, PGM and PPM binary buffers with maxval < 256
 *
 * If context dimensions are lesser than pnm dimensions,
 * the routine loads as much as it can
 *
 * If color mode is not in RGB mode, the routine allocates as
 * much colors as it can
 *
 * Returns: %TRUE on success, otherwise %FALSE
 */
gboolean grx_context_load_from_pnm_data(GrxContext *grc, const unsigned char *pnmbuf)
{
  inputstruct is = {1, NULL, NULL, 0};
  GrxContext grcaux;
  gboolean r;
  GrxPnmFormat format;
  int width, height, maxval;

  is.buffer = pnmbuf;
  
  grx_save_current_context( &grcaux );
  if( grc != NULL ) grx_set_current_context( grc );

  r = loaddata( &is,&format,&width,&height,&maxval );
  if (!r) {
    goto out;
  }
  r = FALSE;
  if (maxval > 255) {
    goto out;
  }
  if ((format < GRX_PNM_FORMAT_BINARY_PBM) || (format > GRX_PNM_FORMAT_BINARY_PPM)) {
    goto out;
  }

  switch( format ){
    case GRX_PNM_FORMAT_BINARY_PBM: r = _GrLoadContextFromPbm( &is,width,height );
                    break;
    case GRX_PNM_FORMAT_BINARY_PGM: r = _GrLoadContextFromPgm( &is,width,height,maxval );
                    break;
    case GRX_PNM_FORMAT_BINARY_PPM: r = _GrLoadContextFromPpm( &is,width,height,maxval );
                    break;
    default:
      break;
  }

out:
  grx_set_current_context( &grcaux );

  return r;
}

/**
 * grx_query_pnm_data:
 * @data: The image data
 * @format: (out): the #GrxPnmFormat of the file
 * @width: (out): return pnm width
 * @height: (out): return pnm height
 * @maxval: (out): max color component value
 *
 * Query format, width and height data from a PNM buffer
 *
 * Returns: %TRUE on success, otherwise %FALSE
 */
gboolean grx_query_pnm_data(GByteArray *pnmbuf, GrxPnmFormat *format, int *width, int *height, int *maxval)
{
  inputstruct is = {1, NULL, NULL, 0};
  gboolean r;

  is.buffer = pnmbuf->data;

  r = loaddata(&is, format, width, height, maxval);

  return r;
}
