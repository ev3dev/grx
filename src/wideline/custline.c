/*
 * custline.c ---- wide and/or dashed line
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

#include <grx/draw.h>
#include <grx/wideline.h>

#include "libgrx.h"
#include "shapes.h"

void grx_draw_line_with_options(int x1,int y1,int x2,int y2,const GrxLineOptions *o)
{
        GrFillArg fval;
        GrxPoint pt[2];
        pt[0].x = x1; pt[0].y = y1;
        pt[1].x = x2; pt[1].y = y2;
        fval.color = o->color;
        _GrDrawCustomPolygon(2,pt,o,&_GrSolidFiller,fval,FALSE,FALSE);
}
