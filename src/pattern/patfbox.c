/**
 ** PATFBOX.C
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "arith.h"
#include "clipping.h"

void GrPatternFilledBox(int x1,int y1,int x2,int y2,GrPattern *p)
{
        int width,height;
        void (*bltfun)(GrFrame*,int,int,GrFrame*,int,int,int,int,long);

        clip_box(CURC,x1,y1,x2,y2);
        mouse_block(CURC,x1,y1,x2,y2);
        width  = x2 - x1 + 1;
        height = y2 - y1 + 1;
        if (CURC->gc_onscreen)
                bltfun = CURC->gc_driver->bltr2v;
        else
                bltfun = CURC->gc_driver->bitblt;
        if(!p->gp_ispixmap)
            while(--height >= 0) _GrFillPattern(x1,y1++,width,p);
        else {
            int pwdt = p->gp_pxp_width;
            int phgt = p->gp_pxp_height;
            int xoff = (x1 + CURC->gc_xoffset) % pwdt;
            int yoff = (y1 + CURC->gc_yoffset) % phgt;
            while(height > 0) {
                int fillh   = min(height,(phgt - yoff));
                int linewdt = width;
                int xpos    = x1;
                int xcuroff = xoff;
                while(linewdt > 0) {
                    int fillw = min(linewdt,(pwdt - xcuroff));
                    (*bltfun)(
                        &CURC->gc_frame,xpos,y1,
                        &p->gp_pxp_source,xcuroff,yoff,fillw,fillh,
                        p->gp_pxp_oper
                    );
                    linewdt -= fillw;
                    xpos += fillw;
                    xcuroff = 0;
                }
                height -= fillh;
                y1 += fillh;
                yoff = 0;
            }
        }
        mouse_unblock();
}

