/*
**  image.h  - Source Image Utility
**             by Michal Stencl Copyright (c) 1998
**
**
**  <e-mail>    - [stenclpmd@ba.telecom.sk]
**
** modifications by Hartmut Schirmer (c) 1998
*/

#ifndef __IMAGE_H_INCLUDED__
#define __IMAGE_H_INCLUDED__

#ifndef __LIBGRX_H_INCLUDED__
#include "libgrx.h"
#endif

#ifndef GrImage
#define GrImage GrPixmap
#endif

int _GrImageTestSize(int wdt,int hgt);
GrImage *_GrImageAllocate(GrContext *ctx, int nwidth,int nheight);

#endif
