/**
 ** MEMFILL.H ---- inline assembly memory fill macros
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **
 ** Basic and optimized memory block fill operations in byte, word and
 ** long sizes. The fills are available in WRITE, XOR, OR and AND modes.
 ** Intel CPU specific support is provided for the Turbo C and GNU C
 ** compilers. The i386 GCC version supports segment overrides. May
 ** work with other compilers and CPU-s, but is not optimized for them.
 **/
#ifndef __MEMFILL_H_INCLUDED__
#define __MEMFILL_H_INCLUDED__

#ifndef __MEMMODE_H_INCLUDED__
#include "memmode.h"
#endif
#ifndef __MEMPEEK_H_INCLUDED__
#include "mempeek.h"
#endif
#ifndef __ARITH_H_INCLUDED__
#include "arith.h"
#endif

/*
 * Unoptimized row and column fills
 */
#ifdef  __GNUC__
#ifdef  __i386__
#define __INLINE_STD_ROWFILL__(P,V,C,SIZE,TYPE) ({                      \
    __asm__ volatile("
        cld
        rep
        stos"#SIZE                                                      \
        : "=D" ((void *)(P)), "=c" ((int)(C))                           \
        : "0"  ((void *)(P)), "1"  ((int)(C)),                          \
          "a"  ((TYPE)(V))                                              \
    );                                                                  \
})
#define __INLINE_386_COLFILL__(P,V,C,SKIP,INS,SIZE,TYPE,SEL) ({         \
    if(__builtin_constant_p(SKIP) && ((SKIP) == 1)) __asm__ volatile("
        incl    %1
        shrl    $1,%1
        jnc     1f
        .align 2,0x90
    0:  "#INS""#SIZE"   %4,"#SEL"(%0)
        incl    %0
    1:  "#INS""#SIZE"   %4,"#SEL"(%0)
        incl    %0
        decl    %1
        jne     0b"                                                     \
        : "=r" ((void *)(P)), "=r" ((int)(C))                           \
        : "0"  ((void *)(P)), "1"  ((int)(C)),                          \
          "qn" ((TYPE)(V))                                              \
    ); else                                                             \
    if(__builtin_constant_p(SKIP) && ((SKIP) == 2)) __asm__ volatile("
        incl    %1
        shrl    $1,%1
        jnc     1f
        .align 2,0x90
    0:  "#INS""#SIZE"   %4,"#SEL"(%0)
        leal    2(%0),%0
    1:  "#INS""#SIZE"   %4,"#SEL"(%0)
        leal    2(%0),%0
        decl    %1
        jne     0b"                                                     \
        : "=r" ((void *)(P)), "=r" ((int)(C))                           \
        : "0"  ((void *)(P)), "1"  ((int)(C)),                          \
          "qn" ((TYPE)(V))                                              \
    ); else                                                             \
    if(__builtin_constant_p(SKIP) && ((SKIP) == 4)) __asm__ volatile("
        incl    %1
        shrl    $1,%1
        jnc     1f
        .align 2,0x90
    0:  "#INS""#SIZE"   %4,"#SEL"(%0)
        leal    4(%0),%0
    1:  "#INS""#SIZE"   %4,"#SEL"(%0)
        leal    4(%0),%0
        decl    %1
        jne     0b"                                                     \
        : "=r" ((void *)(P)), "=r" ((int)(C))                           \
        : "0"  ((void *)(P)), "1"  ((int)(C)),                          \
          "qn" ((TYPE)(V))                                              \
    );                                                                  \
    else __asm__ volatile("
        incl    %1
        shrl    $1,%1
        jnc     1f
        .align 2,0x90
    0:  "#INS""#SIZE"   %5,"#SEL"(%0)
        addl    %4,%0
    1:  "#INS""#SIZE"   %5,"#SEL"(%0)
        addl    %4,%0
        decl    %1
        jne     0b"                                                     \
        : "=r" ((void *)(P)), "=r" ((int)(C))                           \
        : "0"  ((void *)(P)), "1"  ((int)(C)),                          \
          "ng" ((int)(SKIP)), "qn" ((TYPE)(V))                          \
    );                                                                  \
})
#define __INLINE_STD_COLFILL__(P,V,C,SKIP,OP,INS,SIZE,TYPE)             \
        __INLINE_386_COLFILL__(P,V,C,SKIP,INS,SIZE,TYPE,)
#ifdef  I386_GCC_FAR_MEMORY
#define __INLINE_FAR_COLFILL__(P,V,C,SKIP,OP,INS,SIZE,TYPE)             \
        __INLINE_386_COLFILL__(P,V,C,SKIP,INS,SIZE,TYPE,%%fs:)
#else   /* I386_GCC_FAR_MEMORY */
#define __INLINE_FAR_ROWFILL__(P,V,C,SIZE,TYPE)                         \
        __INLINE_STD_ROWFILL__(P,V,C,SIZE,TYPE)
#endif  /* I386_GCC_FAR_MEMORY */
#endif  /* __i386__ */
#endif  /* __GNUC__ */

#ifdef  __TURBOC__
#define __INLINE_STD_ROWFILL__(P,V,C,SIZE,TYPE) do {                    \
    _ES = (unsigned)(void _seg *)(void far *)(P);                       \
    _DI = (unsigned)(void near *)(P);                                   \
    _CX = (int)(C);                                                     \
    _AX = (int)(V);                                                     \
    __emit__((char)(0xfc));                     /* cld     */           \
    __emit__((char)(0xf3));                     /* rep     */           \
    __emit__((char)(0xaa + sizeof(TYPE) - 1));  /* stosB|W */           \
} while(0)
#define __INLINE_STD_COLFILL__(P,V,C,SKIP,OP,INS,SIZE,TYPE) do {        \
    _ES = (unsigned)(void _seg *)(void far *)(P);                       \
    _BX = (unsigned)(void near *)(P);                                   \
    _CX = (unsigned)(C);                                                \
    _AX = (unsigned)(V);                                                \
    do {                                                                \
        *((TYPE _seg *)_ES + (TYPE near *)_BX) OP (TYPE)_AX;            \
        _BX += (SKIP);                                                  \
        _CX--;                                                          \
    } while(_CX);                                                       \
} while(0)
#define __INLINE_FAR_ROWFILL__(P,V,C,SIZE,TYPE)                         \
        __INLINE_STD_ROWFILL__(P,V,C,SIZE,TYPE)
