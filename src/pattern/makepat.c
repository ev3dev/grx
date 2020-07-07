/**
 ** makepat.c
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu]
 **
 ** Copyright (C) 1992, Csaba Biegl
 **   820 Stirrup Dr, Nashville, TN, 37221
 **   csaba@vuse.vanderbilt.edu
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
 ** 200620 M.Alvarez, solved old bugs handling memflags and generating
 **                   pixmaps from bits
 **
 **/

#include "libgrx.h"

#define  BEST_MAX_LINE         128
#define  BEST_MAX_CONTEXT      2048L

GrPixmap *_GrCreatePixmap(GrContext *ctx, int w,int h)
{
    GrPixmap *pix = NULL;
    
    GRX_ENTER();
    if (GrContextSize(w, h) <= 0) goto done;
    if (!GrCreateContext(w, h, NULL, ctx)) goto done;
    pix = (GrPixmap *)malloc(sizeof(GrPixmap));
    if ( !pix ) {
        GrDestroyContext(ctx);
        goto done;
    }
    pix->pxp_source = ctx->gc_frame;
    pix->pxp_source.gf_memflags =  (MGRX_GF_MYCONTEXT | MGRX_GF_MYFRAME);
    pix->pxp_ptype = GR_PTYPE_PIXMAP;
    pix->pxp_width  = w;
    pix->pxp_height = h;
    pix->pxp_oper   = 0;

done:
    GRX_RETURN(pix);
}

/*
 * try to replicate a pixmap for faster bitblt-s
 * in bitplane modes it is especially desirable to replicate until
 * the width is a multiple of 8
 */
static int _GrBestPixmapWidth(int wdt,int hgt)
{
    long total   = GrContextSize(wdt,hgt);
    int  linelen = GrLineOffset(wdt);
    int  factor  = 1;
    int  test;

    if(total == 0L) return(0);
    if((test = (int)(BEST_MAX_CONTEXT / total)) > factor)
        factor = test;
    if((test = (BEST_MAX_LINE / linelen)) < factor)
        factor = test;
    while((factor >>= 1) != 0)
        wdt <<= 1;
    return(wdt);
}

GrPattern *GrBuildPixmapNR(const char *pixels,int w,int h,const GrColorTableP ct)
{
    GrContext csave,cwork;
    GrPixmap  *result = NULL;
    unsigned  char *src;
    int  wdt;
    int  hgt;
    GrColor color;

    GRX_ENTER();
    result = _GrCreatePixmap(&cwork,w,h);
    if (result == NULL) goto bad;

    csave = *CURC;
    *CURC = cwork;
    for(hgt = 0; hgt < h; hgt++) {
        src = (unsigned char *)pixels;
        for(wdt = 0; wdt < w; wdt++) {
            color = *src++;
            if(ct != NULL) color = GR_CTABLE_COLOR(ct,color);
            (*CURC->gc_driver->drawpixel)(wdt,hgt,(color & C_COLOR));
        }
        pixels += w;
    }
    *CURC = csave;
    GRX_RETURN((GrPattern *)result);

bad:
    GRX_RETURN(NULL);
}

GrPattern *GrBuildPixmap(const char *pixels,int w,int h,const GrColorTableP ct)
{
    GrContext csave,cwork;
    GrPixmap  *result = NULL;
    unsigned  char *src;
    int  wdt,wdt2,fullw;
    int  hgt;
    GrColor color;

    GRX_ENTER();
    if ((fullw = _GrBestPixmapWidth(w,h)) <= 0) goto bad;
    result = _GrCreatePixmap(&cwork,fullw,h);
    if (result == NULL) goto bad;

    csave = *CURC;
    *CURC = cwork;
    for(hgt = 0; hgt < h; hgt++) {
        for(wdt2 = fullw; (wdt2 -= w) >= 0; ) {
            src = (unsigned char *)pixels;
            for(wdt = 0; wdt < w; wdt++) {
                color = *src++;
                if(ct != NULL) color = GR_CTABLE_COLOR(ct,color);
                (*CURC->gc_driver->drawpixel)(wdt2+wdt,hgt,(color & C_COLOR));
            }
        }
        pixels += w;
    }
    *CURC = csave;
    GRX_RETURN((GrPattern *)result);

bad:
    GRX_RETURN(NULL);
}

