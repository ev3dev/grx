/**
 ** ALLOCA.H ---- stack based temporary memory allocation
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#ifdef __GNUC__
#include <stdlib.h>
#define setup_alloca()
#define reset_alloca()
#endif

#ifdef __TURBOC__
/*
 * I have TC++ 1.01 (quite old). It is possible that newer TC++ versions
 * have a built-in alloca.
 */
#if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
extern unsigned int __brklvl;
#define alloca(size) (                                                  \
    ((_AX = _SP - (((unsigned int)(size) + 3) & ~3)) > __brklvl) ?      \
    (void near *)(_SP = _AX) :                                          \
    (void near *)(0)                                                    \
)
#else
extern unsigned int _stklen;
#define alloca(size) (                                                  \
    ((_AX = _SP - (((unsigned int)(size) + 3) & ~3)) < _stklen) ?       \
    (void far *)((void _seg *)(_SS) + (void near *)(_SP = _AX)) :       \
    (void far *)(0)                                                     \
)
#endif
#define setup_alloca() { unsigned int __saved_SP__ = _SP;
#define reset_alloca() _SP = __saved_SP__; }

#endif /* __TURBOC__ */

