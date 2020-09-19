/*
 * mouse.h ---- GRX library private include file
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

#ifndef __INCLUDE_MOUSE_H__
#define __INCLUDE_MOUSE_H__

#include "globals.h"

struct _GrxCursor {
    /* <private> */
    GrxContext work;  /* work areas (4) */
    int xcord, ycord; /* cursor position on screen */
    int xsize, ysize; /* cursor size */
    int xoffs, yoffs; /* LU corner to hot point offset */
    int xwork, ywork; /* save/work area sizes */
    int xwpos, ywpos; /* save/work area position on screen */
    int displayed;    /* set if displayed */
    unsigned int ref_count;
};

/*
 * mouse stuff
 */
#define mouse_block(c, x1, y1, x2, y2) \
    {                                  \
        int __mouse_block_flag = 0;    \
        mouse_addblock(c, x1, y1, x2, y2);
#define mouse_addblock(c, x1, y1, x2, y2)                                     \
    if (MOUINFO->docheck && (c)->gc_is_on_screen) {                           \
        __mouse_block_flag |= (*MOUINFO->block)((c), (x1), (y1), (x2), (y2)); \
    }
#define mouse_unblock()                          \
    if (__mouse_block_flag) {                    \
        (*MOUINFO->unblock)(__mouse_block_flag); \
    }                                            \
    }

#endif /* __INCLUDE_MOUSE_H__ */
