/*
 * gfaz.c ---- Mini GUI for GRX
 *
 * Copyright (C) 2000,2001 Mariano Alvarez Fernandez
 * [e-mail: malfer@teleline.es]
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

#include <stdlib.h>
#include <string.h>

#include <grx-3.0.h>

#include "gfaz.h"

GrxColor *egacolors;

/* Internal routines */

static int coord_into(int x, int y, int xo, int yo, int xl, int yl);

/**************************************************************************/

int gfaz_ini(int width, int height, int bpp)
{
    GError *error = NULL;

    if (!grx_set_mode(
            GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_BPP, &error, width, height, bpp)) {
        g_error("%s", error->message);
    }

    egacolors = grx_color_get_ega_colors();

    return 0;
}

/**************************************************************************/

int gfaz_fin(void)
{
    grx_set_mode(GRX_GRAPHICS_MODE_TEXT_DEFAULT, NULL);

    return 0;
}

/**************************************************************************/

static void par_event_queue(GrxEventType type, int p1, int p2, int p3)
{
    EventEx ev = {
        .event.type = type,
        .event.p1 = p1,
        .event.p2 = p2,
        .event.p3 = p3,
    };
    grx_event_put(&ev.grx_event);
}

/**************************************************************************/

static void dboton(int x, int y, int an, int al, GrxTextOptions *to, char *s, int t)

//   x, y posici�n de la esquina izquierda
//   an, al ancho y alto
//   c, ct color del fondo y del texto
//   t, tipo bit 0 0=normal, 1=apretado
//           bit 1 0=no selec, 1=seleccionado

{
    GrxPoint pol[7];
    int prof, pulsd, selec, despl;
    GrxLineOptions glo;
    int mouseblock;

    prof = (t & 0x1) ? 2 : 4;
    pulsd = (t & 0x1) ? 1 : 0;
    selec = (t & 0x2) ? 1 : 0;
    despl = (t & 0x1) ? 1 : 0;

    mouseblock = grx_mouse_block(NULL, x, y, x + an - 1, y + al - 1);
    grx_draw_box(x, y, x + an - 1, y + al - 1, BLACK);
    x = x + 1;
    y = y + 1;
    an = an - 2;
    al = al - 2;

    pol[0].x = x;
    pol[0].y = y;
    pol[1].x = x + an - 1;
    pol[1].y = y;
    pol[2].x = x + an - 2 - prof;
    pol[2].y = y + 1 + prof;
    pol[3].x = x + 1 + prof;
    pol[3].y = y + 1 + prof;
    pol[4].x = x + 1 + prof;
    pol[4].y = y + al - 2 - prof;
    pol[5].x = x;
    pol[5].y = y + al - 1;
    pol[6].x = pol[0].x;
    pol[6].y = pol[0].y;
    grx_draw_filled_polygon(7, pol, pulsd ? DARKGRAY : LIGHTGRAY);
    grx_draw_polygon(7, pol, BLACK);
    grx_draw_line(pol[0].x, pol[0].y, pol[3].x, pol[3].y, BLACK);
    pol[0].x = x + an - 1;
    pol[0].y = y + al - 1;
    pol[3].x = x + an - 2 - prof;
    pol[3].y = y + al - 2 - prof;
    pol[6].x = pol[0].x;
    pol[6].y = pol[0].y;
    grx_draw_filled_polygon(7, pol, pulsd ? LIGHTGRAY : DARKGRAY);
    grx_draw_polygon(7, pol, BLACK);
    grx_draw_line(pol[0].x, pol[0].y, pol[3].x, pol[3].y, BLACK);
    grx_draw_filled_box(x + 2 + prof, y + 2 + prof, x + an - 3 - prof,
        y + al - 3 - prof, grx_text_options_get_bg_color(to));

    if (despl)
        grx_draw_text(s, x + an / 2 + 1, y + al / 2 + 1, to);
    else
        grx_draw_text(s, x + an / 2, y + al / 2, to);

    if (selec) {
        glo.color = grx_text_options_get_fg_color(to);
        glo.width = 1;
        glo.n_dash_patterns = 2;
        glo.dash_pattern0 = 2;
        glo.dash_pattern1 = 1;
        grx_draw_box_with_options(
            x + 8, y + al / 2 - 6, x + an - 8, y + al / 2 + 5, &glo);
    }

    grx_mouse_unblock(mouseblock);
}

/**************************************************************************/

void paint_button(int x, int y, Button *b)
{
    dboton(x + b->x, y + b->y, b->wide, b->high, b->text_opt, b->text, b->status);
}

