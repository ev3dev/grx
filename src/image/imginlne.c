/*
 * imginlne.h ---- Image Utility
 *
 * by Michal Stencl Copyright (c) 1998 for GRX
 *  <e-mail>      - [stenclpmd@ba.telecom.sk]
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

#include "libgrx.h"
#include "image.h"

G_DEFINE_BOXED_TYPE(GrxImage, grx_image, grx_image_copy, grx_image_free);

/**
 * grx_image_create_from_pattern:
 * @pattern:
 *
 * Converts a #GrxPattern to a #GrxImage.
 *
 * Returns: (transfer full): #NULL if the @pattern given is not a #GrxPixmap
 */
GrxImage *(grx_image_create_from_pattern)(GrxPattern *pattern) {
    return grx_image_create_from_pattern(pattern);
}

/**
 * grx_image_create_from_context:
 * @context:
 *
 * Returns: (transfer full):
 */
GrxImage *(grx_image_create_from_context)(GrxContext *context) {
    return grx_image_create_from_context(context);
}

/**
 * grx_pattern_create_from_image:
 * @image:
 *
 * Returns: (transfer full):
 */
GrxPattern *(grx_pattern_create_from_image)(GrxImage *image) {
    return grx_pattern_create_from_image(image);
}

/**
 * grx_image_create_from_data:
 * @pixels: (array):
 * @width:
 * @height:
 * @colors:
 *
 * Returns: (transfer full):
 */
GrxImage *(grx_image_create_from_data)(const unsigned char *pixels, int width,
                                       int height, const GrxColorTable colors)
{
    return grx_image_create_from_data(pixels, width, height, colors);
}

/**
 * grx_image_copy:
 * @image: (transfer none): the image to copy
 *
 * Returns: (transfer full): a copy of the image
 */
GrxImage *(grx_image_copy)(GrxImage *image) {
    grx_image_copy(image);
}

/**
 * grx_image_free:
 * @image: (transfer full): the image to free
 */
void (grx_image_free)(GrxImage *image) {
    grx_image_free(image);
}
