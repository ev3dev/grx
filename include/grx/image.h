/*
 * image.h
 *
 * Copyright (c) 2015 David Lechner <david@lechnology.com>
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
 *
 * By Michal Stencl Copyright (c) 1998 for GRX
 * <e-mail>    - [stenclpmd@ba.telecom.sk]
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

#ifndef __GRX_IMAGE_H__
#define __GRX_IMAGE_H__

#include <glib.h>

#include <grx/common.h>

/**
 * SECTION:image
 * @short_description: Image manipulation
 * @title: Image Manipulation
 * @section_id: image
 * @include: grx-3.0.h
 */

/**
 * GrxImage:
 *
 * Like pixmap patterns images are dependent on the actual video mode set.
 */
typedef GrxPixmap GrxImage;

/**
 * GrxImageMirrorFlags:
 * @GRX_IMAGE_MIRROR_HORIZONTAL: inverse left-right
 * @GRX_IMAGE_MIRROR_VERTICAL: inverse top-bottom
 *
 * Flags used by grx_image_mirror().
 */
typedef enum /*< flags >*/ {
    GRX_IMAGE_MIRROR_HORIZONTAL = 0x01,
    GRX_IMAGE_MIRROR_VERTICAL   = 0x02,
} GrxImageMirrorFlags;

GType grx_image_get_type(void);
GrxImage *grx_image_new(const guint8 *pixels, gint width, gint height,
                        const GrxColorTable colors);
GrxImage *grx_image_new_from_context(GrxContext *context);

GrxImage *grx_image_copy(GrxImage *image);
void grx_image_free(GrxImage *image);

GrxImage *grx_image_mirror(GrxImage *image, GrxImageMirrorFlags flags);
GrxImage *grx_image_stretch(GrxImage *image, gint new_width, gint new_height);

GrxPattern *grx_image_as_pattern(GrxImage *image);
GrxImage *grx_pattern_as_image(GrxPattern *pattern);

void grx_draw_image(gint x, gint y, GrxImage *image);
void grx_draw_image_tiled(gint x1, gint y1, gint x2, gint y2, GrxImage *image);
void grx_draw_filled_box_with_image(gint xo, gint yo, gint x1, gint y1,
                                    gint x2, gint y2, GrxImage *image);
void grx_draw_hline_with_image(gint xo, gint yo, gint x, gint y, gint width, GrxImage *image);
void grx_draw_pixel_with_image(gint xo, gint yo, gint x, gint y, GrxImage *image);


#ifndef GRX_SKIP_INLINES
#define grx_pattern_as_image(p) \
        (((p) && (p)->is_pixmap) ? (&(p)->pixmap) : NULL)
#define grx_image_new_from_context(c) \
        (GrxImage *)grx_pattern_new_pixmap_from_context(c)
#define grx_image_as_pattern(p) \
        (GrxPattern *)(p)
#define grx_image_copy(i)   \
        grx_pattern_copy((GrxPattern *)(i));
#define grx_image_free(i)   \
        grx_pattern_free((GrxPattern *)(i));
#endif

#endif /* __GRX_IMAGE_H__ */
