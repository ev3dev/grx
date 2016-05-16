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
        FALSE,                          /* present */
        4,                              /* bpp */
        80,25,                          /* geometry */
        3,                              /* BIOS mode */
        160,                            /* line_offset */
        0,                              /* private */
        NULL
    },
    .fdriver = {                        /* current frame driver */
        GRX_FRAME_MODE_UNDEFINED,       /* frame mode */
        GRX_FRAME_MODE_UNDEFINED,       /* compatible RAM frame mode */
        FALSE,                          /* onscreen */
        1,                              /* line width alignment */
        1,                              /* number of planes */
        0,                              /* bits per pixel */
        0L,                             /* max plane size the code can handle */
        NULL,
        (GrxColor (*)(GrxFrame*,int,int))                               dummyframefn,
        (void (*)(int,int,GrxColor))                                   dummyframefn,
        (void (*)(int,int,int,int,GrxColor))                           dummyframefn,
        (void (*)(int,int,int,GrxColor))                               dummyframefn,
        (void (*)(int,int,int,GrxColor))                               dummyframefn,
        (void (*)(int,int,int,int,GrxColor))                           dummyframefn,
        (void (*)(int,int,int,int,unsigned char*,int,int,GrxColor,GrxColor)) dummyframefn,
        (void (*)(int,int,int,unsigned char,GrxColor,GrxColor))        dummyframefn,
        (void (*)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor)) dummyframefn,
        (void (*)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor)) dummyframefn,
        (void (*)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor)) dummyframefn
    },
    .sdriver = {                        /* screen frame driver */
        GRX_FRAME_MODE_UNDEFINED,       /* frame mode */
        GRX_FRAME_MODE_UNDEFINED,       /* compatible RAM frame mode */
        FALSE,                          /* onscreen */
        1,                              /* line width alignment */
        1,                              /* number of planes */
        0,                              /* bits per pixel */
        0L,                             /* max plane size the code can handle */
        NULL,
        (GrxColor (*)(GrxFrame*,int,int))                               dummyframefn,
        (void (*)(int,int,GrxColor))                                   dummyframefn,
        (void (*)(int,int,int,int,GrxColor))                           dummyframefn,
        (void (*)(int,int,int,GrxColor))                               dummyframefn,
        (void (*)(int,int,int,GrxColor))                               dummyframefn,
        (void (*)(int,int,int,int,GrxColor))                           dummyframefn,
        (void (*)(int,int,int,int,unsigned char*,int,int,GrxColor,GrxColor)) dummyframefn,
        (void (*)(int,int,int,unsigned char,GrxColor,GrxColor))        dummyframefn,
        (void (*)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor)) dummyframefn,
        (void (*)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor)) dummyframefn,
        (void (*)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor)) dummyframefn
    },
    .tdriver = {                        /* dummy text mode frame driver */
        GRX_FRAME_MODE_TEXT,            /* frame mode */
        GRX_FRAME_MODE_UNDEFINED,       /* compatible RAM frame mode */
        TRUE,                           /* onscreen */
        1,                              /* line width alignment */
        1,                              /* number of planes */
        16,                             /* bits per pixel */
        0L,                             /* max plane size the code can handle */
        NULL,
        (GrxColor (*)(GrxFrame*,int,int))                               dummyframefn,
        (void (*)(int,int,GrxColor))                                   dummyframefn,
        (void (*)(int,int,int,int,GrxColor))                           dummyframefn,
        (void (*)(int,int,int,GrxColor))                               dummyframefn,
        (void (*)(int,int,int,GrxColor))                               dummyframefn,
        (void (*)(int,int,int,int,GrxColor))                           dummyframefn,
        (void (*)(int,int,int,int,unsigned char*,int,int,GrxColor,GrxColor)) dummyframefn,
        (void (*)(int,int,int,unsigned char,GrxColor,GrxColor))        dummyframefn,
        (void (*)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor)) dummyframefn,
        (void (*)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor)) dummyframefn,
        (void (*)(GrxFrame*,int,int,GrxFrame*,int,int,int,int,GrxColor)) dummyframefn
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
    .modehook    = NULL,                        /* mode set hook */
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

