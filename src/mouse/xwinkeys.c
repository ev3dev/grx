/**
 ** XWINKEYS.C ---- DOS (TCC/BCC/DJGPP: "conio.h") style keyboard utilities
 **
 ** Author:        Ulrich Leodolter
 ** E-mail:        ulrich@lab1.psy.univie.ac.at
 ** Date:        Sun Oct  1 08:10:30 1995
 ** RCSId:        $Id: xwinkeys.c 1.1 1995/11/19 16:34:52 ulrich Exp $
 **/
 
#include "libxwin.h"
#include "libgrx.h"
#include "input.h"

int kbhit(void)
{
  if(MOUINFO->msstatus < 2) {
     GrMouseInit();
     GrMouseEventEnable(1,0);
  }
  return(_XGrKeyboardHit());
}

int getkey(void)
{
  GrMouseEvent ev;
  if(MOUINFO->msstatus < 2) {
     GrMouseInit();
     GrMouseEventEnable(1,0); 
  }
  for( ; ; ) {
     GrMouseGetEvent((GR_M_EVENT | GR_M_NOPAINT),&ev);
     if(ev.flags & GR_M_KEYPRESS) {
         return(ev.key);
     }
  }
}

int getch(void)
{
  static int lastkey = (-1);
  int key;
  if(lastkey != (-1)) {
    key = lastkey;
    lastkey = (-1);
    return(key);
  }
  key = getkey();
  if(key < 0x100) {
    return(key);
  }
  lastkey = key & 0xff;
  return(0);
}
  
int getkbstat(void)
{
  if(MOUINFO->msstatus < 2) {
     GrMouseInit();
     GrMouseEventEnable(1,0); 
  }
  return(_XGrKeyboardGetState());
}
