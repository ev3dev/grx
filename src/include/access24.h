/**
 ** ACCESS24.C ---- 16M color (24bit) access macros
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 **/

#ifndef __ACCESS24_H_INCLUDED__
#define __ACCESS24_H_INCLUDED__

#include "mempeek.h"

/* ------------------------- LITTLE ENDIAN:  low byte / mid byte / high byte */
#if BYTE_ORDER==LITTLE_ENDIAN

#define C24BYTE(color,idx)  (((uchar *)(&color))[idx])
#define C24WORD(color)      ((ushort)(color))
#define C24THIRD(color)     ((uchar)((color)>>16))

#define __INTERN_24_PEEK__(p,f)                                             \
          ( (ushort)peek_w##f(p) + ((ulong)(uchar)peek_b##f(&(p)[2]) << 16) )

#define __INTERN_24_POKE__(p,c,f) do {                                      \
                poke_w##f((p),(c));                                         \
                poke_b##f(&(p)[2],(c)>>16);                                 \
        } while (0)


#else

/* ---------------------------- BIG ENDIAN:  high byte / mid byte / low byte */
#define C24BYTE(color,idx)  (((uchar *)(&color))[1+(idx)])
#define C24WORD(color)      ((ushort)((color)>>8))
#define C24THIRD(color)     ((uchar)(color))

#define __INTERN_24_PEEK__(p,f)                                             \
          ( ((ulong)(ushort)peek_w##f(p) << 8) + (uchar)peek_b##f(&(p)[2]) )

#define __INTERN_24_POKE__(p,c,f) do {                                      \
                register ulong cc = (ulong)(c);                             \
                poke_w##f((p),(ushort)((cc)>>8));                           \
                poke_b##f(&(p)[2],(uchar)(cc));                             \
        } while (0)

#endif

#define poke_24_f_xor(p,c)      __INTERN_24_POKE__(p,c,_f_xor)
#define poke_24_f_or(p,c)       __INTERN_24_POKE__(p,c,_f_or)
#define poke_24_f_and(p,c)      __INTERN_24_POKE__(p,c,_f_and)
#define poke_24_f(p,c)          __INTERN_24_POKE__(p,c,_f)

#define poke_24_xor(p,c)        __INTERN_24_POKE__(p,c,_xor)
#define poke_24_or(p,c)         __INTERN_24_POKE__(p,c,_or)
#define poke_24_and(p,c)        __INTERN_24_POKE__(p,c,_and)
#define poke_24(p,c)            __INTERN_24_POKE__(p,c,)

#define peek_24_f(p)            __INTERN_24_PEEK__(p,_f)
#define peek_24(p)              __INTERN_24_PEEK__(p,)


/* --------------------------------------------------- memory fill functions */
#define ptrdiff(a,b) ( ((char *)(a)) - ((char *)(b)) )
#define ptrinc(P,SKIP) do { ((char *)(P)) += (SKIP); } while (0)

/* converting from 24bit colors -> 32bit long words can be
** done by misaligned long writes                          */
#if defined(__TURBOC__) || (defined(__GNUC__) && defined(__i386__))
#define MISALIGNED_ACCESS
#endif

#ifdef MISALIGNED_ACCESS
/* create an array holding the correct long data     */
/* ulong five[5]; five[1]..five[3] hold aligned data */
#define FillFive(p,col,bs) do {                       \
          register uchar *lp = ((uchar *)(p)) + (bs); \
          *((ulong *)(lp  ))  = (col);                \
          *((ulong *)(lp+3))  = (col);                \
          *((ulong *)(lp+6))  = (col);                \
          *((ulong *)(lp+9))  = (col);                \
          *((ulong *)(lp+12)) = (col);                \
  } while (0)

#else
/* create an array holding the correct long data     */
/* ulong five[5]; five[1]..five[3] hold aligned data */
#define FillFive(p,col,bs) do {                   \
          register ulong   c = (col);                 \
          register uchar *lp = ((uchar *)(p)) + (bs); \
          poke_24(lp,c);  ptrinc(lp,3);               \
          poke_24(lp,c);  ptrinc(lp,3);               \
          poke_24(lp,c);  ptrinc(lp,3);               \
          poke_24(lp,c);  ptrinc(lp,3);               \
          poke_24(lp,c);                              \
  } while (0)

#endif

#if defined(__GNUC__) && defined(__i386__)

#define __FILL_24_12__(p,f,w,INS,SEG,OPR) do {                       \
  __asm__ volatile (                                                 \
          "   .align 4,0x90                  \n\t"                   \
          "1: "#INS"l %4,"SEG"(%3)           \n\t"                   \
          "   "#INS"l %5,"SEG"4(%3)          \n\t"                   \
          "   leal -12(%2),%2                \n\t"                   \
          "   "#INS"l %6,"SEG"8(%3)          \n\t"                   \
          "   leal 12(%3),%3                 \n\t"                   \
          "   cmpl $11,%2                    \n\t"                   \
          "   jg 1b                          \n\t"                   \
        : "=r" ((int) w), "=r" ((ulong *)p)                          \
        : "0"  ((int) w), "1" ((ulong *)p),                          \
          "r" ((ulong)f[0]), "r" ((ulong)f[1]), "r" ((ulong)f[2])    \
  ); } while (0)

#elif defined(__TURBOC__)

#pragma inline

