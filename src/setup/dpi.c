/*
 * dpi.c - display resolution information
 *
 * Copyright (c) 2017 David Lechner <david@lechnology.com>
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

#include <grx/mode.h>

/**
 * grx_get_dpi:
 *
 * Gets the display resolution of the current graphics mode or #GRX_DEFAULT_DPI
 * if the current driver does not support getting the resolution.
 *
 * Returns: the resolution
 */
guint grx_get_dpi(void)
{
    if (GrDriverInfo->dpi) {
        return GrDriverInfo->dpi;
    }

    if (!GrDriverInfo->vdriver || !GrDriverInfo->vdriver->get_dpi) {
        return GRX_DEFAULT_DPI;
    }

    return GrDriverInfo->vdriver->get_dpi(GrDriverInfo->vdriver);
}
