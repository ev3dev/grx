/**
 ** SVGA4.C ---- the 16 color (Super) VGA frame driver
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
static int  modereg;
static int  writeops[] = {
    (VGA_FUNC_SET << 8) + VGA_ROT_FN_SEL_REG,           /* C_SET */
    (VGA_FUNC_XOR << 8) + VGA_ROT_FN_SEL_REG,           /* C_XOR */
    (VGA_FUNC_OR  << 8) + VGA_ROT_FN_SEL_REG,           /* C_OR  */
    (VGA_FUNC_AND << 8) + VGA_ROT_FN_SEL_REG           /* C_AND */
};

static int init(GrVideoMode *mp)
{
        /* save original mode register */
        outport_b(VGA_GR_CTRL_PORT,VGA_MODE_REG);
        modereg = ((inport_b(VGA_GR_CTRL_DATA) & 0xfc) << 8) | VGA_MODE_REG;
        /* set write mode 3 and enable color compare */
        outport_w(VGA_GR_CTRL_PORT,(((8 + 3) << 8) | modereg));
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
        long offs = umul32(y,SCRN->gc_lineoffset) + (x >> 3);
        char far *ptr = &SCRN->gc_baseaddr[0][BANKPOS(offs)];
        uint mask = 0x80 >> (x &= 7);
        uint pixval;
        CHKBANK(BANKNUM(offs));
        setup_far_selector(SCRN->gc_selector);
        outport_w(VGA_GR_CTRL_PORT,modereg);
        outport_w(VGA_GR_CTRL_PORT,((3 << 8) | VGA_RD_PLANE_SEL_REG));
        pixval = (peek_b_f(ptr) & mask);
        outport_b(VGA_GR_CTRL_DATA,2);
        pixval = (peek_b_f(ptr) & mask) | (pixval << 1);
        outport_b(VGA_GR_CTRL_DATA,1);
        pixval = (peek_b_f(ptr) & mask) | (pixval << 1);
        outport_b(VGA_GR_CTRL_DATA,0);
        pixval = (peek_b_f(ptr) & mask) | (pixval << 1);
        outport_w(VGA_GR_CTRL_PORT,(((8 + 3) << 8) | modereg));
        lastcolor = (-1L);
        return((long)(pixval >> (7 - x)));
}

static
#ifdef __GNUC__
inline
#endif
void drawpixel(int x,int y,long color)
{
        long offs = umul32(y,CURC->gc_lineoffset) + (x >> 3);
        char far *ptr = &CURC->gc_baseaddr[0][BANKPOS(offs)];
        CHKBANK(BANKNUM(offs));
        setup_far_selector(CURC->gc_selector);
        if(lastcolor != color) {
            outport_w(VGA_GR_CTRL_PORT,writeops[C_OPER(color) & 3]);
            outport_w(VGA_GR_CTRL_PORT,((((int)color & 0x0f) << 8) | VGA_SET_RESET_REG));
            lastcolor = color;
        }
        poke_b_f_and(ptr,(0x80U >> (x & 7)));
}

static void drawhline(int x,int y,int w,long color)
{
        long offs  = umul32(y,CURC->gc_lineoffset) + (x >> 3);
        uint lmask = 0xff >> (x & 7);
        uint rmask = 0xff << ((-(w += x)) & 7);
        uint w1,w2 = BANKLFT(offs);
        int  oper  = C_OPER(color);
        w  = ((w + 7) >> 3) - (x >> 3);
        w2 = w - (w1 = umin(w,w2));
        if(w == 1) lmask &= rmask;
        setup_far_selector(CURC->gc_selector);
        if(lastcolor != color) {
            outport_w(VGA_GR_CTRL_PORT,writeops[oper & 3]);
            outport_w(VGA_GR_CTRL_PORT,((((int)color & 0x0f) << 8) | VGA_SET_RESET_REG));
            lastcolor = color;
        }
        do {
            char far *pp = &CURC->gc_baseaddr[0][BANKPOS(offs)];
            CHKBANK(BANKNUM(offs));
            offs += w1;
            if((uchar)lmask != (uchar)0xff) {
                poke_b_f_and(pp,lmask);
                lmask = 0xff;
                if(--w1 == 0) goto nextbank;
                pp++;
            }
            if(((uchar)rmask != (uchar)0xff) && (w2 == 0)) {
                w1--;
                poke_b_f_and(&pp[w1],rmask);
                if(w1 == 0) break;
            }
            if(oper == C_WRITE) repfill_b_f(   pp,(-1),w1);
            else                rowfill_b_f_or(pp,0xff,w1);
          nextbank:
            w1 = w2;
            w2 = 0;
        } while(w1 != 0);
}

