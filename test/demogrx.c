/**
 ** demogrx.c ---- GRX Test programs launcher
 **
 ** Copyright (C) 2000,2001 Mariano Alvarez Fernandez
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
 **
 **/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grx20.h"
#include "grxkeys.h"
#include "gfaz.h"
#include "drawing.h"

#define WIDTH 640
#define HEIGHT 480
#define BPP 16

#define NDEMOS 24

#define ID_ARCTEST   1
#define ID_BLITTEST  2
#define ID_CIRCTEST  3
#define ID_CLIPTEST  4
#define ID_COLOROPS  5
#define ID_CURSTEST  6
#define ID_FONTTEST  7
#define ID_IMGTEST   8
#define ID_KEYS      9
#define ID_LIFE     10
#define ID_LINETEST 11
#define ID_MOUSETST 12
#define ID_PCIRCTST 13
#define ID_PNMTEST  14
#define ID_POLYTEST 15
#define ID_RGBTEST  16
#define ID_SCROLTST 17
#define ID_SBCTEST  18
#define ID_SPEEDTST 19
#define ID_TEXTPATT 20
#define ID_WINCLIP  21
#define ID_WINTEST  22
#define ID_MODETEST 50
#define ID_EXIT     99

typedef struct{
  int cid;
  char *prog;
  char *text;
  } ProgTable;

static ProgTable ptable[NDEMOS] = {
  { ID_ARCTEST,"arctest","arctest.c -> test arc outline and filled arc drawing" },
  { ID_BLITTEST,"blittest","blittest.c -> test various bitblt-s" },
  { ID_CIRCTEST,"circtest","circtest.c -> test circle and ellipse rendering" },
  { ID_CLIPTEST,"cliptest","cliptest.c -> test clipping" },
  { ID_COLOROPS,"colorops","colorops.c -> test WRITE, XOR, OR, and AND draw modes" },
  { ID_CURSTEST,"curstest","curstest.c -> test cursors" },
  { ID_FONTTEST,"fonttest","fonttest.c -> test text drawing" },
  { ID_IMGTEST,"imgtest","imgtest.c -> test image functions mapping" },
  { ID_KEYS,"keys","keys.c -> test keyboard input" },
  { ID_LIFE,"life","life.c -> Conway's life program" },
  { ID_LINETEST,"linetest","linetest.c -> test wide and patterned lines" },
  { ID_MOUSETST,"mousetst","mousetst.c -> test mouse cursor and mouse/keyboard input" },
  { ID_PCIRCTST,"pcirctst","pcirctst.c -> test custom circle and ellipse rendering" },
  { ID_PNMTEST,"pnmtest","pnmtext.c -> text context to pnm routines" },
  { ID_POLYTEST,"polytest","polytest.c -> test polygon rendering" },
  { ID_RGBTEST,"rgbtest","rgbtest.c -> show 256 color RGB palette" },
  { ID_SBCTEST,"sbctest","sbctest.c -> test subcontext operations" },
  { ID_SCROLTST,"scroltst","scroltst.c -> test virtual screen set/scroll" },
  { ID_SPEEDTST,"speedtst","speedtst.c -> check all available frame drivers speed" },
  { ID_TEXTPATT,"textpatt","textpatt.c -> text patterned text" },
  { ID_WINCLIP,"winclip","winclip.c -> clip a drawing to various windows (contexts)" },
  { ID_WINTEST,"wintest","wintest.c -> test window (context) mapping" },
  { ID_MODETEST,"modetest","modetest.c -> test all available graphics modes" },
  { ID_EXIT,"","Exit GRX test programs launcher" } };

