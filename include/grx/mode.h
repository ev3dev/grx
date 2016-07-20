/*
 * mode.h
 *
 * Copyright (c) 2015 David Lechner <david@lechnology.com>
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

/**
 * SECTION:mode
 * @short_description: Video drivers and modes
 * @title: Video drivers and modes
 * @section_id: mode
 * @include: grx-3.0.h
 *
 * TODO
 */

/**
 * GrxGraphicsMode:
 * @GRX_GRAPHICS_MODE_UNKNOWN: Unknown mode / initial state
 * @GRX_GRAPHICS_MODE_TEXT_80X25: Standard 80x25 text mode
 * @GRX_GRAPHICS_MODE_TEXT_DEFAULT: Default text mode
 * @GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT: Text mode with parameters int w, int h
 * @GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_COLOR: Text mode with parameters int w,
 *     int h, GrxColor nc
 * @GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_BPP: Text mode with parameters int w,
 *     int h, int bpp
 * @GRX_GRAPHICS_MODE_GRAPHICS_DEFAULT: Default graphics mode
 * @GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT: Graphics mode with parameters
 *     int w, int h
 * @GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_COLOR: Graphics mode with parameters
 *     int w, int h, GrxColor nc
 * @GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_BPP: Graphics mode with parameters
 *     int w, int h, int bpp
 * @GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM: Graphics mode with parameters
 *     int w, int h, GrxColor nc, int vx, int vy
 * @GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM_BPP: Graphics mode with parameters
 *     int w, int h, int bpp, int vx, int vy
 * @GRX_GRAPHICS_MODE_TEXT_80X25_NC: Same as #GRX_GRAPHICS_MODE_TEXT_80X25
 *     but does not clear video memory
 * @GRX_GRAPHICS_MODE_TEXT_DEFAULT_NC: Same as #GRX_GRAPHICS_MODE_TEXT_DEFAULT
 *     but does not clear video memory
 * @GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_NC: Same as #GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT
 *     but does not clear video memory
 * @GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_COLOR_NC: Same as #GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_COLOR
 *     but does not clear video memory
 * @GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_BPP_NC: Same as #GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_BPP
 *     but does not clear video memory
 * @GRX_GRAPHICS_MODE_GRAPHICS_DEFAULT_NC: Same as #GRX_GRAPHICS_MODE_GRAPHICS_DEFAULT
 *     but does not clear video memory
 * @GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_NC: Same as #GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT
 *     but does not clear video memory
 * @GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_COLOR_NC: Same as #GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_COLOR
 *     but does not clear video memory
 * @GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_BPP_NC: Same as #GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_BPP
 *     but does not clear video memory
 * @GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM_NC: Same as #GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM
 *     but does not clear video memory
 * @GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM_BPP_NC: Same as #GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM_BPP
 *     but does not clear video memory
 *
 * available video modes (for 'grx_set_mode')
 */
typedef enum {
    GRX_GRAPHICS_MODE_UNKNOWN = (-1),   /* initial state */
    /* ============= modes which clear the video memory ============= */
    GRX_GRAPHICS_MODE_TEXT_80X25 = 0,                 /* Extra parameters for grx_set_mode: */
    GRX_GRAPHICS_MODE_TEXT_DEFAULT,
    GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT,              /* int w,int h */
    GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_COLOR,        /* int w,int h,GrxColor nc */
    GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_BPP,          /* int w,int h,int bpp */
    GRX_GRAPHICS_MODE_GRAPHICS_DEFAULT,
    GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT,          /* int w,int h */
    GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_COLOR,    /* int w,int h,GrxColor nc */
    GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_BPP,      /* int w,int h,int bpp */
    GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM,                /* int w,int h,GrxColor nc,int vx,int vy */
    GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM_BPP,            /* int w,int h,int bpp,int vx,int vy */
    /* ==== equivalent modes which do not clear the video memory ==== */
    GRX_GRAPHICS_MODE_TEXT_80X25_NC,
    GRX_GRAPHICS_MODE_TEXT_DEFAULT_NC,
    GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_NC,           /* int w,int h */
    GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_COLOR_NC,     /* int w,int h,GrxColor nc */
    GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_BPP_NC,       /* int w,int h,int bpp */
    GRX_GRAPHICS_MODE_GRAPHICS_DEFAULT_NC,
    GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_NC,       /* int w,int h */
    GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_COLOR_NC, /* int w,int h,GrxColor nc */
    GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_BPP_NC,   /* int w,int h,int bpp */
    GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM_NC,             /* int w,int h,GrxColor nc,int vx,int vy */
    GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM_BPP_NC          /* int w,int h,int bpp,int vx,int vy */
} GrxGraphicsMode;

