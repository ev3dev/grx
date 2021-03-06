/*
 * driver24.h ---- the 24bpp color padded SVGA linear frame buffer driver
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu].
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
 * Contributions by: (See "doc/credits.doc" for details)
 * Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 * Andrzej Lawa [FidoNet: Andrzej Lawa 2:480/19.77]
 */

#include "access24.h"
#include "arith.h"
#include "colors.h"
#include "globals.h"
#include "grdriver.h"
#include "libgrx.h"
#include "memfill.h"
#include "mempeek.h"

/* frame offset address calculation */
#define MULT3(x)       ((x) + (x) + (x))
#define FOFS(x, y, lo) ((y) * (lo) + MULT3(x))

#define peek24(p)     peek_24((p))
#define poke24_xor    poke_24_xor
#define poke24_or     poke_24_or
#define poke24_and    poke_24_and
#define poke24_set    poke_24
#define repfill8_xor  repfill_b_xor
#define repfill8_or   repfill_b_or
#define repfill8_and  repfill_b_and
#define repfill8_set  repfill_b
#define repfill24_xor repfill_24_xor
#define repfill24_or  repfill_24_or
#define repfill24_and repfill_24_and
#define repfill24_set repfill_24_set

static INLINE GrxColor readpixel(const GrxFrame *c, int x, int y)
{
    GrxColor col;
    unsigned char *p;
    GRX_ENTER();
    p = &c->base_address[FOFS(x, y, c->line_offset)];
    col = peek24(p);
    GRX_RETURN(col);
}

static INLINE void drawpixel(int x, int y, GrxColor color)
{
    unsigned char *p;
    GRX_ENTER();
    p = &CURC->gc_base_address[FOFS(x, y, CURC->gc_line_offset)];

    switch (C_OPER(color)) {
    case C_XOR:
        poke24_xor(p, color);
        break;
    case C_OR:
        poke24_or(p, color);
        break;
    case C_AND:
        poke24_and(p, color);
        break;
    default:
        poke24_set(p, color);
        break;
    }
    GRX_LEAVE();
}

static void drawhline(int x, int y, int w, GrxColor color)
{
    unsigned char *p;
    GRX_ENTER();
    p = &CURC->gc_base_address[FOFS(x, y, CURC->gc_line_offset)];

    w = MULT3(w);

#ifndef GRX_HAVE_FAST_REPFILL24
    {
        GR_int8u c0;
        c0 = RD24BYTE(color, 0);
        if (c0 == RD24BYTE(color, 1) && c0 == RD24BYTE(color, 2)) {
            GR_repl cval = freplicate_b(c0);
            switch (C_OPER(color)) {
            case C_XOR:
                repfill8_xor(p, cval, w);
                break;
            case C_OR:
                repfill8_or(p, cval, w);
                break;
            case C_AND:
                repfill8_and(p, cval, w);
                break;
            default:
                repfill8_set(p, cval, w);
                break;
            }
            goto done;
        }
    }
#endif
    switch (C_OPER(color)) {
    case C_XOR:
        repfill24_xor(p, color, w);
        break;
    case C_OR:
        repfill24_or(p, color, w);
        break;
    case C_AND:
        repfill24_and(p, color, w);
        break;
    default:
        repfill24_set(p, color, w);
        break;
    }
#ifndef GRX_HAVE_FAST_REPFILL24
done:
#endif
    GRX_LEAVE();
}

#include "generic/bitmap.c"
#include "generic/block.c"
#include "generic/line.c"
#include "generic/pattern.c"
#include "generic/vline.c"

static void bitblt(const GrxFrame *dst, int dx, int dy, const GrxFrame *src, int sx,
    int sy, int w, int h, GrxColor op)
{
    GRX_ENTER();
    if (grx_color_get_mode(op) == GRX_COLOR_MODE_IMAGE)
        _GrFrDrvGenericBitBlt(dst, dx, dy, src, sx, sy, w, h, op);
    else
        _GrFrDrvPackedBitBltR2R(
            dst, MULT3(dx), dy, src, MULT3(sx), sy, MULT3(w), h, op);
    GRX_LEAVE();
}
