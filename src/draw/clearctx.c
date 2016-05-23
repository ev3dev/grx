/*
 * clearctx.c ---- clear context
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
 * Contributions by:
 * 081105 Richard, grx_context_clear
 *
 */

#include <grx/draw.h>
#include <grx/draw_nc.h>

#include "mouse.h"
#include "libgrx.h"

/**
 * grx_clear_context:
 * @bg: the background color
 *
 * Clears the current context using the specified background color.
 *
 * Thanks to the special GrColor definition, you can do more than simple clear
 * with this functions, by example with:
 * |[<!-- language="C" -->
 * grx_clear_context (grx_color_to_xor_mode (grx_color_info_get_white ()));
 * ]|
 * the context is negativized, do it again and the context is restored.
 */
void grx_clear_context(GrxColor bg)
{
  mouse_block(CURC,0,0,CURC->x_max,CURC->y_max);
  grx_fast_draw_filled_box(0,0,CURC->x_max,CURC->y_max,bg);
  mouse_unblock();
}

/**
 * grx_context_clear:
 * @context: the context
 * @bg: the background color
 *
 * Clears the specified context using the specified background color.
 *
 * Thanks to the special GrColor definition, you can do more than simple clear
 * with this functions, by example with:
 * |[<!-- language="C" -->
 * grx_context_clear (ctx, grx_color_to_xor_mode (grx_color_info_get_white ()));
 * ]|
 * the context is negativized, do it again and the context is restored.
 *
 * Also see grx_clear_context() for clearing the current context and
 * grx_clear_screen() for clearing the screen.
 */
void grx_context_clear(GrxContext *ctx, GrxColor bg)
{
  GrxContext s;
  grx_context_save(&s);
  grx_context_set_current(ctx);
  grx_clear_context(bg);
  grx_context_set_current(&s);
}
