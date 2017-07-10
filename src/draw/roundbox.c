/*
 * roundbox.c ---- draw a rectangle with round corners
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

#include <grx/draw.h>

/**
 * grx_draw_rounded_box:
 * @x1: the left X coordinate
 * @y1: the top Y coordinate
 * @x2: the right X coordinate
 * @y2: the bottom Y coordinate
 * @r: the radius of the corners
 * @c: the color
 *
 * Draws a rectangle with rounded corners on the current context using the
 * specified coordinates and color.
 */
void grx_draw_rounded_box (gint x1, gint y1, gint x2, gint y2, gint r, GrxColor c)
{
    grx_draw_circle_arc (x1 + r, y1 + r, r, 900, 1800, GRX_ARC_STYLE_OPEN, c);
    grx_draw_circle_arc (x2 - r, y1 + r, r, 0, 900, GRX_ARC_STYLE_OPEN, c);
    grx_draw_circle_arc (x1 + r, y2 - r, r, 1800, 2700, GRX_ARC_STYLE_OPEN, c);
    grx_draw_circle_arc (x2 - r, y2 - r, r, 2700, 3600, GRX_ARC_STYLE_OPEN, c);
    grx_draw_hline (x1 + r, x2 - r, y1, c);
    grx_draw_hline (x1 + r, x2 - r, y2, c);
    grx_draw_vline (x1, y1 + r, y2 - r, c);
    grx_draw_vline (x2, y1 + r, y2 - r, c);
}

/**
 * grx_draw_filled_rounded_box:
 * @x1: the left X coordinate
 * @y1: the top Y coordinate
 * @x2: the right X coordinate
 * @y2: the bottom Y coordinate
 * @r: the radius of the corners
 * @c: the color
 *
 * Draws a filled rectangle with rounded corners on the current context using
 * the specified coordinates and color.
 */
void grx_draw_filled_rounded_box (gint x1, gint y1, gint x2, gint y2, gint r, GrxColor c)
{
    grx_draw_filled_circle_arc (x1 + r, y1 + r, r, 900, 1800, GRX_ARC_STYLE_CLOSED_RADIUS, c);
    grx_draw_filled_circle_arc (x2 - r, y1 + r, r, 0, 900, GRX_ARC_STYLE_CLOSED_RADIUS, c);
    grx_draw_filled_circle_arc (x1 + r, y2 - r, r, 1800, 2700, GRX_ARC_STYLE_CLOSED_RADIUS, c);
    grx_draw_filled_circle_arc (x2 - r, y2 - r, r, 2700, 3600, GRX_ARC_STYLE_CLOSED_RADIUS, c);
    grx_draw_filled_box (x1 + r, y1, x2 - r, y2, c);
    grx_draw_filled_box (x1, y1 + r, x2, y2 - r, c);
}
