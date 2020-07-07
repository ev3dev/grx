/**
 ** vd_mem.c ---- driver for creating image in memory for later exporting
 **
 ** Author:  Andris Pavenis
 ** [e-mail: pavenis@acad.latnet.lv]
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
 ** 20190919 Solved a bug, missing color component positions for 24 bpp
 ** 20190919 Added 16 bpp mode (M.Alvarez)
 **/

#include "libgrx.h"
#include "grdriver.h"
#include "arith.h"

static char * MemBuf = NULL;
static unsigned long MemBufSze = 0;

static void FreeMemBuf(void)
{
    if (MemBuf) free(MemBuf);
    MemBuf = NULL;
    MemBufSze = 0;
}

static int AllocMemBuf(unsigned long sze)
{
    int clear = 1;

    if (!MemBuf) {
        MemBuf = calloc(1,(size_t)sze);
        if (!MemBuf) return 0;
        MemBufSze = sze;
        clear = 0;
    }
    if (MemBufSze < sze) {
        MemBuf = realloc(MemBuf,(size_t)sze);
        if (!MemBuf) return 0;
        MemBufSze = sze;
    }
    if (clear) memset(MemBuf,0,sze);
    return 1;
}

static int mem_setmode (GrVideoMode *mp,int noclear);

static GrVideoModeExt gr1ext = {
    GR_frameRAM1,                       /* frame driver */
    NULL,                               /* frame driver override */
    0,                                  /* frame buffer address */
    { 1, 1, 1 },                        /* color precisions */
    { 0, 0, 0 },                        /* color component bit positions */
    GR_VMODEF_MEMORY,                   /* mode flag bits */
    mem_setmode,                        /* mode set */
    NULL,                               /* virtual size set */
    NULL,                               /* virtual scroll */
    NULL,                               /* bank set function */
    NULL,                               /* double bank set function */
    NULL                                /* color loader */
};

static GrVideoModeExt gr4ext = {
    GR_frameRAM4,                       /* frame driver */
    NULL,                               /* frame driver override */
    NULL,                               /* frame buffer address */
    { 8, 8, 8 },                        /* color precisions */
    { 0, 0, 0 },                        /* color component bit positions */
    GR_VMODEF_MEMORY,                   /* mode flag bits */
    mem_setmode,                        /* mode set */
    NULL,                               /* virtual size set */
    NULL,                               /* virtual scroll */
    NULL,                               /* bank set function */
    NULL,                               /* double bank set function */
    NULL                                /* color loader */
};

static GrVideoModeExt gr8ext = {
    GR_frameRAM8,                       /* frame driver */
    NULL,                               /* frame driver override */
    NULL,                               /* frame buffer address */
    { 8, 8, 8 },                        /* color precisions */
    { 0, 0, 0 },                        /* color component bit positions */
    GR_VMODEF_MEMORY,                   /* mode flag bits */
    mem_setmode,                        /* mode set */
    NULL,                               /* virtual size set */
    NULL,                               /* virtual scroll */
    NULL,                               /* bank set function */
    NULL,                               /* double bank set function */
    NULL                                /* color loader */
};

static GrVideoModeExt gr16ext = {
    GR_frameRAM16,                      /* frame driver */
    NULL,                               /* frame driver override */
    NULL,                               /* frame buffer address */
    { 5, 6, 5 },                        /* color precisions */
    {11, 5, 0 },                        /* color component bit positions */
    GR_VMODEF_MEMORY,                   /* mode flag bits */
    mem_setmode,                        /* mode set */
    NULL,                               /* virtual size set */
    NULL,                               /* virtual scroll */
    NULL,                               /* bank set function */
    NULL,                               /* double bank set function */
    NULL                                /* color loader */
};

