/*
 * Copyright 2014-2015 David Lechner <david@lechnology.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

/* grx-3.0.vapi - Bindings to the GRX graphics library */

/**
 * Old school (circa 1992-2000) 2-D graphics library.
 *
 * Find out more at [[http://grx.gnu.de]].
 */
[CCode (cheader_filename = "grx-3.0.h", cprefix = "Grx", lower_case_cprefix = "grx_")]
namespace Grx {
    [CCode (cname = "int", has_type_id = false)]
    public enum Result {
        [CCode (cname = "0")]
        OK,
        [CCode (cname = "-1")]
        ERROR;
    }

    [CCode (has_destroy_function = false, has_copy_function = false, has_type_id = false)]
    [SimpleType]
    public struct Color : uint32 {
        [CCode (cname = "GrColorValue")]
        public Color value ();
        [CCode (cname = "GrColorMode")]
        public ColorMode mode ();
        [CCode (cname = "GrWriteModeColor")]
        public ColorMode to_write_mode ();
        [CCode (cname = "GrXorModeColor")]
        public ColorMode to_xor_mode ();
        [CCode (cname = "GrOrModeColor")]
        public ColorMode to_or_mode ();
        [CCode (cname = "GrAndModeColor")]
        public ColorMode to_and_mode ();
        [CCode (cname = "GrImageModeColor")]
        public ColorMode to_image_mode ();

        [CCode (cname = "GrResetColors")]
        public static void reset ();
        [CCode (cname = "GrSetRGBcolorMode")]
        public static void set_rgb_mode ();
        [CCode (cname = "GrRefreshColors")]
        public static void refresh ();

        [CCode (cname = "GrNumColors")]
        public static uint num ();
        [CCode (cname = "GrNumFreeColors")]
        public static uint num_free ();

        public static Color black { [CCode (cname = "GrBlack")]get; }
        public static Color white { [CCode (cname = "GrWhite")]get; }
        [CCode (cname = "GrNOCOLOR")]
        const Color NO_COLOR;
        public static Color no_color { get { return NO_COLOR; } }

        [CCode (cname = "GrBuildRGBcolorT")]
        public static Color build_rgb_t (int r, int g, int b);
        [CCode (cname = "GrBuildRGBcolorR")]
        public static Color build_rgb_r (int r, int g, int b);
        [CCode (cname = "GrRGBcolorRed")]
        public int red ();
        [CCode (cname = "GrRGBcolorGreen")]
        public int green ();
        [CCode (cname = "GrRGBcolorBlue")]
        public int blue ();

        [CCode (cname = "GrAllocColor")]
        public static Color alloc (int r, int g, int b);   /* shared, read-only */
        [CCode (cname = "GrAllocColorID")]
        public static Color alloc_id (int r, int g, int b); /* potentially inlined version */
        [CCode (cname = "GrAllocColor2")]
        public static Color alloc_rrggbb (long hcolor);        /* shared, read-only, 0xRRGGBB */
        [CCode (cname = "GrAllocColor2ID")]
        public static Color alloc_rrggbb_id (long hcolor);      /* potentially inlined version */
        [CCode (cname = "GrAllocCell")]
        public static Color alloc_cell ();                 /* unshared, read-write */

        [CCode (cname = "GrAllocEgaColors", array_length_cexpr = "16")]
        public static Color[] alloc_ega_colors ();           /* shared, read-only standard EGA colors */

        [CCode (cname = "GrSetColor")]
        public void set (int r, int g, int b);
        [CCode (cname = "GrFreeColor")]
        public void free ();
        [CCode (cname = "GrFreeCell")]
        public void free_cell ();

        [CCode (cname = "GrQueryColor")]
        public void query (out int r, out int g, out int b);
        [CCode (cname = "GrQueryColorID")]
        public void query_id (out int r, out int g, out int b);
        [CCode (cname = "GrQueryColor2")]
        public void query_rrggbb (out long hcolor);
        [CCode (cname = "GrQueryColor2ID")]
        public void query_rrggbb_id (out long hcolor);

        [CCode (cname = "GrColorSaveBufferSize")]
        public static int save_buffer_size ();
        [CCode (cname = "GrSaveColors")]
        public static void save_colors (void *buffer);
        [CCode (cname = "GrRestoreColors")]
        public static void restore_colors (void *buffer);
    }

    /**
     * Available video modes (for {@link set_mode})
     */
    [CCode (has_type_id = false)]
    public enum GraphicsMode {
        [CCode (cname = "GR_unknown_mode")]
        UNKNOWN_MODE,
        [CCode (cname = "GR_80_25_text")]
        TEXT_80_25,
        [CCode (cname = "GR_default_text")]
        TEXT_DEFAULT,
        [CCode (cname = "GR_width_height_text")]
        TEXT_WIDTH_HEIGHT,
        [CCode (cname = "GR_biggest_text")]
        TEXT_BIGGEST,
        [CCode (cname = "GR_320_200_graphics")]
        GRAPHICS_320_200,
        [CCode (cname = "GR_default_graphics")]
        GRAPHICS_DEFAULT,
        [CCode (cname = "GR_width_height_graphics")]
        GRAPHICS_WIDHT_HEIGHT,
        [CCode (cname = "GR_biggest_noninterlaced_graphics")]
        GRAPHICS_BIGGEST_NONINTERLACED,
        [CCode (cname = "GR_biggest_graphics")]
        GRAPHICS_BIGGEST,
        [CCode (cname = "GR_width_height_color_graphics")]
        GRAPHICS_WIDTH_HEIGHT_COLOR,
        [CCode (cname = "GR_width_height_color_text")]
        TEXT_WIDTH_HEIGHT_COLOR,
        [CCode (cname = "GR_custom_graphics")]
        GRAPHICS_CUSTOM,
        [CCode (cname = "GR_NC_80_25_text")]
        TEXT_80_25_NO_CLEAR,
        [CCode (cname = "GR_NC_default_text")]
        TEXT_DEFAULT_NO_CLEAR,
        [CCode (cname = "GR_NC_width_height_text")]
        TEXT_WIDTH_HEIGHT_NO_CLEAR,
        [CCode (cname = "GR_NC_biggest_text")]
        TEXT_BIGGEST_NO_CLEAR,
        [CCode (cname = "GR_NC_320_200_graphics")]
        GRAPHICS_320_200_NO_CLEAR,
        [CCode (cname = "GR_NC_default_graphics")]
        GRAPHICS_DEFAULT_NO_CLEAR,
        [CCode (cname = "GR_NC_width_height_graphics")]
        GRAPHICS_WIDTH_HEIGHT_NO_CLEAR,
        [CCode (cname = "GR_NC_biggest_noninterlaced_graphics")]
        GRAPHICS_BIGGEST_NONINTERLACED_NO_CLEAR,
        [CCode (cname = "GR_NC_biggest_graphics")]
        GRAPHICS_BIGGEST_NO_CLEAR,
        [CCode (cname = "GR_NC_width_height_color_graphics")]
        GRAPHICS_WIDTH_HEIGHT_COLOR_NO_CLEAR,
        [CCode (cname = "GR_NC_width_height_color_text")]
        TEXT_WIDTH_HEIGHT_COLOR_NO_CLEAR,
        [CCode (cname = "GR_NC_custom_graphics")]
        GRAPHICS_CUSTOM_NO_CLEAR,
        [CCode (cname = "GR_width_height_bpp_graphics")]
        GRAPHICS_WIDTH_HEIGHT_BPP,
        [CCode (cname = "GR_width_height_bpp_text")]
        TEXT_WIDTH_HEIGHT_BPP,
        [CCode (cname = "GR_custom_bpp_graphics")]
        GRAPHICS_CUSTOM_BPP,
        [CCode (cname = "GR_NC_width_height_bpp_graphics")]
        GRAPHICS_WIDTH_HEIGHT_BPP_NO_CLEAR,
        [CCode (cname = "GR_NC_width_height_bpp_text")]
        TEXT_WIDTH_HEIGHT_BPP_NO_CLEAR,
        [CCode (cname = "GR_NC_custom_bpp_graphics")]
        GRAPHICS_CUSTOM_BPP_NO_CLEAR
    }

    /**
     * Available frame modes (video memory layouts)
     */
    [CCode (cprefix = "GRX_FRAME_MODE_", has_type_id = false)]
    public enum FrameMode {
        /* ====== video frame buffer modes ====== */

        /**
         * Undefined
         */
        UNDEFINED,

        /**
         * Text modes
         */
        TEXT,

        /* ==== linear frame buffer modes  ====== */

        /**
         * mono
         */
        LFB_MONO01,

        /**
         * mono
         */
        LFB_MONO10,

        /**
         * (Super) VGA 256 color
         */
        LFB_8BPP,

        /**
         * Super VGA 32768/65536 color
         */
        LFB_16BPP,

        /**
         * Super VGA 16M color
         */
        LFB_24BPP,

        /**
         * Super VGA 16M color padded #1
         */
        LFB_32BPP_LOW,

        /**
         * Super VGA 16M color padded #2
         */
        LFB_32BPP_HIGH,
        /* ====== system RAM frame buffer modes ====== */

        /**
         * mono
         */
        RAM_1BPP,

        /**
         * 16 color planar
         */
        RAM_4BPP,

        /**
         * 256 color
         */
        RAM_8BPP,

        /**
         * 32768/65536 color
         */
        RAM_16BPP,

        /**
         * 16M color
         */
        RAM_24BPP,

        /**
         * 16M color padded #1
         */
        RAM_32BPP_LOW,

        /**
         * 16M color padded #2
         */
        RAM_32BPP_HIGH;

