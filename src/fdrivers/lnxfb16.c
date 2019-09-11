/**
 ** lnxfb16.c ---- the 16bpp linux frame buffer driver
 **
 ** Copyright (c) 2019 Mariano Alvarez Fernandez
 ** [e-mail: malfer@telefonica.net]
 **
 ** This file is part of the GRX graphics library.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** Reading individual pixels from the linux fb is a slow operation, so the
 ** idea of this framedriver is minimize and group reads. Probably using
 ** a double buffer will be better because you have never to read from the
 ** fb, but it doubles the memory required.
 **
 ** This driver doesn't use all the memfill, mempeek infraestructure, only
 ** C standard bit operations and C standard functions.
 **/

#include "libgrx.h"
#include "grdriver.h"
#include "arith.h"
#include "allocate.h"

/* In some systems fb is very slow if more than 4kbyte are read/write 
 * at once (I don't know why), so we decompose big read/write in small
 * chunks */
#define MAXPIX_RDWR_ATONCE 1000

/* frame offset address calculation */
#define FOFS(x,y,lo) umuladd32((y),(lo),((x)<<1))

static INLINE
GrColor readpixel(GrFrame *c, int x, int y)
{
    GR_int16u *ptr;

    GRX_ENTER();
    ptr  = (GR_int16u *)&c->gf_baseaddr[0][FOFS(x,y,c->gf_lineoffset)];
    GRX_RETURN((GrColor)*ptr);
}

static INLINE
void drawpixel(int x, int y, GrColor color)
{
    GR_int16u *ptr, xcolor;
    int op;

    GRX_ENTER();
    ptr = (GR_int16u *)&CURC->gc_baseaddr[0][FOFS(x,y,CURC->gc_lineoffset)];
    op = C_OPER(color);
    xcolor = color & 0xFFFF;
    switch(op) {
        case C_XOR: *ptr ^= xcolor; break;
        case C_OR:  *ptr |= xcolor; break;
        case C_AND: *ptr &= xcolor; break;
        default:    *ptr = xcolor; break;
    }
    GRX_LEAVE();
}

static void getfline(GrFrame *c, int x, int y, int w, GR_int16u *buf)
{
    GR_int16u *ptr;

    ptr = (GR_int16u *)&c->gf_baseaddr[0][FOFS(x,y,c->gf_lineoffset)];
    while (w > MAXPIX_RDWR_ATONCE) {
        memcpy((void *)buf, (void *)ptr, sizeof(GR_int16u)*MAXPIX_RDWR_ATONCE);
        x += MAXPIX_RDWR_ATONCE;
        w -= MAXPIX_RDWR_ATONCE;
        ptr += MAXPIX_RDWR_ATONCE;
        buf += MAXPIX_RDWR_ATONCE;
    }
    memcpy((void *)buf, (void *)ptr, sizeof(GR_int16u)*w);
}

static void putfline(GrFrame *c, int x, int y, int w, GR_int16u *buf)
{
    GR_int16u *ptr;

    ptr = (GR_int16u *)&c->gf_baseaddr[0][FOFS(x,y,c->gf_lineoffset)];
    while (w > MAXPIX_RDWR_ATONCE) {
        memcpy((void *)ptr, (void *)buf, sizeof(GR_int16u)*MAXPIX_RDWR_ATONCE);
        x += MAXPIX_RDWR_ATONCE;
        w -= MAXPIX_RDWR_ATONCE;
        ptr += MAXPIX_RDWR_ATONCE;
        buf += MAXPIX_RDWR_ATONCE;
    }
    memcpy((void *)ptr, (void *)buf, sizeof(GR_int16u)*w);
}

static void drawvline(int x, int y, int h, GrColor color)
{
    unsigned lwdt;
    GR_int16u *ptr, xcolor;
    int op, i;

    GRX_ENTER();
    lwdt = (CURC->gc_lineoffset) >> 1;
    ptr = (GR_int16u *)&CURC->gc_baseaddr[0][FOFS(x,y,CURC->gc_lineoffset)];
    op = C_OPER(color);
    xcolor = color & 0xFFFF;
    switch(op) {
        case C_XOR: for (i=0; i<h; i++) {*ptr ^= xcolor; ptr += lwdt;} break;
        case C_OR:  for (i=0; i<h; i++) {*ptr |= xcolor; ptr += lwdt;} break;
        case C_AND: for (i=0; i<h; i++) {*ptr &= xcolor; ptr += lwdt;} break;
        default:    for (i=0; i<h; i++) {*ptr = xcolor; ptr += lwdt;} break;
    }
    GRX_LEAVE();
}

