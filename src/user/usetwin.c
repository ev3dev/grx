/*
 * usetwin.c
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

void grx_user_set_window(int x1,int y1,int x2,int y2)
{
        CURC->user_x_base  = x1;
        CURC->user_y_base  = y1;
        CURC->user_width  = x2 - x1;
        CURC->user_height = y2 - y1;
}