        [CCode (cname = "GrFrameNumPlanes")]
        public int num_planes ();
        [CCode (cname = "GrFrameLineOffset")]
        public int line_offset (int width);
        [CCode (cname = "GrFramePlaneSize")]
        public long plane_size (int width, int height);
        [CCode (cname = "GrFrameContextSize")]
        public long context_size (int width, int height);
    }

    /**
     * Supported video adapter types.
     */
    [CCode (has_type_id = false)]
    public enum VideoAdapter {

        /**
         * not known (before driver set)
         */
        UNKNOWN,

        /**
         * X11 driver
         */
        XWIN,

        /**
         * WIN32 driver
         */
        WIN32,

        /**
         * Linux framebuffer
         */
        LNXFB,

        /**
         * SDL driver
         */
        SDL,

        /**
         * memory only driver
         */
        MEM
    }

    [CCode (cname = "struct _GR_videoDriver", free_function = "g_free", has_type_id = false)]
    [Compact]
    public class VideoDriver {
        [CCode (cname = "struct _GR_videoDriver", destroy_function = "", has_type_id = false)]
        struct MallocStruct {}

        public string name;
        public VideoAdapter adapter;
        public VideoDriver inherit;
        [CCode (array_length_cname = "nmodes", array_length_type = "int")]
        public VideoMode[] modes;
        public VideoDriverDetect detect;
        public VideoDriverInit init;
        public VideoDriverReset reset;
        [CCode (cname = "selectmode")]
        public VideoDriverSelectMode select_mode;
        [CCode (cname = "drvflags")]
        public uint flags;

        [CCode (cname = "g_malloc0")]
        public VideoDriver (size_t size = sizeof(MallocStruct))
            requires (size == sizeof(MallocStruct));
    }

    [CCode (has_target = false, has_type_id = false)]
    public delegate int VideoDriverDetect ();
    [CCode (has_target = false, has_type_id = false)]
    public delegate int VideoDriverInit (string options);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void VideoDriverReset ();
    [CCode (has_target = false, has_type_id = false)]
    public delegate VideoMode VideoDriverSelectMode (VideoDriver driver, int width, int height, int bpp, int text, ref uint ep);

    /*
     * Video driver mode descriptor structure.
     */
    [CCode (cname = "struct _GR_videoMode", free_function = "g_free", has_type_id = false)]
    [Compact]
    public class VideoMode {
        [CCode (cname = "struct _GR_videoMode", destroy_function = "", has_type_id = false)]
        struct MallocStruct {}

        public char present;
        public char bpp;
        public short width;
        public short height;
        public short mode;
        [CCode (cname = "lineoffset")]
        public int line_offset;
        [CCode (cname = "privdata")]
        public int private_data;
        [CCode (cname = "extinfo")]
        public VideoModeExt extended_info;

        [CCode (cname = "g_malloc0")]
        public VideoMode (size_t size = sizeof(MallocStruct))
            requires (size == sizeof(MallocStruct));
    }

    /**
     * Video driver mode descriptor extension structure.
     *
     * This is a separate structure accessed via a pointer from the main mode
     * descriptor. The reason for this is that frequently several modes can
     * share the same extended info.
     */
    [CCode (cname = "struct _GR_videoModeExt", free_function = "g_free", has_type_id = false)]
    [Compact]
    public class VideoModeExt {
        [CCode (cname = "struct _GR_videoModeExt", destroy_function = "", has_type_id = false)]
        struct MallocStruct {}

        public FrameMode mode;
        public FrameDriver driver;
        [CCode (cname = "frame")]
        public char *fb_data;
        [CCode (cname = "cpos", array_length_cexpr = "3")]
        public char[] color_precision;
        [CCode (cname = "cprec", array_length_cexpr = "3")]
        public char[] color_position;
        public int flags;
        public VideoModeSetup setup;
        [CCode (cname = "setvsize")]
        public VideoModeSetSize set_size;
        public VideoModeScroll scroll;
        [CCode (cname = "setbank")]
        public VideoModeSetBank set_bank;
        [CCode (cname = "setrwbanks")]
        public VideoModeSetRWBanks set_rw_banks;
        [CCode (cname = "loadcolor")]
        public VideoModeLoadColor load_color;
        [CCode (cname = "LFB_Selector")]
        public int lfb_selector;

        [CCode (cname = "g_malloc0")]
        public VideoModeExt (size_t size = sizeof(MallocStruct))
            requires (size == sizeof(MallocStruct));
    }

    [CCode (has_target = false, has_type_id = false)]
    public delegate int VideoModeSetup (VideoMode mode, int no_clear);
    [CCode (has_target = false, has_type_id = false)]
    public delegate int VideoModeSetSize (VideoMode mode, int width, int height, out VideoMode result);
    [CCode (has_target = false, has_type_id = false)]
    public delegate int VideoModeScroll (VideoMode mode, int x, int y, [CCode (array_length = false)]out int[] result);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void VideoModeSetBank (int bank);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void VideoModeSetRWBanks (int read_bank, int write_bank);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void VideoModeLoadColor (int color_index, int red, int green, int blue);

    /**
     * The frame driver descriptor structure.
     */
    [CCode (cname = "struct _GR_frameDriver", free_function = "g_free", has_type_id = false)]
    [Compact]
    public class FrameDriver {
        [CCode (cname = "struct _GR_frameDriver", destroy_function = "", has_type_id = false)]
        struct MallocStruct {}

        public FrameMode mode;
        public FrameMode rmode;
        public int is_video;
        public int row_align;
        public int num_planes;
        public int bits_per_pixel;
        public long max_plane_size;
        public FrameDriverInit init;
        [CCode (cname = "readpixel")]
        public FrameDriverReadPixel read_pixel;
        [CCode (cname = "drawpixel")]
        public FrameDriverDrawPixel draw_pixel;
        [CCode (cname = "drawline")]
        public FrameDriverDrawLine draw_line;
        [CCode (cname = "drawhline")]
        public FrameDriverDrawHLine draw_horiz_line;
        [CCode (cname = "drawvline")]
        public FrameDriverDrawVLine draw_vert_line;
        [CCode (cname = "drawblock")]
        public FrameDriverDrawBlock draw_block;
        [CCode (cname = "drawbitmap")]
        public FrameDriverDrawBitmap draw_bitmap;
        [CCode (cname = "drawpattern")]
        public FrameDriverDrawPattern draw_pattern;
        public FrameDriverBitBlt bitblt;
        public FrameDriverBltV2R bltv2r;
        public FrameDriverBltR2V bltr2v;
        [CCode (cname = "getindexedscanline")]
        public FrameDriverGetIndexedScanLine get_indexed_scan_line;
        [CCode (cname = "putscanline")]
        public FrameDriverPutScanLine put_scan_line;

        [CCode (cname = "g_malloc0")]
        public FrameDriver (size_t size = sizeof(MallocStruct))
            requires (size == sizeof(MallocStruct));
    }

    [CCode (has_target = false, has_type_id = false)]
    public delegate int FrameDriverInit (VideoMode mode);
    [CCode (has_target = false, has_type_id = false)]
    public delegate Color FrameDriverReadPixel (Frame frame, int x, int y);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void  FrameDriverDrawPixel (int x, int y, Color c);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void FrameDriverDrawLine (int x, int y, int dx, int dy, Color c);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void FrameDriverDrawHLine (int x, int y, int w, Color c);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void FrameDriverDrawVLine (int x, int y, int h, Color c);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void FrameDriverDrawBlock (int x, int y, int w, int h, Color c);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void FrameDriverDrawBitmap (int x, int y, int w, int h, char *bmp, int pitch, int start, Color fg, Color bg);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void FrameDriverDrawPattern (int x, int y, int w, char patt, Color fg, Color bg);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void FrameDriverBitBlt (Frame dest, int dx, int dy, Frame src, int x, int y, int w, int h, ColorMode op);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void FrameDriverBltV2R (Frame dest, int dx, int dy, Frame src, int x, int y, int w, int h, ColorMode op);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void FrameDriverBltR2V (Frame dest, int dx, int dy, Frame src, int x, int y, int w, int h, ColorMode op);
    [CCode (has_target = false, has_type_id = false)]
    public delegate Color *FrameDriverGetIndexedScanLine (Frame frame, int x, int y, int w, out int? index);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void FrameDriverPutScanLine (int x, int y, int w, ref Color scl, ColorMode op);

    [CCode (cname = "struct _GR_driverInfo", has_type_id = false)]
    [Compact]
    public class DriverInfo {
        public unowned VideoDriver vdriver;
        [CCode (cname = "curmode")]
        public unowned VideoMode current_mode;
        [CCode (cname = "actmode")]
        public unowned VideoMode actual_mode;
        public unowned FrameDriver fdriver;
        public unowned FrameDriver sdriver;
        public unowned FrameDriver tdriver;
        public GraphicsMode mcode;
        [CCode (cname = "deftw")]
        public int default_text_width;
        [CCode (cname = "defth")]
        public int default_text_height;
        [CCode (cname = "defgw")]
        public int default_graphics_width;
        [CCode (cname = "defgh")]
        public int default_graphics_height;
        [CCode (cname = "deftc")]
        public Color default_text_color;
        [CCode (cname = "defgc")]
        public Color default_graphics_color;
        [CCode (cname = "vposx")]
        public int virt_pos_x;
        [CCode (cname = "vposy")]
        public int virt_pos_y;
        [CCode (cname = "errsfatal")]
        public int errors_are_fatal;
        [CCode (cname = "moderestore")]
        public int mode_restore;
        [CCode (cname = "splitbanks")]
        public int split_banks;
        [CCode (cname = "curbank")]
        public int current_bank;
        [CCode (cname = "mdsethook")]
        public DriverInfoModeSetHook mode_set_hook;
        [CCode (cname = "setbank")]
        public DriverInfoSetBank set_bank;
        [CCode (cname = "setrwbanks")]
        public DriverInfoSetRWBanks set_rw_banks;
    }

