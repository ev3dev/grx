/**
 ** PATTERNF.C ---- data structure for standard pattern filler
 **
 ** Copyright (c) 1998 Hartmut Schirmer
 */

#include "libgrx.h"
#include "shapes.h"

GrFiller _GrPatternFiller = {
   _GrDrawPatternedPixel,
   _GrDrawPatternedLine,
   _GrFillPatternedScanLine
};
