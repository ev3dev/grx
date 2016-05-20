/*
 * clrinlne.c ---- the color inline functions
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

GrxColor (grx_color_get_value)(GrxColor c)
{
        return(grx_color_get_value(c));
}

GrxColor (grx_color_get_mode)(GrxColor c)
{
        return(grx_color_get_mode(c));
}

GrxColor (grx_color_to_write_mode)(GrxColor c)
{
        return(grx_color_to_write_mode(c));
}

GrxColor (grx_color_to_xor_mode)(GrxColor c)
{
        return(grx_color_to_xor_mode(c));
}

GrxColor (grx_color_to_or_mode)(GrxColor c)
{
        return(grx_color_to_or_mode(c));
}
GrxColor (grx_color_to_and_mode)(GrxColor c)
{
        return(grx_color_to_and_mode(c));
}

GrxColor (grx_color_to_image_mode)(GrxColor c)
{
        return(grx_color_to_image_mode(c));
}

int (grx_color_info_n_colors)(void)
{
        return(grx_color_info_n_colors());
}

int (grx_color_info_n_free_colors)(void)
{
        return(grx_color_info_n_free_colors());
}

GrxColor (grx_color_info_build_rgb_color_t)(int r,int g,int b)
{
        return(grx_color_info_build_rgb_color_t(r,g,b));
}

GrxColor (grx_color_info_build_rgb_color_r)(int r,int g,int b)
{
        return(grx_color_info_build_rgb_color_r(r,g,b));
}

int (grx_color_info_get_red_value)(GrxColor c)
{
        return(grx_color_info_get_red_value(c));
}

int (grx_color_info_get_green_value)(GrxColor c)
{
        return(grx_color_info_get_green_value(c));
}

int (grx_color_info_get_blue_value)(GrxColor c)
{
        return(grx_color_info_get_blue_value(c));
}

GrxColor (grx_color_info_alloc_color_id)(int r,int g,int b)
{
        return(grx_color_info_alloc_color_id(r,g,b));
}

GrxColor (grx_color_info_alloc_color2)(long hcolor)
{
        return(grx_color_info_alloc_color2(hcolor));
}

GrxColor (grx_color_info_alloc_color2_id)(long hcolor)
{
        return(grx_color_info_alloc_color2_id(hcolor));
}

void (grx_color_info_query_color_id)(GrxColor c,int *r,int *g,int *b)
{
        grx_color_info_query_color_id(c,r,g,b);
}

void (grx_color_info_query_color2_id)(GrxColor c,long *hcolor)
{
        grx_color_info_query_color2_id(c,hcolor);
}
