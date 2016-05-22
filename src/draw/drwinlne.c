/*
 * drvinlne.c ---- the drawing inline functions
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
 *
 */

#include <grx/context.h>
#include <grx/draw.h>
#include <grx/draw_nc.h>
#include <grx/mode.h>

#include "libgrx.h"

/**
 * SECTION:draw
 * @short_description: Basic drawing functions
 * @title: Graphics Primitives
 * @section_id: draw
 * @include: grx-3.0.h
 *
 * These functions operate on the current context (or, occasionally, the screen).
 */

/**
 * grx_bit_blt:
 * @x: the destination X coordinate
 * @y: the destination Y coordinate
 * @src: (nullable): the source context or %NULL to use the current context
 * @x1: the source bounding rectangle left coordinate
 * @y1: the source bounding rectangle top coordinate
 * @x2: the source bounding rectangle right coordinate
 * @y2: the source bounding rectangle bottom coordinate
 * @op: the #GrxColorMode operator and optional transparent color
 *
 * Copies an area bounded by x1, y2, x2, y2 in the source context to the current
 * context at the location specified by dx, dy using the specified operation.
 */
void (grx_bit_blt)(int dx, int dy, GrxContext *src, int x1, int y1,
                   int x2, int y2, GrxColor oper)
{
    grx_context_bit_blt(NULL,dx,dy,src,x1,x2,y1,y2,oper);
}

/**
 * grx_bit_blt_1bpp:
 * @x: the destination X coordinate
 * @y: the destination Y coordinate
 * @src: (nullable): the source context or %NULL to use the current context
 * @x1: the source bounding rectangle left coordinate
 * @y1: the source bounding rectangle top coordinate
 * @x2: the source bounding rectangle right coordinate
 * @y2: the source bounding rectangle bottom coordinate
 * @fg: the foreground color
 * @bg: the background color
 *
 * Copies an area bounded by x1, y2, x2, y2 in the source context to the
 * current context at the location specified by dx, dy using the specified
 * operation.
 *
 * The source must be a 1 bit per pixel bitmap. The foreground color is used
 * for bits = 1 and the background color is used for bits = 0.
 */
void (grx_bit_blt_1bpp)(int dx, int dy, GrxContext *src, int x1, int y1,
                        int x2,int y2, GrxColor fg, GrxColor bg)
{
    grx_context_bit_blt_1bpp(NULL, dx, dy, src, x1, y1, x2, y2, fg, bg);
}

void (grx_fast_draw_pixel)(int x,int y,GrxColor c)
{
        grx_fast_draw_pixel(x,y,c);
}

GrxColor (grx_fast_get_pixel)(int x,int y)
{
        return(grx_fast_get_pixel(x,y));
}

GrxColor (grx_context_fast_get_pixel)(GrxContext *c,int x,int y)
{
        return(grx_context_fast_get_pixel(c,x,y));
}
