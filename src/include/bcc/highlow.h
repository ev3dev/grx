/**
 ** HIGHLOW.H ---- combining two BYTES into one WORD -- Borland-C++ special
 **
 ** Copyright (c) 1997 Hartmut Schirmer
 **
 **/

/* combine two bytes into one word: optimized x86 version */
#define highlow(hi,lo) ( _AH = (hi), _AL = (lo), _AX )

