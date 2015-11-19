/**
 ** colorbw.c ---- standard colors: black and white
 **
 ** Copyright (c) 1998 Hartmut Schirmer
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

#ifdef grx_color_info_get_black
#undef grx_color_info_get_black
#endif
GrxColor grx_color_info_get_black(void)
{
        GRX_ENTER();
        if(CLRINFO->black == GRX_COLOR_NONE) CLRINFO->black = grx_color_info_alloc_color(0,0,0);
               GRX_RETURN(CLRINFO->black);
}

#ifdef grx_color_info_get_white
#undef grx_color_info_get_white
#endif
GrxColor grx_color_info_get_white(void)
{
        GRX_ENTER();
        if(CLRINFO->white == GRX_COLOR_NONE) CLRINFO->white = grx_color_info_alloc_color(255,255,255);
        GRX_RETURN(CLRINFO->white);
}