#endif  /* TURBOC */

#ifndef __INLINE_STD_COLFILL__
#define __INLINE_STD_COLFILL__(P,V,C,SKIP,OP,INS,SIZE,TYPE) do {        \
    *((TYPE *)(P)) OP (TYPE)(V);                                        \
    ((char *)(P)) += (int)(SKIP);                                       \
} while(--((int)(C)))
#endif

#ifndef __INLINE_STD_OPRFILL__
#define __INLINE_STD_OPRFILL__(P,V,C,OP,INS,SIZE,TYPE)                  \
        __INLINE_STD_COLFILL__(P,V,C,sizeof(TYPE),OP,INS,SIZE,TYPE)
#endif

#ifndef __INLINE_STD_ROWFILL__
#define __INLINE_STD_ROWFILL__(P,V,C,SIZE,TYPE)                         \
        __INLINE_STD_OPRFILL__(P,V,C,=,mov,SIZE,TYPE)
#endif

#ifndef __INLINE_FAR_COLFILL__
#define __INLINE_FAR_COLFILL__(P,V,C,SKIP,OP,INS,SIZE,TYPE)             \
        __INLINE_STD_COLFILL__(P,V,C,SKIP,OP,INS,SIZE,TYPE)
#endif

#ifndef __INLINE_FAR_OPRFILL__
#define __INLINE_FAR_OPRFILL__(P,V,C,OP,INS,SIZE,TYPE)                  \
        __INLINE_FAR_COLFILL__(P,V,C,sizeof(TYPE),OP,INS,SIZE,TYPE)
