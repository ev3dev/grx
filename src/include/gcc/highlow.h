/**
 ** HIGHLOW.H ---- combining two BYTES into one WORD -- GNU-C special
 **
 ** Copyright (c) 1997 Hartmut Schirmer
 **
 **/

/* ================================================================ */
/* ==                     80386 FAMILY                           == */
/* ================================================================ */
#ifdef __i386__

/* combine two bytes into one word: optimized i386 version */
#define highlow(hi,lo) ({                                           \
    register GR_int32u _res_;                                       \
    if(__builtin_constant_p((hi))&& __builtin_constant_p((lo)))     \
        _res_ = __highlow__((hi),(lo));                             \
    else                                                            \
    if(__builtin_constant_p((hi)))                                  \
      __asm__ volatile( "  movb %1,%%al"                            \
          : "=&a" (_res_)                                           \
          : "qnm" ((GR_int8u)(lo)), "0" (((int)(hi))<<8) );         \
    else                                                            \
      __asm__ volatile( "  movb %1,%%ah"                            \
          : "=&a" (_res_)                                           \
          : "qnm" ((GR_int8u)(hi)), "0" ((int)(lo)) );              \
    _res_;                                                          \
})


/* high from *p / low from *(p+1) */
#define highlowP(p) ({                          \
    register GR_int32u _res_;                   \
    __asm__ volatile( "xorl   %0,%0      \n\t"  \
                      "movw   (%1),%%ax  \n\t"  \
                      "exch   %%al,%%ah      "  \
        : "=&a" (_res_)                         \
        : "r" ((GR_int8u *)(p))                 \
    );                                          \
    _res_;                                      \
})

#endif /* __i386__ */
