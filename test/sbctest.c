/*
 * sbctest.c ---- test subcontext
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
 */

#include <string.h>

#include "test.h"

static void drawpf(int border, GrxPixmap *pat);
static void drawp(int border, const GrxLineOptions *o, GrxPixmap *p);

TESTFUNC(sbctest)
{
    const guint8 bits[] = { 0, 76, 50, 0, 0, 76, 60, 0 };
    GrxContext *grc, *grsc;
    GrxPixmap *pat1, *pat2;
    GrxLineOptions grl;
    GrxFont *grf;

    grc = grx_context_new(300, 300, NULL, NULL);
    if (grc == NULL)
        return;
    grsc = grx_context_new_subcontext(10, 10, 290, 290, grc, NULL);
    if (grsc == NULL)
        return;
    pat1 = grx_pixmap_new_from_bits(bits, 8, 8, GRX_COLOR_WHITE, GRX_COLOR_BLACK);
    if (pat1 == NULL)
        return;
    pat2 = grx_pixmap_new_from_bits(bits, 8, 8, GRX_COLOR_BLACK, GRX_COLOR_WHITE);
    if (pat2 == NULL)
        return;
    grf = grx_font_load("lucida", 40, NULL);
    if (grf == NULL) {
        return;
    }

    grx_draw_box(19, 19, 320, 320, GRX_COLOR_WHITE);

    grx_draw_text("White drawing on context       ", 0, 0, white_text_black_bg);
    grx_set_current_context(grc);
    grx_clear_context(GRX_COLOR_BLACK);
    drawing(10, 10, 280, 280, GRX_COLOR_WHITE, GRX_COLOR_NONE);
    grx_set_current_context(NULL);
    grx_bit_blt(20, 20, grc, 0, 0, 299, 299, GRX_COLOR_MODE_WRITE);
    run_main_loop_until_key_press();

    grx_draw_text("Black drawing on subcontext    ", 0, 0, white_text_black_bg);
    grx_set_current_context(grsc);
    drawing(0, 0, 280, 280, GRX_COLOR_BLACK, GRX_COLOR_NONE);
    grx_set_current_context(NULL);
    grx_bit_blt(20, 20, grc, 0, 0, 299, 299, GRX_COLOR_MODE_WRITE);
    run_main_loop_until_key_press();

    grx_draw_text("Pattern drawing on context     ", 0, 0, white_text_black_bg);
    grx_set_current_context(grc);
    grx_clear_context(GRX_COLOR_BLACK);
    drawpf(10, pat1);
    grx_set_current_context(NULL);
    grx_bit_blt(20, 20, grc, 0, 0, 299, 299, GRX_COLOR_MODE_WRITE);
    run_main_loop_until_key_press();

    grx_draw_text("Pattern drawing on subcontext  ", 0, 0, white_text_black_bg);
    grx_set_current_context(grsc);
    grx_clear_context(GRX_COLOR_BLACK);
    drawpf(0, pat2);
    grx_set_current_context(NULL);
    grx_bit_blt(20, 20, grc, 0, 0, 299, 299, GRX_COLOR_MODE_XOR);
    run_main_loop_until_key_press();

    grl.color = GRX_COLOR_WHITE;
    grl.width = 3;
    grl.n_dash_patterns = 0;

    grx_draw_text("Patterned drawing on context   ", 0, 0, white_text_black_bg);
    grx_set_current_context(grc);
    grx_clear_context(GRX_COLOR_BLACK);
    drawp(10, &grl, pat1);
    grx_set_current_context(NULL);
    grx_bit_blt(20, 20, grc, 0, 0, 299, 299, GRX_COLOR_MODE_WRITE);
    run_main_loop_until_key_press();

    grx_draw_text("Patterned drawing on subcontext", 0, 0, white_text_black_bg);
    grx_set_current_context(grsc);
    grx_clear_context(GRX_COLOR_BLACK);
    drawp(0, &grl, pat2);
    grx_set_current_context(NULL);
    grx_bit_blt(20, 20, grc, 0, 0, 299, 299, GRX_COLOR_MODE_XOR);
    run_main_loop_until_key_press();
#if 0
  grt.txo_fgcolor.v = GRX_COLOR_WHITE;
  grt.txo_bgcolor.v = GRX_COLOR_BLACK | GRX_COLOR_MODE_OR;
  grt.txo_font = grf;
  grt.txo_direct = GRX_TEXT_DIRECTION_RIGHT;
  grt.txo_xalign = GRX_TEXT_HALIGN_LEFT;
  grt.txo_yalign = GRX_TEXT_VALIGN_MIDDLE;
  grt.txo_chrtype = GRX_CHAR_TYPE_BYTE;

  grx_draw_text("Patterned text on context      ", 0,0,white_text_black_bg );
  grx_set_current_context( grc );
  grx_clear_context( GRX_COLOR_BLACK );
  grx_draw_string_with_pixmap( "Hello all",9,20,60,&grt,pat1 );
  grx_draw_char_with_pixmap( 'G',20,120,&grt,pat1 );
  grx_draw_string_with_pixmap_ext( "Hola a todos",12,20,180,&grt,pat1 );
  grx_set_current_context( NULL );
  grx_bit_blt( 20,20,grc,0,0,299,299,GRX_COLOR_MODE_WRITE );
  run_main_loop_until_key_press();

  grx_draw_text("Patterned text on subcontext   ", 0,0,white_text_black_bg );
  grx_set_current_context( grsc );
  grx_clear_context( GRX_COLOR_BLACK );
  grx_draw_string_with_pixmap( "Hello all",9,10,50,&grt,pat2 );
  grx_draw_char_with_pixmap( 'G',10,110,&grt,pat2 );
  grx_draw_string_with_pixmap_ext( "Hola a todos",12,10,170,&grt,pat2 );
  grx_set_current_context( NULL );
  grx_bit_blt( 20,20,grc,0,0,299,299,GRX_COLOR_MODE_XOR );
  run_main_loop_until_key_press();
#endif
    grx_font_unref(grf);
    grx_pixmap_free(pat2);
    grx_pixmap_free(pat1);
    grx_context_unref(grsc);
    grx_context_unref(grc);
}