#endif

#ifndef __INLINE_FAR_ROWFILL__
#define __INLINE_FAR_ROWFILL__(P,V,C,SIZE,TYPE)                         \
        __INLINE_FAR_OPRFILL__(P,V,C,=,mov,SIZE,TYPE)
#endif

/*
 * rowfill_<SIZE>[_<FAR>][_<OPER>](pointer,value,count)
 */
#define rowfill_b(p,v,c)          __INLINE_STD_ROWFILL__(p,v,c,b,char)
#define rowfill_w(p,v,c)          __INLINE_STD_ROWFILL__(p,v,c,w,short)
#define rowfill_l(p,v,c)          __INLINE_STD_ROWFILL__(p,v,c,l,long)

#define rowfill_b_xor(p,v,c)      __INLINE_STD_OPRFILL__(p,v,c,^=,xor,b,char)
#define rowfill_w_xor(p,v,c)      __INLINE_STD_OPRFILL__(p,v,c,^=,xor,w,short)
#define rowfill_l_xor(p,v,c)      __INLINE_STD_OPRFILL__(p,v,c,^=,xor,l,long)

#define rowfill_b_or(p,v,c)       __INLINE_STD_OPRFILL__(p,v,c,|=,or,b,char)
#define rowfill_w_or(p,v,c)       __INLINE_STD_OPRFILL__(p,v,c,|=,or,w,short)
#define rowfill_l_or(p,v,c)       __INLINE_STD_OPRFILL__(p,v,c,|=,or,l,long)

#define rowfill_b_and(p,v,c)      __INLINE_STD_OPRFILL__(p,v,c,&=,and,b,char)
#define rowfill_w_and(p,v,c)      __INLINE_STD_OPRFILL__(p,v,c,&=,and,w,short)
#define rowfill_l_and(p,v,c)      __INLINE_STD_OPRFILL__(p,v,c,&=,and,l,long)

#define rowfill_b_f(p,v,c)        __INLINE_FAR_ROWFILL__(p,v,c,b,char)
#define rowfill_w_f(p,v,c)        __INLINE_FAR_ROWFILL__(p,v,c,w,short)
#define rowfill_l_f(p,v,c)        __INLINE_FAR_ROWFILL__(p,v,c,l,long)

#define rowfill_b_f_xor(p,v,c)    __INLINE_FAR_OPRFILL__(p,v,c,^=,xor,b,char)
#define rowfill_w_f_xor(p,v,c)    __INLINE_FAR_OPRFILL__(p,v,c,^=,xor,w,short)
#define rowfill_l_f_xor(p,v,c)    __INLINE_FAR_OPRFILL__(p,v,c,^=,xor,l,long)

#define rowfill_b_f_or(p,v,c)     __INLINE_FAR_OPRFILL__(p,v,c,|=,or,b,char)
#define rowfill_w_f_or(p,v,c)     __INLINE_FAR_OPRFILL__(p,v,c,|=,or,w,short)
#define rowfill_l_f_or(p,v,c)     __INLINE_FAR_OPRFILL__(p,v,c,|=,or,l,long)

#define rowfill_b_f_and(p,v,c)    __INLINE_FAR_OPRFILL__(p,v,c,&=,and,b,char)
#define rowfill_w_f_and(p,v,c)    __INLINE_FAR_OPRFILL__(p,v,c,&=,and,w,short)
#define rowfill_l_f_and(p,v,c)    __INLINE_FAR_OPRFILL__(p,v,c,&=,and,l,long)

/*
 * colfill_<SIZE>[_<FAR>][_<OPER>](pointer,offset,value,count)
 */
#define colfill_b(p,o,v,c)        __INLINE_STD_COLFILL__(p,v,c,o,=,mov,b,char)
#define colfill_w(p,o,v,c)        __INLINE_STD_COLFILL__(p,v,c,o,=,mov,w,short)
#define colfill_l(p,o,v,c)        __INLINE_STD_COLFILL__(p,v,c,o,=,mov,l,long)

