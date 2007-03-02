/**
 ** ioport.h ---- port input/output macros
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu]
 **
 ** This file is part of the GRX graphics library.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** Intel CPU specific input/output instructions
 **
 **/

#ifndef __IOPORT_H_INCLUDED__
#define __IOPORT_H_INCLUDED__

#ifdef  SLOW_DOWN_IO
#ifndef SLOW_DOWN_IO_PORT
#define SLOW_DOWN_IO_PORT   0x80
#endif
#ifdef  __GNUC__
#ifdef  __i386__
#define __INLINE_SLOW_IO_ONCE__  ({                     \
    __asm__ volatile(                                   \
    "outb %%al,%0"                                      \
    : /* no outputs */                                  \
    : "n" (SLOW_DOWN_IO_PORT)                           \
    );                                                  \
})
#endif
#endif

#if (SLOW_DOWN_IO - 0) <= 1
#define __INLINE_SLOW_DOWN_IO__                         \
    __INLINE_SLOW_IO_ONCE__
#elif   (SLOW_DOWN_IO - 0) == 2
#define __INLINE_SLOW_DOWN_IO__                         \
    __INLINE_SLOW_IO_ONCE__,                            \
    __INLINE_SLOW_IO_ONCE__
#elif   (SLOW_DOWN_IO - 0) == 3
#define __INLINE_SLOW_DOWN_IO__                         \
    __INLINE_SLOW_IO_ONCE__,                            \
    __INLINE_SLOW_IO_ONCE__,                            \
    __INLINE_SLOW_IO_ONCE__
#else
#define __INLINE_SLOW_DOWN_IO__                         \
    __INLINE_SLOW_IO_ONCE__,                            \
    __INLINE_SLOW_IO_ONCE__,                            \
    __INLINE_SLOW_IO_ONCE__,                            \
    __INLINE_SLOW_IO_ONCE__
#endif
#define __INLINE_SLOW_DWN_IOC__ __INLINE_SLOW_DOWN_IO__,
#else   /* SLOW_DOWN_IO */
#define __INLINE_SLOW_DOWN_IO__
#define __INLINE_SLOW_DWN_IOC__
#endif

#ifdef  __GNUC__
#ifdef  __i386__
#define __INLINE_INPORT__(P,SIZE,T) ({                  \
    register T _value;                                  \
    __asm__ volatile(                                   \
    "in"#SIZE" %w1,%0"                                  \
    : "=a"  (_value)                                    \
    : "Nd"   ((unsigned short)(P))                      \
    );                                                  \
    __INLINE_SLOW_DOWN_IO__;                            \
    _value;                                             \
})
#define __INLINE_OUTPORT__(P,V,SIZE,T) ({               \
    __asm__ volatile(                                   \
    "out"#SIZE" %"#SIZE"0,%w1"                          \
    : /* no outputs */                                  \
    : "a" ((unsigned T)(V)),                            \
      "Nd" ((unsigned short)(P))                        \
    );                                                  \
    __INLINE_SLOW_DOWN_IO__;                            \
})
#ifndef SLOW_DOWN_IO
#define __INLINE_INPORTS__(P,B,C,SIZE,T) ({             \
    register void *_inportptr = ((void *)(B));          \
    register int   _inportcnt = ((int)(C));             \
    __asm__ volatile(                                   \
    "cld; "                                             \
    "rep; ins"#SIZE""                                   \
    : "=D" (_inportptr), "=c" (_inportcnt)              \
    : "0"  (_inportptr), "1"  (_inportcnt),             \
      "d"  ((unsigned short)(P))                        \
    );                                                  \
})
#define __INLINE_OUTPORTS__(P,B,C,SIZE,T) ({            \
    register void *_outportptr = ((void *)(B));         \
    register int   _outportcnt = ((int)(C));            \
    __asm__ volatile(                                   \
    "cld; "                                             \
    "rep; outs"#SIZE""                                  \
    : "=S" (_outportptr), "=c" (_outportcnt)            \
    : "0"  (_outportptr), "1"  (_outportcnt),           \
      "d"  ((unsigned short)(P))                        \
    );                                                  \
})
#else   /* SLOW_DOWN_IO */
#define __INLINE_INPORTS__(P,B,C,SIZE,T) ({             \
    register T  *_inportptr = ((T *)(B));               \
    register int _inportcnt = ((int)(C));               \
    register int _inportadr = ((int)(P));               \
    do *_inportptr++ = inport_##SIZE(_inportadr);       \
    while(--_inportcnt);                                \
})
#define __INLINE_OUTPORTS__(P,B,C,SIZE,T) ({            \
    register T  *_outportptr = ((T *)(B));              \
    register int _outportcnt = ((int)(C));              \
    register int _outportadr = ((int)(P));              \
    do outport_##SIZE(_outportadr,*_outportptr++);      \
    while(--_outportcnt);                               \
})
#endif  /* SLOW_DOWN_IO */
#endif  /* __i386__ */
#endif  /* __GNUC__ */

#ifndef inport_b
#define inport_b(port)          __INLINE_INPORT__(port,b,char)
#endif
#ifndef inport_w
#define inport_w(port)          __INLINE_INPORT__(port,w,short)
#endif
#ifndef inport_l
#define inport_l(port)          __INLINE_INPORT__(port,l,long)
#endif

#ifndef outport_b
#define outport_b(port,val)     __INLINE_OUTPORT__(port,val,b,char)
#endif
#ifndef outport_w
#define outport_w(port,val)     __INLINE_OUTPORT__(port,val,w,short)
#endif
#ifndef outport_l
#define outport_l(port,val)     __INLINE_OUTPORT__(port,val,l,long)
#endif

#ifndef inport_b_s
#define inport_b_s(port,buf,cnt)    __INLINE_INPORTS__(port,buf,cnt,b,char)
#endif
#ifndef inport_w_s
#define inport_w_s(port,buf,cnt)    __INLINE_INPORTS__(port,buf,cnt,w,short)
#endif
#ifndef inport_l_s
#define inport_l_s(port,buf,cnt)    __INLINE_INPORTS__(port,buf,cnt,l,long)
#endif

#ifndef outport_b_s
#define outport_b_s(port,buf,cnt)   __INLINE_OUTPORTS__(port,buf,cnt,b,char)
#endif
#ifndef outport_w_s
#define outport_w_s(port,buf,cnt)   __INLINE_OUTPORTS__(port,buf,cnt,w,short)
#endif
#ifndef outport_l_s
#define outport_l_s(port,buf,cnt)   __INLINE_OUTPORTS__(port,buf,cnt,l,long)
#endif

#ifdef  __GNUC__
#ifdef  __i386__
#ifdef  __MSDOS__
#define int_disable()           ({ __asm__ volatile("cli"); })
#define int_enable()            ({ __asm__ volatile("sti"); })
#else
#define int_disable()
#define int_enable()
#endif
#endif
#endif

#endif  /* whole file */

