/**
 ** PROPWDT.C ---- calculate the width of a string using a proportional font
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"

int GrProportionalTextWidth(GrFont *font,void *text,int len,int type)
{
        char *txp = (char *)text;
        int   wdt = 0;
        while(--len >= 0) {
            wdt += GrFontCharWidth(font,GR_TEXTSTR_CODE(txp,type));
            txp += GR_TEXTCHR_SIZE(type);
        }
        return(wdt);
}




