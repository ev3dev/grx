/**
 ** ORDSWAP.H ---- definitions for multibyte value order swaping
 **
 ** Copyright (c) 1998 Hartmut Schirmer
 **/


extern void _GR_swap16(GR_int16 far *w);
extern void _GR_swap32(GR_int32 far *l);

#define _GR_swap16s(w) _GR_swap16((GR_int16 far *)(w))
#define _GR_swap16u(w) _GR_swap16((GR_int16 far *)(w))
#define _GR_swap32s(l) _GR_swap32((GR_int32 far *)(l))
#define _GR_swap32u(l) _GR_swap32((GR_int32 far *)(l))

#ifdef GR_int64
extern void _GR_swap64(GR_int64 far *h);
#define _GR_swap64s(h) _GR_swap64((GR_int64 far *)(h))
#define _GR_swap64u(h) _GR_swap64((GR_int64 far *)(h))
#endif
