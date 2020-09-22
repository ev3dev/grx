/*
 * mempeek.h ---- (far) memory read/write operations
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
 * Basic memory peek and poke operations in byte, word and long sizes.
 * The poke operations are available in WRITE, XOR, OR and AND versions.
 * Intel CPU specific support is provided for the Turbo C and GNU C
 * compilers. The i386 GCC version supports segment overrides. May
 * work with other compilers and CPU-s, but is not optimized for them.
 */

#ifndef __MEMPEEK_H_INCLUDED__
#define __MEMPEEK_H_INCLUDED__

#ifdef __GNUC__
#include "gcc/mempeek.h"
#endif

#ifndef INLINE_STD_PEEK
#define INLINE_STD_PEEK(P, S, T) (*(unsigned T *)(P))
#endif

#ifndef INLINE_STD_POKE
#ifdef NO_LEFTSIDE_PTR_CAST
#define INLINE_STD_POKE(P, V, OP, I, S, T)          \
    do {                                            \
        register unsigned T *_ISPptr = (void *)(P); \
        *_ISPptr OP(unsigned T)(V);                 \
    } while (0)
#else
#define INLINE_STD_POKE(P, V, OP, I, S, T) (*(unsigned T *)(P)OP(unsigned T)(V))
#endif
#endif

/*
 * peek_<SIZE>... (pointer)
 * poke_<SIZE>... (pointer,value)
 */

/* some processors (eg. DEC alpha) need special handling for word access */
#ifndef MISALIGNED_16bit_OK
#define peek_w(p) \
    (((GR_int16u)(*((GR_int8u *)(p)))) | ((GR_int16u)(*((GR_int8u *)(p) + 1)) << 8))
#define __SPLIT_16_POKE__(P, V, OP) \
    ((*((GR_int8u *)(P))OP((V))), (*((GR_int8u *)(P) + 1) OP((V) >> 8)))
#define poke_w(p, v)     __SPLIT_16_POKE__(p, v, =)
#define poke_w_xor(p, v) __SPLIT_16_POKE__(p, v, ^=)
#define poke_w_or(p, v)  __SPLIT_16_POKE__(p, v, |=)
#define poke_w_and(p, v) __SPLIT_16_POKE__(p, v, &=)
#endif

/* ------------------------------------- near memory (RAM) access */
#ifndef peek_b
#define peek_b(p) INLINE_STD_PEEK(p, OP8b, GR_int8)
#endif
#ifndef peek_w
#define peek_w(p) INLINE_STD_PEEK(p, OP16b, GR_int16)
#endif
#ifndef peek_l
#define peek_l(p) INLINE_STD_PEEK(p, OP32b, GR_int32)
#endif
#if defined(GR_int64) && !defined(peek_h)
#define peek_h(p) INLINE_STD_PEEK(p, OP64b, GR_int64)
#endif

#ifndef poke_b
#define poke_b(p, v) INLINE_STD_POKE(p, v, =, MOV_INS, OP8b, GR_int8)
#endif
#ifndef poke_w
#define poke_w(p, v) INLINE_STD_POKE(p, v, =, MOV_INS, OP16b, GR_int16)
#endif
#ifndef poke_l
#define poke_l(p, v) INLINE_STD_POKE(p, v, =, MOV_INS, OP32b, GR_int32)
#endif
#if defined(GR_int64) && !defined(poke_h)
#define poke_h(p, v) INLINE_STD_POKE(p, v, =, MOV_INS, OP64b, GR_int64)
#endif

#define poke_b_set poke_b
#define poke_w_set poke_w
#define poke_l_set poke_l
#ifdef poke_h
#define poke_h_set poke_h
#endif

#ifndef poke_b_xor
#define poke_b_xor(p, v) INLINE_STD_POKE(p, v, ^=, XOR_INS, OP8b, GR_int8)
#endif
#ifndef poke_w_xor
#define poke_w_xor(p, v) INLINE_STD_POKE(p, v, ^=, XOR_INS, OP16b, GR_int16)
#endif
#ifndef poke_l_xor
#define poke_l_xor(p, v) INLINE_STD_POKE(p, v, ^=, XOR_INS, OP32b, GR_int32)
#endif
#if defined(GR_int64) && !defined(poke_h_xor)
#define poke_h_xor(p, v) INLINE_STD_POKE(p, v, ^=, XOR_INS, OP64b, GR_int64)
#endif

