/*
 * grx-3.0.h ---- GRX 3.x API functions and data structure declarations
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
 *
 */

#ifndef __GRX_3_0_H_INCLUDED__
#define __GRX_3_0_H_INCLUDED__

#include <glib-2.0/glib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* a couple of forward declarations ... */
typedef struct _GrxFrameDriver  GrxFrameDriver;
typedef struct _GrxVideoDriver  GrxVideoDriver;
typedef struct _GrxVideoMode    GrxVideoMode;
typedef struct _GrxVideoModeExt GrxVideoModeExt;
typedef struct _GrxFrame        GrxFrame;
typedef struct _GrxContext      GrxContext;

/* ================================================================== */
/*                        SYSTEM TYPE DEF's                           */
/* ================================================================== */

/* need unsigned 32 bit integer for color stuff */
typedef guint32 GrxColor;

/* ================================================================== */
/*                           MODE SETTING                             */
/* ================================================================== */

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
typedef enum {
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
} GrxFrameMode;

/**
 * GrxVideoAdapter:
 * @GRX_VIDEO_ADAPTER_UNKNOWN: Unknown (default state before driver is set)
 * @GRX_VIDEO_ADAPTER_XWIN: X11 adapter
 * @GRX_VIDEO_ADAPTER_WIN32: Windows adapter
 * @GRX_VIDEO_ADAPTER_LINUX_FB: Linux framebuffer
 * @GRX_VIDEO_ADAPTER_SDL: Simple DirectMedia Layer adapter
 * @GRX_VIDEO_ADAPTER_MEMORY: Memory-only adapter
 *
 * supported video adapter types
 */
typedef enum {
    GRX_VIDEO_ADAPTER_UNKNOWN = (-1),  /* not known (before driver set) */
    GRX_VIDEO_ADAPTER_XWIN,            /* X11 driver */
    GRX_VIDEO_ADAPTER_WIN32,           /* WIN32 driver */
    GRX_VIDEO_ADAPTER_LINUX_FB,        /* Linux framebuffer */
    GRX_VIDEO_ADAPTER_SDL,             /* SDL driver */
    GRX_VIDEO_ADAPTER_MEMORY           /* memory only driver */
} GrxVideoAdapter;

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
    gchar *name;                        /* driver name */
    GrxVideoAdapter adapter;            /* adapter type */
    struct _GrxVideoDriver  *inherit;   /* inherit video modes from this */
    GrxVideoMode   *modes;              /* table of supported modes */
    gint    n_modes;                    /* number of modes */
    gint  (*detect)(void);
    gint  (*init)(gchar *options);
    void  (*reset)(void);
    GrxVideoMode *(*select_mode)(GrxVideoDriver *drv, gint w, gint h,
                                 gint bpp, gboolean txt, guint *ep);
    GrxVideoDriverFlags flags;
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
    gboolean present;                    /* is it really available? */
    guint8   bpp;                        /* log2 of # of colors */
    guint16  width,height;               /* video mode geometry */
    guint16  mode;                       /* BIOS mode number (if any) */
    gint     line_offset;                /* scan line length */
    gint     user_data;                  /* driver can use it for anything */
    GrxVideoModeExt *extended_info;      /* extra info (maybe shared) */
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
        int     deftw,defth;                /* default text mode size */
        int     defgw,defgh;                /* default graphics mode size */
        GrxColor deftc,defgc;                /* default text and graphics colors */
        int     vposx,vposy;                /* current virtual viewport position */
        int     errsfatal;                  /* if set, exit upon errors */
        int     moderestore;                /* restore startup video mode if set */
        int     splitbanks;                 /* indicates separate R/W banks */
        int     curbank;                    /* currently mapped bank */
        void  (*mdsethook)(void);           /* callback for mode set */
        void  (*set_bank)(int bk);          /* banking routine */
        void  (*set_rw_banks)(int rb,int wb); /* split banking routine */
} * const GrDriverInfo;

typedef void (*GrxHookFunc)(void);

/*
 * setup stuff
 */
gboolean grx_set_driver(gchar *driver_spec);
gboolean grx_set_mode(GrxGraphicsMode mode, ...);
gboolean grx_set_viewport(int xpos, int ypos);
void grx_set_mode_hook_func(GrxHookFunc hook_func);
void grx_set_restore_mode(gboolean restore);
void grx_set_error_handling(gboolean exit_if_error);

/*
 * inquiry stuff ---- many of these are actually macros (see below)
 */
GrxGraphicsMode grx_get_current_graphics_mode(void);
GrxVideoAdapter grx_get_adapter_type(void);
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

gint  grx_get_screen_x(void);
gint  grx_get_screen_y(void);
gint  grx_get_virtual_x(void);
gint  grx_get_virtual_y(void);
gint  grx_get_viewport_x(void);
gint  grx_get_viewport_y(void);

gboolean  grx_is_screen_virtual(void);

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
#define grx_get_adapter_type()         (GrDriverInfo->vdriver ? GrDriverInfo->vdriver->adapter : GRX_VIDEO_ADAPTER_UNKNOWN)
#define grx_get_current_graphics_mode()         (GrDriverInfo->mcode)
#define grx_get_current_frame_mode()    (GrDriverInfo->fdriver.mode)
#define grx_get_screen_frame_mode()     (GrDriverInfo->sdriver.mode)
#define grx_get_core_frame_mode()       (GrDriverInfo->sdriver.rmode)

#define grx_get_current_video_driver()  ((const GrxVideoDriver *)( GrDriverInfo->vdriver))
#define grx_get_current_video_mode()    ((const GrxVideoMode   *)( GrDriverInfo->curmode))
#define grx_get_virtual_video_mode()    ((const GrxVideoMode   *)(&GrDriverInfo->actmode))
#define grx_get_current_frame_driver()  ((const GrxFrameDriver *)(&GrDriverInfo->fdriver))
#define grx_get_screen_frame_driver()   ((const GrxFrameDriver *)(&GrDriverInfo->sdriver))

#define GrIsFixedMode()      (!(  grx_get_current_video_driver()->flags \
                                   & GRX_VIDEO_DRIVER_FLAG_USER_RESOLUTION))

#define grx_get_screen_x()             (grx_get_current_video_mode()->width)
#define grx_get_screen_y()             (grx_get_current_video_mode()->height)
#define grx_get_virtual_x()            (grx_get_virtual_video_mode()->width)
#define grx_get_virtual_y()            (grx_get_virtual_video_mode()->height)
#define grx_get_viewport_x()           (GrDriverInfo->vposx)
#define grx_get_viewport_y()           (GrDriverInfo->vposy)

#define grx_is_screen_virtual()     ((grx_get_screen_x() + grx_get_screen_y()) < (grx_get_virtual_x() + grx_get_virtual_y()))

#define grx_get_n_planes()           grx_frame_mode_get_n_planes(grx_get_core_frame_mode())
#define grx_get_line_offset(w)         grx_frame_mode_get_line_offset(grx_get_core_frame_mode(),w)
#define grx_get_plane_size(w,h)        grx_frame_mode_get_plane_size(grx_get_core_frame_mode(),w,h)
#define grx_get_context_size(w,h)      grx_frame_mode_get_context_size(grx_get_core_frame_mode(),w,h)
#endif  /* GRX_SKIP_INLINES */


/* ================================================================== */
/*              FRAME BUFFER, CONTEXT AND CLIPPING STUFF              */
/* ================================================================== */

struct _GrxFrame {
    guint8         *base_address[4];    /* base address of frame memory */
    gshort          selector;           /* frame memory segment selector */
    gboolean        is_on_screen;       /* is it in video memory ? */
    guint8          memory_flags;       /* memory allocation flags */
    gint            line_offset;        /* offset to next scan line in bytes */
    GrxFrameDriver *driver;             /* frame access functions */
};

struct _GrxContext {
    GrxFrame    frame;                  /* frame buffer info */
    GrxContext *root;                   /* context which owns frame */
    int    x_max;                       /* max X coord (width  - 1) */
    int    y_max;                       /* max Y coord (height - 1) */
    int    x_offset;                    /* X offset from root's base */
    int    y_offset;                    /* Y offset from root's base */
    int    x_clip_low;                  /* low X clipping limit */
    int    y_clip_low;                  /* low Y clipping limit */
    int    x_clip_high;                 /* high X clipping limit */
    int    y_clip_high;                 /* high Y clipping limit */
    int    user_x_base;                 /* user window min X coordinate */
    int    user_y_base;                 /* user window min Y coordinate */
    int    user_width;                  /* user window width  */
    int    user_height;                 /* user window height */
#   define gc_base_address              frame.base_address
#   define gc_selector                  frame.selector
#   define gc_is_on_screen              frame.is_on_screen
#   define gc_memory_flags              frame.memory_flags
#   define gc_line_offset               frame.line_offset
#   define gc_driver                    frame.driver
};

extern const struct _GR_contextInfo {
    GrxContext current;         /* the current context */
    GrxContext screen;          /* the screen context */
} * const GrContextInfo;

GrxContext *grx_context_create(gint w, gint h, guint8 *memory[4], GrxContext *where);
GrxContext *grx_context_create_full(GrxFrameMode md, gint w, gint h,
                                    guint8 *memory[4], GrxContext *where);
GrxContext *grx_context_create_subcontext(gint x1, gint y1, gint x2, gint y2,
                                          const GrxContext *parent, GrxContext *where);
GrxContext *grx_context_save(GrxContext *where);