    [CCode (cname = "GrDriverInfo")]
    public DriverInfo driver_info;

    [CCode (has_target = false, has_type_id = false)]
    public delegate void DriverInfoModeSetHook ();
    [CCode (has_target = false, has_type_id = false)]
    public delegate void DriverInfoSetBank (int bank);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void DriverInfoSetRWBanks (int read_bank, int write_bank);

    /*
     * setup stuff
     */
    [CCode (cname = "GrSetDriver")]
    public bool set_driver (string driver_spec);
    [CCode (cname = "GrSetMode")]
    public bool set_mode (GraphicsMode which, ...);
    [CCode (cname = "GrSetViewport")]
    public bool set_viewport (int xpos, int ypos);
    [CCode (cname = "GrSetModeHook")]
    public void set_mode_hook (DriverInfoModeSetHook hook_func);
    [CCode (cname = "GrSetModeRestore")]
    public void set_mode_restore (int restore_flag);
    [CCode (cname = "GrSetErrorHandling")]
    public void set_error_handling (int exit_if_error);

    [CCode (cname = "GrGetLibraryVersion")]
    public uint get_library_version ();
    [CCode (cname = "GrGetLibrarySystem")]
    public uint get_library_system ();

    /*
     * inquiry stuff
     */
    [CCode (cname = "GrCurrentMode")]
    public GraphicsMode current_mode ();
    [CCode (cname = "GrAdapterType")]
    public VideoAdapter adapter_type ();
    [CCode (cname = "GrCurrentFrameMode")]
    public FrameMode current_frame_mode ();
    [CCode (cname = "GrScreenFrameMode")]
    public FrameMode screen_frame_mode ();
    [CCode (cname = "GrCoreFrameMode")]
    public FrameMode core_frame_mode ();

    [CCode (cname = "GrCurrentVideoDriver")]
    public unowned VideoDriver current_video_driver ();
    [CCode (cname = "GrCurrentVideoMode")]
    public unowned VideoMode current_video_mode ();
    [CCode (cname = "GrVirtualVideoMode")]
    public unowned VideoMode virtual_video_mode ();
    [CCode (cname = "GrCurrentFrameDriver")]
    public unowned FrameDriver current_frame_driver ();
    [CCode (cname = "GrScreenFrameDriver")]
    public unowned FrameDriver screen_frame_driver ();
    [CCode (cname = "GrFirstVideoMode")]
    public unowned VideoMode first_video_mode (FrameMode fmode);
    [CCode (cname = "GrNextVideoMode")]
    public unowned VideoMode next_video_mode (VideoMode prev);

    [CCode (cname = "GrScreenX")]
    public int screen_x ();
    [CCode (cname = "GrScreenY")]
    public int screen_y ();
    [CCode (cname = "GrVirtualX")]
    public int virtual_x ();
    [CCode (cname = "GrVirtualY")]
    public int virtual_y ();
    [CCode (cname = "GrViewportX")]
    public int viewport_x ();
    [CCode (cname = "GrViewportY")]
    public int viewport_y ();

    [CCode (cname = "GrScreenIsVirtual")]
    public int screen_is_virtual ();

    /*
     * RAM context geometry and memory allocation inquiry stuff
     */
    [CCode (cname = "GrNumPlanes")]
    public int num_planes ();
    [CCode (cname = "GrLineOffset")]
    public int line_offset (int width);
    [CCode (cname = "GrPlaneSize")]
    public long plane_size (int width, int height);
    [CCode (cname = "GrContextSize")]
    public long context_size (int width, int height);

    /* ================================================================== */
    /*              FRAME BUFFER, CONTEXT AND CLIPPING STUFF              */
    /* ================================================================== */
    [CCode (cname = "struct _GR_frame", free_function = "g_free", has_type_id = false)]
    [Compact]
    public class Frame {
        [CCode (cname = "struct _GR_frame", destroy_function = "", has_type_id = false)]
        struct MallocStruct {}

        [CCode (cname = "gf_baseaddr", array_length_cexpr = "4")]
        public char*[] base_addr;
        [CCode (cname = "gf_selector")]
        public short selector;
        [CCode (cname = "gf_onscreen")]
        public char on_screen;
        [CCode (cname = "gf_memflags")]
        public char flags;
        [CCode (cname = "gf_lineoffset")]
        public int line_offset;
        [CCode (cname = "gf_driver")]
        public unowned FrameDriver driver;

        [CCode (cname = "g_malloc0")]
        public Frame (size_t size = sizeof(MallocStruct))
            requires (size == sizeof(MallocStruct));
    }

    [CCode (cname = "GrContext", free_function = "GrDestroyContext", has_type_id = false)]
    [Compact]
    public class Context {
        [CCode (cname = "GrContext", destroy_function = "", has_type_id = false)]
        struct MallocStruct {}

        [CCode (cname = "gc_root")]
        public unowned Context root;
        [CCode (cname = "gc_xmax")]
        public int x_max;
        [CCode (cname = "gc_ymax")]
        public int y_max;
        [CCode (cname = "gc_xoffset")]
        public int x_offset;
        [CCode (cname = "gc_yoffset")]
        public int y_offset;
        [CCode (cname = "gc_xcliplo")]
        public int x_clip_low;
        [CCode (cname = "gc_ycliplo")]
        public int y_clip_low;
        [CCode (cname = "gc_xcliphi")]
        public int x_clip_high;
        [CCode (cname = "gc_ycliphi")]
        public int y_clip_high;
        [CCode (cname = "gc_usrxbase")]
        public int user_x_base;
        [CCode (cname = "gc_usrybase")]
        public int user_y_base;
        [CCode (cname = "gc_usrwidth")]
        public int user_width;
        [CCode (cname = "gc_usrheight")]
        public int user_height;
        [CCode (cname = "gc_frame.gf_baseaddr", array_length_cexpr = "4")]
        public char*[] base_addr;
        [CCode (cname = "gc_frame.gf_selector")]
        public short selector;
        [CCode (cname = "gc_frame.gf_onscreen")]
        public char on_screen;
        [CCode (cname = "gc_frame.gf_memflags")]
        public char flags;
        [CCode (cname = "gc_frame.gf_lineoffset")]
        public int line_offset;
        [CCode (cname = "gc_frame.gf_driver")]
        public unowned FrameDriver driver;

        [CCode (cname = "GrCreateContext")]
        public static Context? create (int width, int height, [CCode (array_length = false)]char*[]? memory = null, out Context? where = null);
        [CCode (cname = "GrCreateFrameContext")]
        public static Context? create_with_mode (FrameMode mode, int width, int height, [CCode (array_length = false)]char*[]? memory = null, out Context? where = null);
        [CCode (cname = "GrCreateSubContext")]
        public static Context? create_subcontext (int x1, int y1, int x2, int y2, Context parent, out Context? where = null);
        [CCode (cname = "GrSaveContext")]
        public static Context? save (out Context? where = null);

        public static unowned Context current { [CCode (cname = "GrCurrentContext")]get; }
        public static unowned Context screen {  [CCode (cname = "GrScreenContext")]get; }

        [CCode (cname = "GrResizeSubContext")]
        public void resize_subcontext (int x1, int y1, int x2, int y2);
        [CCode (cname = "GrSetContext")]
        public void set ();

        [CCode (cname = "GrSetClipBoxC")]
        public void set_clip_box (int x1, int y1, int x2, int y2);
        [CCode (cname = "GrGetClipBoxC")]
        public void get_clip_box (out int x1, out int y1, out int x2, out int y2);
        [CCode (cname = "GrResetClipBoxC")]
        public void reset_clip_box ();


        [CCode (cname = "GrClearContextC")]
        public void clear (Color bg);

        [CCode (cname = "GrFloodSpillC")]
        public void flood_spill (int x1, int y1, int x2, int y2, Color old_c, Color new_c);
        [CCode (cname = "GrFloodSpillC2")]
        public void flood_spill_2 (int x1, int y1, int x2, int y2, Color old_c1, Color new_c1, Color old_c2, Color new_c2);

        [CCode (cname = "GrPixelC")]
        public Color pixel (int x, int y);

        [CCode (cname = "GrGetScanLineC")]
        public Color *get_scan_line (int x1, int x2, int yy);


        [CCode (cname = "GrPixelCNC")]
        public Color pixel_no_clip (int x, int y);

        [CCode (cname = "GrConvertToPixmap")]
        public Pattern to_pixmap ();

        [CCode (cname = "GrUsrPixelC")]
        public Color user_pixel (int x, int y);

        [CCode (cname = "GrSaveContextToPbm")]
        public Result save_to_pbm (string file_name, string? comment = null);
        [CCode (cname = "GrSaveContextToPgm")]
        public Result save_to_pgm (string file_name, string? comment = null);
        [CCode (cname = "GrSaveContextToPpm")]
        public Result save_to_ppm (string file_name, string? comment = null);
        [CCode (cname = "GrLoadContextFromPnm")]
        public Result load_from_pnm (string file_name);

        [CCode (cname = "GrLoadContextFromPnmBuffer")]
        public Result load_from_pnm_buffer (char *buffer);

        [CCode (cname = "GrSaveContextToPng")]
        public Result save_to_png (string file_name);
        [CCode (cname = "GrLoadContextFromPng")]
        public Result load_from_png (string file_name, bool use_alpha = true);

        [CCode (cname = "GrLoadContextFromJpeg")]
        public Result load_from_jpeg (string file_name, int scale);

        [CCode (cname = "GrSaveContextToJpeg")]
        public Result save_to_jpeg (string file_name, int quality = 90);
        [CCode (cname = "GrSaveContextToGrayJpeg")]
        public Result save_to_gray_jpeg (string file_name, int quality = 90);

