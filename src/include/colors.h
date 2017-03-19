/*
 * colors.h ---- GRX library private include file
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

#ifndef __INCLUDE_COLORS_H__
#define __INCLUDE_COLORS_H__

#include <glib.h>

#include <grx/color.h>

/*
 * color stuff
 */
G_GNUC_INTERNAL extern int _GR_firstFreeColor; /* can't access all colors on all systems */
G_GNUC_INTERNAL extern int _GR_lastFreeColor;  /* eg. X11 and other windowing systems    */
G_GNUC_INTERNAL int _GrResetColors(void);      /* like grx_color_info_reset_colors but return true on success */

#ifndef C_OPER
#define C_OPER(color)   (unsigned int)(((GrxColor)(color) >> 24) & 15)
#endif
#define C_WRITE         (int)(GRX_COLOR_MODE_WRITE >> 24)
#define C_XOR           (int)(GRX_COLOR_MODE_XOR   >> 24)
#define C_OR            (int)(GRX_COLOR_MODE_OR    >> 24)
#define C_AND           (int)(GRX_COLOR_MODE_AND   >> 24)
#define C_IMAGE         (int)(GRX_COLOR_MODE_IMAGE >> 24)
#define C_COLOR         GRX_COLOR_VALUE_MASK

#endif /* __INCLUDE_COLOR_H__ */
