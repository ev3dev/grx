/**
 ** ctx2pnm.c ---- loads/saves a context from/in a PNM file
 **
 ** Copyright (C) 2000 Mariano Alvarez Fernandez
 ** [e-mail: malfer@teleline.es]
 **
 ** This file is part of the GRX graphics library.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** Contributions by Josu Onandia (jonandia@fagorautomation.es) 10/03/2001
 **   _GrLoadContextFromPpm optimized (applied to Pbm and Pgm too)
 **
 **/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grx20.h"

/*
** GrSaveContextToPbm - Dump a context in a PBM file (bitmap)
**
** This routine works both in RGB and palette modes
** If the pixel color isn't Black it asumes White
**
** Arguments:
**   ctx:   Context to be saved (NULL -> use current context)
**   pbmfn: Name of pbm file
**   docn:  string saved like a comment in the pbm file (can be NULL)
**
** Returns  0 on success
**         -1 on error
*/

int GrSaveContextToPbm( GrContext *grc, char *pbmfn, char *docn )
{
  FILE *f;
  GrContext grcaux;
  char cab[81];
  int currentbyte = 0, currentbit = 7;
  int x, y;

  if( (f = fopen( pbmfn,"wb" )) == NULL ) return -1;
  
  GrSaveContext( &grcaux );
  GrSetContext( grc );
  sprintf( cab,"P4\n#" );
  fwrite( cab,1,strlen( cab ),f );
  if( docn != NULL ) fwrite( docn,1,strlen( docn ), f );
  sprintf( cab,"\n%d %d\n",GrSizeX(),GrSizeY() );
  fwrite( cab,1,strlen( cab ),f );
  for( y=0; y<GrSizeY(); y++ ){
    for( x=0; x<GrSizeX(); x++ ){
      if( GrPixel( x,y ) == GrBlack() )
        currentbyte |= 1 << currentbit;
      currentbit--;
      if( currentbit < 0 ){
        fwrite( &currentbyte,1,1,f );
        currentbyte = 0;
        currentbit = 7;
        }
      }
    if( currentbit < 7 ){
      fwrite( &currentbyte,1,1,f );
      currentbyte = 0;
      currentbit = 7;
      }
    }
  GrSetContext( &grcaux );
  fclose( f );

  return 0;
}

/*
** GrSaveContextToPgm - Dump a context in a PGM file (gray scale)
**
** This routine works both in RGB and palette modes
** The colors are quantized to gray scale using .299r + .587g + .114b
**
** Arguments:
**   ctx:   Context to be saved (NULL -> use current context)
**   pgmfn: Name of pgm file
**   docn:  string saved like a comment in the pgm file (can be NULL)
**
** Returns  0 on success
**         -1 on error
*/

int GrSaveContextToPgm( GrContext *grc, char *pgmfn, char *docn )
{
  FILE *f;
  GrContext grcaux;
  char cab[81];
  unsigned char grey;
  int rgb[3];
  int x, y;

  if( (f = fopen( pgmfn,"wb" )) == NULL ) return -1;
  
  GrSaveContext( &grcaux );
  GrSetContext( grc );
  sprintf( cab,"P5\n#" );
  fwrite( cab,1,strlen( cab ),f );
  if( docn != NULL ) fwrite( docn,1,strlen( docn ), f );
  sprintf( cab,"\n%d %d\n255\n",GrSizeX(),GrSizeY() );
  fwrite( cab,1,strlen( cab ),f );
  for( y=0; y<GrSizeY(); y++ )
    for( x=0; x<GrSizeX(); x++ ){
      GrQueryColor( GrPixel( x,y ),&rgb[0],&rgb[1],&rgb[2] );
      grey = (0.229 * rgb[0]) + (0.587 * rgb[1]) + (0.114 * rgb[2]);
      fwrite( &grey,1,1,f );
      }
  GrSetContext( &grcaux );
  fclose( f );

  return 0;
}

/*
** GrSaveContextToPpm - Dump a context in a PPM file (real color)
**
** This routine works both in RGB and palette modes
**
** Arguments:
**   ctx:   Context to be saved (NULL -> use current context)
**   ppmfn: Name of ppm file
**   docn:  string saved like a comment in the ppm file (can be NULL)
**
** Returns  0 on success
**         -1 on error
*/

