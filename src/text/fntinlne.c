/**
 ** fntinlne.c ---- the font inline functions
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

int (GrFontCharPresent)(GrFont *font,int chr)
{
        return(GrFontCharPresent(font,chr));
}

int (GrFontCharWidth)(GrFont *font,int chr)
{
        return(GrFontCharWidth(font,chr));
}

int (GrFontCharHeight)(GrFont *font,int chr)
{
        return(GrFontCharHeight(font,chr));
}

int (GrFontCharBmpRowSize)(GrFont *font,int chr)
{
        return(GrFontCharBmpRowSize(font,chr));
}

int (GrFontCharBitmapSize)(GrFont *font,int chr)
{
        return(GrFontCharBitmapSize(font,chr));
}

int (GrFontStringWidth)(GrFont *font,void *text,int len,int type)
{
        return(GrFontStringWidth(font,text,len,type));
}

int (GrFontStringHeight)(GrFont *font,void *text,int len,int type)
{
        return(GrFontStringHeight(font,text,len,type));
}

char far *(GrFontCharBitmap)(GrFont *font,int chr)
{
        return(GrFontCharBitmap(font,chr));
}

char far *(GrFontCharAuxBmp)(GrFont *font,int chr,int dir,int ul)
{
        return(GrFontCharAuxBmp(font,chr,dir,ul));
}

int (GrCharWidth)(int chr,GrTextOption *opt)
{
        return(GrCharWidth(chr,opt));
}

int (GrCharHeight)(int chr,GrTextOption *opt)
{
        return(GrCharHeight(chr,opt));
}

void (GrCharSize)(int chr,GrTextOption *opt,int *w,int *h)
{
        GrCharSize(chr,opt,w,h);
}

int (GrStringWidth)(void *text,int length,GrTextOption *opt)
{
        return(GrStringWidth(text,length,opt));
}

int (GrStringHeight)(void *text,int length,GrTextOption *opt)
{
        return(GrStringHeight(text,length,opt));
}

void (GrStringSize)(void *text,int length,GrTextOption *opt,int *w,int *h)
{
        GrStringSize(text,length,opt,w,h);
}