        //[CCode (cname = "SaveContextToTiff")]
        //public int to_tiff (string file_name, uint compr, string? comment = null);
    }

    [CCode (cname = "const struct _GR_contextInfo", has_type_id = false)]
    public struct ContextInfo {
        [CCode (cname = "&self.current")]
        public static Context current;
        [CCode (cname = "&self.screen")]
        public static Context screen;
    }

    [CCode (cname = "GrContextInfo")]
    public ContextInfo context_info;

    [CCode (cname = "GrSetClipBox")]
    public void set_clip_box (int x1, int y1, int x2, int y2);
    [CCode (cname = "GrGetClipBox")]
    public void get_clip_box (out int x1, out int y1, out int x2, out int y2);
    [CCode (cname = "GrResetClipBox")]
    public void reset_clip_box ();

    [CCode (cname = "GrMaxX")]
    public int max_x ();
    [CCode (cname = "GrMaxY")]
    public int max_y ();
    [CCode (cname = "GrSizeX")]
    public int size_x ();
    [CCode (cname = "GrSizeY")]
    public int size_y ();
    [CCode (cname = "GrLowX")]
    public int low_x ();
    [CCode (cname = "GrLowY")]
    public int low_y ();
    [CCode (cname = "GrHighX")]
    public int high_x ();
    [CCode (cname = "GrHighY")]
    public int high_y ();

    [CCode (cname = "GrxColor", has_type_id = false)]
    [Flags]
    public enum ColorMode {
        [CCode (cname = "GrWRITE")]
        WRITE,
        [CCode (cname = "GrXOR")]
        XOR,
        [CCode (cname = "GrOR")]
        OR,
        [CCode (cname = "GrAND")]
        AND,
        [CCode (cname = "GrIMAGE")]
        IMAGE,
        [CCode (cname = "GrCVALUEMASK")]
        COLOR_VALUE_MASK,
        [CCode (cname = "GrCMODEMASK")]
        COLOR_MODE_MASK,
        [CCode (cname = "GR_UNDERLINE_TEXT")]
        UNDERLINE_TEXT
    }

    [CCode (cname = "const struct _GR_colorInfo", has_type_id = false)]
    public struct ColorInfo {

        /**
         * number of colors
         */
        public Color ncolors;

        /**
         * number of unallocated colors
         */
        public Color nfree;

        /**
         * the black color
         */
        public Color black;

        /**
         * the white color
         */
        public Color white;

        /**
         * set when RGB mode
         */
        [CCode (cname = "RGBmode")]
        public uint rgb_mode;

        /**
         * color field precisions
         */
        public uint prec[3];

        /**
         * color field positions
         */
        public uint pos[3];

        /**
         * masks for significant bits
         */
        public uint mask[3];

        /**
         * add these for rounding
         */
        public uint round[3];

        /**
         * shifts for (un)packing color
         */
        public uint shift[3];

        /**
         * normalization for (un)packing
         */
        public uint norm;

        /**
         * color table for non-RGB modes
         */
        [CCode (array_length_cexpr = "256")]
        public ColorInfoTable[] ctable;
    }

    [CCode (has_type_id = false)]
    public struct ColorInfoTable {
        /* loaded components */
        public uchar r;
        public uchar g;
        public uchar b;

        /**
         * r, g, b values are valid if set
         */
        public uint  defined;

        /**
         * can be changed by 'GrSetColor'
         */
        public uint  writable;

        /**
         * usage count
         */
        public ulong nused;
    }

    [CCode (cname = "GrColorInfo")]
    public ColorInfo color_info;

    public const int MAX_POLYGON_POINTS;
    public const int MAX_ELLIPSE_POINTS;

    [CCode (cprefix = "GR_ARC_STYLE_", has_type_id = false)]
    public enum ArcStyle {
        OPEN,
        CLOSE1,
        CLOSE2
    }

    [CCode (cname = "GrFBoxColors", free_function = "g_free", has_type_id = false)]
    [Compact]
    public class FramedBoxColors {
        [CCode (cname = "GrFBoxColors", destroy_function = "", has_type_id = false)]
        struct MallocStruct {}

        [CCode (cname = "fbx_intcolor")]
        public Color fill;
        [CCode (cname = "fbx_topcolor")]
        public Color top;
        [CCode (cname = "fbx_rightcolor")]
        public Color right;
        [CCode (cname = "fbx_bottomcolor")]
        public Color bottom;
        [CCode (cname = "fbx_leftcolor")]
        public Color left;

        [CCode (cname = "g_malloc0")]
        public FramedBoxColors (size_t size = sizeof(MallocStruct))
            requires (size == sizeof(MallocStruct));
    }

    /**
     * Color table (for primitives using several colors).
     *
     * It is an array of colors with the first element being the number of
     * colors in the table
     */
    [CCode (cname = "GrxColor", free_function = "g_free", has_type_id = false)]
    [Compact]
    public class ColorTable {
        [CCode (cname = "GrxColor", destroy_function = "", has_type_id = false)]
        struct MallocStruct {}

        [CCode (cname = "g_malloc0")]
        ColorTable.alloc (size_t size);
        public ColorTable (uint size) {
            this.alloc (alloc_size (size) * sizeof(MallocStruct));
        }

        [CCode (cname = "GR_CTABLE_SIZE")]
        public uint size ();
        [Ccode (cname = "GR_CTABLE_COLOR")]
        public Color get (uint index);

        [CCode (cname = "GR_CTABLE_ALLOCSIZE")]
        int alloc_size (uint num_colors);
    }

    /* ================================================================== */
    /*                       GRAPHICS PRIMITIVES                          */
    /* ================================================================== */

    [CCode (cname = "GrClearScreen")]
    public void clear_screen (Color bg);
    [CCode (cname = "GrClearContext")]
    public void clear_context (Color bg);
    [CCode (cname = "GrClearClipBox")]
    public void clear_clip_box (Color bg);
    [CCode (cname = "GrPlot")]
    public void plot (int x, int y, Color c);
    [CCode (cname = "GrLine")]
    public void line (int x1, int y1, int x2, int y2, Color c);
    [CCode (cname = "GrHLine")]
    public void horiz_line (int x1, int x2, int y, Color c);
    [CCode (cname = "GrVLine")]
    public void vert_line (int x, int y1, int y2, Color c);
    [CCode (cname = "GrBox")]
    public void box (int x1, int y1, int x2, int y2, Color c);
    [CCode (cname = "GrFilledBox")]
    public void filled_box (int x1, int y1, int x2, int y2, Color c);
    [CCode (cname = "GrFramedBox")]
    public void framed_box (int x1, int y1, int x2, int y2, int width, FramedBoxColors c);
    public void rounded_box (int x1, int y1, int x2, int y2, int radius, Color c) {
        circle_arc (x1 + radius, y1 + radius, radius, 900, 1800, ArcStyle.OPEN, c);
        circle_arc (x2 - radius, y1 + radius, radius, 0, 900, ArcStyle.OPEN, c);
        circle_arc (x1 + radius, y2 - radius, radius, 1800, 2700, ArcStyle.OPEN, c);
        circle_arc (x2 - radius, y2 - radius, radius, 2700, 3600, ArcStyle.OPEN, c);
        horiz_line (x1 + radius, x2 - radius, y1, c);
        horiz_line (x1 + radius, x2 - radius, y2, c);
        vert_line (x1, y1 + radius, y2 - radius, c);
        vert_line (x2, y1 + radius, y2 - radius, c);
    }
    public void filled_rounded_box (int x1, int y1, int x2, int y2, int radius, Color c) {
        filled_circle_arc (x1 + radius, y1 + radius, radius, 900, 1800, ArcStyle.CLOSE2, c);
        filled_circle_arc (x2 - radius, y1 + radius, radius, 0, 900, ArcStyle.CLOSE2, c);
        filled_circle_arc (x1 + radius, y2 - radius, radius, 1800, 2700, ArcStyle.CLOSE2, c);
        filled_circle_arc (x2 - radius, y2 - radius, radius, 2700, 3600, ArcStyle.CLOSE2, c);
        filled_box (x1 + radius, y1, x2 - radius, y2, c);
        filled_box (x1, y1 + radius, x2, y2 - radius, c);
    }
    [CCode (cname = " GrGenerateEllipse")]
    public int generate_ellipse (int xc, int yc, int xa, int ya, [CCode (array_length_cexpr = "MAX_POLYGON_POINTS")]Point[] points);
    [CCode (cname = " GrGenerateEllipseArc")]
    public int  generate_ellipse_arc (int xc, int yc, int xa, int ya, int start, int end, [CCode (array_length_cexpr = "MAX_POLYGON_POINTS")]Point[] points);
    [CCode (cname = "GrLastArcCoords")]
    public void last_arc_coords (out int xs, out int ys, out int xe, out int ye, out int xc, out int yc);
    [CCode (cname = "GrCircle")]
    public void circle (int xc, int yc, int r, Color c);
    [CCode (cname = "GrEllipse")]
    public void ellipse (int xc, int yc, int xa, int ya, Color c);
    [CCode (cname = "GrCircleArc")]
    public void circle_arc (int xc, int yc, int r, int start, int end, ArcStyle style, Color c);
    [CCode (cname = "GrEllipseArc")]
    public void ellipse_arc (int xc, int yc, int xa, int ya, int start, int end, ArcStyle style, Color c);
    [CCode (cname = "GrFilledCircle")]
    public void filled_circle (int xc, int yc, int r, Color c);
    [CCode (cname = "GrFilledEllipse")]
    public void filled_ellipse (int xc, int yc, int xa, int ya, Color c);
    [CCode (cname = "GrFilledCircleArc")]
    public void filled_circle_arc (int xc, int yc, int r, int start, int end, ArcStyle style, Color c);
    [CCode (cname = "GrFilledEllipseArc")]
    public void filled_ellipse_rc (int xc, int yc, int xa, int ya, int start, int end, ArcStyle style, Color c);
    [CCode (cname = "GrPolyLine")]
    public void poly_line ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
    [CCode (cname = "GrPolygon")]
    public void polygon ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
    [CCode (cname = "GrFilledConvexPolygon")]
    public void filled_convex_polygon ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
    [CCode (cname = "GrFilledPolygon")]
    public void filled_polygon ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
    [CCode (cname = "GrBitBlt")]
    public void bit_blt (Context dest, int x, int y, Context src, int x1, int y1, int x2, int y2, ColorMode op = ColorMode.WRITE);
    [CCode (cname = "GrBitBlt1bpp")]
    public void bit_blt_1bpp (Context dest, int dx, int dy, Context src, int x1, int y1, int x2, int y2, Color fg, Color bg);
    [CCode (cname = "GrFloodFill")]
    public void flood_fill (int x, int y, Color border, Color c);
    [CCode (cname = "GrFloodSpill")]
    public void flood_spill (int x1, int y1, int x2, int y2, Color old_c, Color new_c);
    [CCode (cname = "GrFloodSpill2")]
    public void flood_spill_2 (int x1,  int y1, int x2, int y2, Color old_c1, Color new_c1, Color old_c2, Color new_c2);