/**
 * GrxFrameMode:
 * @GRX_FRAME_MODE_UNDEFINED: Undefined
 * @GRX_FRAME_MODE_TEXT: Text mode
 * @GRX_FRAME_MODE_LFB_MONO01: Linear frame buffer, 1bpp, 0 = white, 1 = black
 * @GRX_FRAME_MODE_LFB_MONO10: Linear frame buffer, 1bpp, 0 = black, 1 = white
 * @GRX_FRAME_MODE_LFB_8BPP: Linear frame buffer, 8bpp, 256 color
 * @GRX_FRAME_MODE_LFB_16BPP: Linear frame buffer, 16bpp, 32768/65536 color
 * @GRX_FRAME_MODE_LFB_24BPP: Linear frame buffer, 24bpp, 16M color
 * @GRX_FRAME_MODE_LFB_32BPP_LOW: Linear frame buffer, 32bpp, 16M color using lower 24 bits
 * @GRX_FRAME_MODE_LFB_32BPP_HIGH: Linear frame buffer, 32bpp, 16M color using upper 24 bits
 * @GRX_FRAME_MODE_RAM_1BPP: RAM frame buffer, 1bpp, monochome
 * @GRX_FRAME_MODE_RAM_4BPP: RAM frame buffer, 4bpp, 16 color
 * @GRX_FRAME_MODE_RAM_8BPP: RAM frame buffer, 8bpp, 256 color
 * @GRX_FRAME_MODE_RAM_16BPP: RAM frame buffer, 16bpp, 32768/65536 color
 * @GRX_FRAME_MODE_RAM_24BPP: RAM frame buffer, 24bpp, 16M color
 * @GRX_FRAME_MODE_RAM_32BPP_LOW: RAM frame buffer, 32bpp, 16M color using lower 24 bits
 * @GRX_FRAME_MODE_RAM_32BPP_HIGH: RAM frame buffer, 32bpp, 16M color using upper 24 bits
 * @GRX_FRAME_MODE_RAM_3X8BPP: RAM frame buffer, 3x8bpp, 16M color in 3 planes
 *
 * Available frame modes (video memory layouts)
 */
