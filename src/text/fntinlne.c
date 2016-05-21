/*
 * fntinlne.c ---- the font inline functions
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

#include <grx/text.h>

#include "libgrx.h"

int (grx_font_is_char_present)(const GrxFont *font,int chr)
{
        return(grx_font_is_char_present(font,chr));
}

int (grx_font_get_char_width)(const GrxFont *font,int chr)
{
        return(grx_font_get_char_width(font,chr));
}

int (grx_font_get_char_height)(const GrxFont *font,int chr)
{
        return(grx_font_get_char_height(font,chr));
}

int (grx_font_get_char_bmp_row_size)(const GrxFont *font,int chr)
{
        return(grx_font_get_char_bmp_row_size(font,chr));
}

int (grx_font_get_char_bmp_size)(const GrxFont *font,int chr)
{
        return(grx_font_get_char_bmp_size(font,chr));
}

int (grx_font_get_string_width)(const GrxFont *font,const void *text,int len,GrxCharType type)
{
        return(grx_font_get_string_width(font,text,len,type));
}

int (grx_font_get_string_height)(const GrxFont *font,const void *text,int len,GrxCharType type)
{
        return(grx_font_get_string_height(font,text,len,type));
}

unsigned char *(grx_font_get_char_bmp)(const GrxFont *font,int chr)
{
        return(grx_font_get_char_bmp(font,chr));
}

unsigned char *(grx_font_get_char_aux_bmp)(GrxFont *font,int chr,GrxTextDirection dir,int ul)
{
        return(grx_font_get_char_aux_bmp(font,chr,dir,ul));
}

int (grx_text_option_get_char_width)(const GrxTextOption *opt,int chr)
{
        return(grx_text_option_get_char_width(opt,chr));
}

int (grx_text_option_get_char_height)(const GrxTextOption *opt,int chr)
{
        return(grx_text_option_get_char_height(opt,chr));
}

void (grx_text_option_get_char_size)(const GrxTextOption *opt,int chr,int *w,int *h)
{
        grx_text_option_get_char_size(opt,chr,w,h);
}

int (grx_text_option_get_string_width)(const GrxTextOption *opt,void *text,int length)
{
        return(grx_text_option_get_string_width(opt,text,length));
}

int (grx_text_option_get_string_height)(const GrxTextOption *opt,void *text,int length)
{
        return(grx_text_option_get_string_height(opt,text,length));
}

void (grx_text_option_get_string_size)(const GrxTextOption *opt,void *text,int length,int *w,int *h)
{
        grx_text_option_get_string_size(opt,text,length,w,h);
}
