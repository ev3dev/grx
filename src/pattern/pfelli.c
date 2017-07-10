/*
 * pfelli.c
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
 *
 *  Copyright (C) 1992, Csaba Biegl
 *    820 Stirrup Dr, Nashville, TN, 37221
 *    csaba@vuse.vanderbilt.edu
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

#include "libgrx.h"
#include "shapes.h"

/**
 * grx_draw_filled_ellipse_with_pixmap:
 * @xc: the X coordinate of the center of the ellipse
 * @yc: the Y coordinate of the center of the ellipse
 * @rx: the radius in the X direction
 * @ry: the radius in the Y direction
 * @p: the pixmap
 *
 * Draws a filled ellipse on the current context using the specified pixmap.
 *
 * The ellipse can only draw ellipses with its major axis parallel with either
 * the X or Y coordinate axis
 */
void grx_draw_filled_ellipse_with_pixmap(int xc,int yc,int rx,int ry,GrxPixmap *p)
{
        GrFillArg fa;

        fa.p = p;
        _GrScanEllipse(xc,yc,rx,ry,&_GrPatternFiller,fa,TRUE);
}
