/**
 ** ARITH.H ---- some common integer arithmetic macros/inline functions
 **              Special GNU-C handling
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

/*
 * [i|u]scale(X,N,D)
 * scale an integer with long intermediate result but without using long
 * arithmetic all the way
 */

#define irscale(X,N,D) ({                                           \
        register int _SclVal_ = iscale(((int)(X) << 1),N,D);        \
        (_SclVal_ + (_SclVal_ >> (bitsof(int) - 1)) + 1) >> 1;      \
})

/* ================================================================ */
/* ==                     80386 FAMILY                           == */
/* ================================================================ */
#ifdef __i386__
/*
 * replicate_<FROM>2<TO>(byte_or_word_value)
 * copy the lower byte(s) of a byte or word into the upper byte(s)
 */

#define replicate_b2w(BYTE) (__builtin_constant_p(BYTE) ?       \
        (long)(0x00000101UL * (GR_int8u)(BYTE)) :               \
        ({                                                      \
        register long _repvalue;                                \
        __asm__(                                                \
              " movzbl  %%al,%%eax                       \n"    \
              " movb    %%al,%%ah"                              \
                : "=a" (_repvalue)                              \
                : "0"  ((char)(BYTE))                           \
        );                                                      \
        _repvalue;                                              \
        })                                                      \
)

#define replicate_w2l(WORD) (__builtin_constant_p(WORD) ?       \
        (long)(0x00010001UL * (GR_int16u)(WORD)) :              \
        ({                                                      \
          register long  _repvalue;                             \
          __asm__(                                              \
              " movw    %1,%%ax                          \n"    \
              " shll    $16,%%eax                        \n"    \
              " movw    %1,%%ax                          \n"    \
                : "=&a" (_repvalue)                             \
                : "q"  ((GR_int16u)(WORD))                      \
          );                                                    \
        _repvalue;                                              \
        })                                                      \
)

#define replicate_b2l(BYTE) (__builtin_constant_p(BYTE) ?       \
        (long)(0x01010101UL * (GR_int8u)(BYTE)) :               \
        ({                                                      \
        register long  _repvalue;                               \
        __asm__(                                                \
              " movb    %1,%%al                          \n"    \
              " movb    %1,%%ah                          \n"    \
              " shll    $16,%0                           \n"    \
              " movb    %1,%%al                          \n"    \
              " movb    %1,%%ah                            "    \
                : "=&a" (_repvalue)                             \
                : "q"  ((char)(BYTE))                           \
        );                                                      \
        _repvalue;                                              \
        })                                                      \
)

#endif /* __i386__ */
