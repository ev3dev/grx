/**
 ** DOSKEYS.C ---- auxiliary DOS keyboard input functions
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "int86.h"
#include "memfill.h"

#define USE_AT_BIOS

#ifdef  USE_AT_BIOS
#define KBD_BIOS_BASE        0x10
#else
#define KBD_BIOS_BASE        0
#endif

#ifdef __TURBOC__

int getkey(void)
{
        Int86Regs r;
        sttzero(&r);
        IREG_AX(r) = (KBD_BIOS_BASE + 0) << 8;
        int16(&r);
        switch(IREG_AL(r)) {
#ifdef USE_AT_BIOS
          case 0xe0:
#endif
          case 0x00:
            return(IREG_AH(r) + 0x100);
          default:
            return(IREG_AL(r));
        }
}

int getxkey(void)
{
        Int86Regs r;
        sttzero(&r);
        IREG_AX(r) = (KBD_BIOS_BASE + 0) << 8;
        int16(&r);
        switch(IREG_AL(r)) {
#ifdef USE_AT_BIOS
          case 0xe0:
            return(IREG_AH(r) + 0x200);
#endif
          case 0x00:
            return(IREG_AH(r) + 0x100);
          default:
            return(IREG_AL(r));
        }
}

#endif

int getkbstat(void)
{
        Int86Regs r;
        sttzero(&r);
        IREG_AX(r) = (KBD_BIOS_BASE + 2) << 8;
        int16(&r);
        return(IREG_AL(r));
}

