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
#include <grx/common.h>

/**
 * SECTION:context
 * @short_description: Drawing regions
 * @title: Graphics Contexts
 * @section_id: context
 * @include: grx-3.0.h
 *
 * The library supports a set of drawing regions called contexts (#GrxContext).
 * These can be in video memory or in system memory. Contexts in system memory
 * always have the same memory organization as the video memory. When
 * grx_set_mode() is called, a default context is created which maps to the
 * whole graphics screen.
 */

typedef struct {
    guint8 *plane0;
    guint8 *plane1;
    guint8 *plane2;
    guint8 *plane3;
} GrxFrameMemory;

/**
 * GRX_FRAME_MEMORY_PLANE:
 * @f: pointer to #GrxFrameMemory
 * @p: the index of the plane (0-3)
 *
 * Gets a plane from #GrxFrameMemory by its index.
 */
#define GRX_FRAME_MEMORY_PLANE(f,p) (((guint8**)f)[p])

struct _GrxFrame {
    GrxFrameMemory  base_address;       /* base address of frame memory */
    gshort          selector;           /* frame memory segment selector */
    gboolean        is_on_screen;       /* is it in video memory ? */
    guint8          memory_flags;       /* memory allocation flags */
    gint            line_offset;        /* offset to next scan line in bytes */
    GrxFrameDriver *driver;             /* frame access functions */
};

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
GrxContext *grx_context_new(gint w, gint h, GrxFrameMemory *memory, GrxContext *where);
GrxContext *grx_context_new_full(GrxFrameMode md, gint w, gint h,
                                 GrxFrameMemory *memory, GrxContext *where);
GrxContext *grx_context_new_subcontext(gint x1, gint y1, gint x2, gint y2,
                                       const GrxContext *parent, GrxContext *where);
GrxContext *grx_context_save(GrxContext *where);

GrxContext *grx_context_get_current(void);
GrxContext *grx_context_get_screen(void);

GrxContext *grx_context_ref(GrxContext *context);
void  grx_context_unref(GrxContext *context);
void  grx_context_resize_subcontext(GrxContext *context, gint x1, gint y1, gint x2, gint y2);
void  grx_context_set_current(const GrxContext *context);
void  grx_context_clear(GrxContext *context, GrxColor bg);

void grx_context_flood_spill(GrxContext *context, gint x1, gint y1, gint x2, gint y2, GrxColor old_c, GrxColor new_c);
void grx_context_flood_spill2(GrxContext *context, gint x1, gint y1, gint x2, gint y2, GrxColor old_c1, GrxColor new_c1, GrxColor old_c2, GrxColor new_c2);
GrxColor grx_context_get_pixel_at(GrxContext *context, gint x, gint y);
void grx_context_bit_blt(GrxContext *context, gint x, gint y, GrxContext *src, gint x1, gint y1, gint x2, gint y2, GrxColor op);
void grx_context_bit_blt_1bpp(GrxContext *context, gint x, gint y, GrxContext *src, gint x1, gint y1, gint x2, gint y2, GrxColor fg, GrxColor bg);

const GrxColor *grx_context_get_scanline(GrxContext *context, gint x1, gint x2, gint y, guint *n);

void  grx_set_clip_box(gint x1, gint y1, gint x2, gint y2);
void  grx_context_set_clip_box(GrxContext *context, gint x1, gint y1, gint x2, gint y2);
void  grx_get_clip_box(gint *x1p, gint *y1p, gint *x2p, gint *y2p);
void  grx_context_get_clip_box(const GrxContext *context, gint *x1p, gint *y1p, gint *x2p, gint *y2p);
void  grx_reset_clip_box(void);
void  grx_context_reset_clip_box(GrxContext *context);

int   grx_get_max_x(void);
int   grx_get_max_y(void);
int   grx_get_size_x(void);
int   grx_get_size_y(void);
int   grx_get_low_x(void);
int   grx_get_low_y(void);
int   grx_get_high_x(void);
int   grx_get_high_y(void);

#ifndef GRX_SKIP_INLINES
#define grx_context_new(w,h,m,c) (grx_context_new_full(grx_get_core_frame_mode(),w,h,m,c))
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
#define grx_context_get_clip_box(C,x1p,y1p,x2p,y2p) do {    \
    *(x1p) = (C)->x_clip_low;                               \
    *(y1p) = (C)->y_clip_low;                               \
    *(x2p) = (C)->x_clip_high;                              \
    *(y2p) = (C)->y_clip_high;                              \
} while(0)
#define grx_get_clip_box(x1p,y1p,x2p,y2p) do {              \
    *(x1p) = grx_get_low_x();                               \
    *(y1p) = grx_get_low_y();                               \
    *(x2p) = grx_get_high_x();                              \
    *(y2p) = grx_get_high_y();                              \
} while(0)
#endif  /* GRX_SKIP_INLINES */

#endif /* __GRX_CONTEXT_H__ */
