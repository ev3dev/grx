/*
 * imgtest.c ---- test image functions mapping
 *
 * Copyright (c) 1998 Hartmut Schirmer
 *
 * This is a test/demo file of the GRX graphics library.
 * You can use GRX test/demo files as you want.
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

#include "test.h"

#define PARTS 4

TESTFUNC(imgtest)
{
        int  x = grx_get_size_x();
        int  y = grx_get_size_y();
        int  ww = (x / PARTS)-1;
        int  wh = (y / PARTS)-1;
        int m1, m2, d1, d2;
        GrxColor c1, c2, c3;
        GrxContext ctx;
        GrxImage *img1;
        GrxImage *img2;
        if (! grx_context_create(ww,wh,NULL,&ctx)) return;

        grx_context_set_current(&ctx);
        c1 = grx_color_info_alloc_color(255,100,0);
        c2 = grx_color_info_alloc_color(0,0,(grx_color_info_n_colors() >= 16 ? 180 : 63));
        c3 = grx_color_info_alloc_color(0,255,0);
        drawing(0,0,ww,wh,c1,c2);
        grx_draw_box(0,0,ww-1,wh-1,c1);

        grx_context_set_current(NULL);

        img1 = grx_image_create_from_context(&ctx);
        if (!img1) return;

        grx_draw_filled_box(0,0,ww+1,wh+1,c3);
        grx_draw_filled_box(ww+15,0,2*ww+16,wh+1,c3);

        grx_bit_blt(1,1,&ctx,0,0,ww-1,wh-1,0);
        grx_draw_image(ww+16,1,img1);
        grx_draw_image_tiled(0,wh+4,x-1,y-1, img1);

        GrKeyRead();

        for (m1=1; m1 <= PARTS ; m1<<=1) {
          for (d1=1; d1 <= PARTS; d1 <<= 1) {
            for (m2=1; m2 <= PARTS ; m2<<=1) {
              for (d2=1; d2 <= PARTS; d2 <<= 1) {
                img2 = grx_image_stretch(img1,(m1*ww)/d1, (m2*wh)/d2);
                if (img2) {
                  grx_draw_image_tiled(0,0,x-1,y-1,img2);
                  grx_image_free(img2);
                }
              }
            }
          }
        }
        GrKeyRead();

        /* let's finish with some grx_get_scanline / grx_put_scanline tests */
        for (d1 = 1; d1 < 32; ++d1) {
          for (m1 = wh; m1 < y-wh-d1-1; ++m1) {
            const GrxColor *cp;
            cp = grx_get_scanline(ww+1,x-ww-d1,m1+1,NULL);
            if (cp) {
              grx_put_scanline(ww,x-ww-d1-1,m1,cp,GRX_COLOR_MODE_IMAGE|c2);
            }
          }
        }

        GrKeyRead();
}
