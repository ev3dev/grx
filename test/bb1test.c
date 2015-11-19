/**
 ** bb1test.c ---- test the GrBitBlt1bpp routine
 **
 ** Copyright (c) 2001 Josu Onandia
 ** [e-mail: jonandia@fagorautomation.es].
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
#include "grx-3.0.h"
#include "grxkeys.h"

int main(void)
{
  GrxContext *pContext;
  int sizex = 40;
  int sizey = 40;
  int x = 0;
  int y = 40;
  GrxColor fcolor, bcolor;
  GrKeyType k;

  grx_set_mode( GRX_GRAPHICS_MODE_GRAPHICS_DEFAULT );
  /* Create a 1bpp bitmap */
  pContext = GrCreateFrameContext(GRX_FRAME_MODE_RAM_1BPP, sizex, sizey, NULL, NULL);
  /* draw something (black and white) into the bitmap */
  GrSetContext(pContext);
  GrClearContext( GrBlack() );
  GrLine(0, 0, sizex-1, sizey-1, GrWhite());
  GrLine(0, sizey-1, sizex-1, 0, GrWhite());

  GrSetContext(NULL);
  fcolor = GrAllocColor( 255,0,0 );
  bcolor = GrAllocColor( 0,0,255 );
  GrTextXY(0,0,"Type u d l r U D L R to move, 1 2 to change color, q to quit",
           GrWhite(),GrNOCOLOR);
  GrSetClipBox(0, 40, grx_get_screen_x(), grx_get_screen_y());

  /* Put the bitmap into the screen */
  GrBitBlt1bpp(NULL,x,y,pContext,0,0,sizex-1,sizey-1,fcolor,bcolor);

  while( 1 ){
    k = GrKeyRead();
    if( k == 'q' ) break;
    switch( k ) {
      case 'u': y--; break;
      case 'd': y++; break;
      case 'l': x--; break;
      case 'r': x++; break;
      case 'U': y -= 10; break;
      case 'D': y += 10; break;
      case 'L': x -= 10; break;
      case 'R': x += 10; break;
      case '1': fcolor = GrAllocColor( 255,0,0 );
                bcolor = GrAllocColor( 0,0,255 );
                break;
      case '2': fcolor = GrAllocColor( 0,255,255 );
                bcolor = GrAllocColor( 255,255,0 );
                break;
      default:  continue;
      }
    if(x < -40) x = -40;
    if(x > grx_get_screen_x()) x = grx_get_screen_x();
    if(y < 0) y = 0;
    if(y > grx_get_screen_y()) y = grx_get_screen_y();
    GrBitBlt1bpp(NULL,x,y,pContext,0,0,sizex-1,sizey-1,fcolor,bcolor);
    }

  /* Destroy */
  GrDestroyContext(pContext);

  grx_set_mode(GRX_GRAPHICS_MODE_TEXT_DEFAULT);
  return 0;
}
