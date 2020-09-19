/*
 * fframe.c ---- frame driver lookup functions
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
 */

#include "grdriver.h"
#include "libgrx.h"

GrxFrameDriver *_GrFindFrameDriver(GrxFrameMode mode)
{
    int ii = 0;
    while (_GrFrameDriverTable[ii] != NULL) {
        if (_GrFrameDriverTable[ii]->mode == mode)
            break;
        ii++;
    }
    return _GrFrameDriverTable[ii];
}

GrxFrameDriver *_GrFindRAMframeDriver(GrxFrameMode mode)
{
    GrxFrameDriver *dp = _GrFindFrameDriver(mode);
    return (dp && dp->is_video) ? _GrFindFrameDriver(dp->rmode) : dp;
}
