/**
 **  FILLPATT.C ------- draw a pattern filled horizontal line
 **
 **    Copyright (C) 1997, Michael Goffioul
 **    [e-mail : goffioul@emic.ucl.ac.be]
 **
 **    see copying.cb for details
 **/

#include "libgrx.h"

static
#ifdef __GNUC__
inline
#endif
void drawbitpattern(int x,int y,int w,char patt,int offs,long fg,long bg)
{
    if (patt == 0)
        (*CURC->gc_driver->drawhline)(x,y,w,bg);
    else if ((unsigned char)patt == 0xff)
        (*CURC->gc_driver->drawhline)(x,y,w,fg);
    else {
        uchar mask = 0x80;
        mask >>= offs;
        w += x;
        do {
            (*CURC->gc_driver->drawpixel)(x,y,(patt & mask) ? fg : bg);
            if((mask >>= 1) == 0) mask = 0x80;
        } while(++x != w);
    }
}

void _GrFillPattern(int x, int y, int width, GrPattern *p)
{
        if (p->gp_ispixmap) {

                void (*bltfun)(GrFrame*,int,int,GrFrame*,int,int,int,int,long);
                int pattwdt = p->gp_pxp_width;
                int ypatt = y % p->gp_pxp_height;
                int xpatt = x % pattwdt;
                int xdest = x;
                int ydest = y;
                int cpysize = pattwdt - xpatt;
                long optype = p->gp_pxp_oper;

                if (CURC->gc_onscreen)
                        bltfun = CURC->gc_driver->bltr2v;
                else
                        bltfun = CURC->gc_driver->bitblt;
                while (width > 0) {
                        if (cpysize > width) cpysize = width;
                        (*bltfun)(
                                &CURC->gc_frame,xdest,ydest,
                                &p->gp_pxp_source,xpatt,ypatt,cpysize,1,
                                optype
                        );
                        width -= cpysize;
                        xpatt = 0;
                        xdest += cpysize;
                        cpysize = pattwdt;
                }
        }
        else {

                char bits = p->gp_bmp_data[y % p->gp_bmp_height];
                int xoffs = x % 8;
                long fgc = p->gp_bmp_fgcolor;
                long bgc = p->gp_bmp_bgcolor;

                drawbitpattern(x,y,width,bits,xoffs,fgc,bgc);
        }
}

