/*
 * mode.h
 *
 * Copyright (c) 2015-2016 David Lechner <david@lechnology.com>
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

#ifndef __GRX_MODE_H__
#define __GRX_MODE_H__

#include <glib.h>

#include <grx/color.h>
#include <grx/common.h>
#include <grx/frame_mode.h>

/**
 * SECTION:mode
 * @short_description: Video drivers and modes
 * @title: Video drivers and modes
 * @section_id: mode
 * @include: grx-3.0.h
 *
 * GRX3 uses video "driver" plugins to provide the graphics back end. For most
 * programs using GRX3, the driver and it's parameters will be automatically
 * selected. The driver and parameters can be manually specified using the
 * GRX_DRIVER environment variable (see @grx_set_driver() for details). The
 * search path for the video driver plugins can also be manually specified using
 * the GRX_PLUGIN_PATH enviornment variable (useful for development).
 *
 * Each video driver can have mutiple video modes. Video modes specify the
 * resolution (width and height in pixels) and the color depth (bits per pixel).
 * Most programs will just call grx_set_mode_default_graphics() to automatically
 * select the default mode of the video driver.
 *
 * Each video driver also has an associated device manager for interacting with
 * input devices such as keyboards, mice and touchscreens.
 */

/**
 * GrxGraphicsMode:
 * @GRX_GRAPHICS_MODE_UNKNOWN: Unknown mode / initial state
 * @GRX_GRAPHICS_MODE_TEXT_DEFAULT: Default text mode
 * @GRX_GRAPHICS_MODE_TEXT_80X25: Standard 80x25 text mode
 * @GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT: Text mode with parameters int w, int h
 * @GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_COLOR: Text mode with parameters int w,
 *      int h, GrxColor nc
 * @GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_BPP: Text mode with parameters int w,
 *      int h, int bpp
 * @GRX_GRAPHICS_MODE_GRAPHICS_DEFAULT: Default graphics mode
 * @GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT: Graphics mode with parameters
 *      int w, int h
 * @GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_COLOR: Graphics mode with parameters
 *      int w, int h, GrxColor nc
 * @GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_BPP: Graphics mode with parameters
 *      int w, int h, int bpp
 * @GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM: Graphics mode with parameters
 *      int w, int h, GrxColor nc, int vx, int vy
 * @GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM_BPP: Graphics mode with parameters
 *      int w, int h, int bpp, int vx, int vy
 * @GRX_GRAPHICS_MODE_TEXT_DEFAULT_NC: Same as #GRX_GRAPHICS_MODE_TEXT_DEFAULT
 *      but does not clear video memory
 * @GRX_GRAPHICS_MODE_TEXT_80X25_NC: Same as #GRX_GRAPHICS_MODE_TEXT_80X25
 *      but does not clear video memory
 * @GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_NC: Same as #GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT
 *      but does not clear video memory
 * @GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_COLOR_NC: Same as
 *      #GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_COLOR but does not clear video memory
 * @GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_BPP_NC: Same as
 *      #GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_BPP but does not clear video memory
 * @GRX_GRAPHICS_MODE_GRAPHICS_DEFAULT_NC: Same as #GRX_GRAPHICS_MODE_GRAPHICS_DEFAULT
 *      but does not clear video memory
 * @GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_NC: Same as
 *      #GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT but does not clear video memory
 * @GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_COLOR_NC: Same as
 *      #GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_COLOR but does not clear video memory
 * @GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_BPP_NC: Same as
 *      #GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_BPP but does not clear video memory
 * @GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM_NC: Same as #GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM
 *      but does not clear video memory
 * @GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM_BPP_NC: Same as
 *      #GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM_BPP but does not clear video memory
 *
 * available video modes (for grx_set_mode())
 */
