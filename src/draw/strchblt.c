/**
 ** strchblt.c ---- strech blit
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
 **/

#include "libgrx.h"
#include "grdriver.h"
#include "clipping.h"

void GrStretchBlt(GrContext *dst,int dx1,int dy1,int dx2,int dy2,
                  GrContext *src,int x1,int y1,int x2,int y2,GrColor oper)
{
    GrContext *gcaux;

    if(dst == NULL) dst = CURC;
    if(src == NULL) src = CURC;

    isort(x1,x2);
    isort(y1,y2);
    cxclip_ordbox(src,x1,y1,x2,y2);
    isort(dx1,dx2);
    isort(dy1,dy2);

    gcaux = GrCreateContext(dx2-dx1+1, dy2-dy1+1, NULL, NULL);
    if (gcaux == NULL) return;

    mouse_block(src,x1,y1,x2,y2);
    _GrFrDrvGenericStretchBlt(
        &gcaux->gc_frame, 0, 0, dx2-dx1+1, dy2-dy1+1,
        &src->gc_frame, x1+src->gc_xoffset, y1+src->gc_yoffset,
        x2-x1+1, y2-y1+1, GrWRITE);
    mouse_unblock();

    GrBitBlt(dst, dx1, dy1, gcaux, 0, 0, dx2-dx1, dy2-dy1, oper);
    GrDestroyContext(gcaux);
}
