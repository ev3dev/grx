/*
 * context.c ----- context creation and manipulation functions
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

#include <glib-object.h>
#include <string.h>

#include "globals.h"
#include "libgrx.h"
#include "allocate.h"
#include "clipping.h"
#include "memcopy.h"
#include "memfill.h"
#include "util.h"

#define  MYCONTEXT      1
#define  MYFRAME        2

G_DEFINE_BOXED_TYPE(GrxContext, grx_context, grx_context_ref, grx_context_unref);

GrxContext *grx_context_ref(GrxContext *context);
void grx_context_unref(GrxContext *ctx);

/**
 * grx_context_new_full:
 * @mode: the frame mode
 * @w: the width of the context
 * @h: the height of the context
 * @memory: (nullable): memory location(s) or %NULL
 * @where: (nullable): an unused #GrxContex struct or %NULL
 *
 * Creates a new context in system memory using the memory layout specified by
 * @mode.
 *
 * @memory must contain grx_screen_n_planes() pointers (usually just one, but
 * could be as many as 4) of size grx_screen_get_plane_size(). %NULL may also
 * be passed to @memory, in which case the memory will be dynamically allocated.
 *
 * Likewise, @where can be an unused #GrxContext (e.g. you may want to do this
 * if you want a stack allocated context) or it can be %NULL to dynamically
 * allocate a new context.
 *
 * Returns: (nullable): @where or a new context if @where was %NULL. Returns
 *      %NULL on error.
 */
GrxContext *grx_context_new_full(GrxFrameMode md, int w, int h,
                                 GrxFrameMemory *memory, GrxContext *where)
{
        GrxFrameDriver *fd = _GrFindRAMframeDriver(md);
        int  ii,offset,flags = 0;
        GrxFrameMemory mymem;
        long psize;

        if(!fd) return(NULL);
        offset = grx_frame_mode_get_line_offset(md,w);
        psize  = grx_frame_mode_get_plane_size(md,w,h);
        if(psize <= 0L) return(NULL);
        if(psize >  fd->max_plane_size) return(NULL);
        if(!where) {
            where = malloc(sizeof(GrxContext));
            if(!where) return(NULL);
            flags = MYCONTEXT;
        }
        sttzero(where);
        if(!memory) {
            for(ii = 0; ii < fd->num_planes; ii++) {
                GRX_FRAME_MEMORY_PLANE(&mymem,ii) = malloc((size_t)psize);
                if(!GRX_FRAME_MEMORY_PLANE(&mymem,ii)) {
                    while(--ii >= 0) free(GRX_FRAME_MEMORY_PLANE(&mymem,ii));
                    if(flags) free(where);
                    return(NULL);
                }
            }
            while(ii < 4) GRX_FRAME_MEMORY_PLANE(&mymem,ii++) = NULL;
            memory = &mymem;
            flags |= MYFRAME;
        }
        where->ref_count = 1;
        where->gc_driver      = fd;
        where->gc_base_address.plane0 = memory->plane0;
        where->gc_base_address.plane1 = memory->plane1;
        where->gc_base_address.plane2 = memory->plane2;
        where->gc_base_address.plane3 = memory->plane3;
        where->gc_line_offset  = offset;
        where->gc_memory_flags    = flags;
        where->x_clip_high     = where->x_max = w - 1;
        where->y_clip_high     = where->y_max = h - 1;
        return(where);
}

/**
 * grx_context_new_subcontext:
 * @x1: the left bounds
 * @y1: the top bounds
 * @x2: the right bounds
 * @y2: the bottom bounds
 * @parent: (nullable): the parent context or %NULL to use the screen
 * @where: (nullable): an unused #GrxContext struct to hold the subcontext or %NULL
 *
 * Creates a new sub-context which maps to a part of an existing context.
 *
 * The coordinate arguments (x1 through y2) are interpreted relative to the
 * parent context's limits. Pixel addressing is zero-based even in sub-contexts,
 * i.e. the address of the top left pixel is (0,0) even in a sub-context which
 * has been mapped onto the interior of its parent context.
 *
 * Returns: (nullable): @where or a new context if @where was %NULL. Return %NULL on error.
 */