static void drawhline(int x, int y, int w, GrColor color)
{
    GR_int16u buf[w];
    GR_int16u xcolor;
    int op, i;

    GRX_ENTER();
    op   = C_OPER(color);
    xcolor= color & 0xFFFF;
    if (op != C_WRITE) getfline(&CURC->gc_frame, x, y, w, buf);
    switch(op) {
        case C_XOR: for (i=0; i<w; i++) {buf[i] ^= xcolor;} break;
        case C_OR:  for (i=0; i<w; i++) {buf[i] |= xcolor;} break;
        case C_AND: for (i=0; i<w; i++) {buf[i] &= xcolor;} break;
        default:    for (i=0; i<w; i++) {buf[i] = xcolor;} break;
    }
    putfline(&CURC->gc_frame, x, y, w, buf);
    GRX_LEAVE();
}

static void drawblock(int x, int y, int w, int h, GrColor color)
{
    int j;
    
    GRX_ENTER();
    for (j=0; j<h; j++) {
        drawhline(x,y,w,color);
        y++;
    }
    GRX_LEAVE();
}

static void drawline(int x, int y, int dx, int dy, GrColor color)
{
    GR_int16u *ptr, xcolor;
    int cnt, err, yoff, op;
    
    GRX_ENTER();
    op = C_OPER(color);
    xcolor = color & 0xFFFF;

    yoff = 1;
    if (dx < 0) {
        x += dx; dx = (-dx);
        y += dy; dy = (-dy);
    }
    if (dy < 0) {
        yoff = (-1);
        dy   = (-dy);
    }
    if (dx > dy) {
        err = (cnt = dx) >> 1;
        do {
            ptr = (GR_int16u *)&CURC->gc_baseaddr[0][FOFS(x,y,CURC->gc_lineoffset)];
            switch(op) {
                case C_XOR: *ptr ^= xcolor; break;
                case C_OR:  *ptr |= xcolor; break;
                case C_AND: *ptr &= xcolor; break;
                default:    *ptr = xcolor; break;
            }
            if ((err -= dy) < 0) err += dx,y += yoff;
            x++;
        } while(--cnt >= 0);
    }
    else {
        err = (cnt = dy) >> 1;
        do {
            ptr = (GR_int16u *)&CURC->gc_baseaddr[0][FOFS(x,y,CURC->gc_lineoffset)];
            switch(op) {
                case C_XOR: *ptr ^= xcolor; break;
                case C_OR:  *ptr |= xcolor; break;
                case C_AND: *ptr &= xcolor; break;
                default:    *ptr = xcolor; break;
            }
            if ((err -= dx) < 0) err += dy,x++;
            y += yoff;
        } while(--cnt >= 0);
    }
    GRX_LEAVE();
}

static void drawbitmap(int x, int y, int w, int h, char *bmp, int pitch,
                       int start, GrColor fg, GrColor bg)
{
    GR_int16u buf[w];
    GR_int16u xcolorfg, xcolorbg;
    int opfg, opbg;
    unsigned char *bitp;
    unsigned char bits;
    unsigned char mask;
    int xx;

    GRX_ENTER();
    opfg = C_OPER(fg);
    xcolorfg = fg & 0xFFFF;
    if (fg == GrNOCOLOR) opfg = -1; // special value to do nothing in switch
    opbg = C_OPER(bg);
    xcolorbg = bg & 0xFFFF;
    if (bg == GrNOCOLOR) opbg = -1; // special value to do nothing in switch

    h += y;
    bmp += (unsigned int)start >> 3;
    start &= 7;
    do {
        bitp = (unsigned char *)bmp;
        bits = *bitp;
        mask = 0x80 >> start;
        xx = 0;
        if (opfg != C_WRITE || opbg != C_WRITE)
            getfline(&CURC->gc_frame, x, y, w, buf);
        do {
            if (bits & mask) {
                switch(opfg) {
                    case C_XOR:   buf[xx] ^= xcolorfg; break;
                    case C_OR:    buf[xx] |= xcolorfg; break;
                    case C_AND:   buf[xx] &= xcolorfg; break;
                    case C_WRITE: buf[xx] = xcolorfg; break;
                }
            } else {
                switch(opbg) {
                    case C_XOR:   buf[xx] ^= xcolorbg; break;
                    case C_OR:    buf[xx] |= xcolorbg; break;
                    case C_AND:   buf[xx] &= xcolorbg; break;
                    case C_WRITE: buf[xx] = xcolorbg; break;
                }
            }
            if ((mask >>= 1) == 0) bits = *++bitp, mask = 0x80;
        } while (++xx != w);
        putfline(&CURC->gc_frame, x, y, w, buf);
        bmp += pitch;
    } while (++y != h);
    GRX_LEAVE();
}

