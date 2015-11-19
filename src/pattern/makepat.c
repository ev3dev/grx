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
 **/

#include "libgrx.h"
#include "allocate.h"

#define  BEST_MAX_LINE         128
#define  BEST_MAX_CONTEXT      2048L

/*
 * try to replicate a pixmap for faster bitblt-s
 * in bitplane modes it is especially desirable to replicate until
 * the width is a multiple of 8
 */
static int _GrBestPixmapWidth(int wdt,int hgt)
{
        long total   = grx_get_context_size(wdt,hgt);
        int  linelen = grx_get_line_offset(wdt);
        int  factor  = 1;
        int  test;

        if(total == 0L) return(0);
#ifdef _MAXMEMPLANESIZE
        if(total > _MAXMEMPLANESIZE) return(0);
#endif
        if((test = (int)(BEST_MAX_CONTEXT / total)) > factor)
            factor = test;
        if((test = (BEST_MAX_LINE / linelen)) < factor)
            factor = test;
        while((factor >>= 1) != 0)
            wdt <<= 1;
        return(wdt);
}

GrPattern *GrBuildPixmap(const char *pixels,int w,int h,const GrColorTableP ct)
{
        GrxContext csave,cwork;
        GrPixmap  *result;
        unsigned  char *src;
        int  wdt,wdt2,fullw;
        int  hgt;
        GrxColor color;

        if((fullw = _GrBestPixmapWidth(w,h)) <= 0) return(NULL);
        result = (GrPixmap *)malloc(sizeof(GrPixmap));
        if (result == NULL) return(NULL);

        if (!grx_context_create(fullw,h,NULL,&cwork)) {
          free(result);
          return NULL;
        }
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
        result->pxp_source = cwork.frame;
        result->pxp_source.memory_flags = (GRX_MEMORY_FLAG_MY_CONTEXT | GRX_MEMORY_FLAG_MY_MEMORY);
        result->pxp_ispixmap = TRUE;
        result->pxp_width  = fullw;
        result->pxp_height = h;
        result->pxp_oper   = 0;
        return((GrPattern *)result);
}

GrPattern *GrBuildPixmapFromBits(const char *bits,int w,int h,GrxColor fgc,GrxColor bgc)
{
        GrxContext csave,cwork;
        GrPixmap  *result;
        unsigned  char *src;
        int  wdt,wdt2,fullw;
        int  hgt,mask,byte;

        if((fullw = _GrBestPixmapWidth(w,h)) <= 0) return(NULL);
        result = (GrPixmap *)malloc(sizeof(GrPixmap));
        if(result == NULL) return(NULL);

        if (!grx_context_create(fullw,h,NULL,&cwork)) {
          free(result);
          return NULL;
        }
        csave = *CURC;
        *CURC = cwork;
        fgc &= C_COLOR;
        bgc &= C_COLOR;
        for(hgt = 0; hgt < h; hgt++) {
            for(wdt2 = fullw; (wdt2 -= w) >= 0; ) {
                src  = (unsigned char *)bits;
                mask = byte = 0;
                for(wdt = w; --wdt >= 0; ) {
                    if((mask >>= 1) == 0) { mask = 0x80; byte = *src++; }
                    (*CURC->gc_driver->drawpixel)(wdt2+wdt,hgt,((byte & mask) ? fgc : bgc));
                }
            }
            bits += (w + 7) >> 3;
        }
        *CURC = csave;
        result->pxp_source = cwork.frame;
        result->pxp_source.memory_flags = (GRX_MEMORY_FLAG_MY_CONTEXT | GRX_MEMORY_FLAG_MY_MEMORY);
        result->pxp_ispixmap = TRUE;
        result->pxp_width  = fullw;
        result->pxp_height = h;
        result->pxp_oper   = 0;
        return((GrPattern *)result);
}

GrPattern *GrConvertToPixmap(GrxContext *src)
{
        GrPixmap *result;

        if(src->gc_is_on_screen) return(NULL);
        result = malloc(sizeof(GrPixmap));
        if(result == NULL) return(NULL);
        result->pxp_source = src->frame;
        result->pxp_source.memory_flags = GRX_MEMORY_FLAG_MY_CONTEXT;
        result->pxp_ispixmap = TRUE;
        result->pxp_width  = src->x_max + 1;
        result->pxp_height = src->y_max + 1;
        result->pxp_oper   = 0;
        return((GrPattern *)result);
}


void GrDestroyPattern(GrPattern *p)
{
  if (!p) return;
  if (p->gp_ispixmap) {
    if ( p->gp_pxp_source.memory_flags & GRX_MEMORY_FLAG_MY_MEMORY) {
      int ii;
      for ( ii = p->gp_pxp_source.driver->num_planes; ii > 0; ii-- )
         free(p->gp_pxp_source.base_address[ii - 1]);
    }
    if ( p->gp_pxp_source.memory_flags & GRX_MEMORY_FLAG_MY_CONTEXT )
      free(p);
    return;
  }
  if ( p->gp_bitmap.bmp_memflags ) free(p);
}

