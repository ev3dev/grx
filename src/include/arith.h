/*
 * arith.h ---- some common integer arithmetic macros/inline functions
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
 *
 * This file is part of the GRX graphics library.
 *
 * The GRX graphics library is free software; you can redistribute it
 * and/or modify it under some conditions; see the "copying.grx" file
 * for details.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Intel CPU specific support is provided for the Turbo C and GNU C. May
 * work with other compilers and CPU-s, but is not optimized for them.
 */

#ifndef __ARITH_H_INCLUDED__
#define __ARITH_H_INCLUDED__

#ifdef __GNUC__
#include "gcc/arith.h"
#endif

#ifdef _MSC_VER
#define __emit__(x) __asm { __emit (x) }
#endif

/*
 * swap and sort stuff
 */
#define iswap(x, y)              \
    {                            \
        int _swap_tmpval_ = (x); \
        (x) = (y);               \
        (y) = _swap_tmpval_;     \
    }

#define lswap(x, y)               \
    {                             \
        long _swap_tmpval_ = (x); \
        (x) = (y);                \
        (y) = _swap_tmpval_;      \
    }

#define isort(x, y)              \
    {                            \
        if ((int)(x) > (int)(y)) \
            iswap(x, y)          \
    }

#define usort(x, y)                                \
    {                                              \
        if ((unsigned int)(x) > (unsigned int)(y)) \
            iswap(x, y)                            \
    }

#define lsort(x, y)                \
    {                              \
        if ((long)(x) > (long)(y)) \
            lswap(x, y)            \
    }

#define ulsort(x, y)                                 \
    {                                                \
        if ((unsigned long)(x) > (unsigned long)(y)) \
            lswap(x, y)                              \
    }

/*
 * [i|u]scale(X,N,D)
 * scale an integer with long intermediate result but without using long
 * arithmetic all the way
 */
#ifndef iscale
#define iscale(X, N, D) (int)((X) * (N) / (long)(D))
#endif
#ifndef uscale
#define uscale(X, N, D) (unsigned int)((X) * (N) / (unsigned long)(D))
#endif
#ifndef irscale
#define irscale(X, N, D)                                                    \
    ((iscale(((int)(X) << 1), N, D)                                         \
         + (((int)(X) ^ (int)(N) ^ (int)(D)) >> (sizeof(int) * 8 - 1)) + 1) \
        >> 1)
#endif
#ifndef urscale
#define urscale(X, N, D) ((uscale(((int)(X) << 1), N, D) + 1) >> 1)
#endif

/*
 * replicate_<FROM>2<TO>(byte_or_word_value)
 * copy the lower byte(s) of a byte or word into the upper byte(s)
 */
#define INLINE_REPLICATE(V, TFROM, TTO)  \
    (((unsigned TTO)(unsigned TFROM)(V)) \
        | ((unsigned TTO)(unsigned TFROM)(V) << (sizeof(TFROM) * 8)))

#ifndef replicate_b2b
#define replicate_b2b(BYTE) (BYTE)
#endif
#ifndef replicate_b2w
#define replicate_b2w(BYTE) INLINE_REPLICATE(BYTE, GR_int8, GR_int16)
#endif
#ifndef replicate_b2l
#define replicate_b2l(BYTE) replicate_w2l(replicate_b2w(BYTE))
#endif
#ifndef replicate_b2h
#define replicate_b2h(BYTE) replicate_l2h(replicate_w2l(replicate_b2w(BYTE)))
#endif

#ifndef replicate_w2w
#define replicate_w2w(WORD) (WORD)
#endif
#ifndef replicate_w2l
#define replicate_w2l(WORD) INLINE_REPLICATE(WORD, GR_int16, GR_int32)
#endif
#ifndef replicate_w2h
#define replicate_w2h(WORD) replicate_l2h(replicate_w2l(WORD))
#endif

#ifndef replicate_l2l
#define replicate_l2l(LONG) (LONG)
#endif
#ifndef replicate_l2h
#define replicate_l2h(LONG) INLINE_REPLICATE(LONG, GR_int32, GR_int64)
#endif

#ifndef replicate_h2h
#define replicate_h2h(LLONG) (LLONG)
#endif

#endif /* whole file */
