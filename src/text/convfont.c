/**
 ** CONVFONT.C ---- build a converted font from an already loaded (linked) one
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/
#include "grfontdv.h"
#include "libgrx.h"
#include "arith.h"
#include "memcopy.h"

static GrFont *cvfont;

static int charwdt(int chr)
{
        chr -= cvfont->h.minchar;
        if((uint)chr >= cvfont->h.numchars) return(-1);
        return(cvfont->chrinfo[chr].width);
}

static int bitmap(int chr,int w,int h,char *buffer)
{
        chr -= cvfont->h.minchar;
        if((uint)chr >= cvfont->h.numchars)       return(FALSE);
        if((uint)w != cvfont->chrinfo[chr].width) return(FALSE);
        if((uint)h != cvfont->h.height)                  return(FALSE);
        memcopy_b(
            buffer,
            &cvfont->bitmap[cvfont->chrinfo[chr].offset],
            ((w + 7) >> 3) * h
        );
        return(TRUE);
}

GrFont *GrBuildConvertedFont(GrFont *from,int cvt,int w,int h,int minch,int maxch)
{
        if(!(cvfont = from)) return(NULL);
        return(_GrBuildFont(&from->h,cvt,w,h,minch,maxch,charwdt,bitmap,FALSE));
}

