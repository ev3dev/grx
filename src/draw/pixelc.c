/**
 ** PIXELC.C ---- pixel read from context
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "clipping.h"

GrColor GrPixelC(GrContext *c,int x,int y)
{
        GrColor retval;
        cxclip_dot_(c,x,y,return(GrNOCOLOR));
        mouse_block(c,x,y,x,y);
        retval = (*c->gc_driver->readpixel)(
            &c->gc_frame,
            x + c->gc_xoffset,
            y + c->gc_yoffset
        );
        mouse_unblock();
        return(retval);
}