static Button b[NDEMOS] = {
  {  10, 10,100,40,IND_BLUE,IND_YELLOW,"ArcTest",BSTATUS_SELECTED,ID_ARCTEST },
  {  10, 55,100,40,IND_BLUE,IND_YELLOW,"BlitTest",0,ID_BLITTEST },
  {  10,100,100,40,IND_BLUE,IND_YELLOW,"CircTest",0,ID_CIRCTEST },
  {  10,145,100,40,IND_BLUE,IND_YELLOW,"ClipTest",0,ID_CLIPTEST },
  {  10,190,100,40,IND_BLUE,IND_YELLOW,"Colorops",0,ID_COLOROPS },
  {  10,235,100,40,IND_BLUE,IND_YELLOW,"CursTest",0,ID_CURSTEST },
  {  10,280,100,40,IND_BLUE,IND_YELLOW,"FontTest",0,ID_FONTTEST },
  {  10,325,100,40,IND_BLUE,IND_YELLOW,"ImgTest",0,ID_IMGTEST },
  { 115, 10,100,40,IND_BLUE,IND_YELLOW,"Keys",0,ID_KEYS },
  { 115, 55,100,40,IND_BLUE,IND_YELLOW,"Life",0,ID_LIFE },
  { 115,100,100,40,IND_BLUE,IND_YELLOW,"LineTest",0,ID_LINETEST },
  { 115,145,100,40,IND_BLUE,IND_YELLOW,"MouseTst",0,ID_MOUSETST },
  { 115,190,100,40,IND_BLUE,IND_YELLOW,"PcircTst",0,ID_PCIRCTST },
  { 115,235,100,40,IND_BLUE,IND_YELLOW,"PnmTest",0,ID_PNMTEST },
  { 115,280,100,40,IND_BLUE,IND_YELLOW,"PolyTest",0,ID_POLYTEST },
  { 115,325,100,40,IND_BLUE,IND_YELLOW,"RgbTest",0,ID_RGBTEST },
  { 220, 10,100,40,IND_BLUE,IND_YELLOW,"SbcTest",0,ID_SBCTEST },
  { 220, 55,100,40,IND_BLUE,IND_YELLOW,"ScrolTst",0,ID_SCROLTST },
  { 220,100,100,40,IND_BLUE,IND_YELLOW,"SpeedTst",0,ID_SPEEDTST },
  { 220,145,100,40,IND_BLUE,IND_YELLOW,"TextPatt",0,ID_TEXTPATT },
  { 220,190,100,40,IND_BLUE,IND_YELLOW,"WinClip",0,ID_WINCLIP },
  { 220,235,100,40,IND_BLUE,IND_YELLOW,"WinTest",0,ID_WINTEST },
  { 220,280,100,40,IND_GREEN,IND_YELLOW,"ModeTest",0,ID_MODETEST },
  { 220,325,100,40,IND_RED,IND_WHITE,"Exit",0,ID_EXIT } };

static Button_Group bg = { 20,30,b,NDEMOS,0,0 };

static Board brd = { 0,0,640,480,IND_BLACK,IND_CYAN,IND_DARKGRAY,1 };
static Board brdimg = { 384,46,235,157,IND_BLACK,IND_CYAN,IND_DARKGRAY,1 };

static GrFont * grf_std;
static GrFont * grf_big;
GrTextOption grt_centered;
GrTextOption grt_left;

/* Internal routines */

static void ini_objects( void );
static void paint_screen( void );
static void the_title( int x, int y );
static int pev_command( Event *ev );
static int pev_select( Event *ev );
static void paint_foot( char *s );
static void paint_animation( void );
static void disaster( char *s );

/************************************************************************/

#if defined(__WIN32__)
int GRXMain()
#else
int main()
#endif
{
  Event ev;

  gfaz_ini( WIDTH,HEIGHT,BPP );
  ini_objects();
  paint_screen();
  
  while( 1 ){
    event_read( &ev );
    if( ev.type == EV_END ) break;
    if( (ev.type == EV_KEY) && (ev.p1 == GrKey_Escape) ) break;
    if( pev_button_group( &ev,&bg ) ) continue;
    if( pev_command( &ev ) ) continue;
    if( pev_select( &ev ) ) continue;
    if( ev.type == EV_MOUSE ){
      if( ev.p1 == MOUSE_LB_PRESSED )
        paint_foot( "over a button, please" );
      else if( ev.p1 == MOUSE_LB_RELEASED )
        paint_foot( "Hold down left mouse buttom to see a comment" );
      }
    if( ev.type == EV_NULL )
      paint_animation();
    }

  gfaz_fin();
  return 0;
}

/************************************************************************/

static void ini_objects( void )
{
  grf_std = GrLoadFont( "lucb21.fnt" );
  if( grf_std == NULL ){
    grf_std = GrLoadFont( "../fonts/lucb21.fnt" );
    if( grf_std == NULL )
      disaster( "lucb21.fnt not found" );
    }

  grf_big = GrLoadFont( "lucb40b.fnt" );
  if( grf_big == NULL ){
    grf_big = GrLoadFont( "../fonts/lucb40b.fnt" );
    if( grf_big == NULL )
      disaster( "lucb40b.fnt not found" );
    }

  grt_centered.txo_bgcolor.v = GrNOCOLOR;
  grt_centered.txo_direct = GR_TEXT_RIGHT;
  grt_centered.txo_xalign = GR_ALIGN_CENTER;
  grt_centered.txo_yalign = GR_ALIGN_CENTER;
  grt_centered.txo_chrtype = GR_BYTE_TEXT;

  grt_left.txo_bgcolor.v = GrNOCOLOR;
  grt_left.txo_direct = GR_TEXT_RIGHT;
  grt_left.txo_xalign = GR_ALIGN_LEFT;
  grt_left.txo_yalign = GR_ALIGN_CENTER;
  grt_left.txo_chrtype = GR_BYTE_TEXT;
}
    