#define colfill_b_xor(p,o,v,c)    __INLINE_STD_COLFILL__(p,v,c,o,^=,xor,b,char)
#define colfill_w_xor(p,o,v,c)    __INLINE_STD_COLFILL__(p,v,c,o,^=,xor,w,short)
#define colfill_l_xor(p,o,v,c)    __INLINE_STD_COLFILL__(p,v,c,o,^=,xor,l,long)

#define colfill_b_or(p,o,v,c)     __INLINE_STD_COLFILL__(p,v,c,o,|=,or,b,char)
#define colfill_w_or(p,o,v,c)     __INLINE_STD_COLFILL__(p,v,c,o,|=,or,w,short)
#define colfill_l_or(p,o,v,c)     __INLINE_STD_COLFILL__(p,v,c,o,|=,or,l,long)

#define colfill_b_and(p,o,v,c)    __INLINE_STD_COLFILL__(p,v,c,o,&=,and,b,char)
#define colfill_w_and(p,o,v,c)    __INLINE_STD_COLFILL__(p,v,c,o,&=,and,w,short)
#define colfill_l_and(p,o,v,c)    __INLINE_STD_COLFILL__(p,v,c,o,&=,and,l,long)

#define colfill_b_f(p,o,v,c)      __INLINE_FAR_COLFILL__(p,v,c,o,=,mov,b,char)
#define colfill_w_f(p,o,v,c)      __INLINE_FAR_COLFILL__(p,v,c,o,=,mov,w,short)
#define colfill_l_f(p,o,v,c)      __INLINE_FAR_COLFILL__(p,v,c,o,=,mov,l,long)

#define colfill_b_f_xor(p,o,v,c)  __INLINE_FAR_COLFILL__(p,v,c,o,^=,xor,b,char)
#define colfill_w_f_xor(p,o,v,c)  __INLINE_FAR_COLFILL__(p,v,c,o,^=,xor,w,short)
#define colfill_l_f_xor(p,o,v,c)  __INLINE_FAR_COLFILL__(p,v,c,o,^=,xor,l,long)

#define colfill_b_f_or(p,o,v,c)   __INLINE_FAR_COLFILL__(p,v,c,o,|=,or,b,char)
#define colfill_w_f_or(p,o,v,c)   __INLINE_FAR_COLFILL__(p,v,c,o,|=,or,w,short)
#define colfill_l_f_or(p,o,v,c)   __INLINE_FAR_COLFILL__(p,v,c,o,|=,or,l,long)

#define colfill_b_f_and(p,o,v,c)  __INLINE_FAR_COLFILL__(p,v,c,o,&=,and,b,char)
#define colfill_w_f_and(p,o,v,c)  __INLINE_FAR_COLFILL__(p,v,c,o,&=,and,w,short)
#define colfill_l_f_and(p,o,v,c)  __INLINE_FAR_COLFILL__(p,v,c,o,&=,and,l,long)

/*
 * Optimized row fill operations in byte, word and long sizes.
 * The idea is that the bulk of the fill operation is done using
 * aligned long fills with a few possible shorter alignment and/or pad
 * fills at start and/or end. The smaller size routines (byte, word)
 * still assume a long fill argument with the fill value replicated
 * in the upper bytes.
 */
#ifdef  __TURBOC__
#define __INLINE_B_REPFILL__(P,V,C,FMODE)       rowfill_b##FMODE(P,V,C)
#define __INLINE_W_REPFILL__(P,V,C,FMODE)       rowfill_w##FMODE(P,V,C)
#define __INLINE_L_REPFILL__(P,V,C,FMODE)       rowfill_l##FMODE(P,V,C)
#define freplicate_b2w(V)                       (V)
#define freplicate_w2l(V)                       (V)
#define freplicate_b2l(V)                       (V)
#endif

