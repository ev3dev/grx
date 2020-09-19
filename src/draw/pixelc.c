/*
 * pixelc.c ---- pixel read from context
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu].
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

#include "clipping.h"
#include "libgrx.h"
#include "mouse.h"

/**
 * grx_context_get_pixel_at:
 * @context: the context
 * @x: the X coordinate
 * @y: the Y coordinate
 *
 * Gets the color value of the pixel in the context at the specified
 * coordinates.
 *
 * Also see grx_get_pixel_at() for operating on the current context.
 */
GrxColor grx_context_get_pixel_at(GrxContext *c, int x, int y)
{
    GrxColor retval;
    cxclip_dot_(c, x, y, return GRX_COLOR_NONE);
    mouse_block(c, x, y, x, y);
    retval = (*c->gc_driver->readpixel)(&c->frame, x + c->x_offset, y + c->y_offset);
    mouse_unblock();
    return retval;
}
