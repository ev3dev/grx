/**
 ** ARITH.H ---- some common integer arithmetic macros/inline functions
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **
 ** Intel CPU specific support is provided for the Turbo C and GNU C. May
 ** work with other compilers and CPU-s, but is not optimized for them.
 **/

#ifndef __ARITH_H_INCLUDED__
#define __ARITH_H_INCLUDED__

#ifdef __TURBOC__
/* prototype for __emit__() */
#include <dos.h>
#endif

/*
 * old standbys
 */
#ifndef min
#define min(x,y)    (((x) < (y)) ?  (x) : (y))
#endif
#ifndef max
#define max(x,y)    (((x) > (y)) ?  (x) : (y))
#endif
#ifndef __ABS
#define __ABS(x)    (((x) < (0)) ? -(x) : (x))
#endif
#ifndef abs
#define abs(x)      __ABS(x)
#endif
#ifndef scale
#define scale(x,n,d)    (((x) * (n)) / (d))
#endif
#ifndef imin
#define imin(x,y)   min((int)(x),(int)(y))
#endif
#ifndef imax
#define imax(x,y)   max((int)(x),(int)(y))
#endif
#ifndef iabs
#define iabs(x)     abs((int)(x))
#endif
#ifndef umin
#define umin(x,y)   min((unsigned int)(x),(unsigned int)(y))
#endif
#ifndef umax
#define umax(x,y)   max((unsigned int)(x),(unsigned int)(y))
#endif
#ifndef lmin
#define lmin(x,y)   min((long)(x),(long)(y))
#endif
#ifndef lmax
#define lmax(x,y)   max((long)(x),(long)(y))
#endif
#ifndef labs
#define labs(x)     __ABS((long)(x))
#endif
#ifndef ulmin
#define ulmin(x,y)  min((unsigned long)(x),(unsigned long)(y))
#endif
#ifndef ulmax
#define ulmax(x,y)  max((unsigned long)(x),(unsigned long)(y))
#endif

/*
 * swap and sort stuff
 */
#define iswap(x,y) {                                            \
        int _swap_tmpval_ = (x);                                \
        (x) = (y);                                              \
        (y) = _swap_tmpval_;                                    \
}

#define lswap(x,y) {                                            \
        long _swap_tmpval_ = (x);                               \
        (x) = (y);                                              \
        (y) = _swap_tmpval_;                                    \
}

#define isort(x,y) {                                            \
        if((int)(x) > (int)(y)) iswap(x,y)                      \
}

#define usort(x,y) {                                            \
        if((unsigned int)(x) > (unsigned int)(y)) iswap(x,y)    \
}

#define lsort(x,y) {                                            \
        if((long)(x) > (long)(y)) lswap(x,y)                    \
}

#define ulsort(x,y) {                                           \
        if((unsigned long)(x) > (unsigned long)(y)) lswap(x,y)  \
}


/*
 * couple of 'sizeof'-like useful macros
 */
#ifndef bsizeof
#define bsizeof(s)  (sizeof(s) / sizeof(char))
#endif
#ifndef wsizeof
#define wsizeof(s)  (sizeof(s) / sizeof(short))
#endif
#ifndef lsizeof
#define lsizeof(s)  (sizeof(s) / sizeof(long))
#endif
#ifndef bitsof
#define bitsof(s)   (sizeof(s) * 8)
#endif
#ifndef bytesof
#define bytesof(s)  ((sizeof(s) + sizeof(char) - 1) / sizeof(char))
#endif
#ifndef wordsof
#define wordsof(s)  ((sizeof(s) + sizeof(short) - 1) / sizeof(short))
#endif
#ifndef longsof
#define longsof(s)  ((sizeof(s) + sizeof(long) - 1) / sizeof(long))
#endif
#ifndef itemsof
#define itemsof(arr)    (sizeof(arr) / sizeof((arr)[0]))
#endif
#ifndef offsetof
#define offsetof(structype,field) (int)(                        \
        (char *)(&((structype *)(0))->field) -                  \
        (char *)(0)                                             \
)
#endif

