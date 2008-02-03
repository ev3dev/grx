/**
 ** gfaz.c ---- Mini GUI for MGRX
 **
 ** Copyright (C) 2000,2001,2005 Mariano Alvarez Fernandez
 ** [e-mail: malfer@telefonica.net]
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

#include <stdlib.h>
#include <string.h>
#include <mgrx.h>
#include <mgrxkeys.h>
#include "gfaz.h"

GrColor *egacolors;

static int mouse_found = 1;
static int mouse_count = 0;

/* Internal routines */

static int coord_into( int x, int y, int xo, int yo, int xl, int yl );

/**************************************************************************/

int gfaz_ini( int width, int height, int bpp )
{
/*  __djgpp_set_ctrl_c( 0 );*/
/*  _go32_want_ctrl_break( 1 );*/
/*  GrSetMode( GR_default_graphics );*/

  GrSetMode( GR_width_height_bpp_graphics,width,height,bpp );

  egacolors = GrAllocEgaColors();

  GrEventInit();
  GrMouseSetInternalCursor( GR_MCUR_TYPE_ARROW,GrWhite(),GrBlack() );
  show_mouse();

  return 0;
}

/**************************************************************************/

int gfaz_fin( void )
{
  hide_mouse();

  GrEventUnInit();
  GrSetMode( GR_default_text );

  return 0;
}

/**************************************************************************/

void par_event_queue( int type, long p1, long p2, long p3 )
{
  GrEvent ev;

  ev.type = type;
  ev.p1 = p1;
  ev.p2 = p2;
  ev.p3 = p3;
  GrEventEnqueue( &ev );
}

/**************************************************************************/

void show_mouse( void )
{
  if( (mouse_count == 0) && mouse_found )
    GrMouseDisplayCursor();

  mouse_count++;
}

/************************************************************************/

void hide_mouse( void )
{
  mouse_count--;
  
  if( (mouse_count == 0) && mouse_found )
    GrMouseEraseCursor();
}

/**************************************************************************/

void dboton( int x, int y, int an, int al,
             GrColor c, GrColor ct, char * s, int t )

//   x, y posicin de la esquina izquierda
//   an, al ancho y alto
//   c, ct color del fondo y del texto
//   t, tipo bit 0 0=normal, 1=apretado
//           bit 1 0=no selec, 1=seleccionado

{
  int pol[7][2], prof, pulsd, selec, despl;
  GrTextOption grt;
  GrLineOption glo;
  int mouseblock;

  prof = (t & 0x1) ? 2 : 4;
  pulsd = (t & 0x1) ? 1 : 0;
  selec = (t & 0x2) ? 1 : 0;
  despl = (t & 0x1) ? 1 : 0;

  mouseblock = GrMouseBlock( NULL,x,y,x+an-1,y+al-1 );
  GrBox( x,y,x+an-1,y+al-1,BLACK );
  x = x + 1; y = y + 1;
  an = an - 2; al = al - 2;

  pol[0][0] = x;                 pol[0][1] = y;
  pol[1][0] = x + an - 1;        pol[1][1] = y;
  pol[2][0] = x + an - 2 - prof; pol[2][1] = y + 1 + prof;
  pol[3][0] = x + 1 + prof;      pol[3][1] = y + 1 + prof;
  pol[4][0] = x + 1 + prof;      pol[4][1] = y + al - 2 - prof;
  pol[5][0] = x;                 pol[5][1] = y + al - 1;
  pol[6][0] = pol[0][0];         pol[6][1] = pol[0][1];
  GrFilledPolygon( 7,pol,pulsd ? DARKGRAY : LIGHTGRAY );
  GrPolygon( 7,pol,BLACK );
  GrLine( pol[0][0],pol[0][1],pol[3][0],pol[3][1],BLACK );
  pol[0][0] = x + an - 1;        pol[0][1] = y + al - 1;
  pol[3][0] = x + an - 2 - prof; pol[3][1] = y + al - 2 - prof;
  pol[6][0] = pol[0][0];         pol[6][1] = pol[0][1];
  GrFilledPolygon( 7,pol,pulsd ? LIGHTGRAY : DARKGRAY );
  GrPolygon( 7,pol,BLACK );
  GrLine( pol[0][0],pol[0][1],pol[3][0],pol[3][1],BLACK );
  GrFilledBox( x+2+prof,y+2+prof,x+an-3-prof,y+al-3-prof,c );

  grt.txo_font = &GrFont_PC8x14;
  grt.txo_fgcolor.v = ct;
  grt.txo_bgcolor.v = GrNOCOLOR;
  grt.txo_direct = GR_TEXT_RIGHT;
  grt.txo_xalign = GR_ALIGN_CENTER;
  grt.txo_yalign = GR_ALIGN_CENTER;
  grt.txo_chrtype = GR_BYTE_TEXT;
  if( despl )
    GrDrawString( s,strlen( s ),x+an/2+1,y+al/2+1,&grt );
  else
    GrDrawString( s,strlen( s ),x+an/2,y+al/2,&grt );

  if( selec ){
    glo.lno_color = ct;
    glo.lno_width = 1;
    glo.lno_pattlen = 2;
    glo.lno_dashpat = (unsigned char *)"\2\1";
    GrCustomBox( x+8,y+al/2-6,x+an-8,y+al/2+5,&glo );
    }

  GrMouseUnBlock( mouseblock );
}

