/**
 ** HIGHLOW.H ---- combining two BYTES into one WORD
 **
 ** Copyright (c) 1997 Hartmut Schirmer
 **
 **/

/* combine two bytes into one word: generic version */
#define __highlow__(hi,lo) ((((GR_int16u)(hi))<<8)|((GR_int8u)(lo)))

#ifdef __GNUC__
#include "gcc/highlow.h"
#elif defined(__TURBOC__)
#include "bcc/highlow.h"
#endif

#ifndef highlow
#define highlow(hi,lo) __highlow__((hi),(lo))
#endif

#ifndef highlowP
#define highlowP(p) highlow(*((GR_int8u *)(p)),*(((GR_int8u *)(p))+1))
#endif
