/**
 ** HIGHLOW.H ---- combining two BYTES into one WORD
 **
 ** Copyright (c) 1997 Hartmut Schirmer
 **
 **/

/* combine two bytes into one word: generic version */
#define _highlow(hi,lo) ((ushort)( (((ushort)(hi))<<8) | ((uchar)(lo)) ))

#if defined(__GNUC__) && defined(__i386__)
/* combine two bytes into one word: optimized i386 version */
#define highlow(hi,lo) ({                                           \
    register ushort _res_;                                          \
    if(__builtin_constant_p((hi))&& !__builtin_constant_p((lo)))    \
      __asm__ volatile( "  movb %1,%%ah"                            \
          : "=a" (_res_) : "n" ((uchar)(hi)), "0" ((uchar)(lo)) );  \
    else                                                            \
        _res_ = _highlow((hi),(lo));                                \
    _res_;                                                          \
})
#else
#define highlow(hi,lo) _highlow((hi),(lo))
#endif