/***/

static void drawpf(int border, GrxPixmap *pat)
{
    GrxPoint pt1[4] = { { 130, 200 }, { 140, 240 }, { 150, 250 }, { 160, 180 } };
    GrxPoint pt2[4] = { { 230, 200 }, { 235, 240 }, { 246, 250 }, { 258, 180 } };
    GrxPoint ptaux[4];
    int i;

    grx_draw_filled_box_with_pixmap(
        0 + border, 0 + border, 93 + border, 93 + border, pat);
    grx_draw_filled_circle_with_pixmap(139 + border, 46 + border, 45, pat);
    grx_draw_filled_ellipse_with_pixmap(232 + border, 46 + border, 45, 35, pat);
    grx_draw_filled_circle_arc_with_pixmap(
        46 + border, 139 + border, 45, -300, 600, GRX_ARC_STYLE_CLOSED_RADIUS, pat);
    grx_draw_filled_ellipse_arc_with_pixmap(139 + border, 139 + border, 45, 35, -700,
        400, GRX_ARC_STYLE_CLOSED_RADIUS, pat);
    grx_draw_filled_line_with_pixmap(
        188 + border, 139 + border, 278 + border, 139 + border, pat);
    grx_draw_filled_pixel_with_pixmap(47 + border, 228 + border, pat);
    grx_draw_filled_pixel_with_pixmap(47 + border, 229 + border, pat);
    grx_draw_filled_pixel_with_pixmap(47 + border, 230 + border, pat);
    grx_draw_filled_pixel_with_pixmap(47 + border, 231 + border, pat);
    grx_draw_filled_pixel_with_pixmap(47 + border, 232 + border, pat);
    for (i = 0; i < 4; i++) {
        ptaux[i].x = pt1[i].x + border;
        ptaux[i].y = pt1[i].y + border;
    }
    grx_draw_filled_polygon_with_pixmap(4, ptaux, pat);
    for (i = 0; i < 4; i++) {
        ptaux[i].x = pt2[i].x + border;
        ptaux[i].y = pt2[i].y + border;
    }
    grx_draw_filled_convex_polygon_with_pixmap(4, ptaux, pat);
}

/***/

static void drawp(int border, const GrxLineOptions *o, GrxPixmap *p)
{
    GrxPoint pt1[4] = { { 130, 200 }, { 140, 240 }, { 150, 250 }, { 160, 180 } };
    GrxPoint pt2[4] = { { 230, 200 }, { 235, 240 }, { 246, 250 }, { 258, 180 } };
    GrxPoint ptaux[4];
    int i;

    grx_draw_box_with_pixmap(0 + border, 0 + border, 93 + border, 93 + border, o, p);
    grx_draw_circle_with_pixmap(139 + border, 46 + border, 45, o, p);
    grx_draw_ellipse_with_pixmap(232 + border, 46 + border, 45, 35, o, p);
    grx_draw_circle_arc_with_pixmap(
        46 + border, 139 + border, 45, -300, 600, GRX_ARC_STYLE_CLOSED_RADIUS, o, p);
    grx_draw_ellipse_arc_with_pixmap(139 + border, 139 + border, 45, 35, -700, 400,
        GRX_ARC_STYLE_CLOSED_RADIUS, o, p);
    grx_draw_line_with_pixmap(
        188 + border, 139 + border, 278 + border, 139 + border, o, p);
    for (i = 0; i < 4; i++) {
        ptaux[i].x = pt1[i].x + border;
        ptaux[i].y = pt1[i].y + border;
    }
    grx_draw_polygon_with_pixmap(4, ptaux, o, p);
    for (i = 0; i < 4; i++) {
        ptaux[i].x = pt2[i].x + border;
        ptaux[i].y = pt2[i].y + border;
    }
    grx_draw_polyline_with_pixmap(4, ptaux, o, p);
}
