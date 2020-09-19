/*
 * mouinlne.c ---- the mouse inline functions
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

#include <grx/context.h>
#include <grx/mouse.h>

/**
 * grx_mouse_get_cursor:
 *
 * Gets the current mouse cursor.
 *
 * Returns: (transfer none) (nullable): the cursor or %NULL if the cursor has
 * not been set.
 */
GrxCursor *(grx_mouse_get_cursor)(void)
{
    return grx_mouse_get_cursor();
}

/**
 * grx_mouse_is_cursor_shown:
 *
 * Gets the mouse cursor visibility.
 *
 * Returns: %TRUE if the mouse cursor is shown on the screen.
 */
int(grx_mouse_is_cursor_shown)(void)
{
    return grx_mouse_is_cursor_shown();
}

/**
 * grx_mouse_block:
 * @context: (nullable): the target content for %NULL for the global context
 * @x1: the left x coordinate
 * @y1: the top y coordinate
 * @x2: the right x coordinate
 * @y2: the bottom y coordinate
 *
 * Erase the cursor before drawing. Must call grx_mouse_unblock() when finished.
 *
 * Normally, you don't need to call this function manually, but it can be more
 * efficient to do so if there is a long sequence of drawing functions.
 *
 * Returns: state flags that need to be passed to grx_mouse_unblock()
 */
unsigned int(grx_mouse_block)(GrxContext *c, int x1, int y1, int x2, int y2)
{
    return grx_mouse_block(c, x1, y1, x2, y2);
}

/**
 * grx_mouse_unblock:
 * @flags: flags returned by grx_mouse_block()
 *
 * Restore the mouse cursor after a call to grx_mouse_block().
 */
void(grx_mouse_unblock)(unsigned int flags)
{
    grx_mouse_unblock(flags);
}