static void drawpattern(int x, int y, int w, char patt, GrColor fg, GrColor bg)
{
    //no mejorada por no tener caso de uso
    GR_int8u mask;

    GRX_ENTER();
    mask = 0x80;
    w += x;
    do {
        drawpixel(x, y, (patt & mask) ? fg : bg);
        if((mask >>= 1) == 0) mask = 0x80;
    } while (++x != w);
    GRX_LEAVE();
}

static void bitbltv2v(GrFrame *dst, int dx, int dy,
                      GrFrame *src, int sx, int sy,
                      int w, int h, GrColor op)
{
    GR_int16u *sbuf, *dbuf = NULL;
    GR_int16u skipc;
    int op2, i, j;
    int sypos, dypos, incr;

    op2 = C_OPER(op);
    skipc = op & 0xFFFF;

    sbuf = malloc(sizeof(GR_int16u)*w);
    if (sbuf == NULL) return;
    
    if (op2 != C_WRITE) {
        dbuf = malloc(sizeof(GR_int16u)*w);
        if (dbuf == NULL) {
            free(sbuf);
            return;
        }
    }

    // can overlap so check for blit direction
    if(sy <= dy) {
        sypos = sy + h - 1;
        dypos = dy + h - 1;
        incr = -1;
    } else {
        sypos = sy;
        dypos = dy;
        incr = 1;
    }
    
    for (i=0; i<h; i++) {
        getfline(src, sx, sypos, w, sbuf);
        if (op2 != C_WRITE) getfline(dst, dx, dypos, w, dbuf);
        switch(op2) {
            case C_XOR:
                for (j=0; j<w; j++) dbuf[j] ^= sbuf[j];
                putfline(dst, dx, dypos, w, dbuf);
                break;
            case C_OR:
                for (j=0; j<w; j++) dbuf[j] |= sbuf[j];
                putfline(dst, dx, dypos, w, dbuf);
                break;
            case C_AND:
                for (j=0; j<w; j++) dbuf[j] &= sbuf[j];
                putfline(dst, dx, dypos, w, dbuf);
                break;
            case C_IMAGE:
                for (j=0; j<w; j++)
                    if (sbuf[j] != skipc)
                        dbuf[j] =sbuf[j];
                putfline(dst, dx, dypos, w, dbuf);
                break;
            default:
                putfline(dst, dx, dypos, w, sbuf); // C_WRITE, so sbuf here
                break;
        }
        sypos += incr;
        dypos += incr;
    }
    
    if (op2 != C_WRITE) free(dbuf);
    free(sbuf);
}

static void bitbltv2r(GrFrame *dst, int dx, int dy,
                      GrFrame *src, int sx, int sy,
                      int w, int h, GrColor op)
{
    GR_int16u *sbuf, *dbuf;
    GR_int16u skipc;
    int op2, i, j;

    op2 = C_OPER(op);
    skipc = op & 0xFFFF;

    sbuf = malloc(sizeof(GR_int16u)*w);
    if (sbuf == NULL) return;

    for (i=0; i<h; i++) {
        getfline(src, sx, sy+i, w, sbuf);
        dbuf = (GR_int16u *)&dst->gf_baseaddr[0][FOFS(dx,dy+i,dst->gf_lineoffset)];
        switch(op2) {
            case C_XOR:
                for (j=0; j<w; j++) dbuf[j] ^= sbuf[j];
                break;
            case C_OR:
                for (j=0; j<w; j++) dbuf[j] |= sbuf[j];
                break;
            case C_AND:
                for (j=0; j<w; j++) dbuf[j] &= sbuf[j];
                break;
            case C_IMAGE:
                for (j=0; j<w; j++)
                    if (sbuf[j] != skipc)
                        dbuf[j] =sbuf[j];
                break;
            default:
                memcpy((void *)dbuf, (void *)sbuf, sizeof(GR_int16u)*w);
                break;
        }
    }
    
    free(sbuf);
}

