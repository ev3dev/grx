/*
 * flood.c ---- fill an arbitrary area
 *
 * Copyright (c) 1997 Hartmut Schirmer
 * [e-mail: hsc@techfak.uni-kiel.de]
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

#include <grx/draw_nc.h>

#include "globals.h"
#include "libgrx.h"
#include "memfill.h"
#include "mouse.h"
#include "shapes.h"

static ScanFillFunc _filler;
static GrFillArg _fa;
static int lx, ly, mx, my, lxo, lyo;
static GrxColor _border;

typedef unsigned char element;       /* for 1bit/pixel images */
typedef unsigned short line_index;   /* start index table */
static element **done = NULL;        /* bitmap of already processed pixel */
static element **start = NULL;       /* pixel that need to be processed */
static unsigned elements;            /* no. bytes in each bitmap line */
static line_index *start_flg = NULL; /* !=0: index+1 of first start element !=0 */
                                     /* ==0: nothing to do */

#define bits_per_element  (sizeof(element) * 8)
#define offset_div        (bits_per_element)
#define calc_bit(x)       (((element)1) << ((x) & (bits_per_element - 1)))
#define calc_high_bits(x) ((~((element)0)) << ((x) & (bits_per_element - 1)))
#define calc_ofs(x)       ((x) / offset_div)

/* -------- internal line buffer functions
**
**  (x,y) scaled to (0..mx,0..my)
*/

static INLINE element *generate_line(element **buf, int y)
{
    if (buf[y] == NULL) {
        buf[y] = g_new0(element, elements);
    }

    return buf[y];
}

static INLINE void mark_line(element **buf, int x1, int x2, int y)
{
    element *l = generate_line(buf, y);
    element *anf, *ende;
    int start_bit, stop_bit;

    anf = &l[calc_ofs(x1)];
    ende = &l[calc_ofs(x2)];
    start_bit = calc_high_bits(x1);
    if (anf != ende) {
        *(anf++) |= start_bit;
        if (anf != ende) {
            GR_repl cv = ~((GR_repl)0);
            unsigned w = (ende - anf) * sizeof(element);
            repfill_b(anf, cv, w);
        }
        start_bit = ~0;
    }
    /* start_bit rejects all invalid low bits, let stop_bit discard
       all invalid high bits, but make sure stop_bit won't get zero */
    stop_bit = ~calc_high_bits(x2 + 1);
    if (stop_bit)
        start_bit &= stop_bit;
    *ende |= start_bit;
}

static INLINE void set_pix(element **buf, int x, int y)
{
    element *l = generate_line(buf, y);
    l[calc_ofs(x)] |= calc_bit(x);
}

static INLINE int test_pix(element **buf, int x, int y)
{
    element *l = buf[y];
    if (l != NULL)
        return (l[calc_ofs(x)] & calc_bit(x)) != 0;
    return FALSE;
}

static INLINE int test_screen(int x, int y)
{
    return grx_fast_get_pixel_at(x + lx, y + ly) == _border;
}

static INLINE int test_pixel(int x, int y)
{
    if (test_pix(done, x, y))
        return TRUE;
    if (test_screen(x, y)) {
        set_pix(done, x, y);
        return TRUE;
    }
    return FALSE;
}

static INLINE void SetStartFlag(int x, int y)
{
    int _x = calc_ofs(x);
    if (!start_flg[y] || _x < start_flg[y])
        start_flg[y] = _x + 1;
}

/* -------- screen & buffer fill functions
**
**  (x,y) scaled to (0..mx,0..my)
**  (0,0) maps to (lx,ly) on screen
*/

static void fill(int x, int y)
{
    int sx;

    sx = x;
    while (sx > 0 && !test_pixel(sx - 1, y))
        --sx;
    while (x < mx && !test_pixel(x + 1, y))
        ++x;
    (*_filler)(sx + lxo, y + lyo, x - sx + 1, _fa);
    mark_line(done, sx, x, y);
    if (y > 0) {
        mark_line(start, sx, x, y - 1);
        SetStartFlag(sx, y - 1);
    }
    if (y < my) {
        mark_line(start, sx, x, y + 1);
        SetStartFlag(sx, y + 1);
    }
}

/* -------- find fill area
**
**  (x,y) scaled to (0..mx,0..my)
*/

static void work(void)
{
    int top;
    element *s, *d;
    int rescan, dir = 1;

    top = 0;
    do {
        rescan = 0;
        while (0 <= top && top <= my) {
        ThisLine:
            if ((s = start[top]) != NULL && start_flg[top]) {
                unsigned i;
                if ((d = done[top]) != NULL) {
                    i = start_flg[top] - 1; /* start_flg[top] > 1 here */
                    s += i;
                    d += i;
                    for (; i < elements; ++i)
                        *(s++) &= ~*(d++);
                    s = start[top];
                }
                i = start_flg[top] - 1;
                start_flg[top] = 0;
                for (s += i; i < elements; ++i, ++s)
                    if (*s) {
                        element b = 1;
                        int x = i * offset_div;
                        do {
                            while (!(*s & b)) {
                                ++x;
                                b <<= 1;
                            }
                            if (test_screen(x, top)) {
                                set_pix(done, x, top);
                                *s ^= b;
                            }
                            else {
                                SetStartFlag(x, top);
                                fill(x, top);
                                rescan = 1;
                                goto ThisLine;
                            }
                        } while (*s);
                    }
            }
            top += dir;
        }
        dir = -dir;
        top += dir;
    } while (rescan);
}

void _GrFloodFill(int x, int y, GrxColor border, GrFiller *f, GrFillArg fa)
{
    int _x, _y;

    lx = CURC->x_clip_low;
    mx = CURC->x_clip_high;
    ly = CURC->y_clip_low;
    my = CURC->y_clip_high;
    lxo = lx + CURC->x_offset;
    lyo = ly + CURC->y_offset;

    if (x < lx || y < ly || x > mx || y > my || grx_fast_get_pixel_at(x, y) == border)
        return;

    _border = border;
    _filler = f->scan;
    _fa = fa;
    mx -= lx;
    _x = x - lx;
    my -= ly;
    _y = y - ly;
    done = g_new0(element *, my + 1);
    start = g_new0(element *, my + 1);
    start_flg = g_new0(line_index, my + 1);

    elements = calc_ofs(mx + bits_per_element) + 1;

    mouse_block(CURC, lx, ly, mx, my);
    set_pix(start, _x, _y);
    SetStartFlag(_x, _y);
    work();
    mouse_unblock();

    for (int i = my; i >= 0; --i) {
        g_free(done[i]);
    }
    g_free(done);
    done = NULL;

    for (int i = my; i >= 0; --i) {
        g_free(start[i]);
    }
    g_free(start);
    start = NULL;

    g_free(start_flg);
    start_flg = NULL;
}
