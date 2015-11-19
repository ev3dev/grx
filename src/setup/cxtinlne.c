/*
 * ctcinlne.c ---- the context inline functions
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
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

GrxContext *(grx_context_create)(int w,int h,unsigned char *memory[4],GrxContext *where)
{
        return(grx_context_create(w,h,memory,where));
}

GrxContext *(grx_context_get_current)(void)
{
        return(grx_context_get_current());
}

GrxContext *(grx_context_get_screen)(void)
{
        return(grx_context_get_screen());
}

void (grx_get_clip_box)(int *x1p,int *y1p,int *x2p,int *y2p)
{
        grx_get_clip_box(x1p,y1p,x2p,y2p);
}

void (grx_context_get_clip_box)(const GrxContext *c,int *x1p,int *y1p,int *x2p,int *y2p)
{
        grx_context_get_clip_box(c,x1p,y1p,x2p,y2p);
}

int (grx_get_max_x)(void)
{
        return(grx_get_max_x());
}

int (grx_get_max_y)(void)
{
        return(grx_get_max_y());
}

int (grx_get_size_x)(void)
{
        return(grx_get_size_x());
}

int (grx_get_size_y)(void)
{
        return(grx_get_size_y());
}

int (grx_get_low_x)(void)
{
        return(grx_get_low_x());
}

int (grx_get_low_y)(void)
{
        return(grx_get_low_y());
}

int (grx_get_high_x)(void)
{
        return(grx_get_high_x());
}

int (grx_get_high_y)(void)
{
        return(grx_get_high_y());
}
