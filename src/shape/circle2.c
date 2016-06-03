/*
 * circle2.c ---- draw ellipse arc
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

#include <grx/draw.h>

#include "libgrx.h"
#include "allocate.h"
#include "shapes.h"

/**
 * grx_draw_ellipse_arc:
 * @xc: the X coordinate of the center of the arc
 * @yc: the Y coordinate of the center of the arc
 * @rx: the radius in the X direction
 * @ry: the radius in the Y direction
 * @start: the starting angle in 1/10ths of degrees
 * @end: the ending angle in 1/10ths of degrees
 * @style: the arc style
 * @c: the color
 *
 * Draws an arc on the current context centered at the specified coordinates
 * from the starting angle to the ending angle with the specified radii,
 * arc style and color.
 */
void grx_draw_ellipse_arc(int xc,int yc,int rx,int ry,int start,int end,GrxArcStyle style,GrxColor c)
{
    GrxPoint *pnts;
    setup_ALLOC();
    pnts = ALLOC(sizeof(GrxPoint) * (GRX_MAX_ELLIPSE_POINTS + 1));
    if (pnts != NULL)
    {
        GrFillArg fval;
        int npts  = grx_generate_ellipse_arc(xc,yc,rx,ry,start,end,pnts);
        int close = FALSE;
        switch(style) {
          case GRX_ARC_STYLE_OPEN:
            break;
          case GRX_ARC_STYLE_CLOSED_RADIUS:
            pnts[npts].x = xc;
            pnts[npts].y = yc;
            npts++;
          case GRX_ARC_STYLE_CLOSED_CHORD:
            close = TRUE;
            break;
        }
        fval.color = c;
        _GrDrawPolygon(npts,pnts,&_GrSolidFiller,fval,close);
        FREE(pnts);
    }
    reset_ALLOC();
}
