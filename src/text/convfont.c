/**
 ** CONVFONT.C ---- build a converted font from an already loaded (linked) one
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "grfontdv.h"
#include "arith.h"

static GrFont *cvfont;

static int charwdt(int chr)
{
        chr -= cvfont->h.minchar;
        if((unsigned int)chr >= cvfont->h.numchars) return(-1);
        return(cvfont->chrinfo[chr].width);
}

static int bitmap(int chr,int w,int h,char *buffer)
{
        chr -= cvfont->h.minchar;
        if((unsigned int)chr >= cvfont->h.numchars)       return(FALSE);
        if((unsigned int)w != cvfont->chrinfo[chr].width) return(FALSE);
        if((unsigned int)h != cvfont->h.height)           return(FALSE);
        memcpy(
            buffer,
            &cvfont->bitmap[cvfont->chrinfo[chr].offset],
            ((w + 7) >> 3) * h
        );
        return(TRUE);
}

GrFont *GrBuildConvertedFont(GrFont *from,int cvt,int w,int h,int minch,int maxch)
{
        cvfont = from;
        if(!cvfont) return(NULL);
        return(_GrBuildFont(&from->h,cvt,w,h,minch,maxch,charwdt,bitmap,FALSE));
}

