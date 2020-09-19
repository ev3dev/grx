/*
 * genpoints.c - generate points for a polyline/polygon
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

#include <grx/draw.h>

/**
 * grx_generate_points:
 * @coords: (element-type gint): the coordinate pairs
 *
 * Creates an array of points.
 *
 * This function is intended for use with language bindings that don't have
 * an elegant way to create an array of points. Each pair of coordinates is
 * converted into a point, so the returned array will be 1/2 the length of
 * @coords.
 *
 * Returns: (element-type GrxPoint) (transfer full): a new #GArray
 */
GArray *grx_generate_points(GArray *coords)
{
    GArray *points;
    guint n_points;
    gint i;

    g_return_val_if_fail(coords != NULL, NULL);

    n_points = coords->len / 2;
    points = g_array_sized_new(FALSE, FALSE, sizeof(GrxPoint), n_points);

    for (i = 0; i < n_points; i++) {
        GrxPoint p = {
            .x = g_array_index(coords, gint, i * 2),
            .y = g_array_index(coords, gint, i * 2 + 1),
        };
        g_array_append_val(points, p);
    }

    return points;
}