int GrSaveContextToPpm( GrContext *grc, char *ppmfn, char *docn )
{
  FILE *f;
  GrContext grcaux;
  char cab[81];
  unsigned char brgb[3];
  int x, y, r, g, b;

  if( (f = fopen( ppmfn,"wb" )) == NULL ) return -1;
  
  GrSaveContext( &grcaux );
  GrSetContext( grc );
  sprintf( cab,"P6\n#" );
  fwrite( cab,1,strlen( cab ),f );
  if( docn != NULL ) fwrite( docn,1,strlen( docn ), f );
  sprintf( cab,"\n%d %d\n255\n",GrSizeX(),GrSizeY() );
  fwrite( cab,1,strlen( cab ),f );
  for( y=0; y<GrSizeY(); y++ )
    for( x=0; x<GrSizeX(); x++ ){
      GrQueryColor( GrPixel( x,y ),&r,&g,&b );
      brgb[0] = r;
      brgb[1] = g;
      brgb[2] = b;
      fwrite( brgb,1,3,f );
      }
  GrSetContext( &grcaux );
  fclose( f );

  return 0;
}

/**/

static int skipspaces( FILE *f )
{
  int c;
  
  while( 1 ){
    if( (c = fgetc( f )) == EOF ) return -1;
    if( c == '#' ) // it's a comment
      while( 1 ){
        if( (c = fgetc( f )) == EOF ) return -1;
        if( c == '\n' ) break;
        }
    if( c != ' ' && c != '\t' && c != '\n' && c != '\r' ){
      ungetc( c,f );
      return 0;
      }
    }
}

/**/

static int readnumber( FILE *f )
{
  char buf[81];
  int count;
  
  count = 0;
  while( 1 ){
    if( (buf[count] = fgetc( f )) == EOF ) return -1;
    if( buf[count] == ' ' || buf[count] == '\t' || buf[count] == '\n' ){
      ungetc( buf[count],f );
      break;
      }
    if( count > 80 ) break;
    count++;
    }
  buf[count] = '\0';
  return atoi( buf );
}

/**/

static int loaddata( FILE *f, int *width, int *height, int *maxval )
{
  unsigned char buf[10];
  int r;

  if( fread( buf,1,2,f ) != 2 ) return -1;
  if( buf[0] != 'P' ) return -1;
  r = buf[1] - '0';
  if( (r < PLAINPBMFORMAT) || (r > PPMFORMAT) ) return -1;
  if( skipspaces( f ) != 0 ) return -1;
  if( (*width = readnumber( f )) < 0 ) return -1;
  if( skipspaces( f ) != 0 ) return -1;
  if( (*height = readnumber( f )) < 0 ) return -1;
  if( (r == PLAINPBMFORMAT) || (r == PBMFORMAT) )
    *maxval = 1;
  else{
    if( skipspaces( f ) != 0 ) return -1;
    if( (*maxval = readnumber( f )) < 0 ) return -1;
    }
  fgetc( f );  // skip \n
  return r;
}

/**/

static int _GrLoadContextFromPbm( FILE *f, int width, int height )
{
  int x, y;
  int maxwidth, maxheight;
  GrColor color;
  int currentbyte, isbyteread = 0;
  int currentbit = 0;
  GrColor *pColors=NULL;
  int res = 0;

  maxwidth = (width > GrSizeX()) ? GrSizeX() : width;
  maxheight = (height > GrSizeY()) ? GrSizeY() : height;

  pColors = malloc( maxwidth * sizeof(GrColor) );
  if(pColors == NULL) { res = -1; goto salida; }

  for( y=0; y<maxheight; y++ ){
    for( x=0; x<width; x++ ){
      if( !isbyteread ){
        if( fread( &currentbyte,1,1,f ) != 1 ) { res = -1; goto salida; }
        isbyteread = 1;
        currentbit = 7;
        }
      if( x < maxwidth ){
        color = currentbyte & (1 << currentbit) ? GrBlack() : GrWhite();
        pColors[x] = color;
        }
      currentbit--;
      if( currentbit < 0 ) isbyteread = 0;
      }
    GrPutScanline( 0,maxwidth-1,y,pColors,GrWRITE );
    isbyteread = 0;
    }

salida:
  if( pColors != NULL ) free( pColors );
  return res;
}

/**/

