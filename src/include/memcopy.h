/**
 ** MEMCOPY.H ---- inline assembly memory copy macros
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **
 ** Basic and optimized memory block copy operations in byte, word and
 ** long sizes. The copies are available in WRITE, XOR, OR and AND modes.
 ** Intel CPU specific support is provided for the Turbo C and GNU C
 ** compilers. The i386 GCC version supports segment overrides. May
 ** work with other compilers and CPU-s, but is not optimized for them.
 **/
#ifndef __MEMCOPY_H_INCLUDED__
#define __MEMCOPY_H_INCLUDED__

#ifndef __MEMMODE_H_INCLUDED__
#include "memmode.h"
#endif

#ifdef  __GNUC__
#ifdef  __i386__
#define __INLINE_STD_ROWCOPY__(D,S,C,SIZE,TYPE) ({                        \
    __asm__ volatile("
        cld
        rep
        movs"#SIZE                                                      \
        : "=D" ((void *)(D)), "=S" ((void *)(S)), "=c" ((int)(C))        \
        : "0"  ((void *)(D)), "1"  ((void *)(S)), "2"  ((int)(C))        \
    );                                                                        \
})
#endif
#endif

#ifdef  __TURBOC__
#define __INLINE_STD_ROWCOPY__(D,S,C,SIZE,TYPE) do {                        \
    __emit__((char)(0x1e));                        /* push ds */                \
    _ES = (unsigned)(void _seg *)(void far *)(S);                        \
    _SI = (unsigned)(void near *)(S);                                        \
    __emit__((char)(0x06));                        /* push es */                \
    _ES = (unsigned)(void _seg *)(void far *)(D);                        \
    _DI = (unsigned)(void near *)(D);                                        \
    _CX = (int)(C);                                                        \
    __emit__((char)(0x1f));                        /* pop ds  */                \
    __emit__((char)(0xfc));                        /* cld           */                \
    __emit__((char)(0xf3));                        /* rep           */                \
    __emit__((char)(0xa4 + sizeof(TYPE) - 1));  /* stosB|W */                \
    __emit__((char)(0x1f));                        /* pop ds  */                \
} while(0)
#endif

#ifndef __INLINE_STD_ROWCOPY__
#define __INLINE_STD_ROWCOPY__(D,S,C,SIZE,TYPE) do {                        \
    *((TYPE *)(D))++ = *((TYPE *)(S))++;                                \
} while(--((int)(C)))
#endif

/*
 * rowcopy_<SIZE>[_<FAR>][_<OPER>](dst,src,count)
 */
#define rowcopy_b(d,s,c)        __INLINE_STD_ROWCOPY__(d,s,c,b,char)
#define rowcopy_w(d,s,c)        __INLINE_STD_ROWCOPY__(d,s,c,w,short)
#define rowcopy_l(d,s,c)        __INLINE_STD_ROWCOPY__(d,s,c,l,long)


#ifdef  __TURBOC__
#define __INLINE_MEMCOPY__(D,S,C,SIZE,TYPE) do {                        \
    void     far *_DST = (void far *)(D);                                \
    void     far *_SRC = (void far *)(S);                                \
    register int  _CNT = (int)(C);                                        \
    rowcopy_##SIZE(_DST,_SRC,_CNT);                                        \
} while(0)
#endif

#ifndef __INLINE_MEMCOPY__
#define __INLINE_MEMCOPY__(D,S,C,SIZE,TYPE) do {                        \
    register void *_DST = (void *)(D);                                        \
    register void *_SRC = (void *)(S);                                        \
    register long  _CNT = (int)(C);                                        \
    rowcopy_##SIZE(_DST,_SRC,_CNT);                                        \
} while(0)
#endif

/*
 * memcopy_<SIZE>[_<FAR>][_<OPER>](dst,src,count)
 */
#define memcopy_b(d,s,c)        __INLINE_MEMCOPY__(d,s,c,b,char)
#define memcopy_w(d,s,c)        __INLINE_MEMCOPY__(d,s,c,w,short)
#define memcopy_l(d,s,c)        __INLINE_MEMCOPY__(d,s,c,l,long)

/*
 * stuff to copy arrays, structures
 */
#ifdef  __TURBOC__
#define memcopy(d,s,c)                memcopy_w((d),(s),((c) >> 1))
#endif
#ifndef memcopy
#define memcopy(d,s,c)                memcopy_l((d),(s),((c) >> 2))
#endif

#define sttcopy(dstp,srcp)        memcopy((dstp),(srcp),sizeof(*(srcp)))

#ifdef  __TURBOC__
#undef  rowcopy_l
#undef  memcopy_l
#endif

#endif  /* whole file */