static void drawvline(int x,int y,int h,long color)
{
        uint lwdt = CURC->gc_lineoffset;
        long offs = umul32(y,lwdt) + (x >> 3);
        uint mask = 0x80 >> (x & 7);
        setup_far_selector(CURC->gc_selector);
        if(lastcolor != color) {
            outport_w(VGA_GR_CTRL_PORT,writeops[C_OPER(color) & 3]);
            outport_w(VGA_GR_CTRL_PORT,((((int)color & 0x0f) << 8) | VGA_SET_RESET_REG));
            lastcolor = color;
        }
        do {
            char far *pp = &CURC->gc_baseaddr[0][BANKPOS(offs)];
            uint h1 = BANKLFT(offs) / lwdt;
            h -= (h1 = umin(h,umax(h1,1)));
            CHKBANK(BANKNUM(offs));
            offs += (h1 * lwdt);
            colfill_b_f_and(pp,lwdt,mask,h1);
        } while(h != 0);
}

static void drawline(int x,int y,int dx,int dy,long color)
{
        int  mask,cnt,err,yoff = CURC->gc_lineoffset;
        long offs;
        if(dx < 0) {
            x += dx; dx = (-dx);
            y += dy; dy = (-dy);
        }
        if(dy < 0) {
            yoff = (-yoff);
            dy         = (-dy);
        }
        offs = umul32(y,CURC->gc_lineoffset) + (x >> 3);
        mask = 0x80 >> (x & 7);
        setup_far_selector(CURC->gc_selector);
        if(lastcolor != color) {
            outport_w(VGA_GR_CTRL_PORT,writeops[C_OPER(color) & 3]);
            outport_w(VGA_GR_CTRL_PORT,((((int)color & 0x0f) << 8) | VGA_SET_RESET_REG));
            lastcolor = color;
        }
        if(dx > dy) {
            err = (cnt = dx) >> 1;
            do {
                CHKBANK(BANKNUM(offs));
                poke_b_f_and(&CURC->gc_baseaddr[0][BANKPOS(offs)],mask);
                if((err -= dy) < 0) err += dx,offs += yoff;
                if((mask >>= 1) == 0) mask = 0x80,offs++;
            } while(--cnt >= 0);
        }
        else {
            err = (cnt = dy) >> 1;
            do {
                CHKBANK(BANKNUM(offs));
                poke_b_f_and(&CURC->gc_baseaddr[0][BANKPOS(offs)],mask);
                if((err -= dx) < 0) {
                    err += dy;
                    if((mask >>= 1) == 0) mask = 0x80,offs++;
                }
                offs += yoff;
            } while(--cnt >= 0);
        }
}

static
#include "fdrivers/generic/block.c"

static
#include "fdrivers/generic/bitmap.c"

static
#include "fdrivers/generic/pattern.c"

static
#include "fdrivers/generic/bitblt.c"

GrFrameDriver _GrFrameDriverSVGA4 = {
    GR_frameSVGA4,                /* frame mode */
    GR_frameRAM4,                /* compatible RAM frame mode */
    TRUE,                        /* onscreen */
    4,                                /* scan line width alignment */
    4,                                /* number of planes */
    4,                                /* bits per pixel */
    16*1024L*1024L,                /* max plane size the code can handle */
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

