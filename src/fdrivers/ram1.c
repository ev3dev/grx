/*
 * ram1.c ---- the mono system RAM frame driver
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
 */

#include "colors.h"
#include "globals.h"
#include "libgrx.h"
#include "grdriver.h"
#include "allocate.h"
#include "arith.h"
#include "mempeek.h"
#include "memcopy.h"
#include "memfill.h"

#include "rblit_14.h"

/* frame offset address calculation */
#define FOFS(x,y,lo)  umuladd32((y),(lo),((x)>>3))

static INLINE
GrxColor readpixel(GrxFrame *c,int x,int y)
{
        GR_int8u *ptr;
        GRX_ENTER();
        ptr = (GR_int8u *)&c->base_address[0][FOFS(x,y,c->line_offset)];
        GRX_RETURN((GrxColor)( (*ptr >> (x & 7)) & 1));
}

static INLINE
GrxColor readpixel_inv(GrxFrame *c,int x,int y)
{
    GR_int8u *ptr;
    GRX_ENTER();
    ptr = (GR_int8u *)&c->base_address[0][FOFS(x,y,c->line_offset)];
    GRX_RETURN((GrxColor)(((*ptr >> (x & 7)) & 1) ? 0 : 1));
}

static INLINE
void drawpixel(int x,int y,GrxColor color)
{
        GR_int8u *ptr;
        GR_int8u cval;

        GRX_ENTER();
        ptr = (GR_int8u *)&CURC->gc_base_address[0][FOFS(x,y,CURC->gc_line_offset)];
        cval = (color & 1) << (x &= 7);
        switch(C_OPER(color)) {
            case C_XOR: *ptr ^=  cval; break;
            case C_OR:  *ptr |=  cval; break;
            case C_AND: *ptr &= ~cval; break;
            default:    *ptr  = (*ptr & (~0x80 >> (7 -x))) | cval; break;
        }
        GRX_LEAVE();
}

static INLINE
void drawpixel_inv(int x,int y, GrxColor color)
{
    drawpixel(x, y, grx_color_get_mode(color)|grx_color_get_value(~color));
}

#define maskoper(d,op,s,msk,SF,DF) do {                       \
    unsigned char _c_ = peek_b##DF(d);                        \
    poke_b##DF((d), (_c_ & ~(msk)) | ((_c_ op (s)) & (msk))); \
  } while (0)
#define maskset(d,c,msk,DF) \
    poke_b##DF((d),(peek_b##DF(d) & ~(msk)) | ((c) & (msk)))

static void drawhline(int x,int y,int w,GrxColor color) {
  int oper;

  GRX_ENTER();
  oper = C_OPER(color);
  color &= 1;
  if (!( !color && (oper==C_OR||oper==C_XOR)) && !(color && oper==C_AND) ) {
    GR_int8u lm = 0xff << (x & 7);
    GR_int8u rm = 0xff >> ((-(w + x)) & 7);
    GR_int8u *p = (GR_int8u *)&CURC->gc_base_address[0][FOFS(x,y,CURC->gc_line_offset)];
    GR_repl cv = 0;
    if (color) cv = ~cv;
    w = ((x+w+7) >> 3) - (x >> 3);
    if (w==1) lm &= rm;
    if ( ((GR_int8u)(~lm)) ) {
        switch(oper) {
          case C_XOR: maskoper(p,^,(GR_int8u)cv,lm,_set,_n); break;
          case C_OR:  maskoper(p,|,(GR_int8u)cv,lm,_set,_n); break;
          case C_AND: maskoper(p,&,(GR_int8u)cv,lm,_set,_n); break;
          default:    maskset(p,(GR_int8u)cv,lm,_n);         break;
        }
        if (!(--w)) goto done;
        ++p;
    }
    if ( ((GR_int8u)(~rm)) ) --w;
    if (w) {
        switch(oper) {
          case C_XOR: repfill_b_xor(p,cv,w); break;
          case C_OR:  repfill_b_or(p,cv,w);  break;
          case C_AND: repfill_b_and(p,cv,w); break;
          default:    repfill_b(p,cv,w);     break;
        }
    }
    if ( ((GR_int8u)(~rm)) ) {
        switch(oper) {
          case C_XOR: maskoper(p,^,(GR_int8u)cv,rm,_set,_n); break;
          case C_OR:  maskoper(p,|,(GR_int8u)cv,rm,_set,_n); break;
          case C_AND: maskoper(p,&,(GR_int8u)cv,rm,_set,_n); break;
          default:    maskset(p,(GR_int8u)cv,rm,_n);         break;
        }
    }
  }
done:
  GRX_LEAVE();
}

static void drawhline_inv(int x,int y,int h, GrxColor color)
{
    drawhline(x, y, h, grx_color_get_mode(color)|grx_color_get_value(~color));
}

