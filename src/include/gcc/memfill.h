/**
 ** MEMFILL.H ---- inline assembly memory fill macros -- GNU-C code
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

/* ================================================================ */
/* ==                     80386 FAMILY                           == */
/* ================================================================ */
#ifdef __i386__

#ifndef MOV_INS
#include "gcc/asmsupp.h"
#endif

/*
 * Unoptimized row and column fills
 */
#define __INLINE_386_ROWFILL__(P,V,C,SIZE,TYPE) ({                      \
        __asm__ volatile(                                               \
                " cld          \n"                                      \
                " rep          \n"                                      \
                " stos"#SIZE                                            \
                  : "=D" ((void *)(P)), "=c" ((int)(C))                 \
                  : "0"  ((void *)(P)), "1"  ((int)(C)),                \
                    "a"  ((TYPE)(V))                                    \
        );                                                              \
})
#define __INLINE_STD_ROWFILL__(P,V,C,FMODE,SIZE,TYPE)                   \
        __INLINE_386_ROWFILL__(P,V,C,SIZE,TYPE)

#define __INLINE_386_COLFILL__(P,V,C,SKIP,INS,SIZE,TYPE,SEL) ({         \
        if(__builtin_constant_p(SKIP) && ((SKIP) == 1))                 \
          __asm__ volatile(""                                           \
                "    incl    %1                           \n"           \
                "    shrl    $1,%1                        \n"           \
                "    jnc     1f                           \n"           \
                "    jmp     0f                           \n"           \
                "    .align 4,0x90                        \n"           \
                "0:  "#INS""#SIZE"   %4,"#SEL"(%0)        \n"           \
                "    incl    %0                           \n"           \
                "1:  "#INS""#SIZE"   %4,"#SEL"(%0)        \n"           \
                "    incl    %0                           \n"           \
                "    decl    %1                           \n"           \
                "    jne     0b"                                        \
                : "=r" ((void *)(P)), "=r" ((int)(C))                   \
                : "0"  ((void *)(P)), "1"  ((int)(C)),                  \
                  "qn" ((TYPE)(V))                                      \
          );                                                            \
        else                                                            \
        if(__builtin_constant_p(SKIP) && ((SKIP) == 2))                 \
          __asm__ volatile(""                                           \
                "    incl    %1                           \n"           \
                "    shrl    $1,%1                        \n"           \
                "    jnc     1f                           \n"           \
                "    jmp     0f                           \n"           \
                "    .align 4,0x90                        \n"           \
                "0:  "#INS""#SIZE"   %4,"#SEL"(%0)        \n"           \
                "    leal    2(%0),%0                     \n"           \
                "1:  "#INS""#SIZE"   %4,"#SEL"(%0)        \n"           \
                "    leal    2(%0),%0                     \n"           \
                "    decl    %1                           \n"           \
                "    jne     0b"                                        \
                : "=r" ((void *)(P)), "=r" ((int)(C))                   \
                : "0"  ((void *)(P)), "1"  ((int)(C)),                  \
                  "qn" ((TYPE)(V))                                      \
          );                                                            \
        else                                                            \
        if(__builtin_constant_p(SKIP) && ((SKIP) == 4))                 \
          __asm__ volatile(""                                           \
                "    incl    %1                           \n"           \
                "    shrl    $1,%1                        \n"           \
                "    jnc     1f                           \n"           \
                "    jmp     0f                           \n"           \
                "    .align 4,0x90                        \n"           \
                "0:  "#INS""#SIZE"   %4,"#SEL"(%0)        \n"           \
                "    leal    4(%0),%0                     \n"           \
                "1:  "#INS""#SIZE"   %4,"#SEL"(%0)        \n"           \
                "    leal    4(%0),%0                     \n"           \
                "    decl    %1                           \n"           \
                "    jne     0b"                                        \
                : "=r" ((void *)(P)), "=r" ((int)(C))                   \
                : "0"  ((void *)(P)), "1"  ((int)(C)),                  \
                  "qn" ((TYPE)(V))                                      \
          );                                                            \
        else                                                            \
          __asm__ volatile(""                                           \
                "    incl    %1                           \n"           \
                "    shrl    $1,%1                        \n"           \
                "    jnc     1f                           \n"           \
                "    jmp     0f                           \n"           \
                "    .align 4,0x90                        \n"           \
                "0:  "#INS""#SIZE"   %5,"#SEL"(%0)        \n"           \
                "    addl    %4,%0                        \n"           \
                "1:  "#INS""#SIZE"   %5,"#SEL"(%0)        \n"           \
                "    addl    %4,%0                        \n"           \
                "    decl    %1                           \n"           \
                "    jne     0b"                                        \
                : "=r" ((void *)(P)), "=r" ((int)(C))                   \
                : "0"  ((void *)(P)), "1"  ((int)(C)),                  \
                  "ng" ((int)(SKIP)), "qn" ((TYPE)(V))                  \
          );                                                            \
})
#define __INLINE_STD_COLFILL__(P,V,C,SKIP,FMODE,INS,SIZE,TYPE)          \
        __INLINE_386_COLFILL__(P,V,C,SKIP,INS,SIZE,TYPE,)
