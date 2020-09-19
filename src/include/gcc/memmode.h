/*
 * memmode.h ---- determine how to access video and system RAM
 *                GNU-C / DJGPP code
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
 */

#ifdef __i386__

#define MK_FP(s, o) (void *)(((long)(unsigned short)(s) << 4) + (unsigned short)(o))
#define FP_SEG(p)   (unsigned short)((long)(p) >> 4)
#define FP_OFF(p)   (unsigned short)((int)(p)&15)
#define FP86_SEG(p) (unsigned short)((long)(p) >> 16)
#define FP86_OFF(p) (unsigned short)((int)(p))
#define DECLARE_XFER_BUFFER(size)
#define DELETE_XFER_BUFFER

#endif /* __i386__ */
