/*
 * frame_mode.h
 *
 * Copyright (c) 2016 David Lechner <david@lechnology.com>
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
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

#ifndef __GRX_FRAME_MODE_H__
#define __GRX_FRAME_MODE_H__

/**
 * SECTION:frame_mode
 * @short_description: Graphics memory layouts
 * @title: Frame Modes
 * @section_id: frame_mode
 * @include: grx-3.0.h
 *
 * Frame modes describe how graphics are stored in memory.
 */

/**
 * GrxFrameMode:
 * @GRX_FRAME_MODE_UNDEFINED: Undefined
 * @GRX_FRAME_MODE_TEXT: Text mode
 * @GRX_FRAME_MODE_LFB_MONO01: Linear frame buffer, 1bpp, 0 = white, 1 = black
 * @GRX_FRAME_MODE_LFB_MONO10: Linear frame buffer, 1bpp, 0 = black, 1 = white
 * @GRX_FRAME_MODE_LFB_2BPP: Linear frame buffer, 2bpp, 4 color
 * @GRX_FRAME_MODE_LFB_8BPP: Linear frame buffer, 8bpp, 256 color
 * @GRX_FRAME_MODE_LFB_16BPP: Linear frame buffer, 16bpp, 32768/65536 color
 * @GRX_FRAME_MODE_LFB_24BPP: Linear frame buffer, 24bpp, 16M color
 * @GRX_FRAME_MODE_LFB_32BPP_LOW: Linear frame buffer, 32bpp, 16M color using lower 24 bits
 * @GRX_FRAME_MODE_LFB_32BPP_HIGH: Linear frame buffer, 32bpp, 16M color using upper 24 bits
 * @GRX_FRAME_MODE_RAM_1BPP: RAM frame buffer, 1bpp, monochome
 * @GRX_FRAME_MODE_RAM_2BPP: RAM frame buffer, 2bpp, 4 color
 * @GRX_FRAME_MODE_RAM_8BPP: RAM frame buffer, 8bpp, 256 color
 * @GRX_FRAME_MODE_RAM_16BPP: RAM frame buffer, 16bpp, 32768/65536 color
 * @GRX_FRAME_MODE_RAM_24BPP: RAM frame buffer, 24bpp, 16M color
 * @GRX_FRAME_MODE_RAM_32BPP_LOW: RAM frame buffer, 32bpp, 16M color using lower 24 bits
 * @GRX_FRAME_MODE_RAM_32BPP_HIGH: RAM frame buffer, 32bpp, 16M color using upper 24 bits
 * @GRX_FRAME_MODE_FIRST_TEXT: The first text mode
 * @GRX_FRAME_MODE_LAST_TEXT: The last text mode
 * @GRX_FRAME_MODE_FIRST_GRAPHICS: The first graphics mode
 * @GRX_FRAME_MODE_LAST_GRAPHICS: The last graphics mode
 * @GRX_FRAME_MODE_FIRST_RAM: The first RAM mode
 * @GRX_FRAME_MODE_LAST_RAM: The last RAM mode
 *
 * Available frame modes (video memory layouts)
 */
typedef enum {
    GRX_FRAME_MODE_UNDEFINED = -1,      /* undefined */
    GRX_FRAME_MODE_TEXT = 0,            /* text modes */
    /* ==== linear frame buffer modes  ====== */
    GRX_FRAME_MODE_LFB_MONO01 = 32,     /* mono, 0 = white, 1 = black */
    GRX_FRAME_MODE_LFB_MONO10,          /* mono, 0 = black, 1 = white */
    GRX_FRAME_MODE_LFB_2BPP,            /* 4 color */
    GRX_FRAME_MODE_LFB_8BPP,            /* (Super) VGA 256 color */
    GRX_FRAME_MODE_LFB_16BPP,           /* Super VGA 32768/65536 color */
    GRX_FRAME_MODE_LFB_24BPP,           /* Super VGA 16M color */
    GRX_FRAME_MODE_LFB_32BPP_LOW,       /* Super VGA 16M color padded #1 */
    GRX_FRAME_MODE_LFB_32BPP_HIGH,      /* Super VGA 16M color padded #2 */
    /* ====== system RAM frame buffer modes ====== */
    GRX_FRAME_MODE_RAM_1BPP = 64,       /* mono */
    GRX_FRAME_MODE_RAM_2BPP,            /* 4 color */
    GRX_FRAME_MODE_RAM_8BPP,            /* 256 color */
    GRX_FRAME_MODE_RAM_16BPP,           /* 32768/65536 color */
    GRX_FRAME_MODE_RAM_24BPP,           /* 16M color */
    GRX_FRAME_MODE_RAM_32BPP_LOW,       /* 16M color padded #1 */
    GRX_FRAME_MODE_RAM_32BPP_HIGH,      /* 16M color padded #2 */
    /* ====== markers for scanning modes ====== */
    GRX_FRAME_MODE_FIRST_TEXT     = GRX_FRAME_MODE_TEXT,
    GRX_FRAME_MODE_LAST_TEXT      = GRX_FRAME_MODE_TEXT,
    GRX_FRAME_MODE_FIRST_GRAPHICS = GRX_FRAME_MODE_LFB_MONO01,
    GRX_FRAME_MODE_LAST_GRAPHICS  = GRX_FRAME_MODE_LFB_32BPP_HIGH,
    GRX_FRAME_MODE_FIRST_RAM      = GRX_FRAME_MODE_RAM_1BPP,
} GrxFrameMode;

GrxFrameMode grx_frame_mode_get_current(void);
GrxFrameMode grx_frame_mode_get_screen(void);
GrxFrameMode grx_frame_mode_get_screen_core(void);

gint grx_frame_mode_get_bpp(GrxFrameMode mode);
gint grx_frame_mode_get_line_offset(GrxFrameMode mode, gint width);
gint grx_frame_mode_get_plane_size(GrxFrameMode mode, gint width, gint height);
gint grx_frame_mode_get_context_size(GrxFrameMode mode, gint width, gint height);

gint grx_screen_get_line_offset(gint width);
gint grx_screen_get_plane_size(gint width, gint height);
gint grx_screen_get_context_size(gint width, gint height);

/*
 * inline implementation for some of the above
 */
#ifndef GRX_SKIP_INLINES

#define grx_frame_mode_get_current()        (GrDriverInfo->fdriver.mode)
#define grx_frame_mode_get_screen()         (GrDriverInfo->sdriver.mode)
#define grx_frame_mode_get_screen_core()    (GrDriverInfo->sdriver.rmode)

#define grx_screen_get_line_offset(w) \
    grx_frame_mode_get_line_offset(grx_frame_mode_get_screen_core(),(w))
#define grx_screen_get_plane_size(w,h) \
    grx_frame_mode_get_plane_size(grx_frame_mode_get_screen_core(),(w),(h))
#define grx_screen_get_context_size(w,h) \
    grx_frame_mode_get_context_size(grx_frame_mode_get_screen_core(),(w),(h))

#endif  /* GRX_SKIP_INLINES */

#endif /* __GRX_FRAME_MODE_H__*/
