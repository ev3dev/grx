/**
 ** VIEWPORT.C ---- set display start address for virtual screen
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "arith.h"

int GrSetViewport(int x,int y)
{
        int res[2];
        if(!GrScreenIsVirtual())              return(FALSE);
        if(!DRVINFO->actmode.extinfo->scroll) return(FALSE);
        x = imax(0,imin((GrVirtualX() - GrScreenX()),x));
        y = imax(0,imin((GrVirtualY() - GrScreenY()),y));
        if((x == GrViewportX()) && (y == GrViewportY())) return(TRUE);
        if((*DRVINFO->actmode.extinfo->scroll)(&DRVINFO->actmode,x,y,res)) {
            DRVINFO->vposx = res[0];
            DRVINFO->vposy = res[1];
            return(TRUE);
        }
        return(FALSE);
}




