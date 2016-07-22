/*
 * clearclp.c ---- clear clipbox
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

#include <grx/draw.h>

#include "globals.h"
#include "libgrx.h"

/**
 * grx_clear_clip_box:
 * @bg: the background color
 *
 * Clears the current clip box using the specified background color.
 *
 * Thanks to the special #GrxColor definition, you can do more than simple clear
 * with this functions, by example with:
 * |[<!-- language="C" -->
 * grx_clear_clip_box (grx_color_to_xor_mode (grx_color_info_get_white ()));
 * ]|
 * the clip box is negativized, do it again and the clip box is restored.
 */
void grx_clear_clip_box(GrxColor bg)
{
        grx_draw_filled_box(0,0,CURC->x_max,CURC->y_max,bg);
}
