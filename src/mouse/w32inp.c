/**
 ** w32inp.c ---- DOS (TCC/BCC/DJGPP: "conio.h") style keyboard utilities
 **
 ** Author:     Gernot Graeff
 ** E-mail:     gernot.graeff@t-online.de
 ** Date:       02-11-99
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
 **/

#include "libwin32.h"
#include "libgrx.h"
#include "input.h"
#include "arith.h"
#include "memcopy.h"

static int  kbd_enabled = TRUE;
static int  kbd_lastmod = 0;
static int  mou_enabled = TRUE;
static int  mou_buttons = 0;
static long evt_lasttime;

int _GrIsKbdEnabled ( void )
{
  return kbd_enabled;
}

static void uninit ( void )
{
  if ( MOUINFO -> msstatus > 1 ) {
    MOUINFO -> msstatus = 1;
  }
}

int GrMouseDetect ( void )
{
  return GetSystemMetrics ( SM_MOUSEPRESENT );
}

void GrMouseInitN (int queue_size )
{
  uninit ();
  queue_size = umax ( 4, umin ( 256, queue_size ) );
  init_queue ( queue_size );
  if ( GrMouseDetect () ) {
    GrMouseSetSpeed ( 1, 1 );
    GrMouseSetAccel ( 100, 1 );
    GrMouseSetLimits ( 0, 0, SCRN -> gc_xmax, SCRN -> gc_ymax );
    GrMouseWarp ( ( SCRN -> gc_xmax >> 1 ), ( SCRN -> gc_ymax >> 1 ) );
    _GrInitMouseCursor ();
    MOUINFO -> msstatus = 2;
    mou_buttons = 0;
  }
  GrMouseEventEnable ( TRUE, TRUE );
  real_time ( evt_lasttime );
  MOUINFO -> uninit = uninit;
}

void GrMouseSetSpeed ( int spmult, int spdiv )
{
  MOUINFO -> spmult = umin ( 16,umax ( 1, spmult ) );
  MOUINFO -> spdiv = umin ( 16, umax ( 1, spdiv ) );
}

void GrMouseSetAccel ( int thresh, int accel )
{
  MOUINFO -> thresh = umin ( 64, umax ( 1, thresh ) );
  MOUINFO -> accel = umin ( 16, umax ( 1, accel ) );
}

void GrMouseSetLimits ( int x1, int y1, int x2, int y2 )
{
  isort ( x1, x2 );
  isort ( y1, y2 );
  MOUINFO -> xmin = imax ( 0, imin ( x1, SCRN -> gc_xmax ) );
  MOUINFO -> ymin = imax ( 0, imin ( y1, SCRN -> gc_ymax ) );
  MOUINFO -> xmax = imax ( 0, imin ( x2, SCRN -> gc_xmax ) );
  MOUINFO -> ymax = imax ( 0, imin ( y2, SCRN -> gc_ymax ) );
}

void GrMouseWarp(int x,int y)
{
  MOUINFO -> xpos = imax ( MOUINFO -> xmin, imin ( MOUINFO -> xmax, x ) );
  MOUINFO -> ypos = imax ( MOUINFO -> ymin, imin ( MOUINFO -> ymax, y ) );
  GrMouseUpdateCursor ();
  SetCursorPos ( MOUINFO -> xpos, MOUINFO -> ypos );
}

void GrMouseEventEnable ( int enable_kb, int enable_ms )
{
  kbd_enabled = enable_kb;
  mou_enabled = enable_ms;
}