static void drawvline(int x,int y,int h,GrxColor color)
{
        unsigned int lwdt, mask, oper;
        char *p;
        GRX_ENTER();
        oper = C_OPER(color);
        color &= 1;
        lwdt = CURC->gc_line_offset;
        mask = 0x01 << (x & 7);
        switch (oper) {
          case C_XOR:
              /* no need to xor anything with 0 */
              if (color) {
                p = &CURC->gc_base_address[0][FOFS(x,y,lwdt)];
                colfill_b_xor(p,lwdt,mask,h);
              }
              break;
          case C_OR:
              /* no need to or anything with 0 */
              if (color) {
            do_OR:
                p = &CURC->gc_base_address[0][FOFS(x,y,lwdt)];
                colfill_b_or(p,lwdt,mask,h);
              }
              break;
          case C_AND:
              /* no need to and anything with 1 */
              if (!color) {
            do_AND:
                mask = ~mask;
                p = &CURC->gc_base_address[0][FOFS(x,y,lwdt)];
                colfill_b_and(p,lwdt,mask,h);
              }
              break;
          default:
              if (color) goto do_OR;
              goto do_AND;
        }
        GRX_LEAVE();
}

static void drawvline_inv(int x,int y,int h, GrxColor color)
{
    drawvline(x, y, h, grx_color_get_mode(color)|grx_color_get_value(~color));
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

static void bltr2r(GrxFrame *dst,int dx,int dy,
                   GrxFrame *src,int x,int y,int w,int h,
                   GrxColor op)
{
    GRX_ENTER();
    _GR_rblit_14(dst,dx,dy,src,x,y,w,h,op,1,bitblt,FALSE);
    GRX_LEAVE();
}

static void bltr2r_inv(GrxFrame *dst,int dx,int dy,
                       GrxFrame *src,int x,int y,int w,int h,
                       GrxColor op)
{
    GRX_ENTER();
    _GR_rblit_14(dst,dx,dy,src,x,y,w,h,op,1,bitblt,TRUE);
    GRX_LEAVE();
}


/* -------------------------------------------------------------------- */

static
#include "generic/getiscl.c"

/* -------------------------------------------------------------------- */

GrxFrameDriver _GrFrameDriverRAM1 = {
    .mode               = GRX_FRAME_MODE_RAM_1BPP,  /* frame mode */
    .rmode              = GRX_FRAME_MODE_UNDEFINED, /* compatible RAM frame mode */
    .is_video           = FALSE,                    /* onscreen */
    .row_align          = 4,                        /* scan line width alignment */
    .num_planes         = 1,                        /* number of planes */
    .bits_per_pixel     = 1,                        /* bits per pixel */
    .max_plane_size     = 16*1024L*1024L,           /* max plane size the code can handle */
    .init               = NULL,
    .readpixel          = readpixel,
    .drawpixel          = drawpixel,
    .drawline           = drawline,
    .drawhline          = drawhline,
    .drawvline          = drawvline,
    .drawblock          = drawblock,
    .drawbitmap         = drawbitmap,
    .drawpattern        = drawpattern,
    .bitblt             = bltr2r,
    .bltv2r             = NULL,
    .bltr2v             = NULL,
    .getindexedscanline = getindexedscanline,
    .putscanline        = _GrFrDrvGenericPutScanline,
};

GrxFrameDriver _GrFrameDriverMONO01_LFB = {
    .mode               = GRX_FRAME_MODE_LFB_MONO01, /* frame mode */
    .rmode              = GRX_FRAME_MODE_RAM_1BPP,   /* compatible RAM frame mode */
    .is_video           = TRUE,                      /* onscreen */
    .row_align          = 4,                         /* scan line width alignment */
    .num_planes         = 1,                         /* number of planes */
    .bits_per_pixel     = 1,                         /* bits per pixel */
    .max_plane_size     = 16*1024L*1024L,            /* max plane size the code can handle */
    .init               = NULL,
    .readpixel          = readpixel_inv,
    .drawpixel          = drawpixel_inv,
    .drawline           = drawline,
    .drawhline          = drawhline_inv,
    .drawvline          = drawvline_inv,
    .drawblock          = drawblock,
    .drawbitmap         = drawbitmap,
    .drawpattern        = drawpattern,
    .bitblt             = bltr2r,
    .bltv2r             = bltr2r_inv,
    .bltr2v             = bltr2r_inv,
    .getindexedscanline = getindexedscanline,
    .putscanline        = _GrFrDrvGenericPutScanline,
};

GrxFrameDriver _GrFrameDriverMONO10_LFB = {
    .mode               = GRX_FRAME_MODE_LFB_MONO10, /* frame mode */
    .rmode              = GRX_FRAME_MODE_RAM_1BPP,   /* compatible RAM frame mode */
    .is_video           = TRUE,                      /* onscreen */
    .row_align          = 4,                         /* scan line width alignment */
    .num_planes         = 1,                         /* number of planes */
    .bits_per_pixel     = 1,                         /* bits per pixel */
    .max_plane_size     = 16*1024L*1024L,            /* max plane size the code can handle */
    .init               = NULL,
    .readpixel          = readpixel,
    .drawpixel          = drawpixel,
    .drawline           = drawline,
    .drawhline          = drawhline,
    .drawvline          = drawvline,
    .drawblock          = drawblock,
    .drawbitmap         = drawbitmap,
    .drawpattern        = drawpattern,
    .bitblt             = bltr2r,
    .bltv2r             = bltr2r,
    .bltr2v             = bltr2r,
    .getindexedscanline = getindexedscanline,
    .putscanline        = _GrFrDrvGenericPutScanline,
};
