/**
 ** EGAVGA1.C ---- the mono EGA/VGA frame driver
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "grdriver.h"
#include "libgrx.h"
#include "arith.h"
#include "mempeek.h"
#include "memfill.h"
#include "vgaregs.h"
#include "ioport.h"

void GrSetEGAVGAmonoDrawnPlane(int plane)
{
        plane &= 3;
        outport_w(VGA_SEQUENCER_PORT,((0x100 << plane) | VGA_WRT_PLANE_ENB_REG));
        outport_w(VGA_GR_CTRL_PORT,((plane << 8) | VGA_RD_PLANE_SEL_REG));
}
        
void GrSetEGAVGAmonoShownPlane(int plane)
{
        void (*DACload)(int c,int r,int g,int b);
        int i;
        DACload = DRVINFO->actmode.extinfo->loadcolor;
        plane &= 3;
        if(DACload) for(i = 0; i < 16; i++) {
            int v = (i & (1 << plane)) ? 255 : 0;
            (*DACload)(i,v,v,v);
        }
}

static int init(GrVideoMode *mp)
{
        /* set write mode 0 */
        outport_w(VGA_GR_CTRL_PORT,((0 << 8) | VGA_MODE_REG));
        /* don't care register to 0 */
        outport_w(VGA_GR_CTRL_PORT,((0 << 8) | VGA_COLOR_DONTC_REG));
        /* disable all planes for set/reset */
        outport_w(VGA_GR_CTRL_PORT,((0 << 8) | VGA_SET_RESET_ENB_REG));
        GrSetEGAVGAmonoShownPlane(0);
        GrSetEGAVGAmonoDrawnPlane(0);
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
            peek_b_f(&SCRN->gc_baseaddr[0][(y * SCRN->gc_lineoffset) + (x >> 3)]) >> 
            (7 - (x & 7))
        ) & 1));
}

static
#ifdef __GNUC__
inline
#endif
void drawpixel(int x,int y,long color)
{
        char far *ptr = &CURC->gc_baseaddr[0][(y * CURC->gc_lineoffset) + (x >> 3)];
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

GrFrameDriver _GrFrameDriverEGAVGA1 = {
    GR_frameEGAVGA1,                /* frame mode */
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

