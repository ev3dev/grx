/**
 ** ATI2880.C ---- ATI 28800 VGA (The SVGA part on MACH8, etc..) driver
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "grdriver.h"
#include "libgrx.h"
#include "arith.h"
#include "ioport.h"
#include "memmode.h"

static void setbank(int bk)
{
        outport_w(0x1ce,(((bk & 7) << 13) | ((bk & 7) << 9) | 0xb2));
}

static void setrwbanks(int rb,int wb)
{
        outport_w(0x1ce,(((rb & 7) << 13) | ((wb & 7) << 9) | 0xb2));
}

static GrVideoModeExt gr4ext = {
    GR_frameSVGA4,                        /* frame driver */
    NULL,                                /* frame driver override */
    MK_FP(0xa000,0),                        /* frame buffer address */
    { 6, 6, 6 },                        /* color precisions */
    { 0, 0, 0 },                        /* color component bit positions */
    0,                                        /* mode flag bits */
    _GrViDrvSetEGAVGAmode,                /* mode set */
    _GrViDrvVESAsetVirtualSize,                /* virtual size set */
    _GrViDrvVESAvirtualScroll,                /* virtual scroll */
    setbank,                                /* bank set function */
    setrwbanks,                                /* double bank set function */
    _GrViDrvLoadColorVGA4                /* color loader */
};

static GrVideoModeExt gr8ext = {
    GR_frameSVGA8,                        /* frame driver */
    NULL,                                /* frame driver override */
    MK_FP(0xa000,0),                        /* frame buffer address */
    { 6, 6, 6 },                        /* color precisions */
    { 0, 0, 0 },                        /* color component bit positions */
    GR_VMODEF_FAST_SVGA8,                /* mode flag bits */
    _GrViDrvSetEGAVGAmode,                /* mode set */
    _GrViDrvVESAsetVirtualSize,                /* virtual size set */
    _GrViDrvVESAvirtualScroll,                /* virtual scroll */
    setbank,                                /* bank set function */
    setrwbanks,                                /* double bank set function */
    _GrViDrvLoadColorVGA8                /* color loader */
};

static GrVideoMode modes[] = {
    /* pres.  bpp wdt        hgt   BIOS   scan  priv. &ext                                  */
    {  TRUE,  4,  132,  25,   0x23,  264,  0,         &_GrViDrvEGAVGAtextModeExt          },
    {  TRUE,  4,  132,  28,   0x23,  264,  0,         &_GrViDrvEGAVGAcustomTextModeExt },
    {  TRUE,  4,  132,  44,   0x33,  264,  0,         &_GrViDrvEGAVGAtextModeExt          },
    {  TRUE,  4,  132,  50,   0x23,  264,  0,         &_GrViDrvEGAVGAcustomTextModeExt },
    {  TRUE,  4,  800,  600,  0x54,  100,  0,         &gr4ext                          },
    {  TRUE,  4,  1024, 768,  0x55,  128,  0,         &gr4ext                          },
    {  TRUE,  8,  640,  480,  0x62,  640,  0,         &gr8ext                          },
    {  TRUE,  8,  800,  600,  0x63,  800,  0,         &gr8ext                          }
};

GrVideoDriver _GrVideoDriverATI28800 = {
    "ati28800",                                /* name */
    GR_VGA,                                /* adapter type */
    &_GrVideoDriverSTDVGA,                /* inherit modes from this driver */
    modes,                                /* mode table */
    itemsof(modes),                        /* # of modes */
    NULL,                                /* detection routine */
    _GrViDrvInitEGAVGA,                        /* initialization routine */
    _GrViDrvResetEGAVGA                        /* reset routine */
};

