/**
 ** ALLOCATE.H ---- common ground for malloc & friends in 16 & 32 bit envs
 **                 stack based temporary memory allocation
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#if defined(__alpha__) || (GRX_VERSION==GRX_VERSION_GENERIC_X11)
#  include <alloca.h>
#elif defined(__TURBOC__)
#  include <alloc.h>
#  include "bcc/allocate.h"
#elif defined(__WATCOMC__)
#  include <malloc.h>
#else
#  include <stdlib.h>
#endif

#ifdef _MSC_VER
#define farmalloc  _fmalloc
#define farrealloc _frealloc
#define farcalloc  _fcalloc
#define farfree    _ffree
#elif !defined(__TURBOC__)
#define farmalloc  malloc
#define farrealloc realloc
#define farcalloc  calloc
#define farfree    free
#endif

#ifdef _MSC_VER
#define setup_alloca() do { unsigned char _stack_dummy_var_ = '\001'
#define reset_alloca() } while (0)
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
#else
#define ALLOC(sze) alloca(sze)
#define FREE(p)
#define setup_ALLOC  setup_alloca
#define reset_ALLOC  reset_alloca
#endif

/* temp buffer for blits etc. */
extern void far *_GrTempBuffer;
extern unsigned  _GrTempBufferBytes;
#define _GrTempBufferAlloc(b) (                                     \
    ((unsigned)(b) <= _GrTempBufferBytes) ? _GrTempBuffer           \
                                          : _GrTempBufferAlloc_(b) )
extern void far *_GrTempBufferAlloc_(size_t bytes);
extern void _GrTempBufferFree(void);
