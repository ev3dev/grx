/**
 ** FFRAME.C ---- frame driver lookup functions
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "grdriver.h"
#include "libgrx.h"

GrFrameDriver *_GrFindFrameDriver(GrFrameMode mode)
{
        int ii = 0;
        while(_GrFrameDriverTable[ii] != NULL) {
            if(_GrFrameDriverTable[ii]->mode == mode) break;
            ii++;
        }
        return(_GrFrameDriverTable[ii]);
}

GrFrameDriver *_GrFindRAMframeDriver(GrFrameMode mode)
{
        GrFrameDriver *dp = _GrFindFrameDriver(mode);
        return((dp && dp->is_video) ? _GrFindFrameDriver(dp->rmode) : dp);
}