GrxContext *grx_context_get_current(void);
GrxContext *grx_context_get_screen(void);

void  grx_context_free(GrxContext *context);
void  grx_context_resize_subcontext(GrxContext *context, gint x1, gint y1, gint x2, gint y2);
void  grx_context_set_current(const GrxContext *context);

void  grx_set_clip_box(gint x1, gint y1, gint x2, gint y2);
void  grx_context_set_clip_box(GrxContext *c, gint x1, gint y1, gint x2, gint y2);
void  grx_get_clip_box(gint *x1p, gint *y1p, gint *x2p, gint *y2p);
void  grx_context_get_clip_box(const GrxContext *c, gint *x1p, gint *y1p, gint *x2p, gint *y2p);
void  grx_reset_clip_box(void);
void  grx_context_reset_clip_box(GrxContext *c);

int   grx_get_max_x(void);
int   grx_get_max_y(void);
int   grx_get_size_x(void);
int   grx_get_size_y(void);
int   grx_get_low_x(void);
int   grx_get_low_y(void);
int   grx_get_high_x(void);
int   grx_get_high_y(void);

#ifndef GRX_SKIP_INLINES
#define grx_context_create(w,h,m,c) (grx_context_create_full(grx_get_core_frame_mode(),w,h,m,c))
#define grx_context_get_current()       ((GrxContext *)(&GrContextInfo->current))
#define grx_context_get_screen()        ((GrxContext *)(&GrContextInfo->screen))
#define grx_get_max_x()                 (grx_context_get_current()->x_max)
#define grx_get_max_y()                 (grx_context_get_current()->y_max)
#define grx_get_size_x()                (grx_get_max_x() + 1)
#define grx_get_size_y()                (grx_get_max_y() + 1)
#define grx_get_low_x()                 (grx_context_get_current()->x_clip_low)
#define grx_get_low_y()                 (grx_context_get_current()->y_clip_low)
#define grx_get_high_x()                (grx_context_get_current()->x_clip_high)
#define grx_get_high_y()                (grx_context_get_current()->y_clip_high)
#define grx_context_get_clip_box(C,x1p,y1p,x2p,y2p) do {           \
    *(x1p) = (C)->x_clip_low;                           \
    *(y1p) = (C)->y_clip_low;                           \
    *(x2p) = (C)->x_clip_high;                          \
    *(y2p) = (C)->y_clip_high;                          \
} while(0)
#define grx_get_clip_box(x1p,y1p,x2p,y2p) do {              \
    *(x1p) = grx_get_low_x();                                  \
    *(y1p) = grx_get_low_y();                                  \
    *(x2p) = grx_get_high_x();                                 \
    *(y2p) = grx_get_high_y();                                 \
} while(0)
#endif  /* GRX_SKIP_INLINES */

/* ================================================================== */
/*                            COLOR STUFF                             */
/* ================================================================== */

/**
 * GrxColorMode:
 *
 * Flags to 'OR' to colors for various operations
 */
typedef enum /*< flags >*/ {
    GRX_COLOR_MODE_WRITE = 0UL,           /* write color */
    GRX_COLOR_MODE_XOR   = 0x01000000UL,  /* to "XOR" any color to the screen */
    GRX_COLOR_MODE_OR    = 0x02000000UL,  /* to "OR" to the screen */
    GRX_COLOR_MODE_AND   = 0x03000000UL,  /* to "AND" to the screen */
    GRX_COLOR_MODE_IMAGE = 0x04000000UL,  /* BLIT: write, except given color */
} GrxColorMode;

#define GRX_COLOR_VALUE_MASK 0x00ffffffUL /* color value mask */
#define GRX_COLOR_MODE_MASK  0xff000000UL /* color operation mask */
#define GRX_COLOR_NONE ((GrxColor)(GRX_COLOR_MODE_XOR | 0)) /* GRX_COLOR_NONE is used for "no" color */

GrxColor grx_color_get_value(GrxColor c);
GrxColorMode grx_color_get_mode(GrxColor c);
GrxColor grx_color_to_write_mode(GrxColor c);
GrxColor grx_color_to_xor_mode(GrxColor c);
GrxColor grx_color_to_or_mode(GrxColor c);
GrxColor grx_color_to_and_mode(GrxColor c);
GrxColor grx_color_to_image_mode(GrxColor c);

/*
 * color system info structure (all [3] arrays are [r,g,b])
 */
extern const struct _GR_colorInfo {
    GrxColor       ncolors;             /* number of colors */
    GrxColor       nfree;               /* number of unallocated colors */
    GrxColor       black;               /* the black color */
    GrxColor       white;               /* the white color */
    unsigned int  RGBmode;              /* set when RGB mode */
    unsigned int  prec[3];              /* color field precisions */
    unsigned int  pos[3];               /* color field positions */
    unsigned int  mask[3];              /* masks for significant bits */
    unsigned int  round[3];             /* add these for rounding */
    unsigned int  shift[3];             /* shifts for (un)packing color */
    unsigned int  norm;                 /* normalization for (un)packing */
    struct {                            /* color table for non-RGB modes */
        unsigned char r,g,b;            /* loaded components */
        unsigned int  defined:1;        /* r,g,b values are valid if set */
        unsigned int  writable:1;       /* can be changed by 'grx_color_info_set_color' */
        unsigned long int nused;        /* usage count */
    } ctable[256];
} * const GrColorInfo;

void     grx_color_info_reset_colors(void);
void     grx_color_info_set_rgb_color_mode(void);
void     grx_color_info_refresh_colors(void);

gint     grx_color_info_n_colors(void);
gint     grx_color_info_n_free_colors(void);

GrxColor grx_color_info_get_black(void);
GrxColor grx_color_info_get_white(void);

GrxColor grx_color_info_build_rgb_color_t(gint r, gint g, gint b);
GrxColor grx_color_info_build_rgb_color_r(gint r, gint g, gint b);
gint     grx_color_info_get_red_value(GrxColor c);
gint     grx_color_info_get_green_value(GrxColor c);
gint     grx_color_info_get_blue_value(GrxColor c);

GrxColor grx_color_info_alloc_color(gint r, gint g, gint b);    /* shared, read-only */
GrxColor grx_color_info_alloc_color_id(gint r, gint g, gint b); /* potentially inlined version */
GrxColor grx_color_info_alloc_color2(glong hcolor);             /* shared, read-only, 0xRRGGBB */
GrxColor grx_color_info_alloc_color2_id(glong hcolor);          /* potentially inlined version */
GrxColor grx_color_info_alloc_cell(void);                       /* unshared, read-write */

/**
 * grx_color_info_alloc_ega_colors:
 *
 * Returns: (array fixed-size=16): array of color indexes
 */
GrxColor *grx_color_info_alloc_ega_colors(void); /* shared, read-only standard EGA colors */

void     grx_color_info_set_color(GrxColor c, gint r, gint g, gint b);
void     grx_color_info_free_color(GrxColor c);
void     grx_color_info_free_cell(GrxColor c);

void     grx_color_info_query_color(GrxColor c, gint *r, gint *g, gint *b);
void     grx_color_info_query_color_id(GrxColor c, gint *r, gint *g, gint *b);
void     grx_color_info_query_color2(GrxColor c, glong *hcolor);
void     grx_color_info_query_color2_id(GrxColor c, glong *hcolor);

gint     grx_color_info_get_save_buffer_size(void);
void     grx_color_info_save_colors(gpointer buffer);
void     grx_color_info_restore_colors(gpointer buffer);

