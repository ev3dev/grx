/*
 * bldcurs.c ---- create and destroy cursor data structures
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

#include <glib.h>

#include <grx/draw_nc.h>
#include <grx/extents.h>

#include "libgrx.h"
#include "allocate.h"
#include "memfill.h"
#include "mouse.h"

G_DEFINE_BOXED_TYPE (GrxCursor, grx_cursor, grx_cursor_ref, grx_cursor_unref);

/**
 * grx_cursor_new:
 * @pixels: cursor bitmap data
 * @pitch: width of pixel data rows in bytes
 * @width: width of cursor in pixels
 * @height: height of cursor in pixels
 * @x0: hot point of cursor x coordinate
 * @y0: hot point of cursor y coordinate
 * @colors: (element-type GrxColor): color table for pixel data
 *
 * Create a new cursor using the data provided.
 *
 * Pixel data values of 0 are treated as transparent, 1 is the first color in
 * @colors and so on.
 *
 * The hot point @x0, @y0 is relative to the top left of the cursor.
 *
 * Returns: (transfer full) (nullable): the new cursor or %NULL if creating the
 * cursor failed.
 */
GrxCursor *grx_cursor_new(unsigned char *pixels,int pitch,int w,int h,int xo,int yo,const GArray *C)
{
        GrxCursor  *curs;
        GrxContext  save;
        int  wrkw2 = (w + 7) & ~7;
        int  workw = wrkw2 << 1;
        int  workh = ((h + 7) & ~7) << 1;
        int  xx,yy;
        curs = malloc(sizeof(GrxCursor));
        if(!curs) return(NULL);
        sttzero(curs);
        if(!grx_context_new(workw,((workh << 1) + h),NULL,&curs->work)) {
            free(curs);
            return(NULL);
        }
        curs->xsize = w;
        curs->ysize = h;
        curs->xoffs = xo;
        curs->yoffs = yo;
        curs->xwork = workw;
        curs->ywork = workh;
        grx_save_current_context(&save);
        grx_set_current_context(&curs->work);
        grx_fast_draw_filled_box(0,0,(workw - 1),(h - 1),0L);
        for(yy = 0; yy < h; yy++) {
            unsigned char *p = (unsigned char *)pixels + (yy * pitch);
            for(xx = 0; xx < w; xx++,p++) {
                if(*p) grx_fast_draw_pixel(xx,yy,grx_color_get_value(grx_color_lookup(C,(*p - 1))));
                else   grx_fast_draw_pixel((xx + wrkw2),yy,grx_color_get_value(-1L));
            }
        }
        grx_set_current_context(&save);

        return grx_cursor_ref(curs);
}

/**
 * grx_cursor_ref:
 * @cursor: the cursor
 *
 * Increase the reference count by 1.
 *
 * Returns: (transfer none): the cursor
 */
GrxCursor *grx_cursor_ref(GrxCursor *cursor)
{
    g_return_val_if_fail(cursor != NULL, NULL);

    cursor->ref_count++;

    return cursor;
}

/**
 * grx_cursor_unref:
 * @cursor: the cursor
 *
 * Decrease the reference count by 1. If there are no more references, the
 * cursor is freed.
 */
void grx_cursor_unref(GrxCursor *cursor)
{
    g_return_if_fail(cursor != NULL);
    g_return_if_fail(cursor->ref_count != 0);

    cursor->ref_count--;
    if (cursor->ref_count) {
        return;
    }

    grx_cursor_hide(cursor);
    grx_context_unref(&cursor->work);
    free(cursor);
}
