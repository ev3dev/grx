/**
 ** HOOKS.C ---- functions to set up some hooks and control flags
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"

void GrSetModeRestore(int restoreFlag)
{
        DRVINFO->moderestore = restoreFlag;
}

void GrSetErrorHandling(int exitIfError)
{
        DRVINFO->errsfatal = exitIfError;
}

void GrSetModeHook(void (*hookfunc)(void))
{
        DRVINFO->mdsethook = hookfunc;
}

