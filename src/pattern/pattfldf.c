/*
 * pattfldf.c ---- fill an arbitrary area with a pattern
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

#include "clipping.h"
#include "libgrx.h"
#include "shapes.h"

/**
 * grx_flood_fill_with_pixmap:
 * @x: the starting X coordinate
 * @y: the starting Y coordinate
 * @border: the color of the border that contains the fill
 * @p: the pixmap of the fill
 *
 * Flood-fills the area of the current context bounded by the color @border
 * using @x, @y as the starting point.
 */
void grx_flood_fill_with_pixmap(int x, int y, GrxColor border, GrxPixmap *p)
{
    GrFillArg fval;

    fval.p = p;
    _GrFloodFill(x, y, border, &_GrPatternFiller, fval);
}
