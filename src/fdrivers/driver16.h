/*
 * driver16.h ---- the 32k/64k color padded SVGA linear frame buffer driver
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

#include "arith.h"
#include "colors.h"
#include "globals.h"
#include "grdriver.h"
#include "libgrx.h"
#include "memfill.h"
#include "mempeek.h"

/* frame offset address calculation */
#define FOFS(x, y, lo) ((y) * (lo) + ((x) << 1))

#define peek16        peek_w
#define poke16_xor    poke_w_xor
#define poke16_or     poke_w_or
#define poke16_and    poke_w_and
#define poke16        poke_w
#define colfill16_xor colfill_w_xor
#define colfill16_or  colfill_w_or
#define colfill16_and colfill_w_and
#define colfill16     colfill_w
#define repfill16_xor repfill_w_xor
#define repfill16_or  repfill_w_or
#define repfill16_and repfill_w_and
#define repfill16     repfill_w

#ifndef ASM_386_SEL
#define ASM_386_SEL
#endif

static INLINE GrxColor readpixel(const GrxFrame *c, int x, int y)
{
    GR_int16u *pp;
    GRX_ENTER();
    /* problem here? Does c always point to screen? */
    pp = (GR_int16u *)&c->base_address[FOFS(x, y, c->line_offset)];
#if defined(MISALIGNED_16bit_OK)
    GRX_RETURN(*pp);
#else
    GRX_RETURN((GR_int16u)peek16(pp));
#endif
}

static INLINE void drawpixel(int x, int y, GrxColor color)
{
    unsigned char *ptr;
    GRX_ENTER();
    ptr = &CURC->gc_base_address[FOFS(x, y, CURC->gc_line_offset)];

    switch (C_OPER(color)) {
    case C_XOR:
        poke16_xor(ptr, (GR_int16u)color);
        break;
    case C_OR:
        poke16_or(ptr, (GR_int16u)color);
        break;
    case C_AND:
        poke16_and(ptr, (GR_int16u)color);
        break;
    default:
        poke16(ptr, (GR_int16u)color);
        break;
    }
    GRX_LEAVE();
}

static void drawhline(int x, int y, int w, GrxColor color)
{
    unsigned char *pp;
    GR_repl cval;
    GRX_ENTER();
    pp = &CURC->gc_base_address[FOFS(x, y, CURC->gc_line_offset)];
    cval = freplicate_w(color);

    switch (C_OPER(color)) {
    case C_XOR:
        repfill16_xor(pp, cval, w);
        break;
    case C_OR:
        repfill16_or(pp, cval, w);
        break;
    case C_AND:
        repfill16_and(pp, cval, w);
        break;
    default:
        repfill16(pp, cval, w);
        break;
    }
    GRX_LEAVE();
}

static void drawvline(int x, int y, int h, GrxColor color)
{
    unsigned lwdt;
    unsigned char *pp;
    GRX_ENTER();
    lwdt = CURC->gc_line_offset;
    pp = &CURC->gc_base_address[FOFS(x, y, lwdt)];

    switch (C_OPER(color)) {
    case C_XOR:
        colfill16_xor(pp, lwdt, (GR_int16u)color, h);
        break;
    case C_OR:
        colfill16_or(pp, lwdt, (GR_int16u)color, h);
        break;
    case C_AND:
        colfill16_and(pp, lwdt, (GR_int16u)color, h);
        break;
    default:
        colfill16(pp, lwdt, (GR_int16u)color, h);
        break;
    }
    GRX_LEAVE();
}

static void drawblock(int x, int y, int w, int h, GrxColor color)
{
    int skip;
    unsigned char *ptr;
    GR_repl cval;

    GRX_ENTER();
    skip = CURC->gc_line_offset;
    ptr = &CURC->gc_base_address[FOFS(x, y, skip)];
    skip -= w << 1;
    cval = freplicate_w(color);

    switch (C_OPER(color)) {
    case C_XOR:
        while (h-- != 0) {
            int ww = w;
            repfill16_xor(ptr, cval, ww);
            ptr += skip;
        }
        break;
    case C_OR:
        while (h-- != 0) {
            int ww = w;
            repfill16_or(ptr, cval, ww);
            ptr += skip;
        }
        break;
    case C_AND:
        while (h-- != 0) {
            int ww = w;
            repfill16_and(ptr, cval, ww);
            ptr += skip;
        }
        break;
    default:
        while (h-- != 0) {
            int ww = w;
            repfill16(ptr, cval, ww);
            ptr += skip;
        }
        break;
    }
    GRX_LEAVE();
}

