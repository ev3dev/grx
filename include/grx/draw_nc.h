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

void grx_draw_pixel_nc(gint x, gint y, GrxColor c);
void grx_draw_line_nc(gint x1, gint y1, gint x2, gint y2,GrxColor c);
void grx_draw_hline_nc(gint x1, gint x2, gint y, GrxColor c);
void grx_draw_vline_nc(gint x, gint y1, gint y2, GrxColor c);
void grx_draw_box_nc(gint x1, gint y1, gint x2, gint y2, GrxColor c);
void grx_draw_filled_box_nc(gint x1, gint y1, gint x2, gint y2, GrxColor c);
void grx_draw_framed_box_nc(gint x1, gint y1, gint x2, gint y2, gint wdt, const GrxFramedBoxColors *c);
void grx_bit_blt_nc(GrxContext *dst, gint x, gint y, GrxContext *src, gint x1, gint y1, gint x2, gint y2, GrxColor op);

GrxColor grx_get_pixel_nc(gint x, gint y);
GrxColor grx_context_get_pixel_nc(GrxContext *c, gint x, gint y);

#ifndef GRX_SKIP_INLINES
#define grx_draw_pixel_nc(x,y,c) (                                           \
        (*grx_get_current_frame_driver()->drawpixel)(                          \
        ((x) + grx_context_get_current()->x_offset),                           \
        ((y) + grx_context_get_current()->y_offset),                           \
        ((c))                                                                  \
        )                                                                      \
)
#define grx_get_pixel_nc(x,y) (                                              \
        (*grx_get_current_frame_driver()->readpixel)(                          \
        (GrxFrame *)(&grx_context_get_current()->frame),                       \
        ((x) + grx_context_get_current()->x_offset),                           \
        ((y) + grx_context_get_current()->y_offset)                            \
        )                                                                      \
)
#define grx_context_get_pixel_nc(c,x,y) (                                    \
        (*(c)->gc_driver->readpixel)(                                          \
        (&(c)->frame),                                                         \
        ((x) + (c)->x_offset),                                                 \
        ((y) + (c)->y_offset)                                                  \
        )                                                                      \
)
#endif  /* GRX_SKIP_INLINES */

#endif /* __GRX_DRAW_NC_H__ */