/*
 * [i|u]mul32(x,y)
 * multiply two int-s for a long result
 */
#ifdef  __TURBOC__
/* void    __emit__(); */
#define imul32(X,Y) (                                           \
        _AX = (int)(X),                                         \
        __emit__((char)(0x50)),               /* push ax */     \
        _AX = (int)(Y),                                         \
        __emit__((char)(0x5a)),               /* pop dx */      \
        __emit__((char)(0xf7)),               /* imul dx */     \
            __emit__((char)(0xea)),                             \
        _BX = _AX,                                              \
        _CX = _DX,                                              \
        (long)((void _seg *)_CX + (void near *)_BX)             \
)
#define umul32(X,Y) (                                           \
        _AX = (int)(X),                                         \
        __emit__((char)(0x50)),               /* push ax */     \
        _AX = (int)(Y),                                         \
        __emit__((char)(0x5a)),               /* pop dx */      \
        __emit__((char)(0xf7)),               /* mul dx */      \
            __emit__((char)(0xe2)),                             \
        _BX = _AX,                                              \
        _CX = _DX,                                              \
        (unsigned long)((void _seg *)_CX + (void near *)_BX)    \
)
#endif  /* __TURBOC__ */

#ifndef imul32
#define imul32(X,Y) ((long)(X) * (long)(Y))
#define umul32(X,Y) ((unsigned long)(X) * (unsigned long)(Y))
#endif

/*
 * [i|u]scale(X,N,D)
 * scale an integer with long intermediate result but without using long
 * arithmetic all the way
 */
#ifdef  __TURBOC__
#define iscale(X,N,D) (                                             \
        _AX = (int)(D),                                             \
        __emit__((char)(0x50)),                     /* push ax */   \
        _AX = (int)(N),                                             \
        __emit__((char)(0x50)),                     /* push ax */   \
        _AX = (int)(X),                                             \
        __emit__((char)(0x5a)),                     /* pop dx */    \
        __emit__((char)(0x59)),                     /* pop cx */    \
        __emit__((char)(0xf7)),                     /* imul dx */   \
            __emit__((char)(0xea)),                                 \
        __emit__((char)(0xf7)),                     /* idiv cx */   \
            __emit__((char)(0xf9)),                                 \
        (int)_AX                                                    \
)
#define uscale(X,N,D) (                                             \
        _AX = (int)(D),                                             \
        __emit__((char)(0x50)),                     /* push ax */   \
        _AX = (int)(N),                                             \
        __emit__((char)(0x50)),                     /* push ax */   \
        _AX = (int)(X),                                             \
        __emit__((char)(0x5a)),                     /* pop dx */    \
        __emit__((char)(0x59)),                     /* pop cx */    \
        __emit__((char)(0xf7)),                     /* mul dx */    \
            __emit__((char)(0xe2)),                                 \
        __emit__((char)(0xf7)),                     /* div cx */    \
            __emit__((char)(0xf1)),                                 \
        (unsigned int)_AX                                           \
)
#define irscale(X,N,D) (                                                  \
        _DX = iscale(((int)(X) << 1),N,D),                                \
        __emit__((char)(0x03)),                       /* add dx,dx */     \
            __emit__((char)(0xd2)),                                       \
        __emit__((char)(0x1d)),                       /* sbc ax,0xffff */ \
            __emit__((char)(-1)),                                         \
            __emit__((char)(-1)),                                         \
        __emit__((char)(0xd1)),                       /* sar ax,1 */      \
            __emit__((char)(0xf8)),                                       \
        (int)_AX                                                          \
)
#endif  /* __TURBOC__ */

#ifdef  __GNUC__
#define irscale(X,N,D) ({                                           \
        register int _SclVal_ = iscale(((int)(X) << 1),N,D);        \
        (_SclVal_ + (_SclVal_ >> (bitsof(int) - 1)) + 1) >> 1;      \
})
#endif

