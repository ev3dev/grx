/*
**  <imginlne.h>  - Image Utility
**                  by Michal Stencl Copyright (c) 1998 for GRX
**
**
**  <e-mail>      - [stenclpmd@ba.telecom.sk]
**
*/

#include "libgrx.h"
#include "image/image.h"

GrImage *(GrImageFromPattern)(GrPattern *p) {
  return GrImageFromPattern(p);
}

GrImage *(GrImageFromContext)(GrContext *c) {
  return GrImageFromContext(c);
}

GrPattern *(GrPatternFromImage)(GrImage *p) {
  return GrPatternFromImage(p);
}

GrImage *(GrImageBuildUsedAsPattern)(char *pixels,int w,int h,
                                     GrColorTableP colors     ) {
  return GrImageBuildUsedAsPattern(pixels,w,h,colors);
}

void (GrImageDestroy)(GrImage *i) {
  GrImageDestroy(i);
}



