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
/*
  SEventQueue *pEvQ, *pPrevEvQ;
  int ret;
*/
  GrMouseEvent ev;
        
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
/*
  EnterCriticalSection ( &csEventQueue );
  pEvQ = pEventQueue;
  ret = FALSE;
  while ( pEvQ -> pNext != NULL ) {
    pPrevEvQ = pEvQ;
    pEvQ = pEvQ -> pNext;
    if ( ( ( pEvQ -> pEvent -> flags ) & GR_M_KEYPRESS ) == GR_M_KEYPRESS ) {
      ret = pEvQ -> pEvent -> key;
      free ( pEvQ -> pEvent );
      pPrevEvQ -> pNext = pEvQ -> pNext;
      free ( pEvQ );
      break;
    }
  }
  LeaveCriticalSection ( &csEventQueue );

  return ret;
*/
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
/*
  GrMouseEvent ev;
        
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
*/
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
  return (GrKeyType)getch();
}

int GrKeyStat(void)
{
  return getkbstat();
}
