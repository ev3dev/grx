/*
 * pblitr2r.c ---- bitblt routine for packed (8,16,24,32 bpp) modes
 *                 (non banking, source and destination may overlap)
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

#include <grx/context.h>

#include "arith.h"
#include "colors.h"
#include "grdriver.h"
#include "libgrx.h"
#include "memcopy.h"
#include "mempeek.h"

/* check if forward blit would overwrite source */
#define OVERLAP(dp, sp) (((GR_int8 *)dp) > ((GR_int8 *)sp))

void _GrFrDrvPackedBitBltR2R(const GrxFrame *dst, int dx, int dy, const GrxFrame *src,
    int sx, int sy, int w, int h, GrxColor op)
{
    unsigned char *dptr, *sptr;
    unsigned dskip, sskip;
    int oper, ww;

    GRX_ENTER();

    dskip = dst->line_offset - w;
    sskip = src->line_offset - w;
    oper = C_OPER(op);

    dptr = &dst->base_address[umuladd32(dy, dst->line_offset, dx)];
    sptr = &src->base_address[umuladd32(sy, src->line_offset, sx)];

    if (OVERLAP(dptr, sptr)) {
        if (oper == C_IMAGE) {
            memmove(dptr, sptr, w);
        }
        else {
            dptr += umuladd32((h - 1), dst->line_offset, w - 1);
            sptr += umuladd32((h - 1), src->line_offset, w - 1);
            do {
                ww = w;
                switch (oper) {
                case C_XOR:
                    revcopy_xor(dptr, dptr, sptr, ww);
                    break;
                case C_OR:
                    revcopy_or(dptr, dptr, sptr, ww);
                    break;
                case C_AND:
                    revcopy_and(dptr, dptr, sptr, ww);
                    break;
                default:
                    revcopy_set(dptr, dptr, sptr, ww);
                    break;
                }
                dptr -= dskip;
                sptr -= sskip;
            } while (--h != 0);
        }
    }
    else {
        if (oper == C_IMAGE) {
            memcpy(dptr, sptr, w);
        }
        else {
            do {
                ww = w;
                switch (oper) {
                case C_XOR:
                    fwdcopy_xor(dptr, dptr, sptr, ww);
                    break;
                case C_OR:
                    fwdcopy_or(dptr, dptr, sptr, ww);
                    break;
                case C_AND:
                    fwdcopy_and(dptr, dptr, sptr, ww);
                    break;
                default:
                    fwdcopy_set(dptr, dptr, sptr, ww);
                    break;
                }
                dptr += dskip;
                sptr += sskip;
            } while (--h != 0);
        }
    }

    GRX_LEAVE();
}
