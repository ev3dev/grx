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

#include <grx/color.h>
#include <grx/common.h>

/**
 * SECTION:extents
 * @short_description: Size information about the current context and the screen
 * @title: Extents, viewports and clipping
 * @section_id: extents
 * @include: grx-3.0.h
 *
 * These functions are used to get information about the extents of the current
 * context and the screen. Additionally, the clipping limits used by drawing
 * functions can be set to an arbitrary area of the current context.
 */

gint grx_get_width(void);
gint grx_get_height(void);
gint grx_get_max_x(void);
gint grx_get_max_y(void);

gint grx_get_screen_width(void);
gint grx_get_screen_height(void);

gint grx_get_virtual_width(void);
gint grx_get_virtual_height(void);
gboolean grx_is_screen_virtual(void);

gint grx_get_viewport_x(void);
gint grx_get_viewport_y(void);
gboolean grx_set_viewport(gint x, gint y);

void grx_set_clip_box(gint x1, gint y1, gint x2, gint y2);
void grx_get_clip_box(gint *x1, gint *y1, gint *x2, gint *y2);
void grx_reset_clip_box(void);

gint grx_get_clip_box_min_x(void);
gint grx_get_clip_box_min_y(void);
gint grx_get_clip_box_max_x(void);
gint grx_get_clip_box_max_y(void);

#ifndef GRX_SKIP_INLINES
#define grx_get_max_x()                 (grx_context_get_current()->x_max)
#define grx_get_max_y()                 (grx_context_get_current()->y_max)
#define grx_get_width()                 (grx_get_max_x() + 1)
#define grx_get_height()                (grx_get_max_y() + 1)

#define grx_get_screen_width()          (grx_get_current_video_mode()->width)
#define grx_get_screen_height()         (grx_get_current_video_mode()->height)
#define grx_get_virtual_width()         (grx_get_virtual_video_mode()->width)
#define grx_get_virtual_height()        (grx_get_virtual_video_mode()->height)
#define grx_is_screen_virtual() \
    ((grx_get_screen_width() + grx_get_screen_height()) < (grx_get_virtual_width() + grx_get_virtual_height()))
#define grx_get_viewport_x()           (GrDriverInfo->vposx)
#define grx_get_viewport_y()           (GrDriverInfo->vposy)

#define grx_get_clip_box_min_x()        (grx_context_get_current()->x_clip_low)
#define grx_get_clip_box_min_y()        (grx_context_get_current()->y_clip_low)
#define grx_get_clip_box_max_x()        (grx_context_get_current()->x_clip_high)
#define grx_get_clip_box_max_y()        (grx_context_get_current()->y_clip_high)
#define grx_get_clip_box(x1p,y1p,x2p,y2p) do {      \
    *(x1p) = grx_get_clip_box_min_x();              \
    *(y1p) = grx_get_clip_box_min_y();              \
    *(x2p) = grx_get_clip_box_max_x();              \
    *(y2p) = grx_get_clip_box_max_y();              \
} while(0)
#endif  /* GRX_SKIP_INLINES */

#endif /* __GRX_EXTENTS_H__ */
