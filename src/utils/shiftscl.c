/**
 ** SHIFTSCL.C ---- shift and copy an array (scanline)
 **                 for 1bpp and 4bpp frame driver blit operations
 **
 ** Copyright (c) 1998 Hartmut Schirmer
 **
 **/

#include "libgrx.h"
#include "highlow.h"

#ifdef __TURBOC__
#pragma inline
#endif

void _GR_shift_scanline(GR_int8u far **dst,
                        GR_int8u far **src,
                        int ws, int shift, int planes) {
  int plane;

  GRX_ENTER();
  if (shift <= 0) {
    shift = -shift;
    for (plane = 0; plane < planes; ++plane) {
      GR_int8u far *s = *(src++) + ws;
      GR_int8u far *d = *(dst++) + ws;
#     if defined(__GNUC__) && defined(__i386__)
        int w = ws;
        /* sad but true: the x86 bytesex forces this inefficient code :( */
        asm volatile ("\n"
          "   movb    (%3),%%ch    \n"
          "   jmp     1f           \n"
          "   .align  4,0x90       \n"
          "1: decl    %3           \n"
          "   movb    %%ch,%%al    \n"
          "   movb    (%3),%%ah    \n"
          "   movb    %%ah,%%ch    \n"
          "   shrl    %%cl,%%eax   \n"
          "   movb    %%al,(%4)    \n"
          "   decl    %4           \n"
          "   decl    %5           \n"
          "   jne     1b           \n"
          "   shrb    %%cl,%%ch    \n"
          "   movb    %%ch,(%4)      "
          : "=r" ((void *)s), "=r" ((void *)d), "=r" ((int)w)
          : "0" ((void *)s), "1" ((void *)d), "2" ((int)w), "c" ((int)shift)
          : "ax", "cx"
        );
#     elif defined(__TURBOC__)
        asm     push  ds           ;
        asm     mov   bx, ws       ;
        asm     mov   cl, shift    ;
        asm     les   di, d        ;
        asm     lds   si, s        ;
        asm     mov   ch,[si]      ;
        asm     std                ;
         looprv:
        asm     dec   si           ;
        asm     mov   al, ch       ;
        asm     mov   ah, [si]     ;
        asm     mov   ch, ah       ;
        asm     shr   ax, cl       ;
        asm     stosb              ;
        asm     dec   bx           ;
        asm     jnz   short looprv ;
        asm     mov   al, ch       ;
        asm     shr   al, cl       ;
        asm     stosb              ;
        asm     pop   ds           ;
#     else
        int w = ws;
        do {
          --s;
          *(d--) = highlowP(s)>>shift;
        } while (--w);
        *d = *s >> shift;
#     endif
    }
  } else {
    shift = 8-shift;
    for (plane = 0; plane < planes; ++plane) {
      GR_int8u far *s = *(src++);
      GR_int8u far *d = *(dst++);
#     if defined(__GNUC__) && defined(__i386__)
        int w = ws;
        asm volatile ("\n"
          "   movb    (%3),%%ch    \n"
          "   jmp     1f           \n"
          "   .align  4,0x90       \n"
          "1: incl    %3           \n"
          "   movb    %%ch,%%ah    \n"
          "   movb    (%3),%%al    \n"
          "   movb    %%al,%%ch    \n"
          "   shrl    %%cl,%%eax   \n"
          "   movb    %%al,(%4)    \n"
          "   incl    %4           \n"
          "   decl    %5           \n"
          "   jne     1b             "
          : "=r" ((void *)s), "=r" ((void *)d), "=r" ((int)w)
          : "0" ((void *)s), "1" ((void *)d), "2" ((int)w), "c" ((int)shift)
          : "ax", "cx"
        );
#     elif defined(__TURBOC__)
        asm     push  ds           ;
        asm     mov   bx, ws       ;
        asm     mov   cl, shift    ;
        asm     les   di, d        ;
        asm     lds   si, s        ;
        asm     mov   ch,[si]      ;
        asm     cld                ;
         loopfw:
        asm     inc   si           ;
        asm     mov   ah, ch       ;
        asm     mov   al, [si]     ;
        asm     mov   ch, al       ;
        asm     shr   ax, cl       ;
        asm     stosb              ;
        asm     dec   bx           ;
        asm     jnz   short loopfw ;
        asm     pop   ds           ;
#     else
        int w = ws;
        do {
          *(d++) = highlowP(s)>>shift /* sh */;
          s++;
        } while (--w);
#     endif
    }
  }
  GRX_LEAVE();
}