#ifndef iscale
#define iscale(X,N,D)   (int)(imul32(X,N) / (long)(D))
#endif
#ifndef uscale
#define uscale(X,N,D)   (unsigned int)(umul32(X,N) / (unsigned long)(D))
#endif
#ifndef irscale
#define irscale(X,N,D)  ((                                              \
        iscale(((int)(X) << 1),N,D) +                                   \
        (((int)(X) ^ (int)(N) ^ (int)(D)) >> (bitsof(int) - 1)) +       \
        1                                                               \
) >> 1)
#endif
#ifndef urscale
#define urscale(X,N,D)  ((uscale(((int)(X) << 1),N,D) + 1) >> 1)
#endif

/*
 * replicate_<FROM>2<TO>(byte_or_word_value)
 * copy the lower byte(s) of a byte or word into the upper byte(s)
 */
#define __INLINE_REPLICATE__(V,TFROM,TTO) (                             \
        ((unsigned TTO)(unsigned TFROM)(V)) |                           \
        ((unsigned TTO)(unsigned TFROM)(V) << (sizeof(TFROM) * 8))      \
)

#ifdef  __GNUC__
#ifdef  __i386__
#define replicate_b2w(BYTE) (__builtin_constant_p(BYTE) ?       \
        (long)__INLINE_REPLICATE__(BYTE,char,int) :             \
        ({                                                      \
        register long _repvalue;                                \
        __asm__("
                xorl    %%ecx,%%ecx
                movb    %%al,%%cl
                movb    %%al,%%ch"                              \
                : "=c" (_repvalue)                              \
                : "a"  ((char)(BYTE))                           \
        );                                                      \
        _repvalue;                                              \
        })                                                      \
)
#define replicate_w2l(WORD) (__builtin_constant_p(WORD) ?       \
        (long)__INLINE_REPLICATE__(WORD,short,int) :            \
        ({                                                      \
        register long  _repvalue;                               \
        register short _scratch;                                \
        __asm__("
                movw    %%ax,%1
                shll    $16,%%eax
                movw    %1,%%ax"                                \
                : "=a" (_repvalue), "=r" (_scratch)             \
                : "a"  ((short)(WORD))                          \
        );                                                      \
        _repvalue;                                              \
        })                                                      \
)
#define replicate_b2l(BYTE) (__builtin_constant_p(BYTE) ?               \
        (long)__INLINE_REPLICATE__(replicate_b2w(BYTE),short,int) :     \
        ({                                                              \
        register long  _repvalue;                                       \
        register short _scratch;                                        \
        __asm__("
                movb    %%al,%%ah
                movw    %%ax,%1
                shll    $16,%%eax
                movw    %1,%%ax"                                        \
                : "=a" (_repvalue), "=r" (_scratch)                     \
                : "a"  ((char)(BYTE))                                   \
        );                                                              \
        _repvalue;                                                      \
        })                                                              \
)
#endif  /* __i386__ */
#endif  /* __GNUC__ */

#ifdef  __TURBOC__
#define replicate_b2w(BYTE) (                                   \
        _AL = (char)(BYTE),                                     \
        _AH = _AL,                                              \
        (int)_AX                                                \
)
#define replicate_w2l(WORD) (                                   \
        _AX = (int)(WORD),                                      \
        _DX = _AX,                                              \
        (long)((void _seg *)_DX + (void near *)_AX)             \
)
#define replicate_b2l(BYTE) (                                   \
        _AL = (char)(BYTE),                                     \
        _AH = _AL,                                              \
        _DX = _AX,                                              \
        (long)((void _seg *)_DX + (void near *)_AX)             \
)
#endif  /* __TURBOC__ */

#ifndef replicate_b2w
#define replicate_b2w(BYTE) __INLINE_REPLICATE__(BYTE,char,int)
#endif
#ifndef replicate_w2l
#define replicate_w2l(WORD) __INLINE_REPLICATE__(WORD,short,long)
#endif
#ifndef replicate_b2l
#define replicate_b2l(BYTE) replicate_w2l(replicate_b2w(BYTE))
#endif

#endif  /* whole file */