#ifndef __INLINE_B_REPFILL__
#define __INLINE_B_REPFILL__(P,V,C,FMODE) do {                          \
    if((int)(C) > 3) {                                                  \
        if((int)(P) & 1) {                                              \
            poke_b##FMODE((P),(V));                                     \
            ((char *)(P))++;                                            \
            ((int)(C))--;                                               \
        }                                                               \
        if((int)(P) & 2) {                                              \
            poke_w##FMODE((P),(V));                                     \
            ((short *)(P))++;                                           \
            ((int)(C)) -= 2;                                            \
        }                                                               \
        if((int)(C) > 3) {                                              \
            int _cc_ = (unsigned int)(C) >> 2;                          \
            rowfill_l##FMODE((P),(V),_cc_);                             \
        }                                                               \
    }                                                                   \
    if((int)(C) & 2) {                                                  \
        poke_w##FMODE((P),(V));                                         \
        ((short *)(P))++;                                               \
    }                                                                   \
    if((int)(C) & 1) {                                                  \
        poke_b##FMODE((P),(V));                                         \
    }                                                                   \
} while(0)
#endif  /* __INLINE_B_REPFILL__ */

#ifndef __INLINE_W_REPFILL__
#define __INLINE_W_REPFILL__(P,V,C,FMODE) do {                          \
    if((int)(C) > 1) {                                                  \
        if((int)(P) & 2) {                                              \
            poke_w##FMODE((P),(V));                                     \
            ((short *)(P))++;                                           \
            ((int)(C))--;                                               \
        }                                                               \
        if((int)(C) > 1) {                                              \
            int _cc_ = (unsigned int)(C) >> 1;                          \
            rowfill_l##FMODE((P),(V),_cc_);                             \
        }                                                               \
    }                                                                   \
    if((int)(C) & 1) {                                                  \
        poke_w##FMODE((P),(V));                                         \
    }                                                                   \
} while(0)
#endif

#ifndef __INLINE_L_REPFILL__
#define __INLINE_L_REPFILL__(P,V,C,FMODE) do {                          \
    rowfill_l##FMODE((P),(V),(C));                                      \
} while(0)
#endif

/*
 * repfill_<SIZE>[_<FAR>][_<OPER>](pointer,value,count)
 */
#define repfill_b(p,v,c)        __INLINE_B_REPFILL__(p,v,c,)
#define repfill_w(p,v,c)        __INLINE_W_REPFILL__(p,v,c,)
#define repfill_l(p,v,c)        __INLINE_L_REPFILL__(p,v,c,)

#define repfill_b_xor(p,v,c)    __INLINE_B_REPFILL__(p,v,c,_xor)
#define repfill_w_xor(p,v,c)    __INLINE_W_REPFILL__(p,v,c,_xor)
#define repfill_l_xor(p,v,c)    __INLINE_L_REPFILL__(p,v,c,_xor)

#define repfill_b_or(p,v,c)     __INLINE_B_REPFILL__(p,v,c,_or)
#define repfill_w_or(p,v,c)     __INLINE_W_REPFILL__(p,v,c,_or)
#define repfill_l_or(p,v,c)     __INLINE_L_REPFILL__(p,v,c,_or)

#define repfill_b_and(p,v,c)    __INLINE_B_REPFILL__(p,v,c,_and)
#define repfill_w_and(p,v,c)    __INLINE_W_REPFILL__(p,v,c,_and)
#define repfill_l_and(p,v,c)    __INLINE_L_REPFILL__(p,v,c,_and)

#define repfill_b_f(p,v,c)      __INLINE_B_REPFILL__(p,v,c,_f)
#define repfill_w_f(p,v,c)      __INLINE_W_REPFILL__(p,v,c,_f)
#define repfill_l_f(p,v,c)      __INLINE_L_REPFILL__(p,v,c,_f)