    [CCode  (cname =  "GrPixel")]
    public Color pixel (int x, int y);

    [CCode  (cname =  "GrGetScanLine")]
    public Color *get_scan_line (int  x1, int x2, int yy);

    [CCode  (cname =  "GrPutScanline")]
    public void put_scan_line (int x1, int x2, int yy, [CCode (array_length = false)]Color[] c, ColorMode op = ColorMode.WRITE);

    /* ================================================================== */
    /*                 NON CLIPPING DRAWING PRIMITIVES                    */
    /* ================================================================== */

    [CCode (cname = "GrPlotNC")]
    public void plot_no_clip(int x, int y, Color c);
    [CCode (cname = "GrLineNC")]
    public void line_no_clip(int x1, int y1, int x2, int y2, Color c);
    [CCode (cname = "GrHLineNC")]
    public void horiz_line_no_clip(int x1, int x2, int y, Color c);
    [CCode (cname = "GrVLineNC")]
    public void vert_line_no_clip(int x, int y1, int y2, Color c);
    [CCode (cname = "GrBoxNC")]
    public void box_no_clip(int x1, int y1, int x2, int y2, Color c);
    [CCode (cname = "GrFilledBoxNC")]
    public void filled_box_no_clip(int x1, int y1, int x2, int y2, Color c);
    [CCode (cname = "GrFramedBoxNC")]
    public void framed_box_no_clip(int x1, int y1, int x2, int y2, int wdt, FramedBoxColors c);
    [CCode (cname = "GrBitBltNC")]
    public void bit_blt_no_clip(Context dest, int x, int y, Context src, int x1, int y1, int x2, int y2, ColorMode op = ColorMode.WRITE);

    [CCode (cname =  "GrPixelNC")]
    public Color pixel_no_clip (int x, int y);

    [CCode (cname = "int", cprefix = "GR_TEXT_", has_type_id = false)]
    public enum TextDirection {
        RIGHT,
        DOWN,
        LEFT,
        UP,
        DEFAULT;

        [CCode (cname = "GR_TEXT_IS_VERTICAL")]
        public bool is_vertical ();
    }

    [CCode (cname = "int", cprefix = "GR_ALIGN_", has_type_id = false)]
    public enum TextHorizAlign {
        LEFT,
        CENTER,
        RIGHT,
        DEFAULT;
    }

    [CCode (cname = "int", cprefix = "GR_ALIGN_", has_type_id = false)]
    public enum TextVertAlign {
        TOP,
        [CCode (cname = "GR_ALIGN_CENTER")]
        MIDDLE,
        BOTTOM,
        BASELINE,
        DEFAULT;
    }

    [CCode (cname = "int", has_type_id = false)]
    public enum ChrType {
        [CCode (cname = "GR_BYTE_TEXT")]
        BYTE,
        [CCode (cname = "GR_WORD_TEXT")]
        WORD,
        [CCode (cname = "GR_ATTR_TEXT")]
        ATTR;

        [CCode (cname = "GR_TEXTCHR_SIZE")]
        public int size ();
        [CCode (cname = "GR_TEXTCHR_CODE", instance_pos = 1.1)]
        public uint code (int ch);
        [CCode (cname = "GR_TEXTCHR_ATTR", instance_pos = 1.1)]
        public CharAttr attr (int ch);
    }

    [CCode (cname = "char", has_destroy_function = false, has_copy_function = false, has_type_id = false)]
    [SimpleType]
    public struct CharAttr : char {
        [CCode (cname = "GR_BUILD_ATTR")]
        public static CharAttr build (Color fg, Color bg, bool underline);
        [CCode (cname = "GR_ATTR_FGCOLOR")]
        public Color fg_color ();
        [CCode (cname = "GR_ATTR_BGCOLOR")]
        public Color bg_color ();
        [CCode (cname = "GR_ATTR_UNDERLINE")]
        public bool underline ();
    }

    [CCode (cname = "int", cprefix = "GR_FONTCVT_", has_type_id = false)]
    [Flags]
    public enum FontConversionFlag {
        NONE,
        [CCode (cname = "SKIPCHARS")]
        SKIP_CHARS,
        RESIZE,
        ITALICIZE,
        BOLDIFY,
        FIXIFY,
        PROPORTION
    }

    [CCode (cname = "struct _GR_fontChrInfo", free_function = "g_free", has_type_id = false)]
    [Compact]
    public class FontChrInfo {
        [CCode (cname = "struct _GR_fontChrInfo", destroy_function = "", has_type_id = false)]
        struct MallocStruct {}

        public uint width;
        public uint offset;

        [CCode (cname = "g_malloc0")]
        public FontChrInfo (size_t size = sizeof(MallocStruct))
            requires (size == sizeof(MallocStruct));
    }

    [CCode (cname = "struct _GR_font", free_function = "GrUnloadFont", has_type_id = false)]
    [Compact]
    public class Font {
        [CCode (cname = "&GrFont_PC6x8")]
        public static unowned Font pc6x8;
        [CCode (cname = "&GrFont_PC8x8")]
        public static unowned Font pc8x8;
        [CCode (cname = "&GrFont_PC8x14")]
        public static unowned Font pc8x14;
        [CCode (cname = "&GrFont_PC8x16")]
        public static unowned Font pc8x16;
        [CCode (cname = "&GrDefaultFont")]
        public static unowned Font default;

        [CCode (cname = "h.name")]
        public string name;
        [CCode (cname = "h.family")]
        public string family;
        [CCode (cname = "h.proportional")]
        public bool proportional;
        [CCode (cname = "h.scalable")]
        public bool scalable;
        [CCode (cname = "h.prelaoded")]
        public bool prelaoded;
        [CCode (cname = "h.modified")]
        public bool modified;
        [CCode (cname = "h.width")]
        public uint width;
        [CCode (cname = "h.height")]
        public uint height;
        [CCode (cname = "h.baseline")]
        public uint baseline;
        [CCode (cname = "h.ulpos")]
        public uint underline_pos;
        [CCode (cname = "h.ulheight")]
        public uint underline_height;
        [CCode (cname = "h.minchar")]
        public uint min_char;
        [CCode (cname = "h.numchars")]
        public uint num_chars;

        public char *bitmap;
        public char *auxmap;
        public uint minwidth;
        public uint maxwidth;
        public uint auxsize;
        public uint auxnext;
        [CCode (array_length_cexpr = "7")]
        public int[] auxofs;
        [CCode (array_length_cexpr = "1")]
        public FontChrInfo[] chrinfo;

        [CCode (cname = "GrLoadFont")]
        public static Font? load (string name);
        [CCode (cname = "GrLoadConvertedFont")]
        public static Font? load_converted (string name, FontConversionFlag flags, int w = 0, int h = 0, int min_ch = 0, int max_ch = 0);
        [CCode (cname = "GrBuildConvertedFont")]
        public Font build_converted (FontConversionFlag flags, int w = 0, int h = 0, int min_ch = 0, int max_ch = 0);

        [CCode (cname = "GrDumpFont")]
        public void dump (string c_symbol_name, string file_name);
        [CCode (cname = "GrDumpFnaFont")]
        public void dump_fna (string file_name);
        [CCode (cname = "GrSetFontPath")]
        public static void set_path (string path_list);

        [CCode (cname = "GrFontCharPresent")]
        public int char_present (int chr);
        [CCode (cname = "GrFontCharWidth")]
        public int char_width (int chr);
        [CCode (cname = "GrFontCharHeight")]
        public int char_height (int chr);
        [CCode (cname = "GrFontCharBmpRowSize")]
        public int char_bmp_row_size (int chr);
        [CCode (cname = "GrFontCharBitmapSize")]
        public int char_bitmap_size (int chr);
        [CCode (cname = "GrFontStringWidth")]
        public int string_width (void *text, int len, ChrType type = ChrType.BYTE);
        public int vala_string_width (string text) {
            return string_width (text.data, text.length);
        }
        [CCode (cname = "GrFontStringHeight")]
        public int string_height (void *text, int len, ChrType type = ChrType.BYTE);
        public int vala_string_height (string text) {
            return string_height (text.data, text.length);
        }
        [CCode (cname = "GrProportionalTextWidth")]
        public int proportional_text_width (void *text, int len, ChrType type = ChrType.BYTE);
        public int proportional_vala_string_width (string text) {
            return proportional_text_width (text.data, text.length);
        }