#ifndef GRX_SKIP_INLINES
#define grx_color_get_value(c)         ((GrxColor)(c) & GRX_COLOR_VALUE_MASK)
#define grx_color_get_mode(c)          ((GrxColor)(c) & GRX_COLOR_MODE_MASK)
#define grx_color_to_write_mode(c)     (grx_color_get_value(c) | GRX_COLOR_MODE_WRITE)
#define grx_color_to_xor_mode(c)       (grx_color_get_value(c) | GRX_COLOR_MODE_XOR)
#define grx_color_to_or_mode(c)        (grx_color_get_value(c) | GRX_COLOR_MODE_OR)
#define grx_color_to_and_mode(c)       (grx_color_get_value(c) | GRX_COLOR_MODE_AND)
#define grx_color_to_image_mode(c)     (grx_color_get_value(c) | GRX_COLOR_MODE_IMAGE)
#define grx_color_info_n_colors()      (GrColorInfo->ncolors)
#define grx_color_info_n_free_colors() (GrColorInfo->nfree)
#define grx_color_info_get_black() (                                           \
    (GrColorInfo->black == GRX_COLOR_NONE) ?                                   \
    (grx_color_info_get_black)() :                                             \
    GrColorInfo->black                                                         \
)
#define grx_color_info_get_white() (                                           \
    (GrColorInfo->white == GRX_COLOR_NONE) ?                                   \
    (grx_color_info_get_white)() :                                             \
    GrColorInfo->white                                                         \
)
#define grx_color_info_build_rgb_color_t(r,g,b) ((                             \
    ((GrxColor)((int)(r) & GrColorInfo->mask[0]) << GrColorInfo->shift[0]) |   \
    ((GrxColor)((int)(g) & GrColorInfo->mask[1]) << GrColorInfo->shift[1]) |   \
    ((GrxColor)((int)(b) & GrColorInfo->mask[2]) << GrColorInfo->shift[2])     \
    ) >> GrColorInfo->norm                                                     \
)
#define grx_color_info_build_rgb_color_r(r,g,b) grx_color_info_build_rgb_color_t( \
    (((unsigned int)(r)) > GrColorInfo->mask[0]) ? 255 : (unsigned int)(r) + GrColorInfo->round[0], \
    (((unsigned int)(g)) > GrColorInfo->mask[1]) ? 255 : (unsigned int)(g) + GrColorInfo->round[1], \
    (((unsigned int)(b)) > GrColorInfo->mask[2]) ? 255 : (unsigned int)(b) + GrColorInfo->round[2]  \
)
#define grx_color_info_get_red_value(c) (                                      \
    (int)(((GrxColor)(c) << GrColorInfo->norm) >> GrColorInfo->shift[0]) &     \
    (GrColorInfo->mask[0])                                                     \
)
#define grx_color_info_get_green_value(c) (                                    \
    (int)(((GrxColor)(c) << GrColorInfo->norm) >> GrColorInfo->shift[1]) &     \
    (GrColorInfo->mask[1])                                                     \
)
#define grx_color_info_get_blue_value(c) (                                     \
    (int)(((GrxColor)(c) << GrColorInfo->norm) >> GrColorInfo->shift[2]) &     \
    (GrColorInfo->mask[2])                                                     \
)
#define grx_color_info_alloc_color_id(r,g,b) (GrColorInfo->RGBmode ?           \
    grx_color_info_build_rgb_color_r(r,g,b) :                                  \
    grx_color_info_alloc_color(r,g,b)                                          \
)
#define grx_color_info_alloc_color2(hcolor) (grx_color_info_alloc_color(       \
    ((hcolor & 0xff0000) >> 16),                                               \
    ((hcolor & 0x00ff00) >> 8),                                                \
    (hcolor & 0x0000ff))                                                       \
)
#define grx_color_info_alloc_color2_id(hcolor) (grx_color_info_alloc_color_id( \
    ((hcolor & 0xff0000) >> 16),                                               \
    ((hcolor & 0x00ff00) >> 8),                                                \
    (hcolor & 0x0000ff))                                                       \
)
#define grx_color_info_query_color_id(c,r,g,b) do {                            \
    if(GrColorInfo->RGBmode) {                                                 \
    *(r) = grx_color_info_get_red_value(c);                                    \
    *(g) = grx_color_info_get_green_value(c);                                  \
    *(b) = grx_color_info_get_blue_value(c);                                   \
    break;                                                                     \
    }                                                                          \
    if(((GrxColor)(c) < GrColorInfo->ncolors) &&                               \
       (GrColorInfo->ctable[(GrxColor)(c)].defined)) {                         \
    *(r) = GrColorInfo->ctable[(GrxColor)(c)].r;                               \
    *(g) = GrColorInfo->ctable[(GrxColor)(c)].g;                               \
    *(b) = GrColorInfo->ctable[(GrxColor)(c)].b;                               \
    break;                                                                     \
    }                                                                          \
    *(r) = *(g) = *(b) = 0;                                                    \
} while(0)
#define grx_color_info_query_color2_id(c,hcolor) do {                          \
    if(GrColorInfo->RGBmode) {                                                 \
    *(hcolor) = grx_color_info_get_red_value(c) << 16;                         \
    *(hcolor) |= grx_color_info_get_green_value(c) << 8;                       \
    *(hcolor) |= grx_color_info_get_blue_value(c);                             \
    break;                                                                     \
    }                                                                          \
    if(((GrxColor)(c) < GrColorInfo->ncolors) &&                               \
       (GrColorInfo->ctable[(GrxColor)(c)].defined)) {                         \
    *(hcolor) = GrColorInfo->ctable[(GrxColor)(c)].r;                          \
    *(hcolor) = GrColorInfo->ctable[(GrxColor)(c)].g;                          \
    *(hcolor) = GrColorInfo->ctable[(GrxColor)(c)].b;                          \
    break;                                                                     \
    }                                                                          \
    *(hcolor) = 0;                                                             \
} while(0)
#endif  /* GRX_SKIP_INLINES */

/*
 * color table (for primitives using several colors):
 *   it is an array of colors with the first element being
 *   the number of colors in the table
 */
typedef GrxColor *GrxColorTable;

#define GRX_COLOR_TABLE_GET_SIZE(table) (                                      \
        (table) ? (unsigned int)((table)[0]) : 0U                              \
)
#define GRX_COLOR_TABLE_GET_COLOR(table,index) (                               \
        ((unsigned)(index) < GRX_COLOR_TABLE_GET_SIZE(table)) ?                \
        (table)[((unsigned)(index)) + 1] :                                     \
        GRX_COLOR_NONE                                                         \
)
#define GRX_COLOR_TABLE_GET_ALLOC_SIZE(ncolors)    ((ncolors) + 1)

/* ================================================================== */
/*                       GRAPHICS PRIMITIVES                          */
/* ================================================================== */

#define GRX_MAX_POLYGON_POINTS   (1000000)
#define GRX_MAX_ELLIPSE_POINTS   (1024 + 5)
#define GRX_MAX_ANGLE_VALUE      (3600)

typedef enum {
    GRX_ARC_STYLE_OPEN       = 0,
    GRX_ARC_STYLE_CLOSE1     = 1,
    GRX_ARC_STYLE_CLOSE2     = 2,
} GrxArcStyle;

typedef struct {                        /* framed box colors */
    GrxColor background;
    GrxColor border_top;
    GrxColor border_right;
    GrxColor border_bottom;
    GrxColor border_left;
} GrxFramedBoxColors;

void grx_clear_screen(GrxColor bg);
void grx_clear_context(GrxColor bg);
void grx_context_clear(GrxContext *ctx, GrxColor bg);
void grx_clear_clip_box(GrxColor bg);
void grx_draw_point(gint x, gint y, GrxColor c);
void grx_draw_line(gint x1, gint y1, gint x2, gint y2, GrxColor c);
void grx_draw_hline(gint x1, gint x2, gint y, GrxColor c);
void grx_draw_vline(gint x, gint y1, gint y2,GrxColor c);
void grx_draw_box(gint x1, gint y1, gint x2, gint y2, GrxColor c);
void grx_draw_filled_box(gint x1, gint y1, gint x2, gint y2, GrxColor c);
void grx_draw_framed_box(gint x1, gint y1, gint x2, gint y2, gint wdt, const GrxFramedBoxColors *c);
gboolean grx_generate_ellipse(gint xc, gint yc, gint xa, gint ya, gint points[GRX_MAX_ELLIPSE_POINTS][2]);
gboolean grx_generate_ellipse_arc(gint xc, gint yc, gint xa, gint ya, gint start, gint end, gint points[GRX_MAX_ELLIPSE_POINTS][2]);
void grx_get_last_arc_coordinates(gint *xs, gint *ys, gint *xe, gint *ye, gint *xc, gint *yc);
void grx_draw_circle(gint xc, gint yc, gint r, GrxColor c);
void grx_draw_ellipse(gint xc, gint yc, gint xa, gint ya, GrxColor c);
void grx_draw_circle_arc(gint xc, gint yc, gint r, gint start, gint end, GrxArcStyle style, GrxColor c);
void grx_draw_ellipse_arc(gint xc, gint yc, gint xa, gint ya, gint start, gint end,GrxArcStyle style,GrxColor c);
void grx_draw_filled_circle(gint xc, gint yc, gint r, GrxColor c);
void grx_draw_filled_ellipse(gint xc, gint yc, gint xa, gint ya, GrxColor c);
void grx_draw_filled_circle_arc(gint xc, gint yc, gint r, gint start, gint end, GrxArcStyle style, GrxColor c);
void grx_draw_filled_ellipse_arc(gint xc, gint yc, gint xa, gint ya, gint start, gint end, GrxArcStyle style, GrxColor c);
void grx_draw_polyline(gint numpts, gint points[][2], GrxColor c);
void grx_draw_polygon(gint numpts, gint points[][2], GrxColor c);
void grx_draw_filled_convex_polygon(gint numpts, gint points[][2], GrxColor c);
void grx_draw_filled_polygon(gint numpts, gint points[][2], GrxColor c);
void grx_bit_blt(GrxContext *dst, gint x, gint y, GrxContext *src, gint x1, gint y1, gint x2, gint y2, GrxColor op);
void grx_bit_blt_1bpp(GrxContext *dst, gint dx, gint dy, GrxContext *src, gint x1, gint y1, gint x2, gint y2, GrxColor fg, GrxColor bg);
void grx_flood_fill(gint x, gint y, GrxColor border, GrxColor c);
void grx_flood_spill(gint x1, gint y1, gint x2, gint y2, GrxColor old_c, GrxColor new_c);
void grx_flood_spill2(gint x1, gint y1, gint x2, gint y2, GrxColor old_c1, GrxColor new_c1, GrxColor old_c2, GrxColor new_c2);
void grx_context_flood_spill(GrxContext *ctx, gint x1, gint y1, gint x2, gint y2, GrxColor old_c, GrxColor new_c);
void grx_context_flood_spill2(GrxContext *ctx, gint x1, gint y1, gint x2, gint y2, GrxColor old_c1, GrxColor new_c1, GrxColor old_c2, GrxColor new_c2);

GrxColor grx_get_pixel_at(gint x, gint y);
GrxColor grx_context_get_pixel_at(GrxContext *c, gint x, gint y);

