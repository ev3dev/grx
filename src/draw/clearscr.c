/*
 * clearscr.c ---- clear screen
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
 * grx_clear_screen:
 * @bg: the background color
 *
 * Clears the screen using the specified background color.
 *
 * Thanks to the special #GrxColor definition, you can do more than simple clear
 * with this functions, by example with:
 * |[<!-- language="C" -->
 * grx_clear_screen (grx_color_to_xor_mode (grx_color_get_white ()));
 * ]|
 * the screen is negativized, do it again and the screen is restored.
 */
void grx_clear_screen(GrxColor bg)
{
    GrxContext save;
    grx_save_current_context(&save);
    grx_set_current_context(SCRN);
    grx_clear_context(bg);
    grx_set_current_context(&save);
}
