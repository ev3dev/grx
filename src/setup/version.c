/**
 ** VERSION.C ---- return GRX library version
 **
 ** Copyright (c) 1997 Hartmut Schirmer
 **/

#include "libgrx.h"

unsigned GrGetLibraryVersion(void) {
  return (unsigned)GRX_VERSION_API;
}

unsigned GrGetLibrarySystem(void) {
  return (unsigned)GRX_VERSION;
}
