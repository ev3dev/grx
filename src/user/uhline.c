/*
 * uhline.c
 *
 * Copyright (C) 1992, Csaba Biegl
 *   820 Stirrup Dr, Nashville, TN, 37221
 *   csaba@vuse.vanderbilt.edu
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
#include "usercord.h"

void grx_user_draw_hline(int x1,int x2,int y,GrxColor c)
{
        U2SX(x1,CURC);
        U2SX(x2,CURC);
        U2SY(y,CURC);
        grx_draw_hline(x1,x2,y,c);
}