void GrMouseGetEventT ( int flags, GrMouseEvent *ev, long tout )
{
  int     msdraw;
        
  if ( MOUINFO -> msstatus  == 0 ) {
    GrMouseInit ();
  }
  msdraw = ! MOUINFO -> displayed && !( flags & GR_M_NOPAINT );
  if ( msdraw ) {
    GrMouseDisplayCursor ();
  }
  if ( tout <= 0L ) {
    tout = 1L;
  }
  for ( ; ; ) {
    _GrUpdateInputs ();
    GrMouseUpdateCursor ();
    while ( MOUINFO -> qlength > 0 ) {
      dequeue_event ( ( *ev ) );
      if ( ev -> flags & flags ) {
        if ( msdraw ) {
          GrMouseEraseCursor ();
        }
        return;
      }
    }
    if ( ( flags & GR_M_POLL ) ||
         ( tout == 0L ) ||
         ( MOUINFO -> moved && ( flags & GR_M_MOTION ) ) ) {
      fill_mouse_ev ( ( *ev ),
                      mou_buttons, mou_buttons,
                      GR_M_LEFT,
                      GR_M_MIDDLE,
                      GR_M_RIGHT,
                      kbd_lastmod );
      if ( ev->flags ) /* something happend */
        real_dtime(ev->dtime,evt_lasttime);
      else
        ev->dtime = -1; /* special time if nothing happend */
      MOUINFO -> moved = FALSE;
      if ( msdraw ) {
        GrMouseEraseCursor ();
      }
      return;
    }
    if(tout > 0L) {
      Sleep( 10 );
      if((tout -= 10) < 0L) tout = 0L;
    }
  }
}

unsigned int _ButtonEvent2GrButton ( GrMouseEvent *event )
{
  unsigned int    uiRet = 0;

  if ( event -> flags & GR_M_LEFT_DOWN ) {
    uiRet |= GR_M_LEFT;
  }
  if ( event -> flags & GR_M_MIDDLE_DOWN ) {
    uiRet |= GR_M_MIDDLE;
  }
  if ( event -> flags & GR_M_RIGHT_DOWN ) {
    uiRet |= GR_M_RIGHT;
  }

  return uiRet;
}

void _GrUpdateInputs ( void )
{
  int count;
  SEventQueue *pEvents, *pNextEvent;

  EnterCriticalSection ( &csEventQueue );
  pEvents = pEventQueue;
  count = 0;
  while ( pEvents -> pNext != NULL ) {
    count++;
    pEvents = pEvents -> pNext;
  }
  if ( count <= 0 ) {
    LeaveCriticalSection ( &csEventQueue );
    return;
  }
  pEvents = pEventQueue;
  while ( --count >= 0 ) {
    GrMouseEvent ev;
    GrMouseEvent *pWinEv;
    int btn;

    pWinEv = pEvents -> pNext -> pEvent;
    switch ( pWinEv -> flags ) {

      case GR_M_MOTION :
        if ( mou_enabled && ( MOUINFO -> msstatus == 2 ) ) {
          MOUINFO -> xpos = pWinEv -> x;
          MOUINFO -> ypos = pWinEv -> y;
          MOUINFO -> moved = TRUE;
        }
        break;

      case GR_M_LEFT_DOWN :
      case GR_M_MIDDLE_DOWN :
      case GR_M_RIGHT_DOWN :
      case GR_M_LEFT_UP :
      case GR_M_MIDDLE_UP :
      case GR_M_RIGHT_UP :
        if ( mou_enabled && ( MOUINFO -> msstatus == 2 ) ) {
          MOUINFO -> xpos = pWinEv -> x;
          MOUINFO -> ypos = pWinEv -> y;
/*        MOUINFO -> moved = TRUE; */
          btn = _ButtonEvent2GrButton ( pWinEv );
/*        if ( btn != mou_buttons ) { */
            fill_mouse_ev ( ev, mou_buttons, btn, GR_M_LEFT,
                            GR_M_MIDDLE, GR_M_RIGHT, kbd_lastmod );
            real_dtime ( ev.dtime, evt_lasttime );
            enqueue_event ( ev );
            MOUINFO -> moved = FALSE;
            mou_buttons = btn;
/*        } */
        }
        break;

      case GR_M_KEYPRESS :
        if ( kbd_enabled ) {
          fill_keybd_ev ( ev, pWinEv -> key, kbd_lastmod );
          real_dtime ( ev.dtime, evt_lasttime );
          enqueue_event ( ev );
          MOUINFO -> moved = FALSE;
        } else if( _nkeysw32pool < _MAXKEYSW32POOL ) {
          _keysw32pool[_nkeysw32pool++] = pWinEv -> key;
        }
        break;

    }
    free ( pEvents -> pNext -> pEvent );
    pNextEvent = pEvents -> pNext -> pNext;
    free ( pEvents -> pNext );
    pEvents -> pNext = pNextEvent;
  }
  LeaveCriticalSection ( &csEventQueue );
}
