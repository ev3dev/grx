/**
 ** clrinlne.c ---- the color inline functions
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu]
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

GrxColor (GrNumColors)(void)
{
        return(GrNumColors());
}

GrxColor (GrNumFreeColors)(void)
{
        return(GrNumFreeColors());
}

GrxColor (GrBuildRGBcolorT)(int r,int g,int b)
{
        return(GrBuildRGBcolorT(r,g,b));
}

GrxColor (GrBuildRGBcolorR)(int r,int g,int b)
{
        return(GrBuildRGBcolorR(r,g,b));
}

int (GrRGBcolorRed)(GrxColor c)
{
        return(GrRGBcolorRed(c));
}

int (GrRGBcolorGreen)(GrxColor c)
{
        return(GrRGBcolorGreen(c));
}

int (GrRGBcolorBlue)(GrxColor c)
{
        return(GrRGBcolorBlue(c));
}

GrxColor (GrAllocColorID)(int r,int g,int b)
{
        return(GrAllocColorID(r,g,b));
}

GrxColor (GrAllocColor2)(long hcolor)
{
        return(GrAllocColor2(hcolor));
}

GrxColor (GrAllocColor2ID)(long hcolor)
{
        return(GrAllocColor2ID(hcolor));
}

void (GrQueryColorID)(GrxColor c,int *r,int *g,int *b)
{
        GrQueryColorID(c,r,g,b);
}

void (GrQueryColor2ID)(GrxColor c,long *hcolor)
{
        GrQueryColor2ID(c,hcolor);
}

