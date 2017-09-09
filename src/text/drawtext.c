/**
 ** drawtext.c ---- draw a character string with the default font
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

static GrFont *_DefaultFont = &GrFont_PC8x14;

void GrTextXY(int x,int y,char *text,GrColor fg,GrColor bg)
{
    GrTextOption opt;
    opt.txo_font      = _DefaultFont;
    opt.txo_fgcolor   = fg;
    opt.txo_bgcolor   = bg;
    opt.txo_chrtype   = GrGetChrtypeForUserEncoding();
    opt.txo_direct    = GR_TEXT_RIGHT;
    opt.txo_xalign    = GR_ALIGN_LEFT;
    opt.txo_yalign    = GR_ALIGN_TOP;
    GrDrawString(text,0,x,y,&opt);
}

GrFont *GrGetDefaultFont()
{
    return _DefaultFont;
}

void GrSetDefaultFont(GrFont *font)
{
    if (font != NULL)
        _DefaultFont = font;
}

