/*
 * getscl.c ---- get scanline pixels
 *
 * Copyright (c) 1998 Hartmut Schirmer
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

#include "globals.h"
#include "mouse.h"
#include "libgrx.h"
#include "clipping.h"

/**
 * grx_context_get_scanline:
 * @context: the context
 * @x1: the starting X coordinate
 * @x2: the ending X coordinate
 * @y: the Y coordinate
 * @n: (out) (optional): the length of the array
 *
 * An efficient way to get pixels from a context. Important: the return value
 * is only valid until the next Grx call!
 *
 * Also see grx_get_scanline() for operating on the current context.
 *
 * Returns: (array length=n) (nullable) (transfer none):
 *     an array of color values from the scanned pixels or %NULL if there was
 *     an error
 */
const GrxColor *grx_context_get_scanline(GrxContext *ctx,int x1,int x2,int yy,unsigned int *n)
{
        GrxColor *res = NULL;
        if (ctx == NULL) ctx = CURC;
        /* don't accept any clipping .... */
        clip_hline_(ctx,x1,x2,yy,goto done,goto done);
        mouse_block(ctx,x1,yy,x2,yy);
        res = (*ctx->gc_driver->getindexedscanline)(
            &ctx->frame,
            x1 + ctx->x_offset,
            yy + ctx->y_offset,
            x2 - x1 + 1,
            NULL
        );
        mouse_unblock();
        if (n) {
            *n = x2 - x1;
        }
done:   return res;
}

/**
 * grx_get_scanline:
 * @x1: the starting X coordinate
 * @x2: the ending X coordinate
 * @y: the Y coordinate
 * @n: (out) (optional): the length of the array
 *
 * An efficient way to get pixels from the current context. Important: the
 * return value is only valid until the next Grx call!
 *
 * Returns: (array length=n) (nullable) (transfer none):
 *     an array of color values from the scanned pixels or %NULL if there was
 *     an error
 */
const GrxColor *(grx_get_scanline)(int x1,int x2,int yy, unsigned int *n) {
  return grx_context_get_scanline(NULL, x1,x2,yy,n);
}
