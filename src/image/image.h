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

#include <grx/context.h>
#include <grx/image.h>
#include <grx/pattern.h>

G_GNUC_INTERNAL int _GrImageTestSize(int wdt,int hgt);
G_GNUC_INTERNAL GrxImage *_GrImageAllocate(GrxContext *ctx, int nwidth,int nheight);

#endif
