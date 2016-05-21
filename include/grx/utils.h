/*
 * utils.h
 *
 * Copyright (c) 2015 David Lechner <david@lechnology.com>
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

#ifndef __GRX_UTILS_H__
#define __GRX_UTILS_H__

/* ================================================================== */
/*               MISCELLANEOUS UTILITIY FUNCTIONS                     */
/* ================================================================== */

void grx_resize_gray_map(unsigned char *map,int pitch,int ow,int oh,int nw,int nh);

#endif /* __GRX_UTILS_H__ */