        [CCode (cname = "GrBuildAuxiliaryBitmap")]
        public char *build_auxiliary_bitmap (int chr, TextDirection dir, bool ul);
        [CCode (cname = "GrFontCharBitmap")]
        public char *char_bitmap (int chr);
        [CCode (cname = "GrFontCharAuxBmp")]
        public char *char_aux_bitmap (int chr, TextDirection dir, bool ul);
    }

    [CCode (cname = "union _GR_textColor", has_type_id = false)]
    [SimpleType]
    public struct TextColor : uint32 {
        [CCode (cname = "v")]
        public Color as_color;
        [CCode (cname = "p")]
        unowned ColorTable as_color_table;
    }

    [CCode (cname = "struct _GR_textOption", free_function = "g_free", has_type_id = false)]
    [Compact]
    public class TextOption {
        [CCode (cname = "struct _GR_textOption", destroy_function = "", has_type_id = false)]
        struct MallocStruct {}

        [CCode (cname = "txo_font")]
        public unowned Font font;
        [CCode (cname = "txo_fgcolor")]
        public unowned TextColor fg_color;
        [CCode (cname = "txo_bgcolor")]
        public unowned TextColor bg_color;
        [CCode (cname = "txo_chrtype")]
        public ChrType chr_type;
        [CCode (cname = "txo_direct")]
        public TextDirection direction;
        [CCode (cname = "txo_xalign")]
        public TextHorizAlign x_align;
        [CCode (cname = "txo_yalign")]
        public TextVertAlign y_align;

        [CCode (cname = "g_malloc0")]
        public TextOption (size_t size = sizeof(MallocStruct))
            requires (size == sizeof(MallocStruct));

        [CCode (cname = "GrCharWidth", instance_pos = 1.1)]
        public int char_width (int chr);
        [CCode (cname = "GrCharHeight", instance_pos = 1.1)]
        public int char_height (int chr);
        [CCode (cname = "GrCharSize", instance_pos = 1.1)]
        public void char_size (int chr, out int width, out int height);
        [CCode (cname = "GrStringWidth", instance_pos = 2.1)]
        public int string_width (void *text, int length);
        public int vala_string_width (string test) {
            return string_width (test.data, test.length);
        }
        [CCode (cname = "GrStringHeight", instance_pos = 2.1)]
        public int string_height (void *text, int length);
        public int vala_string_height (string text) {
            return string_height (text.data, text.length);
        }
        [CCode (cname = "GrStringSize", instance_pos = 2.1)]
        public void string_size (void *text, int length, out int w, out int h);
        public void vala_string_size (string text, out int w, out int h) {
            string_size (text.data, text.length, out w, out h);
        }
    }

    [CCode (cname = "GrTextRegion", free_function = "g_free", has_type_id = false)]
    [Compact]
    public class TextRegion {
        [CCode (cname = "GrTextRegion", destroy_function = "", has_type_id = false)]
        struct MallocStruct {}

        [CCode (cname = "txr_font")]
        public unowned Font font;
        [CCode (cname = "txr_fgcolor")]
        public Color fg_color;
        [CCode (cname = "txr_bgcolor")]
        public Color bg_color;
        [CCode (cname = "txr_buffer")]
        public void *buffer;
        [CCode (cname = "txr_backup")]
        public void *backup;
        [CCode (cname = "txr_width")]
        public int width;
        [CCode (cname = "txr_height")]
        public int height;
        [CCode (cname = "txr_lineoffset")]
        public int line_offset;
        [CCode (cname = "txr_xpos")]
        public int x_pos;
        [CCode (cname = "txr_ypos")]
        public int y_pos;
        [CCode (cname = "txr_chrtype")]
        public ChrType chr_type;

        [CCode (cname = "g_malloc0")]
        public TextRegion (size_t size = sizeof(MallocStruct))
            requires (size == sizeof(MallocStruct));

        [CCode (cname = "GrDumpChar", instance_pos = 3.1)]
        public void dump_char (int chr, int col, int row);
        [CCode (cname = "GrDumpText", instance_pos = 4.1)]
        public void dump_text (int col, int row, int width, int height);
        [CCode (cname = "GrDumpTextRegion")]
        public void dump ();
    }

    [CCode (cname = "GrDrawChar")]
    public void draw_char (int chr, int x, int y, TextOption opt);
    [CCode (cname = "GrDrawString")]
    public void draw_string (void *text, int length, int x, int y, TextOption opt);
    public void draw_vala_string (string text, int x, int y, TextOption opt) {
        draw_string (text.data, text.length, x, y, opt);
    }
    [CCode (cname = "GrTextXY")]
    public void draw_text_xy (int x, int y, string text, Color fg, Color bg);

    /* ================================================================== */
    /*            THICK AND DASHED LINE DRAWING PRIMITIVES                */
    /* ================================================================== */

    /**
     * Custom line option structure.
     *
     * Zero or one dash pattern length means the line is continuous.
     * The dash pattern always begins with a drawn section.
     */
    [CCode (cname = "GrLineOption", free_function = "g_free", has_type_id = false)]
    [Compact]
    public class LineOption {
        [CCode (cname = "GrLineOption", destroy_function = "", has_type_id = false)]
        struct MallocStruct {}

        /**
         * Color used to draw line.
         */
        [CCode (cname = "lno_color")]
        public Color color;

        /**
         * Width of the line.
         */
        [CCode (cname = "lno_width")]
        public int width;

        [CCode (cname = "g_malloc0")]
        public LineOption (size_t size = sizeof(MallocStruct))
            requires (size == sizeof(MallocStruct));
        /**
         * Draw/no-draw pattern.
         */
        [CCode (cname = "lno_dashpat", array_length_cname = "lno_pattlen")]
        public uchar[] dashpat;
    }

    [CCode (cname = "", destroy_function = "", has_type_id = false)]
    public struct Point {
        int x;
        int y;
    }

    [CCode (cname = "GrCustomLine")]
    public void custom_line(int x1, int y1, int x2, int y2, LineOption o);
    [CCode (cname = "GrCustomBox")]
    public void custom_box(int x1, int y1, int x2, int y2, LineOption o);
    [CCode (cname = "GrCustomCircle")]
    public void custom_circle(int xc, int yc, int r, LineOption o);
    [CCode (cname = "GrCustomEllipse")]
    public void custom_ellipse(int xc, int yc, int xa, int ya, LineOption o);
    [CCode (cname = "GrCustomCircleArc")]
    public void custom_circle_arc(int xc, int yc, int r, int start, int end, ArcStyle style, LineOption o);
    [CCode (cname = "GrCustomEllipseArc")]
    public void custom_ellipse_arc(int xc, int yc, int xa, int ya, int start, int end, ArcStyle style, LineOption o);
    [CCode (cname = "GrCustomPolyLine")]
    public void custom_poly_line([CCode (array_length_pos = 0.9)]Point[] points, LineOption o);
    [CCode (cname = "GrCustomPolygon")]
    public void custom_polygon([CCode (array_length_pos = 0.9)]Point[] points, LineOption o);

    /* ================================================================== */
    /*             PATTERNED DRAWING AND FILLING PRIMITIVES               */
    /* ================================================================== */

    /**
     * A mode independent way to specify a fill pattern of two colors.
     *
     * It is always 8 pixels wide (1 byte per scan line), its height is
     * user-defined. SET THE TYPE FLAG TO ZERO!!!
     */
    [CCode (cname = "struct _GR_bitmap", free_function = "g_free", has_type_id = false)]
    [Compact]
    public class Bitmap {
        [CCode (cname = "struct _GR_bitmap", destroy_function = "", has_type_id = false)]
        struct MallocStruct {}

        /**
         * Type flag for pattern union.
         */
        [CCode (cname = "bmp_ispixmap")]
        public bool is_pixmap;

        /**
         * Bitmap height
         */
        [CCode (cname = "bmp_height")]
        public int height;

        /**
         * Pointer to the bit pattern
         */
        [CCode (cname = "bmp_data")]
        public char *data;

        /**
         * Foreground color for fill
         */
        [CCode (cname = "bmp_fgcolor")]
        public Color fg_color;

        /**
         * background color for fill
         */
        [CCode (cname = "bmp_bgcolor")]
        public Color bg_color;

        [CCode (cname = "g_malloc0")]
        public Bitmap (size_t size = sizeof(MallocStruct))
            requires (size == sizeof(MallocStruct));

        /**
         * Set if dynamically allocated
         */
        [CCode (cname = "bmp_memflags")]
        public int flags;
    }

    /**
     * A fill pattern stored in a layout identical to the video RAM for filling
     * using 'bitblt'-s.
     *
     * It is mode dependent, typically one of the library functions is used to
     * build it. KEEP THE TYPE FLAG NONZERO!!!
     */
    [CCode (cname = "struct _GR_pixmap", free_function = "g_free", has_type_id = false)]
    [Compact]
    public class Pixmap {
        [CCode (cname = "struct _GR_pixmap", destroy_function = "", has_type_id = false)]
        struct MallocStruct {}

        [CCode (cname = "pxp_ispixmap")]
        public bool is_pixmap;               /* type flag for pattern union */
        [CCode (cname = "pxp_width")]
        public int width;                  /* pixmap width (in pixels)  */
        [CCode (cname = "pxp_height")]
        public int height;                 /* pixmap height (in pixels) */
        [CCode (cname = "pxp_oper")]
        public ColorMode operator;                   /* bitblt mode (SET, OR, XOR, AND, IMAGE) */
        [CCode (cname = "&self->pxp_source")]
        public static Frame pxp_source;        /* source context for fill */

        [CCode (cname = "g_malloc0")]
        public Pixmap (size_t size = sizeof(MallocStruct))
            requires (size == sizeof(MallocStruct));
    }

