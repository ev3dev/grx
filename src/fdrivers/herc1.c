/**
 ** HERC1.C ---- the mono Hercules frame driver
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "grdriver.h"
#include "libgrx.h"
#include "arith.h"
#include "mempeek.h"
#include "memfill.h"

#define HRES 720
#define VRES 348

static uint offtab[2 * VRES];

static int init(GrVideoMode *mp)
{
        int i;
        if(mp->width  != HRES) return(FALSE);
        if((mp->height != VRES) && (mp->height != (2 * VRES))) return(FALSE);
        for(i = 0; i < VRES; i++) {
            offtab[i] = ((i & 3) * 0x2000U) + ((i >> 2) * (HRES / 8));
            offtab[i + VRES] = offtab[i] + 0x8000U;
        }
        return(TRUE);
}

static
#ifdef __GNUC__
inline
#endif
long readpixel(GrFrame *c,int x,int y)
{
        setup_far_selector(SCRN->gc_selector);
        return((long)((
            peek_b_f(&SCRN->gc_baseaddr[0][offtab[y] + (x >> 3)]) >> 
            (7 - (x & 7))
        ) & 1));
}

static
#ifdef __GNUC__
inline
#endif
void drawpixel(int x,int y,long color)
{
        char far *ptr = &CURC->gc_baseaddr[0][offtab[y] + (x >> 3)];
        uint cval = ((uint)color & 1) << (7 - (x &= 7));
        setup_far_selector(CURC->gc_selector);
        switch(C_OPER(color)) {
            case C_XOR: poke_b_f_xor(ptr,cval);  break;
            case C_OR:  poke_b_f_or(ptr,cval);   break;       
            case C_AND: poke_b_f_and(ptr,~cval); break;       
            default:    poke_b_f(ptr,((peek_b_f(ptr) & (~0x80 >> x)) | cval));
        }
}

static
#include "fdrivers/generic/hline.c"

static
#include "fdrivers/generic/vline.c"

static
#include "fdrivers/generic/block.c"

static
#include "fdrivers/generic/line.c"

static
#include "fdrivers/generic/bitmap.c"

static
#include "fdrivers/generic/pattern.c"

static
#include "fdrivers/generic/bitblt.c"

GrFrameDriver _GrFrameDriverHERC1 = {
    GR_frameHERC1,                /* frame mode */
    GR_frameRAM1,                /* compatible RAM frame mode */
    TRUE,                        /* onscreen */
    4,                                /* line width alignment */
    1,                                /* number of planes */
    1,                                /* bits per pixel */
    64*1024L,                        /* max plane size the code can handle */
    init,
    readpixel,
    drawpixel,
    drawline,
    drawhline,
    drawvline,
    drawblock,
    drawbitmap,
    drawpattern,
    bitblt,
    _GrFrDrvGenericBitBlt,
    _GrFrDrvGenericBitBlt
};

