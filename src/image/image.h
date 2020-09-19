/*
 * image.h ---- Source Image Utility
 *
 * by Michal Stencl Copyright (c) 1998
 * <e-mail>    - [stenclpmd@ba.telecom.sk]
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
 * modifications by Hartmut Schirmer (c) 1998
 */

#ifndef __IMAGE_H_INCLUDED__
#define __IMAGE_H_INCLUDED__

#include <glib.h>

#include <grx/common.h>

G_GNUC_INTERNAL int _GrImageTestSize(gint wdt, gint hgt);
G_GNUC_INTERNAL GrxPixmap *_GrImageAllocate(GrxContext *ctx, gint nwidth, gint nheight);

#endif
