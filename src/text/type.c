/*
 * type.c ---- the font GType
 *
 * Copyright (c) 2015 David Lechner <david@lechnology.com>
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

G_DEFINE_BOXED_TYPE(GrxFont, grx_font, grx_font_copy, grx_font_free);