/************************************************************************/

static void paint_screen( void )
{
  GrContext *grc;
  
  paint_board( &brd );
  paint_button_group( &bg );
  paint_board( &brdimg );
  grc = GrCreateSubContext( brdimg.x+4,brdimg.y+4,
                            brdimg.x+brdimg.wide-5,
                            brdimg.y+brdimg.high-5,
                            NULL,NULL );
  GrLoadContextFromPnm( grc,"pnmtest.ppm" );
  GrDestroyContext( grc );
  the_title( 500,90 );
  drawing( 400,210,200,200,BROWN,DARKGRAY);
  paint_foot( "Hold down left mouse buttom to see a comment" );
}

/************************************************************************/

static void the_title( int x, int y )
{
  char *t1 = "GRX 2.4";
  char *t2 = "test programs launcher";
  char aux[81];

  grt_centered.txo_fgcolor.v = LIGHTGREEN;

  grt_centered.txo_font = grf_big;
  GrDrawString( t1,strlen( t1 ),0+x,0+y,&grt_centered );

  grt_centered.txo_font = grf_std;
  GrDrawString( t2,strlen( t2 ),0+x,40+y,&grt_centered );

  sprintf( aux,"Version:%x System:%d",
           GrGetLibraryVersion(),
           GrGetLibrarySystem() );
  GrDrawString( aux,strlen( aux ),0+x,90+y,&grt_centered );
}

/************************************************************************/

static int pev_command( Event *ev )
{
  int i;
  char nprog[81];
  
  if( ev->type == EV_COMMAND ){
    if( ev->p1 == ID_EXIT ){
      par_event_queue( EV_END,0,0,0 );
      return 1;
      }
    for( i=0; i<NDEMOS; i++ ){
      if( ev->p1 == ptable[i].cid ){
        gfaz_fin();
#if defined(__MSDOS__) || defined(__WIN32__)
        strcpy( nprog,".\\" );
#else
        strcpy( nprog,"./" );
#endif
#if defined(__XWIN__)
        strcat( nprog,"x" );
#endif
        strcat( nprog,ptable[i].prog );
        system( nprog );
        gfaz_ini( WIDTH,HEIGHT,BPP );
        paint_screen();
        return 1;
        }
      }
    }
  return 0;
}
    
/************************************************************************/

static int pev_select( Event *ev )
{
  int i;

  if( ev->type == EV_SELECT ){
    for( i=0; i<NDEMOS; i++ ){
      if( ev->p1 == ptable[i].cid ){
        paint_foot( ptable[i].text );
        return 1;
        }
      }
    }
  return 0;
}
    
/************************************************************************/

static void paint_foot( char *s )
{
  grt_centered.txo_fgcolor.v = LIGHTGREEN;
  grt_centered.txo_font = grf_std;

  GrSetClipBox( 10,440,630,470 );
  GrClearClipBox( CYAN );
  GrDrawString( s,strlen( s ),320,455,&grt_centered );
  GrResetClipBox();
}

/************************************************************************/

static void paint_animation( void )
{
  static char *text =
    "GRX 2.4, the graphics library for DJGPPv2, Linux, X11 and Win32";
  static int pos = 620;
  static int ini = 0;
  static int ltext, wtext;
  static GrContext *grc;
  
  if( !ini ){
    grc = GrCreateContext( 620,30,NULL,NULL );
    if( grc == NULL ) return;
    ltext = strlen( text );
//    wtext = GrStringWidth( text,ltext,&grt_left ); ! doesn't work
    wtext = 620;
    ini = 1;
    }
    
    grt_left.txo_fgcolor.v = CYAN;
    grt_left.txo_font = grf_std;

    GrSetContext( grc );
    GrClearContext( DARKGRAY );
    GrDrawString( text,ltext,pos,15,&grt_left );
    GrSetContext( NULL );
    GrBitBlt( NULL,10,8,grc,0,0,629,29,GrWRITE );

    pos -= 1;
    if( pos <= -wtext ) pos = 620;
}

/************************************************************************/

static void disaster( char *s )
{
  void _GrCloseVideoDriver(void);
  
  char aux[81];
  
  gfaz_fin();
  _GrCloseVideoDriver();
  printf( "DemoGRX: %s\n",s );
  printf( "press Return to continue\n" );
  fgets( aux,80,stdin );
  exit( 1 );
}

  
