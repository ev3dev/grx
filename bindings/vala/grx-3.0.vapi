/*
 * Copyright 2014-2016 David Lechner <david@lechnology.com>
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
        public Color value { get; }
        public ColorMode mode { get; }
        public ColorMode to_write_mode ();
        public ColorMode to_xor_mode ();
        public ColorMode to_or_mode ();
        public ColorMode to_and_mode ();
        public ColorMode to_image_mode ();

        public static Color black { [CCode (cname = "grx_color_info_get_black")]get; }
        public static Color white { [CCode (cname = "grx_color_info_get_white")]get; }

        public const Color VALUE_MASK;
        public const Color MODE_MASK;
        public const Color NONE;
    }

    namespace ColorInfo {
        public static void reset_colors ();
        public static void set_rgb_color_mode ();
        public static void refresh_colors ();

        public static int n_colors ();
        public static int n_free_colors ();

        public static Color build_rgb_color_t (int r, int g, int b);
        public static Color build_rgb_color_r (int r, int g, int b);
        public static int get_red_value (Color c);
        public static int get_green_value (Color c);
        public static int get_blue_value (Color c);

        public static Color alloc_color (int r, int g, int b);    /* shared, read-only */
        public static Color alloc_color_id (int r, int g, int b); /* potentially inlined version */
        public static Color alloc_color2 (long hcolor);           /* shared, read-only, 0xRRGGBB */
        public static Color alloc_color2_id (long hcolor);        /* potentially inlined version */
        public static Color alloc_cell ();                        /* unshared, read-write */

        [CCode (array_length_cexpr = "16")]
        public static Color[] alloc_ega_colors (); /* shared, read-only standard EGA colors */

        public static void set_color (Color c, int r, int g, int b);
        public static void free_color (Color c);
        public static void free_cell (Color c);

        public static void query_color (Color c, out int r, out int g, out int b);
        public static void query_color_id (Color c, out int r, out int g, out int b);
        public static void query_color2 (Color c, out long hcolor);
        public static void query_color2_id (Color c, out long hcolor);

        public static int get_save_buffer_size ();
        public static void save_colors (void *buffer);
        public static void restore_colors (void *buffer);
    }

    /**
     * Available video modes (for {@link set_mode})
     */
    [CCode (cprefix = "GRX_GRAPHICS_MODE_", has_type_id = false)]
    public enum GraphicsMode {
        UNKNOWN_MODE,
        TEXT_80X25,
        TEXT_DEFAULT,
        TEXT_WIDTH_HEIGHT,
        TEXT_WIDTH_HEIGHT_COLOR,
        TEXT_WIDTH_HEIGHT_BPP,
        GRAPHICS_DEFAULT,
        GRAPHICS_WIDHT_HEIGHT,
        GRAPHICS_WIDTH_HEIGHT_COLOR,
        GRAPHICS_WIDTH_HEIGHT_BPP,
        GRAPHICS_CUSTOM,
        GRAPHICS_CUSTOM_BPP,
        TEXT_80_25_NC,
        TEXT_DEFAULT_NC,
        TEXT_WIDTH_HEIGHT_NC,
        TEXT_WIDTH_HEIGHT_COLOR_NC,
        TEXT_WIDTH_HEIGHT_BPP_NC,
        GRAPHICS_DEFAULT_NC,
        GRAPHICS_WIDTH_HEIGHT_NC,
        GRAPHICS_WIDTH_HEIGHT_COLOR_NC,
        GRAPHICS_WIDTH_HEIGHT_BPP_NC,
        GRAPHICS_CUSTOM_NC,
        GRAPHICS_CUSTOM_BPP_NC
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

        public int get_n_planes ();
        public int get_line_offset (int width);
        public long get_plane_size (int width, int height);
        public long get_context_size (int width, int height);
    }

    /**
     * Supported video adapter types.
     */
    [CCode (cprefix = "GRX_VIDEO_ADAPTER_", has_type_id = false)]
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
        LINUX_FB,

        /**
         * SDL driver
         */
        SDL,

        /**
         * memory only driver
         */
        MEMORY
    }

    [CCode (cprefix = "GRX_VIDEO_DRIVER_FLAG_", has_type_id = false)]
    [Flags]
    public enum VideoDriverFlags {
        USER_RESOLUTION
    }

    [CCode (has_type_id = false)]
    public struct VideoDriver {
        public string name;
        public VideoAdapter adapter;
        public VideoDriver *inherit;
        [CCode (array_length_cname = "n_modes", array_length_type = "int")]
        public VideoMode[] modes;
        public VideoDriverDetect detect;
        public VideoDriverInit init;
        public VideoDriverReset reset;
        public VideoDriverSelectMode select_mode;
        public VideoDriverFlags flags;
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
    [CCode (has_type_id = false)]
    public struct VideoMode {
        public char present;
        public char bpp;
        public short width;
        public short height;
        public short mode;
        public int line_offset;
        public int user_data;
        public VideoModeExt *extended_info;
    }

    /**
     * Video mode flag bits (in the #GrxVideoModeExt structure)
     */
    [CCode (cprefix = "GRX_VIDEO_MODE_FLAG_", has_type_id = false)]
    [Flags]
    public enum VideoModeFlags {
        LINEAR,
        ACCEL,
        FAST_SVGA8 ,
        MEMORY,
    }

    /**
     * Video driver mode descriptor extension structure.
     *
     * This is a separate structure accessed via a pointer from the main mode
     * descriptor. The reason for this is that frequently several modes can
     * share the same extended info.
     */
    [CCode (has_type_id = false)]
    public struct VideoModeExt {
        public FrameMode mode;
        public FrameDriver *driver;
        [CCode (cname = "frame")]
        public char *fb_data;
        [CCode (cname = "cpos", array_length_cexpr = "3")]
        public char[] color_precision;
        [CCode (cname = "cprec", array_length_cexpr = "3")]
        public char[] color_position;
        public VideoModeFlags flags;
        public VideoModeSetup setup;
        public VideoModeSetSize set_virtual_size;
        public VideoModeScroll scroll;
        public VideoModeSetBank set_bank;
        public VideoModeSetRWBanks set_rw_banks;
        public VideoModeLoadColor load_color;
        public int lfb_selector;
    }

    [CCode (has_target = false, has_type_id = false)]
    public delegate bool VideoModeSetup (VideoMode mode, bool no_clear);
    [CCode (has_target = false, has_type_id = false)]
    public delegate bool VideoModeSetSize (VideoMode mode, uint width, uint height, out VideoMode result);
    [CCode (has_target = false, has_type_id = false)]
    public delegate bool VideoModeScroll (VideoMode mode, int x, int y, [CCode (array_length = false)]out int[] result);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void VideoModeSetBank (uint bank);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void VideoModeSetRWBanks (uint read_bank, uint write_bank);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void VideoModeLoadColor (Color color_index, Color red, Color green, Color blue);

    /**
     * The frame driver descriptor structure.
     */
    [CCode (has_type_id = false)]
    public struct FrameDriver {
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
    public delegate void FrameDriverDrawBitmap (int x, int y, int w, int h, uint8 bmp[], int pitch, int start, Color fg, Color bg);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void FrameDriverDrawPattern (int x, int y, int w, uint8 patt, Color fg, Color bg);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void FrameDriverBitBlt (Frame dest, int dx, int dy, Frame src, int x, int y, int w, int h, ColorMode op);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void FrameDriverBltV2R (Frame dest, int dx, int dy, Frame src, int x, int y, int w, int h, ColorMode op);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void FrameDriverBltR2V (Frame dest, int dx, int dy, Frame src, int x, int y, int w, int h, ColorMode op);
    [CCode (has_target = false, has_type_id = false)]
    public delegate Color *FrameDriverGetIndexedScanLine (Frame frame, int x, int y, int w, int index[]);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void FrameDriverPutScanLine (int x, int y, int w, Color scl[], ColorMode op);

    [CCode (cname = "struct _GR_driverInfo", has_type_id = false)]
    [Compact]
    public class DriverInfo {
        public VideoDriver *vdriver;
        [CCode (cname = "curmode")]
        public VideoMode *current_mode;
        [CCode (cname = "actmode")]
        public VideoMode actual_mode;
        public FrameDriver fdriver;
        public FrameDriver sdriver;
        public FrameDriver tdriver;
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
        [CCode (cname = "set_bank")]
        public DriverInfoSetBank set_bank;
        [CCode (cname = "set_rw_banks")]
        public DriverInfoSetRWBanks set_rw_banks;
    }

    [CCode (cname = "GrDriverInfo")]
    public DriverInfo driver_info;

    [CCode (has_target = false, has_type_id = false)]
    public delegate void DriverInfoSetBank (int bank);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void DriverInfoSetRWBanks (int read_bank, int write_bank);

    /*
     * setup stuff
     */
    public bool set_driver (string driver_spec);
    public bool set_mode (GraphicsMode mode, ...);
    public bool set_mode_default_graphics (bool clear);
    public void set_restore_mode (bool restore_flag);
    public void set_error_handling (bool exit_if_error);

    /*
     * inquiry stuff
     */
    public GraphicsMode get_current_graphics_mode ();
    public VideoAdapter get_adapter_type ();
    public FrameMode get_current_frame_mode ();
    public FrameMode get_screen_frame_mode ();
    public FrameMode get_core_frame_mode ();

    public unowned VideoDriver get_current_video_driver ();
    public unowned VideoMode get_current_video_mode ();
    public unowned VideoMode get_virtual_video_mode ();
    public unowned FrameDriver get_current_frame_driver ();
    public unowned FrameDriver get_screen_frame_driver ();
    public unowned VideoMode get_first_video_mode (FrameMode mode);
    public unowned VideoMode get_next_video_mode (VideoMode prev);

    /*
     * screen extents
     */
    public int get_screen_width ();
    public int get_screen_height ();
    public int get_virtual_width ();
    public int get_virtual_height ();
    public bool is_screen_virtual ();
    public bool set_viewport (int xpos, int ypos);
    public int get_viewport_x ();
    public int get_viewport_y ();

    /*
     * RAM context geometry and memory allocation inquiry stuff
     */
    public int get_n_planes ();
    public int get_line_offset (int width);
    public long get_plane_size (int width, int height);
    public long get_context_size (int width, int height);

    /* ================================================================== */
    /*              FRAME BUFFER, CONTEXT AND CLIPPING STUFF              */
    /* ================================================================== */

    public struct FrameMemory {
        public uint8 *plane0;
        public uint8 *plane1;
        public uint8 *plane2;
        public uint8 *plane3;
    }

    [CCode (has_type_id = false)]
    public struct Frame {
        public FrameMemory base_address;
        public short selector;
        public bool is_on_screen;
        public uint8 memory_flags;
        public int line_offset;
        public FrameDriver *driver;
    }

    [CCode (copy_function = "grx_context_ref", free_function = "grx_context_unref")]
    [Compact]
    public class Context {
        public unowned Context root;
        public int x_max;
        public int y_max;
        public int x_offset;
        public int y_offset;
        public int x_clip_low;
        public int y_clip_low;
        public int x_clip_high;
        public int y_clip_high;
        public int user_x_base;
        public int user_y_base;
        public int user_width;
        public int user_height;
        [CCode (cname = "frame.base_address", array_length_cexpr = "4")]
        public char*[] base_address;
        [CCode (cname = "frame.selector")]
        public short selector;
        [CCode (cname = "frame.is_on_screen")]
        public bool is_on_screen;
        [CCode (cname = "frame.memory_flags")]
        public uint8 flags;
        [CCode (cname = "frame.line_offset")]
        public int line_offset;
        [CCode (cname = "frame.driver")]
        public unowned FrameDriver *driver;

        public static Context? create (int width, int height, [CCode (array_length = false)]char*[]? memory = null, out Context? where = null);
        public static Context? create_full (FrameMode mode, int width, int height, [CCode (array_length = false)]char*[]? memory = null, out Context? where = null);
        public static Context? create_subcontext (int x1, int y1, int x2, int y2, Context parent, out Context? where = null);
        public static Context? save (out Context? where = null);

        public static unowned Context current { get; set; }
        public static unowned Context screen { get; }

        public void resize_subcontext (int x1, int y1, int x2, int y2);

        public void set_clip_box (int x1, int y1, int x2, int y2);
        public void get_clip_box (out int x1, out int y1, out int x2, out int y2);
        public void reset_clip_box ();

        public void clear (Color bg);

        public void flood_spill (int x1, int y1, int x2, int y2, Color old_c, Color new_c);
        public void flood_spill_2 (int x1, int y1, int x2, int y2, Color old_c1, Color new_c1, Color old_c2, Color new_c2);

        public Color get_pixel_at (int x, int y);

        public Color *get_scan_line (int x1, int x2, int yy);

        public Color fast_get_pixel_at (int x, int y);

        public Color get_pixel_at_user (int x, int y);

        public Result save_to_pbm (string file_name, string? comment = null);
        public Result save_to_pgm (string file_name, string? comment = null);
        public Result save_to_ppm (string file_name, string? comment = null);
        public Result load_from_pnm (string file_name);

        public Result load_from_pnm_data (char *buffer);

        public Result save_to_png (string file_name);
        public Result load_from_png(string file_name, bool use_alpha = true);

        public Result load_from_jpeg (string file_name, int scale);
        public Result save_to_jpeg (string file_name, int quality = 90);
        public Result save_to_jpeg_grayscale (string file_name, int quality = 90);
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

    public void set_clip_box (int x1, int y1, int x2, int y2);
    public void get_clip_box (out int x1, out int y1, out int x2, out int y2);
    public void reset_clip_box ();

    public int get_max_x ();
    public int get_max_y ();
    public int get_width ();
    public int get_height ();
    public int get_clip_box_min_x ();
    public int get_clip_box_min_y ();
    public int get_clip_box_max_x ();
    public int get_clip_box_max_y ();

    [CCode (cprefix = "GRX_COLOR_MODE_", has_type_id = false)]
    [Flags]
    public enum ColorMode {
        WRITE,
        XOR,
        OR,
        AND,
        IMAGE,
        UNDERLINE_TEXT
    }

    public const int MAX_POLYGON_POINTS;
    public const int MAX_ELLIPSE_POINTS;

    [CCode (cprefix = "GRX_ARC_STYLE_", has_type_id = false)]
    public enum ArcStyle {
        OPEN,
        CLOSE1,
        CLOSE2
    }

    [CCode (has_type_id = false)]
    public struct FramedBoxColors {
        public Color background;
        public Color border_top;
        public Color border_right;
        public Color border_bottom;
        public Color border_left;
    }

    /**
     * Color table (for primitives using several colors).
     *
     * It is an array of colors with the first element being the number of
     * colors in the table
     */
    [CCode (has_type_id = false)]
    public struct ColorTable {
        public uint size { [CCode (cname = "GRX_COLOR_TABLE_GET_SIZE")]get; }
        [Ccode (cname = "GRX_COLOR_TABLE_GET_COLOR")]
        public Color get (uint index);

        [CCode (cname = "GRX_COLOR_TABLE_GET_ALLOC_SIZE")]
        static int get_alloc_size (uint num_colors);
    }

    /* ================================================================== */
    /*                       GRAPHICS PRIMITIVES                          */
    /* ================================================================== */

    public void clear_screen (Color bg);
    public void clear_context (Color bg);
    public void clear_clip_box (Color bg);
    public void draw_pixel (int x, int y, Color c);
    public void draw_line (int x1, int y1, int x2, int y2, Color c);
    public void draw_hline (int x1, int x2, int y, Color c);
    public void draw_vline (int x, int y1, int y2, Color c);
    public void draw_box (int x1, int y1, int x2, int y2, Color c);
    public void draw_filled_box (int x1, int y1, int x2, int y2, Color c);
    public void draw_framed_box (int x1, int y1, int x2, int y2, int width, FramedBoxColors c);
    public void draw_rounded_box (int x1, int y1, int x2, int y2, int radius, Color c) {
        draw_circle_arc (x1 + radius, y1 + radius, radius, 900, 1800, ArcStyle.OPEN, c);
        draw_circle_arc (x2 - radius, y1 + radius, radius, 0, 900, ArcStyle.OPEN, c);
        draw_circle_arc (x1 + radius, y2 - radius, radius, 1800, 2700, ArcStyle.OPEN, c);
        draw_circle_arc (x2 - radius, y2 - radius, radius, 2700, 3600, ArcStyle.OPEN, c);
        draw_hline (x1 + radius, x2 - radius, y1, c);
        draw_hline (x1 + radius, x2 - radius, y2, c);
        draw_vline (x1, y1 + radius, y2 - radius, c);
        draw_vline (x2, y1 + radius, y2 - radius, c);
    }
    public void draw_filled_rounded_box (int x1, int y1, int x2, int y2, int radius, Color c) {
        draw_filled_circle_arc (x1 + radius, y1 + radius, radius, 900, 1800, ArcStyle.CLOSE2, c);
        draw_filled_circle_arc (x2 - radius, y1 + radius, radius, 0, 900, ArcStyle.CLOSE2, c);
        draw_filled_circle_arc (x1 + radius, y2 - radius, radius, 1800, 2700, ArcStyle.CLOSE2, c);
        draw_filled_circle_arc (x2 - radius, y2 - radius, radius, 2700, 3600, ArcStyle.CLOSE2, c);
        draw_filled_box (x1 + radius, y1, x2 - radius, y2, c);
        draw_filled_box (x1, y1 + radius, x2, y2 - radius, c);
    }
    [CCode (cname = "grx_generate_ellipse")]
    private int _generate_ellipse (int xc, int yc, int rx, int ry, [CCode (array_length_cexpr = "MAX_ELLIPSE_POINTS")]Point[] points);
    [CCode (cname = "vala_generate_ellipse")]
    public Point[] generate_ellipse (int xc, int yc, int rx, int ry) {
        Point[] points = new Point[MAX_ELLIPSE_POINTS];
        var length = _generate_ellipse (xc, yc, rx, ry, points);
        points.length = length;
        return (owned)points;
    }
    [CCode (cname = "grx_generate_ellipse_arc")]
    private int _generate_ellipse_arc (int xc, int yc, int rx, int ry, int start, int end, [CCode (array_length_cexpr = "MAX_ELLIPSE_POINTS")]Point[] points);
    [CCode (cname = "vala_generate_ellipse_arc")]
    public Point[] generate_ellipse_arc (int xc, int yc, int rx, int ry, int start, int end) {
        Point[] points = new Point[MAX_ELLIPSE_POINTS];
        var length = _generate_ellipse_arc (xc, yc, rx, ry, start, end, points);
        points.length = length;
        return (owned)points;
    }
    public void get_last_arc_coordinates (out int xs, out int ys, out int xe, out int ye, out int xc, out int yc);
    public void draw_circle (int xc, int yc, int r, Color c);
    public void draw_ellipse (int xc, int yc, int rx, int ry, Color c);
    public void draw_circle_arc (int xc, int yc, int r, int start, int end, ArcStyle style, Color c);
    public void draw_ellipse_arc (int xc, int yc, int rx, int ry, int start, int end, ArcStyle style, Color c);
    public void draw_filled_circle (int xc, int yc, int r, Color c);
    public void draw_filled_ellipse (int xc, int yc, int rx, int ry, Color c);
    public void draw_filled_circle_arc (int xc, int yc, int r, int start, int end, ArcStyle style, Color c);
    public void draw_filled_ellipse_arc (int xc, int yc, int rx, int ry, int start, int end, ArcStyle style, Color c);
    public void draw_poly_line ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
    public void draw_polygon ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
    public void draw_filled_convex_polygon ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
    public void draw_filled_polygon ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
    public void bit_blt (Context dest, int x, int y, Context src, int x1, int y1, int x2, int y2, ColorMode op = ColorMode.WRITE);
    public void bit_blt_1bpp (Context dest, int dx, int dy, Context src, int x1, int y1, int x2, int y2, Color fg, Color bg);
    public void flood_fill (int x, int y, Color border, Color c);
    public void flood_spill (int x1, int y1, int x2, int y2, Color old_c, Color new_c);
    public void flood_spill_2 (int x1,  int y1, int x2, int y2, Color old_c1, Color new_c1, Color old_c2, Color new_c2);

    public Color get_pixel_at (int x, int y);

    [CCode  (cname = "GrGetScanLine")]
    public Color *get_scan_line (int  x1, int x2, int yy);

    [CCode  (cname = "grx_put_scanline")]
    public void put_scan_line (int x1, int x2, int yy, [CCode (array_length = false)]Color[] c, ColorMode op = ColorMode.WRITE);

    /* ================================================================== */
    /*                 NON CLIPPING DRAWING PRIMITIVES                    */
    /* ================================================================== */

    public void fast_draw_pixel(int x, int y, Color c);
    public void fast_draw_line(int x1, int y1, int x2, int y2, Color c);
    public void fast_draw_hline(int x1, int x2, int y, Color c);
    public void fast_draw_vline(int x, int y1, int y2, Color c);
    public void fast_draw_box(int x1, int y1, int x2, int y2, Color c);
    public void fast_draw_filled_box(int x1, int y1, int x2, int y2, Color c);
    public void fast_draw_framed_box(int x1, int y1, int x2, int y2, int wdt, FramedBoxColors c);
    public void fast_bit_blt(Context dest, int x, int y, Context src, int x1, int y1, int x2, int y2, ColorMode op = ColorMode.WRITE);

    public Color fast_get_pixel_at (int x, int y);

    /* ================================================================== */
    /*                   FONTS AND TEXT PRIMITIVES                        */
    /* ================================================================== */

    [CCode (has_type_id = false)]
    public enum TextDirection {
        RIGHT,
        DOWN,
        LEFT,
        UP;

        [CCode (cname = "GRX_TEXT_DIRECTION_IS_VERTICAL")]
        public bool is_vertical ();
    }

    [CCode (cprefix = "GRX_TEXT_ALIGN_", has_type_id = false)]
    public enum TextAlignment {
        LEFT,
        CENTER,
        RIGHT
    }

    [CCode (cprefix = "GRX_TEXT_VALIGN_", has_type_id = false)]
    public enum TextVerticalAlignment {
        TOP,
        MIDDLE,
        BOTTOM,
        BASELINE
    }

    [CCode (has_type_id = false)]
    public enum CharType {
        BYTE,
        WORD,
        ATTR;

        [CCode (cname = "GRX_CHAR_TYPE_GET_SIZE")]
        public int get_size ();
        [CCode (cname = "GRX_CHAR_TYPE_GET_CODE")]
        public uint get_code (int ch);
        [CCode (cname = "GRX_CHAR_TYPE_GET_ATTR")]
        public CharAttr get_attr (int ch);
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

    [CCode (has_type_id = false)]
    public struct FontCharInfo {
        public uint width;
        public uint offset;
    }

    [CCode (copy_function = "grx_font_ref", free_function = "grx_font_unref")]
    [Compact]
    public class Font {
        [CCode (cname = "&grx_font_pc6x8")]
        public static unowned Font pc6x8;
        [CCode (cname = "&grx_font_pc8x8")]
        public static unowned Font pc8x8;
        [CCode (cname = "&grx_font_pc8x14")]
        public static unowned Font pc8x14;
        [CCode (cname = "&grx_font_pc8x16")]
        public static unowned Font pc8x16;
        [CCode (cname = "&grx_font_default")]
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
        public FontCharInfo[] chrinfo;

        public static Font? load (string name);
        public static Font? load_converted (string name, FontConversionFlag flags, int w = 0, int h = 0, int min_ch = 0, int max_ch = 0);
        public Font build_converted (FontConversionFlag flags, int w = 0, int h = 0, int min_ch = 0, int max_ch = 0);

        public void dump (string c_symbol_name, string file_name);
        public void dump_fna (string file_name);
        public static void set_path (string path_list);

        public int is_char_present (int chr);
        public int get_char_width (int chr);
        public int get_char_height (int chr);
        public int get_char_bmp_row_size (int chr);
        public int get_char_bmp_size (int chr);
        [CCode (cname = "grx_font_get_string_width")]
        int internal_get_string_width (void *text, int len, CharType type = CharType.BYTE);
        [CCode (cname = "vala_grx_font_get_string_width")]
        public int get_string_width (string text) {
            return internal_get_string_width (text.data, text.length);
        }
        [CCode (cname = "grx_font_get_string_height")]
        int internal_get_string_height (void *text, int len, CharType type = CharType.BYTE);
        [CCode (cname = "vala_grx_font_get_string_height")]
        public int get_string_height (string text) {
            return internal_get_string_height (text.data, text.length);
        }
        [CCode (cname = "grx_font_get_proportional_text_width")]
        int internal_get_proportional_text_width (void *text, int len, CharType type = CharType.BYTE);
        [CCode (cname = "vala_grx_font_get_proportional_text_width")]
        public int get_proportional_text_width (string text) {
            return internal_get_proportional_text_width (text.data, text.length);
        }

        public uint8 *build_auxiliary_bitmap (int chr, TextDirection dir, bool ul);
        public uint8 *get_char_bitmap (int chr);
        public uint8 *get_char_aux_bitmap (int chr, TextDirection dir, bool ul);
    }

    [CCode (has_type_id = false)]
    [SimpleType]
    public struct TextColor : uint32 {
        [CCode (cname = "v")]
        public Color as_color;
        [CCode (cname = "p")]
        unowned ColorTable as_color_table;
    }

    [CCode (has_type_id = false)]
    public struct TextOptions {
        [CCode (cname = "txo_font")]
        public unowned Font font;
        [CCode (cname = "txo_fgcolor")]
        public unowned TextColor fg_color;
        [CCode (cname = "txo_bgcolor")]
        public unowned TextColor bg_color;
        [CCode (cname = "txo_chrtype")]
        public CharType chr_type;
        [CCode (cname = "txo_direct")]
        public TextDirection direction;
        [CCode (cname = "txo_xalign")]
        public TextAlignment x_align;
        [CCode (cname = "txo_yalign")]
        public TextVerticalAlignment y_align;

        public int get_char_width (int chr);
        public int get_char_height (int chr);
        public void get_char_size (int chr, out int width, out int height);
        [CCode (cname = "grx_text_option_get_string_width")]
        int internal_string_width (void *text, int length);
        [CCode (cname = "vala_grx_text_option_get_string_width")]
        public int get_string_width (string test) {
            return internal_string_width (test.data, test.length);
        }
        [CCode (cname = "grx_text_option_get_string_height")]
        int internal_get_string_height (void *text, int length);
        [CCode (cname = "vala_grx_text_option_get_string_height")]
        public int get_string_height (string text) {
            return internal_get_string_height (text.data, text.length);
        }
        [CCode (cname = "grx_text_option_get_string_size")]
        void internal_get_string_size (void *text, int length, out int w, out int h);
        [CCode (cname = "vala_grx_text_option_get_string_size")]
        public void get_string_size (string text, out int w, out int h) {
            internal_get_string_size (text.data, text.length, out w, out h);
        }
    }

    [CCode (has_type_id = false)]
    public struct TextRegion {
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
        public CharType chr_type;

        public void dump_char (int chr, int col, int row);
        public void dump_text (int col, int row, int width, int height);
        public void dump ();
    }

    public void draw_char_with_text_options (int chr, int x, int y, TextOptions opt);
    [CCode (cname = "grx_draw_string_with_text_options")]
    void internal_draw_string_with_text_options (void *text, int length, int x, int y, TextOptions opt);
    [CCode (cname = "vala_grx_draw_string_with_text_options")]
    public void draw_string_with_text_options (string text, int x, int y, TextOptions opt) {
        internal_draw_string_with_text_options (text.data, text.length, x, y, opt);
    }
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
    [CCode (has_type_id = false)]
    public struct LineOptions {
        /**
         * Color used to draw line.
         */
        public Color color;

        /**
         * Width of the line.
         */
        public int width;

        /**
         * Draw/no-draw pattern.
         */
        [CCode (array_length_cname = "n_dash_patterns")]
        public uchar[] dash_patterns;
    }

    [CCode (has_type_id = false)]
    public struct Point {
        int x;
        int y;
    }

    public void draw_line_with_options(int x1, int y1, int x2, int y2, LineOptions o);
    public void draw_box_with_options(int x1, int y1, int x2, int y2, LineOptions o);
    public void draw_circle_with_options(int xc, int yc, int r, LineOptions o);
    public void draw_ellipse_with_options(int xc, int yc, int rx, int ry, LineOptions o);
    public void draw_circle_arc_with_options(int xc, int yc, int r, int start, int end, ArcStyle style, LineOptions o);
    public void draw_ellipse_arc_with_options(int xc, int yc, int rx, int ry, int start, int end, ArcStyle style, LineOptions o);
    public void draw_polyline_with_options([CCode (array_length_pos = 0.9)]Point[] points, LineOptions o);
    public void draw_polygon_with_options([CCode (array_length_pos = 0.9)]Point[] points, LineOptions o);

    /* ================================================================== */
    /*             PATTERNED DRAWING AND FILLING PRIMITIVES               */
    /* ================================================================== */

    /**
     * A mode independent way to specify a fill pattern of two colors.
     *
     * It is always 8 pixels wide (1 byte per scan line), its height is
     * user-defined. SET THE TYPE FLAG TO ZERO!!!
     */
    [CCode (has_type_id = false)]
    public struct Bitmap {
        /**
         * Type flag for pattern union.
         */
        public bool is_pixmap;

        /**
         * Bitmap height
         */
        public int height;

        /**
         * Pointer to the bit pattern
         */
        public char *data;

        /**
         * Foreground color for fill
         */
        public Color fg_color;

        /**
         * background color for fill
         */
        public Color bg_color;

        /**
         * Set if dynamically allocated
         */
        public bool free_on_pattern_destroy;
    }

    /**
     * A fill pattern stored in a layout identical to the video RAM for filling
     * using 'bitblt'-s.
     *
     * It is mode dependent, typically one of the library functions is used to
     * build it. KEEP THE TYPE FLAG NONZERO!!!
     */
    [CCode (has_type_id = false)]
    public struct Pixmap {
        /**
         * type flag for pattern union
         */
        public bool is_pixmap;

        /**
         * pixmap width (in pixels)
         */
        public int width;

        /**
         * pixmap height (in pixels)
         */
        public int height;

        /**
         * bitblt mode (SET, OR, XOR, AND, IMAGE)
         */
        public ColorMode mode;

        /**
         * source context for fill
         */
        public Frame source;
    }

    /**
     * Fill pattern union -- can either be a bitmap or a pixmap
     */
    [CCode (copy_function = "grx_pattern_copy", free_function = "grx_pattern_free", has_type_id = false)]
    [Compact]
    public class Pattern {
        public bool is_pixmap;               /* true for pixmaps */

        public static Pattern new_pixmap ([CCode (array_length = false)]char *pixels, int w, int h, ColorTable colors);
        public static Pattern new_pixmap_from_bits ([CCode (array_length = false)]char *bits, int w, int h, Color fgc, Color bgc);
        public static Pattern new_pixmap_from_context (GrxContext context);
    }

    /**
     * Draw pattern for line drawings.
     *
     * Specifies both the:
     * 1. Fill pattern, and the
     * 2. Custom line drawing option
     */
    [CCode (has_type_id = false)]
    public struct LinePattern {
        /**
         * Fill pattern
         */
        Pattern *pattern;

        /**
         * width + dash pattern
         */
        LinePattern *options;
    }

    public void draw_line_with_pattern (int x1, int y1, int x2, int y2, LinePattern line_pattern);
    public void draw_box_with_pattern (int x1, int y1, int x2, int y2, LinePattern line_pattern);
    public void draw_circle_with_pattern (int xc, int yc, int r, LinePattern line_pattern);
    public void draw_ellipse_with_pattern (int xc, int yc, int rx, int ry, LinePattern line_pattern);
    public void draw_circle_arc_with_pattern (int xc, int yc, int r, int start, int end, ArcStyle style, LinePattern line_pattern);
    public void draw_ellipse_arc_with_pattern (int xc, int yc, int rx, int ry, int start, int end, ArcStyle style, LinePattern line_pattern);
    public void draw_polyline_with_pattern ([CCode (array_length_pos = 0.9)]Point[] points, LinePattern line_pattern);
    public void draw_polygon_with_pattern ([CCode (array_length_pos = 0.9)]Point[] points, LinePattern line_pattern);

    public void draw_filled_pixel_with_pattern (int x, int y, Pattern pattern);
    public void draw_filled_line_with_pattern (int x1, int y1, int x2, int y2, Pattern pattern);
    public void draw_filled_box_with_pattern (int x1, int y1, int x2, int y2, Pattern pattern);
    public void draw_filled_circle_with_pattern (int xc, int yc, int r, Pattern pattern);
    public void draw_filled_ellipse_with_pattern (int xc, int yc, int rx, int ry, Pattern pattern);
    public void draw_filled_circle_arc_with_pattern (int xc, int yc, int r, int start, int end, int style, Pattern pattern);
    public void draw_filled_ellipse_arc_with_pattern (int xc, int yc, int rx, int ry, int start, int end, int style, Pattern pattern);
    public void draw_filled_convex_polygon_with_pattern ([CCode (array_length_pos = 0.9)]Point[] points, Pattern pattern);
    public void draw_filled_polygon_with_pattern ([CCode (array_length_pos = 0.9)]Point[] points, Pattern pattern);
    public void flood_fill_with_pattern (int x, int y, Color border, Pattern pattern);

    public void draw_char_with_pattern (int chr, int x, int y, TextOptions opt, Pattern pattern);
    [CCode (cname = "grx_draw_string_with_pattern")]
    void internal_draw_string_with_pattern (void *text, int length, int x, int y, TextOptions opt, Pattern pattern);
    [CCode (cname = "vala_grx_draw_string_with_pattern")]

    public void draw_string_with_pattern (string text, int x, int y, TextOptions opt, Pattern pattern) {
        internal_draw_string_with_pattern (text.data, text.length, x, y, opt, pattern);
    }
    [CCode (cname = "grx_draw_string_with_pattern_ext")]
    void internal_draw_string_with_pattern_ext (void *text, int length, int x, int y, TextOptions opt, Pattern pattern);
    [CCode (cname = "vala_grx_draw_string_with_pattern_ext")]
    public void draw_string_with_pattern_ext (string text, int x, int y, TextOptions opt, Pattern pattern) {
        internal_draw_string_with_pattern_ext (text.data, text.length, x, y, opt, pattern);
    }

    /* ================================================================== */
    /*                      IMAGE MANIPULATION                            */
    /* ================================================================== */


    /**
     * Flags for {@link Image.mirror}.
     */
    [CCode (cprefix = "GRX_IMAGE_MIRROR_", has_type_id = false)]
    [Flags]
    public enum ImageMirrorFlags {
        HORIZONTAL, /* inverse left right */
        VERTICAL /* inverse top down */
    }

    [CCode (copy_function = "grx_image_copy", free_function = "grx_image_free", has_type_id = false)]
    [Compact]
    public class Image {
        Pattern pattern;         /* fill pattern */
        LineOptions options;          /* width + dash pattern */

        public static Image create ([CCode (array_length = false)]char[] pixels, int w, int h, ColorTable colors);
        public static Image create_from_data ([CCode (array_length = false)]char[] pixels, int w, int h, ColorTable colors);

        public Image mirror (ImageMirrorFlags flags);
        public Image stretch (int width, int height);

        public static Image create_from_pattern (Pattern pattern);
        public static Image create_from_context (Context context);

        [CCode (cname = "grx_pattern_new_from_image")]
        public Pattern to_pattern ();
    }

    public void draw_image (int x, int y);
    public void draw_image_tiled (int x1, int y1, int x2, int y2);
    public void draw_filled_box_with_image (int xo, int yo, int x1, int y1, int x2, int y2);
    public void draw_hline_with_image (int xo, int yo, int x, int y, int width);
    public void draw_pixel_with_image (int xo, int yo, int x, int y);

    /* ================================================================== */
    /*               DRAWING IN USER WINDOW COORDINATES                   */
    /* ================================================================== */

    namespace User {

        public void set_window (int x1, int y1, int x2, int y2);
        public void get_window (out int x1, out int y1, out int x2, out int y2);
        public void convert_user_to_screen (ref int x, ref int y);
        public void convert_screen_to_user (ref int x, ref int y);

        public void draw_pixel (int x, int y, Color c);
        public void draw_line (int x1, int y1, int x2, int y2, Color c);
        public void draw_hline (int x1, int x2, int y, Color c);
        public void draw_vline (int x, int y1, int y2, Color c);
        public void draw_box (int x1, int y1, int x2, int y2, Color c);
        public void draw_filled_box (int x1, int y1, int x2, int y2, Color c);
        public void draw_framed_box (int x1, int y1, int x2, int y2, int wdt, FramedBoxColors c);
        public void draw_circle (int xc, int yc, int r, Color c);
        public void draw_ellipse (int xc, int yc, int rx, int ry, Color c);
        public void draw_circle_arc (int xc, int yc, int r, int start, int end, ArcStyle style, Color c);
        public void draw_ellipse_arc (int xc, int yc, int rx, int ry, int start, int end, ArcStyle style, Color c);
        public void draw_filled_circle (int xc, int yc, int r, Color c);
        public void draw_filled_ellipse (int xc, int yc, int rx, int ry, Color c);
        public void draw_filled_circle_arc (int xc, int yc, int r, int start, int end, ArcStyle style, Color c);
        public void draw_filled_ellipse_arc (int xc, int yc, int rx, int ry, int start, int end, ArcStyle style, Color c);
        public void draw_polyline ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
        public void draw_polygon ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
        public void draw_filled_convex_polygon ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
        public void draw_filled_polygon ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
        public void flood_fill (int x, int y, Color border, Color c);
        public void bit_blt (int x, int y, Context src, int x1, int y1, int x2, int y2, Color oper);
        public void bit_blt_1bpp (int x, int y, Context src, int x1, int y1, int x2, int y2, Color fg, Color bg);

        public Color get_pixel_at (int x, int y);

        public void draw_line_with_options (int x1, int y1, int x2, int y2, LineOptions options);
        public void draw_box_with_options (int x1, int y1, int x2, int y2, LineOptions options);
        public void draw_circle_with_options (int xc, int yc, int r, LineOptions options);
        public void draw_ellipse_with_options (int xc, int yc, int rx, int ry, LineOptions options);
        public void draw_circle_arc_with_options (int xc, int yc, int r, int start, int end, ArcStyle style, LineOptions options);
        public void draw_ellipse_arc_with_options (int xc, int yc, int rx, int ry, int start, int end, ArcStyle style, LineOptions options);
        public void draw_polyline_with_options ([CCode (array_length_pos = 0.9)]Point[] points, LineOptions options);
        public void draw_polygon_with_options ([CCode (array_length_pos = 0.9)]Point[] points, LineOptions options);

        public void draw_line_with_pattern (int x1, int y1, int x2, int y2, LinePattern pattern);
        public void draw_box_with_pattern (int x1, int y1, int x2, int y2, LinePattern pattern);
        public void draw_circle_with_pattern (int xc, int yc, int r, LinePattern pattern);
        public void draw_ellipse_with_pattern (int xc, int yc, int rx, int ry, LinePattern pattern);
        public void draw_circle_arc_with_pattern (int xc, int yc, int r, int start, int end, ArcStyle style, LinePattern pattern);
        public void draw_ellipse_arc_with_pattern (int xc, int yc, int rx, int ry, int start, int end, ArcStyle style, LinePattern pattern);
        public void draw_polyline_with_pattern ([CCode (array_length_pos = 0.9)]Point[] points, LinePattern pattern);
        public void draw_polygon_with_pattern ([CCode (array_length_pos = 0.9)]Point[] points, LinePattern pattern);

        public void draw_filled_pixel_with_pattern (int x, int y, Pattern pattern);
        public void draw_filled_line_with_pattern (int x1, int y1, int x2, int y2, Pattern pattern);
        public void draw_filled_box_with_pattern (int x1, int y1, int x2, int y2, Pattern pattern);
        public void draw_filled_circle_with_pattern (int xc, int yc, int r, Pattern pattern);
        public void draw_filled_ellipse_with_pattern (int xc, int yc, int rx, int ry, Pattern pattern);
        public void draw_filled_circle_arc_with_pattern (int xc, int yc, int r, int start, int end, int style, Pattern pattern);
        public void draw_filled_ellipse_arc_with_pattern (int xc, int yc, int rx, int ry, int start, int end, int style, Pattern pattern);
        public void draw_filled_convex_polygon_with_pattern ([CCode (array_length_pos = 0.9)]Point[] points, Pattern pattern);
        public void draw_filled_polygon_with_pattern ([CCode (array_length_pos = 0.9)]Point[] points, Pattern pattern);
        public void flood_fill_with_pattern (int x, int y, Color border, Pattern pattern);

        public void draw_char_with_text_options (int chr, int x, int y, TextOptions opt);
        [CCode (cname = "grx_user_draw_text_with_text_options")]
        void _draw_text_with_text_options (char *text, int length, int x, int y, TextOptions opt);
        [CCode (cname = "vala_grx_user_draw_text_with_text_options")]
        public void draw_text_with_text_options (string text, int x, int y, TextOptions opt) {
            _draw_text_with_text_options (text.data, text.length, x, y, opt);
        }
        public void draw_text (string text, int x, int y, Color fg, Color bg);
    }

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
    [CCode (has_type_id = false)]
    public enum PnmFormat {
        ASCII_PBM,
        ASCII_PGM,
        ASCII_PPM,
        BINARY_PBM,
        BINARY_PGM,
        BINARY_PPM
    }

    /* The PNM functions */

    public PnmFormat check_pnm_file (string file_name, out int width, out int height, out int maxval);
    public PnmFormat check_pnm_data (char *buffer, out int width, out int height, out int maxval);

    /* ================================================================== */
    /*                           PNG FUNCTIONS                            */
    /*  these functions may not be installed or available on all system   */
    /* ================================================================== */

    public bool is_png_supported ();
    public Result check_png_file (string file_name, out int width, out int height);

    /* ================================================================== */
    /*                          JPEG FUNCTIONS                            */
    /*  these functions may not be installed or available on all system   */
    /* ================================================================== */

    [CCode (cname = "grx_is_jpeg_supported")]
    public bool jpeg_support ();
    [CCode (cname = "grx_check_jpeg_file")]
    public Result query_jpeg (string file_name, out int width, out int height);

    /* ================================================================== */
    /*               MISCELLANEOUS UTILITIY FUNCTIONS                     */
    /* ================================================================== */

    public void resize_gray_map (uchar *map, int pitch, int old_width, int old_height, int new_width, int new_height);

    public class LinuxConsoleApplication : GLib.Application, GLib.Initable {
        public LinuxConsoleApplication.new (GLib.Cancellable? cancellable = null) throws GLib.Error;
        public bool init (GLib.Cancellable? cancellable = null) throws GLib.Error;
        public bool is_console_active { [CCode (cname = "grx_linux_console_application_is_console_active")]get; }
    }
}
