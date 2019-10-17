/**
 ** fframe.c ---- frame driver lookup functions
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu]
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
 ** 190913 Added GrFrameDriverName (M.Alvarez)
 **/

#include "libgrx.h"
#include "grdriver.h"

GrFrameDriver *_GrFindFrameDriver(GrFrameMode mode)
{
	int ii = 0;
	while(_GrFrameDriverTable[ii] != NULL) {
	    if(_GrFrameDriverTable[ii]->mode == mode) break;
	    ii++;
	}
	return(_GrFrameDriverTable[ii]);
}

GrFrameDriver *_GrFindRAMframeDriver(GrFrameMode mode)
{
	GrFrameDriver *dp = _GrFindFrameDriver(mode);
	return((dp && dp->is_video) ? _GrFindFrameDriver(dp->rmode) : dp);
}

char *GrFrameDriverName(GrFrameMode m) {

  switch(m) {
    case GR_frameUndef: return "Undef";
    case GR_frameText : return "Text";
    case GR_frameHERC1: return "HERC1";
    case GR_frameEGAVGA1: return "EGAVGA1";
    case GR_frameEGA4: return "EGA4";
    case GR_frameSVGA4: return "SVGA4";
    case GR_frameSVGA8: return "SVGA8";
    case GR_frameVGA8X: return "VGA8X";
    case GR_frameSVGA16: return "SVGA16";
    case GR_frameSVGA24: return "SVGA24";
    case GR_frameSVGA32L: return "SVGA32L";
    case GR_frameSVGA32H: return "SVGA32H";
    case GR_frameSVGA8_LFB: return "LFB8";
    case GR_frameSVGA16_LFB: return "LFB16";
    case GR_frameSVGA24_LFB: return "LFB24";
    case GR_frameSVGA32L_LFB: return "LFB32L";
    case GR_frameSVGA32H_LFB: return "LFB32H";
    case GR_frameXWIN1: return "XWIN1";
    case GR_frameXWIN4: return "XWIN4";
    case GR_frameXWIN8: return "XWIN8";
    case GR_frameXWIN16: return "XWIN16";
    case GR_frameXWIN24: return "XWIN24";
    case GR_frameXWIN32L: return "XWIN32L";
    case GR_frameXWIN32H: return "XWIN32H";
    case GR_frameWIN32_1: return "WIN32_1";
    case GR_frameWIN32_4: return "WIN32_4";
    case GR_frameWIN32_8: return "WIN32_8";
    case GR_frameWIN32_16: return "WIN32_16";
    case GR_frameWIN32_24: return "WIN32_24";
    case GR_frameWIN32_32L: return "WIN32_32L";
    case GR_frameWIN32_32H: return "WIN32_32H";
    case GR_frameLNXFB_1: return "LNXFB1";
    case GR_frameLNXFB_4: return "LNXFB4";
    case GR_frameLNXFB_8: return "LNXFB8";
    case GR_frameLNXFB_16: return "LNXFB16";
    case GR_frameLNXFB_24: return "LNXFB24";
    case GR_frameLNXFB_32L: return "LNXFB32L";
    case GR_frameLNXFB_32H: return "LNXFB32H";
    case GR_frameRAM1: return "RAM1";
    case GR_frameRAM4: return "RAM4";
    case GR_frameRAM8: return "RAM8";
    case GR_frameRAM16: return "RAM16";
    case GR_frameRAM24: return "RAM24";
    case GR_frameRAM32L: return "RAM32L";
    case GR_frameRAM32H: return "RAM32H";
  }

  return "UNKNOWN";
}
