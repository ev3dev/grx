/**
 ** MEMMODE.H ---- determine how to access video and system RAM
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/
#ifndef  __MEMMODE_H_INCLUDED__
#define  __MEMMODE_H_INCLUDED__

#ifdef __GNUC__
#include "gcc/memmode.h"
#elif defined(__TURBOC__)
#include "bcc/memmode.h"
#elif defined(__WATCOMC__)
#include "watcom/memmode.h"
#endif

#ifndef  LINP_PTR
#define  LINP_PTR(p)                     (p)
#endif
#ifndef  LINP_SEL
#define  LINP_SEL(p)                     0
#endif

#ifndef MK_FP
#define  MK_FP(s,o)      (void *)(                  \
        ((GR_int32u)(GR_int16u)(s) << 4) +          \
        (GR_int16u)(o)                              \
)
#endif

#endif   /* whole file */

