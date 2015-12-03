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

#include "libgrx.h"
#include "allocate.h"
#include "clipping.h"
#include "memcopy.h"
#include "memfill.h"

#define  MYCONTEXT      1
#define  MYFRAME        2

G_DEFINE_BOXED_TYPE(GrxContext, grx_context, grx_context_copy, grx_context_free);

GrxContext *grx_context_create_full(GrxFrameMode md, int w, int h,
                                    unsigned char *memory[4], GrxContext *where)
{
        GrxFrameDriver *fd = _GrFindRAMframeDriver(md);
        int  ii,offset,flags = 0;
        unsigned char *mymem[4];
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
                mymem[ii] = malloc((size_t)psize);
                if(!mymem[ii]) {
                    while(--ii >= 0) free(mymem[ii]);
                    if(flags) free(where);
                    return(NULL);
                }
            }
            while(ii < 4) mymem[ii++] = NULL;
            memory = mymem;
            flags |= MYFRAME;
        }
        where->gc_driver      = fd;
        where->gc_base_address[0] = memory[0];
        where->gc_base_address[1] = memory[1];
        where->gc_base_address[2] = memory[2];
        where->gc_base_address[3] = memory[3];
        where->gc_line_offset  = offset;
        where->gc_memory_flags    = flags;
        where->x_clip_high     = where->x_max = w - 1;
        where->y_clip_high     = where->y_max = h - 1;
        return(where);
}

GrxContext *grx_context_create_subcontext(int x1, int y1, int x2, int y2,
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
        where->gc_memory_flags = flags;
        where->x_offset  = x1;
        where->y_offset  = y1;
        where->x_clip_high  = where->x_max = x2 - x1;
        where->y_clip_high  = where->y_max = y2 - y1;
        where->root     = (GrxContext *)parent;
        return(where);
}

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

GrxContext *grx_context_copy(GrxContext *context)
{
    GrxContext *new_context;
    long psize;
    int i;

    new_context = grx_context_create_full(context->gc_driver->mode,
                                          context->x_max + 1,
                                          context->y_max + 1, NULL, NULL);
    if (!new_context) {
        return NULL;
    }
    psize  = grx_frame_mode_get_plane_size(context->gc_driver->mode,
                                           context->x_max + 1,
                                           context->y_max + 1);
    for (i = 0; i < context->gc_driver->num_planes; i++) {
        memcopy(new_context->gc_base_address[i], context->gc_base_address[i],
                psize);
    }

    return new_context;
}

void grx_context_free(GrxContext *cxt)
{
        if(cxt && (cxt != CURC) && (cxt != SCRN)) {
            if(cxt->gc_memory_flags & MYFRAME) {
                int ii = cxt->gc_driver->num_planes;
                while(--ii >= 0) free(cxt->gc_base_address[ii]);
            }
            if(cxt->gc_memory_flags & MYCONTEXT) free(cxt);
        }
}

void grx_context_set_current(const GrxContext *context)
{
        if(!context) context = SCRN;
        sttcopy(CURC,context);
        sttcopy(FDRV,context->gc_driver);
}

GrxContext *grx_context_save(GrxContext *where)
{
        int flags = 0;

        if(!where) {
            where = malloc(sizeof(GrxContext));
            if(!where) return(NULL);
            flags = MYCONTEXT;
        }
        sttcopy(where,CURC);
        where->gc_memory_flags = flags;
        return(where);
}
