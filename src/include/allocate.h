/*
 * allocate.h ---- common ground for malloc & friends in 16 & 32 bit envs
 *                 stack based temporary memory allocation
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

#include <glib.h>

/* temp buffer for blits etc. */
G_GNUC_INTERNAL extern void *_GrTempBuffer;
G_GNUC_INTERNAL extern unsigned _GrTempBufferBytes;
#define _GrTempBufferAlloc(b) \
    (((unsigned)(b) <= _GrTempBufferBytes) ? _GrTempBuffer : _GrTempBufferAlloc_(b))
G_GNUC_INTERNAL extern void *_GrTempBufferAlloc_(size_t bytes);
G_GNUC_INTERNAL extern void _GrTempBufferFree(void);
