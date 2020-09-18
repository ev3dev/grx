/*
 * ram2.c - the 4 color RAM frame driver
 *
 * Copyright (c) 2017 David Lechner <david@lechnology.com>
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
 */

#include "colors.h"
#include "globals.h"
#include "libgrx.h"
#include "grdriver.h"
#include "arith.h"
#include "memcopy.h"
#include "memfill.h"

/* frame offset address calculation */
#define FOFS(x,y,lo) ( umul32((y),(int)(lo)) + ((x)>>2) )


static INLINE
GrxColor readpixel(GrxFrame *c,int x,int y)
{
    GR_int32u offs;
    GrxColor pix;

    GRX_ENTER();
    offs = FOFS(x,y,c->line_offset);
    pix = (GrxColor)((c->base_address[offs] >> ((x & 3) << 1)) & 3);
    GRX_RETURN(pix);
}

static INLINE
void drawpixel(int x,int y,GrxColor color)
{
        GR_int32u offs;
        unsigned int mask;
        unsigned int cval;
        int  op;

        GRX_ENTER();
        offs = FOFS(x,y,CURC->gc_line_offset);
        mask = 0x03 << (x = (x & 3) << 1);
        cval = (unsigned int)color << x;
        switch (op = C_OPER(color) & 3) {
        case C_WRITE:
        case C_AND:
            CURC->gc_base_address[offs] &= cval | ~mask;
            if (op != C_WRITE) {
                break;
            }
        case C_OR:
            CURC->gc_base_address[offs] |= cval & mask;
            break;
        default:
            CURC->gc_base_address[offs] ^= cval & mask;
            break;
        }
        GRX_LEAVE();
}

#define maskoper(d,op,s,msk) do {                          \
    unsigned char _c_ = peek_b(d);                         \
    poke_b((d),(_c_ & ~(msk)) | ((_c_ op (s)) & (msk)) );  \
} while (0)

#define maskset(d,c,msk) \
    poke_b((d),(peek_b(d) & ~(msk)) | ((c) & (msk)))

static void drawhline(int x,int y,int w,GrxColor color) {
    int oper, cval, wd;
    GR_int32u DO;
    GR_int8u  lm, rm;
    GR_int32u cv;

    GRX_ENTER();
    oper = C_OPER(color);
    cval = (int)color;
    DO   = FOFS(x,y,CURC->gc_line_offset);
    lm   = 0xff << ((x & 3) << 1);
    rm   = 0xff >> (((-(w + x)) & 3) << 1);
    wd   = ((x + w + 3) >> 2) - (x >> 2);
    if (wd == 1) {
        lm &= rm;
    }
    cv = (cval & 3) * 0x55555555;

    if (!( !cv && (oper==C_OR||oper==C_XOR)) && !(cv && oper==C_AND) ) {
        GR_int8u *dptr = (GR_int8u *)&CURC->gc_base_address[DO];
        int ww = wd;

        if ((GR_int8u)(~lm)) {
            switch(oper) {
            case C_XOR:
                maskoper(dptr,^,(GR_int8u)cv,lm);
                break;
            case C_OR:
                maskoper(dptr,|,(GR_int8u)cv,lm);
                break;
            case C_AND:
                maskoper(dptr,&,(GR_int8u)cv,lm);
                break;
            default:
                maskset(dptr,(GR_int8u)cv,lm);
                break;
            }
            if (!(--ww)) {
                GRX_LEAVE();
                return;
            }
            ++dptr;
        }
        if ((GR_int8u)(~rm)) {
            --ww;
        }
        if (ww) {
            switch(oper) {
            case C_XOR:
                repfill_b_xor(dptr,cv,ww);
                break;
            case C_OR:
                repfill_b_or(dptr,cv,ww);
                break;
            case C_AND:
                repfill_b_and(dptr,cv,ww);
                break;
            default:
                repfill_b(dptr,cv,ww);
                break;
            }
        }
        if ((GR_int8u)(~rm)) {
            switch(oper) {
            case C_XOR:
                maskoper(dptr,^,(GR_int8u)cv,rm);
                break;
            case C_OR:
                maskoper(dptr,|,(GR_int8u)cv,rm);
                break;
            case C_AND:
                maskoper(dptr,&,(GR_int8u)cv,rm);
                break;
            default:
                maskset(dptr,(GR_int8u)cv,rm);
                break;
            }
        }
    }
    GRX_LEAVE();
}
#undef maskoper
#undef maskset