/**************************************************************************/

void paint_button_group(Button_Group *bg)
{
    int i;

    for (i = 0; i < bg->nb; i++)
        paint_button(bg->x, bg->y, &(bg->b[i]));
}

/**************************************************************************/

int pev_button_group(GrxEvent *ev, Button_Group *bg)
{
    int i;

    if (ev->type == GRX_EVENT_TYPE_BUTTON_PRESS) {
        for (i = 0; i < bg->nb; i++)
            if (coord_into(ev->button.x, ev->button.y, bg->x + bg->b[i].x,
                    bg->y + bg->b[i].y, bg->b[i].wide, bg->b[i].high)) {
                bg->b[bg->pb].status &= ~BSTATUS_SELECTED;
                paint_button(bg->x, bg->y, &(bg->b[bg->pb]));
                bg->b[i].status |= BSTATUS_PRESSED | BSTATUS_SELECTED;
                paint_button(bg->x, bg->y, &(bg->b[i]));
                bg->pb = i;
                bg->abp = 1;
                par_event_queue(EV_SELECT, bg->b[i].bid, 0, 0);
                return 1;
            }
    }
    else if (ev->type == GRX_EVENT_TYPE_BUTTON_RELEASE) {
        if (bg->abp) {
            i = bg->pb;
            bg->b[i].status &= ~BSTATUS_PRESSED;
            paint_button(bg->x, bg->y, &(bg->b[i]));
            bg->abp = 0;
            if (coord_into(ev->button.x, ev->button.y, bg->x + bg->b[i].x,
                    bg->y + bg->b[i].y, bg->b[i].wide, bg->b[i].high)) {
                par_event_queue(EV_COMMAND, bg->b[i].bid, 0, 0);
            }
            return 1;
        }
    }
    else if (ev->type == GRX_EVENT_TYPE_KEY_DOWN) {
        if (ev->key.keysym == GRX_KEY_RIGHT || ev->key.keysym == GRX_KEY_DOWN
            || ev->key.keysym == GRX_KEY_TAB) {
            if (bg->pb < bg->nb - 1) {
                bg->b[bg->pb].status &= ~BSTATUS_SELECTED;
                paint_button(bg->x, bg->y, &(bg->b[bg->pb]));
                bg->pb++;
                bg->b[bg->pb].status |= BSTATUS_SELECTED;
                paint_button(bg->x, bg->y, &(bg->b[bg->pb]));
                par_event_queue(EV_SELECT, bg->b[bg->pb].bid, 0, 0);
            }
            return 1;
        }
        else if (ev->key.keysym == GRX_KEY_LEFT || ev->key.keysym == GRX_KEY_UP
                 || ev->key.keysym == GRX_KEY_BACK_TAB) {
            if (bg->pb > 0) {
                bg->b[bg->pb].status &= ~BSTATUS_SELECTED;
                paint_button(bg->x, bg->y, &(bg->b[bg->pb]));
                bg->pb--;
                bg->b[bg->pb].status |= BSTATUS_SELECTED;
                paint_button(bg->x, bg->y, &(bg->b[bg->pb]));
                par_event_queue(EV_SELECT, bg->b[bg->pb].bid, 0, 0);
            }
            return 1;
        }
        else if (ev->key.keysym == GRX_KEY_RETURN) {
            par_event_queue(EV_COMMAND, bg->b[bg->pb].bid, 0, 0);
            return 1;
        }
    }

    return 0;
}

/**************************************************************************/

static int coord_into(int x, int y, int xo, int yo, int xl, int yl)
{
    if (x < xo)
        return 0;
    if (x >= xo + xl)
        return 0;
    if (y < yo)
        return 0;
    if (y >= yo + yl)
        return 0;
    return 1;
}

/**************************************************************************/

void paint_board(Board *b)
{
    int x1, y1, x2, y2;

    x1 = b->x;
    y1 = b->y;
    x2 = b->x + b->wide - 1;
    y2 = b->y + b->high - 1;

    grx_draw_box(x1, y1, x2, y2, egacolors[b->lcolor]);
    grx_draw_box(x1 + 1, y1 + 1, x2 - 1, y2 - 1, egacolors[b->bcolor]);
    grx_draw_box(x1 + 2, y1 + 2, x2 - 2, y2 - 2, egacolors[b->bcolor]);
    grx_draw_box(x1 + 3, y1 + 3, x2 - 3, y2 - 3, egacolors[b->lcolor]);
    grx_draw_filled_box(x1 + 4, y1 + 4, x2 - 4, y2 - 4, egacolors[b->color]);
}
