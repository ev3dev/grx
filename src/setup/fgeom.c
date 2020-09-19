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
 */

#include <grx/mode.h>

#include "arith.h"
#include "libgrx.h"
#include "util.h"

/**
 * grx_frame_mode_get_bpp:
 * @mode: a #GrxFrameMode
 *
 * Gets the color depth of the frame mode in bits per pixel.
 *
 * Returns: the color depth or -1 if the frame mode was not valid
 */
gint grx_frame_mode_get_bpp(GrxFrameMode mode)
{
    switch (mode) {
    case GRX_FRAME_MODE_LFB_MONO01:
    case GRX_FRAME_MODE_LFB_MONO10:
    case GRX_FRAME_MODE_RAM_1BPP:
        return 1;
    case GRX_FRAME_MODE_LFB_2BPP:
    case GRX_FRAME_MODE_RAM_2BPP:
        return 2;
    case GRX_FRAME_MODE_LFB_8BPP:
    case GRX_FRAME_MODE_RAM_8BPP:
        return 8;
    case GRX_FRAME_MODE_LFB_16BPP:
    case GRX_FRAME_MODE_RAM_16BPP:
        return 16;
    case GRX_FRAME_MODE_LFB_24BPP:
    case GRX_FRAME_MODE_RAM_24BPP:
        return 24;
    case GRX_FRAME_MODE_LFB_32BPP_LOW:
    case GRX_FRAME_MODE_LFB_32BPP_HIGH:
    case GRX_FRAME_MODE_RAM_32BPP_LOW:
    case GRX_FRAME_MODE_RAM_32BPP_HIGH:
        return 32;
    default:
        return -1;
    }
}

/**
 * grx_frame_mode_get_line_offset:
 * @mode: a #GrxFrameMode
 * @width: the width in pixels
 *
 * Gets the offset in bytes for a row of @width pixels.
 *
 * Returns: the number of bytes per row.
 */
gint grx_frame_mode_get_line_offset(GrxFrameMode mode, gint width)
{
    GrxFrameDriver *dp = _GrFindRAMframeDriver(mode);
    if (dp) {
        unsigned int w = (unsigned int)width * dp->bits_per_pixel;
        w = ((w + 7) >> 3);
        w = ((w + dp->row_align - 1) / dp->row_align) * dp->row_align;
        return w;
    }
    return 0;
}

/**
 * grx_frame_mode_get_memory_size:
 * @mode: a #GrxFrameMode
 * @width: the width in pixels
 * @height: the height in pixels
 *
 * Gets the memory size in bytes required to store an image of @width x @height
 * for this frame mode.
 *
 * Returns: the size in bytes.
 */
gint grx_frame_mode_get_memory_size(GrxFrameMode mode, gint width, gint height)
{
    return umul32(grx_frame_mode_get_line_offset(mode, width), height);
}

/**
 * grx_frame_mode_get_context_size:
 * @mode: a #GrxFrameMode
 * @width: the width in pixels
 * @height: the height in pixels
 *
 * Gets the total size in bytes required to store an image of @width x @height
 * for this frame mode.
 *
 * Returns: the size in bytes.
 */
gint grx_frame_mode_get_context_size(GrxFrameMode mode, gint width, gint height)
{
    return umul32(grx_frame_mode_get_line_offset(mode, width), height);
}
