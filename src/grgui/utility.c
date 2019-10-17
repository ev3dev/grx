/**
 ** utility.c ---- Mini GUI for MGRX, some utility functions
 **
 ** Copyright (C) 2002,2006,2019 Mariano Alvarez Fernandez
 ** [e-mail: malfer at telefonica dot net]
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

#include <stdlib.h>
#include "grguip.h"

int GrCheckCoordInto(int x, int y, int xo, int yo, int width, int height)
{
    if (x < xo) return 0;
    if (x >= xo+width) return 0;
    if (y < yo) return 0;
    if (y >= yo+height) return 0;
    return 1;
}

void GrDrawBorder(int x1, int y1, int x2, int y2, int wborder,
                  GrColor lcolor, GrColor bcolor)
{
    //we expect the context set
    int i;

    if (wborder <= 0) return;
    
    GrBox(x1, y1, x2, y2, lcolor);
    if (wborder > 2) {
        for (i=0; i<(wborder-2); i++)
            GrBox(x1+i+1, y1+i+1, x2-i-1, y2-i-1, bcolor);
    }
    if (wborder > 1) {
        i = wborder - 1;
        GrBox(x1+i, y1+i, x2-i, y2-i, lcolor);
    }
}
