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
 */

#include <glib-object.h>

#include <grx/color.h>
#include <grx/image.h>

#include "libgrx.h"
#include "image.h"

G_DEFINE_BOXED_TYPE(GrxImage, grx_image, grx_image_copy, grx_image_free);

/**
 * grx_image_new_from_context:
 * @context: (transfer none): the context
 *
 * Creates a new image using @context.
 *
 * The image and the original context share the drawing RAM.
 *
 * Returns: (transfer full): the new image or %NULL if there was an error
 */
GrxImage *(grx_image_new_from_context)(GrxContext *context) {
    return grx_image_new_from_context(context);
}

/**
 * grx_image_copy:
 * @image: (transfer none): the image to copy
 *
 * FIXME: this is not implemented and calling will cause the program to abort.
 *
 * Returns: (transfer full): a copy of the image
 */
GrxImage *(grx_image_copy)(GrxImage *image) {
    return grx_image_copy(image);
}

/**
 * grx_image_free:
 * @image: (transfer full): the image to free
 *
 * Frees all memory allocated by the image.
 */
void (grx_image_free)(GrxImage *image) {
    grx_image_free(image);
}