static void drawvline(int x, int y, int h, GrxColor color)
{
    unsigned int lwdt, mask, imask, oper, hh;
    unsigned char *p;
    GR_int32u offs;

    GRX_ENTER();
    oper = C_OPER(color);
    lwdt = CURC->gc_line_offset;
    offs = FOFS(x,y,lwdt);
    color &= 3;
    mask = color << (x = (x & 3) << 1);
    imask = ~((~color & 3) << x);
    switch (oper) {
    case C_XOR:
        /* no need to xor anything with 0 */
        if (color) {
            p = &CURC->gc_base_address[offs];
            hh = h;
            colfill_b_xor(p,lwdt,mask,hh);
        }
        break;
    case C_OR:
        /* no need to or anything with 0 */
        if (color) {
            p = &CURC->gc_base_address[offs];
            hh = h;
            colfill_b_or(p,lwdt,mask,hh);
        }
        break;
    case C_AND:
        /* no need to and anything with 3 */
        if (color != 0x03) {
            p = &CURC->gc_base_address[offs];
            hh = h;
            colfill_b_and(p,lwdt,imask,hh);
        }
        break;
    default:
        if (color) {
            p = &CURC->gc_base_address[offs];
            hh = h;
            colfill_b_or(p,lwdt,mask,hh);
        }
        if (color != 0x03) {
            p = &CURC->gc_base_address[offs];
            hh = h;
            colfill_b_and(p,lwdt,imask,hh);
        }
        break;
    }
    GRX_LEAVE();
}

static
#include "generic/block.c"

static
#include "generic/line.c"

static
#include "generic/bitmap.c"

static
#include "generic/pattern.c"

static
#include "generic/bitblt.c"

GrxFrameDriver _GrFrameDriverRAM2 = {
    .mode               = GRX_FRAME_MODE_RAM_2BPP,
    .rmode              = GRX_FRAME_MODE_UNDEFINED,
    .is_video           = FALSE,
    .row_align          = 4,
    .bits_per_pixel     = 2,
    .max_plane_size     = 2*16*1024L*1024L,
    .init               = NULL,
    .readpixel          = readpixel,
    .drawpixel          = drawpixel,
    .drawline           = drawline,
    .drawhline          = drawhline,
    .drawvline          = drawvline,
    .drawblock          = drawblock,
    .drawbitmap         = drawbitmap,
    .drawpattern        = drawpattern,
    .bitblt             = bitblt,
    .bltv2r             = NULL,
    .bltr2v             = NULL,
    .getindexedscanline = _GrFrDrvGenericGetIndexedScanline,
    .putscanline        = _GrFrDrvGenericPutScanline,
};

GrxFrameDriver _GrFrameDriverCFB2_LFB = {
    .mode               = GRX_FRAME_MODE_LFB_2BPP,
    .rmode              = GRX_FRAME_MODE_RAM_2BPP,
    .is_video           = TRUE,
    .row_align          = 4,
    .bits_per_pixel     = 2,
    .max_plane_size     = 2*16*1024L*1024L,
    .init               = NULL,
    .readpixel          = readpixel,
    .drawpixel          = drawpixel,
    .drawline           = drawline,
    .drawhline          = drawhline,
    .drawvline          = drawvline,
    .drawblock          = drawblock,
    .drawbitmap         = drawbitmap,
    .drawpattern        = drawpattern,
    .bitblt             = bitblt,
    .bltv2r             = bitblt,
    .bltr2v             = bitblt,
    .getindexedscanline = _GrFrDrvGenericGetIndexedScanline,
    .putscanline        = _GrFrDrvGenericPutScanline,
};