/**************************************************************************/

void paint_button( int x, int y, Button *b )
{
  dboton( x+b->x,y+b->y,b->wide,b->high,
          egacolors[b->tbcolor],egacolors[b->tfcolor],
          b->text,b->status );
}

/**************************************************************************/

void paint_button_group( Button_Group *bg )
{
  int i;

  for( i=0; i<bg->nb; i++ )
    paint_button( bg->x,bg->y,&(bg->b[i]) );
}

/**************************************************************************/

int pev_button_group( GrEvent *ev, Button_Group *bg )
{
  int i;

  if( ev->type == GREV_MOUSE ){
    if( ev->p1 == GRMOUSE_LB_PRESSED ){
      for( i=0; i<bg->nb; i++ )
        if( coord_into( ev->p2,ev->p3,
                        bg->x+bg->b[i].x,bg->y+bg->b[i].y,
                        bg->b[i].wide,bg->b[i].high ) ){
          bg->b[bg->pb].status &= ~BSTATUS_SELECTED;
          paint_button( bg->x,bg->y,&(bg->b[bg->pb]) );
          bg->b[i].status |= BSTATUS_PRESSED | BSTATUS_SELECTED;
          paint_button( bg->x,bg->y,&(bg->b[i]) );
          bg->pb = i;
          bg->abp = 1;
          par_event_queue( GREV_SELECT,bg->b[i].bid,0,0 );
          return 1;
          }
      }
    if( ev->p1 == GRMOUSE_LB_RELEASED ){
      if( bg->abp ){
        i = bg->pb;
        bg->b[i].status &= ~BSTATUS_PRESSED;
        paint_button( bg->x,bg->y,&(bg->b[i]) );
        bg->abp = 0;
        if( coord_into( ev->p2,ev->p3,
                        bg->x+bg->b[i].x,bg->y+bg->b[i].y,
                        bg->b[i].wide,bg->b[i].high ) ){
          par_event_queue( GREV_COMMAND,bg->b[i].bid,0,0 );
          }
        return 1;
        }
      }
    }
  else if( ev->type == GREV_KEY ){
    if( ev->p1 == GrKey_Right ||
        ev->p1 == GrKey_Down ||
        ev->p1 == GrKey_Tab ){
      if( bg->pb < bg->nb-1 ){
        bg->b[bg->pb].status &= ~BSTATUS_SELECTED;
        paint_button( bg->x,bg->y,&(bg->b[bg->pb]) );
        bg->pb++;
        bg->b[bg->pb].status |= BSTATUS_SELECTED;
        paint_button( bg->x,bg->y,&(bg->b[bg->pb]) );
        par_event_queue( GREV_SELECT,bg->b[bg->pb].bid,0,0 );
        }
      return 1;
      }
    else if( ev->p1 == GrKey_Left ||
             ev->p1 == GrKey_Up ||
             ev->p1 == GrKey_BackTab ){
      if( bg->pb > 0 ){
        bg->b[bg->pb].status &= ~BSTATUS_SELECTED;
        paint_button( bg->x,bg->y,&(bg->b[bg->pb]) );
        bg->pb--;
        bg->b[bg->pb].status |= BSTATUS_SELECTED;
        paint_button( bg->x,bg->y,&(bg->b[bg->pb]) );
        par_event_queue( GREV_SELECT,bg->b[bg->pb].bid,0,0 );
        }
      return 1;
      }
    else if( ev->p1 == GrKey_Return ){
      par_event_queue( GREV_COMMAND,bg->b[bg->pb].bid,0,0 );
      return 1;
      }
    }

  return 0;
}

/**************************************************************************/

static int coord_into( int x, int y, int xo, int yo, int xl, int yl )
{
  if( x < xo ) return 0;
  if( x >= xo+xl ) return 0;
  if( y < yo ) return 0;
  if( y >= yo+yl ) return 0;
  return 1;
}

/**************************************************************************/

void paint_board( Board *b )
{
  int x1, y1, x2, y2;

  x1 = b->x;
  y1 = b->y;
  x2 = b->x + b->wide - 1;
  y2 = b->y + b->high - 1;
  
  GrBox( x1,y1,x2,y2,egacolors[b->lcolor] );
  GrBox( x1+1,y1+1,x2-1,y2-1,egacolors[b->bcolor] );
  GrBox( x1+2,y1+2,x2-2,y2-2,egacolors[b->bcolor] );
  GrBox( x1+3,y1+3,x2-3,y2-3,egacolors[b->lcolor] );
  GrFilledBox( x1+4,y1+4,x2-4,y2-4,egacolors[b->color] );
}

