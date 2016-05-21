/*
 * cellia.c ---- draw dashed and/or wide ellipse arcs
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
#include "allocate.h"
#include "shapes.h"

void grx_draw_ellipse_arc_with_options(int xc,int yc,int xa,int ya,int start,int end,GrxArcStyle style,const GrxLineOptions *o)
{
    int (*pnts)[2];
    setup_ALLOC();
    pnts = ALLOC(sizeof(int) * 2 * (GRX_MAX_ELLIPSE_POINTS+1));
    if (pnts != NULL)
    {
        GrFillArg fval;
        int npts  = grx_generate_ellipse_arc(xc,yc,xa,ya,start,end,pnts);
        int close = FALSE;
        switch(style) {
          case GRX_ARC_STYLE_CLOSED_RADIUS:
            pnts[npts][0] = xc;
            pnts[npts][1] = yc;
            npts++;
          case GRX_ARC_STYLE_CLOSED_CHORD:
            close = TRUE;
            break;
        }
        fval.color = o->color;
        _GrDrawCustomPolygon(npts,pnts,o,&_GrSolidFiller,fval,close,TRUE);
        FREE(pnts);
    }
    reset_ALLOC();
}
