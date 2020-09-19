/*
 * mempeek.h ---- (far) memory read/write operations
 *                GNU-C special assembler code
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
 */

/* ================================================================ */
/* ==                     80386 FAMILY                           == */
/* ================================================================ */
#ifdef __i386__

#define __INLINE_386_PEEK24__(P, SEL)                     \
    ({                                                    \
        register GR_int32u _pix_;                         \
        __asm__ volatile(                                 \
            "\n"                                          \
            "    xorl    %%eax,%%eax                  \n" \
            "    movb   " SEL                             \
            "2(%1),%%ah             \n"                   \
            "    sall    $8,%%eax                     \n" \
            "    movw    " SEL "(%1),%%ax               " \
            : "=&a"((GR_int32u)_pix_)                     \
            : "r"((void *)(P)));                          \
        (GrxColor) _pix_;                                 \
    })

#define __INLINE_386_POKE24__(P, C, INS, SEL)               \
    do {                                                    \
        int _dummy_;                                        \
        __asm__ volatile(                                   \
            "\n"                                            \
            "    " #INS "w %%ax," SEL                       \
            "(%2)             \n"                           \
            "    shrl    $8,%%eax                     \n"   \
            "    " #INS "b %%ah," SEL "2(%2)            \n" \
            : "=a"(_dummy_)                                 \
            : "0"(C), "r"((void *)(P)));                    \
    } while (0)

#define __INLINE_24_PEEK__(p) __INLINE_386_PEEK24__(p, )

#define __INLINE_24_FAR_PEEK__(p) (peek_l_f(p) & 0xffffff)
#define PEEK_24_F_READS_ONE_MORE

#define __INLINE_24_POKE__(p, c, op, INS)     __INLINE_386_POKE24__(p, c, INS, )
#define __INLINE_24_FAR_POKE__(p, c, op, INS) __INLINE_386_POKE24__(p, c, INS, )

#endif /* __i386__ */