enum _GrxFrameMode {
    GRX_FRAME_MODE_UNDEFINED,           /* undefined */
    GRX_FRAME_MODE_TEXT,                /* text modes */
    /* ==== linear frame buffer modes  ====== */
    GRX_FRAME_MODE_LFB_MONO01,          /* mono, 0 = white, 1 = black */
    GRX_FRAME_MODE_LFB_MONO10,          /* mono, 0 = black, 1 = white */
    GRX_FRAME_MODE_LFB_8BPP,            /* (Super) VGA 256 color */
    GRX_FRAME_MODE_LFB_16BPP,           /* Super VGA 32768/65536 color */
    GRX_FRAME_MODE_LFB_24BPP,           /* Super VGA 16M color */
    GRX_FRAME_MODE_LFB_32BPP_LOW,       /* Super VGA 16M color padded #1 */
    GRX_FRAME_MODE_LFB_32BPP_HIGH,      /* Super VGA 16M color padded #2 */
    /* ====== system RAM frame buffer modes ====== */
    GRX_FRAME_MODE_RAM_1BPP,            /* mono */
    GRX_FRAME_MODE_RAM_4BPP,            /* 16 color planar */
    GRX_FRAME_MODE_RAM_8BPP,            /* 256 color */
    GRX_FRAME_MODE_RAM_16BPP,           /* 32768/65536 color */
    GRX_FRAME_MODE_RAM_24BPP,           /* 16M color */
    GRX_FRAME_MODE_RAM_32BPP_LOW,       /* 16M color padded #1 */
    GRX_FRAME_MODE_RAM_32BPP_HIGH,      /* 16M color padded #2 */
    GRX_FRAME_MODE_RAM_3X8BPP,          /* 16M color planar (image mode) */
    /*< private >*/
    /* ====== markers for scanning modes ====== */
    GRX_FRAME_MODE_FIRST_TEXT     = GRX_FRAME_MODE_TEXT,
    GRX_FRAME_MODE_LAST_TEXT      = GRX_FRAME_MODE_TEXT,
    GRX_FRAME_MODE_FIRST_GRAPHICS = GRX_FRAME_MODE_LFB_MONO01,
    GRX_FRAME_MODE_LAST_GRAPHICS  = GRX_FRAME_MODE_LFB_32BPP_HIGH,
    GRX_FRAME_MODE_FIRST_RAM      = GRX_FRAME_MODE_RAM_1BPP,
    GRX_FRAME_MODE_LAST_RAM       = GRX_FRAME_MODE_RAM_3X8BPP
};

/**
 * GrxVideoAdapterType:
 * @GRX_VIDEO_ADAPTER_TYPE_UNKNOWN: Unknown (default state before driver is set)
 * @GRX_VIDEO_ADAPTER_TYPE_MEMORY: Memory-only adapter
 * @GRX_VIDEO_ADAPTER_TYPE_XWIN: X11 adapter
 * @GRX_VIDEO_ADAPTER_TYPE_WIN32: Windows adapter
 * @GRX_VIDEO_ADAPTER_TYPE_LINUX_FB: Linux framebuffer
 * @GRX_VIDEO_ADAPTER_TYPE_SDL: Simple DirectMedia Layer adapter
 * @GRX_VIDEO_ADAPTER_TYPE_GTK3: Gtk+ 3 adapter
 *
 * Supported video adapter types.
 */
typedef enum {
    GRX_VIDEO_ADAPTER_TYPE_UNKNOWN = (-1),
    GRX_VIDEO_ADAPTER_TYPE_MEMORY,
    GRX_VIDEO_ADAPTER_TYPE_XWIN,
    GRX_VIDEO_ADAPTER_TYPE_WIN32,
    GRX_VIDEO_ADAPTER_TYPE_LINUX_FB,
    GRX_VIDEO_ADAPTER_TYPE_SDL,
    GRX_VIDEO_ADAPTER_TYPE_GTK3,
} GrxVideoAdapterType;

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
 * @name: The name of the driver
 * @adapter: The adapter type
 * @inherit: Video modes from this driver will be inherited
 * @modes: Table of supported modes
 * @n_modes: Number of modes in @modes
 * @detect: Function to detect the driver
 * @init: Function to initialize the driver
 * @reset: Function to reset the driver
 * @select_mode: Function to select the video mode of the driver
 * @flags: Driver flags
 *
 * The video driver descriptor structure
 */
struct _GrxVideoDriver {
    gchar                   *name;
    GrxVideoAdapterType     adapter;
    GrxVideoDriver          *inherit;
    GrxVideoMode            *modes;
    gint                    n_modes;
    gboolean                (*detect)(void);
    gboolean                (*init)(const gchar *options);
    void                    (*reset)(void);
    GrxVideoMode            *(*select_mode)(GrxVideoDriver *drv, gint w, gint h,
                                            gint bpp, gboolean txt, guint *ep);
    GrxVideoDriverFlags     flags;
};