#define __FILL_24_12__(p,f,w,INS,SEG,OPR) do {                    \
    _SI = (unsigned)(void near *)(f);                             \
    _ES = (unsigned)(void _seg *)(void far *)(p);                 \
    _DI = (unsigned)(void near *)(p);                             \
    _CX = (w);                                                    \
            asm mov  ax, word ptr ss:[si];                        \
            asm mov  bx, word ptr ss:[si+2];                      \
            asm mov  dx, word ptr ss:[si+4];                      \
 INS##24loop:                                                     \
            asm INS  word ptr es:[di], ax;                        \
            asm INS  word ptr es:[di+2], bx;                      \
            asm INS  word ptr es:[di+4], dx;                      \
            asm INS  word ptr es:[di+6], ax;                      \
            asm INS  word ptr es:[di+8], bx;                      \
            asm INS  word ptr es:[di+10], dx;                     \
            asm sub  cx, 12;                                      \
            asm add  di, 12;                                      \
            asm cmp  cx, 12;                                      \
            asm jge  INS##24loop;                                 \
    (unsigned)(void near *)(p) = _DI;                             \
    (w) = _CX;                                                    \
} while (0)

#define __SHORTFILL_24__(p,f,w,INS,SEG,OPR,LBL) do {              \
    _SI = (unsigned)(void near *)(f);                             \
    _ES = (unsigned)(void _seg *)(void far *)(p);                 \
    _DI = (unsigned)(void near *)(p);                             \
    _CX = (w);                                                    \
 INS##24short##LBL:                                               \
            asm sub  cx, 2;                                       \
            asm jb   INS##24short1##LBL;                          \
            asm mov  ax, word ptr ss:[si];                        \
            asm add  si, 2;                                       \
            asm INS  word ptr es:[di], ax;                        \
            asm add  di, 2;                                       \
            asm jmp  INS##24short##LBL;                           \
 INS##24short1##LBL:                                              \
            asm add  cx, 2;                                       \
            asm jz   INS##24short2##LBL;                          \
            asm mov  al, byte ptr ss:[si];                        \
            asm inc  si;                                          \
            asm INS  byte ptr es:[di], al;                        \
            asm inc  di;                                          \
            asm dec  cx;                                          \
 INS##24short2##LBL:                                              \
    (unsigned)(void near *)(p) = _DI;                             \
    (unsigned)(void near *)(f) = _SI;                             \
    (w) = _CX;                                                    \
} while (0)

#else /* !(__GNUC__ && __i386__) && !__TURBOC__ */

#define __FILL_24_12__(p,f,w,INS,SEG,OPR) do {                       \
                        poke_l##OPR(p, f[0]); ptrinc(p,4);           \
                        poke_l##OPR(p, f[1]); ptrinc(p,4);           \
                        poke_l##OPR(p, f[2]); ptrinc(p,4);           \
          } while ( ((w) -= 12) >= 12)

#endif

#ifndef __SHORTFILL_24__
#define __SHORTFILL_24__(p,f,w,INS,SEG,OPR,LBL) do {        \
                while ((w) >= 4) {                          \
                        poke_l##OPR(p, *((ulong *)f));      \
                        ptrinc(p,4); ptrinc(f,4); (w) -= 4; \
                }                                           \
                if ((w)&2) {                                \
                        poke_w##OPR(p, *((ushort *)f));     \
                        ptrinc(p,2); ptrinc(f,2); (w) -= 2; \
                }                                           \
                if ((w)&1) {                                \
                        poke_b##OPR(p, *((uchar *)f));      \
                        ptrinc(p,1); ptrinc(f,1); (w) -= 1; \
                }                                           \
        } while (0)
#endif

#ifndef __ROWFILL_24__
#define __ROWFILL_24__(p,f,w,INS,SEG,OPR) do {              \
                if ((w) >= 12)                              \
                  __FILL_24_12__(p,f,w,INS,SEG,OPR);        \
                __SHORTFILL_24__(p,f,w,INS,SEG,OPR,rf);     \
        } while (0)
#endif

#define rowfill_24_xor(p,fve,w)    __ROWFILL_24__(p,f,w,xor,"",_xor)
#define rowfill_24_or(p,fve,w)     __ROWFILL_24__(p,f,w,or,"",_or)
#define rowfill_24_and(p,fve,w)    __ROWFILL_24__(p,f,w,and,"",_and)
#define rowfill_24(p,fve,w)        __ROWFILL_24__(p,f,w,mov,"",)

#define rowfill_24_f_xor(p,fve,w)  __ROWFILL_24__(p,f,w,xor,I386_GCC_FAR_SELECTOR,_f_xor)
#define rowfill_24_f_or(p,fve,w)   __ROWFILL_24__(p,f,w,or,I386_GCC_FAR_SELECTOR,_f_or)
#define rowfill_24_f_and(p,fve,w)  __ROWFILL_24__(p,f,w,and,I386_GCC_FAR_SELECTOR,_f_and)
#define rowfill_24_f(p,fve,w)      __ROWFILL_24__(p,f,w,mov,I386_GCC_FAR_SELECTOR,_f)

#define rowfill_24s_f_xor(p,fve,w) __SHORTFILL_24__(p,f,w,xor,I386_GCC_FAR_SELECTOR,_f_xor,sf)
#define rowfill_24s_f_or(p,fve,w)  __SHORTFILL_24__(p,f,w,or,I386_GCC_FAR_SELECTOR,_f_or,sf)
#define rowfill_24s_f_and(p,fve,w) __SHORTFILL_24__(p,f,w,and,I386_GCC_FAR_SELECTOR,_f_and,sf)
#define rowfill_24s_f(p,fve,w)     __SHORTFILL_24__(p,f,w,mov,I386_GCC_FAR_SELECTOR,_f,sf)

#endif