const GrxColor *grx_get_scanline(int x1,int x2,int yy);
const GrxColor *grx_context_get_scanline(GrxContext *ctx,int x1,int x2,int yy);
/* Input   ctx: source context, if NULL the current context is used */
/*         x1 : first x coordinate read                             */
/*         x2 : last  x coordinate read                             */
/*         yy : y coordinate                                        */
/* Output  NULL     : error / no data (clipping occured)            */
/*         else                                                     */
/*           p[0..w]: pixel values read                             */
/*                      (w = |x2-y1|)                               */
/*           Output data is valid until next GRX call !             */

void grx_put_scanline(int x1,int x2,int yy,const GrxColor *c, GrxColor op);
/* Input   x1 : first x coordinate to be set                        */
/*         x2 : last  x coordinate to be set                        */
/*         yy : y coordinate                                        */
/*         c  : c[0..(|x2-x1|] hold the pixel data                  */
/*         op : Operation (GRX_COLOR_MODE_WRITE/GRX_COLOR_MODE_XOR/GRX_COLOR_MODE_OR/GRX_COLOR_MODE_AND/GRX_COLOR_MODE_IMAGE)        */
/*                                                                  */
/* Note    c[..] data must fit GRX_COLOR_VALUE_MASK otherwise the results   */
/*         are implementation dependend.                            */
/*         => You can't supply operation code with the pixel data!  */


#ifndef GRX_SKIP_INLINES
#define grx_get_scanline(x1,x2,yy) \
        grx_context_get_scanline(NULL,(x1),(x2),(yy))
#endif

/* ================================================================== */
/*                 NON CLIPPING DRAWING PRIMITIVES                    */
/* ================================================================== */

void grx_draw_point_nc(gint x, gint y, GrxColor c);
void grx_draw_line_nc(gint x1, gint y1, gint x2, gint y2,GrxColor c);
void grx_draw_hline_nc(gint x1, gint x2, gint y, GrxColor c);
void grx_draw_vline_nc(gint x, gint y1, gint y2, GrxColor c);
void grx_draw_box_nc(gint x1, gint y1, gint x2, gint y2, GrxColor c);
void grx_draw_filled_box_nc(gint x1, gint y1, gint x2, gint y2, GrxColor c);
void grx_draw_framed_box_nc(gint x1, gint y1, gint x2, gint y2, gint wdt,
                            const GrxFramedBoxColors *c);
void grx_bit_blt_nc(GrxContext *dst, gint x, gint y, GrxContext *src, gint x1,
                    gint y1, gint x2, gint y2, GrxColor op);

GrxColor grx_get_pixel_nc(gint x, gint y);
GrxColor grx_context_get_pixel_nc(GrxContext *c, gint x, gint y);

#ifndef GRX_SKIP_INLINES
#define grx_draw_point_nc(x,y,c) (                                             \
        (*grx_get_current_frame_driver()->drawpixel)(                          \
        ((x) + grx_context_get_current()->x_offset),                           \
        ((y) + grx_context_get_current()->y_offset),                           \
        ((c))                                                                  \
        )                                                                      \
)
#define grx_get_pixel_nc(x,y) (                                                \
        (*grx_get_current_frame_driver()->readpixel)(                          \
        (GrxFrame *)(&grx_context_get_current()->frame),                       \
        ((x) + grx_context_get_current()->x_offset),                           \
        ((y) + grx_context_get_current()->y_offset)                            \
        )                                                                      \
)
#define grx_context_get_pixel_nc(c,x,y) (                                      \
        (*(c)->gc_driver->readpixel)(                                          \
        (&(c)->frame),                                                         \
        ((x) + (c)->x_offset),                                                 \
        ((y) + (c)->y_offset)                                                  \
        )                                                                      \
)
#endif  /* GRX_SKIP_INLINES */

/* ================================================================== */
/*                   FONTS AND TEXT PRIMITIVES                        */
/* ================================================================== */

/**
 * GrxTextDirection:
 * @GRX_TEXT_DIRECTION_RIGHT: normal
 * @GRX_TEXT_DIRECTION_DOWN: downward
 * @GRX_TEXT_DIRECTION_LEFT: upsidedown, right to left
 * @GRX_TEXT_DIRECTION_UP: upward
 *
 * text drawing directions
 */
typedef enum {
    GRX_TEXT_DIRECTION_RIGHT,
    GRX_TEXT_DIRECTION_DOWN,
    GRX_TEXT_DIRECTION_LEFT,
    GRX_TEXT_DIRECTION_UP,
    GRX_TEXT_DIRECTION_DEFAULT = GRX_TEXT_DIRECTION_RIGHT
} GrxTextDirection;

#define GRX_TEXT_DIRECTION_IS_VERTICAL(d)  ((d) & 1)

/*
 * text alignment options
 */
typedef enum {
    GRX_TEXT_ALIGN_LEFT,
    GRX_TEXT_ALIGN_CENTER,
    GRX_TEXT_ALIGN_RIGHT,
} GrxTextAlignment;

typedef enum {
    GRX_TEXT_VALIGN_TOP,
    GRX_TEXT_VALIGN_MIDDLE,
    GRX_TEXT_VALIGN_BOTTOM,
    GRX_TEXT_VALIGN_BASELINE,
} GrxTextVerticalAlignment;

/**
 * GrxCharType:
 * @GRX_CHAR_TYPE_BYTE: one byte per character
 * @GRX_CHAR_TYPE_WORD: two bytes per character
 * @GRX_CHAR_TYPE_ATTR: chr w/ PC style attribute byte
 *
 * character types in text strings
 */
typedef enum {
    GRX_CHAR_TYPE_BYTE = 0,
    GRX_CHAR_TYPE_WORD = 1,
    GRX_CHAR_TYPE_ATTR = 2,
} GrxCharType;

/*
 * macros to access components of various string/character types
 */
#define GRX_CHAR_TYPE_GET_SIZE(ty)     (((ty) == GRX_CHAR_TYPE_BYTE) ? sizeof(char) : sizeof(short))
#define GRX_CHAR_TYPE_GET_CODE(ty,ch)  (((ty) == GRX_CHAR_TYPE_WORD) ? (unsigned short)(ch) : (unsigned char)(ch))
#define GRX_CHAR_TYPE_GET_ATTR(ty,ch)  (((ty) == GRX_CHAR_TYPE_ATTR) ? ((unsigned short)(ch) >> 8) : 0)
#define GRX_CHAR_TYPE_GET_CODE_STR(ty,pt)  (((ty) == GRX_CHAR_TYPE_WORD) ? ((unsigned short *)(pt))[0] : ((unsigned char *)(pt))[0])
#define GRX_CHAR_TYPE_GET_ATTR_STR(ty,pt)  (((ty) == GRX_CHAR_TYPE_ATTR) ? ((unsigned char *)(pt))[1] : 0)

/*
 * text attribute macros for the GRX_CHAR_TYPE_ATTR type
 * _GR_textattrintensevideo drives if the eighth bit is used for
 * underline (false, default) or more background colors (true)
 */
extern int _GR_textattrintensevideo;

#define GR_BUILD_ATTR(fg,bg,ul) (_GR_textattrintensevideo ? \
                                (((fg) & 15) | (((bg) & 15) << 4)) \
                                : \
                                (((fg) & 15) | (((bg) & 7) << 4) | ((ul) ? 128 : 0)) \
                                )
#define GR_ATTR_FGCOLOR(attr)   (((attr)     ) &  15)
#define GR_ATTR_BGCOLOR(attr)   (_GR_textattrintensevideo ? \
                                (((attr) >> 4) &  15) \
                                : \
                                (((attr) >> 4) &   7) \
                                )
#define GR_ATTR_UNDERLINE(attr) (_GR_textattrintensevideo ? \
                                (0) \
                                : \
                                (((attr)     ) & 128) \
                                )

/*
 * OR this to the foreground color value for underlined text when
 * using GRX_CHAR_TYPE_BYTE or GRX_CHAR_TYPE_WORD modes.
 */
#define GR_UNDERLINE_TEXT       (GRX_COLOR_MODE_XOR << 4)

/**
 * GrxFontConversionFlags:
 * @GRX_FONT_CONV_FLAG_NONE: no conversion
 * @GRX_FONT_CONV_FLAG_SKIP_CHARS: load only selected characters
 * @GRX_FONT_CONV_FLAG_RESIZE: resize the font
 * @GRX_FONT_CONV_FLAG_ITALICIZE: tilt font for "italic" look
 * @GRX_FONT_CONV_FLAG_BOLDIFY: make a "bold"(er) font
 * @GRX_FONT_CONV_FLAG_FIXIFY: convert proportional font to fixed width
 * @GRX_FONT_CONV_FLAG_PROPORTION: convert fixed font to proportional width
 *
 * Font conversion flags for 'grx_font_load_converted'. OR them as desired.
 */
typedef enum /*< flags >*/ {
    GRX_FONT_CONV_FLAG_NONE         = 0,
    GRX_FONT_CONV_FLAG_SKIP_CHARS   = 1,
    GRX_FONT_CONV_FLAG_RESIZE       = 2,
    GRX_FONT_CONV_FLAG_ITALICIZE    = 4,
    GRX_FONT_CONV_FLAG_BOLDIFY      = 8,
    GRX_FONT_CONV_FLAG_FIXIFY       = 16,
    GRX_FONT_CONV_FLAG_PROPORTION   = 32,
} GrxFontConversionFlags;

/*
 * font structures
 */