    /**
     * Fill pattern union -- can either be a bitmap or a pixmap
     */
    [CCode (cname = "GrPattern", free_function = "GrDestroyPattern", has_type_id = false)]
    [Compact]
    public class Pattern {
        [CCode (cname = "gp_ispixmap")]
        public bool is_pixmap;               /* nonzero for pixmaps */

        [CCode (cname = "GrBuildPixmap")]
        public Pattern.buildPixmap ([CCode (array_length = false)]char *pixels, int w, int h, ColorTable colors);
        [CCode (cname = "GrBuildPixmapFromBits")]
        public Pattern.buildPixmapFromBits ([CCode (array_length = false)]char *bits, int w, int h, Color fgc, Color bgc);
    }

    /**
     * Draw pattern for line drawings.
     *
     * Specifies both the:
     * 1. Fill pattern, and the
     * 2. Custom line drawing option
     */
    [CCode (cname = "GrLinePattern", free_function = "g_free", has_type_id = false)]
    [Compact]
    public class LinePattern {
        [CCode (cname = "GrLinePattern", destroy_function = "", has_type_id = false)]
        struct MallocStruct {}

        /**
         * Fill pattern
         */
        [CCode (cname = "lnp_pattern")]
        Pattern pattern;
        /**
         * width + dash pattern
         */
        [CCode (cname = "lnp_option")]
        LineOption option;

        [CCode (cname = "g_malloc0")]
        public LinePattern (size_t size = sizeof(MallocStruct))
            requires (size == sizeof(MallocStruct));
    }

    [CCode (cname = "GrPatternedLine")]
    public void patterned_line (int x1, int y1, int x2, int y2, LinePattern line_pattern);
    [CCode (cname = "GrPatternedBox")]
    public void patterned_box  (int x1, int y1, int x2, int y2, LinePattern line_pattern);
    [CCode (cname = "GrPatternedCircle")]
    public void patterned_circle (int xc, int yc, int r, LinePattern line_pattern);
    [CCode (cname = "GrPatternedEllipse")]
    public void patterned_ellipse (int xc, int yc, int xa, int ya, LinePattern line_pattern);
    [CCode (cname = "GrPatternedCircleArc")]
    public void patterned_circle_arc (int xc, int yc, int r, int start, int end, ArcStyle style, LinePattern line_pattern);
    [CCode (cname = "GrPatternedEllipseArc")]
    public void patterned_ellipse_arc (int xc, int yc, int xa, int ya, int start, int end, ArcStyle style, LinePattern line_pattern);
    [CCode (cname = "GrPatternedPolyLine")]
    public void patterned_poly_line ([CCode (array_length_pos = 0.9)]Point[] points, LinePattern line_pattern);
    [CCode (cname = "GrPatternedPolygon")]
    public void patterned_polygon ([CCode (array_length_pos = 0.9)]Point[] points, LinePattern line_pattern);

    [CCode (cname = "GrPatternFilledPlot")]
    public void pattern_filled_plot (int x, int y, Pattern pattern);
    [CCode (cname = "GrPatternFilledLine")]
    public void pattern_filled_line (int x1, int y1, int x2, int y2, Pattern pattern);
    [CCode (cname = "GrPatternFilledBox")]
    public void pattern_filled_box (int x1, int y1, int x2, int y2, Pattern pattern);
    [CCode (cname = "GrPatternFilledCircle")]
    public void pattern_filled_circle (int xc, int yc, int r, Pattern pattern);
    [CCode (cname = "GrPatternFilledEllipse")]
    public void pattern_filled_ellipse (int xc, int yc, int xa, int ya, Pattern pattern);
    [CCode (cname = "GrPatternFilledCircleArc")]
    public void pattern_filled_circle_arc (int xc, int yc, int r, int start, int end, int style, Pattern pattern);
    [CCode (cname = "GrPatternFilledEllipseArc")]
    public void pattern_filled_ellipse_arc (int xc, int yc, int xa, int ya, int start, int end, int style, Pattern pattern);
    [CCode (cname = "GrPatternFilledConvexPolygon")]
    public void pattern_filled_convex_polygon ([CCode (array_length_pos = 0.9)]Point[] points, Pattern pattern);
    [CCode (cname = "GrPatternFilledPolygon")]
    public void pattern_filled_polygon ([CCode (array_length_pos = 0.9)]Point[] points, Pattern pattern);
    [CCode (cname = "GrPatternFloodFill")]
    public void pattern_flood_fill (int x, int y, Color border, Pattern pattern);

    [CCode (cname = "GrPatternDrawChar")]
    public void pattern_draw_char (int chr, int x, int y, TextOption opt, Pattern pattern);
    [CCode (cname = "GrPatternDrawString")]
    public void pattern_draw_string (void *text, int length, int x, int y, TextOption opt, Pattern pattern);
    public void pattern_draw_vala_string (string text, int x, int y, TextOption opt, Pattern pattern) {
        pattern_draw_string (text.data, text.length, x, y, opt, pattern);
    }
    [CCode (cname = "GrPatternDrawStringExt")]
    public void pattern_draw_string_ext (void *text, int length, int x, int y, TextOption opt, Pattern pattern);
    public void pattern_draw_vala_string_ext (string text, int x, int y, TextOption opt, Pattern pattern) {
        pattern_draw_string_ext (text.data, text.length, x, y, opt, pattern);
    }

    /* ================================================================== */
    /*                      IMAGE MANIPULATION                            */
    /* ================================================================== */


    /**
     * Flags for {@link Image.inverse}.
     */
    [CCode (cname = "int", has_type_id = false)]
    [Flags]
    public enum ImageInverseFlag {
        [CCode (cname = "GR_IMAGE_INVERSE_LR")]
        LEFT_RIGHT, /* inverse left right */
        [CCode (cname = "GR_IMAGE_INVERSE_TD")]
        TOP_DOWN /* inverse top down */
    }

    [CCode (cname = "GrImage", free_function = "GrImageDestroy", has_type_id = false)]
    [Compact]
    public class Image {
        [CCode (cname = "GrImage", destroy_function = "", has_type_id = false)]
        struct MallocStruct {}

        [CCode (cname = "lnp_pattern")]
        Pattern pattern;         /* fill pattern */
        [CCode (cname = "lnp_option")]
        LineOption option;          /* width + dash pattern */

        [CCode (cname = "GrImageBuild")]
        public Image ([CCode (array_length = false)]char[] pixels, int w, int h, ColorTable colors);
        [CCode (cname = "GrImageBuildUsedAsPattern")]
        public Image.used_as_pattern ([CCode (array_length = false)]char[] pixels, int w, int h, ColorTable colors);

        [CCode (cname = "GrImageDisplay", instance_pos = 2.1)]
        public void display (int x, int y);
        [CCode (cname = "GrImageDisplayExt", instance_pos = 4.1)]
        public void display_ext (int x1, int y1, int x2, int y2);
        [CCode (cname = "GrImageFilledBoxAlign", instance_pos = 6.1)]
        public void filled_box_align (int xo, int yo, int x1, int y1, int x2, int y2);
        [CCode (cname = "GrImageHLineAlign", instance_pos = 6.1)]
        public void HLineAlign (int xo, int yo, int x, int y, int width);
        [CCode (cname = "GrImagePlotAlign", instance_pos = 4.1)]
        public void PlotAlign (int xo, int yo, int x, int y);

        [CCode (cname = "GrImageInverse")]
        public Image inverse (ImageInverseFlag flag);
        [CCode (cname = "GrImageStretch")]
        public Image stretch (int width, int height);

        [CCode (cname = "GrImageFromPattern")]
        public static Image from_pattern (Pattern pattern);
        [CCode (cname = "GrImageFromContext")]
        public static Image from_context (Context context);

        [CCode (cname = "GrPatternFromImage")]
        public Pattern to_pattern ();
    }

    /* ================================================================== */
    /*               DRAWING IN USER WINDOW COORDINATES                   */
    /* ================================================================== */

    [CCode (cname = "GrSetUserWindow")]
    public void set_user_window (int x1, int y1, int x2, int y2);
    [CCode (cname = "GrGetUserWindow")]
    public void get_user_window (out int x1, out int y1, out int x2, out int y2);
    [CCode (cname = "GrGetScreenCoord")]
    public void get_screen_coord (out int x, out int y);
    [CCode (cname = "GrGetUserCoord")]
    public void get_user_coord (out int x, out int y);

    [CCode (cname = "GrUsrPlot")]
    public void user_plot (int x, int y, Color c);
    [CCode (cname = "GrUsrLine")]
    public void user_line (int x1, int y1, int x2, int y2, Color c);
    [CCode (cname = "GrUsrHLine")]
    public void user_horiz_line (int x1, int x2, int y, Color c);
    [CCode (cname = "GrUsrVLine")]
    public void user_vert_line (int x, int y1, int y2, Color c);
    [CCode (cname = "GrUsrBox")]
    public void user_box (int x1, int y1, int x2, int y2, Color c);
    [CCode (cname = "GrUsrFilledBox")]
    public void user_filled_box (int x1, int y1, int x2, int y2, Color c);
    [CCode (cname = "GrUsrFramedBox")]
    public void user_framed_box (int x1, int y1, int x2, int y2, int wdt, FramedBoxColors c);
    [CCode (cname = "GrUsrCircle")]
    public void user_circle (int xc, int yc, int r, Color c);
    [CCode (cname = "GrUsrEllipse")]
    public void user_ellipse (int xc, int yc, int xa, int ya, Color c);
    [CCode (cname = "GrUsrCircleArc")]
    public void user_circle_arc (int xc, int yc, int r, int start, int end, ArcStyle style, Color c);
    [CCode (cname = "GrUsrEllipseArc")]
    public void user_ellipse_arc (int xc, int yc, int xa, int ya, int start, int end, ArcStyle style, Color c);
    [CCode (cname = "GrUsrFilledCircle")]
    public void user_filled_circle (int xc, int yc, int r, Color c);
    [CCode (cname = "GrUsrFilledEllipse")]
    public void user_filled_ellipse (int xc, int yc, int xa, int ya, Color c);
    [CCode (cname = "GrUsrFilledCircleArc")]
    public void user_filled_circle_arc (int xc, int yc, int r, int start, int end, ArcStyle style, Color c);
    [CCode (cname = "GrUsrFilledEllipseArc")]
    public void user_filled_ellipse_arc (int xc, int yc, int xa, int ya, int start, int end, ArcStyle style, Color c);
    [CCode (cname = "GrUsrPolyLine")]
    public void useR_poly_line ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
    [CCode (cname = "GrUsrPolygon")]
    public void user_polygon ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
    [CCode (cname = "GrUsrFilledConvexPolygon")]
    public void user_filled_convex_polygon ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
    [CCode (cname = "GrUsrFilledPolygon")]
    public void user_filled_polygon ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
    [CCode (cname = "GrUsrFloodFill")]
    public void user_flood_fill (int x, int y, Color border, Color c);

