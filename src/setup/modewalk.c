/**
 ** MODEWALK.C ---- utilities to iterate over the available video modes
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "grdriver.h"
#include "libgrx.h"

static GrVideoMode *modewalk(GrVideoMode *pm,GrVideoMode *dup,GrFrameMode md)
{
        GrVideoDriver *vd;
        GrVideoMode   *vm;
        int n,seen = TRUE;
        if(pm && pm->extinfo) {
            md   = pm->extinfo->mode;
            seen = FALSE;
        }
        for(vd = VDRV; vd != NULL; vd = vd->inherit) {
            for(n = vd->nmodes,vm = vd->modes; --n >= 0; vm++) {
                    if(vm->present == FALSE)    continue;
                    if(vm->extinfo == NULL)     continue;
                    if(vm->extinfo->mode != md) continue;
                    if(dup) {
                        if(vm == dup) return(NULL);
                        if(vm->width  != dup->width)  continue;
                        if(vm->height != dup->height) continue;
                        if(vm->bpp    != dup->bpp)    continue;
                        return(vm);
                    }
                    if(seen) {
                        if(!modewalk(NULL,vm,md)) return(vm);
                        continue;
                    }
                    if(pm == vm) {
                        seen = TRUE;
                    }
            }
        }
        return(NULL);
}
                    
const GrVideoMode *GrFirstVideoMode(GrFrameMode fmode)
{
        return(modewalk(NULL,NULL,fmode));
}
        
const GrVideoMode *GrNextVideoMode(const GrVideoMode *prev)
{
        return(modewalk((GrVideoMode *)prev,NULL,GR_frameUndef));
}