typedef struct {                    /* font descriptor */
    gchar    *name;                 /* font name */
    gchar    *family;               /* font family name */
    gboolean  proportional;         /* characters have varying width */
    gboolean  scalable;             /* derived from a scalable font */
    gboolean  preloaded;            /* set when linked into program */
    gboolean  modified;             /* "tweaked" font (resized, etc..) */
    guint     width;                /* width (proportional=>average) */
    guint     height;               /* font height */
    guint     baseline;             /* baseline pixel pos (from top) */
    guint     ulpos;                /* underline pixel pos (from top) */
    guint     ulheight;             /* underline width */
    guint     minchar;              /* lowest character code in font */
    guint     numchars;             /* number of characters in font */
} GrxFontHeader;

typedef struct {                    /* character descriptor */
    guint     width;                /* width of this character */
    guint     offset;               /* offset from start of bitmap */
} GrxFontCharInfo;

typedef struct {                    /* the complete font */
    GrxFontHeader h;                /* the font info structure */
    guint8   *bitmap;               /* character bitmap array */
    guint8   *auxmap;               /* map for rotated & underline chrs */
    guint     minwidth;             /* width of narrowest character */
    guint     maxwidth;             /* width of widest character */
    guint     auxsize;              /* allocated size of auxiliary map */
    guint     auxnext;              /* next free byte in auxiliary map */
    guint    *auxoffs[7];           /* offsets to completed aux chars */
    GrxFontCharInfo chrinfo[1];     /* character info (not act. size) */
} GrxFont;

extern  GrxFont            grx_font_pc6x8;
extern  GrxFont            grx_font_pc8x8;
extern  GrxFont            grx_font_pc8x14;
extern  GrxFont            grx_font_pc8x16;
#define grx_font_default   grx_font_pc8x14

GrxFont *grx_font_load(gchar *name);
GrxFont *grx_font_load_converted(gchar *name, GrxFontConversionFlags cvt,
                                 gint w, gint h, gint minch, gint maxch);
GrxFont *grx_font_build_converted(const GrxFont *from, GrxFontConversionFlags cvt,
                                  gint w, gint h, gint minch, gint maxch);

void grx_font_unload(GrxFont *font);
void grx_font_dump(const GrxFont *f,char *CsymbolName,char *fileName);
void grx_font_dump_fna(const GrxFont *f, char *fileName);
void grx_font_set_path(char *path_list);

gboolean grx_font_is_char_present(const GrxFont *font, gint chr);
gint grx_font_get_char_width(const GrxFont *font, gint chr);
gint grx_font_get_char_height(const GrxFont *font, gint chr);
gint grx_font_get_char_bmp_row_size(const GrxFont *font, gint chr);
gint grx_font_get_char_bmp_size(const GrxFont *font, gint chr);
gint grx_font_get_string_width(const GrxFont *font, gconstpointer text, gint len, GrxCharType type);
gint grx_font_get_string_height(const GrxFont *font, gconstpointer text, gint len, GrxCharType type);
gint grx_font_get_proportional_text_width(const GrxFont *font, gconstpointer text, gint len, GrxCharType type);

guint8 *grx_font_build_aux_bmp(GrxFont *font, gint chr, GrxTextDirection dir, gint ul);
guint8 *grx_font_get_char_bmp(const GrxFont *font, gint chr);
guint8 *grx_font_get_char_aux_bmp(GrxFont *font, gint chr, GrxTextDirection dir, gint ul);

typedef union {           /* text color union */
    GrxColor      v;                    /* color value for "direct" text */
    GrxColorTable p;                    /* color table for attribute text */
} GrxTextColor;

typedef struct {                          /* text drawing option structure */
    GrxFont                 *txo_font;    /* font to be used */
    GrxTextColor             txo_fgcolor; /* foreground color */
    GrxTextColor             txo_bgcolor; /* background color */
    GrxCharType              txo_chrtype; /* character type */
    GrxTextDirection         txo_direct;  /* direction */
    GrxTextAlignment         txo_xalign;  /* X alignment */
    GrxTextVerticalAlignment txo_yalign;  /* Y alignment */
} GrxTextOption;

typedef struct {                        /* fixed font text window desc. */
    GrxFont     *txr_font;              /* font to be used */
    GrxTextColor txr_fgcolor;           /* foreground color */
    GrxTextColor txr_bgcolor;           /* background color */
    gpointer     txr_buffer;            /* pointer to text buffer */
    gpointer     txr_backup;            /* optional backup buffer */
    gint         txr_width;             /* width of area in chars */
    gint         txr_height;            /* height of area in chars */
    gint         txr_lineoffset;        /* offset in buffer(s) between rows */
    gint         txr_xpos;              /* upper left corner X coordinate */
    gint         txr_ypos;              /* upper left corner Y coordinate */
    GrxCharType  txr_chrtype;           /* character type */
} GrxTextRegion;

gint grx_text_option_get_char_width(const GrxTextOption *opt, gint chr);
gint grx_text_option_get_char_height(const GrxTextOption *opt, gint chr);
void grx_text_option_get_char_size(const GrxTextOption *opt, gint chr, gint *w, gint *h);
gint grx_text_option_get_string_width(const GrxTextOption *opt, gpointer text, gint length);
gint grx_text_option_get_string_height(const GrxTextOption *opt, gpointer text, gint length);
void grx_text_option_get_string_size(const GrxTextOption *opt, gpointer text, gint length, gint *w, gint *h);

void grx_draw_char_with_text_options(gint chr, gint x, gint y, const GrxTextOption *opt);
void grx_draw_string_with_text_options(gpointer text, gint length, gint x, gint y, const GrxTextOption *opt);
void grx_draw_text_xy(gint x, gint y, gchar *text, GrxColor fg, GrxColor bg);

void grx_text_region_dump_char(const GrxTextRegion *r, gint chr, gint col, gint row);
void grx_text_region_dump_text(const GrxTextRegion *r, gint col, gint row, gint width, gint height);
void grx_text_region_dump(const GrxTextRegion *r);

#ifndef GRX_SKIP_INLINES
#define grx_font_is_char_present(f,ch) (                                       \
        ((unsigned int)(ch) - (f)->h.minchar) < (f)->h.numchars                \
)
#define grx_font_get_char_width(f,ch) (                                        \
        grx_font_is_char_present(f,ch) ?                                       \
        (int)(f)->chrinfo[(unsigned int)(ch) - (f)->h.minchar].width :         \
        (f)->h.width                                                           \
)
#define grx_font_get_char_height(f,ch) (                                       \
        (f)->h.height                                                          \
)
#define grx_font_get_char_bmp_row_size(f,ch) (                                 \
        grx_font_is_char_present(f,ch) ?                                       \
        (((f)->chrinfo[(unsigned int)(ch) - (f)->h.minchar].width + 7) >> 3) : \
        0                                                                      \
)
#define grx_font_get_char_bmp_size(f,ch) (                                     \
        grx_font_get_char_bmp_row_size(f,ch) * (f)->h.height                   \
)
#define grx_font_get_string_width(f,t,l,tp) (                                  \
        (f)->h.proportional ?                                                  \
        grx_font_get_proportional_text_width((f),(t),(l),(tp)) :               \
        (f)->h.width * (l)                                                     \
)
#define grx_font_get_string_height(f,t,l,tp) (                                 \
        (f)->h.height                                                          \
)
#define grx_font_get_char_bmp(f,ch) (                                          \
        grx_font_is_char_present(f,ch) ?                                       \
        &(f)->bitmap[(f)->chrinfo[(unsigned int)(ch) - (f)->h.minchar].offset]:\
        (guint8 *)0                                                            \
)
#define grx_font_get_char_aux_bmp(f,ch,dir,ul) (                               \
        (((dir) == GRX_TEXT_DIRECTION_DEFAULT) && !(ul)) ?                     \
        grx_font_get_char_bmp(f,ch) :                                          \
        grx_font_build_aux_bmp((f),(ch),(dir),(ul))                            \
)
#define grx_text_option_get_char_width(o,c) (                                  \
        GRX_TEXT_DIRECTION_IS_VERTICAL((o)->txo_direct) ?                      \
        grx_font_get_char_height((o)->txo_font,GRX_CHAR_TYPE_GET_CODE((o)->txo_chrtype,c)) :  \
        grx_font_get_char_width( (o)->txo_font,GRX_CHAR_TYPE_GET_CODE((o)->txo_chrtype,c))    \
)
#define grx_text_option_get_char_height(o,c) (                                 \
        GRX_TEXT_DIRECTION_IS_VERTICAL((o)->txo_direct) ?                      \
        grx_font_get_char_width( (o)->txo_font,GRX_CHAR_TYPE_GET_CODE((o)->txo_chrtype,c)) :  \
        grx_font_get_char_height((o)->txo_font,GRX_CHAR_TYPE_GET_CODE((o)->txo_chrtype,c))    \
)
#define grx_text_option_get_char_size(o,c,wp,hp) do {                          \
        *(wp) = grx_text_option_get_char_height(o,c);                          \
        *(hp) = grx_text_option_get_char_width(o,c);                           \
} while(0)
#define grx_text_option_get_string_width(o,t,l) (                              \
        GRX_TEXT_DIRECTION_IS_VERTICAL((o)->txo_direct) ?                      \
        grx_font_get_string_height((o)->txo_font,(t),(l),(o)->txo_chrtype) :   \
        grx_font_get_string_width( (o)->txo_font,(t),(l),(o)->txo_chrtype)     \
)
#define grx_text_option_get_string_height(o,t,l) (                             \
        GRX_TEXT_DIRECTION_IS_VERTICAL((o)->txo_direct) ?                      \
        grx_font_get_string_width( (o)->txo_font,(t),(l),(o)->txo_chrtype) :   \
        grx_font_get_string_height((o)->txo_font,(t),(l),(o)->txo_chrtype)     \
)
#define grx_text_option_get_string_size(o,t,l,wp,hp) do {                      \
        *(wp) = grx_text_option_get_string_width( o,t,l);                      \
        *(hp) = grx_text_option_get_string_height(o,t,l);                      \
} while(0)
#endif /* GRX_SKIP_INLINES */

