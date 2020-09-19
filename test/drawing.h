/*
 * DRAWING.H ---- a stupid little drawing used all over in test programs
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu] See "copying.grx" for details.
 */

#include <grx/color.h>
#include <grx/draw.h>

#include "rand.h"

void drawing(int xpos, int ypos, int xsize, int ysize, long fg, long bg)
{
#define XP(x) (int)((((long)(x) * (long)xsize) / 100L) + xpos)
#define YP(y) (int)((((long)(y) * (long)ysize) / 100L) + ypos)
    int ii;
    if (bg != GRX_COLOR_NONE) {
        grx_draw_filled_box(xpos, ypos, xpos + xsize - 1, ypos + ysize - 1, bg);
    }
    grx_draw_line(XP(10), YP(10), XP(40), YP(40), fg);
    grx_draw_line(XP(40), YP(10), XP(10), YP(40), fg);
    grx_draw_line(XP(35), YP(10), XP(65), YP(40), fg);
    grx_draw_line(XP(35), YP(40), XP(65), YP(10), fg);
    grx_draw_line(XP(70), YP(10), XP(90), YP(40), fg);
    grx_draw_line(XP(70), YP(40), XP(90), YP(10), fg);
    for (ii = 0; ii < 5; ii++) {
        grx_draw_box(
            XP(70 + 2 * ii), YP(10 + 3 * ii), XP(90 - 2 * ii), YP(40 - 3 * ii), fg);
    }
    grx_draw_filled_box(XP(10), YP(50), XP(60), YP(90), fg);
    grx_draw_box(XP(70), YP(50), XP(90), YP(90), fg);
    for (ii = 0; ii < 100; ii++) {
        grx_draw_pixel(XP((RND() % 20U) + 70), YP((RND() % 40U) + 50), fg);
    }
}

#undef XP
#undef YP
