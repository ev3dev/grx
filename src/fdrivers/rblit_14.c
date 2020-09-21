/*
 * rblit_14.c ---- ram to ram blit support functions for 1bpp and
 *                 4x1bpp RAM frame drivers
 *
 * Copyright (c) 1998 Hartmut Schirmer
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

#include <grx/color.h>
#include <grx/context.h>

#include "arith.h"
#include "colors.h"
#include "grdriver.h"
#include "libgrx.h"
#include "memcopy.h"
#include "memfill.h"
#include "mempeek.h"
#include "rblit_14.h"

/* frame offset address calculation */
#define FOFS(x, y, lo) umuladd32((y), (lo), ((x) >> 3))

#define maskoper(d, op, s, msk)                              \
    do {                                                     \
        unsigned char _c_ = peek_b(d);                       \
        poke_b((d), (_c_ & ~(msk)) | ((_c_ op(s)) & (msk))); \
    } while (0)
#define maskset(d, c, msk) poke_b((d), (peek_b(d) & ~(msk)) | ((c) & (msk)))

static void get_scanline(unsigned char *dptr, unsigned char *sptr, int w)
{
    GRX_ENTER();
    fwdcopy_set(sptr, dptr, sptr, w);
    GRX_LEAVE();
}

static void put_scanline(
    unsigned char *dptr, unsigned char *sptr, int w, GR_int8u lm, GR_int8u rm, int op)
{
    GRX_ENTER();
    if (w == 1)
        lm &= rm;
    if (((GR_int8u)(~lm))) {
        switch (op) {
        case C_XOR:
            maskoper(dptr, ^, *sptr, lm);
            break;
        case C_OR:
            maskoper(dptr, |, *sptr, lm);
            break;
        case C_AND:
            maskoper(dptr, &, *sptr, lm);
            break;
        default:
            maskset(dptr, *sptr, lm);
            break;
        }
        if (--w == 0)
            goto done;
        ++dptr;
        ++sptr;
    }
    if (((GR_int8u)(~rm)))
        --w;
    if (w)
        switch (op) {
        case C_XOR:
            fwdcopy_xor(dptr, dptr, sptr, w);
            break;
        case C_OR:
            fwdcopy_or(dptr, dptr, sptr, w);
            break;
        case C_AND:
            fwdcopy_and(dptr, dptr, sptr, w);
            break;
        default:
            fwdcopy_set(dptr, dptr, sptr, w);
            break;
        }
    if (((GR_int8u)(~rm)))
        switch (op) {
        case C_XOR:
            maskoper(dptr, ^, *sptr, rm);
            break;
        case C_OR:
            maskoper(dptr, |, *sptr, rm);
            break;
        case C_AND:
            maskoper(dptr, &, *sptr, rm);
            break;
        default:
            maskset(dptr, *sptr, rm);
            break;
        }
done:
    GRX_LEAVE();
}

static void invert_scanline(unsigned char *sptr, int w)
{
    GRX_ENTER();
    while (w--)
        sptr[w] = ~sptr[w];
    GRX_LEAVE();
}

G_GNUC_INTERNAL extern void _GR_shift_scanline(
    GR_int8u **dst, GR_int8u **src, int ws, int shift);
#define shift_scanline(dst, src, w, sh) \
    _GR_shift_scanline((GR_int8u **)&(dst), (GR_int8u **)&(src), (w), (sh))

void _GR_rblit_14(GrxFrame *dst, gint dx, gint dy, GrxFrame *src, gint x, gint y,
    gint w, gint h, GrxColor op, _GR_blitFunc bitblt, gboolean invert)
{
    GRX_ENTER();

    if (grx_color_get_mode(op) != GRX_COLOR_MODE_IMAGE) {
        GR_int32u doffs, soffs;
        int oper = C_OPER(op);
        int shift = ((int)(x & 7)) - ((int)(dx & 7));
        GR_int8u lm = 0xff << (dx & 7);
        GR_int8u rm = 0xff >> ((-(w + dx)) & 7);
        int ws = ((x + w + 7) >> 3) - (x >> 3);
        int wd = ((dx + w + 7) >> 3) - (dx >> 3);
        int dskip = dst->line_offset;
        int sskip = src->line_offset;
        guint8 *scanline = g_newa(guint8, ((w + 7) >> 3) + 2);

        if ((dy > y) && (dst->base_address == src->base_address)) {
            /* reverse */
            dy += h - 1;
            y += h - 1;
            doffs = FOFS(dx, dy, dskip);
            soffs = FOFS(x, y, sskip);

            unsigned char *dptr = &dst->base_address[doffs];
            unsigned char *sptr = &src->base_address[soffs];
            int hh = h;

            if (shift) {
                while (hh-- > 0) {
                    shift_scanline(scanline, sptr, ws, shift);

                    if (invert) {
                        invert_scanline(scanline, ws);
                    }

                    put_scanline(dptr, scanline, wd, lm, rm, oper);
                    dptr -= dskip;
                    sptr -= sskip;
                }
            }
            else {
                while (hh-- > 0) {
                    get_scanline(scanline, sptr, ws);

                    if (invert) {
                        invert_scanline(scanline, ws);
                    }

                    put_scanline(dptr, scanline, wd, lm, rm, oper);
                    dptr -= dskip;
                    sptr -= sskip;
                }
            }
        }
        else {
            /* forward */
            doffs = FOFS(dx, dy, dst->line_offset);
            soffs = FOFS(x, y, src->line_offset);

            unsigned char *dptr = &dst->base_address[doffs];
            unsigned char *sptr = &src->base_address[soffs];
            int hh = h;

            if (shift) {
                while (hh-- > 0) {
                    shift_scanline(scanline, sptr, ws, shift);

                    if (invert) {
                        invert_scanline(scanline, ws);
                    }

                    put_scanline(dptr, scanline, wd, lm, rm, oper);
                    dptr += dskip;
                    sptr += sskip;
                }
            }
            else {
                while (hh-- > 0) {
                    get_scanline(scanline, sptr, ws);

                    if (invert) {
                        invert_scanline(scanline, ws);
                    }

                    put_scanline(dptr, scanline, wd, lm, rm, oper);
                    dptr += dskip;
                    sptr += sskip;
                }
            }
        }
    }
    else {
        bitblt(dst, dx, dy, src, x, y, w, h, op);
    }

    GRX_LEAVE();
}
