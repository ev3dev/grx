/**
 ** patfline.c
 **
 **  Copyright (C) 1992, 1995, Csaba Biegl
 **    820 Stirrup Dr, Nashville, TN, 37221
 **    csaba@vuse.vanderbilt.edu
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
 ** 200620 M.Alvarez, solved a bug
 ** 200623 M.Alvarez, adding GrPatAlignFilledLine
 **
 **/

#include "libgrx.h"
#include "clipping.h"
#include "shapes.h"

void _GrPatternFilledLineExt(int x1,int y1,int dx,int dy,int sx,int sy,GrPattern *p)
{
    GrColor fgc = 0, bgc = 0, pxc = 0;
    int sigy,ymajor;
    int pw, ph, px, py;
    int points,error;
    
    if (dx < 0) {
        x1 += dx; dx = -dx;
        y1 += dy; dy = -dy;
    }
    if(dy==0) {
        /*int check_if_offsets_correct;*/
        _GrFillPatternExt(x1,y1,sx,sy,dx+1,p);
        return;
    }

    if(dy >= 0) {
        sigy = 1;
    }
    else {
        dy = (-dy);
        sigy = (-1);
    }
    if(dy > dx) {
        points = dy + 1;
        error  = dy >> 1;
        ymajor = TRUE;
    }
    else {
        points = dx + 1;
        error  = dx >> 1;
        ymajor = FALSE;
    }

    if(p->gp_ptype == GR_PTYPE_PIXMAP) {
        GrFrame *src = &p->gp_pxp_source;
        pw = p->gp_pxp_width;
        ph = p->gp_pxp_height;
        px = (x1-sx) % pw;
        py = (y1-sy) % ph;
        if (px < 0) px += pw;
        if (py < 0) py += ph;
        
        while(--points >= 0) {
            pxc = (*src->gf_driver->readpixel)(src,px,py);
            
            (*CURC->gc_driver->drawpixel)(x1,y1,pxc);
            
            if(ymajor) {
                if((error -= dx) < 0) error += dy,x1++,px++;
                y1 += sigy,py += sigy;
            }
            else {
                if((error -= dy) < 0) error += dx,y1 += sigy,py += sigy;
                x1++,px++;
            }
            if((unsigned)py >= (unsigned)ph) {
                if(py < 0) py += ph;
                else       py -= ph;
            }
            if(px >= pw) px = 0;
        }
    }
    else if(p->gp_ptype == GR_PTYPE_BITMAP) {
        unsigned char *src = (unsigned char *) p->gp_bmp_data;
        pw = 8;
        ph = p->gp_bmp_height;
        px = x1 & 7;
        py = y1 % ph;
        fgc = p->gp_bmp_fgcolor;
        bgc = p->gp_bmp_bgcolor;
        
        while(--points >= 0) {
            pxc = (src[py] & (0x80U >> px)) ? fgc : bgc;
            
            (*CURC->gc_driver->drawpixel)(x1,y1,pxc);
            
            if(ymajor) {
                if((error -= dx) < 0) error += dy,x1++,px++;
                y1 += sigy,py += sigy;
            }
            else {
                if((error -= dy) < 0) error += dx,y1 += sigy,py += sigy;
                x1++,px++;
            }
            if((unsigned)py >= (unsigned)ph) {
                if(py < 0) py += ph;
                else       py -= ph;
            }
            if(px >= pw) px = 0;
        }
    }
    else if(p->gp_ptype == GR_PTYPE_GRADIENT) {
        while(--points >= 0) {
            if (p->gp_grd_mode == GR_RADIAL_GRADIENT) {
                pxc = _GrRadGradientColor((GrGradient *)p, x1, y1, sx, sy);
            } else {
                pxc = _GrLinGradientColor((GrGradient *)p, x1, y1, sx, sy);
            }
            
            (*CURC->gc_driver->drawpixel)(x1,y1,pxc);
            
            if(ymajor) {
                if((error -= dx) < 0) error += dy, x1++;
                y1 += sigy;
            }
            else {
                if((error -= dy) < 0) error += dx, y1 += sigy;
                x1++;
            }
        }
    }
}

void GrPatternFilledLine(int x1,int y1,int x2,int y2,GrPattern *p)
{
    clip_line(CURC,x1,y1,x2,y2);
    mouse_block(CURC,x1,y1,x2,y2);
    _GrPatternFilledLineExt(x1+CURC->gc_xoffset,y1+CURC->gc_yoffset,
                            x2-x1,y2-y1,0,0,p);
    mouse_unblock();
}

void GrPatAlignFilledLine(int xo,int yo,int x1,int y1,int x2,int y2,GrPattern *p)
{
    //xo = min(xo, min(x1,x2));
    //yo = min(yo, min(y1,y2));
    clip_line(CURC,x1,y1,x2,y2);
    mouse_block(CURC,x1,y1,x2,y2);
    _GrPatternFilledLineExt(x1+CURC->gc_xoffset,y1+CURC->gc_yoffset,x2-x1,y2-y1,
                            xo+CURC->gc_xoffset,yo+CURC->gc_yoffset,p);
    mouse_unblock();
}
