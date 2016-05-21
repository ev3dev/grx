/*
 * drawpoly.c ---- draw the outline of a polygon
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

#include "globals.h"
#include "libgrx.h"
#include "shapes.h"
#include "clipping.h"
#include "arith.h"
#include "mouse.h"

void _GrDrawPolygon(int n,GrxPoint *pt,GrFiller *f,GrFillArg c,int doClose)
{
        int i,px,py,x1,y1,x2,y2;
        if(n <= 0) return;
        if(n == 1) doClose = TRUE;
        x1 = x2 = pt[0].x;
        y1 = y2 = pt[0].y;
        for(i = 1; i < n; i++) {
            GrxPoint ppt = pt[i];
            if(x1 > ppt.x) x1 = ppt.x;
            if(x2 < ppt.x) x2 = ppt.x;
            if(y1 > ppt.y) y1 = ppt.y;
            if(y2 < ppt.y) y2 = ppt.y;
        }
        clip_ordbox(CURC,x1,y1,x2,y2);
        mouse_block(CURC,x1,y1,x2,y2);
        px = pt[n - 1].x;
        py = pt[n - 1].y;
        for(i = 0; i < n; i++) {
            x1 = px;
            y1 = py;
            x2 = px = pt[i].x;
            y2 = py = pt[i].y;
            if(i | doClose) {
                if(y1 > y2) {
                    iswap(x1,x2);
                    iswap(y1,y2);
                }
                clip_line_(CURC,x1,y1,x2,y2,continue,CLIP_EMPTY_MACRO_ARG);
                (*f->line)(
                    (x1 + CURC->x_offset),
                    (y1 + CURC->y_offset),
                    (x2 - x1),
                    (y2 - y1),
                    c
                );
            }
        }
        mouse_unblock();
}
