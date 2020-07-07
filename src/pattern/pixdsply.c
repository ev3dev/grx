/**
 ** pixdsply.c ---- Fuctions to display pixmaps
 **
 ** Copyright (c) 2020 M.Alvarez [malfer at telefonica.net]
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

void GrPixmapDisplay(int x,int y, GrPixmap *p)
{
    GRX_ENTER();
    if (p->pxp_ptype == GR_PTYPE_PIXMAP)
        GrPatAlignFilledBox(x,y,x,y,x+p->pxp_width-1,y+p->pxp_height-1,(GrPattern *)p);
    GRX_LEAVE();
}

void GrPixmapDisplayExt(int x1,int y1,int x2,int y2, GrPixmap *p)
{
    GRX_ENTER();
    GrPatAlignFilledBox(x1,y1,x1,y1,x2,y2,(GrPattern *)p);
    GRX_LEAVE();
}