typedef enum {
    GRX_GRAPHICS_MODE_UNKNOWN = (-1), /* initial state */
    /* ============= modes which clear the video memory ============= */
    GRX_GRAPHICS_MODE_TEXT_DEFAULT = 0, /* Extra parameters for grx_set_mode: */
    GRX_GRAPHICS_MODE_TEXT_80X25,
    GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT,       /* int w,int h */
    GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_COLOR, /* int w,int h,GrxColor nc */
    GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_BPP,   /* int w,int h,int bpp */
    GRX_GRAPHICS_MODE_GRAPHICS_DEFAULT = GRX_GRAPHICS_MODE_TEXT_DEFAULT + 64,
    GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT,       /* int w,int h */
    GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_COLOR, /* int w,int h,GrxColor nc */
    GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_BPP,   /* int w,int h,int bpp */
    GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM,     /* int w,int h,GrxColor nc,int vx,int vy */
    GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM_BPP, /* int w,int h,int bpp,int vx,int vy */
    /* ==== equivalent modes which do not clear the video memory ==== */
    GRX_GRAPHICS_MODE_TEXT_DEFAULT_NC = GRX_GRAPHICS_MODE_TEXT_DEFAULT + 32,
    GRX_GRAPHICS_MODE_TEXT_80X25_NC,
    GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_NC,       /* int w,int h */
    GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_COLOR_NC, /* int w,int h,GrxColor nc */
    GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_BPP_NC,   /* int w,int h,int bpp */
    GRX_GRAPHICS_MODE_GRAPHICS_DEFAULT_NC = GRX_GRAPHICS_MODE_GRAPHICS_DEFAULT + 32,
    GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_NC,       /* int w,int h */
    GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_COLOR_NC, /* int w,int h,GrxColor nc */
    GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_BPP_NC,   /* int w,int h,int bpp */
    GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM_NC,    /* int w,int h,GrxColor nc,int vx,int vy */
    GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM_BPP_NC /* int w,int h,int bpp,int vx,int vy */
} GrxGraphicsMode;

/**
 * GrxVideoDriverFlags:
 * @GRX_VIDEO_DRIVER_FLAG_USER_RESOLUTION: Driver supports arbitrary
 *    user-specified resolution.
 *
 * Flags used by #GrxVideoDriver.
 */
typedef enum /*< flags >*/ {
    GRX_VIDEO_DRIVER_FLAG_USER_RESOLUTION = 0x01,
} GrxVideoDriverFlags;

/**
 * GrxVideoDriver:
 *
 * The video driver descriptor structure.
 */
/*
 * @name: The name of the driver
 * @flags: Driver flags
 * @inherit: Video modes from this driver will be inherited
 * @modes: Table of supported modes
 * @n_modes: Number of modes in @modes
 * @detect: Function to detect the driver
 * @init: Function to initialize the driver
 * @reset: Function to reset the driver
 * @select_mode: Function to select the video mode of the driver
 * @get_dpi: Function to get the display resolution from the video driver
 * @reserved: For future use
 */
struct _GrxVideoDriver {
    /*<private>*/
    gchar *name;
    GrxVideoDriverFlags flags;
    GrxVideoDriver *inherit;
    GrxVideoMode *modes;
    gint n_modes;
    gboolean (*detect)(void);
    gboolean (*init)(const gchar *options);
    void (*reset)(void);
    GrxVideoMode *(*select_mode)(
        GrxVideoDriver *drv, gint w, gint h, gint bpp, gboolean txt, guint *ep);
    guint (*get_dpi)(GrxVideoDriver *drv);
    gpointer reserved[6];
};

/**
 * GrxVideoMode:
 *
 * Video driver mode descriptor structure.
 */
/*
 * @present: Indicates if the video mode is actually present
 * @bpp: Bits per pixel
 * @width: Width in pixels
 * @height: Height in pixels
 * @mode: BIOS mode number (if any)
 * @line_offset: Scan line length
 * @user_data: Can be used by the driver for anything.
 * @extended_info: Extra info (may be shared with other video modes)
 */
struct _GrxVideoMode {
    /*<private>*/
    gboolean present;
    guint8 bpp;
    guint16 width;
    guint16 height;
    guint16 mode;
    gint line_offset;
    gpointer user_data;
    GrxVideoModeExt *extended_info;
};

