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

int (GrFontCharPresent)(const GrFont *font,unsigned int chr)
{
	return(GrFontCharPresent(font,chr));
}

int (GrFontCharWidth)(const GrFont *font,unsigned int chr)
{
	return(GrFontCharWidth(font,chr));
}

int (GrFontCharHeight)(const GrFont *font,unsigned int chr)
{
	return(GrFontCharHeight(font,chr));
}

int (GrFontCharBmpRowSize)(const GrFont *font,unsigned int chr)
{
	return(GrFontCharBmpRowSize(font,chr));
}

int (GrFontCharBitmapSize)(const GrFont *font,unsigned int chr)
{
	return(GrFontCharBitmapSize(font,chr));
}

char *(GrFontCharBitmap)(const GrFont *font,unsigned int chr)
{
	return(GrFontCharBitmap(font,chr));
}

char *(GrFontCharAuxBmp)(GrFont *font,unsigned int chr,int dir,int ul)
{
	return(GrFontCharAuxBmp(font,chr,dir,ul));
}