#ifdef  I386_GCC_FAR_MEMORY
#define __INLINE_FAR_COLFILL__(P,V,C,SKIP,FMODE,INS,SIZE,TYPE)          \
        __INLINE_386_COLFILL__(P,V,C,SKIP,INS,SIZE,TYPE,%%fs:)
#else   /* I386_GCC_FAR_MEMORY */
#define __INLINE_FAR_ROWFILL__(P,V,C,FMODE,SIZE,TYPE)                   \
        __INLINE_STD_ROWFILL__(P,V,C,FMODE,SIZE,TYPE)
#endif  /* I386_GCC_FAR_MEMORY */


#define __INLINE_386_REPFILL24__(p,c,b,INS,SEG) do {               \
  __asm__ volatile (                "\n"                           \
    "      testl  $1,%0              \n"                           \
    "      je     1f                 \n"                           \
    "    "#INS"b  %%dl,"#SEG"(%0)    \n"                           \
    "      incl   %0                 \n"                           \
    "      decl   %1                 \n"                           \
    "      shll   $8,%%edx           \n"                           \
    "      movb   %%dh,%%dl          \n"                           \
    "      rorl   $16,%%edx          \n"                           \
    "1:    testl  $2,%0              \n"                           \
    "      je     2f                 \n"                           \
    "    "#INS"w  %%dx,"#SEG"(%0)    \n"                           \
    "      leal   2(%0),%0           \n"                           \
    "      leal   -2(%1),%1          \n"                           \
    "      roll   $16,%%edx          \n"                           \
    "      movb   %%dl,%%dh          \n"                           \
    "      shrl   $8,%%edx           \n"                           \
    "2:    cmpl   $4,%1              \n"                           \
    "      jb     7f                 \n"                           \
    "      movl   %%edx,%%eax        \n"                           \
    "      shl    $8,%%eax           \n"                           \
    "      shldl  $8,%%eax,%%edx     \n"                           \
    "      movl   %%edx,%%ecx        \n"                           \
    "      shl    $8,%%ecx           \n"                           \
    "      movb   %%ah,%%al          \n"                           \
    "      rorl   $8,%%eax           \n"                           \
    "      movb   %%ah,%%cl          \n"                           \
    /* now we have : eax=La, ecx=Lb, edx=Lc */                     \
    "      subl   $12,%1             \n"                           \
    "      jb     4f                 \n"                           \
    "      jmp    3f                 \n"                           \
    "     .align  4,0x90             \n"                           \
    "3:  "#INS"l  %%eax,"#SEG"(%0)   \n"                           \
    "      leal   4(%0),%0           \n"                           \
    "    "#INS"l  %%ecx,"#SEG"(%0)   \n"                           \
    "      leal   4(%0),%0           \n"                           \
    "    "#INS"l  %%edx,"#SEG"(%0)   \n"                           \
    "      leal   4(%0),%0           \n"                           \
    "      subl   $12,%1             \n"                           \
    "      jnb    3b                 \n"                           \
    /* 0 .. 11 bytes left */                                       \
    "4:    leal   12(%1),%1          \n"                           \
    "      cmpl   $4,%1              \n"                           \
    "      jnb    5f                 \n"                           \
    "      movl   %%eax,%%edx        \n"                           \
    "      jmp    7f                 \n"                           \
    "5:  "#INS"l  %%eax,"#SEG"(%0)   \n"                           \
    "      leal  -4(%1),%1           \n"                           \
    "      leal   4(%0),%0           \n"                           \
    "      cmpl   $4,%1              \n"                           \
    "      jnb    6f                 \n"                           \
    "      movl   %%ecx,%%edx        \n"                           \
    "      jmp    7f                 \n"                           \
    "6:  "#INS"l  %%ecx,"#SEG"(%0)   \n"                           \
    "      leal   4(%0),%0           \n"                           \
    /* 0 .. 3 bytes left */                                        \
    "7:    testl  $2,%1              \n"                           \
    "      je     8f                 \n"                           \
    "    "#INS"w  %%dx,"#SEG"(%0)    \n"                           \
    "      leal   2(%0),%0           \n"                           \
    "      shrl   $16,%%edx          \n"                           \
    "8:    testl  $1,%1              \n"                           \
    "      je     9f                 \n"                           \
    "    "#INS"b  %%dl,"#SEG"(%0)    \n"                           \
    "      incl   %0                 \n"                           \
    "9:                              \n"                           \
          : "=r" ((void *)(p)), "=r" ((int)(b))                    \
          : "d"  ((int)(c)), "0"  ((void *)(p)), "1"  ((int)(b))   \
          : "ax", "cx", "dx"                                       \
  );                                                               \
} while (0)

#define __INLINE_24_REPFILL__(P,C,B,FMODE,INS) \
        __INLINE_386_REPFILL24__(P,C,B,INS,)

#define __INLINE_24_FAR_REPFILL__(P,C,B,FMODE,INS) \
        __INLINE_386_REPFILL24__(P,C,B,INS,%%fs:)

#define GRX_HAVE_FAST_REPFILL24

#endif /* __i386__ */