/**
 * GrxVideoModeFlags:
 * @GRX_VIDEO_MODE_FLAG_LINEAR: Uses linear memory mapping
 * @GRX_VIDEO_MODE_FLAG_MEMORY: Uses memory only (virtual screen)
 *
 * Video mode flag bits (in the #GrxVideoModeExt structure)
 */
typedef enum /*< flags >*/ {
    GRX_VIDEO_MODE_FLAG_LINEAR = 0x01,
    GRX_VIDEO_MODE_FLAG_MEMORY = 0x02,
} GrxVideoModeFlags;

/**
 * GrxVideoModeExt:
 *
 * Video driver mode descriptor extension structure. This is a separate
 * structure accessed via a pointer from the main mode descriptor. The
 * reason for this is that frequently several modes can share the same
 * extended info.
 */
struct _GrxVideoModeExt {
    /*<private>*/
    GrxFrameMode mode;       /* frame driver for this video mode */
    GrxFrameDriver *drv;     /* optional frame driver override */
    guint8 *frame;           /* frame buffer address */
    guint8 cprec[3];         /* color component precisions */
    guint8 cpos[3];          /* color component bit positions */
    GrxVideoModeFlags flags; /* mode flag bits; see "grdriver.h" */
    gboolean (*setup)(GrxVideoMode *md, gboolean no_clear);
    gboolean (*set_virtual_size)(
        GrxVideoMode *md, guint w, guint h, GrxVideoMode *result);
    gboolean (*scroll)(GrxVideoMode *md, gint x, gint y, gint result[2]);
    void (*set_bank)(gint bank);
    void (*set_rw_banks)(gint read_bank, gint write_bank);
    void (*load_color)(GrxColor c, GrxColor r, GrxColor g, GrxColor b);
    gint lfb_selector;
};

/**
 * GrxFrameDriver:
 *
 * The frame driver descriptor structure.
 */
struct _GrxFrameDriver {
    /*<private>*/
    GrxFrameMode mode;   /* supported frame access mode */
    GrxFrameMode rmode;  /* matching RAM frame (if video) */
    gboolean is_video;   /* video RAM frame driver ? */
    gint row_align;      /* scan line size alignment */
    gint bits_per_pixel; /* bits per pixel */
    gint max_mem_size;   /* maximum frame memory size in bytes */
    gboolean (*init)(GrxVideoMode *md);
    GrxColor (*readpixel)(const GrxFrame *c, gint x, gint y);
    void (*drawpixel)(gint x, gint y, GrxColor c);
    void (*drawline)(gint x, gint y, gint dx, gint dy, GrxColor c);
    void (*drawhline)(gint x, gint y, gint w, GrxColor c);
    void (*drawvline)(gint x, gint y, gint h, GrxColor c);
    void (*drawblock)(gint x, gint y, gint w, gint h, GrxColor c);
    void (*drawbitmap)(gint x, gint y, gint w, gint h, guint8 *bmp, gint pitch,
        gint start, GrxColor fg, GrxColor bg);
    void (*drawpattern)(gint x, gint y, gint w, guint8 patt, GrxColor fg, GrxColor bg);
    void (*bitblt)(const GrxFrame *dst, gint dx, gint dy, const GrxFrame *src, gint x,
        gint y, gint w, gint h, GrxColor op);
    void (*bltv2r)(const GrxFrame *dst, gint dx, gint dy, const GrxFrame *src, gint x,
        gint y, gint w, gint h, GrxColor op);
    void (*bltr2v)(const GrxFrame *dst, gint dx, gint dy, const GrxFrame *src, gint x,
        gint y, gint w, gint h, GrxColor op);
    GrxColor *(*getindexedscanline)(
        const GrxFrame *c, gint x, gint y, gint w, gint *indx);
    /* will return an array of pixel values pv[] read from frame   */
    /*    if indx == NULL: pv[i=0..w-1] = readpixel(x+i,y)         */
    /*    else             pv[i=0..w-1] = readpixel(x+indx[i],y)   */
    void (*putscanline)(gint x, gint y, gint w, const GrxColor *scl, GrxColor op);
    /* will draw scl[i=0..w-1] to frame:                           */
    /*    if (scl[i] != skipcolor) drawpixel(x+i,y,(scl[i] | op))  */
};

