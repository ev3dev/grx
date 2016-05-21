/*
 * upfplot.c
 *
 * Copyright (C) 1997, Michael Goffioul
 * [goffioul@emic.ucl.ac.be]
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

#include <grx/pattern.h>

#include "globals.h"
#include "libgrx.h"
#include "usercord.h"

void grx_user_draw_filled_pixel_with_pattern(int x,int y,GrxPattern *p)
{
        U2SX(x,CURC);
        U2SY(y,CURC);
        grx_draw_filled_pixel_with_pattern(x,y,p);
}
