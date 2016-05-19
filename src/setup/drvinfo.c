/**
 ** drvinfo.c ---- the driver info data structure
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
 **/

#include "libgrx.h"
#include "grdriver.h"

#undef GrDriverInfo

static GrxColor dummyframefn(void);

const
struct _GR_driverInfo * const GrDriverInfo = &_GrDriverInfo;
struct _GR_driverInfo _GrDriverInfo = {
    .vdriver = NULL,                    /* video driver */
    .curmode = &DRVINFO->actmode,       /* current video mode pointer */
    .actmode = {                        /* current video mode struct */
        .present       = FALSE,         /* present */
        .bpp           = 4,             /* bpp */
        .width         = 80,            /* geometry */
        .height        = 25,
        .mode          = 3,             /* BIOS mode */
        .line_offset   = 160,           /* line_offset */
        .user_data     = NULL,          /* private */
        .extended_info = NULL,
    },
    /* current frame driver */
    .fdriver = {
        .mode           = GRX_FRAME_MODE_UNDEFINED,
        .rmode          = GRX_FRAME_MODE_UNDEFINED,
        .is_video       = FALSE,
        .row_align      = 1,
        .num_planes     = 1,
        .bits_per_pixel = 0,
        .max_plane_size = 0L,
        .init        = NULL,
        .readpixel   = (GrxColor (*)(GrxFrame*,int,int))dummyframefn,
        .drawpixel   = (void (*)(int,int,GrxColor))dummyframefn,
        .drawline    = (void (*)(int,int,int,int,GrxColor))dummyframefn,
        .drawhline   = (void (*)(int,int,int,GrxColor))dummyframefn,
        .drawvline   = (void (*)(int,int,int,GrxColor))dummyframefn,
        .drawblock   = (void (*)(int,int,int,int,GrxColor))dummyframefn,
        .drawbitmap  = (void (*)(int,int,int,int,unsigned char*,int,int,GrxColor,GrxColor))dummyframefn,
        .drawpattern = (void (*)(int,int,int,unsigned char,GrxColor,GrxColor))dummyframefn,
        .bitblt      = (void (*)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor)) dummyframefn,
        .bltv2r      = (void (*)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor)) dummyframefn,
        .bltr2v      = (void (*)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor)) dummyframefn
    },
    /* screen frame driver */
    .sdriver = {
        .mode           = GRX_FRAME_MODE_UNDEFINED,
        .rmode          = GRX_FRAME_MODE_UNDEFINED,
        .is_video       = FALSE,
        .row_align      = 1,
        .num_planes     = 1,
        .bits_per_pixel = 0,
        .max_plane_size = 0L,
        .init        = NULL,
        .readpixel   = (GrxColor (*)(GrxFrame*,int,int))dummyframefn,
        .drawpixel   = (void (*)(int,int,GrxColor))dummyframefn,
        .drawline    = (void (*)(int,int,int,int,GrxColor))dummyframefn,
        .drawhline   = (void (*)(int,int,int,GrxColor))dummyframefn,
        .drawvline   = (void (*)(int,int,int,GrxColor))dummyframefn,
        .drawblock   = (void (*)(int,int,int,int,GrxColor))dummyframefn,
        .drawbitmap  = (void (*)(int,int,int,int,unsigned char*,int,int,GrxColor,GrxColor))dummyframefn,
        .drawpattern = (void (*)(int,int,int,unsigned char,GrxColor,GrxColor))dummyframefn,
        .bitblt      = (void (*)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor))dummyframefn,
        .bltv2r      = (void (*)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor))dummyframefn,
        .bltr2v      = (void (*)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor))dummyframefn
    },
    /* dummy text mode frame driver */
    .tdriver = {
        .mode           = GRX_FRAME_MODE_TEXT,
        .rmode          = GRX_FRAME_MODE_UNDEFINED,
        .is_video       = TRUE,
        .row_align      = 1,
        .num_planes     = 1,
        .bits_per_pixel = 16,
        .max_plane_size = 0L,
        .init        = NULL,
        .readpixel   = (GrxColor (*)(GrxFrame*,int,int))dummyframefn,
        .drawpixel   = (void (*)(int,int,GrxColor))dummyframefn,
        .drawline    = (void (*)(int,int,int,int,GrxColor))dummyframefn,
        .drawhline   = (void (*)(int,int,int,GrxColor))dummyframefn,
        .drawvline   = (void (*)(int,int,int,GrxColor))dummyframefn,
        .drawblock   = (void (*)(int,int,int,int,GrxColor))dummyframefn,
        .drawbitmap  = (void (*)(int,int,int,int,unsigned char*,int,int,GrxColor,GrxColor))dummyframefn,
        .drawpattern = (void (*)(int,int,int,unsigned char,GrxColor,GrxColor))dummyframefn,
        .bitblt      = (void (*)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor))dummyframefn,
        .bltv2r      = (void (*)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor))dummyframefn,
        .bltr2v      = (void (*)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor))dummyframefn
    },
    .mcode = GRX_GRAPHICS_MODE_TEXT_DEFAULT,    /* current mode code */
    .deftw = 80,  .defth = 25,                  /* default text size */
    .defgw = 640, .defgh = 480,                 /* default graphics size */
    .deftc = 16L, .defgc = 16L,                 /* default txt and gr colors */
    .vposx = 0,   .vposy = 0,                   /* virtual position */
    .errsfatal   = TRUE,                        /* exit upon errors */
    .moderestore = TRUE,                        /* restore startup mode */
    .splitbanks  = FALSE,                       /* split banks */
    .curbank     = (-1),                        /* current bank */
    .set_bank = (void (*)(int))_GrDummyFunction,/* banking func */
    .set_rw_banks = (void (*)(int,int))_GrDummyFunction /* split banking func */
};

static GrxColor dummyframefn(void)
{
        if(DRVINFO->errsfatal) {
            _GrCloseVideoDriver();
            fprintf(stderr,
                "GRX Error: graphics operation attempted %s\n",
                (DRVINFO->fdriver.mode == GRX_FRAME_MODE_TEXT)
                    ? "in text mode" : "before mode set"
            );
            exit(1);
        }
        return(GRX_COLOR_NONE);
}

void _GrDummyFunction(void)
{
        return;
}

