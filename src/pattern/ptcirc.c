/*
 * ptcirc.c
 *
 * Copyright (C) 1997, Michael Goffioul
 * [e-mail : goffioul@emic.ucl.ac.be]
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
 *
 *  Copyright (C) 1992, Csaba Biegl
 *    820 Stirrup Dr, Nashville, TN, 37221
 *    csaba@vuse.vanderbilt.edu
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

#include "libgrx.h"

void grx_draw_circle_with_pattern(int xc,int yc,int r,GrxLinePattern *lp)
{
        grx_draw_ellipse_with_pattern(xc,yc,r,r,lp);
}
