/**
 ** imginlne.h ---- Image Utility
 **
 ** by Michal Stencl Copyright (c) 1998 for GRX
 **  <e-mail>      - [stenclpmd@ba.telecom.sk]
 **
 ** This file is part of the GRX graphics library.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 **/

#include "libgrx.h"
#include "image/image.h"

GrxImage *(grx_image_create_from_pattern)(GrxPattern *p) {
  return grx_image_create_from_pattern(p);
}

GrxImage *(grx_image_create_from_context)(GrxContext *c) {
  return grx_image_create_from_context(c);
}

GrxPattern *(grx_pattern_create_from_image)(GrxImage *p) {
  return grx_pattern_create_from_image(p);
}

GrxImage *(grx_pattern_create_from_data)(const unsigned char *pixels,int w,int h,
                                     const GrxColorTable colors     ) {
  return grx_pattern_create_from_data(pixels,w,h,colors);
}

void (grx_image_free)(GrxImage *i) {
  grx_image_free(i);
}
