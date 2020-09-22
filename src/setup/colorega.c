/*
 * colorega.c ---- Alloc the standard EGA palette
 *
 * Copyright (c) 1998  Hartmut Schirmer
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
 */

#include <grx/color.h>

#include "libgrx.h"

static struct {
    GR_int8u r, g, b;
} EGArgb[16] = {
    { 0, 0, 0 },       /* black */
    { 0, 0, 170 },     /* blue */
    { 0, 170, 0 },     /* green */
    { 0, 170, 170 },   /* cyan */
    { 170, 0, 0 },     /* red */
    { 170, 0, 170 },   /* magenta */
    { 170, 85, 0 },    /* brown */
    { 170, 170, 170 }, /* light gray */
    { 85, 85, 85 },    /* dark gray */
    { 85, 85, 255 },   /* light blue */
    { 85, 255, 85 },   /* light green */
    { 85, 255, 255 },  /* light cyan */
    { 255, 85, 85 },   /* light red */
    { 255, 85, 255 },  /* light magenta */
    { 255, 255, 85 },  /* yellow */
    { 255, 255, 255 }  /* white */
};

/**
 * grx_color_get_ega_colors:
 *
 * Gets an array of the 16 classic EGA colors. Use #GrxEgaColorIndex to
 * determine the color at each index of the array.
 *
 * The the colors and the array itself must not be freed.
 *
 * Returns: (array fixed-size=16): array of color indexes
 */
const GrxColor *grx_color_get_ega_colors(void)
{
    static GrxColor egapal[16];
    int i;
    for (i = 0; i < 16; ++i)
        egapal[i] = grx_color_get(EGArgb[i].r, EGArgb[i].g, EGArgb[i].b);
    return egapal;
}
