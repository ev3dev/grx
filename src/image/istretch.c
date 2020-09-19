/*
 * istretch.c ---- Source Image Utility
 *
 * Copyright (c) 1998 by Michal Stencl and Hartmut Schirmer
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

#include <grx/context.h>
#include <grx/pixmap.h>

#include "clipping.h"
#include "grdriver.h"
#include "image.h"
#include "libgrx.h"

/**
 * grx_pixmap_stretch:
 * @pixmap: the source pixmap
 * @new_width: the new width
 * @new_height: the new height
 *
 * Creates a new pixmap stretching @pixmap to @new_width by @new_height.
 *
 * Returns: (transfer full): the new pixmap
 */
GrxPixmap *grx_pixmap_stretch(GrxPixmap *p, gint nwidth, gint nheight)
{
    GrxContext ctx;
    GrxPixmap *img;

    GRX_ENTER();
    img = _GrImageAllocate(&ctx, nwidth, nheight);
    if (img)
        _GrFrDrvGenericStretchBlt(&ctx.frame, 0, 0, nwidth, nheight, &p->source, 0, 0,
            p->width, p->height, p->mode);
    GRX_RETURN(img);
}
