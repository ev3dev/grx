/**
 ** TMPBUFF.C ---- temporary buffer support
 **
 ** Copyright (c) 1998 Hartmut Schirmer
 **
 **/

#include "libgrx.h"
#include "allocate.h"

void far *_GrTempBuffer = NULL;
unsigned  _GrTempBufferBytes = 0;

void far *_GrTempBufferAlloc_(size_t bytes) {
  GRX_ENTER();
  if (bytes > _GrTempBufferBytes || _GrTempBuffer == NULL) {
    void far *neu = farrealloc(_GrTempBuffer, bytes);
    if (neu) {
      _GrTempBuffer = neu;
      _GrTempBufferBytes = bytes;
    }
  }
  GRX_RETURN( (bytes<=_GrTempBufferBytes && _GrTempBuffer)
             ? _GrTempBuffer : NULL);
}

void _GrTempBufferFree(void) {
  if (_GrTempBuffer) farfree(_GrTempBuffer);
  _GrTempBuffer = NULL;
  _GrTempBufferBytes = 0;
}
