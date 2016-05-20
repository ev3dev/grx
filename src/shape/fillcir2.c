/*
 * fillcir2.c ---- filled circle arc
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
 *
 */

#include "libgrx.h"
#include "shapes.h"

void grx_draw_filled_circle_arc(int xc,int yc,int r,int start,int end,GrxArcStyle style,GrxColor c)
{
        grx_draw_filled_ellipse_arc(xc,yc,r,r,start,end,style,c);
}
