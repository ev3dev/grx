/*
 * allocate.h ---- common ground for malloc & friends in 16 & 32 bit envs
 *                 stack based temporary memory allocation
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

#include <glib.h>

#if defined(__alpha__) || (GRX_VERSION == GRX_VERSION_GENERIC_X11) && !defined(_AIX)
#include <alloca.h>
#elif defined(_MSC_VER) && defined(_WIN32)
#include <malloc.h>
#elif defined(__MINGW32__) && !defined(alloca)
#define alloca __builtin_alloca
#else
#include <stdlib.h>
#endif

#ifndef setup_alloca
#define setup_alloca()
#define reset_alloca()
#endif

/* ALLOC / FREE : use alloca if possible */
#ifdef SMALL_STACK
#define ALLOC(sze) malloc(sze)
#define FREE(p)    free(p)
#define setup_ALLOC()
#define reset_ALLOC()
#elif defined(_MSC_VER) && !defined(_WIN32)
#define ALLOC(sze) _alloca(sze)
#define FREE(p)
#define setup_ALLOC setup_alloca
#define reset_ALLOC reset_alloca
#else
#define ALLOC(sze) alloca(sze)
#define FREE(p)
#define setup_ALLOC setup_alloca
#define reset_ALLOC reset_alloca
#endif

/* temp buffer for blits etc. */
G_GNUC_INTERNAL extern void *_GrTempBuffer;
G_GNUC_INTERNAL extern unsigned _GrTempBufferBytes;
#define _GrTempBufferAlloc(b) \
    (((unsigned)(b) <= _GrTempBufferBytes) ? _GrTempBuffer : _GrTempBufferAlloc_(b))
G_GNUC_INTERNAL extern void *_GrTempBufferAlloc_(size_t bytes);
G_GNUC_INTERNAL extern void _GrTempBufferFree(void);
