/**
 ** ALLOCATE.H ---- common ground for malloc & friends in 16 & 32 bit envs
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#ifdef         __TURBOC__
#include <alloc.h>
#else
#include <malloc.h>
#endif

#ifndef  __TURBOC__
#define  farmalloc        malloc
#define  farfree        free
#endif

