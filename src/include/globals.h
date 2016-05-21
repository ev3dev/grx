/*
 * globals.h ---- GRX library private include file
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

#ifndef __INCLUDE_GLOBALS_H__
#define __INCLUDE_GLOBALS_H__

#include <grx-3.0.h>

/*
 * global library data structures
 */
extern  struct _GR_driverInfo  _GrDriverInfo;
extern  struct _GR_contextInfo _GrContextInfo;
extern  struct _GR_colorInfo   _GrColorInfo;
extern  struct _GR_mouseInfo   _GrMouseInfo;

#define GrDriverInfo    (&_GrDriverInfo)
#define GrContextInfo   (&_GrContextInfo)
#define GrColorInfo     (&_GrColorInfo)
#define GrMouseInfo     (&_GrMouseInfo)

#define DRVINFO         (&_GrDriverInfo)
#define CXTINFO         (&_GrContextInfo)
#define CLRINFO         (&_GrColorInfo)
#define MOUINFO         (&_GrMouseInfo)

#define CURC            (&(CXTINFO->current))
#define SCRN            (&(CXTINFO->screen))
#define FDRV            (&(DRVINFO->fdriver))
#define SDRV            (&(DRVINFO->sdriver))
#define VDRV            ( (DRVINFO->vdriver))

#endif /* __INCLUDE_GLOBALS_H__ */
