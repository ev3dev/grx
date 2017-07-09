/*
 * patfbox.c
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

#include "globals.h"
#include "mouse.h"
#include "libgrx.h"
#include "arith.h"
#include "clipping.h"
#include "shapes.h"

/**
 * grx_draw_filled_box_with_pattern:
 * @x1: the left X coordinate
 * @y1: the top Y coordinate
 * @x2: the right X coordinate
 * @y2: the bottom Y coordinate
 * @p: the pattern
 *
 * Draws a filled rectangle on the current context using the specified
 * coordinates and pattern.
 */
void grx_draw_filled_box_with_pattern(int x1,int y1,int x2,int y2,GrxPattern *p)
{
        int width,height;

        clip_box(CURC,x1,y1,x2,y2);
        mouse_block(CURC,x1,y1,x2,y2);
        width  = x2 - x1 + 1;
        height = y2 - y1 + 1;
        x1 += CURC->x_offset;
        y1 += CURC->y_offset;
        if(!p->is_pixmap)
            while(--height >= 0) _GrFillPattern(x1,y1++,width,p);
        else {
            void (*bltfun)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor);
            int pwdt = p->pixmap.width;
            int phgt = p->pixmap.height;
            int xoff = x1 % pwdt;
            int ypos = y1;
            int yoff = ypos % phgt;
            if (CURC->gc_is_on_screen) bltfun = CURC->gc_driver->bltr2v;
            else                   bltfun = CURC->gc_driver->bitblt;
            while(height > 0) {
                int fillh   = min(height,(phgt - yoff));
                int linewdt = width;
                int xpos    = x1;
                int xcuroff = xoff;
                while(linewdt > 0) {
                    int fillw = min(linewdt,(pwdt - xcuroff));
                    (*bltfun)(
                        &CURC->frame,xpos,ypos,
                        &p->pixmap.source,xcuroff,yoff,fillw,fillh,
                        p->pixmap.mode
                    );
                    linewdt -= fillw;
                    xpos += fillw;
                    xcuroff = 0;
                }
                height -= fillh;
                ypos += fillh;
                yoff = 0;
            }
        }
        mouse_unblock();
}
