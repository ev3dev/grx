/*
 * usercord.c
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

#include "globals.h"
#include "libgrx.h"
#include "usercord.h"

void grx_user_convert_user_to_screen(int *x,int *y)
{
        U2SX(*x,CURC);
        U2SY(*y,CURC);
}

void grx_user_convert_screen_to_user(int *x,int *y)
{
        S2UX(*x,CURC);
        S2UY(*y,CURC);
}
