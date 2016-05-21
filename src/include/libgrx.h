/*
 * libgrx.h ---- GRX library private include file
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
 */

#ifndef __LIBGRX_H_INCLUDED__
#define __LIBGRX_H_INCLUDED__

#define USE_GRX_INTERNAL_DEFINITIONS

/* The LCC compiler on Linux requires this */
#if defined(__LCC__) && defined(__linux__)
/* make alloca work ... */
#  define __USE_MISC
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef __GRX20_H_INCLUDED__
#include "grx-3.0.h"
#endif

/*
** identify the compiler / system
** and check for special restrictions
*/
/* DEC alpha chips have special alignment
** restrictions. We'll have do care about them */
#if !defined(__alpha__) && defined(__alpha)
#define __alpha__ __alpha
#endif


/* Casting a lvalue on the left side of an assignment 
** causes error or warnings on several compilers:
**
** LCC v4.0
** SUN cc v4.0
** GCC v > 3
*/
#if !defined(NO_LEFTSIDE_LVALUE_CAST) &&                  \
    (   defined(__LCC__)                                  \
     || (defined(__GNUC__) && (__GNUC__>=3))              \
     || defined(__SUNPRO_C)                               )
#define NO_LEFTSIDE_LVALUE_CAST
#endif
/* Casting a pointer on the left side of an assignment
** also cuses problems on several systems:
**
** LCC v4.0
*/
#if !defined(NO_LEFTSIDE_PTR_CAST) &&                     \
    (   defined(__LCC__)                                  )
#define NO_LEFTSIDE_PTR_CAST
#endif

/* some CPU allow misaligned access to non byte location */
#if   defined(_MSC_VER) \
   || defined(__386__) \
   || defined(__i386__) \
   || defined(__i386)  \
   || defined(__x86_64__)
   /* x86 can write to misalgined 16bit locations */
#  define MISALIGNED_16bit_OK
#endif

#if   defined(__386__) \
   || defined(__i386__) \
   || defined(__i386)        \
   || defined(__x86_64__)
   /* x86 can write to misalgined 32bit locations */
#  define MISALIGNED_32bit_OK
#endif


/* need some n-bit types */
/* char should always be 8bit and short 16bit ... */
#define GR_int8  char
#define GR_int16 short
#if defined(__alpha__) || (defined(_MIPS_SZLONG) && _MIPS_SZLONG == 64)        || defined(__x86_64__)
#define GR_int32 int
#define GR_int64 long
#define GR_PtrInt long
#else
#define GR_int32 long
#define GR_PtrInt int
#endif

/* signed and unsigned variants of the above */
typedef   signed GR_int8  GR_int8s;
typedef   signed GR_int16 GR_int16s;
typedef   signed GR_int32 GR_int32s;
typedef unsigned GR_int8  GR_int8u;
typedef unsigned GR_int16 GR_int16u;
typedef unsigned GR_int32 GR_int32u;
#ifdef GR_int64
typedef   signed GR_int64 GR_int64s;
typedef unsigned GR_int64 GR_int64u;
#endif

/*
 * Debug support
 */
#if defined(DEBUG) && !defined(__GRXDEBUG_H_INCLUDED__)
# include "grxdebug.h"
#endif
#ifndef DBGPRINTF
# define DBGPRINTF(chk,x)
# define GRX_ENTER()
# define GRX_LEAVE()
# define GRX_RETURN(x) return x
#endif

/* simple pointer arithmetic */
#define ptrdiff(a,b) ( ((GR_int8 *)(a)) - ((GR_int8 *)(b)) )
#define ptradd(P,SKIP) ( (void *)( ((GR_int8 *)(P))+(SKIP)) )
#ifdef NO_LEFTSIDE_LVALUE_CAST
#define ptrinc(P,SKIP) do (P) = ptradd((P),(SKIP)); while (0)
#else
#define ptrinc(P,SKIP) do ((GR_int8 *)(P)) += (SKIP); while (0)
#endif

/*
 * function inline
 */
#ifdef __GNUC__
#define INLINE __inline__
#endif
#ifndef INLINE
#define INLINE
#endif

#endif  /* whole file */
