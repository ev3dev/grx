/*
 * vd_mem.c ---- driver for creating image in memory for later exporting
 *
 * Author:  Andris Pavenis
 * [e-mail: pavenis@acad.latnet.lv]
 *
 * This file is part of the GRX graphics library.
 *
 * The GRX graphics library is free software; you can redistribute it
 * and/or modify it under some conditions; see the "copying.grx" file
 * for details.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <stdio.h>
#include <string.h>

#include <glib.h>
#include <gmodule.h>

#include "globals.h"
#include "grdriver.h"
#include "libgrx.h"

static guint8 *mem_buf = NULL;

static int mem_setmode(GrxVideoMode *mp, int noclear)
{
    return mem_buf != NULL;
}

static GrxVideoModeExt gr1ext = {
    .mode = GRX_FRAME_MODE_RAM_1BPP,     /* frame driver */
    .drv = NULL,                         /* frame driver override */
    .frame = NULL,                       /* frame buffer address */
    .cprec = { 1, 1, 1 },                /* color precisions */
    .cpos = { 0, 0, 0 },                 /* color component bit positions */
    .flags = GRX_VIDEO_MODE_FLAG_MEMORY, /* mode flag bits */
    .setup = mem_setmode,                /* mode set */
    .set_virtual_size = NULL,            /* virtual size set */
    .scroll = NULL,                      /* virtual scroll */
    .set_bank = NULL,                    /* bank set function */
    .set_rw_banks = NULL,                /* double bank set function */
    .load_color = NULL,                  /* color loader */
};

static GrxVideoModeExt gr2ext = {
    .mode = GRX_FRAME_MODE_RAM_2BPP,     /* frame driver */
    .drv = NULL,                         /* frame driver override */
    .frame = NULL,                       /* frame buffer address */
    .cprec = { 2, 2, 2 },                /* color precisions */
    .cpos = { 0, 0, 0 },                 /* color component bit positions */
    .flags = GRX_VIDEO_MODE_FLAG_MEMORY, /* mode flag bits */
    .setup = mem_setmode,                /* mode set */
    .set_virtual_size = NULL,            /* virtual size set */
    .scroll = NULL,                      /* virtual scroll */
    .set_bank = NULL,                    /* bank set function */
    .set_rw_banks = NULL,                /* double bank set function */
    .load_color = NULL,                  /* color loader */
};

static GrxVideoModeExt gr8ext = {
    .mode = GRX_FRAME_MODE_RAM_8BPP,     /* frame driver */
    .drv = NULL,                         /* frame driver override */
    .frame = NULL,                       /* frame buffer address */
    .cprec = { 8, 8, 8 },                /* color precisions */
    .cpos = { 0, 0, 0 },                 /* color component bit positions */
    .flags = GRX_VIDEO_MODE_FLAG_MEMORY, /* mode flag bits */
    .setup = mem_setmode,                /* mode set */
    .set_virtual_size = NULL,            /* virtual size set */
    .scroll = NULL,                      /* virtual scroll */
    .set_bank = NULL,                    /* bank set function */
    .set_rw_banks = NULL,                /* double bank set function */
    .load_color = NULL,                  /* color loader */
};

static GrxVideoModeExt gr24ext = {
    .mode = GRX_FRAME_MODE_RAM_24BPP,    /* frame driver */
    .drv = NULL,                         /* frame driver override */
    .frame = NULL,                       /* frame buffer address */
    .cprec = { 8, 8, 8 },                /* color precisions */
    .cpos = { 0, 8, 16 },                /* color component bit positions */
    .flags = GRX_VIDEO_MODE_FLAG_MEMORY, /* mode flag bits */
    .setup = mem_setmode,                /* mode set */
    .set_virtual_size = NULL,            /* virtual size set */
    .scroll = NULL,                      /* virtual scroll */
    .set_bank = NULL,                    /* bank set function */
    .set_rw_banks = NULL,                /* double bank set function */
    .load_color = NULL,                  /* color loader */
};

static int dummymode(GrxVideoMode *mp, int noclear)
{
    g_clear_pointer(&mem_buf, g_free);
    return TRUE;
}

static GrxVideoModeExt dummyExt = {
    .mode = GRX_FRAME_MODE_TEXT, /* frame driver */
    .drv = NULL,                 /* frame driver override */
    .frame = NULL,               /* frame buffer address */
    .cprec = { 0, 0, 0 },        /* color precisions */
    .cpos = { 0, 0, 0 },         /* color component bit positions */
    .flags = 0,                  /* mode flag bits */
    .setup = dummymode,          /* mode set */
    .set_virtual_size = NULL,    /* virtual size set */
    .scroll = NULL,              /* virtual scroll */
    .set_bank = NULL,            /* bank set function */
    .set_rw_banks = NULL,        /* double bank set function */
    .load_color = NULL,          /* color loader */
};

static GrxVideoMode modes[] = {
    /* pres.  bpp wdt   hgt   BIOS   scan  priv. &ext                             */
    { TRUE, 1, 640, 480, 0x00, 80, 0, &gr1ext },
    { TRUE, 2, 640, 480, 0x00, 160, 0, &gr2ext },
    { TRUE, 8, 640, 480, 0x00, 640, 0, &gr8ext },
    { TRUE, 24, 640, 480, 0x00, 1920, 0, &gr24ext },
    { TRUE, 1, 80, 25, 0x00, 160, 0, &dummyExt }
};

static GrxVideoMode *mem_selectmode(
    GrxVideoDriver *drv, int w, int h, int bpp, int txt, unsigned int *ep)
{
    int index;
    unsigned long size;
    int LineOffset;

    if (txt)
        return _grx_select_mode(drv, w, h, bpp, txt, ep);

    if (w < 1 || h < 1)
        return NULL;

    switch (bpp) {
    case 1:
        index = 0;
        LineOffset = (w + 7) >> 3;
        size = h;
        break;
    case 4:
        index = 1;
        LineOffset = (w + 7) >> 3;
        size = 4 * h;
        break;
    case 8:
        index = 2;
        LineOffset = w;
        size = h;
        break;
    case 24:
        index = 3;
        LineOffset = 3 * w;
        size = h;
        break;
    default:
        return NULL;
    }

    LineOffset = (LineOffset + 7) & (~7); /* align rows to 64bit boundary */
    size *= LineOffset;

    if (((size_t)size) != size)
        return NULL;

    /* why ???       */
    modes[index].width = /* w<320 ? 320 : */ w;
    modes[index].height = /* h<200 ? 200 : */ h;
    modes[index].bpp = bpp;
    modes[index].line_offset = LineOffset;

    modes[index].extended_info->frame = mem_buf = g_realloc(mem_buf, size);
    return _grx_select_mode(drv, w, h, bpp, txt, ep);
}

static void mem_reset(void)
{
    g_clear_pointer(&mem_buf, g_free);
}

G_MODULE_EXPORT GrxVideoDriver grx_memory_video_driver = {
    .name = "memory",               /* name */
    .inherit = NULL,                /* inherit modes from this driver */
    .modes = modes,                 /* mode table */
    .n_modes = G_N_ELEMENTS(modes), /* # of modes */
    .detect = NULL,
    /* detect, */                                   /* detection routine */
    .init = NULL,                                   /* initialization routine */
    .reset = mem_reset,                             /* reset routine */
    .select_mode = mem_selectmode,                  /* special mode select routine */
    .flags = GRX_VIDEO_DRIVER_FLAG_USER_RESOLUTION, /* arbitrary resolution possible */
};