/* ================================================================== */
/*            THICK AND DASHED LINE DRAWING PRIMITIVES                */
/* ================================================================== */

/*
 * custom line options structure
 *   zero or one dash pattern length means the line is continuous
 *   the dash pattern always begins with a drawn section
 */
typedef struct {
    GrxColor color;                   /* color used to draw line */
    gint     width;                   /* width of the line */
    gint     n_dash_patterns;         /* length of the dash pattern */
    guint8  *dash_patterns;           /* draw/nodraw pattern */
} GrxLineOptions;

void grx_draw_line_with_options(gint x1, gint y1, gint x2, gint y2, const GrxLineOptions *o);
void grx_draw_box_with_options(gint x1, gint y1, gint x2, gint y2, const GrxLineOptions *o);
void grx_draw_circle_with_options(gint xc, gint yc, gint r, const GrxLineOptions *o);
void grx_draw_ellipse_with_options(gint xc, gint yc, gint xa, gint ya, const GrxLineOptions *o);
void grx_draw_circle_arc_with_options(gint xc, gint yc, gint r, gint start, gint end,
                                      GrxArcStyle style, const GrxLineOptions *o);
void grx_draw_ellipse_arc_with_options(gint xc, gint yc, gint xa, gint ya, gint start, gint end,
                                       GrxArcStyle style, const GrxLineOptions *o);
void grx_draw_polyline_with_options(gint numpts, gint points[][2], const GrxLineOptions *o);
void grx_draw_polygon_with_options(gint numpts, gint points[][2], const GrxLineOptions *o);

/* ================================================================== */
/*             PATTERNED DRAWING AND FILLING PRIMITIVES               */
/* ================================================================== */

/**
 * GrxBitmap:
 *
 * A mode independent way to specify a fill pattern of two
 * colors. It is always 8 pixels wide (1 byte per scan line), its
 * height is user-defined. SET is_pixmap TO FALSE!!!
 */
typedef struct {
    gboolean is_pixmap;               /* type flag for pattern union */
    gint     height;                  /* bitmap height */
    guint8  *data;                    /* pointer to the bit pattern */
    GrxColor fg_color;                /* foreground color for fill */
    GrxColor bg_color;                /* background color for fill */
    gboolean free_on_pattern_destroy; /* set if dynamically allocated */
} GrxBitmap;

/**
 * GrxPixmap:
 *
 * A fill pattern stored in a layout identical to the video RAM
 * for filling using 'bitblt'-s. It is mode dependent, typically one
 * of the library functions is used to build it. SET is_pixmap TO TRUE!!!
 */
typedef struct {
    gboolean     is_pixmap;      /* type flag for pattern union */
    gint         width;          /* pixmap width (in pixels)  */
    gint         height;         /* pixmap height (in pixels) */
    GrxColorMode mode;           /* bitblt mode (SET, OR, XOR, AND, IMAGE) */
    GrxFrame     source;         /* source context for fill */
} GrxPixmap;

/**
 * GrxPattern:
 *
 * Fill pattern union -- can either be a bitmap or a pixmap
 */
typedef union {
    gboolean  is_pixmap;              /* nonzero for pixmaps */
    GrxBitmap bitmap;                 /* fill bitmap */
    GrxPixmap pixmap;                 /* fill pixmap */
    #define gp_bmp_data               bitmap.data
    #define gp_bmp_height             bitmap.height
    #define gp_bmp_fgcolor            bitmap.fg_color
    #define gp_bmp_bgcolor            bitmap.bg_color
    #define gp_pxp_width              pixmap.width
    #define gp_pxp_height             pixmap.height
    #define gp_pxp_oper               pixmap.mode
    #define gp_pxp_source             pixmap.source
} GrxPattern;


/**
 * GrxLinePattern:
 *
 * Draw pattern for line drawings -- specifies both the:
 *   (1) fill pattern, and the
 *   (2) custom line drawing option
 */
typedef struct {
    GrxPattern     *pattern;         /* fill pattern */
    GrxLineOptions *options;         /* width + dash pattern */
} GrxLinePattern;

GrxPattern *grx_pattern_create_pixmap(const guint8 *pixels, gint w, gint h, const GrxColorTable colors);
GrxPattern *grx_pattern_create_pixmap_from_bits(const guint8 *bits, gint w, gint h, GrxColor fgc, GrxColor bgc);
GrxPattern *grx_pattern_create_pixmap_from_context(GrxContext *src);

void grx_pattern_free(GrxPattern *p);

void grx_draw_line_with_pattern(gint x1, gint y1, gint x2, gint y2, GrxLinePattern *lp);
void grx_draw_box_with_pattern(gint x1, gint y1, gint x2, gint y2, GrxLinePattern *lp);
void grx_draw_circle_with_pattern(gint xc, gint yc, gint r, GrxLinePattern *lp);
void grx_draw_ellipse_with_pattern(gint xc, gint yc, gint xa, gint ya, GrxLinePattern *lp);
void grx_draw_circle_arc_with_pattern(gint xc, gint yc, gint r, gint start, gint end,
                                      GrxArcStyle style, GrxLinePattern *lp);
void grx_draw_ellipse_arc_with_pattern(gint xc, gint yc, gint xa, gint ya, gint start, gint end,
                                       GrxArcStyle style, GrxLinePattern *lp);
void grx_draw_polyline_with_pattern(gint numpts, gint points[][2], GrxLinePattern *lp);
void grx_draw_polygon_with_pattern(gint numpts, gint points[][2], GrxLinePattern *lp);

void grx_draw_filled_point_with_pattern(gint x, gint y, GrxPattern *p);
void grx_draw_filled_line_with_pattern(gint x1, gint y1, gint x2, gint y2, GrxPattern *p);
void grx_draw_filled_box_with_pattern(gint x1, gint y1, gint x2, gint y2,GrxPattern *p);
void grx_draw_filled_circle_with_pattern(gint xc, gint yc, gint r,GrxPattern *p);
void grx_draw_filled_ellipse_with_pattern(gint xc, gint yc, gint xa, gint ya, GrxPattern *p);
void grx_draw_filled_circle_arc_with_pattern(gint xc, gint yc, gint r, gint start, gint end,
                                             GrxArcStyle style, GrxPattern *p);
void grx_draw_filled_ellipse_arc_with_pattern(gint xc, gint yc, gint xa, gint ya, gint start, gint end,
                                              GrxArcStyle style,GrxPattern *p);
void grx_draw_filled_convex_polygon_with_pattern(gint numpts, gint points[][2], GrxPattern *p);
void grx_draw_filled_polygon_with_pattern(gint numpts, gint points[][2], GrxPattern *p);
void grx_flood_fill_with_pattern(gint x, gint y, GrxColor border, GrxPattern *p);

void grx_draw_char_with_pattern(gint chr, gint x, gint y, const GrxTextOption *opt, GrxPattern *p);
void grx_draw_string_with_pattern(gpointer text, gint length, gint x, gint y,
                                  const GrxTextOption *opt, GrxPattern *p);
void grx_draw_string_with_pattern_ext(gpointer text, gint length, gint x, gint y,
                                      const GrxTextOption *opt, GrxPattern *p);

/* ================================================================== */
/*                      IMAGE MANIPULATION                            */
/* ================================================================== */

/*
 *  by Michal Stencl Copyright (c) 1998 for GRX
 *  <e-mail>    - [stenclpmd@ba.telecom.sk]
 */

#ifndef GrxImage
#define GrxImage GrxPixmap
#endif

typedef enum /*< flags >*/ {
    GRX_IMAGE_MIRROR_HORIZONTAL = 0x01,  /* inverse left right */
    GRX_IMAGE_MIRROR_VERTICAL = 0x02,  /* inverse top down */
} GrxImageMirrorFlags;

GrxImage *grx_image_create(const guint8 *pixels, gint w, gint h, const GrxColorTable colors);
void grx_image_free(GrxImage *i);
void grx_draw_image(gint x, gint y, GrxImage *i);
void grx_draw_image_tiled(gint x1, gint y1, gint x2, gint y2, GrxImage *i);
void grx_draw_filled_box_with_image(gint xo, gint yo, gint x1, gint y1, gint x2, gint y2, GrxImage *p);
void grx_draw_hline_with_image(gint xo, gint yo, gint x, gint y, gint width, GrxImage *p);
void grx_draw_point_with_image(gint xo, gint yo, gint x, gint y, GrxImage *p);

GrxImage *grx_image_mirror(GrxImage *p, GrxImageMirrorFlags flags);
GrxImage *grx_image_stretch(GrxImage *p, gint nwidth, gint nheight);

GrxImage *grx_image_create_from_pattern(GrxPattern *p);
GrxImage *grx_image_create_from_context(GrxContext *c);
GrxImage *grx_pattern_create_from_data(const guint8 *pixels, gint w, gint h, const GrxColorTable colors);

