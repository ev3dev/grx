/**
 ** APIVERS.C ---- return GRX library version
 **
 ** Copyright (c) 1997 Hartmut Schirmer
 **/

#include "libgrx.h"

unsigned GrGetLibraryVersion(void) {
  return (unsigned)GRX_VERSION_API;
}
