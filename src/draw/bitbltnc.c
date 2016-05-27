/*
 * bitbltnc.c ---- the pixel transfer routine (no clip)
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu].
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
#include "libgrx.h"
#include "clipping.h"

/**
 * grx_context_fast_bit_blt:
 * @context: the destination context
 * @x: the destination X coordinate
 * @y: the destination Y coordinate
 * @src: (nullable): the source context or %NULL to use the current context
 * @x1: the source bounding rectangle left coordinate
 * @y1: the source bounding rectangle top coordinate
 * @x2: the source bounding rectangle right coordinate
 * @y2: the source bounding rectangle bottom coordinate
 * @op: the #GrxColorMode operator and optional transparent color
 *
 * Copies an area bounded by x1, y2, x2, y2 in the source context to the
 * destination context at the location specified by dx, dy using the specified
 * operation.
 *
 * This function does not perform clipping. If you cannot ensure that the
 * destination rectangle is within the bounds of the context, then use
 * grx_context_bit_blt() instead.
 */
void grx_context_fast_bit_blt(GrxContext *dst,int dx,int dy,
                GrxContext *src,int x1,int y1,int x2,int y2,GrxColor oper)
{
        void (*bltfun)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor);
        if(dst == NULL) dst = CURC;
        if(src == NULL) src = CURC;
        isort(x1,x2);
        isort(y1,y2);
        if(src->gc_driver == dst->gc_driver)
            bltfun = src->gc_driver->bitblt;
        else if(src->gc_driver->mode == dst->gc_driver->rmode)
            bltfun = dst->gc_driver->bltr2v;
        else if(src->gc_driver->rmode == dst->gc_driver->mode)
            bltfun = src->gc_driver->bltv2r;
        else return;
        (*bltfun)(
            &dst->frame,(dx + dst->x_offset),(dy + dst->y_offset),
            &src->frame,(x1 + src->x_offset),(y1 + src->y_offset),
            (x2 - x1 + 1),
            (y2 - y1 + 1),
            oper
        );
}