#define repfill_b_f_xor(p,v,c)  __INLINE_B_REPFILL__(p,v,c,_f_xor)
#define repfill_w_f_xor(p,v,c)  __INLINE_W_REPFILL__(p,v,c,_f_xor)
#define repfill_l_f_xor(p,v,c)  __INLINE_L_REPFILL__(p,v,c,_f_xor)

#define repfill_b_f_or(p,v,c)   __INLINE_B_REPFILL__(p,v,c,_f_or)
#define repfill_w_f_or(p,v,c)   __INLINE_W_REPFILL__(p,v,c,_f_or)
#define repfill_l_f_or(p,v,c)   __INLINE_L_REPFILL__(p,v,c,_f_or)

#define repfill_b_f_and(p,v,c)  __INLINE_B_REPFILL__(p,v,c,_f_and)
#define repfill_w_f_and(p,v,c)  __INLINE_W_REPFILL__(p,v,c,_f_and)
#define repfill_l_f_and(p,v,c)  __INLINE_L_REPFILL__(p,v,c,_f_and)

/*
 * Another set of optimized fills which also do the replication.
 */
#ifndef freplicate_b2l
#define freplicate_b2l(V)       replicate_b2l(V)
#endif
#ifndef freplicate_w2l
#define freplicate_w2l(V)       replicate_w2l(V)
#endif
#ifndef freplicate_l2l
#define freplicate_l2l(V)       (V)
#endif

/*
 * optfill_<SIZE>[_<FAR>][_<OPER>](pointer,value,count)
 */
#define optfill_b(p,v,c)        repfill_b(p,freplicate_b2l(v),c)
#define optfill_w(p,v,c)        repfill_w(p,freplicate_w2l(v),c)
#define optfill_l(p,v,c)        repfill_l(p,freplicate_l2l(v),c)

#define optfill_b_xor(p,v,c)    repfill_b_xor(p,freplicate_b2l(v),c)
#define optfill_w_xor(p,v,c)    repfill_w_xor(p,freplicate_w2l(v),c)
#define optfill_l_xor(p,v,c)    repfill_l_xor(p,freplicate_l2l(v),c)

#define optfill_b_or(p,v,c)     repfill_b_or(p,freplicate_b2l(v),c)
#define optfill_w_or(p,v,c)     repfill_w_or(p,freplicate_w2l(v),c)
#define optfill_l_or(p,v,c)     repfill_l_or(p,freplicate_l2l(v),c)

#define optfill_b_and(p,v,c)    repfill_b_and(p,freplicate_b2l(v),c)
#define optfill_w_and(p,v,c)    repfill_w_and(p,freplicate_w2l(v),c)
#define optfill_l_and(p,v,c)    repfill_l_and(p,freplicate_l2l(v),c)

#define optfill_b_f(p,v,c)      repfill_b_f(p,freplicate_b2l(v),c)
#define optfill_w_f(p,v,c)      repfill_w_f(p,freplicate_w2l(v),c)
#define optfill_l_f(p,v,c)      repfill_l_f(p,freplicate_l2l(v),c)

#define optfill_b_f_xor(p,v,c)  repfill_b_f_xor(p,freplicate_b2l(v),c)
#define optfill_w_f_xor(p,v,c)  repfill_w_f_xor(p,freplicate_w2l(v),c)
#define optfill_l_f_xor(p,v,c)  repfill_l_f_xor(p,freplicate_l2l(v),c)

#define optfill_b_f_or(p,v,c)   repfill_b_f_or(p,freplicate_b2l(v),c)
#define optfill_w_f_or(p,v,c)   repfill_w_f_or(p,freplicate_w2l(v),c)
#define optfill_l_f_or(p,v,c)   repfill_l_f_or(p,freplicate_l2l(v),c)

#define optfill_b_f_and(p,v,c)  repfill_b_f_and(p,freplicate_b2l(v),c)
#define optfill_w_f_and(p,v,c)  repfill_w_f_and(p,freplicate_w2l(v),c)
#define optfill_l_f_and(p,v,c)  repfill_l_f_and(p,freplicate_l2l(v),c)

