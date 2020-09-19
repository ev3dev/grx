/*
 * upfflood.c ---- pattern fill an arbitrary area in user coordinates
 *
 * Copyright (c) 1997 Hartmut Schirmer
 * [e-mail: hsc@techfak.uni-kiel.de]
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

#include <grx/pixmap.h>

#include "globals.h"
#include "libgrx.h"
#include "usercord.h"

/**
 * grx_user_flood_fill_with_pixmap:
 * @x: the starting X coordinate
 * @y: the starting Y coordinate
 * @border: the color of the border that contains the fill
 * @p: the pixmap of the fill
 *
 * Flood-fills the area of the current context bounded by the color @border
 * using @x, @y as the starting point.
 */
void grx_user_flood_fill_with_pixmap(int x, int y, GrxColor border, GrxPixmap *p)
{
    U2SX(x, CURC);
    U2SY(y, CURC);
    grx_flood_fill_with_pixmap(x, y, border, p);
}