GrxPattern *grx_pattern_create_from_image(GrxImage *p);


#ifndef GRX_SKIP_INLINES
#define grx_image_create_from_pattern(p) \
        (((p) && (p)->is_pixmap) ? (&(p)->pixmap) : NULL)
#define grx_image_create_from_context(c) \
        (GrxImage *)grx_pattern_create_pixmap_from_context(c)
#define grx_pattern_create_from_image(p) \
        (GrxPattern *)(p)
#define grx_pattern_create_from_data(pixels,w,h,colors) \
        (GrxImage *)grx_pattern_create_pixmap(pixels,w,h,colors);
#define grx_image_free(i)   \
        grx_pattern_free((GrxPattern *)(i));
#endif

/* ================================================================== */
/*               DRAWING IN USER WINDOW COORDINATES                   */
/* ================================================================== */

void GrSetUserWindow(int x1,int y1,int x2,int y2);
void GrGetUserWindow(int *x1,int *y1,int *x2,int *y2);
void GrGetScreenCoord(int *x,int *y);
void GrGetUserCoord(int *x,int *y);

void GrUsrPlot(int x,int y,GrxColor c);
void GrUsrLine(int x1,int y1,int x2,int y2,GrxColor c);
void GrUsrHLine(int x1,int x2,int y,GrxColor c);
void GrUsrVLine(int x,int y1,int y2,GrxColor c);
void GrUsrBox(int x1,int y1,int x2,int y2,GrxColor c);
void GrUsrFilledBox(int x1,int y1,int x2,int y2,GrxColor c);
void GrUsrFramedBox(int x1,int y1,int x2,int y2,int wdt,GrxFramedBoxColors *c);
void GrUsrCircle(int xc,int yc,int r,GrxColor c);
void GrUsrEllipse(int xc,int yc,int xa,int ya,GrxColor c);
void GrUsrCircleArc(int xc,int yc,int r,int start,int end,GrxArcStyle style,GrxColor c);
void GrUsrEllipseArc(int xc,int yc,int xa,int ya,int start,int end,GrxArcStyle style,GrxColor c);
void GrUsrFilledCircle(int xc,int yc,int r,GrxColor c);
void GrUsrFilledEllipse(int xc,int yc,int xa,int ya,GrxColor c);
void GrUsrFilledCircleArc(int xc,int yc,int r,int start,int end,GrxArcStyle style,GrxColor c);
void GrUsrFilledEllipseArc(int xc,int yc,int xa,int ya,int start,int end,GrxArcStyle style,GrxColor c);
void GrUsrPolyLine(int numpts,int points[][2],GrxColor c);
void GrUsrPolygon(int numpts,int points[][2],GrxColor c);
void GrUsrFilledConvexPolygon(int numpts,int points[][2],GrxColor c);
void GrUsrFilledPolygon(int numpts,int points[][2],GrxColor c);
void GrUsrFloodFill(int x, int y, GrxColor border, GrxColor c);

GrxColor GrUsrPixel(int x,int y);
GrxColor GrUsrPixelC(GrxContext *c,int x,int y);

void GrUsrCustomLine(int x1,int y1,int x2,int y2,const GrxLineOptions *o);
void GrUsrCustomBox(int x1,int y1,int x2,int y2,const GrxLineOptions *o);
void GrUsrCustomCircle(int xc,int yc,int r,const GrxLineOptions *o);
void GrUsrCustomEllipse(int xc,int yc,int xa,int ya,const GrxLineOptions *o);
void GrUsrCustomCircleArc(int xc,int yc,int r,int start,int end,GrxArcStyle style,const GrxLineOptions *o);
void GrUsrCustomEllipseArc(int xc,int yc,int xa,int ya,int start,int end,GrxArcStyle style,const GrxLineOptions *o);
void GrUsrCustomPolyLine(int numpts,int points[][2],const GrxLineOptions *o);
void GrUsrCustomPolygon(int numpts,int points[][2],const GrxLineOptions *o);

void GrUsrPatternedLine(int x1,int y1,int x2,int y2,GrxLinePattern *lp);
void GrUsrPatternedBox(int x1,int y1,int x2,int y2,GrxLinePattern *lp);
void GrUsrPatternedCircle(int xc,int yc,int r,GrxLinePattern *lp);
void GrUsrPatternedEllipse(int xc,int yc,int xa,int ya,GrxLinePattern *lp);
void GrUsrPatternedCircleArc(int xc,int yc,int r,int start,int end,GrxArcStyle style,GrxLinePattern *lp);
void GrUsrPatternedEllipseArc(int xc,int yc,int xa,int ya,int start,int end,GrxArcStyle style,GrxLinePattern *lp);
void GrUsrPatternedPolyLine(int numpts,int points[][2],GrxLinePattern *lp);
void GrUsrPatternedPolygon(int numpts,int points[][2],GrxLinePattern *lp);

void GrUsrPatternFilledPlot(int x,int y,GrxPattern *p);
void GrUsrPatternFilledLine(int x1,int y1,int x2,int y2,GrxPattern *p);
void GrUsrPatternFilledBox(int x1,int y1,int x2,int y2,GrxPattern *p);
void GrUsrPatternFilledCircle(int xc,int yc,int r,GrxPattern *p);
void GrUsrPatternFilledEllipse(int xc,int yc,int xa,int ya,GrxPattern *p);
void GrUsrPatternFilledCircleArc(int xc,int yc,int r,int start,int end,GrxArcStyle style,GrxPattern *p);
void GrUsrPatternFilledEllipseArc(int xc,int yc,int xa,int ya,int start,int end,GrxArcStyle style,GrxPattern *p);
void GrUsrPatternFilledConvexPolygon(int numpts,int points[][2],GrxPattern *p);
void GrUsrPatternFilledPolygon(int numpts,int points[][2],GrxPattern *p);
void GrUsrPatternFloodFill(int x, int y, GrxColor border, GrxPattern *p);

void GrUsrDrawChar(int chr,int x,int y,const GrxTextOption *opt);
void GrUsrDrawString(char *text,int length,int x,int y,const GrxTextOption *opt);
void GrUsrTextXY(int x,int y,char *text,GrxColor fg,GrxColor bg);

/* ================================================================== */
/*                    GRAPHICS CURSOR UTILITIES                       */
/* ================================================================== */

typedef struct _GR_cursor {
        GrxContext work;                            /* work areas (4) */
        int     xcord,ycord;                        /* cursor position on screen */
        int     xsize,ysize;                        /* cursor size */
        int     xoffs,yoffs;                        /* LU corner to hot point offset */
        int     xwork,ywork;                        /* save/work area sizes */
        int     xwpos,ywpos;                        /* save/work area position on screen */
        int     displayed;                          /* set if displayed */
} GrCursor;

GrCursor *GrBuildCursor(char *pixels,int pitch,int w,int h,int xo,int yo,const GrxColorTable c);
void GrDestroyCursor(GrCursor *cursor);
void GrDisplayCursor(GrCursor *cursor);
void GrEraseCursor(GrCursor *cursor);
void GrMoveCursor(GrCursor *cursor,int x,int y);

/* ================================================================== */
/*               MOUSE AND KEYBOARD INPUT UTILITIES                   */
/* ================================================================== */

#define GR_M_MOTION         0x001               /* mouse event flag bits */
#define GR_M_LEFT_DOWN      0x002
#define GR_M_LEFT_UP        0x004
#define GR_M_RIGHT_DOWN     0x008
#define GR_M_RIGHT_UP       0x010
#define GR_M_MIDDLE_DOWN    0x020
#define GR_M_MIDDLE_UP      0x040
#define GR_M_P4_DOWN        0x400
#define GR_M_P4_UP          0x800
#define GR_M_P5_DOWN        0x2000
#define GR_M_P5_UP          0x4000
#define GR_M_BUTTON_DOWN    (GR_M_LEFT_DOWN | GR_M_MIDDLE_DOWN | GR_M_RIGHT_DOWN | GR_M_P4_DOWN | GR_M_P5_DOWN)
#define GR_M_BUTTON_UP      (GR_M_LEFT_UP   | GR_M_MIDDLE_UP   | GR_M_RIGHT_UP   | GR_M_P4_UP   | GR_M_P5_UP)
#define GR_M_BUTTON_CHANGE  (GR_M_BUTTON_UP | GR_M_BUTTON_DOWN )

#define GR_M_LEFT           0x01                /* mouse button index bits */
#define GR_M_RIGHT          0x02
#define GR_M_MIDDLE         0x04
#define GR_M_P4             0x08
#define GR_M_P5             0x10

#define GR_M_KEYPRESS       0x080               /* other event flag bits */
#define GR_M_POLL           0x100
#define GR_M_NOPAINT        0x200
#define GR_COMMAND          0x1000
#define GR_M_EVENT          (GR_M_MOTION | GR_M_KEYPRESS | GR_M_BUTTON_CHANGE | GR_COMMAND)

#define GR_KB_RIGHTSHIFT    0x01                /* Keybd states: right shift key depressed */
#define GR_KB_LEFTSHIFT     0x02                /* left shift key depressed */
#define GR_KB_CTRL          0x04                /* CTRL depressed */
#define GR_KB_ALT           0x08                /* ALT depressed */
#define GR_KB_SCROLLOCK     0x10                /* SCROLL LOCK active */
#define GR_KB_NUMLOCK       0x20                /* NUM LOCK active */
#define GR_KB_CAPSLOCK      0x40                /* CAPS LOCK active */
#define GR_KB_INSERT        0x80                /* INSERT state active */
#define GR_KB_SHIFT         (GR_KB_LEFTSHIFT | GR_KB_RIGHTSHIFT)

