/**
 ** EGA4.C ---- the 16 color EGA frame driver
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

static long lastcolor;
static int  writeops[] = {
    (VGA_FUNC_SET << 8) + VGA_ROT_FN_SEL_REG,           /* C_SET */
    (VGA_FUNC_XOR << 8) + VGA_ROT_FN_SEL_REG,           /* C_XOR */
    (VGA_FUNC_OR  << 8) + VGA_ROT_FN_SEL_REG,           /* C_OR  */
    (VGA_FUNC_AND << 8) + VGA_ROT_FN_SEL_REG           /* C_AND */
};

static int init(GrVideoMode *mp)
{
        /* set write mode 0 */
        outport_w(VGA_GR_CTRL_PORT,((0 << 8) | VGA_MODE_REG));
        /* don't care register to 0 */
        outport_w(VGA_GR_CTRL_PORT,((0 << 8) | VGA_COLOR_DONTC_REG));
        /* enable all 4 planes for writing */
        outport_w(VGA_SEQUENCER_PORT,((0x0f << 8) | VGA_WRT_PLANE_ENB_REG));
        /* enable all 4 planes for set/reset */
        outport_w(VGA_GR_CTRL_PORT,((0x0f << 8) | VGA_SET_RESET_ENB_REG));
        lastcolor = (-1L);
        return(TRUE);
}


static
#ifdef __GNUC__
inline
#endif
long readpixel(GrFrame *c,int x,int y)
{
        char far *ptr = &SCRN->gc_baseaddr[0][(y * SCRN->gc_lineoffset) + (x >> 3)];
        uint mask = 0x80U >> (x &= 7);
        uint pixval;
        setup_far_selector(SCRN->gc_selector);
        outport_w(VGA_GR_CTRL_PORT,((3 << 8) | VGA_RD_PLANE_SEL_REG));
        pixval = (peek_b_f(ptr) & mask);
        outport_b(VGA_GR_CTRL_DATA,2);
        pixval = (peek_b_f(ptr) & mask) | (pixval << 1);
        outport_b(VGA_GR_CTRL_DATA,1);
        pixval = (peek_b_f(ptr) & mask) | (pixval << 1);
        outport_b(VGA_GR_CTRL_DATA,0);
        pixval = (peek_b_f(ptr) & mask) | (pixval << 1);
        lastcolor = (-1L);
        return((long)(pixval >> (7 - x)));
}

static
#ifdef __GNUC__
inline
#endif
void drawpixel(int x,int y,long color)
{
        char far *ptr = &CURC->gc_baseaddr[0][(y * SCRN->gc_lineoffset) + (x >> 3)];
        setup_far_selector(CURC->gc_selector);
        if(lastcolor != color) {
            outport_w(VGA_GR_CTRL_PORT,writeops[C_OPER(color) & 3]);
            outport_w(VGA_GR_CTRL_PORT,((((int)color & 0x0f) << 8) | VGA_SET_RESET_REG));
            lastcolor = color;
        }
        outport_w(VGA_GR_CTRL_PORT,((0x8000U >> (x & 7)) | VGA_BIT_MASK_REG));
        poke_b_f_or(ptr,0);
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

GrFrameDriver _GrFrameDriverEGA4 = {
    GR_frameEGA4,                /* frame mode */
    GR_frameRAM4,                /* compatible RAM frame mode */
    TRUE,                        /* onscreen */
    4,                                /* scan line width alignment */
    4,                                /* number of planes */
    4,                                /* bits per pixel */
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

