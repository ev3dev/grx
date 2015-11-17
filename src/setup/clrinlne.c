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

GrxColor (GrColorValue)(GrxColor c)
{
        return(GrColorValue(c));
}

GrxColor (GrColorMode)(GrxColor c)
{
        return(GrColorMode(c));
}

GrxColor (GrWriteModeColor)(GrxColor c)
{
        return(GrWriteModeColor(c));
}

GrxColor (GrXorModeColor)(GrxColor c)
{
        return(GrXorModeColor(c));
}

GrxColor (GrOrModeColor)(GrxColor c)
{
        return(GrOrModeColor(c));
}
GrxColor (GrAndModeColor)(GrxColor c)
{
        return(GrAndModeColor(c));
}

GrxColor (GrImageModeColor)(GrxColor c)
{
        return(GrImageModeColor(c));
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