static void bitbltr2v(GrFrame *dst, int dx, int dy,
                      GrFrame *src, int sx, int sy,
                      int w, int h, GrColor op)
{
    GR_int16u *sbuf, *dbuf = NULL;
    GR_int16u skipc;
    int op2, i, j;

    op2 = C_OPER(op);
    skipc = op & 0xFFFF;

    if (op2 != C_WRITE) {
        dbuf = malloc(sizeof(GR_int16u)*w);
        if (dbuf == NULL) return;
    }

    for (i=0; i<h; i++) {
        sbuf = (GR_int16u *)&src->gf_baseaddr[0][FOFS(sx,sy+i,src->gf_lineoffset)];
        if (op2 != C_WRITE) getfline(dst, dx, dy+i, w, dbuf);
        switch(op2) {
            case C_XOR:
                for (j=0; j<w; j++) dbuf[j] ^= sbuf[j];
                putfline(dst, dx, dy+i, w, dbuf);
                break;
            case C_OR:
                for (j=0; j<w; j++) dbuf[j] |= sbuf[j];
                putfline(dst, dx, dy+i, w, dbuf);
                break;
            case C_AND:
                for (j=0; j<w; j++) dbuf[j] &= sbuf[j];
                putfline(dst, dx, dy+i, w, dbuf);
                break;
            case C_IMAGE:
                for (j=0; j<w; j++)
                    if (sbuf[j] != skipc)
                        dbuf[j] =sbuf[j];
                putfline(dst, dx, dy+i, w, dbuf);
                break;
            default:
                putfline(dst, dx, dy+i, w, sbuf); // C_WRITE, so sbuf here
                break;
        }
    }
    
    if (op2 != C_WRITE) free(dbuf);
}

static
GrColor *getindexedscanline(GrFrame *c, int x, int y, int w, int *indx)
{
    GR_int16u buf[w];
    GrColor *pixels;
    int i;

    if (indx) return _GrFrDrvGenericGetIndexedScanline(c,x,y,w,indx);
    
    GRX_ENTER();
    pixels = _GrTempBufferAlloc(sizeof(GrColor) * (w+1));
    if (pixels) {
        getfline(c, x, y, w, buf);
        for (i=0; i<w; i++) {
            pixels[i] = buf[i];
        }
    }
    GRX_RETURN(pixels);
}

static
void putscanline(int x, int y, int w, const GrColor *scl, GrColor op)
{
    GR_int16u buf[w];
    GrColor skipc;
    int op2, i;

    GRX_ENTER();
    op2 = C_OPER(op);
    skipc = op & GrCVALUEMASK;
    if (op2 != C_WRITE)
        getfline(&CURC->gc_frame, x, y, w, buf);

    switch(op2) {
        case C_XOR: for (i=0; i<w; i++) {buf[i] ^= (scl[i] & 0xFFFF);} break;
        case C_OR:  for (i=0; i<w; i++) {buf[i] |= (scl[i] & 0xFFFF);} break;
        case C_AND: for (i=0; i<w; i++) {buf[i] &= (scl[i] & 0xFFFF);} break;
        case C_IMAGE:
            for (i=0; i<w; i++) {
                if (scl[i] != skipc)
                    buf[i] = (scl[i] & 0xFFFF);
            }
            break;
        default:    for (i=0; i<w; i++) {buf[i] = (scl[i] & 0xFFFF);} break;
    }
    putfline(&CURC->gc_frame, x, y, w, buf);
    GRX_LEAVE();
}

GrFrameDriver _GrFrameDriverLNXFB_16 = {
    GR_frameLNXFB_16,           /* frame mode */
    GR_frameRAM16,              /* compatible RAM frame mode */
    TRUE,                       /* onscreen */
    4,                          /* line width alignment */
    1,                          /* number of planes */
    16,                         /* bits per pixel */
    16*16*1024L*1024L,          /* max plane size the code can handle */
    NULL,
    readpixel,
    drawpixel,
    drawline,
    drawhline,
    drawvline,
    drawblock,
    drawbitmap,
    drawpattern,
    bitbltv2v,
    bitbltv2r, //v2r
    bitbltr2v, //r2v
    getindexedscanline,
    putscanline
};