/*
 * A set of optimized fills which preserves the address and count arguments
 * and performs small constant sized fills unrolled if the compiler supports
 * this (e.g. GCC)
 */
#ifdef  __GNUC__
#ifdef  __i386__
#define __INLINE_MEMFILL__(P,V,C,SIZE,TYPE,FMODE) ({                    \
    char *_FP = (char *)(P);                                            \
    long  _FV = freplicate_##SIZE##2l((TYPE)(V));                       \
    if((__builtin_constant_p((C))) &&                                   \
       (((unsigned)(C) * sizeof(TYPE)) <= 32)) {                        \
        if(((C) * sizeof(TYPE)) >= 4)  poke_l##FMODE(&_FP[0], _FV);     \
        if(((C) * sizeof(TYPE)) >= 8)  poke_l##FMODE(&_FP[4], _FV);     \
        if(((C) * sizeof(TYPE)) >= 12) poke_l##FMODE(&_FP[8], _FV);     \
        if(((C) * sizeof(TYPE)) >= 16) poke_l##FMODE(&_FP[12],_FV);     \
        if(((C) * sizeof(TYPE)) >= 20) poke_l##FMODE(&_FP[16],_FV);     \
        if(((C) * sizeof(TYPE)) >= 24) poke_l##FMODE(&_FP[20],_FV);     \
        if(((C) * sizeof(TYPE)) >= 28) poke_l##FMODE(&_FP[24],_FV);     \
        if(((C) * sizeof(TYPE)) >= 32) poke_l##FMODE(&_FP[28],_FV);     \
        if(((C) * sizeof(TYPE)) & 2) {                                  \
            poke_w##FMODE(&_FP[((C) * sizeof(TYPE)) & (~3)],_FV);       \
        }                                                               \
        if(((C) * sizeof(TYPE)) & 1) {                                  \
            poke_b##FMODE(&_FP[((C) * sizeof(TYPE)) & (~1)],_FV);       \
        }                                                               \
    }                                                                   \
    else {                                                              \
        int _FC = (int)(C);                                             \
        repfill_##SIZE##FMODE(_FP,_FV,_FC);                             \
    }                                                                   \
})
#endif  /* __i386__ */
#endif  /* __GNUC__ */

#ifdef  __TURBOC__
#define __INLINE_MEMFILL__(P,V,C,SIZE,TYPE,FMODE) do {                  \
    void     far *_PTR = (void far *)(P);                               \
    register int  _VAL = (int)(V);                                      \
    register int  _CNT = (int)(C);                                      \
    rowfill_##SIZE##FMODE(_PTR,_VAL,_CNT);                              \
} while(0)
#endif

#ifndef __INLINE_MEMFILL__
#define __INLINE_MEMFILL__(P,V,C,SIZE,TYPE,FMODE) do {                  \
    register void *_FP = (void *)(P);                                   \
    register long  _FV = freplicate_##SIZE##2l((TYPE)(V));              \
    register int   _FC = (int)(C);                                      \
    repfill_##SIZE##FMODE(_FP,_FV,_FC);                                 \
} while(0)
#endif

/*
 * memfill_<SIZE>[_<FAR>][_<OPER>](pointer,value,count)
 */
#define memfill_b(p,v,c)        __INLINE_MEMFILL__(p,v,c,b,char,)
#define memfill_w(p,v,c)        __INLINE_MEMFILL__(p,v,c,w,short,)
#define memfill_l(p,v,c)        __INLINE_MEMFILL__(p,v,c,l,long,)

#define memfill_b_xor(p,v,c)    __INLINE_MEMFILL__(p,v,c,b,char,_xor)
#define memfill_w_xor(p,v,c)    __INLINE_MEMFILL__(p,v,c,w,short,_xor)
#define memfill_l_xor(p,v,c)    __INLINE_MEMFILL__(p,v,c,l,long,_xor)

