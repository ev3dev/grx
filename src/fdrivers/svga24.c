/**
 ** SVGA24.C ---- the 16M color Super VGA frame driver
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 **/

#include "grdriver.h"
#include "libgrx.h"
#include "access24.h"
#include "arith.h"
#include "mempeek.h"

#if BYTE_ORDER!=HARDWARE_BYTE_ORDER
#error Mismatching byte order between ram and video ram !
#endif

#define US(x) ( *( (ushort *) &(x) ) )

static
#ifdef __GNUC__
inline
#endif
long readpixel(GrFrame *c,int x,int y)
{
        long cval   = 0L;
        long offs   = umul32(y,SCRN->gc_lineoffset) + (x + x + x);
        int  bank   = BANKNUM(offs);
        char far *p = &SCRN->gc_baseaddr[0][BANKPOS(offs)];
        setup_far_selector(SCRN->gc_selector);
        CHKBANK(bank);
        switch (BANKLFT(offs)) {
          case 1 : C24BYTE(cval,0) = peek_b_f(p);
                   SETBANK(++bank);
                   p = SCRN->gc_baseaddr[0];
                   US(C24BYTE(cval,1)) = peek_w_f(p);
                   return cval;
          case 2 : US(C24BYTE(cval,0)) = peek_w_f(p);
                   SETBANK(++bank);
                   p = SCRN->gc_baseaddr[0];
                   C24BYTE(cval,2) = peek_b_f(p);
                   return cval;
        }
        return peek_24_f(p);
}

static
#ifdef __GNUC__
inline
#endif
void drawpixel(int x,int y,long color)
{
        long cmask;
        long offs   = umul32(y,CURC->gc_lineoffset) + (x + x + x);
        int  bank   = BANKNUM(offs);
        char far *p = &CURC->gc_baseaddr[0][BANKPOS(offs)];
        switch(C_OPER(color)) {
            case C_XOR: cmask = -1L;                break;
            case C_OR:  cmask = ~color;             break;
            case C_AND: cmask =  color; color = 0L; break;
            default:    cmask =  0L;                break;
        }
        setup_far_selector(CURC->gc_selector);
        CHKBANK(bank);
        switch (BANKLFT(offs)) {
          case 1 : poke_b_f(p,((peek_b_f(p) & C24BYTE(cmask,0)) ^ C24BYTE(color,0)));
                   SETBANK(++bank);
                   p = SCRN->gc_baseaddr[0];
                   poke_w_f(p,((peek_w_f(p) & US(C24BYTE(cmask,1))) ^ US(C24BYTE(color,1))));
                   return;
          case 2 : poke_w_f(p,((peek_w_f(p) & US(C24BYTE(cmask,0))) ^ US(C24BYTE(color,0))));
                   SETBANK(++bank);
                   p = SCRN->gc_baseaddr[0];
                   goto lastbyte;
        }
        poke_w_f(p,((peek_w_f(p) & US(C24BYTE(cmask,0))) ^ US(C24BYTE(color,0))));
        ptrinc(p,2);
lastbyte:
        poke_b_f(p,((peek_b_f(p) & C24BYTE(cmask,2)) ^ C24BYTE(color,2)));
}