static GrVideoModeExt gr24ext = {
    GR_frameRAM24,                      /* frame driver */
    NULL,                               /* frame driver override */
    NULL,                               /* frame buffer address */
    { 8, 8, 8 },                        /* color precisions */
    {16, 8, 0 },                        /* color component bit positions */
    GR_VMODEF_MEMORY,                   /* mode flag bits */
    mem_setmode,                        /* mode set */
    NULL,                               /* virtual size set */
    NULL,                               /* virtual scroll */
    NULL,                               /* bank set function */
    NULL,                               /* double bank set function */
    NULL                                /* color loader */
};

static int dummymode (GrVideoMode * mp , int noclear )
{
    FreeMemBuf();
    return TRUE;
}

GrVideoModeExt   dummyExt = {
    GR_frameText,                       /* frame driver */
    NULL,                               /* frame driver override */
    NULL,                               /* frame buffer address */
    { 0, 0, 0 },                        /* color precisions */
    { 0, 0, 0 },                        /* color component bit positions */
    0,                                  /* mode flag bits */
    dummymode,                          /* mode set */
    NULL,                               /* virtual size set */
    NULL,                               /* virtual scroll */
    NULL,                               /* bank set function */
    NULL,                               /* double bank set function */
    NULL                                /* color loader */
};

static GrVideoMode modes[] = {
    /* pres.  bpp wdt   hgt   BIOS   scan  priv. &ext  */
    {  TRUE,  1,  640,  480,  0x00,   80,    0,  &gr1ext    },
    {  TRUE,  4,  640,  480,  0x00,  320,    0,  &gr4ext    },
    {  TRUE,  8,  640,  480,  0x00,  640,    0,  &gr8ext    },
    {  TRUE, 16,  640,  480,  0x00, 1280,    0,  &gr16ext   },
    {  TRUE, 24,  640,  480,  0x00, 1920,    0,  &gr24ext   },
    {  TRUE,  1,   80,   25,  0x00,  160,    0,  &dummyExt  }
};

static int mem_setmode (GrVideoMode *mp,int noclear)
{
     return MemBuf ? TRUE : FALSE;
}

static GrVideoMode * mem_selectmode ( GrVideoDriver * drv, int w, int h,
                                      int bpp, int txt, unsigned int * ep )
{
    int  index;
    unsigned long  size;
    int  LineOffset;

    if (txt) return _gr_selectmode (drv,w,h,bpp,txt,ep);

    if (w < 1 || h < 1) return NULL;

    switch (bpp) {
        case 1:
            index = 0;
            LineOffset = (w + 7) >> 3;
            size = h;
            break;
        case 4:
            index = 1;
            LineOffset = (w + 7) >> 3;
            size = 4*h;
            break;
        case 8:
            index = 2;
            LineOffset = w;
            size = h;
            break;
        case 16:
            index = 3;
            LineOffset = 2*w;
            size = h;
            break;
        case 24:
            index = 4;
            LineOffset = 3*w;
            size = h;
            break;
        default:
            return NULL;
      }

    LineOffset = (LineOffset+7) & (~7); /* align rows to 64bit boundary */
    size *= LineOffset;

    if (((size_t)size) != size) return NULL;

    modes[index].width       = w;
    modes[index].height      = h;
    modes[index].bpp         = bpp;
    modes[index].lineoffset  = LineOffset;

    if ( AllocMemBuf(size) ) {
        modes[index].extinfo->frame = MemBuf;
        return _gr_selectmode (drv,w,h,bpp,txt,ep);
    }
    return FALSE;
}

/*
static int detect (void)
{
    return TRUE;
}
*/

static void mem_reset (void)
{
    if(DRVINFO->moderestore) {
        FreeMemBuf();
    }
}

GrVideoDriver _GrDriverMEM = {
    "memory",                           /* name */
    GR_MEM,                             /* adapter type */
    NULL,                               /* inherit modes from this driver */
    modes,                              /* mode table */
    itemsof(modes),                     /* # of modes */
    NULL, /* detect, */                 /* detection routine */
    NULL,                               /* initialization routine */
    mem_reset,                          /* reset routine */
    mem_selectmode,                     /* special mode select routine */
    GR_DRIVERF_USER_RESOLUTION          /* arbitrary resolution possible */
};
