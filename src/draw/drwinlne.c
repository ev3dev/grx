/*
 * drvinlne.c ---- the drawing inline functions
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
 *
 */

#include <grx/context.h>
#include <grx/draw.h>
#include <grx/mode.h>

#include "libgrx.h"

/**
 * SECTION:draw
 * @short_description: Basic drawing functions
 * @title: Graphics Primitives
 * @section_id: draw
 * @include: grx-3.0.h
 *
 * These functions operate on the current context (or, occasionally, the screen).
 */

void (grx_draw_pixel_nc)(int x,int y,GrxColor c)
{
        grx_draw_pixel_nc(x,y,c);
}

GrxColor (grx_get_pixel_nc)(int x,int y)
{
        return(grx_get_pixel_nc(x,y));
}

GrxColor (grx_context_get_pixel_nc)(GrxContext *c,int x,int y)
{
        return(grx_context_get_pixel_nc(c,x,y));
}