static void drawhline(int x,int y,int w,long color)
{
      ulong *f, five[5];
      int   bytes;
      uint  ww;
      int op =  C_OPER(color);
      long offs = umul32(y,CURC->gc_lineoffset) + (x + x + x);
      setup_far_selector(CURC->gc_selector);

      /* make sure we don't run in problems on first pixel */
      if (BANKLFT(offs) < 3) {
        _GrFrameDriverSVGA24.drawpixel(x,y,color);
        offs += 3;
        if ( !(--w) ) return ;
      };
      /* now prepare fast filling */
      w = w + w + w;
      bytes = (int)(offs & 3);
      if (!bytes) bytes = 4;
      FillFive(five, color, bytes);
      if (bytes!=4) {
          /* we need to write up to 3 bytes for correct alignment */
          /* can't run into a bank switch here */
          char far *p = &CURC->gc_baseaddr[0][BANKPOS(offs)];
          uchar   *cp = ((uchar *)five) + bytes;
          CHKBANK(BANKNUM(offs));
          bytes = 4-bytes;
          w    -= bytes;
          offs += bytes;
          switch (op) {
              case C_XOR: if (bytes&1) { poke_b_f_xor(p,*cp);
                                         ptrinc(p,1); ptrinc(cp,1);      }
                          if (bytes&2) poke_w_f_xor(p,*((ushort *)cp));
                          break;
              case C_OR:  if (bytes&1) { poke_b_f_or(p,*cp);
                                        ptrinc(p,1); ptrinc(cp,1); }
                          if (bytes&2) poke_w_f_or(p,*((ushort *)cp));
                          break;
              case C_AND: if (bytes&1) { poke_b_f_and(p,*cp);
                                         ptrinc(p,1); ptrinc(cp,1); }
                          if (bytes&2) poke_w_f_and(p,*((ushort *)cp));
                          break;
              default:    if (bytes&1) { poke_b_f(p,*cp);
                                         ptrinc(p,1); ptrinc(cp,1); }
                          if (bytes&2) poke_w_f(p,*((ushort *)cp));
                          break;
          }
          if (!w) return;
      }
      /* now we're ready for fast 32bit fill ... */
      ww = 0;
      f = NULL;
      for (;;) {
          char far *p = &CURC->gc_baseaddr[0][BANKPOS(offs)];
          CHKBANK(BANKNUM(offs));
          if (ww) {
            /* did bankswitch before all 3 longs were done */
            /* can't reach end of bank here ! */
            ww = umin(w,ww);
            offs += ww;
            w    -= ww;
            switch (op) {
              case C_XOR: rowfill_24s_f_xor(p,f,ww); break;
              case C_OR:  rowfill_24s_f_or( p,f,ww); break;
              case C_AND: rowfill_24s_f_and(p,f,ww); break;
              default:    rowfill_24s_f(    p,f,ww); break;
            }
            if (!w) return;
          }
          ww = BANKLFT(offs);
          ww = umin(w,ww);
          offs += ww;
          w    -= ww;
          f = &five[1];
          switch (op) {
              case C_XOR: rowfill_24_f_xor(p,f,ww); break;
              case C_OR:  rowfill_24_f_or( p,f,ww); break;
              case C_AND: rowfill_24_f_and(p,f,ww); break;
              default:    rowfill_24_f(    p,f,ww); break;
          }
          if (!w) return;
          ww = (int)ptrdiff(&five[4], f);
          if (ww > 8) ww = 0;
      }
}


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

static void bitblt(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,long op)
{
        if(GrColorMode(op) == GrIMAGE) _GrFrDrvGenericBitBlt(
            dst,dx,dy,
            src,sx,sy,
            w,h,
            op
        );
        else _GrFrDrvPackedBitBltV2V(
            dst,(dx * 3),dy,
            src,(sx * 3),sy,
            (w * 3),h,
            op
        );
}

static void bltv2r(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,long op)
{
        if(GrColorMode(op) == GrIMAGE) _GrFrDrvGenericBitBlt(
            dst,dx,dy,
            src,sx,sy,
            w,h,
            op
        );
        else _GrFrDrvPackedBitBltV2R(
            dst,(dx * 3),dy,
            src,(sx * 3),sy,
            (w * 3),h,
            op
        );
}

static void bltr2v(GrFrame *dst,int dx,int dy,GrFrame *src,int sx,int sy,int w,int h,long op)
{
        if(GrColorMode(op) == GrIMAGE) _GrFrDrvGenericBitBlt(
            dst,dx,dy,
            src,sx,sy,
            w,h,
            op
        );
        else _GrFrDrvPackedBitBltR2V(
            dst,(dx * 3),dy,
            src,(sx * 3),sy,
            (w * 3),h,
            op
        );
}

GrFrameDriver _GrFrameDriverSVGA24 = {
    GR_frameSVGA24,             /* frame mode */
    GR_frameRAM24,              /* compatible RAM frame mode */
    TRUE,                       /* onscreen */
    4,                          /* line width alignment */
    1,                          /* number of planes */
    24,                         /* bits per pixel */
    24*16*1024L*1024L,          /* max plane size the code can handle */
    NULL,
    readpixel,
    drawpixel,
    drawline,
    drawhline,
    drawvline,
    drawblock,
    drawbitmap,
    drawpattern,
    bitblt,
    bltv2r,
    bltr2v
};

