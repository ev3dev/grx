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
    public errordomain Error {
        FAILED,
        PLUGIN_FILE_NOT_FOUND,
        PLUGIN_SYMBOL_NOT_FOUND,
    }

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
        public Color to_write_mode ();
        public Color to_xor_mode ();
        public Color to_or_mode ();
        public Color to_and_mode ();
        public Color to_image_mode ();

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

        public static Color build_rgb_color (uchar r, uchar g, uchar b);
        public static Color build_rgb_color_round (uchar r, uchar g, uchar b);
        public static uchar get_red_value (Color c);
        public static uchar get_green_value (Color c);
        public static uchar get_blue_value (Color c);

        public static Color alloc_color (uchar r, uchar g, uchar b);
        public static Color alloc_color_inline (uchar r, uchar g, uchar b);
        public static Color alloc_color2 (uint hcolor);
        public static Color alloc_color2_inline (uint hcolor);
        public static void free_color (Color c);

        public static Color alloc_cell ();
        public static void set_cell (Color c, uchar r, uchar g, uchar b);
        public static void free_cell (Color c);

        [CCode (array_length_cexpr = "16")]
        public static Color[] alloc_ega_colors ();

        public static void query_color (Color c, out uchar r, out uchar g, out uchar b);
        public static void query_color_inline (Color c, out uchar r, out uchar g, out uchar b);
        public static void query_color2 (Color c, out uint hcolor);
        public static void query_color2_inline (Color c, out uint hcolor);

        public static int get_save_buffer_size ();
        public static void save_colors (void *buffer);
        public static void restore_colors (void *buffer);
    }

    [CCode (has_type_id = false)]
    public enum EgaColorIndex {
        BLACK,
        BLUE,
        GREEN,
        CYAN,
        RED,
        MAGENTA,
        BROWN,
        LIGHT_GRAY,
        DARK_GRAY,
        LIGHT_BLUE,
        LIGHT_GREEN,
        LIGHT_CYAN,
        LIGHT_RED,
        LIGHT_MAGENTA,
        YELLOW,
        WHITE,
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

        public static FrameMode get_current ();
        public static FrameMode get_screen ();
        public static FrameMode get_screen_core ();
        public int get_n_planes ();
        public int get_line_offset (int width);
        public long get_plane_size (int width, int height);
        public long get_context_size (int width, int height);
    }

    [CCode (cprefix = "GRX_VIDEO_DRIVER_FLAG_", has_type_id = false)]
    [Flags]
    public enum VideoDriverFlags {
        USER_RESOLUTION
    }

    [CCode (has_type_id = false)]
    public struct VideoDriver {
        public string name;
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

    [CCode (has_target = false, has_type_id = false)]
    public delegate void DriverInfoSetBank (int bank);
    [CCode (has_target = false, has_type_id = false)]
    public delegate void DriverInfoSetRWBanks (int read_bank, int write_bank);

    /*
     * setup stuff
     */
    public bool set_driver (string driver_spec) throws Grx.Error;
    public bool set_mode (GraphicsMode mode, ...) throws Grx.Error;
    public bool set_mode_default_graphics (bool clear) throws Grx.Error;

    /*
     * inquiry stuff
     */
    public GraphicsMode get_mode ();

    public unowned VideoDriver get_current_video_driver ();
    public unowned DeviceManager get_device_manager ();
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
    public int screen_get_n_planes ();
    public int screen_get_line_offset (int width);
    public long screen_get_plane_size (int width, int height);
    public long screen_get_context_size (int width, int height);

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

        public static unowned Context current {
            [CCode (cname = "grx_get_current_context")]get;
            [CCode (cname = "grx_set_current_context")]set;
        }
        public static unowned Context screen {
            [CCode (cname = "grx_get_screen_context")]get;
        }

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

    [CCode (cprefix = "GRX_TEXT_HALIGN_", has_type_id = false)]
    public enum TextHAlign {
        LEFT,
        CENTER,
        RIGHT
    }

    [CCode (cprefix = "GRX_TEXT_VALIGN_", has_type_id = false)]
    public enum TextVAlign {
        TOP,
        MIDDLE,
        BOTTOM,
        BASELINE
    }

    [CCode (copy_function = "grx_font_ref", free_function = "grx_font_unref")]
    [Compact]
    public class Font {
        public static Font load_from_file(string filename) throws GLib.Error;
        [CCode (cname = "grx_font_load_full")]
        public static Font load(string? family = null, int size = -1, string? style = null, string? lang = null, int dpi = -1) throws GLib.Error;
        public unowned string family { get; }
        public unowned string style { get; }
        public int dump(Context context, int start, Color fg, Color bg);
        public int get_char_width(uint32 c);
        public int get_char_height(uint32 c);
        public int get_text_width(string? text);
        public int get_text_height(string? text);
    }

    public void draw_text (string text, int x, int y, Font font, Color fg, Color bg, TextHAlign hAlign, TextVAlign vAlign);

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
        public static Pattern new_pixmap_from_context (Context context);

        public unowned Image as_image ();
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
        public static Image new ([CCode (array_length = false)]char[] pixels, int w, int h, ColorTable colors);
        public static Image new_from_context (Context context);

        public Image mirror (ImageMirrorFlags flags);
        public Image stretch (int width, int height);

        public unowned Pattern as_pattern ();
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

    /* ================================================================== */
    /*                              INPUT                                 */
    /* ================================================================== */

    namespace Key {
        public const int @0;
        public const int @1;
        public const int @2;
        public const int @3;
        public const int @3270_AltCursor;
        public const int @3270_Attn;
        public const int @3270_BackTab;
        public const int @3270_ChangeScreen;
        public const int @3270_Copy;
        public const int @3270_CursorBlink;
        public const int @3270_CursorSelect;
        public const int @3270_DeleteWord;
        public const int @3270_Duplicate;
        public const int @3270_Enter;
        public const int @3270_EraseEOF;
        public const int @3270_EraseInput;
        public const int @3270_ExSelect;
        public const int @3270_FieldMark;
        public const int @3270_Ident;
        public const int @3270_Jump;
        public const int @3270_KeyClick;
        public const int @3270_Left2;
        public const int @3270_PA1;
        public const int @3270_PA2;
        public const int @3270_PA3;
        public const int @3270_Play;
        public const int @3270_PrintScreen;
        public const int @3270_Quit;
        public const int @3270_Record;
        public const int @3270_Reset;
        public const int @3270_Right2;
        public const int @3270_Rule;
        public const int @3270_Setup;
        public const int @3270_Test;
        public const int @4;
        public const int @5;
        public const int @6;
        public const int @7;
        public const int @8;
        public const int @9;
        public const int A;
        public const int AE;
        public const int Aacute;
        public const int Abelowdot;
        public const int Abreve;
        public const int Abreveacute;
        public const int Abrevebelowdot;
        public const int Abrevegrave;
        public const int Abrevehook;
        public const int Abrevetilde;
        public const int AccessX_Enable;
        public const int AccessX_Feedback_Enable;
        public const int Acircumflex;
        public const int Acircumflexacute;
        public const int Acircumflexbelowdot;
        public const int Acircumflexgrave;
        public const int Acircumflexhook;
        public const int Acircumflextilde;
        public const int AddFavorite;
        public const int Adiaeresis;
        public const int Agrave;
        public const int Ahook;
        public const int Alt_L;
        public const int Alt_R;
        public const int Amacron;
        public const int Aogonek;
        public const int ApplicationLeft;
        public const int ApplicationRight;
        public const int Arabic_0;
        public const int Arabic_1;
        public const int Arabic_2;
        public const int Arabic_3;
        public const int Arabic_4;
        public const int Arabic_5;
        public const int Arabic_6;
        public const int Arabic_7;
        public const int Arabic_8;
        public const int Arabic_9;
        public const int Arabic_ain;
        public const int Arabic_alef;
        public const int Arabic_alefmaksura;
        public const int Arabic_beh;
        public const int Arabic_comma;
        public const int Arabic_dad;
        public const int Arabic_dal;
        public const int Arabic_damma;
        public const int Arabic_dammatan;
        public const int Arabic_ddal;
        public const int Arabic_farsi_yeh;
        public const int Arabic_fatha;
        public const int Arabic_fathatan;
        public const int Arabic_feh;
        public const int Arabic_fullstop;
        public const int Arabic_gaf;
        public const int Arabic_ghain;
        public const int Arabic_ha;
        public const int Arabic_hah;
        public const int Arabic_hamza;
        public const int Arabic_hamza_above;
        public const int Arabic_hamza_below;
        public const int Arabic_hamzaonalef;
        public const int Arabic_hamzaonwaw;
        public const int Arabic_hamzaonyeh;
        public const int Arabic_hamzaunderalef;
        public const int Arabic_heh;
        public const int Arabic_heh_doachashmee;
        public const int Arabic_heh_goal;
        public const int Arabic_jeem;
        public const int Arabic_jeh;
        public const int Arabic_kaf;
        public const int Arabic_kasra;
        public const int Arabic_kasratan;
        public const int Arabic_keheh;
        public const int Arabic_khah;
        public const int Arabic_lam;
        public const int Arabic_madda_above;
        public const int Arabic_maddaonalef;
        public const int Arabic_meem;
        public const int Arabic_noon;
        public const int Arabic_noon_ghunna;
        public const int Arabic_peh;
        public const int Arabic_percent;
        public const int Arabic_qaf;
        public const int Arabic_question_mark;
        public const int Arabic_ra;
        public const int Arabic_rreh;
        public const int Arabic_sad;
        public const int Arabic_seen;
        public const int Arabic_semicolon;
        public const int Arabic_shadda;
        public const int Arabic_sheen;
        public const int Arabic_sukun;
        public const int Arabic_superscript_alef;
        public const int Arabic_switch;
        public const int Arabic_tah;
        public const int Arabic_tatweel;
        public const int Arabic_tcheh;
        public const int Arabic_teh;
        public const int Arabic_tehmarbuta;
        public const int Arabic_thal;
        public const int Arabic_theh;
        public const int Arabic_tteh;
        public const int Arabic_veh;
        public const int Arabic_waw;
        public const int Arabic_yeh;
        public const int Arabic_yeh_baree;
        public const int Arabic_zah;
        public const int Arabic_zain;
        public const int Aring;
        public const int Armenian_AT;
        public const int Armenian_AYB;
        public const int Armenian_BEN;
        public const int Armenian_CHA;
        public const int Armenian_DA;
        public const int Armenian_DZA;
        public const int Armenian_E;
        public const int Armenian_FE;
        public const int Armenian_GHAT;
        public const int Armenian_GIM;
        public const int Armenian_HI;
        public const int Armenian_HO;
        public const int Armenian_INI;
        public const int Armenian_JE;
        public const int Armenian_KE;
        public const int Armenian_KEN;
        public const int Armenian_KHE;
        public const int Armenian_LYUN;
        public const int Armenian_MEN;
        public const int Armenian_NU;
        public const int Armenian_O;
        public const int Armenian_PE;
        public const int Armenian_PYUR;
        public const int Armenian_RA;
        public const int Armenian_RE;
        public const int Armenian_SE;
        public const int Armenian_SHA;
        public const int Armenian_TCHE;
        public const int Armenian_TO;
        public const int Armenian_TSA;
        public const int Armenian_TSO;
        public const int Armenian_TYUN;
        public const int Armenian_VEV;
        public const int Armenian_VO;
        public const int Armenian_VYUN;
        public const int Armenian_YECH;
        public const int Armenian_ZA;
        public const int Armenian_ZHE;
        public const int Armenian_accent;
        public const int Armenian_amanak;
        public const int Armenian_apostrophe;
        public const int Armenian_at;
        public const int Armenian_ayb;
        public const int Armenian_ben;
        public const int Armenian_but;
        public const int Armenian_cha;
        public const int Armenian_da;
        public const int Armenian_dza;
        public const int Armenian_e;
        public const int Armenian_exclam;
        public const int Armenian_fe;
        public const int Armenian_full_stop;
        public const int Armenian_ghat;
        public const int Armenian_gim;
        public const int Armenian_hi;
        public const int Armenian_ho;
        public const int Armenian_hyphen;
        public const int Armenian_ini;
        public const int Armenian_je;
        public const int Armenian_ke;
        public const int Armenian_ken;
        public const int Armenian_khe;
        public const int Armenian_ligature_ew;
        public const int Armenian_lyun;
        public const int Armenian_men;
        public const int Armenian_nu;
        public const int Armenian_o;
        public const int Armenian_paruyk;
        public const int Armenian_pe;
        public const int Armenian_pyur;
        public const int Armenian_question;
        public const int Armenian_ra;
        public const int Armenian_re;
        public const int Armenian_se;
        public const int Armenian_separation_mark;
        public const int Armenian_sha;
        public const int Armenian_shesht;
        public const int Armenian_tche;
        public const int Armenian_to;
        public const int Armenian_tsa;
        public const int Armenian_tso;
        public const int Armenian_tyun;
        public const int Armenian_verjaket;
        public const int Armenian_vev;
        public const int Armenian_vo;
        public const int Armenian_vyun;
        public const int Armenian_yech;
        public const int Armenian_yentamna;
        public const int Armenian_za;
        public const int Armenian_zhe;
        public const int Atilde;
        public const int AudibleBell_Enable;
        public const int AudioCycleTrack;
        public const int AudioForward;
        public const int AudioLowerVolume;
        public const int AudioMedia;
        public const int AudioMicMute;
        public const int AudioMute;
        public const int AudioNext;
        public const int AudioPause;
        public const int AudioPlay;
        public const int AudioPrev;
        public const int AudioRaiseVolume;
        public const int AudioRandomPlay;
        public const int AudioRecord;
        public const int AudioRepeat;
        public const int AudioRewind;
        public const int AudioStop;
        public const int Away;
        public const int B;
        public const int Babovedot;
        public const int Back;
        public const int BackForward;
        public const int BackSpace;
        public const int Battery;
        public const int Begin;
        public const int Blue;
        public const int Bluetooth;
        public const int Book;
        public const int BounceKeys_Enable;
        public const int Break;
        public const int BrightnessAdjust;
        public const int Byelorussian_SHORTU;
        public const int Byelorussian_shortu;
        public const int C;
        public const int CD;
        public const int CH;
        public const int C_H;
        public const int C_h;
        public const int Cabovedot;
        public const int Cacute;
        public const int Calculator;
        public const int Calendar;
        public const int Cancel;
        public const int Caps_Lock;
        public const int Ccaron;
        public const int Ccedilla;
        public const int Ccircumflex;
        public const int Ch;
        public const int Clear;
        public const int ClearGrab;
        public const int Close;
        public const int Codeinput;
        public const int ColonSign;
        public const int Community;
        public const int ContrastAdjust;
        public const int Control_L;
        public const int Control_R;
        public const int Copy;
        public const int CruzeiroSign;
        public const int Cut;
        public const int CycleAngle;
        public const int Cyrillic_A;
        public const int Cyrillic_BE;
        public const int Cyrillic_CHE;
        public const int Cyrillic_CHE_descender;
        public const int Cyrillic_CHE_vertstroke;
        public const int Cyrillic_DE;
        public const int Cyrillic_DZHE;
        public const int Cyrillic_E;
        public const int Cyrillic_EF;
        public const int Cyrillic_EL;
        public const int Cyrillic_EM;
        public const int Cyrillic_EN;
        public const int Cyrillic_EN_descender;
        public const int Cyrillic_ER;
        public const int Cyrillic_ES;
        public const int Cyrillic_GHE;
        public const int Cyrillic_GHE_bar;
        public const int Cyrillic_HA;
        public const int Cyrillic_HARDSIGN;
        public const int Cyrillic_HA_descender;
        public const int Cyrillic_I;
        public const int Cyrillic_IE;
        public const int Cyrillic_IO;
        public const int Cyrillic_I_macron;
        public const int Cyrillic_JE;
        public const int Cyrillic_KA;
        public const int Cyrillic_KA_descender;
        public const int Cyrillic_KA_vertstroke;
        public const int Cyrillic_LJE;
        public const int Cyrillic_NJE;
        public const int Cyrillic_O;
        public const int Cyrillic_O_bar;
        public const int Cyrillic_PE;
        public const int Cyrillic_SCHWA;
        public const int Cyrillic_SHA;
        public const int Cyrillic_SHCHA;
        public const int Cyrillic_SHHA;
        public const int Cyrillic_SHORTI;
        public const int Cyrillic_SOFTSIGN;
        public const int Cyrillic_TE;
        public const int Cyrillic_TSE;
        public const int Cyrillic_U;
        public const int Cyrillic_U_macron;
        public const int Cyrillic_U_straight;
        public const int Cyrillic_U_straight_bar;
        public const int Cyrillic_VE;
        public const int Cyrillic_YA;
        public const int Cyrillic_YERU;
        public const int Cyrillic_YU;
        public const int Cyrillic_ZE;
        public const int Cyrillic_ZHE;
        public const int Cyrillic_ZHE_descender;
        public const int Cyrillic_a;
        public const int Cyrillic_be;
        public const int Cyrillic_che;
        public const int Cyrillic_che_descender;
        public const int Cyrillic_che_vertstroke;
        public const int Cyrillic_de;
        public const int Cyrillic_dzhe;
        public const int Cyrillic_e;
        public const int Cyrillic_ef;
        public const int Cyrillic_el;
        public const int Cyrillic_em;
        public const int Cyrillic_en;
        public const int Cyrillic_en_descender;
        public const int Cyrillic_er;
        public const int Cyrillic_es;
        public const int Cyrillic_ghe;
        public const int Cyrillic_ghe_bar;
        public const int Cyrillic_ha;
        public const int Cyrillic_ha_descender;
        public const int Cyrillic_hardsign;
        public const int Cyrillic_i;
        public const int Cyrillic_i_macron;
        public const int Cyrillic_ie;
        public const int Cyrillic_io;
        public const int Cyrillic_je;
        public const int Cyrillic_ka;
        public const int Cyrillic_ka_descender;
        public const int Cyrillic_ka_vertstroke;
        public const int Cyrillic_lje;
        public const int Cyrillic_nje;
        public const int Cyrillic_o;
        public const int Cyrillic_o_bar;
        public const int Cyrillic_pe;
        public const int Cyrillic_schwa;
        public const int Cyrillic_sha;
        public const int Cyrillic_shcha;
        public const int Cyrillic_shha;
        public const int Cyrillic_shorti;
        public const int Cyrillic_softsign;
        public const int Cyrillic_te;
        public const int Cyrillic_tse;
        public const int Cyrillic_u;
        public const int Cyrillic_u_macron;
        public const int Cyrillic_u_straight;
        public const int Cyrillic_u_straight_bar;
        public const int Cyrillic_ve;
        public const int Cyrillic_ya;
        public const int Cyrillic_yeru;
        public const int Cyrillic_yu;
        public const int Cyrillic_ze;
        public const int Cyrillic_zhe;
        public const int Cyrillic_zhe_descender;
        public const int D;
        public const int DOS;
        public const int Dabovedot;
        public const int Dcaron;
        public const int Delete;
        public const int Display;
        public const int Documents;
        public const int DongSign;
        public const int Down;
        public const int Dstroke;
        public const int E;
        public const int ENG;
        public const int ETH;
        public const int EZH;
        public const int Eabovedot;
        public const int Eacute;
        public const int Ebelowdot;
        public const int Ecaron;
        public const int Ecircumflex;
        public const int Ecircumflexacute;
        public const int Ecircumflexbelowdot;
        public const int Ecircumflexgrave;
        public const int Ecircumflexhook;
        public const int Ecircumflextilde;
        public const int EcuSign;
        public const int Ediaeresis;
        public const int Egrave;
        public const int Ehook;
        public const int Eisu_Shift;
        public const int Eisu_toggle;
        public const int Eject;
        public const int Emacron;
        public const int End;
        public const int Eogonek;
        public const int Escape;
        public const int Eth;
        public const int Etilde;
        public const int EuroSign;
        public const int Excel;
        public const int Execute;
        public const int Explorer;
        public const int F;
        public const int F1;
        public const int F10;
        public const int F11;
        public const int F12;
        public const int F13;
        public const int F14;
        public const int F15;
        public const int F16;
        public const int F17;
        public const int F18;
        public const int F19;
        public const int F2;
        public const int F20;
        public const int F21;
        public const int F22;
        public const int F23;
        public const int F24;
        public const int F25;
        public const int F26;
        public const int F27;
        public const int F28;
        public const int F29;
        public const int F3;
        public const int F30;
        public const int F31;
        public const int F32;
        public const int F33;
        public const int F34;
        public const int F35;
        public const int F4;
        public const int F5;
        public const int F6;
        public const int F7;
        public const int F8;
        public const int F9;
        public const int FFrancSign;
        public const int Fabovedot;
        public const int Farsi_0;
        public const int Farsi_1;
        public const int Farsi_2;
        public const int Farsi_3;
        public const int Farsi_4;
        public const int Farsi_5;
        public const int Farsi_6;
        public const int Farsi_7;
        public const int Farsi_8;
        public const int Farsi_9;
        public const int Farsi_yeh;
        public const int Favorites;
        public const int Finance;
        public const int Find;
        public const int First_Virtual_Screen;
        public const int Forward;
        public const int FrameBack;
        public const int FrameForward;
        public const int G;
        public const int Gabovedot;
        public const int Game;
        public const int Gbreve;
        public const int Gcaron;
        public const int Gcedilla;
        public const int Gcircumflex;
        public const int Georgian_an;
        public const int Georgian_ban;
        public const int Georgian_can;
        public const int Georgian_char;
        public const int Georgian_chin;
        public const int Georgian_cil;
        public const int Georgian_don;
        public const int Georgian_en;
        public const int Georgian_fi;
        public const int Georgian_gan;
        public const int Georgian_ghan;
        public const int Georgian_hae;
        public const int Georgian_har;
        public const int Georgian_he;
        public const int Georgian_hie;
        public const int Georgian_hoe;
        public const int Georgian_in;
        public const int Georgian_jhan;
        public const int Georgian_jil;
        public const int Georgian_kan;
        public const int Georgian_khar;
        public const int Georgian_las;
        public const int Georgian_man;
        public const int Georgian_nar;
        public const int Georgian_on;
        public const int Georgian_par;
        public const int Georgian_phar;
        public const int Georgian_qar;
        public const int Georgian_rae;
        public const int Georgian_san;
        public const int Georgian_shin;
        public const int Georgian_tan;
        public const int Georgian_tar;
        public const int Georgian_un;
        public const int Georgian_vin;
        public const int Georgian_we;
        public const int Georgian_xan;
        public const int Georgian_zen;
        public const int Georgian_zhar;
        public const int Go;
        public const int Greek_ALPHA;
        public const int Greek_ALPHAaccent;
        public const int Greek_BETA;
        public const int Greek_CHI;
        public const int Greek_DELTA;
        public const int Greek_EPSILON;
        public const int Greek_EPSILONaccent;
        public const int Greek_ETA;
        public const int Greek_ETAaccent;
        public const int Greek_GAMMA;
        public const int Greek_IOTA;
        public const int Greek_IOTAaccent;
        public const int Greek_IOTAdiaeresis;
        public const int Greek_IOTAdieresis;
        public const int Greek_KAPPA;
        public const int Greek_LAMBDA;
        public const int Greek_LAMDA;
        public const int Greek_MU;
        public const int Greek_NU;
        public const int Greek_OMEGA;
        public const int Greek_OMEGAaccent;
        public const int Greek_OMICRON;
        public const int Greek_OMICRONaccent;
        public const int Greek_PHI;
        public const int Greek_PI;
        public const int Greek_PSI;
        public const int Greek_RHO;
        public const int Greek_SIGMA;
        public const int Greek_TAU;
        public const int Greek_THETA;
        public const int Greek_UPSILON;
        public const int Greek_UPSILONaccent;
        public const int Greek_UPSILONdieresis;
        public const int Greek_XI;
        public const int Greek_ZETA;
        public const int Greek_accentdieresis;
        public const int Greek_alpha;
        public const int Greek_alphaaccent;
        public const int Greek_beta;
        public const int Greek_chi;
        public const int Greek_delta;
        public const int Greek_epsilon;
        public const int Greek_epsilonaccent;
        public const int Greek_eta;
        public const int Greek_etaaccent;
        public const int Greek_finalsmallsigma;
        public const int Greek_gamma;
        public const int Greek_horizbar;
        public const int Greek_iota;
        public const int Greek_iotaaccent;
        public const int Greek_iotaaccentdieresis;
        public const int Greek_iotadieresis;
        public const int Greek_kappa;
        public const int Greek_lambda;
        public const int Greek_lamda;
        public const int Greek_mu;
        public const int Greek_nu;
        public const int Greek_omega;
        public const int Greek_omegaaccent;
        public const int Greek_omicron;
        public const int Greek_omicronaccent;
        public const int Greek_phi;
        public const int Greek_pi;
        public const int Greek_psi;
        public const int Greek_rho;
        public const int Greek_sigma;
        public const int Greek_switch;
        public const int Greek_tau;
        public const int Greek_theta;
        public const int Greek_upsilon;
        public const int Greek_upsilonaccent;
        public const int Greek_upsilonaccentdieresis;
        public const int Greek_upsilondieresis;
        public const int Greek_xi;
        public const int Greek_zeta;
        public const int Green;
        public const int H;
        public const int Hangul;
        public const int Hangul_A;
        public const int Hangul_AE;
        public const int Hangul_AraeA;
        public const int Hangul_AraeAE;
        public const int Hangul_Banja;
        public const int Hangul_Cieuc;
        public const int Hangul_Codeinput;
        public const int Hangul_Dikeud;
        public const int Hangul_E;
        public const int Hangul_EO;
        public const int Hangul_EU;
        public const int Hangul_End;
        public const int Hangul_Hanja;
        public const int Hangul_Hieuh;
        public const int Hangul_I;
        public const int Hangul_Ieung;
        public const int Hangul_J_Cieuc;
        public const int Hangul_J_Dikeud;
        public const int Hangul_J_Hieuh;
        public const int Hangul_J_Ieung;
        public const int Hangul_J_Jieuj;
        public const int Hangul_J_Khieuq;
        public const int Hangul_J_Kiyeog;
        public const int Hangul_J_KiyeogSios;
        public const int Hangul_J_KkogjiDalrinIeung;
        public const int Hangul_J_Mieum;
        public const int Hangul_J_Nieun;
        public const int Hangul_J_NieunHieuh;
        public const int Hangul_J_NieunJieuj;
        public const int Hangul_J_PanSios;
        public const int Hangul_J_Phieuf;
        public const int Hangul_J_Pieub;
        public const int Hangul_J_PieubSios;
        public const int Hangul_J_Rieul;
        public const int Hangul_J_RieulHieuh;
        public const int Hangul_J_RieulKiyeog;
        public const int Hangul_J_RieulMieum;
        public const int Hangul_J_RieulPhieuf;
        public const int Hangul_J_RieulPieub;
        public const int Hangul_J_RieulSios;
        public const int Hangul_J_RieulTieut;
        public const int Hangul_J_Sios;
        public const int Hangul_J_SsangKiyeog;
        public const int Hangul_J_SsangSios;
        public const int Hangul_J_Tieut;
        public const int Hangul_J_YeorinHieuh;
        public const int Hangul_Jamo;
        public const int Hangul_Jeonja;
        public const int Hangul_Jieuj;
        public const int Hangul_Khieuq;
        public const int Hangul_Kiyeog;
        public const int Hangul_KiyeogSios;
        public const int Hangul_KkogjiDalrinIeung;
        public const int Hangul_Mieum;
        public const int Hangul_MultipleCandidate;
        public const int Hangul_Nieun;
        public const int Hangul_NieunHieuh;
        public const int Hangul_NieunJieuj;
        public const int Hangul_O;
        public const int Hangul_OE;
        public const int Hangul_PanSios;
        public const int Hangul_Phieuf;
        public const int Hangul_Pieub;
        public const int Hangul_PieubSios;
        public const int Hangul_PostHanja;
        public const int Hangul_PreHanja;
        public const int Hangul_PreviousCandidate;
        public const int Hangul_Rieul;
        public const int Hangul_RieulHieuh;
        public const int Hangul_RieulKiyeog;
        public const int Hangul_RieulMieum;
        public const int Hangul_RieulPhieuf;
        public const int Hangul_RieulPieub;
        public const int Hangul_RieulSios;
        public const int Hangul_RieulTieut;
        public const int Hangul_RieulYeorinHieuh;
        public const int Hangul_Romaja;
        public const int Hangul_SingleCandidate;
        public const int Hangul_Sios;
        public const int Hangul_Special;
        public const int Hangul_SsangDikeud;
        public const int Hangul_SsangJieuj;
        public const int Hangul_SsangKiyeog;
        public const int Hangul_SsangPieub;
        public const int Hangul_SsangSios;
        public const int Hangul_Start;
        public const int Hangul_SunkyeongeumMieum;
        public const int Hangul_SunkyeongeumPhieuf;
        public const int Hangul_SunkyeongeumPieub;
        public const int Hangul_Tieut;
        public const int Hangul_U;
        public const int Hangul_WA;
        public const int Hangul_WAE;
        public const int Hangul_WE;
        public const int Hangul_WEO;
        public const int Hangul_WI;
        public const int Hangul_YA;
        public const int Hangul_YAE;
        public const int Hangul_YE;
        public const int Hangul_YEO;
        public const int Hangul_YI;
        public const int Hangul_YO;
        public const int Hangul_YU;
        public const int Hangul_YeorinHieuh;
        public const int Hangul_switch;
        public const int Hankaku;
        public const int Hcircumflex;
        public const int Hebrew_switch;
        public const int Help;
        public const int Henkan;
        public const int Henkan_Mode;
        public const int Hibernate;
        public const int Hiragana;
        public const int Hiragana_Katakana;
        public const int History;
        public const int Home;
        public const int HomePage;
        public const int HotLinks;
        public const int Hstroke;
        public const int Hyper_L;
        public const int Hyper_R;
        public const int I;
        public const int ISO_Center_Object;
        public const int ISO_Continuous_Underline;
        public const int ISO_Discontinuous_Underline;
        public const int ISO_Emphasize;
        public const int ISO_Enter;
        public const int ISO_Fast_Cursor_Down;
        public const int ISO_Fast_Cursor_Left;
        public const int ISO_Fast_Cursor_Right;
        public const int ISO_Fast_Cursor_Up;
        public const int ISO_First_Group;
        public const int ISO_First_Group_Lock;
        public const int ISO_Group_Latch;
        public const int ISO_Group_Lock;
        public const int ISO_Group_Shift;
        public const int ISO_Last_Group;
        public const int ISO_Last_Group_Lock;
        public const int ISO_Left_Tab;
        public const int ISO_Level2_Latch;
        public const int ISO_Level3_Latch;
        public const int ISO_Level3_Lock;
        public const int ISO_Level3_Shift;
        public const int ISO_Level5_Latch;
        public const int ISO_Level5_Lock;
        public const int ISO_Level5_Shift;
        public const int ISO_Lock;
        public const int ISO_Move_Line_Down;
        public const int ISO_Move_Line_Up;
        public const int ISO_Next_Group;
        public const int ISO_Next_Group_Lock;
        public const int ISO_Partial_Line_Down;
        public const int ISO_Partial_Line_Up;
        public const int ISO_Partial_Space_Left;
        public const int ISO_Partial_Space_Right;
        public const int ISO_Prev_Group;
        public const int ISO_Prev_Group_Lock;
        public const int ISO_Release_Both_Margins;
        public const int ISO_Release_Margin_Left;
        public const int ISO_Release_Margin_Right;
        public const int ISO_Set_Margin_Left;
        public const int ISO_Set_Margin_Right;
        public const int Iabovedot;
        public const int Iacute;
        public const int Ibelowdot;
        public const int Ibreve;
        public const int Icircumflex;
        public const int Idiaeresis;
        public const int Igrave;
        public const int Ihook;
        public const int Imacron;
        public const int Insert;
        public const int Iogonek;
        public const int Itilde;
        public const int J;
        public const int Jcircumflex;
        public const int K;
        public const int KP_0;
        public const int KP_1;
        public const int KP_2;
        public const int KP_3;
        public const int KP_4;
        public const int KP_5;
        public const int KP_6;
        public const int KP_7;
        public const int KP_8;
        public const int KP_9;
        public const int KP_Add;
        public const int KP_Begin;
        public const int KP_Decimal;
        public const int KP_Delete;
        public const int KP_Divide;
        public const int KP_Down;
        public const int KP_End;
        public const int KP_Enter;
        public const int KP_Equal;
        public const int KP_F1;
        public const int KP_F2;
        public const int KP_F3;
        public const int KP_F4;
        public const int KP_Home;
        public const int KP_Insert;
        public const int KP_Left;
        public const int KP_Multiply;
        public const int KP_Next;
        public const int KP_Page_Down;
        public const int KP_Page_Up;
        public const int KP_Prior;
        public const int KP_Right;
        public const int KP_Separator;
        public const int KP_Space;
        public const int KP_Subtract;
        public const int KP_Tab;
        public const int KP_Up;
        public const int Kana_Lock;
        public const int Kana_Shift;
        public const int Kanji;
        public const int Kanji_Bangou;
        public const int Katakana;
        public const int KbdBrightnessDown;
        public const int KbdBrightnessUp;
        public const int KbdLightOnOff;
        public const int Kcedilla;
        public const int Korean_Won;
        public const int L;
        public const int L1;
        public const int L10;
        public const int L2;
        public const int L3;
        public const int L4;
        public const int L5;
        public const int L6;
        public const int L7;
        public const int L8;
        public const int L9;
        public const int Lacute;
        public const int Last_Virtual_Screen;
        public const int Launch0;
        public const int Launch1;
        public const int Launch2;
        public const int Launch3;
        public const int Launch4;
        public const int Launch5;
        public const int Launch6;
        public const int Launch7;
        public const int Launch8;
        public const int Launch9;
        public const int LaunchA;
        public const int LaunchB;
        public const int LaunchC;
        public const int LaunchD;
        public const int LaunchE;
        public const int LaunchF;
        public const int Lbelowdot;
        public const int Lcaron;
        public const int Lcedilla;
        public const int Left;
        public const int LightBulb;
        public const int Linefeed;
        public const int LiraSign;
        public const int LogGrabInfo;
        public const int LogOff;
        public const int LogWindowTree;
        public const int Lstroke;
        public const int M;
        public const int Mabovedot;
        public const int Macedonia_DSE;
        public const int Macedonia_GJE;
        public const int Macedonia_KJE;
        public const int Macedonia_dse;
        public const int Macedonia_gje;
        public const int Macedonia_kje;
        public const int Mae_Koho;
        public const int Mail;
        public const int MailForward;
        public const int Market;
        public const int Massyo;
        public const int Meeting;
        public const int Memo;
        public const int Menu;
        public const int MenuKB;
        public const int MenuPB;
        public const int Messenger;
        public const int Meta_L;
        public const int Meta_R;
        public const int MillSign;
        public const int ModeLock;
        public const int Mode_switch;
        public const int MonBrightnessDown;
        public const int MonBrightnessUp;
        public const int MouseKeys_Accel_Enable;
        public const int MouseKeys_Enable;
        public const int Muhenkan;
        public const int Multi_key;
        public const int MultipleCandidate;
        public const int Music;
        public const int MyComputer;
        public const int MySites;
        public const int N;
        public const int Nacute;
        public const int NairaSign;
        public const int Ncaron;
        public const int Ncedilla;
        public const int New;
        public const int NewSheqelSign;
        public const int News;
        public const int Next;
        public const int Next_VMode;
        public const int Next_Virtual_Screen;
        public const int Ntilde;
        public const int Num_Lock;
        public const int O;
        public const int OE;
        public const int Oacute;
        public const int Obarred;
        public const int Obelowdot;
        public const int Ocaron;
        public const int Ocircumflex;
        public const int Ocircumflexacute;
        public const int Ocircumflexbelowdot;
        public const int Ocircumflexgrave;
        public const int Ocircumflexhook;
        public const int Ocircumflextilde;
        public const int Odiaeresis;
        public const int Odoubleacute;
        public const int OfficeHome;
        public const int Ograve;
        public const int Ohook;
        public const int Ohorn;
        public const int Ohornacute;
        public const int Ohornbelowdot;
        public const int Ohorngrave;
        public const int Ohornhook;
        public const int Ohorntilde;
        public const int Omacron;
        public const int Ooblique;
        public const int Open;
        public const int OpenURL;
        public const int Option;
        public const int Oslash;
        public const int Otilde;
        public const int Overlay1_Enable;
        public const int Overlay2_Enable;
        public const int P;
        public const int Pabovedot;
        public const int Page_Down;
        public const int Page_Up;
        public const int Paste;
        public const int Pause;
        public const int PesetaSign;
        public const int Phone;
        public const int Pictures;
        public const int Pointer_Accelerate;
        public const int Pointer_Button1;
        public const int Pointer_Button2;
        public const int Pointer_Button3;
        public const int Pointer_Button4;
        public const int Pointer_Button5;
        public const int Pointer_Button_Dflt;
        public const int Pointer_DblClick1;
        public const int Pointer_DblClick2;
        public const int Pointer_DblClick3;
        public const int Pointer_DblClick4;
        public const int Pointer_DblClick5;
        public const int Pointer_DblClick_Dflt;
        public const int Pointer_DfltBtnNext;
        public const int Pointer_DfltBtnPrev;
        public const int Pointer_Down;
        public const int Pointer_DownLeft;
        public const int Pointer_DownRight;
        public const int Pointer_Drag1;
        public const int Pointer_Drag2;
        public const int Pointer_Drag3;
        public const int Pointer_Drag4;
        public const int Pointer_Drag5;
        public const int Pointer_Drag_Dflt;
        public const int Pointer_EnableKeys;
        public const int Pointer_Left;
        public const int Pointer_Right;
        public const int Pointer_Up;
        public const int Pointer_UpLeft;
        public const int Pointer_UpRight;
        public const int PowerDown;
        public const int PowerOff;
        public const int Prev_VMode;
        public const int Prev_Virtual_Screen;
        public const int PreviousCandidate;
        public const int Print;
        public const int Prior;
        public const int Q;
        public const int R;
        public const int R1;
        public const int R10;
        public const int R11;
        public const int R12;
        public const int R13;
        public const int R14;
        public const int R15;
        public const int R2;
        public const int R3;
        public const int R4;
        public const int R5;
        public const int R6;
        public const int R7;
        public const int R8;
        public const int R9;
        public const int Racute;
        public const int Rcaron;
        public const int Rcedilla;
        public const int Red;
        public const int Redo;
        public const int Refresh;
        public const int Reload;
        public const int RepeatKeys_Enable;
        public const int Reply;
        public const int Return;
        public const int Right;
        public const int RockerDown;
        public const int RockerEnter;
        public const int RockerUp;
        public const int Romaji;
        public const int RotateWindows;
        public const int RotationKB;
        public const int RotationPB;
        public const int RupeeSign;
        public const int S;
        public const int SCHWA;
        public const int Sabovedot;
        public const int Sacute;
        public const int Save;
        public const int Scaron;
        public const int Scedilla;
        public const int Scircumflex;
        public const int ScreenSaver;
        public const int ScrollClick;
        public const int ScrollDown;
        public const int ScrollUp;
        public const int Scroll_Lock;
        public const int Search;
        public const int Select;
        public const int SelectButton;
        public const int Send;
        public const int Serbian_DJE;
        public const int Serbian_DZE;
        public const int Serbian_JE;
        public const int Serbian_LJE;
        public const int Serbian_NJE;
        public const int Serbian_TSHE;
        public const int Serbian_dje;
        public const int Serbian_dze;
        public const int Serbian_je;
        public const int Serbian_lje;
        public const int Serbian_nje;
        public const int Serbian_tshe;
        public const int Shift_L;
        public const int Shift_Lock;
        public const int Shift_R;
        public const int Shop;
        public const int SingleCandidate;
        public const int Sinh_a;
        public const int Sinh_aa;
        public const int Sinh_aa2;
        public const int Sinh_ae;
        public const int Sinh_ae2;
        public const int Sinh_aee;
        public const int Sinh_aee2;
        public const int Sinh_ai;
        public const int Sinh_ai2;
        public const int Sinh_al;
        public const int Sinh_au;
        public const int Sinh_au2;
        public const int Sinh_ba;
        public const int Sinh_bha;
        public const int Sinh_ca;
        public const int Sinh_cha;
        public const int Sinh_dda;
        public const int Sinh_ddha;
        public const int Sinh_dha;
        public const int Sinh_dhha;
        public const int Sinh_e;
        public const int Sinh_e2;
        public const int Sinh_ee;
        public const int Sinh_ee2;
        public const int Sinh_fa;
        public const int Sinh_ga;
        public const int Sinh_gha;
        public const int Sinh_h2;
        public const int Sinh_ha;
        public const int Sinh_i;
        public const int Sinh_i2;
        public const int Sinh_ii;
        public const int Sinh_ii2;
        public const int Sinh_ja;
        public const int Sinh_jha;
        public const int Sinh_jnya;
        public const int Sinh_ka;
        public const int Sinh_kha;
        public const int Sinh_kunddaliya;
        public const int Sinh_la;
        public const int Sinh_lla;
        public const int Sinh_lu;
        public const int Sinh_lu2;
        public const int Sinh_luu;
        public const int Sinh_luu2;
        public const int Sinh_ma;
        public const int Sinh_mba;
        public const int Sinh_na;
        public const int Sinh_ndda;
        public const int Sinh_ndha;
        public const int Sinh_ng;
        public const int Sinh_ng2;
        public const int Sinh_nga;
        public const int Sinh_nja;
        public const int Sinh_nna;
        public const int Sinh_nya;
        public const int Sinh_o;
        public const int Sinh_o2;
        public const int Sinh_oo;
        public const int Sinh_oo2;
        public const int Sinh_pa;
        public const int Sinh_pha;
        public const int Sinh_ra;
        public const int Sinh_ri;
        public const int Sinh_rii;
        public const int Sinh_ru2;
        public const int Sinh_ruu2;
        public const int Sinh_sa;
        public const int Sinh_sha;
        public const int Sinh_ssha;
        public const int Sinh_tha;
        public const int Sinh_thha;
        public const int Sinh_tta;
        public const int Sinh_ttha;
        public const int Sinh_u;
        public const int Sinh_u2;
        public const int Sinh_uu;
        public const int Sinh_uu2;
        public const int Sinh_va;
        public const int Sinh_ya;
        public const int Sleep;
        public const int SlowKeys_Enable;
        public const int Spell;
        public const int SplitScreen;
        public const int Standby;
        public const int Start;
        public const int StickyKeys_Enable;
        public const int Stop;
        public const int Subtitle;
        public const int Super_L;
        public const int Super_R;
        public const int Support;
        public const int Suspend;
        public const int Switch_VT_1;
        public const int Switch_VT_10;
        public const int Switch_VT_11;
        public const int Switch_VT_12;
        public const int Switch_VT_2;
        public const int Switch_VT_3;
        public const int Switch_VT_4;
        public const int Switch_VT_5;
        public const int Switch_VT_6;
        public const int Switch_VT_7;
        public const int Switch_VT_8;
        public const int Switch_VT_9;
        public const int Sys_Req;
        public const int T;
        public const int THORN;
        public const int Tab;
        public const int Tabovedot;
        public const int TaskPane;
        public const int Tcaron;
        public const int Tcedilla;
        public const int Terminal;
        public const int Terminate_Server;
        public const int Thai_baht;
        public const int Thai_bobaimai;
        public const int Thai_chochan;
        public const int Thai_chochang;
        public const int Thai_choching;
        public const int Thai_chochoe;
        public const int Thai_dochada;
        public const int Thai_dodek;
        public const int Thai_fofa;
        public const int Thai_fofan;
        public const int Thai_hohip;
        public const int Thai_honokhuk;
        public const int Thai_khokhai;
        public const int Thai_khokhon;
        public const int Thai_khokhuat;
        public const int Thai_khokhwai;
        public const int Thai_khorakhang;
        public const int Thai_kokai;
        public const int Thai_lakkhangyao;
        public const int Thai_lekchet;
        public const int Thai_lekha;
        public const int Thai_lekhok;
        public const int Thai_lekkao;
        public const int Thai_leknung;
        public const int Thai_lekpaet;
        public const int Thai_leksam;
        public const int Thai_leksi;
        public const int Thai_leksong;
        public const int Thai_leksun;
        public const int Thai_lochula;
        public const int Thai_loling;
        public const int Thai_lu;
        public const int Thai_maichattawa;
        public const int Thai_maiek;
        public const int Thai_maihanakat;
        public const int Thai_maihanakat_maitho;
        public const int Thai_maitaikhu;
        public const int Thai_maitho;
        public const int Thai_maitri;
        public const int Thai_maiyamok;
        public const int Thai_moma;
        public const int Thai_ngongu;
        public const int Thai_nikhahit;
        public const int Thai_nonen;
        public const int Thai_nonu;
        public const int Thai_oang;
        public const int Thai_paiyannoi;
        public const int Thai_phinthu;
        public const int Thai_phophan;
        public const int Thai_phophung;
        public const int Thai_phosamphao;
        public const int Thai_popla;
        public const int Thai_rorua;
        public const int Thai_ru;
        public const int Thai_saraa;
        public const int Thai_saraaa;
        public const int Thai_saraae;
        public const int Thai_saraaimaimalai;
        public const int Thai_saraaimaimuan;
        public const int Thai_saraam;
        public const int Thai_sarae;
        public const int Thai_sarai;
        public const int Thai_saraii;
        public const int Thai_sarao;
        public const int Thai_sarau;
        public const int Thai_saraue;
        public const int Thai_sarauee;
        public const int Thai_sarauu;
        public const int Thai_sorusi;
        public const int Thai_sosala;
        public const int Thai_soso;
        public const int Thai_sosua;
        public const int Thai_thanthakhat;
        public const int Thai_thonangmontho;
        public const int Thai_thophuthao;
        public const int Thai_thothahan;
        public const int Thai_thothan;
        public const int Thai_thothong;
        public const int Thai_thothung;
        public const int Thai_topatak;
        public const int Thai_totao;
        public const int Thai_wowaen;
        public const int Thai_yoyak;
        public const int Thai_yoying;
        public const int Thorn;
        public const int Time;
        public const int ToDoList;
        public const int Tools;
        public const int TopMenu;
        public const int TouchpadOff;
        public const int TouchpadOn;
        public const int TouchpadToggle;
        public const int Touroku;
        public const int Travel;
        public const int Tslash;
        public const int U;
        public const int UWB;
        public const int Uacute;
        public const int Ubelowdot;
        public const int Ubreve;
        public const int Ucircumflex;
        public const int Udiaeresis;
        public const int Udoubleacute;
        public const int Ugrave;
        public const int Uhook;
        public const int Uhorn;
        public const int Uhornacute;
        public const int Uhornbelowdot;
        public const int Uhorngrave;
        public const int Uhornhook;
        public const int Uhorntilde;
        public const int Ukrainian_GHE_WITH_UPTURN;
        public const int Ukrainian_I;
        public const int Ukrainian_IE;
        public const int Ukrainian_YI;
        public const int Ukrainian_ghe_with_upturn;
        public const int Ukrainian_i;
        public const int Ukrainian_ie;
        public const int Ukrainian_yi;
        public const int Ukranian_I;
        public const int Ukranian_JE;
        public const int Ukranian_YI;
        public const int Ukranian_i;
        public const int Ukranian_je;
        public const int Ukranian_yi;
        public const int Umacron;
        public const int Undo;
        public const int Ungrab;
        public const int Uogonek;
        public const int Up;
        public const int Uring;
        public const int User1KB;
        public const int User2KB;
        public const int UserPB;
        public const int Utilde;
        public const int V;
        public const int VendorHome;
        public const int Video;
        public const int View;
        public const int VoidSymbol;
        public const int W;
        public const int WLAN;
        public const int WWW;
        public const int Wacute;
        public const int WakeUp;
        public const int Wcircumflex;
        public const int Wdiaeresis;
        public const int WebCam;
        public const int Wgrave;
        public const int WheelButton;
        public const int WindowClear;
        public const int WonSign;
        public const int Word;
        public const int X;
        public const int Xabovedot;
        public const int Xfer;
        public const int Y;
        public const int Yacute;
        public const int Ybelowdot;
        public const int Ycircumflex;
        public const int Ydiaeresis;
        public const int Yellow;
        public const int Ygrave;
        public const int Yhook;
        public const int Ytilde;
        public const int Z;
        public const int Zabovedot;
        public const int Zacute;
        public const int Zcaron;
        public const int Zen_Koho;
        public const int Zenkaku;
        public const int Zenkaku_Hankaku;
        public const int ZoomIn;
        public const int ZoomOut;
        public const int Zstroke;
        public const int a;
        public const int aacute;
        public const int abelowdot;
        public const int abovedot;
        public const int abreve;
        public const int abreveacute;
        public const int abrevebelowdot;
        public const int abrevegrave;
        public const int abrevehook;
        public const int abrevetilde;
        public const int acircumflex;
        public const int acircumflexacute;
        public const int acircumflexbelowdot;
        public const int acircumflexgrave;
        public const int acircumflexhook;
        public const int acircumflextilde;
        public const int acute;
        public const int adiaeresis;
        public const int ae;
        public const int agrave;
        public const int ahook;
        public const int amacron;
        public const int ampersand;
        public const int aogonek;
        public const int apostrophe;
        public const int approxeq;
        public const int approximate;
        public const int aring;
        public const int asciicircum;
        public const int asciitilde;
        public const int asterisk;
        public const int at;
        public const int atilde;
        public const int b;
        public const int babovedot;
        public const int backslash;
        public const int ballotcross;
        public const int bar;
        public const int because;
        public const int blank;
        public const int botintegral;
        public const int botleftparens;
        public const int botleftsqbracket;
        public const int botleftsummation;
        public const int botrightparens;
        public const int botrightsqbracket;
        public const int botrightsummation;
        public const int bott;
        public const int botvertsummationconnector;
        public const int braceleft;
        public const int braceright;
        public const int bracketleft;
        public const int bracketright;
        public const int braille_blank;
        public const int braille_dot_1;
        public const int braille_dot_10;
        public const int braille_dot_2;
        public const int braille_dot_3;
        public const int braille_dot_4;
        public const int braille_dot_5;
        public const int braille_dot_6;
        public const int braille_dot_7;
        public const int braille_dot_8;
        public const int braille_dot_9;
        public const int braille_dots_1;
        public const int braille_dots_12;
        public const int braille_dots_123;
        public const int braille_dots_1234;
        public const int braille_dots_12345;
        public const int braille_dots_123456;
        public const int braille_dots_1234567;
        public const int braille_dots_12345678;
        public const int braille_dots_1234568;
        public const int braille_dots_123457;
        public const int braille_dots_1234578;
        public const int braille_dots_123458;
        public const int braille_dots_12346;
        public const int braille_dots_123467;
        public const int braille_dots_1234678;
        public const int braille_dots_123468;
        public const int braille_dots_12347;
        public const int braille_dots_123478;
        public const int braille_dots_12348;
        public const int braille_dots_1235;
        public const int braille_dots_12356;
        public const int braille_dots_123567;
        public const int braille_dots_1235678;
        public const int braille_dots_123568;
        public const int braille_dots_12357;
        public const int braille_dots_123578;
        public const int braille_dots_12358;
        public const int braille_dots_1236;
        public const int braille_dots_12367;
        public const int braille_dots_123678;
        public const int braille_dots_12368;
        public const int braille_dots_1237;
        public const int braille_dots_12378;
        public const int braille_dots_1238;
        public const int braille_dots_124;
        public const int braille_dots_1245;
        public const int braille_dots_12456;
        public const int braille_dots_124567;
        public const int braille_dots_1245678;
        public const int braille_dots_124568;
        public const int braille_dots_12457;
        public const int braille_dots_124578;
        public const int braille_dots_12458;
        public const int braille_dots_1246;
        public const int braille_dots_12467;
        public const int braille_dots_124678;
        public const int braille_dots_12468;
        public const int braille_dots_1247;
        public const int braille_dots_12478;
        public const int braille_dots_1248;
        public const int braille_dots_125;
        public const int braille_dots_1256;
        public const int braille_dots_12567;
        public const int braille_dots_125678;
        public const int braille_dots_12568;
        public const int braille_dots_1257;
        public const int braille_dots_12578;
        public const int braille_dots_1258;
        public const int braille_dots_126;
        public const int braille_dots_1267;
        public const int braille_dots_12678;
        public const int braille_dots_1268;
        public const int braille_dots_127;
        public const int braille_dots_1278;
        public const int braille_dots_128;
        public const int braille_dots_13;
        public const int braille_dots_134;
        public const int braille_dots_1345;
        public const int braille_dots_13456;
        public const int braille_dots_134567;
        public const int braille_dots_1345678;
        public const int braille_dots_134568;
        public const int braille_dots_13457;
        public const int braille_dots_134578;
        public const int braille_dots_13458;
        public const int braille_dots_1346;
        public const int braille_dots_13467;
        public const int braille_dots_134678;
        public const int braille_dots_13468;
        public const int braille_dots_1347;
        public const int braille_dots_13478;
        public const int braille_dots_1348;
        public const int braille_dots_135;
        public const int braille_dots_1356;
        public const int braille_dots_13567;
        public const int braille_dots_135678;
        public const int braille_dots_13568;
        public const int braille_dots_1357;
        public const int braille_dots_13578;
        public const int braille_dots_1358;
        public const int braille_dots_136;
        public const int braille_dots_1367;
        public const int braille_dots_13678;
        public const int braille_dots_1368;
        public const int braille_dots_137;
        public const int braille_dots_1378;
        public const int braille_dots_138;
        public const int braille_dots_14;
        public const int braille_dots_145;
        public const int braille_dots_1456;
        public const int braille_dots_14567;
        public const int braille_dots_145678;
        public const int braille_dots_14568;
        public const int braille_dots_1457;
        public const int braille_dots_14578;
        public const int braille_dots_1458;
        public const int braille_dots_146;
        public const int braille_dots_1467;
        public const int braille_dots_14678;
        public const int braille_dots_1468;
        public const int braille_dots_147;
        public const int braille_dots_1478;
        public const int braille_dots_148;
        public const int braille_dots_15;
        public const int braille_dots_156;
        public const int braille_dots_1567;
        public const int braille_dots_15678;
        public const int braille_dots_1568;
        public const int braille_dots_157;
        public const int braille_dots_1578;
        public const int braille_dots_158;
        public const int braille_dots_16;
        public const int braille_dots_167;
        public const int braille_dots_1678;
        public const int braille_dots_168;
        public const int braille_dots_17;
        public const int braille_dots_178;
        public const int braille_dots_18;
        public const int braille_dots_2;
        public const int braille_dots_23;
        public const int braille_dots_234;
        public const int braille_dots_2345;
        public const int braille_dots_23456;
        public const int braille_dots_234567;
        public const int braille_dots_2345678;
        public const int braille_dots_234568;
        public const int braille_dots_23457;
        public const int braille_dots_234578;
        public const int braille_dots_23458;
        public const int braille_dots_2346;
        public const int braille_dots_23467;
        public const int braille_dots_234678;
        public const int braille_dots_23468;
        public const int braille_dots_2347;
        public const int braille_dots_23478;
        public const int braille_dots_2348;
        public const int braille_dots_235;
        public const int braille_dots_2356;
        public const int braille_dots_23567;
        public const int braille_dots_235678;
        public const int braille_dots_23568;
        public const int braille_dots_2357;
        public const int braille_dots_23578;
        public const int braille_dots_2358;
        public const int braille_dots_236;
        public const int braille_dots_2367;
        public const int braille_dots_23678;
        public const int braille_dots_2368;
        public const int braille_dots_237;
        public const int braille_dots_2378;
        public const int braille_dots_238;
        public const int braille_dots_24;
        public const int braille_dots_245;
        public const int braille_dots_2456;
        public const int braille_dots_24567;
        public const int braille_dots_245678;
        public const int braille_dots_24568;
        public const int braille_dots_2457;
        public const int braille_dots_24578;
        public const int braille_dots_2458;
        public const int braille_dots_246;
        public const int braille_dots_2467;
        public const int braille_dots_24678;
        public const int braille_dots_2468;
        public const int braille_dots_247;
        public const int braille_dots_2478;
        public const int braille_dots_248;
        public const int braille_dots_25;
        public const int braille_dots_256;
        public const int braille_dots_2567;
        public const int braille_dots_25678;
        public const int braille_dots_2568;
        public const int braille_dots_257;
        public const int braille_dots_2578;
        public const int braille_dots_258;
        public const int braille_dots_26;
        public const int braille_dots_267;
        public const int braille_dots_2678;
        public const int braille_dots_268;
        public const int braille_dots_27;
        public const int braille_dots_278;
        public const int braille_dots_28;
        public const int braille_dots_3;
        public const int braille_dots_34;
        public const int braille_dots_345;
        public const int braille_dots_3456;
        public const int braille_dots_34567;
        public const int braille_dots_345678;
        public const int braille_dots_34568;
        public const int braille_dots_3457;
        public const int braille_dots_34578;
        public const int braille_dots_3458;
        public const int braille_dots_346;
        public const int braille_dots_3467;
        public const int braille_dots_34678;
        public const int braille_dots_3468;
        public const int braille_dots_347;
        public const int braille_dots_3478;
        public const int braille_dots_348;
        public const int braille_dots_35;
        public const int braille_dots_356;
        public const int braille_dots_3567;
        public const int braille_dots_35678;
        public const int braille_dots_3568;
        public const int braille_dots_357;
        public const int braille_dots_3578;
        public const int braille_dots_358;
        public const int braille_dots_36;
        public const int braille_dots_367;
        public const int braille_dots_3678;
        public const int braille_dots_368;
        public const int braille_dots_37;
        public const int braille_dots_378;
        public const int braille_dots_38;
        public const int braille_dots_4;
        public const int braille_dots_45;
        public const int braille_dots_456;
        public const int braille_dots_4567;
        public const int braille_dots_45678;
        public const int braille_dots_4568;
        public const int braille_dots_457;
        public const int braille_dots_4578;
        public const int braille_dots_458;
        public const int braille_dots_46;
        public const int braille_dots_467;
        public const int braille_dots_4678;
        public const int braille_dots_468;
        public const int braille_dots_47;
        public const int braille_dots_478;
        public const int braille_dots_48;
        public const int braille_dots_5;
        public const int braille_dots_56;
        public const int braille_dots_567;
        public const int braille_dots_5678;
        public const int braille_dots_568;
        public const int braille_dots_57;
        public const int braille_dots_578;
        public const int braille_dots_58;
        public const int braille_dots_6;
        public const int braille_dots_67;
        public const int braille_dots_678;
        public const int braille_dots_68;
        public const int braille_dots_7;
        public const int braille_dots_78;
        public const int braille_dots_8;
        public const int breve;
        public const int brokenbar;
        public const int c;
        public const int c_h;
        public const int cabovedot;
        public const int cacute;
        public const int careof;
        public const int caret;
        public const int caron;
        public const int ccaron;
        public const int ccedilla;
        public const int ccircumflex;
        public const int cedilla;
        public const int cent;
        public const int ch;
        public const int checkerboard;
        public const int checkmark;
        public const int circle;
        public const int club;
        public const int colon;
        public const int comma;
        public const int containsas;
        public const int copyright;
        public const int cr;
        public const int crossinglines;
        public const int cuberoot;
        public const int currency;
        public const int cursor;
        public const int d;
        public const int dabovedot;
        public const int dagger;
        public const int dcaron;
        public const int dead_A;
        public const int dead_E;
        public const int dead_I;
        public const int dead_O;
        public const int dead_U;
        public const int dead_a;
        public const int dead_abovecomma;
        public const int dead_abovedot;
        public const int dead_abovereversedcomma;
        public const int dead_abovering;
        public const int dead_acute;
        public const int dead_belowbreve;
        public const int dead_belowcircumflex;
        public const int dead_belowcomma;
        public const int dead_belowdiaeresis;
        public const int dead_belowdot;
        public const int dead_belowmacron;
        public const int dead_belowring;
        public const int dead_belowtilde;
        public const int dead_breve;
        public const int dead_capital_schwa;
        public const int dead_caron;
        public const int dead_cedilla;
        public const int dead_circumflex;
        public const int dead_currency;
        public const int dead_dasia;
        public const int dead_diaeresis;
        public const int dead_doubleacute;
        public const int dead_doublegrave;
        public const int dead_e;
        public const int dead_grave;
        public const int dead_greek;
        public const int dead_hook;
        public const int dead_horn;
        public const int dead_i;
        public const int dead_invertedbreve;
        public const int dead_iota;
        public const int dead_macron;
        public const int dead_o;
        public const int dead_ogonek;
        public const int dead_perispomeni;
        public const int dead_psili;
        public const int dead_semivoiced_sound;
        public const int dead_small_schwa;
        public const int dead_stroke;
        public const int dead_tilde;
        public const int dead_u;
        public const int dead_voiced_sound;
        public const int decimalpoint;
        public const int degree;
        public const int diaeresis;
        public const int diamond;
        public const int digitspace;
        public const int dintegral;
        public const int division;
        public const int dollar;
        public const int doubbaselinedot;
        public const int doubleacute;
        public const int doubledagger;
        public const int doublelowquotemark;
        public const int downarrow;
        public const int downcaret;
        public const int downshoe;
        public const int downstile;
        public const int downtack;
        public const int dstroke;
        public const int e;
        public const int eabovedot;
        public const int eacute;
        public const int ebelowdot;
        public const int ecaron;
        public const int ecircumflex;
        public const int ecircumflexacute;
        public const int ecircumflexbelowdot;
        public const int ecircumflexgrave;
        public const int ecircumflexhook;
        public const int ecircumflextilde;
        public const int ediaeresis;
        public const int egrave;
        public const int ehook;
        public const int eightsubscript;
        public const int eightsuperior;
        public const int elementof;
        public const int ellipsis;
        public const int em3space;
        public const int em4space;
        public const int emacron;
        public const int emdash;
        public const int emfilledcircle;
        public const int emfilledrect;
        public const int emopencircle;
        public const int emopenrectangle;
        public const int emptyset;
        public const int emspace;
        public const int endash;
        public const int enfilledcircbullet;
        public const int enfilledsqbullet;
        public const int eng;
        public const int enopencircbullet;
        public const int enopensquarebullet;
        public const int enspace;
        public const int eogonek;
        public const int equal;
        public const int eth;
        public const int etilde;
        public const int exclam;
        public const int exclamdown;
        public const int ezh;
        public const int f;
        public const int fabovedot;
        public const int femalesymbol;
        public const int ff;
        public const int figdash;
        public const int filledlefttribullet;
        public const int filledrectbullet;
        public const int filledrighttribullet;
        public const int filledtribulletdown;
        public const int filledtribulletup;
        public const int fiveeighths;
        public const int fivesixths;
        public const int fivesubscript;
        public const int fivesuperior;
        public const int fourfifths;
        public const int foursubscript;
        public const int foursuperior;
        public const int fourthroot;
        public const int function;
        public const int g;
        public const int gabovedot;
        public const int gbreve;
        public const int gcaron;
        public const int gcedilla;
        public const int gcircumflex;
        public const int grave;
        public const int greater;
        public const int greaterthanequal;
        public const int guillemotleft;
        public const int guillemotright;
        public const int h;
        public const int hairspace;
        public const int hcircumflex;
        public const int heart;
        public const int hebrew_aleph;
        public const int hebrew_ayin;
        public const int hebrew_bet;
        public const int hebrew_beth;
        public const int hebrew_chet;
        public const int hebrew_dalet;
        public const int hebrew_daleth;
        public const int hebrew_doublelowline;
        public const int hebrew_finalkaph;
        public const int hebrew_finalmem;
        public const int hebrew_finalnun;
        public const int hebrew_finalpe;
        public const int hebrew_finalzade;
        public const int hebrew_finalzadi;
        public const int hebrew_gimel;
        public const int hebrew_gimmel;
        public const int hebrew_he;
        public const int hebrew_het;
        public const int hebrew_kaph;
        public const int hebrew_kuf;
        public const int hebrew_lamed;
        public const int hebrew_mem;
        public const int hebrew_nun;
        public const int hebrew_pe;
        public const int hebrew_qoph;
        public const int hebrew_resh;
        public const int hebrew_samech;
        public const int hebrew_samekh;
        public const int hebrew_shin;
        public const int hebrew_taf;
        public const int hebrew_taw;
        public const int hebrew_tet;
        public const int hebrew_teth;
        public const int hebrew_waw;
        public const int hebrew_yod;
        public const int hebrew_zade;
        public const int hebrew_zadi;
        public const int hebrew_zain;
        public const int hebrew_zayin;
        public const int hexagram;
        public const int horizconnector;
        public const int horizlinescan1;
        public const int horizlinescan3;
        public const int horizlinescan5;
        public const int horizlinescan7;
        public const int horizlinescan9;
        public const int hstroke;
        public const int ht;
        public const int hyphen;
        public const int i;
        public const int iTouch;
        public const int iacute;
        public const int ibelowdot;
        public const int ibreve;
        public const int icircumflex;
        public const int identical;
        public const int idiaeresis;
        public const int idotless;
        public const int ifonlyif;
        public const int igrave;
        public const int ihook;
        public const int imacron;
        public const int implies;
        public const int includedin;
        public const int includes;
        public const int infinity;
        public const int integral;
        public const int intersection;
        public const int iogonek;
        public const int itilde;
        public const int j;
        public const int jcircumflex;
        public const int jot;
        public const int k;
        public const int kana_A;
        public const int kana_CHI;
        public const int kana_E;
        public const int kana_FU;
        public const int kana_HA;
        public const int kana_HE;
        public const int kana_HI;
        public const int kana_HO;
        public const int kana_HU;
        public const int kana_I;
        public const int kana_KA;
        public const int kana_KE;
        public const int kana_KI;
        public const int kana_KO;
        public const int kana_KU;
        public const int kana_MA;
        public const int kana_ME;
        public const int kana_MI;
        public const int kana_MO;
        public const int kana_MU;
        public const int kana_N;
        public const int kana_NA;
        public const int kana_NE;
        public const int kana_NI;
        public const int kana_NO;
        public const int kana_NU;
        public const int kana_O;
        public const int kana_RA;
        public const int kana_RE;
        public const int kana_RI;
        public const int kana_RO;
        public const int kana_RU;
        public const int kana_SA;
        public const int kana_SE;
        public const int kana_SHI;
        public const int kana_SO;
        public const int kana_SU;
        public const int kana_TA;
        public const int kana_TE;
        public const int kana_TI;
        public const int kana_TO;
        public const int kana_TSU;
        public const int kana_TU;
        public const int kana_U;
        public const int kana_WA;
        public const int kana_WO;
        public const int kana_YA;
        public const int kana_YO;
        public const int kana_YU;
        public const int kana_a;
        public const int kana_closingbracket;
        public const int kana_comma;
        public const int kana_conjunctive;
        public const int kana_e;
        public const int kana_fullstop;
        public const int kana_i;
        public const int kana_middledot;
        public const int kana_o;
        public const int kana_openingbracket;
        public const int kana_switch;
        public const int kana_tsu;
        public const int kana_tu;
        public const int kana_u;
        public const int kana_ya;
        public const int kana_yo;
        public const int kana_yu;
        public const int kappa;
        public const int kcedilla;
        public const int kra;
        public const int l;
        public const int lacute;
        public const int latincross;
        public const int lbelowdot;
        public const int lcaron;
        public const int lcedilla;
        public const int leftanglebracket;
        public const int leftarrow;
        public const int leftcaret;
        public const int leftdoublequotemark;
        public const int leftmiddlecurlybrace;
        public const int leftopentriangle;
        public const int leftpointer;
        public const int leftradical;
        public const int leftshoe;
        public const int leftsinglequotemark;
        public const int leftt;
        public const int lefttack;
        public const int less;
        public const int lessthanequal;
        public const int lf;
        public const int logicaland;
        public const int logicalor;
        public const int lowleftcorner;
        public const int lowrightcorner;
        public const int lstroke;
        public const int m;
        public const int mabovedot;
        public const int macron;
        public const int malesymbol;
        public const int maltesecross;
        public const int marker;
        public const int masculine;
        public const int minus;
        public const int minutes;
        public const int mu;
        public const int multiply;
        public const int musicalflat;
        public const int musicalsharp;
        public const int n;
        public const int nabla;
        public const int nacute;
        public const int ncaron;
        public const int ncedilla;
        public const int ninesubscript;
        public const int ninesuperior;
        public const int nl;
        public const int nobreakspace;
        public const int notapproxeq;
        public const int notelementof;
        public const int notequal;
        public const int notidentical;
        public const int notsign;
        public const int ntilde;
        public const int numbersign;
        public const int numerosign;
        public const int o;
        public const int oacute;
        public const int obarred;
        public const int obelowdot;
        public const int ocaron;
        public const int ocircumflex;
        public const int ocircumflexacute;
        public const int ocircumflexbelowdot;
        public const int ocircumflexgrave;
        public const int ocircumflexhook;
        public const int ocircumflextilde;
        public const int odiaeresis;
        public const int odoubleacute;
        public const int oe;
        public const int ogonek;
        public const int ograve;
        public const int ohook;
        public const int ohorn;
        public const int ohornacute;
        public const int ohornbelowdot;
        public const int ohorngrave;
        public const int ohornhook;
        public const int ohorntilde;
        public const int omacron;
        public const int oneeighth;
        public const int onefifth;
        public const int onehalf;
        public const int onequarter;
        public const int onesixth;
        public const int onesubscript;
        public const int onesuperior;
        public const int onethird;
        public const int ooblique;
        public const int openrectbullet;
        public const int openstar;
        public const int opentribulletdown;
        public const int opentribulletup;
        public const int ordfeminine;
        public const int oslash;
        public const int otilde;
        public const int overbar;
        public const int overline;
        public const int p;
        public const int pabovedot;
        public const int paragraph;
        public const int parenleft;
        public const int parenright;
        public const int partdifferential;
        public const int partialderivative;
        public const int percent;
        public const int period;
        public const int periodcentered;
        public const int permille;
        public const int phonographcopyright;
        public const int plus;
        public const int plusminus;
        public const int prescription;
        public const int prolongedsound;
        public const int punctspace;
        public const int q;
        public const int quad;
        public const int question;
        public const int questiondown;
        public const int quotedbl;
        public const int quoteleft;
        public const int quoteright;
        public const int r;
        public const int racute;
        public const int radical;
        public const int rcaron;
        public const int rcedilla;
        public const int registered;
        public const int rightanglebracket;
        public const int rightarrow;
        public const int rightcaret;
        public const int rightdoublequotemark;
        public const int rightmiddlecurlybrace;
        public const int rightmiddlesummation;
        public const int rightopentriangle;
        public const int rightpointer;
        public const int rightshoe;
        public const int rightsinglequotemark;
        public const int rightt;
        public const int righttack;
        public const int s;
        public const int sabovedot;
        public const int sacute;
        public const int scaron;
        public const int scedilla;
        public const int schwa;
        public const int scircumflex;
        public const int script_switch;
        public const int seconds;
        public const int section;
        public const int semicolon;
        public const int semivoicedsound;
        public const int seveneighths;
        public const int sevensubscript;
        public const int sevensuperior;
        public const int signaturemark;
        public const int signifblank;
        public const int similarequal;
        public const int singlelowquotemark;
        public const int sixsubscript;
        public const int sixsuperior;
        public const int slash;
        public const int soliddiamond;
        public const int space;
        public const int squareroot;
        public const int ssharp;
        public const int sterling;
        public const int stricteq;
        public const int t;
        public const int tabovedot;
        public const int tcaron;
        public const int tcedilla;
        public const int telephone;
        public const int telephonerecorder;
        public const int therefore;
        public const int thinspace;
        public const int thorn;
        public const int threeeighths;
        public const int threefifths;
        public const int threequarters;
        public const int threesubscript;
        public const int threesuperior;
        public const int tintegral;
        public const int topintegral;
        public const int topleftparens;
        public const int topleftradical;
        public const int topleftsqbracket;
        public const int topleftsummation;
        public const int toprightparens;
        public const int toprightsqbracket;
        public const int toprightsummation;
        public const int topt;
        public const int topvertsummationconnector;
        public const int trademark;
        public const int trademarkincircle;
        public const int tslash;
        public const int twofifths;
        public const int twosubscript;
        public const int twosuperior;
        public const int twothirds;
        public const int u;
        public const int uacute;
        public const int ubelowdot;
        public const int ubreve;
        public const int ucircumflex;
        public const int udiaeresis;
        public const int udoubleacute;
        public const int ugrave;
        public const int uhook;
        public const int uhorn;
        public const int uhornacute;
        public const int uhornbelowdot;
        public const int uhorngrave;
        public const int uhornhook;
        public const int uhorntilde;
        public const int umacron;
        public const int underbar;
        public const int underscore;
        public const int union;
        public const int uogonek;
        public const int uparrow;
        public const int upcaret;
        public const int upleftcorner;
        public const int uprightcorner;
        public const int upshoe;
        public const int upstile;
        public const int uptack;
        public const int uring;
        public const int utilde;
        public const int v;
        public const int variation;
        public const int vertbar;
        public const int vertconnector;
        public const int voicedsound;
        public const int vt;
        public const int w;
        public const int wacute;
        public const int wcircumflex;
        public const int wdiaeresis;
        public const int wgrave;
        public const int x;
        public const int xabovedot;
        public const int y;
        public const int yacute;
        public const int ybelowdot;
        public const int ycircumflex;
        public const int ydiaeresis;
        public const int yen;
        public const int ygrave;
        public const int yhook;
        public const int ytilde;
        public const int z;
        public const int zabovedot;
        public const int zacute;
        public const int zcaron;
        public const int zerosubscript;
        public const int zerosuperior;
        public const int zstroke;
    }

    [CCode (has_type_id = false)]
    [Flags]
    public enum GrxKeyModifier {
        LOCK,
        SHIFT,
        CTRL,
        ALT,
        SUPER,
    }

    [CCode (has_type_id = false)]
    public enum EventType {
        NONE,
        KEY_UP,
        KEY_DOWN,
        BUTTON_PRESS,
        BUTTON_RELEASE,
        BUTTON_DOUBLE_PRESS,
        TOUCH_DOWN,
        TOUCH_MOTION,
        TOUCH_UP,
        TOUCH_CANCEL,
    }

    [CCode (has_type_id = false)]
    public struct AnyEvent {
        EventType type;
        Device *device;
    }

    [CCode (has_type_id = false)]
    public struct KeyEvent {
        EventType type;
        Device *device;
        uint keysym;
        unichar unichar;
        uint code;
    }

    [CCode (has_type_id = false)]
    public struct ButtonEvent {
        EventType type;
        Device *device;
        uint button;
    }

    [CCode (has_type_id = false)]
    public struct TouchEvent {
        EventType type;
        Device *device;
        int id;
        int x;
        int y;
    }

    [CCode (has_type_id = false)]
    public struct Event {
        EventType type;
        AnyEvent any;
        KeyEvent key;
        ButtonEvent button;
        TouchEvent touch;
    }

    public abstract class Device : GLib.Object {
        public string name { get; }
        public string sysname { get; }
        public bool has_keyboard { get; }
        public bool has_pointer { get; }
        public bool has_touch { get; }
        public bool uncalibrate ();
    }

    public abstract class DeviceManager : GLib.Object {
        public signal void device_added (Device device);
        public signal void device_removed (Device device);
    }

    public class Application : GLib.Application, GLib.Initable {
        public Application.new (GLib.Cancellable? cancellable = null) throws GLib.Error;
        public bool init (GLib.Cancellable? cancellable = null) throws GLib.Error;
        public virtual void input_event (Event event);
        public bool is_active { [CCode (cname = "grx_application_is_active")]get; }
    }
}