#define memfill_b_or(p,v,c)     __INLINE_MEMFILL__(p,v,c,b,char,_or)
#define memfill_w_or(p,v,c)     __INLINE_MEMFILL__(p,v,c,w,short,_or)
#define memfill_l_or(p,v,c)     __INLINE_MEMFILL__(p,v,c,l,long,_or)

#define memfill_b_and(p,v,c)    __INLINE_MEMFILL__(p,v,c,b,char,_and)
#define memfill_w_and(p,v,c)    __INLINE_MEMFILL__(p,v,c,w,short,_and)
#define memfill_l_and(p,v,c)    __INLINE_MEMFILL__(p,v,c,l,long,_and)

#define memfill_b_f(p,v,c)      __INLINE_MEMFILL__(p,v,c,b,char,_f)
#define memfill_w_f(p,v,c)      __INLINE_MEMFILL__(p,v,c,w,short,_f)
#define memfill_l_f(p,v,c)      __INLINE_MEMFILL__(p,v,c,l,long,_f)

#define memfill_b_f_xor(p,v,c)  __INLINE_MEMFILL__(p,v,c,b,char,_f_xor)
#define memfill_w_f_xor(p,v,c)  __INLINE_MEMFILL__(p,v,c,w,short,_f_xor)
#define memfill_l_f_xor(p,v,c)  __INLINE_MEMFILL__(p,v,c,l,long,_f_xor)

#define memfill_b_f_or(p,v,c)   __INLINE_MEMFILL__(p,v,c,b,char,_f_or)
#define memfill_w_f_or(p,v,c)   __INLINE_MEMFILL__(p,v,c,w,short,_f_or)
#define memfill_l_f_or(p,v,c)   __INLINE_MEMFILL__(p,v,c,l,long,_f_or)

#define memfill_b_f_and(p,v,c)  __INLINE_MEMFILL__(p,v,c,b,char,_f_and)
#define memfill_w_f_and(p,v,c)  __INLINE_MEMFILL__(p,v,c,w,short,_f_and)
#define memfill_l_f_and(p,v,c)  __INLINE_MEMFILL__(p,v,c,l,long,_f_and)

/*
 * stuff to clear arrays, structures
 */
#ifdef  __TURBOC__
#define memzero(p,s)            memfill_w((p),0,((s) >> 1))
#endif
#ifndef memzero
#define memzero(p,s)            memfill_l((p),0,((s) >> 2))
#endif
#define sttzero(p)              memzero((p),sizeof(*(p)))

#ifdef  __TURBOC__
/*
 * These are not really here!
 */
#undef  rowfill_l
#undef  rowfill_l_xor
#undef  rowfill_l_or
#undef  rowfill_l_and
#undef  rowfill_l_f
#undef  rowfill_l_f_xor
#undef  rowfill_l_f_or
#undef  rowfill_l_f_and

#undef  colfill_l
#undef  colfill_l_xor
#undef  colfill_l_or
#undef  colfill_l_and
#undef  colfill_l_f
#undef  colfill_l_f_xor
#undef  colfill_l_f_or
#undef  colfill_l_f_and

#undef  repfill_l
#undef  repfill_l_xor
#undef  repfill_l_or
#undef  repfill_l_and
#undef  repfill_l_f
#undef  repfill_l_f_xor
#undef  repfill_l_f_or
#undef  repfill_l_f_and

#undef  optfill_l
#undef  optfill_l_xor
#undef  optfill_l_or
#undef  optfill_l_and
#undef  optfill_l_f
#undef  optfill_l_f_xor
#undef  optfill_l_f_or
#undef  optfill_l_f_and

#undef  memfill_l
#undef  memfill_l_xor
#undef  memfill_l_or
#undef  memfill_l_and
#undef  memfill_l_f
#undef  memfill_l_f_xor
#undef  memfill_l_f_or
#undef  memfill_l_f_and
#endif

#endif  /* whole file */

