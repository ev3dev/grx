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
 */

#include <grx/context.h>
#include <grx/extents.h>
#include <grx/mode.h>

#include "libgrx.h"

/**
 * grx_context_new:
 * @w: the width of the context
 * @h: the height of the context
 * @memory: (nullable): memory location or %NULL
 * @where: (nullable): an unused #GrxContext struct or %NULL
 *
 * Creates a new context in system memory using the memory layout specified by
 * grx_frame_mode_get_screen_core().
 *
 * @memory must contain a buffer of size grx_screen_get_memory_size(). %NULL may also
 * be passed to @memory, in which case the memory will be dynamically allocated.
 *
 * Likewise, @where can be an unused #GrxContext (e.g. you may want to do this
 * if you want a stack allocated context) or it can be %NULL to dynamically
 * allocate a new context.
 *
 * Returns: (nullable): @where or a new context if @where was %NULL. Returns
 *      %NULL on error.
 */
GrxContext *(grx_context_new)(gint w, gint h, guint8 *memory, GrxContext *where)
{
        return(grx_context_new(w,h,memory,where));
}

/**
 * grx_get_current_context:
 *
 * Gets the current context.
 *
 * Returns: (transfer none): the current context
 */
GrxContext *(grx_get_current_context)(void)
{
        return(grx_get_current_context());
}

/**
 * grx_get_screen_context:
 *
 * Gets the screen context.
 *
 * Returns: (transfer none): the screen context
 */
GrxContext *(grx_get_screen_context)(void)
{
        return(grx_get_screen_context());
}

/**
 * grx_get_clip_box:
 * @x1: (out): the left bounds
 * @y1: (out): the top bounds
 * @x2: (out): the right bounds
 * @y2: (out): the bottom bounds
 *
 * Gets the clipping limits of the current context.
 */
void (grx_get_clip_box)(int *x1p,int *y1p,int *x2p,int *y2p)
{
        grx_get_clip_box(x1p,y1p,x2p,y2p);
}

/**
 * grx_context_get_clip_box:
 * @context: the context
 * @x1: (out): the left bounds
 * @y1: (out): the top bounds
 * @x2: (out): the right bounds
 * @y2: (out): the bottom bounds
 *
 * Gets the clipping limits of @context.
 */
void (grx_context_get_clip_box)(const GrxContext *c,int *x1p,int *y1p,int *x2p,int *y2p)
{
        grx_context_get_clip_box(c,x1p,y1p,x2p,y2p);
}

/**
 * grx_get_max_x:
 *
 * Gets the index of the last pixel of the current context in the X direction.
 *
 * In other words, this is grx_get_width() -1.
 *
 * Returns: the index
 */
int (grx_get_max_x)(void)
{
        return(grx_get_max_x());
}

/**
 * grx_get_max_y:
 *
 * Gets the index of the last pixel of the current context in the Y direction.
 *
 * In other words, this is grx_get_height() -1.
 *
 * Returns: the index
 */
int (grx_get_max_y)(void)
{
        return(grx_get_max_y());
}

/**
 * grx_get_width:
 *
 * Gets the width of the current context in pixels.
 *
 * Returns: the width
 */
int (grx_get_width)(void)
{
        return(grx_get_width());
}

/**
 * grx_get_height:
 *
 * Gets the height of the current context in pixels.
 *
 * Returns: the height
 */
int (grx_get_height)(void)
{
        return(grx_get_height());
}

/**
 * grx_get_clip_box_min_x:
 *
 * Gets the left bounds of the clipping limits of the current context.
 *
 * Returns: the current value
 */
int (grx_get_clip_box_min_x)(void)
{
        return(grx_get_clip_box_min_x());
}

/**
 * grx_get_clip_box_min_y:
 *
 * Gets the top bounds of the clipping limits of the current context.
 *
 * Returns: the current value
 */
int (grx_get_clip_box_min_y)(void)
{
        return(grx_get_clip_box_min_y());
}

/**
 * grx_get_clip_box_max_x:
 *
 * Gets the right bounds of the clipping limits of the current context.
 *
 * Returns: the current value
 */
int (grx_get_clip_box_max_x)(void)
{
        return(grx_get_clip_box_max_x());
}

/**
 * grx_get_clip_box_max_y:
 *
 * Gets the bottom bounds of the clipping limits of the current context.
 *
 * Returns: the current value
 */
int (grx_get_clip_box_max_y)(void)
{
        return(grx_get_clip_box_max_y());
}

/**
 * grx_context_get_width:
 * @context: a #GrxContext
 *
 * Gets the width of @context.
 *
 * Returns: the width
 */
gint (grx_context_get_width)(GrxContext *context)
{
        return (grx_context_get_width(context));
}

/**
 * grx_context_get_height:
 * @context: a #GrxContext
 *
 * Gets the height of @context.
 *
 * Returns: the height
 */
gint (grx_context_get_height)(GrxContext *context)
{
        return (grx_context_get_height(context));
}

/**
 * grx_context_get_max_x:
 * @context: a #GrxContext
 *
 * Gets the right bounds of @context.
 *
 * Returns: the value
 */
gint (grx_context_get_max_x)(GrxContext *context)
{
        return (grx_context_get_max_x(context));
}

/**
 * grx_context_get_max_y:
 * @context: a #GrxContext
 *
 * Gets the bottom bounds of @context.
 *
 * Returns: the value
 */
gint (grx_context_get_max_y)(GrxContext *context)
{
        return (grx_context_get_max_y(context));
}