#if defined(__GNUC__) && defined(__i386__)
static void drawline(int x, int y, int dx, int dy, GrxColor color)
{
    struct {
        int errsub;  /* subtract from error term */
        int erradd;  /* add to error term when carry */
        int offset1; /* add to pointer if no carry on error term */
        int offset2; /* add to pointer if carry / banking dir */
    } lndata;
    int npts, error, xstep;
    unsigned char *ptr;

    GRX_ENTER();

#ifdef __GNUC__
#ifdef __i386__
#define ASM_LINE1(OPC)                             \
    asm volatile(                                  \
        ""                                         \
        "   .align 2,0x90                      \n" \
        "0: " #OPC "w %6," ASM_386_SEL             \
        "(%0)       \n"                            \
        "   subl %7,%2                         \n" \
        "   jb   1f                            \n" \
        "   leal -2(%3),%3                     \n" \
        "   decl %1                            \n" \
        "   jne  0b                            \n" \
        "   jmp  2f                            \n" \
        "   .align 2,0x90                      \n" \
        "1: addl 4  + %7,%2                    \n" \
        "   addl 12 + %7,%3                    \n" \
        "   decl %1                            \n" \
        "   jne  0b                            \n" \
        "2: "                                      \
        : "=r"(ptr), "=r"(npts), "=r"(error)       \
        : "0"((long)(ptr)), "1"(npts), "2"(error), "r"((short)(color)), "o"(lndata))
#define ASM_LINE2(OPC)                             \
    asm volatile(                                  \
        ""                                         \
        "   .align 2,0x90                      \n" \
        "0: " #OPC "w %6," ASM_386_SEL             \
        "(%0)       \n"                            \
        "   subl %7,%2                         \n" \
        "   jb   1f                            \n" \
        "   addl 8 + %7,%3                     \n" \
        "   decl %1                            \n" \
        "   jne  0b                            \n" \
        "   jmp  2f                            \n" \
        "   .align 2,0x90                      \n" \
        "1: addl 4  + %7,%2                    \n" \
        "   addl 12 + %7,%3                    \n" \
        "   decl %1                            \n" \
        "   jne  0b                            \n" \
        "2: "                                      \
        : "=r"(ptr), "=r"(npts), "=r"(error)       \
        : "0"((long)(ptr)), "1"(npts), "2"(error), "r"((short)(color)), "o"(lndata))
#endif
#endif

    if (dy < 0) {
        y -= (dy = (-dy));
        x -= (dx = (-dx));
    }
    if (dx < 0) {
        xstep = (-2);
        dx = (-dx);
    }
    else
        xstep = 2;

    ptr = &CURC->gc_base_address[FOFS(x, y, CURC->gc_line_offset)];

    if (dx > dy) {
        npts = dx + 1;
        error = dx >> 1;
        lndata.errsub = dy;
        lndata.erradd = dx;
        lndata.offset1 = xstep;
        lndata.offset2 = CURC->gc_line_offset + xstep;
        if (xstep < 0) {
            lndata.offset1 = 1;
            switch (C_OPER(color)) {
            case C_XOR:
                ASM_LINE1(xor);
                break;
            case C_OR:
                ASM_LINE1(or);
                break;
            case C_AND:
                ASM_LINE1(and);
                break;
            default:
                ASM_LINE1(mov);
                break;
            }
            goto done;
        }
    }
    else {
        npts = dy + 1;
        error = dy >> 1;
        lndata.errsub = dx;
        lndata.erradd = dy;
        lndata.offset1 = CURC->gc_line_offset;
        lndata.offset2 = CURC->gc_line_offset + xstep;
    }
    switch (C_OPER(color)) {
    case C_XOR:
        ASM_LINE2(xor);
        break;
    case C_OR:
        ASM_LINE2(or);
        break;
    case C_AND:
        ASM_LINE2(and);
        break;
    default:
        ASM_LINE2(mov);
        break;
    }
done:
    GRX_LEAVE();
}
#else
#include "generic/line.c"
#endif

#include "generic/bitmap.c"
#include "generic/pattern.c"

static void bitblt(const GrxFrame *dst, int dx, int dy, const GrxFrame *src, int sx,
    int sy, int w, int h, GrxColor op)
{
    GRX_ENTER();
    if (grx_color_get_mode(op) == GRX_COLOR_MODE_IMAGE)
        _GrFrDrvGenericBitBlt(dst, dx, dy, src, sx, sy, w, h, op);
    else
        _GrFrDrvPackedBitBltR2R(
            dst, (dx << 1), dy, src, (sx << 1), sy, (w << 1), h, op);
    GRX_LEAVE();
}
