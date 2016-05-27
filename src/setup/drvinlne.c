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

GrxGraphicsMode (grx_get_current_graphics_mode)(void)
{
        return(grx_get_current_graphics_mode());
}

GrxVideoAdapter (grx_get_adapter_type)(void)
{
        return(grx_get_adapter_type());
}

GrxFrameMode (grx_get_current_frame_mode)(void)
{
        return(grx_get_current_frame_mode());
}

GrxFrameMode (grx_get_screen_frame_mode)(void)
{
        return(grx_get_screen_frame_mode());
}

GrxFrameMode (grx_get_core_frame_mode)(void)
{
        return(grx_get_core_frame_mode());
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

int (grx_get_n_planes)(void)
{
        return(grx_get_n_planes());
}

int (grx_get_line_offset)(int width)
{
        return(grx_get_line_offset(width));
}

long (grx_get_plane_size)(int w,int h)
{
        return(grx_get_plane_size(w,h));
}

long (grx_get_context_size)(int w,int h)
{
        return(grx_get_context_size(w,h));
}
