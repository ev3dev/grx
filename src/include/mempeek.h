/**
 ** MEMPEEK.H ---- (far) memory read/write operations
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **
 ** Basic memory peek and poke operations in byte, word and long sizes.
 ** The poke operations are available in WRITE, XOR, OR and AND versions.
 ** Intel CPU specific support is provided for the Turbo C and GNU C
 ** compilers. The i386 GCC version supports segment overrides. May
 ** work with other compilers and CPU-s, but is not optimized for them.
 **/
#ifndef __MEMPEEK_H_INCLUDED__
#define __MEMPEEK_H_INCLUDED__

#ifndef __MEMMODE_H_INCLUDED__
#include "memmode.h"
#endif

#ifdef  __GNUC__
#ifdef  __i386__
#define __INLINE_386_PEEK__(P,SIZE,T,SEL) ({                \
    unsigned T _peekvalue;                                \
    if(sizeof(T) == 1) __asm__ volatile(                \
        "mov"#SIZE" "#SEL"(%1),%0"                        \
        : "=q" (_peekvalue)                                \
        : "r"  (((unsigned T *)(P)))                        \
    );                                                        \
    else __asm__ volatile(                                \
        "mov"#SIZE" "#SEL"(%1),%0"                        \
        : "=r" (_peekvalue)                                \
        : "r"  (((unsigned T *)(P)))                        \
    );                                                        \
    _peekvalue;                                                \
})
#define __INLINE_386_POKE__(P,V,INS,SIZE,T,SEL) ({        \
    if(sizeof(T) == 1) __asm__ volatile(                \
        ""#INS""#SIZE" %1,"#SEL"%0"                        \
        : "=m" (*((unsigned T *)(P)))                        \
        : "qn" ((unsigned T)(V))                        \
    );                                                        \
    else __asm__ volatile(                                \
        ""#INS""#SIZE" %1,"#SEL"%0"                        \
        : "=m" (*((unsigned T *)(P)))                        \
        : "rn" ((unsigned T)(V))                        \
    );                                                        \
})
#define __INLINE_STD_PEEK__(P,S,T)          __INLINE_386_PEEK__(P,S,T,)
#define __INLINE_STD_POKE__(P,V,OP,I,S,T) __INLINE_386_POKE__(P,V,I,S,T,)
#ifdef  I386_GCC_FAR_MEMORY
#define I386_GCC_FAR_SELECTOR                  "%%fs:"
#define __INLINE_FAR_PEEK__(P,S,T)          __INLINE_386_PEEK__(P,S,T,%%fs:)
#define __INLINE_FAR_POKE__(P,V,OP,I,S,T) __INLINE_386_POKE__(P,V,I,S,T,%%fs:)
#define setup_far_selector(S) ({                        \
    __asm__ volatile(                                        \
        "movw %0,%%fs"                                        \
        : /* no outputs */                                \
        : "r" ((unsigned short)(S))                        \
    );                                                        \
})
#else        /* I386_GCC_FAR_MEMORY */
#define I386_GCC_FAR_SELECTOR                  ""
#endif  /* I386_GCC_FAR_MEMORY */
#endif  /* __i386__ */
#endif  /* __GNUC__ */

#ifdef  __TURBOC__
#define __INLINE_STD_PEEK__(P,S,T)          (*(unsigned T far *)(P))
#define __INLINE_STD_POKE__(P,V,OP,I,S,T) (*(unsigned T far *)(P) OP (V))
#endif  /* __TURBOC__ */

#ifndef __INLINE_STD_PEEK__
#define __INLINE_STD_PEEK__(P,S,T)          (*(unsigned T *)(P))
#define __INLINE_STD_POKE__(P,V,OP,I,S,T) (*(unsigned T *)(P) OP (V))
#endif

#ifndef __INLINE_FAR_PEEK__
#define __INLINE_FAR_PEEK__                  __INLINE_STD_PEEK__
#define __INLINE_FAR_POKE__                  __INLINE_STD_POKE__
#endif

/*
 * setup_far_selector(segment_selector)
 */
#ifndef setup_far_selector
#define setup_far_selector(S)
#endif

/*
 * peek_<SIZE>... (pointer)
 * poke_<SIZE>... (pointer,value)
 */
#define peek_b(p)            __INLINE_STD_PEEK__(p,b,char)
#define peek_w(p)            __INLINE_STD_PEEK__(p,w,short)
#define peek_l(p)            __INLINE_STD_PEEK__(p,l,long)

#define poke_b(p,v)            __INLINE_STD_POKE__(p,v,=,mov,b,char)
#define poke_w(p,v)            __INLINE_STD_POKE__(p,v,=,mov,w,short)
#define poke_l(p,v)            __INLINE_STD_POKE__(p,v,=,mov,l,long)

#define poke_b_xor(p,v)            __INLINE_STD_POKE__(p,v,^=,xor,b,char)
#define poke_w_xor(p,v)            __INLINE_STD_POKE__(p,v,^=,xor,w,short)
#define poke_l_xor(p,v)            __INLINE_STD_POKE__(p,v,^=,xor,l,long)

#define poke_b_or(p,v)            __INLINE_STD_POKE__(p,v,|=,or,b,char)
#define poke_w_or(p,v)            __INLINE_STD_POKE__(p,v,|=,or,w,short)
#define poke_l_or(p,v)            __INLINE_STD_POKE__(p,v,|=,or,l,long)

#define poke_b_and(p,v)            __INLINE_STD_POKE__(p,v,&=,and,b,char)
#define poke_w_and(p,v)            __INLINE_STD_POKE__(p,v,&=,and,w,short)
#define poke_l_and(p,v)            __INLINE_STD_POKE__(p,v,&=,and,l,long)

#define peek_b_f(p)            __INLINE_FAR_PEEK__(p,b,char)
#define peek_w_f(p)            __INLINE_FAR_PEEK__(p,w,short)
#define peek_l_f(p)            __INLINE_FAR_PEEK__(p,l,long)

#define poke_b_f(p,v)            __INLINE_FAR_POKE__(p,v,=,mov,b,char)
#define poke_w_f(p,v)            __INLINE_FAR_POKE__(p,v,=,mov,w,short)
#define poke_l_f(p,v)            __INLINE_FAR_POKE__(p,v,=,mov,l,long)

#define poke_b_f_xor(p,v)   __INLINE_FAR_POKE__(p,v,^=,xor,b,char)
#define poke_w_f_xor(p,v)   __INLINE_FAR_POKE__(p,v,^=,xor,w,short)
#define poke_l_f_xor(p,v)   __INLINE_FAR_POKE__(p,v,^=,xor,l,long)

#define poke_b_f_or(p,v)    __INLINE_FAR_POKE__(p,v,|=,or,b,char)
#define poke_w_f_or(p,v)    __INLINE_FAR_POKE__(p,v,|=,or,w,short)
#define poke_l_f_or(p,v)    __INLINE_FAR_POKE__(p,v,|=,or,l,long)

#define poke_b_f_and(p,v)   __INLINE_FAR_POKE__(p,v,&=,and,b,char)
#define poke_w_f_and(p,v)   __INLINE_FAR_POKE__(p,v,&=,and,w,short)
#define poke_l_f_and(p,v)   __INLINE_FAR_POKE__(p,v,&=,and,l,long)

#endif  /* whole file */

