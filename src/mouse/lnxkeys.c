/**
 ** LNXKEYS.C ---- DOS (TCC/BCC/DJGPP: "conio.h") style keyboard utilities
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/
 
#include "libgrx.h"
#include "input.h"

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

