/**
 ** w32keys.c ---- DOS (TCC/BCC/DJGPP: "conio.h") style keyboard utilities
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
#include "grxkeys.h"

int kbhit(void)
{
  SEventQueue *pEvQ;
  int ret;
        
  if ( ! _GrIsKbdEnabled () ) {
    return ( _nkeysw32pool > 0 );
  }

  if ( MOUINFO -> msstatus < 2 ) {
    GrMouseInit ();
    GrMouseEventEnable ( 1, 0 );
  }
  EnterCriticalSection ( &csEventQueue );
  pEvQ = pEventQueue;
  ret = FALSE;
  while ( pEvQ -> pNext != NULL ) {
    pEvQ = pEvQ -> pNext;
    if ( ( ( pEvQ -> pEvent -> flags ) & GR_M_KEYPRESS ) == GR_M_KEYPRESS ) {
      ret = TRUE;
      break;
    }
  }
  LeaveCriticalSection ( &csEventQueue );

  return ret;
}

int getkey(void)
{
  GrMouseEvent ev;
  int i, key;
        
  if ( ! _GrIsKbdEnabled () ) {
    while( _nkeysw32pool < 1 )
      _GrUpdateInputs ();
    key = _keysw32pool[0];
    for ( i=0; i<_nkeysw32pool; i++ )
      _keysw32pool[i] = _keysw32pool[i+1];
    _nkeysw32pool--;
    return key;
  }    

  if ( MOUINFO -> msstatus < 2 ) {
    GrMouseInit ();
    GrMouseEventEnable ( 1, 0 ); 
  }
  for ( ; ; ) {
    GrMouseGetEvent ( ( GR_M_EVENT | GR_M_NOPAINT ), &ev );
    if ( ev.flags & GR_M_KEYPRESS ) {
      return ( ev.key );
    }
  }
}

int getch(void)
{
  static int lastkey = ( -1 );
  int key;
        
  if ( lastkey != ( -1 ) ) {
    key = lastkey;
    lastkey = ( -1 );
    return ( key );
  }
  key = getkey ();
  if ( key < 0x100 ) {
    return ( key );
  }
  lastkey = key & 0xff;
        
  return ( 0 );
}

int getkbstat(void)
{
  SEventQueue *pEvQ, *pPrevEvQ;
  int ret;

  EnterCriticalSection ( &csEventQueue );
  pEvQ = pEventQueue;
  ret = 0;
  while ( pEvQ -> pNext != NULL ) {
    pPrevEvQ = pEvQ;
    pEvQ = pEvQ -> pNext;
    if ( ( ( pEvQ -> pEvent -> flags ) & GR_M_KEYPRESS ) == GR_M_KEYPRESS ) {
      ret = pEvQ -> pEvent -> kbstat;
    }
  }
  LeaveCriticalSection ( &csEventQueue );

  return ret;
}

int GrKeyPressed(void)
{
  return kbhit();
}

GrKeyType GrKeyRead(void)
{
  return (GrKeyType)getkey();
}

int GrKeyStat(void)
{
  return getkbstat();
}
