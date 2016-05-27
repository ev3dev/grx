/*
 * drawstrg.c ---- low level character string output
 *
 * Copyright (c) 1998 Hartmut Schirmer
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
 */

#include "globals.h"
#include "libgrx.h"
#include "clipping.h"
#include "mouse.h"
#include "text.h"

int _GR_textattrintensevideo = 0;

void _GrDrawString(const void *text,int length,int x,int y,
                   const GrxTextOptions *opt, GrxPattern *p, TextDrawBitmapFunc dbm)
{
    GrxFont *f;
    int    x1;
    GRX_ENTER();

    if (  ((f = opt->txo_font) != NULL)
       && (x1= grx_font_get_string_width(f,text,length,opt->txo_chrtype))!=0 ) {
        GrxColorTable fgcp  = opt->txo_fgcolor.p;
        GrxColorTable bgcp  = opt->txo_bgcolor.p;
        GrxColor fgcv  = opt->txo_fgcolor.v;
        GrxColor bgcv  = opt->txo_bgcolor.v;
        int     undl  = (fgcv & GRX_UNDERLINE_TEXT) ? 1 : 0;
        int     rotat = GRX_TEXT_DIRECTION_IS_VERTICAL(opt->txo_direct) ? ~0 : 0;
        int     dxpre = 0;
        int     dypre = 0;
        int     dxpost= 0;
        int     dypost= 0;
        int     oldx  = x;
        int     oldy  = y;
        int     y1    = f->h.height;
        int     ww    = (x1 & ~rotat) | (y1 &  rotat);
        int     hh    = (x1 &  rotat) | (y1 & ~rotat);
        int type, step, x2, y2;
        switch(opt->txo_xalign) {
          case GRX_TEXT_ALIGN_RIGHT:
            x -= ww - 1;
            break;
          case GRX_TEXT_ALIGN_CENTER:
            x -= (ww >> 1);
            break;
        }
        switch(opt->txo_yalign) {
          case GRX_TEXT_VALIGN_BASELINE:
            if(opt->txo_direct == GRX_TEXT_DIRECTION_DEFAULT) y -= f->h.baseline;
            break;
          case GRX_TEXT_VALIGN_BOTTOM:
            y -= hh - 1;
            break;
          case GRX_TEXT_VALIGN_MIDDLE:
            y -= (hh >> 1);
            break;
        }
        mouse_block(CURC,x,y,(x + ww - 1),(y + hh - 1));
        switch(opt->txo_direct) {
          case GRX_TEXT_DIRECTION_DOWN:
            dypost = ~0;
            break;
          case GRX_TEXT_DIRECTION_LEFT:
            dxpre = ~0;
            x += ww;
            break;
          case GRX_TEXT_DIRECTION_UP:
            dypre = ~0;
            y += hh;
            break;
          default:
            dxpost = ~0;
            break;
        }
        type = opt->txo_chrtype;
        step = GRX_CHAR_TYPE_GET_SIZE(type);
        while(--length >= 0) {
            int  chr = GRX_CHAR_TYPE_GET_CODE_STR(type, text);
            int  attr,xx,yy,cw,ch;
            char *bmp;
            if(type == GRX_CHAR_TYPE_ATTR) {
                attr = GRX_CHAR_TYPE_GET_ATTR_STR(GRX_CHAR_TYPE_ATTR, text);
                fgcv = GRX_COLOR_TABLE_GET_COLOR(fgcp,GR_ATTR_FGCOLOR(attr));
                bgcv = GRX_COLOR_TABLE_GET_COLOR(bgcp,GR_ATTR_BGCOLOR(attr));
                undl = GR_ATTR_UNDERLINE(attr);
            }
            text = (void *)((char *)(text) + step);
            x1 = grx_font_get_char_width(f,chr);
            y1 = f->h.height;
            cw = (x1 & ~rotat) | (y1 &  rotat);
            ch = (x1 &  rotat) | (y1 & ~rotat);
            x1 = (x -= (cw & dxpre));
            y1 = (y -= (ch & dypre));
            x2 = (xx = x1) + cw - 1;
            y2 = (yy = y1) + ch - 1;
            x += (cw & dxpost);
            y += (ch & dypost);
            clip_ordbox_(CURC,x1,y1,x2,y2,continue,CLIP_EMPTY_MACRO_ARG);
            bmp = grx_font_get_char_aux_bmp(f,chr,opt->txo_direct,undl);
            if(bmp) (*dbm)(
                (x1 + CURC->x_offset),
                (y1 + CURC->y_offset),
                (x2 - x1 + 1),
                (y2 - y1 + 1),
                oldx, oldy,
                bmp,
                ((cw + 7) >> 3),
                ((x1 - xx) + ((y1 - yy) * ((cw + 7) & ~7))),
                fgcv,bgcv,
                p
            );
            else (*FDRV->drawblock)(
                (x1 + CURC->x_offset),
                (y1 + CURC->y_offset),
                (x2 - x1 + 1),
                (y2 - y1 + 1),
                bgcv
            );
        }
        mouse_unblock();
    }
    GRX_LEAVE();
}
