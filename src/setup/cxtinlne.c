/**
 ** ctcinlne.c ---- the context inline functions
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

GrxContext *(GrCreateContext)(int w,int h,char *memory[4],GrxContext *where)
{
        return(GrCreateContext(w,h,memory,where));
}

GrxContext *(GrCurrentContext)(void)
{
        return(GrCurrentContext());
}

GrxContext *(GrScreenContext)(void)
{
        return(GrScreenContext());
}

void (GrGetClipBox)(int *x1p,int *y1p,int *x2p,int *y2p)
{
        GrGetClipBox(x1p,y1p,x2p,y2p);
}

void (GrGetClipBoxC)(const GrxContext *c,int *x1p,int *y1p,int *x2p,int *y2p)
{
        GrGetClipBoxC(c,x1p,y1p,x2p,y2p);
}

int (GrMaxX)(void)
{
        return(GrMaxX());
}

int (GrMaxY)(void)
{
        return(GrMaxY());
}

int (GrSizeX)(void)
{
        return(GrSizeX());
}

int (GrSizeY)(void)
{
        return(GrSizeY());
}

int (GrLowX)(void)
{
        return(GrLowX());
}

int (GrLowY)(void)
{
        return(GrLowY());
}

int (GrHighX)(void)
{
        return(GrHighX());
}

int (GrHighY)(void)
{
        return(GrHighY());
}