/**
 * GrxVideoMode:
 * @present: Indicates if the video mode is actually present
 * @bpp: Bits per pixel
 * @width: Width in pixels
 * @height: Height in pixels
 * @mode: BIOS mode number (if any)
 * @line_offset: Scan line length
 * @user_data: Can be used by the driver for anything.
 * @extended_info: Extra info (may be shared with other video modes)
 * 
 * Video driver mode descriptor structure
 */
struct _GrxVideoMode {
    gboolean         present;
    guint8           bpp;
    guint16          width;
    guint16          height;
    guint16          mode;
    gint             line_offset;
    gpointer         user_data;
    GrxVideoModeExt *extended_info;
};

/**
 * GrxVideoModeFlags:
 * @GRX_VIDEO_MODE_FLAG_LINEAR: Uses linear memory mapping
 * @GRX_VIDEO_MODE_FLAG_ACCEL: Uses an acellerated video mode
 * @GRX_VIDEO_MODE_FLAG_FAST_SVGA8: Uses faster mixed-planar access
 * @GRX_VIDEO_MODE_FLAG_MEMORY: Uses memory only (virtual screen)
 *
 * Video mode flag bits (in the #GrxVideoModeExt structure)
 */
typedef enum /*< flags >*/ {
    GRX_VIDEO_MODE_FLAG_LINEAR        = 0x01,
    GRX_VIDEO_MODE_FLAG_ACCEL         = 0x02,
    GRX_VIDEO_MODE_FLAG_FAST_SVGA8    = 0x04,
    GRX_VIDEO_MODE_FLAG_MEMORY        = 0x08,
} GrxVideoModeFlags;

/**
 * GrxVideoModeExt:
 * @mode: Frame driver mode for this video mode
 * @drv: Optional frame driver override
 * @frame: Frame buffer address
 * @cprec: Color component precisions
 * @cpos: Color component positions
 * @flags: Video mode flags
 * @setup: Setup function
 * @set_virtual_size: Set virtual size function
 * @scroll: Scroll function
 * @set_bank: Set bank function
 * @set_rw_banks: Set read/write banks function
 * @load_color: Load color pallet function
 * @lfb_selector: Linear frame buffer selector
 *
 * Video driver mode descriptor extension structure. This is a separate
 * structure accessed via a pointer from the main mode descriptor. The
 * reason for this is that frequently several modes can share the same
 * extended info.
 */
struct _GrxVideoModeExt {
    GrxFrameMode mode;                  /* frame driver for this video mode */
    GrxFrameDriver *drv;                /* optional frame driver override */
    guint8 *frame;                      /* frame buffer address */
    guint8  cprec[3];                   /* color component precisions */
    guint8  cpos[3];                    /* color component bit positions */
    GrxVideoModeFlags flags;            /* mode flag bits; see "grdriver.h" */
    gboolean (*setup)(GrxVideoMode *md, gboolean no_clear);
    gboolean (*set_virtual_size)(GrxVideoMode *md, guint w, guint h, GrxVideoMode *result);
    gboolean (*scroll)(GrxVideoMode *md, gint x, gint y, gint result[2]);
    void  (*set_bank)(gint bank);
    void  (*set_rw_banks)(gint read_bank, gint write_bank);
    void  (*load_color)(GrxColor c, GrxColor r, GrxColor g, GrxColor b);
    gint    lfb_selector;
};

/**
 * GrxFrameDriver:
 * @mode: The supported frame access mode.
 * @rmode: The compatible ram frame mode (only if this is a video driver)
 * @is_video: Indicates that this is a video driver
 * @row_align: Scan line size alignment
 * @num_planes: Number of planes
 * @max_plane_size: The maximum plan size in bytes
 * @init:
 * @readpixel:
 * @drawpixel:
 * @drawline:
 * @drawhline:
 * @drawvline:
 * @drawblock:
 * @drawbitmap:
 * @drawpattern:
 * @bitblt:
 * @bltv2r:
 * @bltr2v:
 * @getindexedscanline:
 * @putscanline:
 *
 * The frame driver descriptor structure.
 */
