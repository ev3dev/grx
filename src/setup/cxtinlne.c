/*
 * ctcinlne.c ---- the context inline functions
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
 *
 */

#include <grx/context.h>
#include <grx/mode.h>

#include "libgrx.h"

/**
 * grx_context_new:
 * @w: the width of the context
 * @h: the height of the context
 * @memory: (nullable): memory location(s) or %NULL
 * @where: (nullable): an unused #GrxContex struct or %NULL
 *
 * Creates a new context in system memory.
 *
 * @memory must contain grx_screen_n_planes() pointers (usually just one, but
 * could be as many as 4) of size grx_screen_get_plane_size(). %NULL may also
 * be passed to @memory, in which case the memory will be dynamically allocated.
 *
 * Likewise, @where can be an unused #GrxContext (e.g. you may want to do this
 * if you want a stack allocated context) or it can be %NULL to dynamically
 * allocate a new context.
 *
 * Returns: @where or a new context if @where was %NULL. Returns %NULL on error.
 */
GrxContext *(grx_context_new)(int w,int h,GrxFrameMemory *memory,GrxContext *where)
{
        return(grx_context_new(w,h,memory,where));
}

GrxContext *(grx_context_get_current)(void)
{
        return(grx_context_get_current());
}

GrxContext *(grx_context_get_screen)(void)
{
        return(grx_context_get_screen());
}

void (grx_get_clip_box)(int *x1p,int *y1p,int *x2p,int *y2p)
{
        grx_get_clip_box(x1p,y1p,x2p,y2p);
}

void (grx_context_get_clip_box)(const GrxContext *c,int *x1p,int *y1p,int *x2p,int *y2p)
{
        grx_context_get_clip_box(c,x1p,y1p,x2p,y2p);
}

int (grx_get_max_x)(void)
{
        return(grx_get_max_x());
}

int (grx_get_max_y)(void)
{
        return(grx_get_max_y());
}

int (grx_get_size_x)(void)
{
        return(grx_get_size_x());
}

int (grx_get_size_y)(void)
{
        return(grx_get_size_y());
}

int (grx_get_low_x)(void)
{
        return(grx_get_low_x());
}

int (grx_get_low_y)(void)
{
        return(grx_get_low_y());
}

int (grx_get_high_x)(void)
{
        return(grx_get_high_x());
}

int (grx_get_high_y)(void)
{
        return(grx_get_high_y());
}
