/**
 ** MEMMODE.H ---- determine how to access video and system RAM
 **                Borland-C++ code
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include <dos.h>

#define  DECLARE_XFER_BUFFER(size)       char XFER_BUFFER[size]
#define  DELETE_XFER_BUFFER
#define  FP86_SEG(p)                     FP_SEG(p)
#define  FP86_OFF(p)                     FP_OFF(p)
