/**
 ** DRVINLNE.C ---- the drawing inline functions
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"

void (GrPlotNC)(int x,int y,long c)
{
        GrPlotNC(x,y,c);
}

long (GrPixelNC)(int x,int y)
{
        return(GrPixelNC(x,y));
}

long (GrPixelCNC)(GrContext *c,int x,int y)
{
        return(GrPixelCNC(c,x,y));
}