#define GR_M_CUR_NORMAL     0                   /* MOUSE CURSOR modes: just the cursor */
#define GR_M_CUR_RUBBER     1                   /* rectangular rubber band (XOR-d to the screen) */
#define GR_M_CUR_LINE       2                   /* line attached to the cursor */
#define GR_M_CUR_BOX        3                   /* rectangular box dragged by the cursor */

#define GR_M_QUEUE_SIZE     128                 /* default queue size */

typedef struct _GR_mouseEvent {                 /* mouse event buffer structure */
        int  flags;                                 /* event type flags (see above) */
        int  x,y;                                   /* mouse coordinates */
        int  buttons;                               /* mouse button state */
        int  key;                                   /* key code from keyboard */
        int  kbstat;                                /* keybd status (ALT, CTRL, etc..) */
        long dtime;                                 /* time since last event (msec) */
} GrMouseEvent;

/*
 * mouse status information
 */
extern const struct _GR_mouseInfo {
        int   (*block)(GrxContext*,int,int,int,int); /* mouse block function */
        void  (*unblock)(int flags);                /* mouse unblock function */
        void  (*uninit)(void);                      /* mouse cleanupt function */
        struct _GR_cursor     *cursor;              /* the mouse cursor */
        struct _GR_mouseEvent *queue;               /* queue of pending input events */
        int     msstatus;                           /* -1:missing, 0:unknown, 1:present, 2:initted */
        int     displayed;                          /* cursor is (generally) drawn */
        int     blockflag;                          /* cursor temp. erase/block flag */
        int     docheck;                            /* need to check before gr. op. to screen */
        int     cursmode;                           /* mouse cursor draw mode */
        int     x1,y1,x2,y2;                        /* auxiliary params for some cursor draw modes */
        GrxColor curscolor;                          /* color for some cursor draw modes */
        int     owncursor;                          /* auto generated cursor */
        int     xpos,ypos;                          /* current mouse position */
        int     xmin,xmax;                          /* mouse movement X coordinate limits */
        int     ymin,ymax;                          /* mouse movement Y coordinate limits */
        int     spmult,spdiv;                       /* mouse cursor speed factors */
        int     thresh,accel;                       /* mouse acceleration parameters */
        int     moved;                              /* mouse cursor movement flag */
        int     qsize;                              /* max size of the queue */
        int     qlength;                            /* current # of items in the queue */
        int     qread;                              /* read pointer for the queue */
        int     qwrite;                             /* write pointer for the queue */
} * const GrMouseInfo;

int  GrMouseDetect(void);
void GrMouseEventMode(int dummy);
void GrMouseInit(void);
void GrMouseInitN(int queue_size);
void GrMouseUnInit(void);
void GrMouseSetSpeed(int spmult,int spdiv);
void GrMouseSetAccel(int thresh,int accel);
void GrMouseSetLimits(int x1,int y1,int x2,int y2);
void GrMouseGetLimits(int *x1,int *y1,int *x2,int *y2);
void GrMouseWarp(int x,int y);
void GrMouseEventEnable(int enable_kb,int enable_ms);
void GrMouseGetEvent(int flags,GrMouseEvent *event);

void GrMouseGetEventT(int flags,GrMouseEvent *event,long timout_msecs);
/* Note:
**       event->dtime is only valid if any event occured (event->flags !=0)
**       otherwise it's set as -1.
**       Additionally event timing is now real world time. (X11 && Linux
**       used clock(), user process time, up to 2.28f)
*/

int  GrMousePendingEvent(void);

GrCursor *GrMouseGetCursor(void);
void GrMouseSetCursor(GrCursor *cursor);
void GrMouseSetColors(GrxColor fg,GrxColor bg);
void GrMouseSetCursorMode(int mode,...);
void GrMouseDisplayCursor(void);
void GrMouseEraseCursor(void);
void GrMouseUpdateCursor(void);
int  GrMouseCursorIsDisplayed(void);

int  GrMouseBlock(GrxContext *c,int x1,int y1,int x2,int y2);
void GrMouseUnBlock(int return_value_from_GrMouseBlock);

#ifndef GRX_SKIP_INLINES
#define GrMouseEventMode(x)         /* nothing! */
#define GrMouseGetCursor()          (GrMouseInfo->cursor)
#define GrMouseCursorIsDisplayed()  (GrMouseInfo->displayed)
#define GrMouseInit()               GrMouseInitN(GR_M_QUEUE_SIZE);
#define GrMouseGetEvent(f,ev)       GrMouseGetEventT((f),(ev),(-1L));
#define GrMousePendingEvent() (                                                \
        GrMouseUpdateCursor(),                                                 \
   (GrMouseInfo->qlength > 0)                                                  \
)
#define GrMouseUnInit() do {                                                   \
        if(GrMouseInfo->uninit) {                                              \
        (*GrMouseInfo->uninit)();                                              \
        }                                                                      \
} while(0)
#define GrMouseGetLimits(x1p,y1p,x2p,y2p) do {                                 \
        *(x1p) = GrMouseInfo->xmin; *(y1p) = GrMouseInfo->ymin;                \
        *(x2p) = GrMouseInfo->xmax; *(y2p) = GrMouseInfo->ymax;                \
} while(0)
#define GrMouseBlock(c,x1,y1,x2,y2) (                                          \
        (((c) ? (const GrxContext*)(c) : grx_context_get_current())->gc_is_on_screen && \
         (GrMouseInfo->docheck)) ?                                             \
        (*GrMouseInfo->block)((c),(x1),(y1),(x2),(y2)) :                       \
        0                                                                      \
)
#define GrMouseUnBlock(f) do {                                                 \
        if((f) && GrMouseInfo->displayed) {                                    \
        (*GrMouseInfo->unblock)((f));                                          \
        }                                                                      \
} while(0)
#endif  /* GRX_SKIP_INLINES */

/* ================================================================== */
/*                           PNM FUNCTIONS                            */
/* ================================================================== */

/*
 *  The PNM formats, grx support load/save of
 *  binaries formats (4,5,6) only
 */

#define PLAINPBMFORMAT 1
#define PLAINPGMFORMAT 2
#define PLAINPPMFORMAT 3
#define PBMFORMAT      4
#define PGMFORMAT      5
#define PPMFORMAT      6

/* The PNM functions */

int GrSaveContextToPbm( GrxContext *grc, char *pbmfn, char *docn );
int GrSaveContextToPgm( GrxContext *grc, char *pgmfn, char *docn );
int GrSaveContextToPpm( GrxContext *grc, char *ppmfn, char *docn );
int GrLoadContextFromPnm( GrxContext *grc, char *pnmfn );
int GrQueryPnm( char *pnmfn, int *width, int *height, int *maxval );
int GrLoadContextFromPnmBuffer( GrxContext *grc, const char *buffer );
int GrQueryPnmBuffer( const char *buffer, int *width, int *height, int *maxval );

/* ================================================================== */
/*                           PNG FUNCTIONS                            */
/*  these functions may not be installed or available on all system   */
/* ================================================================== */

int GrPngSupport( void );
int GrSaveContextToPng( GrxContext *grc, char *pngfn );
int GrLoadContextFromPng( GrxContext *grc, char *pngfn, int use_alpha );
int GrQueryPng( char *pngfn, int *width, int *height );

/* ================================================================== */
/*                          JPEG FUNCTIONS                            */
/*  these functions may not be installed or available on all system   */
/* ================================================================== */

int GrJpegSupport( void );
int GrLoadContextFromJpeg( GrxContext *grc, char *jpegfn, int scale );
int GrQueryJpeg( char *jpegfn, int *width, int *height );
int GrSaveContextToJpeg( GrxContext *grc, char *jpegfn, int quality );
int GrSaveContextToGrayJpeg( GrxContext *grc, char *jpegfn, int quality );

/* ================================================================== */
/*               MISCELLANEOUS UTILITIY FUNCTIONS                     */
/* ================================================================== */

void GrResizeGrayMap(unsigned char *map,int pitch,int ow,int oh,int nw,int nh);
int  GrMatchString(const char *pattern,const char *strg);
void GrSetWindowTitle(char *title);
void GrSleep(int msec);
void GrFlush(void);
long GrMsecTime(void);        

/* ================================================================== */
/*                        TIFF ADDON FUNCTIONS                        */
/*  these functions may not be installed or available on all system   */
/* ================================================================== */

/*
** SaveContextToTiff - Dump a context in a TIFF file
**
** Arguments:
**   cxt:   Context to be saved (NULL -> use current context)
**   tiffn: Name of tiff file
**   compr: Compression method (see tiff.h), 0: automatic selection
**   docn:  string saved in the tiff file (DOCUMENTNAME tag)
**
**  Returns  0 on success
**          -1 on error
**
** requires tifflib by  Sam Leffler (sam@engr.sgi.com)
**        available at  ftp://ftp.sgi.com/graphics/tiff
*/
int SaveContextToTiff(GrxContext *cxt, char *tiffn, unsigned compr, char *docn);

#ifdef __cplusplus
}
#endif
#endif  /* __GRX_3_0_H_INCLUDED__ */
