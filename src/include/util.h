/*
 * util.h ---- GRX library private include file
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
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

#ifndef __INCLUDE_UTIL_H__
#define __INCLUDE_UTIL_H__

#include <glib.h>

#include <grx/common.h>
#include <grx/frame_mode.h>

/*
 * internal utility functions
 */
G_GNUC_INTERNAL const GrxFrameDriver *_GrFindFrameDriver(GrxFrameMode mode);
G_GNUC_INTERNAL const GrxFrameDriver *_GrFindRAMframeDriver(GrxFrameMode mode);

G_GNUC_INTERNAL void _GrCloseVideoDriver(void);
G_GNUC_INTERNAL void _GrDummyFunction(void);

#endif /* __INCLUDE_UTIL_H__ */