#ifndef poke_b_or
#define poke_b_or(p, v) INLINE_STD_POKE(p, v, |=, OR_INS, OP8b, GR_int8)
#endif
#ifndef poke_w_or
#define poke_w_or(p, v) INLINE_STD_POKE(p, v, |=, OR_INS, OP16b, GR_int16)
#endif
#ifndef poke_l_or
#define poke_l_or(p, v) INLINE_STD_POKE(p, v, |=, OR_INS, OP32b, GR_int32)
#endif
#if defined(GR_int64) && !defined(poke_h_or)
#define poke_h_or(p, v) INLINE_STD_POKE(p, v, |=, OR_INS, OP64b, GR_int64)
#endif

#ifndef poke_b_and
#define poke_b_and(p, v) INLINE_STD_POKE(p, v, &=, AND_INS, OP8b, GR_int8)
#endif
#ifndef poke_w_and
#define poke_w_and(p, v) INLINE_STD_POKE(p, v, &=, AND_INS, OP16b, GR_int16)
#endif
#ifndef poke_l_and
#define poke_l_and(p, v) INLINE_STD_POKE(p, v, &=, AND_INS, OP32b, GR_int32)
#endif
#if defined(GR_int64) && !defined(poke_h_and)
#define poke_h_and(p, v) INLINE_STD_POKE(p, v, &=, AND_INS, OP64b, GR_int64)
#endif

/* ------------------------------------------- special 24bpp handling --- */

#define __INTERN_24_PEEK__(P, F)                                      \
    (peek_b##F(P) | ((GR_int16u)peek_b##F(((GR_int8 *)(P)) + 1) << 8) \
        | ((GR_int32u)peek_b##F(((GR_int8 *)(P)) + 2) << 16))

#define __INTERN_24_POKE__(P, C, F, OP)                            \
    do {                                                           \
        poke_b##F##OP((P), (GR_int8)(C));                          \
        poke_b##F##OP(((GR_int8 *)(P)) + 1, (GR_int8)((C) >> 8));  \
        poke_b##F##OP(((GR_int8 *)(P)) + 2, (GR_int8)((C) >> 16)); \
    } while (0)

#ifndef INLINE_24_PEEK
#define INLINE_24_PEEK(p) __INTERN_24_PEEK__(p, _n)
#endif
#ifndef INLINE_24_POKE
#define INLINE_24_POKE(p, c, op, INS) __INTERN_24_POKE__(p, c, _n, op)
#endif

#ifndef peek_24
#define peek_24(p) INLINE_24_PEEK(p)
#endif

#ifndef poke_24_set
#define poke_24_set(p, c) INLINE_24_POKE(p, c, _set, MOV_INS)
#endif
#ifndef poke_24_xor
#define poke_24_xor(p, c) INLINE_24_POKE(p, c, _xor, XOR_INS)
#endif
#ifndef poke_24_or
#define poke_24_or(p, c) INLINE_24_POKE(p, c, _or, OR_INS)
#endif
#ifndef poke_24_and
#define poke_24_and(p, c) INLINE_24_POKE(p, c, _and, AND_INS)
#endif
#define poke_24 poke_24_set

/* ..._n is used in some macros to keep the preprocessor happy.
** Mapped to standard near memory commands :                    */
#define peek_b_n     peek_b
#define peek_w_n     peek_w
#define peek_l_n     peek_l
#define poke_b_n     poke_b
#define poke_w_n     poke_w
#define poke_l_n     poke_l
#define poke_b_n_set poke_b
#define poke_w_n_set poke_w
#define poke_l_n_set poke_l
#define poke_b_n_xor poke_b_xor
#define poke_w_n_xor poke_w_xor
#define poke_l_n_xor poke_l_xor
#define poke_b_n_or  poke_b_or
#define poke_w_n_or  poke_w_or
#define poke_l_n_or  poke_l_or
#define poke_b_n_and poke_b_and
#define poke_w_n_and poke_w_and
#define poke_l_n_and poke_l_and
#ifdef poke_h
#define peek_h_n     peek_h
#define poke_h_n     poke_h
#define poke_h_n_set poke_h
#define poke_h_n_xor poke_h_xor
#define poke_h_n_or  poke_h_or
#define poke_h_n_and poke_h_and
#endif

#define peek_24_n     peek_24
#define poke_24_n     poke_24_set
#define poke_24_n_set poke_24_set
#define poke_24_n_xor poke_24_xor
#define poke_24_n_or  poke_24_or
#define poke_24_n_and poke_24_and

#endif /* whole file */