struct _GrxFrameDriver {
    GrxFrameMode mode;                   /* supported frame access mode */
    GrxFrameMode rmode;                  /* matching RAM frame (if video) */
    gboolean is_video;                   /* video RAM frame driver ? */
    gint     row_align;                  /* scan line size alignment */
    gint     num_planes;                 /* number of planes */
    gint     bits_per_pixel;             /* bits per pixel */
    glong    max_plane_size;             /* maximum plane size in bytes */
    gboolean (*init)(GrxVideoMode *md);
    GrxColor (*readpixel)(GrxFrame *c, gint x, gint y);
    void     (*drawpixel)(gint x, gint y, GrxColor c);
    void     (*drawline)(gint x, gint y, gint dx, gint dy, GrxColor c);
    void     (*drawhline)(gint x, gint y, gint w, GrxColor c);
    void     (*drawvline)(gint x, gint y, gint h, GrxColor c);
    void     (*drawblock)(gint x, gint y, gint w, gint h, GrxColor c);
    void     (*drawbitmap)(gint x, gint y, gint w, gint h, guint8 *bmp,
                           gint pitch, gint start, GrxColor fg, GrxColor bg);
    void     (*drawpattern)(gint x, gint y, gint w, guint8 patt, GrxColor fg, GrxColor bg);
    void     (*bitblt)(GrxFrame *dst, gint dx, gint dy, GrxFrame *src,
                       gint x, gint y, gint w, gint h, GrxColor op);
    void     (*bltv2r)(GrxFrame *dst, gint dx, gint dy, GrxFrame *src,
                       gint x, gint y, gint w, gint h, GrxColor op);
    void     (*bltr2v)(GrxFrame *dst, gint dx, gint dy, GrxFrame *src,
                       gint x, gint y, gint w, gint h, GrxColor op);
    GrxColor *(*getindexedscanline)(GrxFrame *c, gint x, gint y, gint w, gint *indx);
      /* will return an array of pixel values pv[] read from frame   */
      /*    if indx == NULL: pv[i=0..w-1] = readpixel(x+i,y)         */
      /*    else             pv[i=0..w-1] = readpixel(x+indx[i],y)   */
    void     (*putscanline)(gint x, gint y, gint w, const GrxColor *scl, GrxColor op);
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
        GrxVideoDriver     *vdriver;        /* the current video driver */
        GrxVideoMode       *curmode;        /* current video mode pointer */
        GrxVideoMode        actmode;        /* copy of above, resized if virtual */
        GrxFrameDriver      fdriver;        /* frame driver for the current context */
        GrxFrameDriver      sdriver;        /* frame driver for the screen */
        GrxFrameDriver      tdriver;        /* a dummy driver for text modes */
        GrxGraphicsMode     mcode;          /* code for the current mode */
        GrxDeviceManager   *device_manager; /* device manager for current video driver */
        int     deftw,defth;                /* default text mode size */
        int     defgw,defgh;                /* default graphics mode size */
        GrxColor deftc,defgc;               /* default text and graphics colors */
        int     vposx,vposy;                /* current virtual viewport position */
        int     errsfatal;                  /* if set, exit upon errors */
        int     moderestore;                /* restore startup video mode if set */
        int     splitbanks;                 /* indicates separate R/W banks */
        int     curbank;                    /* currently mapped bank */
        void  (*set_bank)(int bk);          /* banking routine */
        void  (*set_rw_banks)(int rb,int wb); /* split banking routine */
} * const GrDriverInfo;

#endif /* __GTK_DOC_IGNORE__ */
#endif /* __GI_SCANNER__ */

/*
 * setup stuff
 */
