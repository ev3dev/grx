/**
 ** U_VSVIRT.C ---- virtual screen utility functions using VESA BIOS calls
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "vesa.h"
#include "arith.h"
#include "mempeek.h"
#include "memfill.h"
#include "int86.h"

int _GrViDrvVESAsetVirtualSize(GrVideoMode *md,int w,int h,GrVideoMode *result)
{
        Int86Regs r;
        sttzero(&r);
        IREG_AX(r) = VESA_FUNC + VESA_SCAN_LNLEN;
        IREG_BX(r) = 0;
        IREG_CX(r) = w;
        int10(&r);
        if(IREG_AX(r) == VESA_SUCCESS) {
            result->lineoffset = IREG_BX(r);
            result->width      = IREG_CX(r);
            result->height     = umin(IREG_DX(r),h);
            return(TRUE);
        }
        return(FALSE);
}

/*
** VESA 2.0 has a PM function for VESA_DISP_START
** Bad news: register values are incompatible with
** real mode function. PM adaption should be done
*/
int _GrViDrvVESAvirtualScroll(GrVideoMode *md,int x,int y,int result[2])
{
        Int86Regs r;
        sttzero(&r);
        IREG_AX(r) = VESA_FUNC + VESA_DISP_START;
        IREG_BX(r) = 0;
        IREG_CX(r) = x;
        IREG_DX(r) = y;
        int10(&r);
        if(IREG_AX(r) == VESA_SUCCESS) {
            IREG_AX(r) = VESA_FUNC + VESA_DISP_START;
            IREG_BX(r) = 1;
            int10(&r);
            result[0] = IREG_CX(r);
            result[1] = IREG_DX(r);
            return(TRUE);
        }
        return(FALSE);
}

