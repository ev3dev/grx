/**
 ** LNXKEYS.C ---- DOS (TCC/BCC/DJGPP: "conio.h") style keyboard utilities
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 **/

#include "libgrx.h"
#include "input.h"

#include "grxkeys.h"

int kbhit(void)
{
        return(_GrCheckKeyboardHit());
}

int getch(void)
{
        return(_GrReadCharFromKeyboard());
}

int getkey(void)
{
        return(_GrReadCharFromKeyboard());
}

int getkbstat(void)
{
        return(0);
}


/*
** new functions to replace the old style
**   kbhit / getch / getkey / getxkey / getkbstat
** keyboard interface
*/
int GrKeyPressed(void) {
  return(_GrCheckKeyboardHit());
}

GrKeyType GrKeyRead(void) {
  return (GrKeyType)_GrReadCharFromKeyboard();
}

int GrKeyStat(void) {
  return(0);
}
