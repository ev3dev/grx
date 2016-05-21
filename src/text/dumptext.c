/*
 * dumptext.c ---- optimized fixed font text drawing
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
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

#include "globals.h"
#include "libgrx.h"
#include "arith.h"
#include "mouse.h"

void grx_text_region_dump_text(const GrxTextRegion *r,int col,int row,int wdt,int hgt)
{
        GrxColorTable fgcp = r->txr_fgcolor.p;
        GrxColorTable bgcp = r->txr_bgcolor.p;
        GrxColor fgcv = r->txr_fgcolor.v;
        GrxColor bgcv = r->txr_bgcolor.v;
        int  undl = (fgcv & GRX_UNDERLINE_TEXT) ? 1 : 0;
        GrxFont *f = r->txr_font;
        char *ptr = r->txr_buffer;
        char *bpt = r->txr_backup;
        int  cofs = GRX_CHAR_TYPE_GET_SIZE(r->txr_chrtype);
        int  offs = r->txr_lineoffset;
        int  fast = bpt ? TRUE : FALSE;
        int  chrw,chrh,bmpw;
        int  xpos,ypos;
        if((f == NULL) || f->h.proportional)   return;
        if((unsigned int)col >= (unsigned int)r->txr_width)  return;
        if((unsigned int)row >= (unsigned int)r->txr_height) return;
        wdt = umin(wdt,(r->txr_width  - col));
        hgt = umin(hgt,(r->txr_height - row));
        if((wdt <= 0) || (hgt <= 0)) return;
        chrw = f->h.width;
        chrh = f->h.height;
        bmpw = (chrw + 7) >> 3;
        xpos = r->txr_xpos + (chrw * col);
        ypos = r->txr_ypos + (chrh * row);
        if(xpos < grx_get_low_x()) {
            int clip = (grx_get_low_x() - xpos + chrw - 1) / chrw;
            if((wdt -= clip) <= 0) return;
            col += clip;
        }
        if(ypos < grx_get_low_y()) {
            int clip = (grx_get_low_y() - ypos + chrh - 1) / chrh;
            if((hgt -= clip) <= 0) return;
            row += clip;
        }
        xpos = r->txr_xpos + (chrw * (col + wdt)) - 1;
        ypos = r->txr_ypos + (chrh * (row + hgt)) - 1;
        if(xpos > grx_get_high_x()) {
            int clip = (xpos - grx_get_high_x() + chrw - 1) / chrw;
            if((wdt -= clip) <= 0) return;
        }
        if(ypos > grx_get_high_y()) {
            int clip = (ypos - grx_get_high_y() + chrh - 1) / chrh;
            if((hgt -= clip) <= 0) return;
        }
        ptr += (row * offs) + (col * cofs);
        bpt += (row * offs) + (col * cofs);
        xpos = r->txr_xpos  + (col * chrw);
        ypos = r->txr_ypos  + (row * chrh);
        mouse_block(
            CURC,
            xpos,ypos,
            (xpos + (wdt * chrw) - 1),
            (ypos + (hgt * chrh) - 1)
        );
        for( ; --hgt >= 0; ptr += offs,bpt += offs,ypos += chrh) {
            char *pt2 = ptr;
            char *bp2 = bpt;
            int   wd2 = wdt;
            int   xp2 = xpos;
            for( ; --wd2 >= 0; pt2 += cofs,bp2 += cofs,xp2 += chrw) {
                int  chr,attr;
                char *bmp;
                switch(r->txr_chrtype) {
                  case GRX_CHAR_TYPE_WORD:
                    chr = *((unsigned short *)(pt2));
                    if(fast) {
                        if(*((unsigned short *)(bp2)) == chr) continue;
                        *((unsigned short *)(bp2)) = chr;
                    }
                    break;
                  case GRX_CHAR_TYPE_ATTR:
                    chr = *((unsigned short *)(pt2));
                    if(fast) {
                        if(*((unsigned short *)(bp2)) == chr) continue;
                        *((unsigned short *)(bp2)) = chr;
                    }
                    attr = GRX_CHAR_TYPE_GET_ATTR(GRX_CHAR_TYPE_ATTR, chr);
                    chr  = GRX_CHAR_TYPE_GET_CODE(GRX_CHAR_TYPE_ATTR, chr);
                    fgcv = GRX_COLOR_TABLE_GET_COLOR(fgcp,GR_ATTR_FGCOLOR(attr));
                    bgcv = GRX_COLOR_TABLE_GET_COLOR(bgcp,GR_ATTR_BGCOLOR(attr));
                    undl = GR_ATTR_UNDERLINE(attr);
                    break;
                  default:
                    chr = *((unsigned char *)(pt2));
                    if(fast) {
                        if(*((unsigned char *)(bp2)) == chr) continue;
                        *((unsigned char *)(bp2)) = chr;
                    }
                    break;
                }
                bmp = grx_font_get_char_aux_bmp(f,chr,GRX_TEXT_DIRECTION_RIGHT,undl);
                if(bmp) (*FDRV->drawbitmap)(
                    (xp2  + CURC->x_offset),
                    (ypos + CURC->y_offset),
                    chrw,chrh,
                    bmp,bmpw,0,
                    fgcv,bgcv
                );
                else (*FDRV->drawblock)(
                    (xp2  + CURC->x_offset),
                    (ypos + CURC->y_offset),
                    chrw,chrh,
                    bgcv
                );
            }
        }
        mouse_unblock();
}

void grx_text_region_dump(const GrxTextRegion *r)
{
        grx_text_region_dump_text(r,0,0,r->txr_width,r->txr_height);
}

void grx_text_region_dump_char(const GrxTextRegion *r,int chr,int col,int row)
{
        int offs;
        if((unsigned int)col >= (unsigned int)r->txr_width)  return;
        if((unsigned int)row >= (unsigned int)r->txr_height) return;
        switch(r->txr_chrtype) {
          case GRX_CHAR_TYPE_WORD:
          case GRX_CHAR_TYPE_ATTR:
            offs = (row * r->txr_lineoffset) + (col * sizeof(short));
            *((short *)((char *)r->txr_buffer + offs)) = chr;
            break;
          default:
            offs = (row * r->txr_lineoffset) + (col * sizeof(char));
            *((char *)((char *)r->txr_buffer + offs)) = chr;
            break;
        }
        grx_text_region_dump_text(r,col,row,1,1);
}
