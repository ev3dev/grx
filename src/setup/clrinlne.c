/**
 ** CLRINLNE.C ---- the color inline functions
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"

long (GrColorValue)(long c)
{
        return(GrColorValue(c));
}

long (GrColorMode)(long c)
{
        return(GrColorMode(c));
}

long (GrWriteModeColor)(long c)
{
        return(GrWriteModeColor(c));
}

long (GrXorModeColor)(long c)
{
        return(GrXorModeColor(c));
}

long (GrOrModeColor)(long c)
{
        return(GrOrModeColor(c));
}
long (GrAndModeColor)(long c)
{
        return(GrAndModeColor(c));
}

long (GrImageModeColor)(long c)
{
        return(GrImageModeColor(c));
}

long (GrNumColors)(void)
{
        return(GrNumColors());
}

long (GrNumFreeColors)(void)
{
        return(GrNumFreeColors());
}

long (GrBuildRGBcolorT)(int r,int g,int b)
{
        return(GrBuildRGBcolorT(r,g,b));
}

long (GrBuildRGBcolorR)(int r,int g,int b)
{
        return(GrBuildRGBcolorR(r,g,b));
}

int (GrRGBcolorRed)(long c)
{
        return(GrRGBcolorRed(c));
}

int (GrRGBcolorGreen)(long c)
{
        return(GrRGBcolorGreen(c));
}

int (GrRGBcolorBlue)(long c)
{
        return(GrRGBcolorBlue(c));
}

long (GrAllocColorID)(int r,int g,int b)
{
        return(GrAllocColorID(r,g,b));
}

void (GrQueryColorID)(long c,int *r,int *g,int *b)
{
        GrQueryColorID(c,r,g,b);
}