#ifndef __GI_SCANNER__
#ifndef __GTK_DOC_IGNORE__
/* TODO: This struct should probably be private */

/*
 * driver and mode info structure
 */
extern const struct _GR_driverInfo {
    GrxVideoDriver *vdriver;              /* the current video driver */
    GrxVideoMode *curmode;                /* current video mode pointer */
    GrxVideoMode actmode;                 /* copy of above, resized if virtual */
    GrxFrameDriver fdriver;               /* frame driver for the current context */
    GrxFrameDriver sdriver;               /* frame driver for the screen */
    GrxFrameDriver tdriver;               /* a dummy driver for text modes */
    GrxGraphicsMode mcode;                /* code for the current mode */
    GrxDeviceManager *device_manager;     /* device manager for current video driver */
    int deftw, defth;                     /* default text mode size */
    int defgw, defgh;                     /* default graphics mode size */
    GrxColor deftc, defgc;                /* default text and graphics colors */
    int dpi;                              /* override vdriver get_dpi */
    int vposx, vposy;                     /* current virtual viewport position */
    int splitbanks;                       /* indicates separate R/W banks */
    int curbank;                          /* currently mapped bank */
    void (*set_bank)(int bk);             /* banking routine */
    void (*set_rw_banks)(int rb, int wb); /* split banking routine */
    void *reserved[6];                    /* for future use */
} *const GrDriverInfo;

#endif /* __GTK_DOC_IGNORE__ */
#endif /* __GI_SCANNER__ */

/*
 * setup stuff
 */
gboolean grx_set_driver(const gchar *driver_spec, GError **error);
gboolean grx_set_mode(GrxGraphicsMode mode, GError **error, ...);
gboolean grx_set_mode_default_graphics(gboolean clear, GError **error);

/*
 * inquiry stuff ---- many of these are actually macros (see below)
 */
guint grx_get_dpi(void);
GrxDeviceManager *grx_get_device_manager(void);

const GrxVideoDriver *grx_get_current_video_driver(void);
const GrxVideoMode *grx_get_current_video_mode(void);
const GrxVideoMode *grx_get_virtual_video_mode(void);
const GrxFrameDriver *grx_get_current_frame_driver(void);
const GrxFrameDriver *grx_get_screen_frame_driver(void);
const GrxVideoMode *grx_get_first_video_mode(GrxFrameMode mode);
const GrxVideoMode *grx_get_next_video_mode(const GrxVideoMode *prev);

/**
 * GRX_DEFAULT_DPI:
 *
 * The default screen resolution (dots per inch).
 *
 * Used when graphics drivers do not support getting the actual screen resolution.
 */
#define GRX_DEFAULT_DPI 100

/*
 * inline implementation for some of the above
 */
#ifndef GRX_SKIP_INLINES
#define grx_get_device_manager() (GrDriverInfo->device_manager)

#define grx_get_current_video_driver() ((const GrxVideoDriver *)(GrDriverInfo->vdriver))
#define grx_get_current_video_mode()   ((const GrxVideoMode *)(GrDriverInfo->curmode))
#define grx_get_virtual_video_mode()   ((const GrxVideoMode *)(&GrDriverInfo->actmode))
#define grx_get_current_frame_driver() \
    ((const GrxFrameDriver *)(&GrDriverInfo->fdriver))
#define grx_get_screen_frame_driver() ((const GrxFrameDriver *)(&GrDriverInfo->sdriver))

#endif /* GRX_SKIP_INLINES */

#endif /* __GRX_MODE_H__ */
