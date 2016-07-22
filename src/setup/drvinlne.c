/*
 * drvinlne.c ---- inline functions related to drivers
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

#include <grx/extents.h>
#include <grx/mode.h>

#include "libgrx.h"

/**
 * grx_get_mode:
 *
 * Gets the current graphics mode.
 *
 * Returns: the mode.
 */
GrxGraphicsMode (grx_get_mode)(void)
{
        return(grx_get_mode());
}

/**
 * grx_frame_mode_get_current:
 *
 * Get the frame mode of the current context.
 *
 * Returns: the frame mode.
 */
GrxFrameMode (grx_frame_mode_get_current)(void)
{
        return(grx_frame_mode_get_current());
}

/**
 * grx_frame_mode_get_screen:
 *
 * Get the frame mode of the screen.
 *
 * Returns: the frame mode.
 */
GrxFrameMode (grx_frame_mode_get_screen)(void)
{
        return(grx_frame_mode_get_screen());
}

/**
 * grx_frame_mode_get_screen_core:
 *
 * Get the compatible RAM frame mode of the screen.
 *
 * Returns: the frame mode.
 */
GrxFrameMode (grx_frame_mode_get_screen_core)(void)
{
        return(grx_frame_mode_get_screen_core());
}

const GrxVideoDriver *(grx_get_current_video_driver)(void)
{
        return(grx_get_current_video_driver());
}

const GrxVideoMode *(grx_get_current_video_mode)(void)
{
        return(grx_get_current_video_mode());
}

const GrxVideoMode *(grx_get_virtual_video_mode)(void)
{
        return(grx_get_virtual_video_mode());
}

const GrxFrameDriver *(grx_get_current_frame_driver)(void)
{
        return(grx_get_current_frame_driver());
}

const GrxFrameDriver *(grx_get_screen_frame_driver)(void)
{
        return(grx_get_screen_frame_driver());
}

/**
 * grx_get_device_manager:
 *
 * Gets the device manager for the current video driver
 *
 * Returns: (transfer none) (nullable): the #GrxDeviceManager or %NULL
 * if a video driver has not be loaded yet
 */
GrxDeviceManager *(grx_get_device_manager)(void)
{
    return grx_get_device_manager ();
}


/**
 * grx_get_screen_width:
 *
 * Gets the width of the physical screen in pixels.
 *
 * Returns: the width
 */
int (grx_get_screen_width)(void)
{
        return(grx_get_screen_width());
}

/**
 * grx_get_screen_height:
 *
 * Gets the height of the physical screen in pixels.
 *
 * Returns: the height
 */
int (grx_get_screen_height)(void)
{
        return(grx_get_screen_height());
}

/**
 * grx_get_virtual_width:
 *
 * Gets the width of the virtual screen in pixels.
 *
 * Returns: the width
 */
int (grx_get_virtual_width)(void)
{
        return(grx_get_virtual_width());
}

/**
 * grx_get_virtual_height:
 *
 * Gets the height of the virtual screen in pixels.
 *
 * Returns: the height
 */
int (grx_get_virtual_height)(void)
{
        return(grx_get_virtual_height());
}

/**
 * grx_get_viewport_x:
 *
 * Gets the starting X coordinate of the current viewport.
 *
 * Returns: the X coordinate
 */
int (grx_get_viewport_x)(void)
{
        return(grx_get_viewport_x());
}

/**
 * grx_get_viewport_y:
 *
 * Gets the starting Y coordinate of the current viewport.
 *
 * Returns: the Y coordinate
 */
int (grx_get_viewport_y)(void)
{
        return(grx_get_viewport_y());
}

/**
 * grx_is_screen_virtual:
 *
 * Checks if the current screen is a virtual screen (i.e. the virtual size is
 * larger than the physical size).
 *
 * Returns: %TRUE if the screen is a virtual screen
 */
int (grx_is_screen_virtual)(void)
{
        return(grx_is_screen_virtual());
}

/**
 * grx_screen_get_n_planes:
 *
 * Gets the number of planes for the screen's RAM frame mode.
 *
 * Returns: The number of planes.
 */
int (grx_screen_get_n_planes)(void)
{
    return(grx_screen_get_n_planes());
}

/**
 * grx_screen_get_line_offset:
 * @width: the width in pixels
 *
 * Gets the offset in bytes for a row of @width pixels on the screen.
 *
 * Returns: the number of bytes per row.
 */
int (grx_screen_get_line_offset)(int width)
{
    return(grx_screen_get_line_offset(width));
}

/**
 * grx_screen_get_plane_size:
 * @width: the width in pixels
 * @height: the height in pixels
 *
 * Gets the plane size in bytes required to store an image of @width x @height
 * for the screen.
 *
 * Returns: the size in bytes.
 */
long (grx_screen_get_plane_size)(int w,int h)
{
    return(grx_screen_get_plane_size(w,h));
}

/**
 * grx_screen_get_context_size:
 * @width: the width in pixels
 * @height: the height in pixels
 *
 * Gets the total size in bytes required to store an image of @width x @height
 * for the screen.
 *
 * Returns: the size in bytes.
 */
long (grx_screen_get_context_size)(int w,int h)
{
    return(grx_screen_get_context_size(w,h));
}