GrxContext *grx_context_new_subcontext(int x1, int y1, int x2, int y2,
                                          const GrxContext *parent,
                                          GrxContext *where)
{
        int flags = 0;

        if(!parent) parent = SCRN;
        if(parent->root) {
            x1 += parent->x_offset;
            y1 += parent->y_offset;
            x2 += parent->x_offset;
            y2 += parent->y_offset;
            parent = parent->root;
        }
        cxclip_box_(parent,x1,y1,x2,y2,return(NULL),CLIP_EMPTY_MACRO_ARG);
        if(!where) {
            where = malloc(sizeof(GrxContext));
            if(!where) return(NULL);
            flags = MYCONTEXT;
        }
        sttzero(where);
        sttcopy(&where->frame,&parent->frame);
        where->ref_count = 1;
        where->gc_memory_flags = flags;
        where->x_offset  = x1;
        where->y_offset  = y1;
        where->x_clip_high  = where->x_max = x2 - x1;
        where->y_clip_high  = where->y_max = y2 - y1;
        where->root     = (GrxContext *)parent;
        grx_context_ref (where->root);
        return(where);
}

/**
 * grx_context_resize_subcontext:
 * @context: the context
 * @x1: the new left bounds
 * @y1: the new top bounds
 * @x2: the new right bounds
 * @y2: the new bottom bounds
 *
 * Sub-contexts can be resized, but not their parents (i.e. anything returned
 * by grx_context_new() or set up by grx_set_mode() cannot be resized – because
 * this could lead to irrecoverable "loss" of drawing memory.
 *
 * The coordinate arguments (x1 through y2) are interpreted relative to the
 * parent context's limits.
 */
void grx_context_resize_subcontext(GrxContext *context, int x1, int y1, int x2,
                                   int y2)
{
        GrxContext *parent = context->root;

        if((parent = context->root) == NULL) return;
/*
        x1 += context->x_offset;
        y1 += context->y_offset;
        x2 += context->x_offset;
        y2 += context->y_offset;
*/
        cxclip_box(parent,x1,y1,x2,y2);
        context->x_offset = x1;
        context->y_offset = y1;
        context->x_clip_high = context->x_max = x2 - x1;
        context->y_clip_high = context->y_max = y2 - y1;
        context->x_clip_low = 0;
        context->y_clip_low = 0;
}

/**
 * grx_context_ref:
 * @context: the context
 *
 * Increases the reference count to @context.
 *
 * Returns: the @context
 */
GrxContext *grx_context_ref(GrxContext *context)
{
    g_return_val_if_fail(context != NULL, NULL);

    context->ref_count++;

    return context;
}

static void grx_context_free(GrxContext *cxt)
{
        if(cxt && (cxt != CURC) && (cxt != SCRN)) {
            if (cxt->root) {
                grx_context_unref (cxt->root);
            }
            if(cxt->gc_memory_flags & MYFRAME) {
                int ii = cxt->gc_driver->num_planes;
                while(--ii >= 0) free(GRX_FRAME_MEMORY_PLANE(&cxt->gc_base_address,ii));
            }
            if(cxt->gc_memory_flags & MYCONTEXT) free(cxt);
        }
}

/**
 * grx_context_unref:
 * @context: the context
 *
 * Decreases the reference count to @context.
 *
 * When the reference count drops to 0, the context and any dynamically
 * allocated memory of the context will be freed.
 */
void grx_context_unref(GrxContext *ctx)
{
    g_return_if_fail(ctx != NULL);
    g_return_if_fail(ctx->ref_count > 0);

    ctx->ref_count--;
    if (ctx->ref_count == 0) {
        grx_context_free(ctx);
    }
}

/**
 * grx_context_set_current:
 * @context: the context
 *
 * Sets the current context to @context.
 *
 * The current context structure is stored in a static location in the library.
 * (For efficiency reasons – it is used quite frequently, and this way no
 * pointer dereferencing is necessary.) The context stores all relevant
 * information about the video organization, coordinate limits, etc...
 */
void grx_context_set_current(const GrxContext *context)
{
        if(!context) context = SCRN;
        sttcopy(CURC,context);
        sttcopy(FDRV,context->gc_driver);
}

/**
 * grx_context_save: (skip)
 * @where: (nullable): unused #GrxContext where the context will be saved or %NULL
 *
 * Saves a copy of the current context.
 *
 * This is generally used to temporarily save and restore a context. Example:
 * |[<!-- language="C" -->
 * void my_function (GrxContext *my_context)
 * {
 *     GrxContext save;
 *
 *     grx_context_save (&save);
 *     grx_context_set_current (my_context);
 *     // call some drawing functions
 *     grx_context_set_current (&save);
 * }
 * ]|
 *
 * Returns: (nullable): @where or a new context if @where was %NULL. Returns
 *      %NULL on error.
 */
GrxContext *grx_context_save(GrxContext *where)
{
        int flags = 0;

        if(!where) {
            where = malloc(sizeof(GrxContext));
            if(!where) return(NULL);
            flags = MYCONTEXT;
        }
        sttcopy(where,CURC);
        where->ref_count = 1;
        where->gc_memory_flags = flags;
        return(where);
}