static int _GrLoadContextFromPgm( FILE *f, int width, int height, int maxval )
{
  int x, y;
  int needcoloradjust = 0;
  int maxwidth, maxheight;
  double coloradjust = 255.0;
  GrColor *pColors=NULL;
  unsigned char *pData=NULL, *pCursor;
  int res = 0;

  maxwidth = (width > GrSizeX()) ? GrSizeX() : width;
  maxheight = (height > GrSizeY()) ? GrSizeY() : height;

  if( maxval < 255 ){
    needcoloradjust = 1;
    coloradjust = 255.0 / maxval;
    }

  pData = NULL;
  pColors = malloc( maxwidth * sizeof(GrColor) );
  if(pColors == NULL) { res = -1; goto salida; }
  pData = malloc( width * sizeof(char) );
  if(pData == NULL) { res = -1; goto salida; }

  for( y=0; y<maxheight; y++ ){
    if( fread( pData,1,width,f ) != width ) { res = -1; goto salida; }
    pCursor = pData;
    for( x=0; x<maxwidth; x++ ){
      if( needcoloradjust )
        *pCursor *= coloradjust;
      pColors[x] = GrAllocColor( *pCursor,*pCursor,*pCursor );
      pCursor += 1;
      }
    GrPutScanline( 0,maxwidth-1,y,pColors,GrWRITE );
    }

salida:
  if( pColors != NULL ) free( pColors );
  if( pData != NULL ) free( pData );
  return res;
}

/**/

static int _GrLoadContextFromPpm( FILE *f, int width, int height, int maxval )
{
  int x, y;
  int needcoloradjust = 0;
  int maxwidth, maxheight;
  double coloradjust = 255.0;
  GrColor *pColors=NULL;
  unsigned char *pRGB=NULL, *pCursor;
  int res = 0;

  maxwidth = (width > GrSizeX()) ? GrSizeX() : width;
  maxheight = (height > GrSizeY()) ? GrSizeY() : height;

  if( maxval < 255 ){
    needcoloradjust = 1;
    coloradjust = 255.0 / maxval;
    }

  pRGB = NULL;
  pColors = malloc( maxwidth * sizeof(GrColor) );
  if(pColors == NULL) { res = -1; goto salida; }
  pRGB = malloc( width * 3 * sizeof(char) );
  if(pRGB == NULL) { res = -1; goto salida; }

  for( y=0; y<maxheight; y++ ){
    if( fread( pRGB,3,width,f ) != width ) { res = -1; goto salida; }
    pCursor = pRGB;
    for( x=0; x<maxwidth; x++ ){
      if( needcoloradjust ){
        pCursor[0] *= coloradjust;
        pCursor[1] *= coloradjust;
        pCursor[2] *= coloradjust;
        }
      pColors[x] = GrAllocColor( pCursor[0],pCursor[1],pCursor[2] );
      pCursor += 3;
      }
    GrPutScanline( 0,maxwidth-1,y,pColors,GrWRITE );
    }

salida:
  if( pColors != NULL ) free( pColors );
  if( pRGB != NULL ) free( pRGB );
  return res;
}

/*
** GrLoadContextFromPnm - Load a context from a PNM file
**
** Support only PBM, PGM and PPM binary files with maxval < 256
**
** If context dimensions are lesser than pnm dimensions,
** the routine loads as much as it can
**
** If color mode is not in RGB mode, the routine allocates as
** much colors as it can
**
** Arguments:
**   ctx:   Context to be loaded (NULL -> use current context)
**   pnmfn: Name of pnm file
**
** Returns  0 on success
**         -1 on error
*/

int GrLoadContextFromPnm( GrContext *grc, char *pnmfn )
{
  FILE *f;
  GrContext grcaux;
  int r = -1;
  int format, width, height, maxval;

  if( (f = fopen( pnmfn,"rb" )) == NULL ) return -1;

  GrSaveContext( &grcaux );
  GrSetContext( grc );

  format = loaddata( f,&width,&height,&maxval );
  if( maxval > 255 ) goto ENDFUNCTION;
  if( (format < PBMFORMAT) || (format > PPMFORMAT) ) goto ENDFUNCTION;

  switch( format ){
    case PBMFORMAT: r = _GrLoadContextFromPbm( f,width,height );
                    break;
    case PGMFORMAT: r = _GrLoadContextFromPgm( f,width,height,maxval );
                    break;
    case PPMFORMAT: r = _GrLoadContextFromPpm( f,width,height,maxval );
                    break;
    }

ENDFUNCTION:
  GrSetContext( &grcaux );
  fclose( f );

  return r;
}

/*
** GrQueryPnm - Query format, width and height data from a PNM file
**
** Arguments:
**   pnmfn:   Name of pnm file
**   width:   return pnm width
**   height:  return pnm height
**   maxval:  max color component value
**
** Returns  1 to 6 on success (PNM FORMAT)
**         -1 on error
*/

int GrQueryPnm( char *ppmfn, int *width, int *height, int *maxval )
{
  FILE *f;
  int r;

  if( (f = fopen( ppmfn,"rb" )) == NULL ) return -1;

  r = loaddata( f,width,height,maxval );

  fclose( f );

  return r;
}