    [CCode (cname = "GrUsrPixel")]
    public Color user_pixel (int x, int y);

    [CCode (cname = "GrUsrCustomLine")]
    public void user_custom_line (int x1, int y1, int x2, int y2, LineOption option);
    [CCode (cname = "GrUsrCustomBox")]
    public void user_custom_box (int x1, int y1, int x2, int y2, LineOption option);
    [CCode (cname = "GrUsrCustomCircle")]
    public void user_custom_circle (int xc, int yc, int r, LineOption option);
    [CCode (cname = "GrUsrCustomEllipse")]
    public void user_custom_ellipse (int xc, int yc, int xa, int ya, LineOption option);
    [CCode (cname = "GrUsrCustomCircleArc")]
    public void user_custom_circle_arc (int xc, int yc, int r, int start, int end, ArcStyle style, LineOption option);
    [CCode (cname = "GrUsrCustomEllipseArc")]
    public void user_custom_ellipse_arc (int xc, int yc, int xa, int ya, int start, int end, ArcStyle style, LineOption option);
    [CCode (cname = "GrUsrCustomPolyLine")]
    public void user_custom_poly_line ([CCode (array_length_pos = 0.9)]Point[] points, LineOption option);
    [CCode (cname = "GrUsrCustomPolygon")]
    public void user_custom_polygon ([CCode (array_length_pos = 0.9)]Point[] points, LineOption option);

    [CCode (cname = "GrUsrPatternedLine")]
    public void user_patterned_line (int x1, int y1, int x2, int y2, LinePattern pattern);
    [CCode (cname = "GrUsrPatternedBox")]
    public void user_patterned_box (int x1, int y1, int x2, int y2, LinePattern pattern);
    [CCode (cname = "GrUsrPatternedCircle")]
    public void user_patterned_circle (int xc, int yc, int r, LinePattern pattern);
    [CCode (cname = "GrUsrPatternedEllipse")]
    public void user_patterned_ellipse (int xc, int yc, int xa, int ya, LinePattern pattern);
    [CCode (cname = "GrUsrPatternedCircleArc")]
    public void user_patterned_circle_arc (int xc, int yc, int r, int start, int end, ArcStyle style, LinePattern pattern);
    [CCode (cname = "GrUsrPatternedEllipseArc")]
    public void user_patterned_ellipse_arc (int xc, int yc, int xa, int ya, int start, int end, ArcStyle style, LinePattern pattern);
    [CCode (cname = "GrUsrPatternedPolyLine")]
    public void user_patterned_poly_line ([CCode (array_length_pos = 0.9)]Point[] points, LinePattern pattern);
    [CCode (cname = "GrUsrPatternedPolygon")]
    public void user_patterned_polygon ([CCode (array_length_pos = 0.9)]Point[] points, LinePattern pattern);

    [CCode (cname = "GrUsrPatternFilledPlot")]
    public void user_pattern_filled_plot (int x, int y, Pattern pattern);
    [CCode (cname = "GrUsrPatternFilledLine")]
    public void user_pattern_filled_line (int x1, int y1, int x2, int y2, Pattern pattern);
    [CCode (cname = "GrUsrPatternFilledBox")]
    public void user_pattern_filled_box (int x1, int y1, int x2, int y2, Pattern pattern);
    [CCode (cname = "GrUsrPatternFilledCircle")]
    public void user_pattern_filled_circle (int xc, int yc, int r, Pattern pattern);
    [CCode (cname = "GrUsrPatternFilledEllipse")]
    public void user_pattern_filled_ellipse (int xc, int yc, int xa, int ya, Pattern pattern);
    [CCode (cname = "GrUsrPatternFilledCircleArc")]
    public void user_pattern_filled_circle_arc (int xc, int yc, int r, int start, int end, int style, Pattern pattern);
    [CCode (cname = "GrUsrPatternFilledEllipseArc")]
    public void user_pattern_filled_ellipse_arc (int xc, int yc, int xa, int ya, int start, int end, int style, Pattern pattern);
    [CCode (cname = "GrUsrPatternFilledConvexPolygon")]
    public void user_pattern_filled_convex_polygon ([CCode (array_length_pos = 0.9)]Point[] points, Pattern pattern);
    [CCode (cname = "GrUsrPatternFilledPolygon")]
    public void user_pattern_filled_polygon ([CCode (array_length_pos = 0.9)]Point[] points, Pattern pattern);
    [CCode (cname = "GrUsrPatternFloodFill")]
    public void user_pattern_flood_fill (int x, int y, Color border, Pattern pattern);

    [CCode (cname = "GrUsrDrawChar")]
    public void user_draw_char (int chr, int x, int y, TextOption opt);
    [CCode (cname = "GrUsrDrawString")]
    public void user_draw_string (char *text, int length, int x, int y, TextOption opt);
    public void user_draw_vala_string (string text, int x, int y, TextOption opt) {
        user_draw_string (text.data, text.length, x, y, opt);
    }
    [CCode (cname = "GrUsrTextXY")]
    public void user_text_xy (int x, int y, string text, Color fg, Color bg);

    /* ================================================================== */
    /*                    GRAPHICS CURSOR UTILITIES                       */
    /* ================================================================== */

    [CCode (cname = "struct _GR_cursor", free_function = "GrDestroyCursor", has_type_id = false)]
    [Compact]
    public class Cursor {

        [CCode (cname = "&self->work")]
        public static Context work;                    /* work areas (4) */
        public int xcord;                        /* cursor position on screen */
        public int ycord;
        public int xsize;                        /* cursor size */
        public int ysize;
        public int xoffs;                        /* LU corner to hot point offset */
        public int yoffs;
        public int xwork;                        /* save/work area sizes */
        public int ywork;
        public int xwpos;                        /* save/work area position on screen */
        public int ywpos;
        public int displayed;                          /* set if displayed */

        [CCode (cname = "GrBuildCursor")]
        public Cursor ([CCode (array_length = false)]char[] pixels, int pitch, int w, int h, int xo, int yo, ColorTable table);

        [CCode (cname = "GrDisplayCursor")]
        public void display ();
        [CCode (cname = "GrEraseCursor")]
        public void erase ();
        [CCode (cname = "GrMoveCursor")]
        public void move (int x, int y);
    }

    /* ================================================================== */
    /*                           PNM FUNCTIONS                            */
    /* ================================================================== */

    /**
     *  The PNM formats, grx support load/save of
     *  binaries formats (4, 5, 6) only
     */
    [CCode (cname = "int", has_type_id = false)]
    public enum PNMFormat {
        [CCode (cname = "PLAINPBMFORMAT")]
        PLAIN_PBM,
        [CCode (cname = "PLAINPGMFORMAT")]
        PLAIN_PGM,
        [CCode (cname = "PLAINPPMFORMAT")]
        PLAIN_PPM,
        [CCode (cname = "PBMFORMAT")]
        PBM,
        [CCode (cname = "PGMFORMAT")]
        PGM,
        [CCode (cname = "PPMFORMAT")]
        PPM
    }

    /* The PNM functions */

    [CCode (cname = "GrQueryPnm")]
    public Result query_pnm (string file_name, out int width, out int height, out int maxval);
    [CCode (cname = "GrQueryPnmBuffer")]
    public Result query_pnm_buffer (char *buffer, out int width, out int height, out int maxval);

    /* ================================================================== */
    /*                           PNG FUNCTIONS                            */
    /*  these functions may not be installed or available on all system   */
    /* ================================================================== */

    [CCode (cname = "GrPngSupport")]
    public bool png_support ();
    [CCode (cname = "GrQueryPng")]
    public Result query_png (string file_name, out int width, out int height);

    /* ================================================================== */
    /*                          JPEG FUNCTIONS                            */
    /*  these functions may not be installed or available on all system   */
    /* ================================================================== */

    [CCode (cname = "GrJpegSupport")]
    public bool jpeg_support ();
    [CCode (cname = "GrQueryJpeg")]
    public Result query_jpeg (string file_name, out int width, out int height);

    /* ================================================================== */
    /*               MISCELLANEOUS UTILITIY FUNCTIONS                     */
    /* ================================================================== */

    [CCode (cname = "GrResizeGrayMap")]
    public void resize_gray_map (uchar *map, int pitch, int old_width, int old_height, int new_width, int new_height);
    [CCode (cname = "GrMatchString")]
    public int match_string (string pattern, string str);
    [CCode (cname = "GrSetWindowTitle")]
    public void set_window_title (string title);
    [CCode (cname = "GrSleep")]
    public void sleep (int msec);
    [CCode (cname = "GrFlush")]
    public void flush ();
    [CCode (cname = "GrMsecTime")]
    public long msec_time ();
}
