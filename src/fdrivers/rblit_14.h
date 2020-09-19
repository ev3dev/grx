/*
 * rblit_14.h ---- ram to ram blit support functions for 1bpp and
 *                 4x1bpp RAM frame drivers
 *
 * Copyright (c) 1998 Hartmut Schirmer
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

#include <grdriver.h>

#include <glib.h>

#include <grx/color.h>
#include <grx/common.h>

G_GNUC_INTERNAL void _GR_rblit_14(GrxFrame *dst, gint dx, gint dy, GrxFrame *src,
    gint x, gint y, gint w, gint h, GrxColor op, _GR_blitFunc bitblt, gboolean invert);
