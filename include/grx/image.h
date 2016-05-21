/*
 * image.h
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

#ifndef __GRX_IMAGE_H__
#define __GRX_IMAGE_H__

#include <glib.h>

#include <grx/pattern.h>

/* ================================================================== */
/*                      IMAGE MANIPULATION                            */
/* ================================================================== */

/*
 *  by Michal Stencl Copyright (c) 1998 for GRX
 *  <e-mail>    - [stenclpmd@ba.telecom.sk]
 */

/**
 * GrxImage:
 *
 * Like pixmap patterns images are dependent of the actual video mode set.
 */
typedef GrxPixmap GrxImage;

typedef enum /*< flags >*/ {
    GRX_IMAGE_MIRROR_HORIZONTAL = 0x01,  /* inverse left right */
    GRX_IMAGE_MIRROR_VERTICAL   = 0x02,  /* inverse top down */
} GrxImageMirrorFlags;

GType grx_image_get_type(void);
GrxImage *grx_image_create(const guint8 *pixels, gint width, gint height,
                           const GrxColorTable colors);
GrxImage *grx_image_copy(GrxImage *image);
void grx_image_free(GrxImage *image);
void grx_draw_image(gint x, gint y, GrxImage *image);
void grx_draw_image_tiled(gint x1, gint y1, gint x2, gint y2, GrxImage *image);
void grx_draw_filled_box_with_image(gint xo, gint yo, gint x1, gint y1,
                                    gint x2, gint y2, GrxImage *image);
void grx_draw_hline_with_image(gint xo, gint yo, gint x, gint y, gint width, GrxImage *image);
void grx_draw_pixel_with_image(gint xo, gint yo, gint x, gint y, GrxImage *image);

GrxImage *grx_image_mirror(GrxImage *image, GrxImageMirrorFlags flags);
GrxImage *grx_image_stretch(GrxImage *image, gint new_width, gint new_height);

GrxImage *grx_image_create_from_pattern(GrxPattern *pattern);
GrxImage *grx_image_create_from_context(GrxContext *context);
GrxImage *grx_image_create_from_data(const guint8 *pixels, gint width, gint height,
                                     const GrxColorTable colors);

GrxPattern *grx_pattern_create_from_image(GrxImage *image);


#ifndef GRX_SKIP_INLINES
#define grx_image_create_from_pattern(p) \
        (((p) && (p)->is_pixmap) ? (&(p)->pixmap) : NULL)
#define grx_image_create_from_context(c) \
        (GrxImage *)grx_pattern_create_pixmap_from_context(c)
#define grx_pattern_create_from_image(p) \
        (GrxPattern *)(p)
#define grx_image_create_from_data(pixels,w,h,colors) \
        (GrxImage *)grx_pattern_create_pixmap(pixels,w,h,colors);
#define grx_image_copy(i)   \
        grx_pattern_copy((GrxPattern *)(i));
#define grx_image_free(i)   \
        grx_pattern_free((GrxPattern *)(i));
#endif

#endif /* __GRX_IMAGE_H__ */