gboolean grx_set_driver(const gchar *driver_spec, GError **error);
gboolean grx_set_mode(GrxGraphicsMode mode, ...);
gboolean grx_set_mode_default_graphics(gboolean clear);
void grx_set_restore_mode(gboolean restore);
void grx_set_error_handling(gboolean exit_if_error);

/*
 * inquiry stuff ---- many of these are actually macros (see below)
 */
GrxGraphicsMode grx_get_current_graphics_mode(void);
GrxVideoAdapterType grx_get_adapter_type(void);
GrxFrameMode    grx_get_current_frame_mode(void);
GrxFrameMode    grx_get_screen_frame_mode(void);
GrxFrameMode    grx_get_core_frame_mode(void);

const GrxVideoDriver *grx_get_current_video_driver(void);
const GrxVideoMode   *grx_get_current_video_mode(void);
const GrxVideoMode   *grx_get_virtual_video_mode(void);
const GrxFrameDriver *grx_get_current_frame_driver(void);
const GrxFrameDriver *grx_get_screen_frame_driver(void);
const GrxVideoMode   *grx_get_first_video_mode(GrxFrameMode mode);
const GrxVideoMode   *grx_get_next_video_mode(const GrxVideoMode *prev);

/*
 * RAM context geometry and memory allocation inquiry stuff
 */

gint  grx_frame_mode_get_n_planes(GrxFrameMode mode);
gint  grx_frame_mode_get_line_offset(GrxFrameMode mode, gint width);
glong grx_frame_mode_get_plane_size(GrxFrameMode mode, gint width, gint height);
glong grx_frame_mode_get_context_size(GrxFrameMode mode, gint w, gint h);

gint  grx_get_n_planes(void);
gint  grx_get_line_offset(gint width);
glong grx_get_plane_size(gint w, gint h);
glong grx_get_context_size(gint w, gint h);

/*
 * inline implementation for some of the above
 */
#ifndef GRX_SKIP_INLINES
#define grx_get_adapter_type() \
    (GrDriverInfo->vdriver ? GrDriverInfo->vdriver->adapter : GRX_VIDEO_ADAPTER_TYPE_UNKNOWN)
#define grx_get_current_graphics_mode() (GrDriverInfo->mcode)
#define grx_get_current_frame_mode()    (GrDriverInfo->fdriver.mode)
#define grx_get_screen_frame_mode()     (GrDriverInfo->sdriver.mode)
#define grx_get_core_frame_mode()       (GrDriverInfo->sdriver.rmode)
#define grx_get_device_manager()        (GrDriverInfo->device_manager)

#define grx_get_current_video_driver()  ((const GrxVideoDriver *)( GrDriverInfo->vdriver))
#define grx_get_current_video_mode()    ((const GrxVideoMode   *)( GrDriverInfo->curmode))
#define grx_get_virtual_video_mode()    ((const GrxVideoMode   *)(&GrDriverInfo->actmode))
#define grx_get_current_frame_driver()  ((const GrxFrameDriver *)(&GrDriverInfo->fdriver))
#define grx_get_screen_frame_driver()   ((const GrxFrameDriver *)(&GrDriverInfo->sdriver))

#define GrIsFixedMode()      (!(  grx_get_current_video_driver()->flags \
                                   & GRX_VIDEO_DRIVER_FLAG_USER_RESOLUTION))

#define grx_get_n_planes()             grx_frame_mode_get_n_planes(grx_get_core_frame_mode())
#define grx_get_line_offset(w)         grx_frame_mode_get_line_offset(grx_get_core_frame_mode(),w)
#define grx_get_plane_size(w,h)        grx_frame_mode_get_plane_size(grx_get_core_frame_mode(),w,h)
#define grx_get_context_size(w,h)      grx_frame_mode_get_context_size(grx_get_core_frame_mode(),w,h)
#endif  /* GRX_SKIP_INLINES */

#endif /* __GRX_MODE_H__ */