GrPattern *GrBuildPixmapFromBitsNR(const char *bits,int w,int h,GrColor fgc,GrColor bgc)
{
    GrContext csave,cwork;
    GrPixmap  *result = NULL;
    unsigned  char *src;
    int  wdt,hgt;
    int  mask,byte;

    GRX_ENTER();
    result = _GrCreatePixmap(&cwork,w,h);
    if (result == NULL) goto bad;

    csave = *CURC;
    *CURC = cwork;
    fgc &= C_COLOR;
    bgc &= C_COLOR;
    for(hgt = 0; hgt < h; hgt++) {
        src  = (unsigned char *)bits;
        mask = byte = 0;
        for(wdt = 0; wdt < w; wdt++) {
            if((mask >>= 1) == 0) { mask = 0x80; byte = *src++; }
            (*CURC->gc_driver->drawpixel)(wdt,hgt,((byte & mask) ? fgc : bgc));
        }
        bits += (w + 7) >> 3;
    }
    *CURC = csave;
    GRX_RETURN((GrPattern *)result);

bad:
    GRX_RETURN(NULL);
}

GrPattern *GrBuildPixmapFromBits(const char *bits,int w,int h,GrColor fgc,GrColor bgc)
{
    GrContext csave,cwork;
    GrPixmap  *result = NULL;
    unsigned  char *src;
    int  wdt,wdt2,fullw;
    int  hgt,mask,byte;

    GRX_ENTER();
    if((fullw = _GrBestPixmapWidth(w,h)) <= 0) goto bad;
    result = _GrCreatePixmap(&cwork,fullw,h);
    if (result == NULL) goto bad;

    csave = *CURC;
    *CURC = cwork;
    fgc &= C_COLOR;
    bgc &= C_COLOR;
    for(hgt = 0; hgt < h; hgt++) {
        for(wdt2 = fullw; (wdt2 -= w) >= 0; ) {
            src  = (unsigned char *)bits;
            mask = byte = 0;
            //for(wdt = w; --wdt >= 0; ) { // this is a bug
            for(wdt = 0; wdt < w; wdt++) {
                if((mask >>= 1) == 0) { mask = 0x80; byte = *src++; }
                (*CURC->gc_driver->drawpixel)(wdt2+wdt,hgt,((byte & mask) ? fgc : bgc));
            }
        }
        bits += (w + 7) >> 3;
    }
    *CURC = csave;
    GRX_RETURN((GrPattern *)result);

bad:
    GRX_RETURN(NULL);
}

GrPattern *GrConvertToPixmap(GrContext *src)
{
    GrPixmap *result = NULL;

    GRX_ENTER();
    if(src->gc_onscreen) goto bad;
    result = malloc(sizeof(GrPixmap));
    if(result == NULL) goto bad;;
    result->pxp_source = src->gc_frame;
    result->pxp_source.gf_memflags = MGRX_GF_MYCONTEXT;
    result->pxp_ptype = GR_PTYPE_PIXMAP;
    result->pxp_width  = src->gc_xmax + 1;
    result->pxp_height = src->gc_ymax + 1;
    result->pxp_oper   = 0;
    GRX_RETURN((GrPattern *)result);

bad:
    GRX_RETURN(NULL);
}

void GrDestroyPattern(GrPattern *p)
{
    GRX_ENTER();
    if (p) {
        if (p->gp_ptype == GR_PTYPE_PIXMAP) {
            if ( p->gp_pxp_source.gf_memflags & MGRX_GF_MYFRAME) {
                int ii;
                for ( ii = p->gp_pxp_source.gf_driver->num_planes; ii > 0; ii-- )
                free(p->gp_pxp_source.gf_baseaddr[ii - 1]);
            }
            if ( p->gp_pxp_source.gf_memflags & MGRX_GF_MYCONTEXT )
                free(p);
        } else if(p->gp_ptype == GR_PTYPE_BITMAP) {
            if ( p->gp_bitmap.bmp_memflags ) free(p);
        } else if(p->gp_ptype == GR_PTYPE_GRADIENT) {
            if ( p->gp_gradient.grd_memflags ) free(p);
        }
    }
    GRX_LEAVE();
}
