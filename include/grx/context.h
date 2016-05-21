/*
 * context.h
 *
 * Copyright (c) 2015 David Lechner <david@lechnology.com>
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

#ifndef __GRX_CONTEXT_H__
#define __GRX_CONTEXT_H__

#include <glib.h>
#include <glib-object.h>

#include <grx/color.h>
#include <grx/mode.h>

/* ================================================================== */
/*              FRAME BUFFER, CONTEXT AND CLIPPING STUFF              */
/* ================================================================== */

struct _GrxContext {
    GrxFrame    frame;                  /* frame buffer info */
    GrxContext *root;                   /* context which owns frame */
    guint  ref_count;                   /* private reference count */
    int    x_max;                       /* max X coord (width  - 1) */
    int    y_max;                       /* max Y coord (height - 1) */
    int    x_offset;                    /* X offset from root's base */
    int    y_offset;                    /* Y offset from root's base */
    int    x_clip_low;                  /* low X clipping limit */
    int    y_clip_low;                  /* low Y clipping limit */
    int    x_clip_high;                 /* high X clipping limit */
    int    y_clip_high;                 /* high Y clipping limit */
    int    user_x_base;                 /* user window min X coordinate */
    int    user_y_base;                 /* user window min Y coordinate */
    int    user_width;                  /* user window width  */
    int    user_height;                 /* user window height */
#   define gc_base_address              frame.base_address
#   define gc_selector                  frame.selector
#   define gc_is_on_screen              frame.is_on_screen
#   define gc_memory_flags              frame.memory_flags
#   define gc_line_offset               frame.line_offset
#   define gc_driver                    frame.driver
};

#ifndef __GI_SCANNER__
/* TODO: This struct should probably be private */

extern const struct _GR_contextInfo {
    GrxContext current;         /* the current context */
    GrxContext screen;          /* the screen context */
} * const GrContextInfo;

#endif /* __GI_SCANNER__ */

GType       grx_context_get_type(void);
GrxContext *grx_context_create(gint w, gint h, guint8 *memory[4], GrxContext *where);
GrxContext *grx_context_create_full(GrxFrameMode md, gint w, gint h,
                                    guint8 *memory[4], GrxContext *where);
GrxContext *grx_context_create_subcontext(gint x1, gint y1, gint x2, gint y2,
                                          const GrxContext *parent, GrxContext *where);
GrxContext *grx_context_save(GrxContext *where);

GrxContext *grx_context_get_current(void);
GrxContext *grx_context_get_screen(void);

GrxContext *grx_context_ref(GrxContext *context);
void  grx_context_unref(GrxContext *context);
void  grx_context_resize_subcontext(GrxContext *context, gint x1, gint y1, gint x2, gint y2);
void  grx_context_set_current(const GrxContext *context);
void  grx_context_clear(GrxContext *ctx, GrxColor bg);

void  grx_set_clip_box(gint x1, gint y1, gint x2, gint y2);
void  grx_context_set_clip_box(GrxContext *c, gint x1, gint y1, gint x2, gint y2);
void  grx_get_clip_box(gint *x1p, gint *y1p, gint *x2p, gint *y2p);
void  grx_context_get_clip_box(const GrxContext *c, gint *x1p, gint *y1p, gint *x2p, gint *y2p);
void  grx_reset_clip_box(void);
void  grx_context_reset_clip_box(GrxContext *c);

int   grx_get_max_x(void);
int   grx_get_max_y(void);
int   grx_get_size_x(void);
int   grx_get_size_y(void);
int   grx_get_low_x(void);
int   grx_get_low_y(void);
int   grx_get_high_x(void);
int   grx_get_high_y(void);

#ifndef GRX_SKIP_INLINES
#define grx_context_create(w,h,m,c) (grx_context_create_full(grx_get_core_frame_mode(),w,h,m,c))
#define grx_context_get_current()       ((GrxContext *)(&GrContextInfo->current))
#define grx_context_get_screen()        ((GrxContext *)(&GrContextInfo->screen))
#define grx_get_max_x()                 (grx_context_get_current()->x_max)
#define grx_get_max_y()                 (grx_context_get_current()->y_max)
#define grx_get_size_x()                (grx_get_max_x() + 1)
#define grx_get_size_y()                (grx_get_max_y() + 1)
#define grx_get_low_x()                 (grx_context_get_current()->x_clip_low)
#define grx_get_low_y()                 (grx_context_get_current()->y_clip_low)
#define grx_get_high_x()                (grx_context_get_current()->x_clip_high)
#define grx_get_high_y()                (grx_context_get_current()->y_clip_high)
#define grx_context_get_clip_box(C,x1p,y1p,x2p,y2p) do {           \
    *(x1p) = (C)->x_clip_low;                           \
    *(y1p) = (C)->y_clip_low;                           \
    *(x2p) = (C)->x_clip_high;                          \
    *(y2p) = (C)->y_clip_high;                          \
} while(0)
#define grx_get_clip_box(x1p,y1p,x2p,y2p) do {              \
    *(x1p) = grx_get_low_x();                                  \
    *(y1p) = grx_get_low_y();                                  \
    *(x2p) = grx_get_high_x();                                 \
    *(y2p) = grx_get_high_y();                                 \
} while(0)
#endif  /* GRX_SKIP_INLINES */

#endif /* __GRX_CONTEXT_H__ */
