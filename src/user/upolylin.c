/*
 * upolylin.c
 *
 * Copyright (C), Michael Goffioul
 * [goffioul@emic.ucl.ac.be]
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

#include "globals.h"
#include "libgrx.h"
#include "allocate.h"
#include "usercord.h"

/**
 * grx_user_draw_polyline:
 * @n_points: the number of points in @points
 * @points: (array length=n_points): an array of #GrxPoint
 * @c: the color
 *
 * Draw a multi-segment line on the current context that connects each point in
 * the @points array using the specified color.
 */
void grx_user_draw_polyline(int numpts,GrxPoint *points,GrxColor c)
{
        int pt;
        GrxPoint *tmp;
        setup_ALLOC();
        tmp = ALLOC(sizeof(GrxPoint) * numpts);

        if (tmp != NULL) {
          for ( pt = 0; pt < numpts; pt++) {
                tmp[pt] = points[pt];
                U2SX(tmp[pt].x,CURC);
                U2SY(tmp[pt].y,CURC);
          }
          grx_draw_polyline(numpts,tmp,c);
          FREE(tmp);
        }
        reset_ALLOC();
}
