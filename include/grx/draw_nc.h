/*
 * draw_nc.h
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

#ifndef __GRX_DRAW_NC_H__
#define __GRX_DRAW_NC_H__

#include <glib.h>

#include <grx/color.h>
#include <grx/common.h>

/* ================================================================== */
/*                 NON CLIPPING DRAWING PRIMITIVES                    */
/* ================================================================== */

void grx_fast_draw_pixel(gint x, gint y, GrxColor c);
void grx_fast_draw_line(gint x1, gint y1, gint x2, gint y2,GrxColor c);
void grx_fast_draw_hline(gint x1, gint x2, gint y, GrxColor c);
void grx_fast_draw_vline(gint x, gint y1, gint y2, GrxColor c);
void grx_fast_draw_box(gint x1, gint y1, gint x2, gint y2, GrxColor c);
void grx_fast_draw_filled_box(gint x1, gint y1, gint x2, gint y2, GrxColor c);
void grx_fast_draw_framed_box(gint x1, gint y1, gint x2, gint y2, gint wdt, const GrxFramedBoxColors *c);
void grx_fast_bit_blt(GrxContext *dst, gint x, gint y, GrxContext *src, gint x1, gint y1, gint x2, gint y2, GrxColor op);

GrxColor grx_fast_get_pixel(gint x, gint y);
GrxColor grx_context_fast_get_pixel(GrxContext *c, gint x, gint y);

#ifndef GRX_SKIP_INLINES
#define grx_fast_draw_pixel(x,y,c) (                                           \
        (*grx_get_current_frame_driver()->drawpixel)(                          \
        ((x) + grx_context_get_current()->x_offset),                           \
        ((y) + grx_context_get_current()->y_offset),                           \
        ((c))                                                                  \
        )                                                                      \
)
#define grx_fast_get_pixel(x,y) (                                              \
        (*grx_get_current_frame_driver()->readpixel)(                          \
        (GrxFrame *)(&grx_context_get_current()->frame),                       \
        ((x) + grx_context_get_current()->x_offset),                           \
        ((y) + grx_context_get_current()->y_offset)                            \
        )                                                                      \
)
#define grx_context_fast_get_pixel(c,x,y) (                                    \
        (*(c)->gc_driver->readpixel)(                                          \
        (&(c)->frame),                                                         \
        ((x) + (c)->x_offset),                                                 \
        ((y) + (c)->y_offset)                                                  \
        )                                                                      \
)
#endif  /* GRX_SKIP_INLINES */

#endif /* __GRX_DRAW_NC_H__ */
