/*
 * fgeom.c ---- frame geometry and memory allocation utilities
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
 *
 */

#include <grx/mode.h>

#include "libgrx.h"
#include "arith.h"
#include "util.h"

/**
 * grx_frame_mode_get_n_planes:
 * @mode: a #GrxFrameMode
 *
 * Returns: The number of planes.
 */
gint grx_frame_mode_get_n_planes(GrxFrameMode mode)
{
    GrxFrameDriver *dp = _GrFindRAMframeDriver(mode);
    return(dp ? dp->num_planes : 0);
}

gint grx_frame_mode_get_line_offset(GrxFrameMode mode, gint width)
{
    GrxFrameDriver *dp = _GrFindRAMframeDriver(mode);
    if(dp) {
        unsigned int w = (unsigned int)width 
                       * (dp->bits_per_pixel / dp->num_planes);
        w = ((w + 7) >> 3);
        w = ((w + dp->row_align - 1) / dp->row_align) * dp->row_align;
        return(w);
    }
    return(0);
}

glong grx_frame_mode_get_plane_size(GrxFrameMode mode, gint width, gint height)
{
        return(umul32(grx_frame_mode_get_line_offset(mode, width), height));
}

glong grx_frame_mode_get_context_size(GrxFrameMode mode, gint width, gint height)
{
    return(umul32(grx_frame_mode_get_line_offset(mode, width),
                  (grx_frame_mode_get_n_planes(mode) * height)));
}
