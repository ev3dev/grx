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

#ifndef __GRX_EXTENTS_H__
#define __GRX_EXTENTS_H__

#include <glib.h>
#include <glib-object.h>

#include <grx/color.h>
#include <grx/common.h>
#include <grx/context.h>

/**
 * SECTION:extents
 * @short_description: Size information about the current context
 * @title: Extents and clipping
 * @section_id: extents
 * @include: grx-3.0.h
 *
 * These functions are used to get information about the extents of the current
 * context. Additionally, the clipping limits used by drawing functions can be
 * set to an arbitrary area of the current context.
 */

int grx_get_width(void);
int grx_get_height(void);
int grx_get_max_x(void);
int grx_get_max_y(void);

void grx_set_clip_box(gint x1, gint y1, gint x2, gint y2);
void grx_get_clip_box(gint *x1, gint *y1, gint *x2, gint *y2);
void grx_reset_clip_box(void);

int grx_get_clip_box_min_x(void);
int grx_get_clip_box_min_y(void);
int grx_get_clip_box_max_x(void);
int grx_get_clip_box_max_y(void);

#ifndef GRX_SKIP_INLINES
#define grx_get_max_x()                 (grx_context_get_current()->x_max)
#define grx_get_max_y()                 (grx_context_get_current()->y_max)
#define grx_get_width()                (grx_get_max_x() + 1)
#define grx_get_height()                (grx_get_max_y() + 1)
#define grx_get_clip_box_min_x()        (grx_context_get_current()->x_clip_low)
#define grx_get_clip_box_min_y()                 (grx_context_get_current()->y_clip_low)
#define grx_get_clip_box_max_x()                (grx_context_get_current()->x_clip_high)
#define grx_get_clip_box_max_y()                (grx_context_get_current()->y_clip_high)
#define grx_get_clip_box(x1p,y1p,x2p,y2p) do {              \
    *(x1p) = grx_get_clip_box_min_x();                               \
    *(y1p) = grx_get_clip_box_min_y();                               \
    *(x2p) = grx_get_clip_box_max_x();                              \
    *(y2p) = grx_get_clip_box_max_y();                              \
} while(0)
#endif  /* GRX_SKIP_INLINES */

#endif /* __GRX_EXTENTS_H__ */
