/*
 * scanline_buf.h ---- common scan line buffer
 *
 * Copyright (c) 2020 David Lechner <david@lechnology.com>
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

#include <glib.h>

#include <grx/color.h>

G_GNUC_INTERNAL GrxColor *_grx_get_scanline_buf(gint n_colors);
