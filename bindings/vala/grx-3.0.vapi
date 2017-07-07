/*
 * Copyright 2014-2017 David Lechner <david@lechnology.com>
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
 */
[CCode (cheader_filename = "grx-3.0.h", cprefix = "Grx", lower_case_cprefix = "grx_", gir_namespace = "Grx", gir_version = "3.0")]
namespace Grx {
    public errordomain Error {
        FAILED,
        PLUGIN_FILE_NOT_FOUND,
        PLUGIN_SYMBOL_NOT_FOUND,
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

        public const Color VALUE_MASK;
        public const Color MODE_MASK;

        public const Color BLACK;
        public const Color WHITE;
        public const Color NONE;

        public static Color build_rgb (uint8 r, uint8 g, uint8 b);
        public static Color build_rgb_round (uint8 r, uint8 g, uint8 b);
        public static Color build_grayscale (uint8 r, uint8 g, uint8 b);
        public uint8 red { get; }
        public uint8 green { get; }
        public uint8 blue { get; }

        public static Color alloc (uint8 r, uint8 g, uint8 b);
        public static Color alloc_inline (uint8 r, uint8 g, uint8 b);
        public static Color alloc2 (uint hcolor);
        public static Color alloc2_inline (uint hcolor);
        public void free ();

        public static Color lookup (GLib.Array<Color> table, uint index);

        [CCode (array_length_cexpr = "16")]
        public static Color[] alloc_ega_colors ();

        public void query (Color c, out uint8 r, out uint8 g, out uint8 b);
        public void query_inline (Color c, out uint8 r, out uint8 g, out uint8 b);
        public void query2 (Color c, out uint hcolor);
        public void query2_inline (Color c, out uint hcolor);
    }

    [CCode (has_destroy_function = false, has_copy_function = false, has_type_id = false)]
    [SimpleType]
    public struct ColorCell : Color {
        public static ColorCell alloc ();
        public void set (uint8 r, uint8 g, uint8 b);
        public void free ();
    }

    [CCode (has_type_id = false)]
    public enum ColorPaletteType {
        GRAYSCALE,
        COLOR_TABLE,
        RGB,
    }

    namespace ColorInfo {
        public static void reset_colors ();
        public static void set_palette_type_rgb ();
        public static ColorPaletteType get_palette_type ();
        public static void refresh_colors ();

        public static int n_colors ();
        public static int n_free_colors ();

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
         * 4 color
         */
        LFB_2BPP,

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
         * 4 color
         */
        RAM_2BPP,

        /**
         * 16 color planar
         */
        RAM_4XBPP,

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
        public int get_bpp ();
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
    public uint get_dpi ();
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
        public int width { get; }
        public int height { get; }
        public int max_x { get; }
        public int max_y { get; }
        public static Context? new (int width, int height, [CCode (array_length = false)]char*[]? memory = null, out Context? where = null);
        public static Context? new_full (FrameMode mode, int width, int height, [CCode (array_length = false)]char*[]? memory = null, out Context? where = null);
        public static Context? new_subcontext (int x1, int y1, int x2, int y2, Context parent, out Context? where = null);
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

        public void bit_blt (int x, int y, Context src, int x1, int y1, int x2, int y2, Color op = (Color)ColorMode.WRITE);
        public void bit_blt_1bpp (int dx, int dy, Context src, int x1, int y1, int x2, int y2, Color fg, Color bg);
        public void fast_bit_blt(int x, int y, Context src, int x1, int y1, int x2, int y2, Color op = (Color)ColorMode.WRITE);

        public bool save_to_pbm (string file_name, string? comment = null) throws GLib.Error;
        public bool save_to_pgm (string file_name, string? comment = null) throws GLib.Error;
        public bool save_to_ppm (string file_name, string? comment = null) throws GLib.Error;
        public bool load_from_pnm (string file_name);

        public bool load_from_pnm_data (uint8 *buffer) throws GLib.Error;

        public bool save_to_png (string file_name) throws GLib.Error;
        public bool load_from_png(string file_name, bool use_alpha = true) throws GLib.Error;

        public bool load_from_jpeg (string file_name, int scale = 1) throws GLib.Error;
        public bool save_to_jpeg (string file_name, int quality = 90) throws GLib.Error;
        public bool save_to_jpeg_grayscale (string file_name, int quality = 90) throws GLib.Error;
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

    [CCode (has_type_id = false)]
    public enum ArcStyle {
        OPEN,
        CLOSED_CHORD,
        CLOSED_RADIUS
    }

    [CCode (has_type_id = false)]
    public struct FramedBoxColors {
        public Color background;
        public Color border_top;
        public Color border_right;
        public Color border_bottom;
        public Color border_left;
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
        draw_filled_circle_arc (x1 + radius, y1 + radius, radius, 900, 1800, ArcStyle.CLOSED_RADIUS, c);
        draw_filled_circle_arc (x2 - radius, y1 + radius, radius, 0, 900, ArcStyle.CLOSED_RADIUS, c);
        draw_filled_circle_arc (x1 + radius, y2 - radius, radius, 1800, 2700, ArcStyle.CLOSED_RADIUS, c);
        draw_filled_circle_arc (x2 - radius, y2 - radius, radius, 2700, 3600, ArcStyle.CLOSED_RADIUS, c);
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
    public void bit_blt (int x, int y, Context src, int x1, int y1, int x2, int y2, Color op = (Color)ColorMode.WRITE);
    public void bit_blt_1bpp (int dx, int dy, Context src, int x1, int y1, int x2, int y2, Color fg, Color bg);
    public void flood_fill (int x, int y, Color border, Color c);
    public void flood_spill (int x1, int y1, int x2, int y2, Color old_c, Color new_c);
    public void flood_spill_2 (int x1,  int y1, int x2, int y2, Color old_c1, Color new_c1, Color old_c2, Color new_c2);

    public Color get_pixel_at (int x, int y);

    public Color *get_scan_line (int  x1, int x2, int yy);
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
    public void fast_bit_blt(int x, int y, Context src, int x1, int y1, int x2, int y2, Color op = (Color)ColorMode.WRITE);

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

    [CCode (has_type_id = false)]
    public enum FontWeight {
        REGULAR,
        BOLD
    }

    [CCode (has_type_id = false)]
    public enum FontSlant {
        REGULAR,
        ITALIC
    }

    [CCode (has_type_id = false)]
    public enum FontWidth {
        NARROW,
        REGULAR,
        WIDE
    }

    [CCode (copy_function = "grx_font_ref", free_function = "grx_font_unref")]
    [Compact]
    public class Font {
        [CCode (cname = "grx_font_load_full")]
        public static Font load (string? family = null, int size = -1, int dpi = (int)get_dpi (),
            FontWeight weight = FontWeight.REGULAR, FontSlant slant = FontSlant.REGULAR,
            FontWidth width = FontWidth.REGULAR, bool monospace = false, string? lang = null,
            string? script = null) throws GLib.Error;
        public static Font load_from_file (string filename) throws GLib.Error;
        public unowned string family { get; }
        public unowned string style { get; }
        public int width { get; }
        public int height { get; }
        public int dump (Context context, int start, Color fg, Color bg);
        public int get_char_width (uint32 c);
        public int get_char_height (uint32 c);
        public int get_text_width (string? text);
        public int get_text_height (string? text);
    }

    [CCode (copy_function = "grx_text_options_ref", free_function = "grx_text_options_unref")]
    [Compact]
    public class TextOptions {
        [CCode (cname = "grx_text_options_new_full")]
        public TextOptions (Font font, Color fg, Color bg = Color.NONE, TextHAlign h_align = TextHAlign.LEFT, TextVAlign v_align = TextVAlign.TOP);
        public Font font { get; set; }
        public Color fg_color { get; set; }
        public Color bg_color { get; set; }
        public TextHAlign h_align { get; set; }
        public TextVAlign v_align { get; set; }
    }

    public void draw_text (string text, int x, int y, TextOptions options);

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
        public uint8[] dash_patterns;
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

        public static Pattern new_pixmap ([CCode (array_length = false)]char *pixels, int w, int h, GLib.Array<Color>? colors);
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
        public static Image new ([CCode (array_length = false)]char[] pixels, int w, int h, GLib.Array<Color> colors);
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
        public void bit_blt (int x, int y, Context src, int x1, int y1, int x2, int y2, Color oper = (Color)ColorMode.WRITE);
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

    [CCode (has_type_id = false)]
    public enum CursorModes {
        NORMAL,
        RUBBER,
        LINE,
        BOX
    }

    [CCode (copy_function = "grx_cursor_ref", free_function = "grx_cursor_unref")]
    [Compact]
    public class Cursor {
        public Cursor ([CCode (array_length = false)]char[] pixels, int pitch, int w, int h, int xo, int yo, GLib.Array<Color> table);

        [CCode (cname = "grx_cursor_show")]
        public void display ();
        [CCode (cname = "grx_cursor_hide")]
        public void erase ();
        [CCode (cname = "grx_cursor_move")]
        public void move (int x, int y);
    }

    namespace Mouse {
        public static Cursor? get_cursor ();
        public static void set_cursor (Cursor? cursor);
        public static void set_cursor_default (Color fill, Color outline);
        public static bool is_cursor_shown ();
        public static uint block (Context? context, int x1, int y1, int x2, int y2);
        public static void unblock (uint flags);
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

    public const int PNM_FORMAT_ERROR;

    /* The PNM functions */

    public bool query_pnm_file (string file_name, out PnmFormat format, out int width, out int height, out int maxval);
    public bool query_pnm_data (uint8 *buffer, out PnmFormat format, out int width, out int height, out int maxval);

    /* ================================================================== */
    /*                           PNG FUNCTIONS                            */
    /*  these functions may not be installed or available on all system   */
    /* ================================================================== */

    public bool query_png_file (string file_name, out int width, out int height);

    /* ================================================================== */
    /*                          JPEG FUNCTIONS                            */
    /*  these functions may not be installed or available on all system   */
    /* ================================================================== */

    public bool query_jpeg_file (string file_name, out int width, out int height);

    /* ================================================================== */
    /*               MISCELLANEOUS UTILITIY FUNCTIONS                     */
    /* ================================================================== */

    public void resize_gray_map (uint8 *map, int pitch, int old_width, int old_height, int new_width, int new_height);

    /* ================================================================== */
    /*                              INPUT                                 */
    /* ================================================================== */

    [CCode (has_type_id = false)]
    [Flags]
    public enum ModifierFlags {
        SHIFT,
        CTRL,
        ALT,
        SUPER,
    }

    [CCode (has_type_id = false)]
    public enum EventType {
        NONE,
        APP_ACTIVATE,
        APP_DEACTIVATE,
        APP_QUIT,
        KEY_UP,
        KEY_DOWN,
        POINTER_MOTION,
        BUTTON_PRESS,
        BUTTON_RELEASE,
        BUTTON_DOUBLE_PRESS,
        TOUCH_DOWN,
        TOUCH_MOTION,
        TOUCH_UP,
        TOUCH_CANCEL,
    }

    [CCode (has_type_id = false)]
    public struct KeyEvent {
        EventType type;
        Key keysym;
        unichar unichar;
        uint code;
        ModifierFlags modifiers;
        Device *device;
    }

    [CCode (has_type_id = false)]
    public struct MotionEvent {
        EventType type;
        int32 x;
        int32 y;
        ModifierFlags modifiers;
        Device *device;
    }

    [CCode (has_type_id = false)]
    public struct ButtonEvent {
        EventType type;
        uint button;
        ModifierFlags modifiers;
        Device *device;
    }

    [CCode (has_type_id = false)]
    public struct TouchEvent {
        EventType type;
        int id;
        int x;
        int y;
        ModifierFlags modifiers;
        Device *device;
    }

    /**
     * A union of all event types.
     */
    [CCode (has_type_id = false)]
    [Compact]
    public class Event {
        /**
         * The type of event.
         */
        public EventType type;

        /**
         * The event as a {@link KeyEvent}.
         */
        public KeyEvent key;

        /**
         * The event as a {@link MotionEvent}.
         */
        public MotionEvent motion;

        /**
         * The event as a {@link ButtonEvent}.
         */
        public ButtonEvent button;

        /**
         * The event as a {@link TouchEvent}.
         */
        public TouchEvent touch;

        /**
         * Gets the modifier keys for the event.
         *
         * Events that do not support modifier keys will always return `0`.
         */
        public ModifierFlags modifiers { get; }

        /**
         * Gets the device associated with the event.
         */
        public unowned Device? device { get; }

        /**
         * Gets the key symbol for a key event.
         *
         * @param keysym    the key symbol
         * @return          true if the event is a key event, otherwise false
         */
        public bool get_keysym (out Key keysym);

        /**
         * Gets the unicode character for a key event.
         *
         * @param keychar   the unicode character
         * @return          true if the event is a key event, otherwise false
         */
        public bool get_keychar (out unichar keychar);

        /**
         * Gets the platform dependant key code for the event.
         *
         * @param keycode   the key code
         * @return          true if the event is a key event, otherwise false
         */
        public bool get_keycode (out uint32 keycode);

        /**
         * Gets the screen coordinates of an event.
         *
         * @param x         the x coordinate
         * @param y         the y coordinate
         * @return          true if the event is a motion or touch event, otherwise false
         */
        public bool get_coords (out uint32 x, out uint32 y);

        /**
         * Gets the button for a button event
         *
         * @param button    the button index
         * @return          true if the event is a button event, otherwise false
         */
        public bool get_button (out uint32 button);
    }

    public abstract class Device : GLib.Object {
        public string name { get; }
        public string sysname { get; }
        public bool has_keyboard { get; }
        public bool has_pointer { get; }
        public bool has_touch { get; }
        public bool reset_calibration ();
    }

    public abstract class DeviceManager : GLib.Object {
        public signal void device_added (Device device);
        public signal void device_removed (Device device);
    }

    public class Application : GLib.Application, GLib.Initable {
        [CCode (cname = "grx_application_new_full")]
        public Application (string? id = null, GLib.ApplicationFlags flags = GLib.ApplicationFlags.FLAGS_NONE) throws GLib.Error;
        public virtual signal bool event (Event event);
        public bool is_active { [CCode (cname = "grx_application_is_active")]get; }
        public bool quit_on_signal;
    }

    [CCode (has_type_id = false)]
    public enum Key {
        /**
         * U+02D9 DOT ABOVE
         */
        ABOVEDOT,

        /**
         * Access X Enable
         */
        ACCESS_X_ENABLE,

        /**
         * Access X Feedback Enable
         */
        ACCESS_X_FEEDBACK_ENABLE,

        /**
         * U+00B4 ACUTE ACCENT
         */
        ACUTE,

        /**
         * Left alt
         */
        ALT_L,

        /**
         * Right alt
         */
        ALT_R,

        /**
         * U+0026 AMPERSAND
         */
        AMPERSAND,

        /**
         * U+0027 APOSTROPHE
         */
        APOSTROPHE,

        /**
         * U+2245 ALMOST EQUAL TO
         */
        APPROXEQ,

        /**
         * U+223C TILDE OPERATOR
         */
        APPROXIMATE,

        /**
         * U+0660 ARABIC-INDIC DIGIT ZERO
         */
        ARABIC_0,

        /**
         * U+0661 ARABIC-INDIC DIGIT ONE
         */
        ARABIC_1,

        /**
         * U+0662 ARABIC-INDIC DIGIT TWO
         */
        ARABIC_2,

        /**
         * U+0663 ARABIC-INDIC DIGIT THREE
         */
        ARABIC_3,

        /**
         * U+0664 ARABIC-INDIC DIGIT FOUR
         */
        ARABIC_4,

        /**
         * U+0665 ARABIC-INDIC DIGIT FIVE
         */
        ARABIC_5,

        /**
         * U+0666 ARABIC-INDIC DIGIT SIX
         */
        ARABIC_6,

        /**
         * U+0667 ARABIC-INDIC DIGIT SEVEN
         */
        ARABIC_7,

        /**
         * U+0668 ARABIC-INDIC DIGIT EIGHT
         */
        ARABIC_8,

        /**
         * U+0669 ARABIC-INDIC DIGIT NINE
         */
        ARABIC_9,

        /**
         * U+0639 ARABIC LETTER AIN
         */
        ARABIC_AIN,

        /**
         * U+0627 ARABIC LETTER ALEF
         */
        ARABIC_ALEF,

        /**
         * U+0649 ARABIC LETTER ALEF MAKSURA
         */
        ARABIC_ALEFMAKSURA,

        /**
         * U+0628 ARABIC LETTER BEH
         */
        ARABIC_BEH,

        /**
         * U+060C ARABIC COMMA
         */
        ARABIC_COMMA,

        /**
         * U+0636 ARABIC LETTER DAD
         */
        ARABIC_DAD,

        /**
         * U+062F ARABIC LETTER DAL
         */
        ARABIC_DAL,

        /**
         * U+064F ARABIC DAMMA
         */
        ARABIC_DAMMA,

        /**
         * U+064C ARABIC DAMMATAN
         */
        ARABIC_DAMMATAN,

        /**
         * U+0688 ARABIC LETTER DDAL
         */
        ARABIC_DDAL,

        /**
         * U+06CC ARABIC LETTER FARSI YEH
         */
        ARABIC_FARSI_YEH,

        /**
         * U+064E ARABIC FATHA
         */
        ARABIC_FATHA,

        /**
         * U+064B ARABIC FATHATAN
         */
        ARABIC_FATHATAN,

        /**
         * U+0641 ARABIC LETTER FEH
         */
        ARABIC_FEH,

        /**
         * U+06D4 ARABIC FULL STOP
         */
        ARABIC_FULLSTOP,

        /**
         * U+06AF ARABIC LETTER GAF
         */
        ARABIC_GAF,

        /**
         * U+063A ARABIC LETTER GHAIN
         */
        ARABIC_GHAIN,

        /**
         * U+0647 ARABIC LETTER HEH
         */
        ARABIC_HA,

        /**
         * U+062D ARABIC LETTER HAH
         */
        ARABIC_HAH,

        /**
         * U+0621 ARABIC LETTER HAMZA
         */
        ARABIC_HAMZA,

        /**
         * U+0623 ARABIC LETTER ALEF WITH HAMZA ABOVE
         */
        ARABIC_HAMZAONALEF,

        /**
         * U+0624 ARABIC LETTER WAW WITH HAMZA ABOVE
         */
        ARABIC_HAMZAONWAW,

        /**
         * U+0626 ARABIC LETTER YEH WITH HAMZA ABOVE
         */
        ARABIC_HAMZAONYEH,

        /**
         * U+0625 ARABIC LETTER ALEF WITH HAMZA BELOW
         */
        ARABIC_HAMZAUNDERALEF,

        /**
         * U+0654 ARABIC HAMZA ABOVE
         */
        ARABIC_HAMZA_ABOVE,

        /**
         * U+0655 ARABIC HAMZA BELOW
         */
        ARABIC_HAMZA_BELOW,

        /**
         * deprecated
         */
        ARABIC_HEH,

        /**
         * U+06BE ARABIC LETTER HEH DOACHASHMEE
         */
        ARABIC_HEH_DOACHASHMEE,

        /**
         * U+06C1 ARABIC LETTER HEH GOAL
         */
        ARABIC_HEH_GOAL,

        /**
         * U+062C ARABIC LETTER JEEM
         */
        ARABIC_JEEM,

        /**
         * U+0698 ARABIC LETTER JEH
         */
        ARABIC_JEH,

        /**
         * U+0643 ARABIC LETTER KAF
         */
        ARABIC_KAF,

        /**
         * U+0650 ARABIC KASRA
         */
        ARABIC_KASRA,

        /**
         * U+064D ARABIC KASRATAN
         */
        ARABIC_KASRATAN,

        /**
         * U+06A9 ARABIC LETTER KEHEH
         */
        ARABIC_KEHEH,

        /**
         * U+062E ARABIC LETTER KHAH
         */
        ARABIC_KHAH,

        /**
         * U+0644 ARABIC LETTER LAM
         */
        ARABIC_LAM,

        /**
         * U+0622 ARABIC LETTER ALEF WITH MADDA ABOVE
         */
        ARABIC_MADDAONALEF,

        /**
         * U+0653 ARABIC MADDAH ABOVE
         */
        ARABIC_MADDA_ABOVE,

        /**
         * U+0645 ARABIC LETTER MEEM
         */
        ARABIC_MEEM,

        /**
         * U+0646 ARABIC LETTER NOON
         */
        ARABIC_NOON,

        /**
         * U+06BA ARABIC LETTER NOON GHUNNA
         */
        ARABIC_NOON_GHUNNA,

        /**
         * U+067E ARABIC LETTER PEH
         */
        ARABIC_PEH,

        /**
         * U+066A ARABIC PERCENT SIGN
         */
        ARABIC_PERCENT,

        /**
         * U+0642 ARABIC LETTER QAF
         */
        ARABIC_QAF,

        /**
         * U+061F ARABIC QUESTION MARK
         */
        ARABIC_QUESTION_MARK,

        /**
         * U+0631 ARABIC LETTER REH
         */
        ARABIC_RA,

        /**
         * U+0691 ARABIC LETTER RREH
         */
        ARABIC_RREH,

        /**
         * U+0635 ARABIC LETTER SAD
         */
        ARABIC_SAD,

        /**
         * U+0633 ARABIC LETTER SEEN
         */
        ARABIC_SEEN,

        /**
         * U+061B ARABIC SEMICOLON
         */
        ARABIC_SEMICOLON,

        /**
         * U+0651 ARABIC SHADDA
         */
        ARABIC_SHADDA,

        /**
         * U+0634 ARABIC LETTER SHEEN
         */
        ARABIC_SHEEN,

        /**
         * U+0652 ARABIC SUKUN
         */
        ARABIC_SUKUN,

        /**
         * U+0670 ARABIC LETTER SUPERSCRIPT ALEF
         */
        ARABIC_SUPERSCRIPT_ALEF,

        /**
         * Alias for mode_switch
         */
        ARABIC_SWITCH,

        /**
         * U+0637 ARABIC LETTER TAH
         */
        ARABIC_TAH,

        /**
         * U+0640 ARABIC TATWEEL
         */
        ARABIC_TATWEEL,

        /**
         * U+0686 ARABIC LETTER TCHEH
         */
        ARABIC_TCHEH,

        /**
         * U+062A ARABIC LETTER TEH
         */
        ARABIC_TEH,

        /**
         * U+0629 ARABIC LETTER TEH MARBUTA
         */
        ARABIC_TEHMARBUTA,

        /**
         * U+0630 ARABIC LETTER THAL
         */
        ARABIC_THAL,

        /**
         * U+062B ARABIC LETTER THEH
         */
        ARABIC_THEH,

        /**
         * U+0679 ARABIC LETTER TTEH
         */
        ARABIC_TTEH,

        /**
         * U+06A4 ARABIC LETTER VEH
         */
        ARABIC_VEH,

        /**
         * U+0648 ARABIC LETTER WAW
         */
        ARABIC_WAW,

        /**
         * U+064A ARABIC LETTER YEH
         */
        ARABIC_YEH,

        /**
         * U+06D2 ARABIC LETTER YEH BARREE
         */
        ARABIC_YEH_BAREE,

        /**
         * U+0638 ARABIC LETTER ZAH
         */
        ARABIC_ZAH,

        /**
         * U+0632 ARABIC LETTER ZAIN
         */
        ARABIC_ZAIN,

        /**
         * U+055B ARMENIAN EMPHASIS MARK
         */
        ARMENIAN_ACCENT,

        /**
         * U+055C ARMENIAN EXCLAMATION MARK
         */
        ARMENIAN_AMANAK,

        /**
         * U+055A ARMENIAN APOSTROPHE
         */
        ARMENIAN_APOSTROPHE,

        /**
         * U+055D ARMENIAN COMMA
         */
        ARMENIAN_BUT,

        /**
         * U+055C ARMENIAN EXCLAMATION MARK
         */
        ARMENIAN_EXCLAM,

        /**
         * U+0589 ARMENIAN FULL STOP
         */
        ARMENIAN_FULL_STOP,

        /**
         * U+058A ARMENIAN HYPHEN
         */
        ARMENIAN_HYPHEN,

        /**
         * U+0568 ARMENIAN SMALL LETTER ET
         */
        ARMENIAN_LCASE_AT,

        /**
         * U+0561 ARMENIAN SMALL LETTER AYB
         */
        ARMENIAN_LCASE_AYB,

        /**
         * U+0562 ARMENIAN SMALL LETTER BEN
         */
        ARMENIAN_LCASE_BEN,

        /**
         * U+0579 ARMENIAN SMALL LETTER CHA
         */
        ARMENIAN_LCASE_CHA,

        /**
         * U+0564 ARMENIAN SMALL LETTER DA
         */
        ARMENIAN_LCASE_DA,

        /**
         * U+0571 ARMENIAN SMALL LETTER JA
         */
        ARMENIAN_LCASE_DZA,

        /**
         * U+0567 ARMENIAN SMALL LETTER EH
         */
        ARMENIAN_LCASE_E,

        /**
         * U+0586 ARMENIAN SMALL LETTER FEH
         */
        ARMENIAN_LCASE_FE,

        /**
         * U+0572 ARMENIAN SMALL LETTER GHAD
         */
        ARMENIAN_LCASE_GHAT,

        /**
         * U+0563 ARMENIAN SMALL LETTER GIM
         */
        ARMENIAN_LCASE_GIM,

        /**
         * U+0575 ARMENIAN SMALL LETTER YI
         */
        ARMENIAN_LCASE_HI,

        /**
         * U+0570 ARMENIAN SMALL LETTER HO
         */
        ARMENIAN_LCASE_HO,

        /**
         * U+056B ARMENIAN SMALL LETTER INI
         */
        ARMENIAN_LCASE_INI,

        /**
         * U+057B ARMENIAN SMALL LETTER JHEH
         */
        ARMENIAN_LCASE_JE,

        /**
         * U+0584 ARMENIAN SMALL LETTER KEH
         */
        ARMENIAN_LCASE_KE,

        /**
         * U+056F ARMENIAN SMALL LETTER KEN
         */
        ARMENIAN_LCASE_KEN,

        /**
         * U+056D ARMENIAN SMALL LETTER XEH
         */
        ARMENIAN_LCASE_KHE,

        /**
         * U+056C ARMENIAN SMALL LETTER LIWN
         */
        ARMENIAN_LCASE_LYUN,

        /**
         * U+0574 ARMENIAN SMALL LETTER MEN
         */
        ARMENIAN_LCASE_MEN,

        /**
         * U+0576 ARMENIAN SMALL LETTER NOW
         */
        ARMENIAN_LCASE_NU,

        /**
         * U+0585 ARMENIAN SMALL LETTER OH
         */
        ARMENIAN_LCASE_O,

        /**
         * U+057A ARMENIAN SMALL LETTER PEH
         */
        ARMENIAN_LCASE_PE,

        /**
         * U+0583 ARMENIAN SMALL LETTER PIWR
         */
        ARMENIAN_LCASE_PYUR,

        /**
         * U+057C ARMENIAN SMALL LETTER RA
         */
        ARMENIAN_LCASE_RA,

        /**
         * U+0580 ARMENIAN SMALL LETTER REH
         */
        ARMENIAN_LCASE_RE,

        /**
         * U+057D ARMENIAN SMALL LETTER SEH
         */
        ARMENIAN_LCASE_SE,

        /**
         * U+0577 ARMENIAN SMALL LETTER SHA
         */
        ARMENIAN_LCASE_SHA,

        /**
         * U+0573 ARMENIAN SMALL LETTER CHEH
         */
        ARMENIAN_LCASE_TCHE,

        /**
         * U+0569 ARMENIAN SMALL LETTER TO
         */
        ARMENIAN_LCASE_TO,

        /**
         * U+056E ARMENIAN SMALL LETTER CA
         */
        ARMENIAN_LCASE_TSA,

        /**
         * U+0581 ARMENIAN SMALL LETTER CO
         */
        ARMENIAN_LCASE_TSO,

        /**
         * U+057F ARMENIAN SMALL LETTER TIWN
         */
        ARMENIAN_LCASE_TYUN,

        /**
         * U+057E ARMENIAN SMALL LETTER VEW
         */
        ARMENIAN_LCASE_VEV,

        /**
         * U+0578 ARMENIAN SMALL LETTER VO
         */
        ARMENIAN_LCASE_VO,

        /**
         * U+0582 ARMENIAN SMALL LETTER YIWN
         */
        ARMENIAN_LCASE_VYUN,

        /**
         * U+0565 ARMENIAN SMALL LETTER ECH
         */
        ARMENIAN_LCASE_YECH,

        /**
         * U+0566 ARMENIAN SMALL LETTER ZA
         */
        ARMENIAN_LCASE_ZA,

        /**
         * U+056A ARMENIAN SMALL LETTER ZHE
         */
        ARMENIAN_LCASE_ZHE,

        /**
         * U+0587 ARMENIAN SMALL LIGATURE ECH YIWN
         */
        ARMENIAN_LIGATURE_EW,

        /**
         * U+055E ARMENIAN QUESTION MARK
         */
        ARMENIAN_PARUYK,

        /**
         * U+055E ARMENIAN QUESTION MARK
         */
        ARMENIAN_QUESTION,

        /**
         * U+055D ARMENIAN COMMA
         */
        ARMENIAN_SEPARATION_MARK,

        /**
         * U+055B ARMENIAN EMPHASIS MARK
         */
        ARMENIAN_SHESHT,

        /**
         * U+0538 ARMENIAN CAPITAL LETTER ET
         */
        ARMENIAN_UCASE_AT,

        /**
         * U+0531 ARMENIAN CAPITAL LETTER AYB
         */
        ARMENIAN_UCASE_AYB,

        /**
         * U+0532 ARMENIAN CAPITAL LETTER BEN
         */
        ARMENIAN_UCASE_BEN,

        /**
         * U+0549 ARMENIAN CAPITAL LETTER CHA
         */
        ARMENIAN_UCASE_CHA,

        /**
         * U+0534 ARMENIAN CAPITAL LETTER DA
         */
        ARMENIAN_UCASE_DA,

        /**
         * U+0541 ARMENIAN CAPITAL LETTER JA
         */
        ARMENIAN_UCASE_DZA,

        /**
         * U+0537 ARMENIAN CAPITAL LETTER EH
         */
        ARMENIAN_UCASE_E,

        /**
         * U+0556 ARMENIAN CAPITAL LETTER FEH
         */
        ARMENIAN_UCASE_FE,

        /**
         * U+0542 ARMENIAN CAPITAL LETTER GHAD
         */
        ARMENIAN_UCASE_GHAT,

        /**
         * U+0533 ARMENIAN CAPITAL LETTER GIM
         */
        ARMENIAN_UCASE_GIM,

        /**
         * U+0545 ARMENIAN CAPITAL LETTER YI
         */
        ARMENIAN_UCASE_HI,

        /**
         * U+0540 ARMENIAN CAPITAL LETTER HO
         */
        ARMENIAN_UCASE_HO,

        /**
         * U+053B ARMENIAN CAPITAL LETTER INI
         */
        ARMENIAN_UCASE_INI,

        /**
         * U+054B ARMENIAN CAPITAL LETTER JHEH
         */
        ARMENIAN_UCASE_JE,

        /**
         * U+0554 ARMENIAN CAPITAL LETTER KEH
         */
        ARMENIAN_UCASE_KE,

        /**
         * U+053F ARMENIAN CAPITAL LETTER KEN
         */
        ARMENIAN_UCASE_KEN,

        /**
         * U+053D ARMENIAN CAPITAL LETTER XEH
         */
        ARMENIAN_UCASE_KHE,

        /**
         * U+053C ARMENIAN CAPITAL LETTER LIWN
         */
        ARMENIAN_UCASE_LYUN,

        /**
         * U+0544 ARMENIAN CAPITAL LETTER MEN
         */
        ARMENIAN_UCASE_MEN,

        /**
         * U+0546 ARMENIAN CAPITAL LETTER NOW
         */
        ARMENIAN_UCASE_NU,

        /**
         * U+0555 ARMENIAN CAPITAL LETTER OH
         */
        ARMENIAN_UCASE_O,

        /**
         * U+054A ARMENIAN CAPITAL LETTER PEH
         */
        ARMENIAN_UCASE_PE,

        /**
         * U+0553 ARMENIAN CAPITAL LETTER PIWR
         */
        ARMENIAN_UCASE_PYUR,

        /**
         * U+054C ARMENIAN CAPITAL LETTER RA
         */
        ARMENIAN_UCASE_RA,

        /**
         * U+0550 ARMENIAN CAPITAL LETTER REH
         */
        ARMENIAN_UCASE_RE,

        /**
         * U+054D ARMENIAN CAPITAL LETTER SEH
         */
        ARMENIAN_UCASE_SE,

        /**
         * U+0547 ARMENIAN CAPITAL LETTER SHA
         */
        ARMENIAN_UCASE_SHA,

        /**
         * U+0543 ARMENIAN CAPITAL LETTER CHEH
         */
        ARMENIAN_UCASE_TCHE,

        /**
         * U+0539 ARMENIAN CAPITAL LETTER TO
         */
        ARMENIAN_UCASE_TO,

        /**
         * U+053E ARMENIAN CAPITAL LETTER CA
         */
        ARMENIAN_UCASE_TSA,

        /**
         * U+0551 ARMENIAN CAPITAL LETTER CO
         */
        ARMENIAN_UCASE_TSO,

        /**
         * U+054F ARMENIAN CAPITAL LETTER TIWN
         */
        ARMENIAN_UCASE_TYUN,

        /**
         * U+054E ARMENIAN CAPITAL LETTER VEW
         */
        ARMENIAN_UCASE_VEV,

        /**
         * U+0548 ARMENIAN CAPITAL LETTER VO
         */
        ARMENIAN_UCASE_VO,

        /**
         * U+0552 ARMENIAN CAPITAL LETTER YIWN
         */
        ARMENIAN_UCASE_VYUN,

        /**
         * U+0535 ARMENIAN CAPITAL LETTER ECH
         */
        ARMENIAN_UCASE_YECH,

        /**
         * U+0536 ARMENIAN CAPITAL LETTER ZA
         */
        ARMENIAN_UCASE_ZA,

        /**
         * U+053A ARMENIAN CAPITAL LETTER ZHE
         */
        ARMENIAN_UCASE_ZHE,

        /**
         * U+0589 ARMENIAN FULL STOP
         */
        ARMENIAN_VERJAKET,

        /**
         * U+058A ARMENIAN HYPHEN
         */
        ARMENIAN_YENTAMNA,

        /**
         * U+005E CIRCUMFLEX ACCENT
         */
        ASCIICIRCUM,

        /**
         * U+007E TILDE
         */
        ASCIITILDE,

        /**
         * U+002A ASTERISK
         */
        ASTERISK,

        /**
         * U+0040 COMMERCIAL AT
         */
        AT,

        /**
         * Audible Bell Enable
         */
        AUDIBLE_BELL_ENABLE,

        /**
         * U+005C REVERSE SOLIDUS
         */
        BACKSLASH,

        /**
         * Back space, back char
         */
        BACK_SPACE,

        /**
         * Back Tab
         */
        BACK_TAB,

        /**
         * U+2717 BALLOT X
         */
        BALLOTCROSS,

        /**
         * U+007C VERTICAL LINE
         */
        BAR,

        /**
         * U+2235 BECAUSE
         */
        BECAUSE,

        /**
         * BOL
         */
        BEGIN,

        /**
         * Blank
         */
        BLANK,

        /**
         * Block
         */
        BLOCK,

        /**
         * U+2321 BOTTOM HALF INTEGRAL
         */
        BOTINTEGRAL,

        /**
         * U+239D LEFT PARENTHESIS LOWER HOOK
         */
        BOTLEFTPARENS,

        /**
         * U+23A3 LEFT SQUARE BRACKET LOWER CORNER
         */
        BOTLEFTSQBRACKET,

        /**
         * Botleftsummation
         */
        BOTLEFTSUMMATION,

        /**
         * U+23A0 RIGHT PARENTHESIS LOWER HOOK
         */
        BOTRIGHTPARENS,

        /**
         * U+23A6 RIGHT SQUARE BRACKET LOWER CORNER
         */
        BOTRIGHTSQBRACKET,

        /**
         * Botrightsummation
         */
        BOTRIGHTSUMMATION,

        /**
         * U+2534 BOX DRAWINGS LIGHT UP AND HORIZONTAL
         */
        BOTT,

        /**
         * Botvertsummationconnector
         */
        BOTVERTSUMMATIONCONNECTOR,

        /**
         * Bounce Keys Enable
         */
        BOUNCE_KEYS_ENABLE,

        /**
         * U+007B LEFT CURLY BRACKET
         */
        BRACELEFT,

        /**
         * U+007D RIGHT CURLY BRACKET
         */
        BRACERIGHT,

        /**
         * U+005B LEFT SQUARE BRACKET
         */
        BRACKETLEFT,

        /**
         * U+005D RIGHT SQUARE BRACKET
         */
        BRACKETRIGHT,

        /**
         * U+2800 BRAILLE PATTERN BLANK
         */
        BRAILLE_BLANK,

        /**
         * U+2801 BRAILLE PATTERN DOTS-1
         */
        BRAILLE_DOTS_1,

        /**
         * U+2803 BRAILLE PATTERN DOTS-12
         */
        BRAILLE_DOTS_12,

        /**
         * U+2807 BRAILLE PATTERN DOTS-123
         */
        BRAILLE_DOTS_123,

        /**
         * U+280f BRAILLE PATTERN DOTS-1234
         */
        BRAILLE_DOTS_1234,

        /**
         * U+281f BRAILLE PATTERN DOTS-12345
         */
        BRAILLE_DOTS_12345,

        /**
         * U+283f BRAILLE PATTERN DOTS-123456
         */
        BRAILLE_DOTS_123456,

        /**
         * U+287f BRAILLE PATTERN DOTS-1234567
         */
        BRAILLE_DOTS_1234567,

        /**
         * U+28ff BRAILLE PATTERN DOTS-12345678
         */
        BRAILLE_DOTS_12345678,

        /**
         * U+28bf BRAILLE PATTERN DOTS-1234568
         */
        BRAILLE_DOTS_1234568,

        /**
         * U+285f BRAILLE PATTERN DOTS-123457
         */
        BRAILLE_DOTS_123457,

        /**
         * U+28df BRAILLE PATTERN DOTS-1234578
         */
        BRAILLE_DOTS_1234578,

        /**
         * U+289f BRAILLE PATTERN DOTS-123458
         */
        BRAILLE_DOTS_123458,

        /**
         * U+282f BRAILLE PATTERN DOTS-12346
         */
        BRAILLE_DOTS_12346,

        /**
         * U+286f BRAILLE PATTERN DOTS-123467
         */
        BRAILLE_DOTS_123467,

        /**
         * U+28ef BRAILLE PATTERN DOTS-1234678
         */
        BRAILLE_DOTS_1234678,

        /**
         * U+28af BRAILLE PATTERN DOTS-123468
         */
        BRAILLE_DOTS_123468,

        /**
         * U+284f BRAILLE PATTERN DOTS-12347
         */
        BRAILLE_DOTS_12347,

        /**
         * U+28cf BRAILLE PATTERN DOTS-123478
         */
        BRAILLE_DOTS_123478,

        /**
         * U+288f BRAILLE PATTERN DOTS-12348
         */
        BRAILLE_DOTS_12348,

        /**
         * U+2817 BRAILLE PATTERN DOTS-1235
         */
        BRAILLE_DOTS_1235,

        /**
         * U+2837 BRAILLE PATTERN DOTS-12356
         */
        BRAILLE_DOTS_12356,

        /**
         * U+2877 BRAILLE PATTERN DOTS-123567
         */
        BRAILLE_DOTS_123567,

        /**
         * U+28f7 BRAILLE PATTERN DOTS-1235678
         */
        BRAILLE_DOTS_1235678,

        /**
         * U+28b7 BRAILLE PATTERN DOTS-123568
         */
        BRAILLE_DOTS_123568,

        /**
         * U+2857 BRAILLE PATTERN DOTS-12357
         */
        BRAILLE_DOTS_12357,

        /**
         * U+28d7 BRAILLE PATTERN DOTS-123578
         */
        BRAILLE_DOTS_123578,

        /**
         * U+2897 BRAILLE PATTERN DOTS-12358
         */
        BRAILLE_DOTS_12358,

        /**
         * U+2827 BRAILLE PATTERN DOTS-1236
         */
        BRAILLE_DOTS_1236,

        /**
         * U+2867 BRAILLE PATTERN DOTS-12367
         */
        BRAILLE_DOTS_12367,

        /**
         * U+28e7 BRAILLE PATTERN DOTS-123678
         */
        BRAILLE_DOTS_123678,

        /**
         * U+28a7 BRAILLE PATTERN DOTS-12368
         */
        BRAILLE_DOTS_12368,

        /**
         * U+2847 BRAILLE PATTERN DOTS-1237
         */
        BRAILLE_DOTS_1237,

        /**
         * U+28c7 BRAILLE PATTERN DOTS-12378
         */
        BRAILLE_DOTS_12378,

        /**
         * U+2887 BRAILLE PATTERN DOTS-1238
         */
        BRAILLE_DOTS_1238,

        /**
         * U+280b BRAILLE PATTERN DOTS-124
         */
        BRAILLE_DOTS_124,

        /**
         * U+281b BRAILLE PATTERN DOTS-1245
         */
        BRAILLE_DOTS_1245,

        /**
         * U+283b BRAILLE PATTERN DOTS-12456
         */
        BRAILLE_DOTS_12456,

        /**
         * U+287b BRAILLE PATTERN DOTS-124567
         */
        BRAILLE_DOTS_124567,

        /**
         * U+28fb BRAILLE PATTERN DOTS-1245678
         */
        BRAILLE_DOTS_1245678,

        /**
         * U+28bb BRAILLE PATTERN DOTS-124568
         */
        BRAILLE_DOTS_124568,

        /**
         * U+285b BRAILLE PATTERN DOTS-12457
         */
        BRAILLE_DOTS_12457,

        /**
         * U+28db BRAILLE PATTERN DOTS-124578
         */
        BRAILLE_DOTS_124578,

        /**
         * U+289b BRAILLE PATTERN DOTS-12458
         */
        BRAILLE_DOTS_12458,

        /**
         * U+282b BRAILLE PATTERN DOTS-1246
         */
        BRAILLE_DOTS_1246,

        /**
         * U+286b BRAILLE PATTERN DOTS-12467
         */
        BRAILLE_DOTS_12467,

        /**
         * U+28eb BRAILLE PATTERN DOTS-124678
         */
        BRAILLE_DOTS_124678,

        /**
         * U+28ab BRAILLE PATTERN DOTS-12468
         */
        BRAILLE_DOTS_12468,

        /**
         * U+284b BRAILLE PATTERN DOTS-1247
         */
        BRAILLE_DOTS_1247,

        /**
         * U+28cb BRAILLE PATTERN DOTS-12478
         */
        BRAILLE_DOTS_12478,

        /**
         * U+288b BRAILLE PATTERN DOTS-1248
         */
        BRAILLE_DOTS_1248,

        /**
         * U+2813 BRAILLE PATTERN DOTS-125
         */
        BRAILLE_DOTS_125,

        /**
         * U+2833 BRAILLE PATTERN DOTS-1256
         */
        BRAILLE_DOTS_1256,

        /**
         * U+2873 BRAILLE PATTERN DOTS-12567
         */
        BRAILLE_DOTS_12567,

        /**
         * U+28f3 BRAILLE PATTERN DOTS-125678
         */
        BRAILLE_DOTS_125678,

        /**
         * U+28b3 BRAILLE PATTERN DOTS-12568
         */
        BRAILLE_DOTS_12568,

        /**
         * U+2853 BRAILLE PATTERN DOTS-1257
         */
        BRAILLE_DOTS_1257,

        /**
         * U+28d3 BRAILLE PATTERN DOTS-12578
         */
        BRAILLE_DOTS_12578,

        /**
         * U+2893 BRAILLE PATTERN DOTS-1258
         */
        BRAILLE_DOTS_1258,

        /**
         * U+2823 BRAILLE PATTERN DOTS-126
         */
        BRAILLE_DOTS_126,

        /**
         * U+2863 BRAILLE PATTERN DOTS-1267
         */
        BRAILLE_DOTS_1267,

        /**
         * U+28e3 BRAILLE PATTERN DOTS-12678
         */
        BRAILLE_DOTS_12678,

        /**
         * U+28a3 BRAILLE PATTERN DOTS-1268
         */
        BRAILLE_DOTS_1268,

        /**
         * U+2843 BRAILLE PATTERN DOTS-127
         */
        BRAILLE_DOTS_127,

        /**
         * U+28c3 BRAILLE PATTERN DOTS-1278
         */
        BRAILLE_DOTS_1278,

        /**
         * U+2883 BRAILLE PATTERN DOTS-128
         */
        BRAILLE_DOTS_128,

        /**
         * U+2805 BRAILLE PATTERN DOTS-13
         */
        BRAILLE_DOTS_13,

        /**
         * U+280d BRAILLE PATTERN DOTS-134
         */
        BRAILLE_DOTS_134,

        /**
         * U+281d BRAILLE PATTERN DOTS-1345
         */
        BRAILLE_DOTS_1345,

        /**
         * U+283d BRAILLE PATTERN DOTS-13456
         */
        BRAILLE_DOTS_13456,

        /**
         * U+287d BRAILLE PATTERN DOTS-134567
         */
        BRAILLE_DOTS_134567,

        /**
         * U+28fd BRAILLE PATTERN DOTS-1345678
         */
        BRAILLE_DOTS_1345678,

        /**
         * U+28bd BRAILLE PATTERN DOTS-134568
         */
        BRAILLE_DOTS_134568,

        /**
         * U+285d BRAILLE PATTERN DOTS-13457
         */
        BRAILLE_DOTS_13457,

        /**
         * U+28dd BRAILLE PATTERN DOTS-134578
         */
        BRAILLE_DOTS_134578,

        /**
         * U+289d BRAILLE PATTERN DOTS-13458
         */
        BRAILLE_DOTS_13458,

        /**
         * U+282d BRAILLE PATTERN DOTS-1346
         */
        BRAILLE_DOTS_1346,

        /**
         * U+286d BRAILLE PATTERN DOTS-13467
         */
        BRAILLE_DOTS_13467,

        /**
         * U+28ed BRAILLE PATTERN DOTS-134678
         */
        BRAILLE_DOTS_134678,

        /**
         * U+28ad BRAILLE PATTERN DOTS-13468
         */
        BRAILLE_DOTS_13468,

        /**
         * U+284d BRAILLE PATTERN DOTS-1347
         */
        BRAILLE_DOTS_1347,

        /**
         * U+28cd BRAILLE PATTERN DOTS-13478
         */
        BRAILLE_DOTS_13478,

        /**
         * U+288d BRAILLE PATTERN DOTS-1348
         */
        BRAILLE_DOTS_1348,

        /**
         * U+2815 BRAILLE PATTERN DOTS-135
         */
        BRAILLE_DOTS_135,

        /**
         * U+2835 BRAILLE PATTERN DOTS-1356
         */
        BRAILLE_DOTS_1356,

        /**
         * U+2875 BRAILLE PATTERN DOTS-13567
         */
        BRAILLE_DOTS_13567,

        /**
         * U+28f5 BRAILLE PATTERN DOTS-135678
         */
        BRAILLE_DOTS_135678,

        /**
         * U+28b5 BRAILLE PATTERN DOTS-13568
         */
        BRAILLE_DOTS_13568,

        /**
         * U+2855 BRAILLE PATTERN DOTS-1357
         */
        BRAILLE_DOTS_1357,

        /**
         * U+28d5 BRAILLE PATTERN DOTS-13578
         */
        BRAILLE_DOTS_13578,

        /**
         * U+2895 BRAILLE PATTERN DOTS-1358
         */
        BRAILLE_DOTS_1358,

        /**
         * U+2825 BRAILLE PATTERN DOTS-136
         */
        BRAILLE_DOTS_136,

        /**
         * U+2865 BRAILLE PATTERN DOTS-1367
         */
        BRAILLE_DOTS_1367,

        /**
         * U+28e5 BRAILLE PATTERN DOTS-13678
         */
        BRAILLE_DOTS_13678,

        /**
         * U+28a5 BRAILLE PATTERN DOTS-1368
         */
        BRAILLE_DOTS_1368,

        /**
         * U+2845 BRAILLE PATTERN DOTS-137
         */
        BRAILLE_DOTS_137,

        /**
         * U+28c5 BRAILLE PATTERN DOTS-1378
         */
        BRAILLE_DOTS_1378,

        /**
         * U+2885 BRAILLE PATTERN DOTS-138
         */
        BRAILLE_DOTS_138,

        /**
         * U+2809 BRAILLE PATTERN DOTS-14
         */
        BRAILLE_DOTS_14,

        /**
         * U+2819 BRAILLE PATTERN DOTS-145
         */
        BRAILLE_DOTS_145,

        /**
         * U+2839 BRAILLE PATTERN DOTS-1456
         */
        BRAILLE_DOTS_1456,

        /**
         * U+2879 BRAILLE PATTERN DOTS-14567
         */
        BRAILLE_DOTS_14567,

        /**
         * U+28f9 BRAILLE PATTERN DOTS-145678
         */
        BRAILLE_DOTS_145678,

        /**
         * U+28b9 BRAILLE PATTERN DOTS-14568
         */
        BRAILLE_DOTS_14568,

        /**
         * U+2859 BRAILLE PATTERN DOTS-1457
         */
        BRAILLE_DOTS_1457,

        /**
         * U+28d9 BRAILLE PATTERN DOTS-14578
         */
        BRAILLE_DOTS_14578,

        /**
         * U+2899 BRAILLE PATTERN DOTS-1458
         */
        BRAILLE_DOTS_1458,

        /**
         * U+2829 BRAILLE PATTERN DOTS-146
         */
        BRAILLE_DOTS_146,

        /**
         * U+2869 BRAILLE PATTERN DOTS-1467
         */
        BRAILLE_DOTS_1467,

        /**
         * U+28e9 BRAILLE PATTERN DOTS-14678
         */
        BRAILLE_DOTS_14678,

        /**
         * U+28a9 BRAILLE PATTERN DOTS-1468
         */
        BRAILLE_DOTS_1468,

        /**
         * U+2849 BRAILLE PATTERN DOTS-147
         */
        BRAILLE_DOTS_147,

        /**
         * U+28c9 BRAILLE PATTERN DOTS-1478
         */
        BRAILLE_DOTS_1478,

        /**
         * U+2889 BRAILLE PATTERN DOTS-148
         */
        BRAILLE_DOTS_148,

        /**
         * U+2811 BRAILLE PATTERN DOTS-15
         */
        BRAILLE_DOTS_15,

        /**
         * U+2831 BRAILLE PATTERN DOTS-156
         */
        BRAILLE_DOTS_156,

        /**
         * U+2871 BRAILLE PATTERN DOTS-1567
         */
        BRAILLE_DOTS_1567,

        /**
         * U+28f1 BRAILLE PATTERN DOTS-15678
         */
        BRAILLE_DOTS_15678,

        /**
         * U+28b1 BRAILLE PATTERN DOTS-1568
         */
        BRAILLE_DOTS_1568,

        /**
         * U+2851 BRAILLE PATTERN DOTS-157
         */
        BRAILLE_DOTS_157,

        /**
         * U+28d1 BRAILLE PATTERN DOTS-1578
         */
        BRAILLE_DOTS_1578,

        /**
         * U+2891 BRAILLE PATTERN DOTS-158
         */
        BRAILLE_DOTS_158,

        /**
         * U+2821 BRAILLE PATTERN DOTS-16
         */
        BRAILLE_DOTS_16,

        /**
         * U+2861 BRAILLE PATTERN DOTS-167
         */
        BRAILLE_DOTS_167,

        /**
         * U+28e1 BRAILLE PATTERN DOTS-1678
         */
        BRAILLE_DOTS_1678,

        /**
         * U+28a1 BRAILLE PATTERN DOTS-168
         */
        BRAILLE_DOTS_168,

        /**
         * U+2841 BRAILLE PATTERN DOTS-17
         */
        BRAILLE_DOTS_17,

        /**
         * U+28c1 BRAILLE PATTERN DOTS-178
         */
        BRAILLE_DOTS_178,

        /**
         * U+2881 BRAILLE PATTERN DOTS-18
         */
        BRAILLE_DOTS_18,

        /**
         * U+2802 BRAILLE PATTERN DOTS-2
         */
        BRAILLE_DOTS_2,

        /**
         * U+2806 BRAILLE PATTERN DOTS-23
         */
        BRAILLE_DOTS_23,

        /**
         * U+280e BRAILLE PATTERN DOTS-234
         */
        BRAILLE_DOTS_234,

        /**
         * U+281e BRAILLE PATTERN DOTS-2345
         */
        BRAILLE_DOTS_2345,

        /**
         * U+283e BRAILLE PATTERN DOTS-23456
         */
        BRAILLE_DOTS_23456,

        /**
         * U+287e BRAILLE PATTERN DOTS-234567
         */
        BRAILLE_DOTS_234567,

        /**
         * U+28fe BRAILLE PATTERN DOTS-2345678
         */
        BRAILLE_DOTS_2345678,

        /**
         * U+28be BRAILLE PATTERN DOTS-234568
         */
        BRAILLE_DOTS_234568,

        /**
         * U+285e BRAILLE PATTERN DOTS-23457
         */
        BRAILLE_DOTS_23457,

        /**
         * U+28de BRAILLE PATTERN DOTS-234578
         */
        BRAILLE_DOTS_234578,

        /**
         * U+289e BRAILLE PATTERN DOTS-23458
         */
        BRAILLE_DOTS_23458,

        /**
         * U+282e BRAILLE PATTERN DOTS-2346
         */
        BRAILLE_DOTS_2346,

        /**
         * U+286e BRAILLE PATTERN DOTS-23467
         */
        BRAILLE_DOTS_23467,

        /**
         * U+28ee BRAILLE PATTERN DOTS-234678
         */
        BRAILLE_DOTS_234678,

        /**
         * U+28ae BRAILLE PATTERN DOTS-23468
         */
        BRAILLE_DOTS_23468,

        /**
         * U+284e BRAILLE PATTERN DOTS-2347
         */
        BRAILLE_DOTS_2347,

        /**
         * U+28ce BRAILLE PATTERN DOTS-23478
         */
        BRAILLE_DOTS_23478,

        /**
         * U+288e BRAILLE PATTERN DOTS-2348
         */
        BRAILLE_DOTS_2348,

        /**
         * U+2816 BRAILLE PATTERN DOTS-235
         */
        BRAILLE_DOTS_235,

        /**
         * U+2836 BRAILLE PATTERN DOTS-2356
         */
        BRAILLE_DOTS_2356,

        /**
         * U+2876 BRAILLE PATTERN DOTS-23567
         */
        BRAILLE_DOTS_23567,

        /**
         * U+28f6 BRAILLE PATTERN DOTS-235678
         */
        BRAILLE_DOTS_235678,

        /**
         * U+28b6 BRAILLE PATTERN DOTS-23568
         */
        BRAILLE_DOTS_23568,

        /**
         * U+2856 BRAILLE PATTERN DOTS-2357
         */
        BRAILLE_DOTS_2357,

        /**
         * U+28d6 BRAILLE PATTERN DOTS-23578
         */
        BRAILLE_DOTS_23578,

        /**
         * U+2896 BRAILLE PATTERN DOTS-2358
         */
        BRAILLE_DOTS_2358,

        /**
         * U+2826 BRAILLE PATTERN DOTS-236
         */
        BRAILLE_DOTS_236,

        /**
         * U+2866 BRAILLE PATTERN DOTS-2367
         */
        BRAILLE_DOTS_2367,

        /**
         * U+28e6 BRAILLE PATTERN DOTS-23678
         */
        BRAILLE_DOTS_23678,

        /**
         * U+28a6 BRAILLE PATTERN DOTS-2368
         */
        BRAILLE_DOTS_2368,

        /**
         * U+2846 BRAILLE PATTERN DOTS-237
         */
        BRAILLE_DOTS_237,

        /**
         * U+28c6 BRAILLE PATTERN DOTS-2378
         */
        BRAILLE_DOTS_2378,

        /**
         * U+2886 BRAILLE PATTERN DOTS-238
         */
        BRAILLE_DOTS_238,

        /**
         * U+280a BRAILLE PATTERN DOTS-24
         */
        BRAILLE_DOTS_24,

        /**
         * U+281a BRAILLE PATTERN DOTS-245
         */
        BRAILLE_DOTS_245,

        /**
         * U+283a BRAILLE PATTERN DOTS-2456
         */
        BRAILLE_DOTS_2456,

        /**
         * U+287a BRAILLE PATTERN DOTS-24567
         */
        BRAILLE_DOTS_24567,

        /**
         * U+28fa BRAILLE PATTERN DOTS-245678
         */
        BRAILLE_DOTS_245678,

        /**
         * U+28ba BRAILLE PATTERN DOTS-24568
         */
        BRAILLE_DOTS_24568,

        /**
         * U+285a BRAILLE PATTERN DOTS-2457
         */
        BRAILLE_DOTS_2457,

        /**
         * U+28da BRAILLE PATTERN DOTS-24578
         */
        BRAILLE_DOTS_24578,

        /**
         * U+289a BRAILLE PATTERN DOTS-2458
         */
        BRAILLE_DOTS_2458,

        /**
         * U+282a BRAILLE PATTERN DOTS-246
         */
        BRAILLE_DOTS_246,

        /**
         * U+286a BRAILLE PATTERN DOTS-2467
         */
        BRAILLE_DOTS_2467,

        /**
         * U+28ea BRAILLE PATTERN DOTS-24678
         */
        BRAILLE_DOTS_24678,

        /**
         * U+28aa BRAILLE PATTERN DOTS-2468
         */
        BRAILLE_DOTS_2468,

        /**
         * U+284a BRAILLE PATTERN DOTS-247
         */
        BRAILLE_DOTS_247,

        /**
         * U+28ca BRAILLE PATTERN DOTS-2478
         */
        BRAILLE_DOTS_2478,

        /**
         * U+288a BRAILLE PATTERN DOTS-248
         */
        BRAILLE_DOTS_248,

        /**
         * U+2812 BRAILLE PATTERN DOTS-25
         */
        BRAILLE_DOTS_25,

        /**
         * U+2832 BRAILLE PATTERN DOTS-256
         */
        BRAILLE_DOTS_256,

        /**
         * U+2872 BRAILLE PATTERN DOTS-2567
         */
        BRAILLE_DOTS_2567,

        /**
         * U+28f2 BRAILLE PATTERN DOTS-25678
         */
        BRAILLE_DOTS_25678,

        /**
         * U+28b2 BRAILLE PATTERN DOTS-2568
         */
        BRAILLE_DOTS_2568,

        /**
         * U+2852 BRAILLE PATTERN DOTS-257
         */
        BRAILLE_DOTS_257,

        /**
         * U+28d2 BRAILLE PATTERN DOTS-2578
         */
        BRAILLE_DOTS_2578,

        /**
         * U+2892 BRAILLE PATTERN DOTS-258
         */
        BRAILLE_DOTS_258,

        /**
         * U+2822 BRAILLE PATTERN DOTS-26
         */
        BRAILLE_DOTS_26,

        /**
         * U+2862 BRAILLE PATTERN DOTS-267
         */
        BRAILLE_DOTS_267,

        /**
         * U+28e2 BRAILLE PATTERN DOTS-2678
         */
        BRAILLE_DOTS_2678,

        /**
         * U+28a2 BRAILLE PATTERN DOTS-268
         */
        BRAILLE_DOTS_268,

        /**
         * U+2842 BRAILLE PATTERN DOTS-27
         */
        BRAILLE_DOTS_27,

        /**
         * U+28c2 BRAILLE PATTERN DOTS-278
         */
        BRAILLE_DOTS_278,

        /**
         * U+2882 BRAILLE PATTERN DOTS-28
         */
        BRAILLE_DOTS_28,

        /**
         * U+2804 BRAILLE PATTERN DOTS-3
         */
        BRAILLE_DOTS_3,

        /**
         * U+280c BRAILLE PATTERN DOTS-34
         */
        BRAILLE_DOTS_34,

        /**
         * U+281c BRAILLE PATTERN DOTS-345
         */
        BRAILLE_DOTS_345,

        /**
         * U+283c BRAILLE PATTERN DOTS-3456
         */
        BRAILLE_DOTS_3456,

        /**
         * U+287c BRAILLE PATTERN DOTS-34567
         */
        BRAILLE_DOTS_34567,

        /**
         * U+28fc BRAILLE PATTERN DOTS-345678
         */
        BRAILLE_DOTS_345678,

        /**
         * U+28bc BRAILLE PATTERN DOTS-34568
         */
        BRAILLE_DOTS_34568,

        /**
         * U+285c BRAILLE PATTERN DOTS-3457
         */
        BRAILLE_DOTS_3457,

        /**
         * U+28dc BRAILLE PATTERN DOTS-34578
         */
        BRAILLE_DOTS_34578,

        /**
         * U+289c BRAILLE PATTERN DOTS-3458
         */
        BRAILLE_DOTS_3458,

        /**
         * U+282c BRAILLE PATTERN DOTS-346
         */
        BRAILLE_DOTS_346,

        /**
         * U+286c BRAILLE PATTERN DOTS-3467
         */
        BRAILLE_DOTS_3467,

        /**
         * U+28ec BRAILLE PATTERN DOTS-34678
         */
        BRAILLE_DOTS_34678,

        /**
         * U+28ac BRAILLE PATTERN DOTS-3468
         */
        BRAILLE_DOTS_3468,

        /**
         * U+284c BRAILLE PATTERN DOTS-347
         */
        BRAILLE_DOTS_347,

        /**
         * U+28cc BRAILLE PATTERN DOTS-3478
         */
        BRAILLE_DOTS_3478,

        /**
         * U+288c BRAILLE PATTERN DOTS-348
         */
        BRAILLE_DOTS_348,

        /**
         * U+2814 BRAILLE PATTERN DOTS-35
         */
        BRAILLE_DOTS_35,

        /**
         * U+2834 BRAILLE PATTERN DOTS-356
         */
        BRAILLE_DOTS_356,

        /**
         * U+2874 BRAILLE PATTERN DOTS-3567
         */
        BRAILLE_DOTS_3567,

        /**
         * U+28f4 BRAILLE PATTERN DOTS-35678
         */
        BRAILLE_DOTS_35678,

        /**
         * U+28b4 BRAILLE PATTERN DOTS-3568
         */
        BRAILLE_DOTS_3568,

        /**
         * U+2854 BRAILLE PATTERN DOTS-357
         */
        BRAILLE_DOTS_357,

        /**
         * U+28d4 BRAILLE PATTERN DOTS-3578
         */
        BRAILLE_DOTS_3578,

        /**
         * U+2894 BRAILLE PATTERN DOTS-358
         */
        BRAILLE_DOTS_358,

        /**
         * U+2824 BRAILLE PATTERN DOTS-36
         */
        BRAILLE_DOTS_36,

        /**
         * U+2864 BRAILLE PATTERN DOTS-367
         */
        BRAILLE_DOTS_367,

        /**
         * U+28e4 BRAILLE PATTERN DOTS-3678
         */
        BRAILLE_DOTS_3678,

        /**
         * U+28a4 BRAILLE PATTERN DOTS-368
         */
        BRAILLE_DOTS_368,

        /**
         * U+2844 BRAILLE PATTERN DOTS-37
         */
        BRAILLE_DOTS_37,

        /**
         * U+28c4 BRAILLE PATTERN DOTS-378
         */
        BRAILLE_DOTS_378,

        /**
         * U+2884 BRAILLE PATTERN DOTS-38
         */
        BRAILLE_DOTS_38,

        /**
         * U+2808 BRAILLE PATTERN DOTS-4
         */
        BRAILLE_DOTS_4,

        /**
         * U+2818 BRAILLE PATTERN DOTS-45
         */
        BRAILLE_DOTS_45,

        /**
         * U+2838 BRAILLE PATTERN DOTS-456
         */
        BRAILLE_DOTS_456,

        /**
         * U+2878 BRAILLE PATTERN DOTS-4567
         */
        BRAILLE_DOTS_4567,

        /**
         * U+28f8 BRAILLE PATTERN DOTS-45678
         */
        BRAILLE_DOTS_45678,

        /**
         * U+28b8 BRAILLE PATTERN DOTS-4568
         */
        BRAILLE_DOTS_4568,

        /**
         * U+2858 BRAILLE PATTERN DOTS-457
         */
        BRAILLE_DOTS_457,

        /**
         * U+28d8 BRAILLE PATTERN DOTS-4578
         */
        BRAILLE_DOTS_4578,

        /**
         * U+2898 BRAILLE PATTERN DOTS-458
         */
        BRAILLE_DOTS_458,

        /**
         * U+2828 BRAILLE PATTERN DOTS-46
         */
        BRAILLE_DOTS_46,

        /**
         * U+2868 BRAILLE PATTERN DOTS-467
         */
        BRAILLE_DOTS_467,

        /**
         * U+28e8 BRAILLE PATTERN DOTS-4678
         */
        BRAILLE_DOTS_4678,

        /**
         * U+28a8 BRAILLE PATTERN DOTS-468
         */
        BRAILLE_DOTS_468,

        /**
         * U+2848 BRAILLE PATTERN DOTS-47
         */
        BRAILLE_DOTS_47,

        /**
         * U+28c8 BRAILLE PATTERN DOTS-478
         */
        BRAILLE_DOTS_478,

        /**
         * U+2888 BRAILLE PATTERN DOTS-48
         */
        BRAILLE_DOTS_48,

        /**
         * U+2810 BRAILLE PATTERN DOTS-5
         */
        BRAILLE_DOTS_5,

        /**
         * U+2830 BRAILLE PATTERN DOTS-56
         */
        BRAILLE_DOTS_56,

        /**
         * U+2870 BRAILLE PATTERN DOTS-567
         */
        BRAILLE_DOTS_567,

        /**
         * U+28f0 BRAILLE PATTERN DOTS-5678
         */
        BRAILLE_DOTS_5678,

        /**
         * U+28b0 BRAILLE PATTERN DOTS-568
         */
        BRAILLE_DOTS_568,

        /**
         * U+2850 BRAILLE PATTERN DOTS-57
         */
        BRAILLE_DOTS_57,

        /**
         * U+28d0 BRAILLE PATTERN DOTS-578
         */
        BRAILLE_DOTS_578,

        /**
         * U+2890 BRAILLE PATTERN DOTS-58
         */
        BRAILLE_DOTS_58,

        /**
         * U+2820 BRAILLE PATTERN DOTS-6
         */
        BRAILLE_DOTS_6,

        /**
         * U+2860 BRAILLE PATTERN DOTS-67
         */
        BRAILLE_DOTS_67,

        /**
         * U+28e0 BRAILLE PATTERN DOTS-678
         */
        BRAILLE_DOTS_678,

        /**
         * U+28a0 BRAILLE PATTERN DOTS-68
         */
        BRAILLE_DOTS_68,

        /**
         * U+2840 BRAILLE PATTERN DOTS-7
         */
        BRAILLE_DOTS_7,

        /**
         * U+28c0 BRAILLE PATTERN DOTS-78
         */
        BRAILLE_DOTS_78,

        /**
         * U+2880 BRAILLE PATTERN DOTS-8
         */
        BRAILLE_DOTS_8,

        /**
         * Braille Dot 1
         */
        BRAILLE_DOT_1,

        /**
         * Braille Dot 10
         */
        BRAILLE_DOT_10,

        /**
         * Braille Dot 2
         */
        BRAILLE_DOT_2,

        /**
         * Braille Dot 3
         */
        BRAILLE_DOT_3,

        /**
         * Braille Dot 4
         */
        BRAILLE_DOT_4,

        /**
         * Braille Dot 5
         */
        BRAILLE_DOT_5,

        /**
         * Braille Dot 6
         */
        BRAILLE_DOT_6,

        /**
         * Braille Dot 7
         */
        BRAILLE_DOT_7,

        /**
         * Braille Dot 8
         */
        BRAILLE_DOT_8,

        /**
         * Braille Dot 9
         */
        BRAILLE_DOT_9,

        /**
         * Break
         */
        BREAK,

        /**
         * U+02D8 BREVE
         */
        BREVE,

        /**
         * U+00A6 BROKEN BAR
         */
        BROKENBAR,

        /**
         * U+045E CYRILLIC SMALL LETTER SHORT U
         */
        BYELORUSSIAN_LCASE_SHORTU,

        /**
         * U+040E CYRILLIC CAPITAL LETTER SHORT U
         */
        BYELORUSSIAN_UCASE_SHORTU,

        /**
         * Cancel, stop, abort, exit
         */
        CANCEL,

        /**
         * Caps lock
         */
        CAPS_LOCK,

        /**
         * U+2105 CARE OF
         */
        CAREOF,

        /**
         * U+2038 CARET
         */
        CARET,

        /**
         * U+02C7 CARON
         */
        CARON,

        /**
         * U+00B8 CEDILLA
         */
        CEDILLA,

        /**
         * U+00A2 CENT SIGN
         */
        CENT,

        /**
         * U+2592 MEDIUM SHADE
         */
        CHECKERBOARD,

        /**
         * U+2713 CHECK MARK
         */
        CHECKMARK,

        /**
         * U+25CB WHITE CIRCLE
         */
        CIRCLE,

        /**
         * Clear
         */
        CLEAR,

        /**
         * Clear Line
         */
        CLEAR_LINE,

        /**
         * U+2663 BLACK CLUB SUIT
         */
        CLUB,

        /**
         * Codeinput
         */
        CODEINPUT,

        /**
         * U+003A COLON
         */
        COLON,

        /**
         * U+20A1 COLON SIGN
         */
        COLON_SIGN,

        /**
         * U+002C COMMA
         */
        COMMA,

        /**
         * U+220B CONTAINS AS MEMBER
         */
        CONTAINSAS,

        /**
         * Left control
         */
        CONTROL_L,

        /**
         * Right control
         */
        CONTROL_R,

        /**
         * U+00A9 COPYRIGHT SIGN
         */
        COPYRIGHT,

        /**
         * U+240D SYMBOL FOR CARRIAGE RETURN
         */
        CR,

        /**
         * U+253C BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL
         */
        CROSSINGLINES,

        /**
         * U+20A2 CRUZEIRO SIGN
         */
        CRUZEIRO_SIGN,

        /**
         * U+221B CUBE ROOT
         */
        CUBEROOT,

        /**
         * U+00A4 CURRENCY SIGN
         */
        CURRENCY,

        /**
         * Cursor
         */
        CURSOR,

        /**
         * U+0430 CYRILLIC SMALL LETTER A
         */
        CYRILLIC_LCASE_A,

        /**
         * U+0431 CYRILLIC SMALL LETTER BE
         */
        CYRILLIC_LCASE_BE,

        /**
         * U+0447 CYRILLIC SMALL LETTER CHE
         */
        CYRILLIC_LCASE_CHE,

        /**
         * U+04B7 CYRILLIC SMALL LETTER CHE WITH DESCENDER
         */
        CYRILLIC_LCASE_CHE_DESCENDER,

        /**
         * U+04B9 CYRILLIC SMALL LETTER CHE WITH VERTICAL STROKE
         */
        CYRILLIC_LCASE_CHE_VERTSTROKE,

        /**
         * U+0434 CYRILLIC SMALL LETTER DE
         */
        CYRILLIC_LCASE_DE,

        /**
         * U+045F CYRILLIC SMALL LETTER DZHE
         */
        CYRILLIC_LCASE_DZHE,

        /**
         * U+044D CYRILLIC SMALL LETTER E
         */
        CYRILLIC_LCASE_E,

        /**
         * U+0444 CYRILLIC SMALL LETTER EF
         */
        CYRILLIC_LCASE_EF,

        /**
         * U+043B CYRILLIC SMALL LETTER EL
         */
        CYRILLIC_LCASE_EL,

        /**
         * U+043C CYRILLIC SMALL LETTER EM
         */
        CYRILLIC_LCASE_EM,

        /**
         * U+043D CYRILLIC SMALL LETTER EN
         */
        CYRILLIC_LCASE_EN,

        /**
         * U+04A3 CYRILLIC SMALL LETTER EN WITH DESCENDER
         */
        CYRILLIC_LCASE_EN_DESCENDER,

        /**
         * U+0440 CYRILLIC SMALL LETTER ER
         */
        CYRILLIC_LCASE_ER,

        /**
         * U+0441 CYRILLIC SMALL LETTER ES
         */
        CYRILLIC_LCASE_ES,

        /**
         * U+0433 CYRILLIC SMALL LETTER GHE
         */
        CYRILLIC_LCASE_GHE,

        /**
         * U+0493 CYRILLIC SMALL LETTER GHE WITH STROKE
         */
        CYRILLIC_LCASE_GHE_BAR,

        /**
         * U+0445 CYRILLIC SMALL LETTER HA
         */
        CYRILLIC_LCASE_HA,

        /**
         * U+044A CYRILLIC SMALL LETTER HARD SIGN
         */
        CYRILLIC_LCASE_HARDSIGN,

        /**
         * U+04B3 CYRILLIC SMALL LETTER HA WITH DESCENDER
         */
        CYRILLIC_LCASE_HA_DESCENDER,

        /**
         * U+0438 CYRILLIC SMALL LETTER I
         */
        CYRILLIC_LCASE_I,

        /**
         * U+0435 CYRILLIC SMALL LETTER IE
         */
        CYRILLIC_LCASE_IE,

        /**
         * U+0451 CYRILLIC SMALL LETTER IO
         */
        CYRILLIC_LCASE_IO,

        /**
         * U+04E3 CYRILLIC SMALL LETTER I WITH MACRON
         */
        CYRILLIC_LCASE_I_MACRON,

        /**
         * U+0458 CYRILLIC SMALL LETTER JE
         */
        CYRILLIC_LCASE_JE,

        /**
         * U+043A CYRILLIC SMALL LETTER KA
         */
        CYRILLIC_LCASE_KA,

        /**
         * U+049B CYRILLIC SMALL LETTER KA WITH DESCENDER
         */
        CYRILLIC_LCASE_KA_DESCENDER,

        /**
         * U+049D CYRILLIC SMALL LETTER KA WITH VERTICAL STROKE
         */
        CYRILLIC_LCASE_KA_VERTSTROKE,

        /**
         * U+0459 CYRILLIC SMALL LETTER LJE
         */
        CYRILLIC_LCASE_LJE,

        /**
         * U+045A CYRILLIC SMALL LETTER NJE
         */
        CYRILLIC_LCASE_NJE,

        /**
         * U+043E CYRILLIC SMALL LETTER O
         */
        CYRILLIC_LCASE_O,

        /**
         * U+04E9 CYRILLIC SMALL LETTER BARRED O
         */
        CYRILLIC_LCASE_O_BAR,

        /**
         * U+043F CYRILLIC SMALL LETTER PE
         */
        CYRILLIC_LCASE_PE,

        /**
         * U+04D9 CYRILLIC SMALL LETTER SCHWA
         */
        CYRILLIC_LCASE_SCHWA,

        /**
         * U+0448 CYRILLIC SMALL LETTER SHA
         */
        CYRILLIC_LCASE_SHA,

        /**
         * U+0449 CYRILLIC SMALL LETTER SHCHA
         */
        CYRILLIC_LCASE_SHCHA,

        /**
         * U+04BB CYRILLIC SMALL LETTER SHHA
         */
        CYRILLIC_LCASE_SHHA,

        /**
         * U+0439 CYRILLIC SMALL LETTER SHORT I
         */
        CYRILLIC_LCASE_SHORTI,

        /**
         * U+044C CYRILLIC SMALL LETTER SOFT SIGN
         */
        CYRILLIC_LCASE_SOFTSIGN,

        /**
         * U+0442 CYRILLIC SMALL LETTER TE
         */
        CYRILLIC_LCASE_TE,

        /**
         * U+0446 CYRILLIC SMALL LETTER TSE
         */
        CYRILLIC_LCASE_TSE,

        /**
         * U+0443 CYRILLIC SMALL LETTER U
         */
        CYRILLIC_LCASE_U,

        /**
         * U+04EF CYRILLIC SMALL LETTER U WITH MACRON
         */
        CYRILLIC_LCASE_U_MACRON,

        /**
         * U+04AF CYRILLIC SMALL LETTER STRAIGHT U
         */
        CYRILLIC_LCASE_U_STRAIGHT,

        /**
         * U+04B1 CYRILLIC SMALL LETTER STRAIGHT U WITH STROKE
         */
        CYRILLIC_LCASE_U_STRAIGHT_BAR,

        /**
         * U+0432 CYRILLIC SMALL LETTER VE
         */
        CYRILLIC_LCASE_VE,

        /**
         * U+044F CYRILLIC SMALL LETTER YA
         */
        CYRILLIC_LCASE_YA,

        /**
         * U+044B CYRILLIC SMALL LETTER YERU
         */
        CYRILLIC_LCASE_YERU,

        /**
         * U+044E CYRILLIC SMALL LETTER YU
         */
        CYRILLIC_LCASE_YU,

        /**
         * U+0437 CYRILLIC SMALL LETTER ZE
         */
        CYRILLIC_LCASE_ZE,

        /**
         * U+0436 CYRILLIC SMALL LETTER ZHE
         */
        CYRILLIC_LCASE_ZHE,

        /**
         * U+0497 CYRILLIC SMALL LETTER ZHE WITH DESCENDER
         */
        CYRILLIC_LCASE_ZHE_DESCENDER,

        /**
         * U+0410 CYRILLIC CAPITAL LETTER A
         */
        CYRILLIC_UCASE_A,

        /**
         * U+0411 CYRILLIC CAPITAL LETTER BE
         */
        CYRILLIC_UCASE_BE,

        /**
         * U+0427 CYRILLIC CAPITAL LETTER CHE
         */
        CYRILLIC_UCASE_CHE,

        /**
         * U+04B6 CYRILLIC CAPITAL LETTER CHE WITH DESCENDER
         */
        CYRILLIC_UCASE_CHE_DESCENDER,

        /**
         * U+04B8 CYRILLIC CAPITAL LETTER CHE WITH VERTICAL STROKE
         */
        CYRILLIC_UCASE_CHE_VERTSTROKE,

        /**
         * U+0414 CYRILLIC CAPITAL LETTER DE
         */
        CYRILLIC_UCASE_DE,

        /**
         * U+040F CYRILLIC CAPITAL LETTER DZHE
         */
        CYRILLIC_UCASE_DZHE,

        /**
         * U+042D CYRILLIC CAPITAL LETTER E
         */
        CYRILLIC_UCASE_E,

        /**
         * U+0424 CYRILLIC CAPITAL LETTER EF
         */
        CYRILLIC_UCASE_EF,

        /**
         * U+041B CYRILLIC CAPITAL LETTER EL
         */
        CYRILLIC_UCASE_EL,

        /**
         * U+041C CYRILLIC CAPITAL LETTER EM
         */
        CYRILLIC_UCASE_EM,

        /**
         * U+041D CYRILLIC CAPITAL LETTER EN
         */
        CYRILLIC_UCASE_EN,

        /**
         * U+04A2 CYRILLIC CAPITAL LETTER EN WITH DESCENDER
         */
        CYRILLIC_UCASE_EN_DESCENDER,

        /**
         * U+0420 CYRILLIC CAPITAL LETTER ER
         */
        CYRILLIC_UCASE_ER,

        /**
         * U+0421 CYRILLIC CAPITAL LETTER ES
         */
        CYRILLIC_UCASE_ES,

        /**
         * U+0413 CYRILLIC CAPITAL LETTER GHE
         */
        CYRILLIC_UCASE_GHE,

        /**
         * U+0492 CYRILLIC CAPITAL LETTER GHE WITH STROKE
         */
        CYRILLIC_UCASE_GHE_BAR,

        /**
         * U+0425 CYRILLIC CAPITAL LETTER HA
         */
        CYRILLIC_UCASE_HA,

        /**
         * U+042A CYRILLIC CAPITAL LETTER HARD SIGN
         */
        CYRILLIC_UCASE_HARDSIGN,

        /**
         * U+04B2 CYRILLIC CAPITAL LETTER HA WITH DESCENDER
         */
        CYRILLIC_UCASE_HA_DESCENDER,

        /**
         * U+0418 CYRILLIC CAPITAL LETTER I
         */
        CYRILLIC_UCASE_I,

        /**
         * U+0415 CYRILLIC CAPITAL LETTER IE
         */
        CYRILLIC_UCASE_IE,

        /**
         * U+0401 CYRILLIC CAPITAL LETTER IO
         */
        CYRILLIC_UCASE_IO,

        /**
         * U+04E2 CYRILLIC CAPITAL LETTER I WITH MACRON
         */
        CYRILLIC_UCASE_I_MACRON,

        /**
         * U+0408 CYRILLIC CAPITAL LETTER JE
         */
        CYRILLIC_UCASE_JE,

        /**
         * U+041A CYRILLIC CAPITAL LETTER KA
         */
        CYRILLIC_UCASE_KA,

        /**
         * U+049A CYRILLIC CAPITAL LETTER KA WITH DESCENDER
         */
        CYRILLIC_UCASE_KA_DESCENDER,

        /**
         * U+049C CYRILLIC CAPITAL LETTER KA WITH VERTICAL STROKE
         */
        CYRILLIC_UCASE_KA_VERTSTROKE,

        /**
         * U+0409 CYRILLIC CAPITAL LETTER LJE
         */
        CYRILLIC_UCASE_LJE,

        /**
         * U+040A CYRILLIC CAPITAL LETTER NJE
         */
        CYRILLIC_UCASE_NJE,

        /**
         * U+041E CYRILLIC CAPITAL LETTER O
         */
        CYRILLIC_UCASE_O,

        /**
         * U+04E8 CYRILLIC CAPITAL LETTER BARRED O
         */
        CYRILLIC_UCASE_O_BAR,

        /**
         * U+041F CYRILLIC CAPITAL LETTER PE
         */
        CYRILLIC_UCASE_PE,

        /**
         * U+04D8 CYRILLIC CAPITAL LETTER SCHWA
         */
        CYRILLIC_UCASE_SCHWA,

        /**
         * U+0428 CYRILLIC CAPITAL LETTER SHA
         */
        CYRILLIC_UCASE_SHA,

        /**
         * U+0429 CYRILLIC CAPITAL LETTER SHCHA
         */
        CYRILLIC_UCASE_SHCHA,

        /**
         * U+04BA CYRILLIC CAPITAL LETTER SHHA
         */
        CYRILLIC_UCASE_SHHA,

        /**
         * U+0419 CYRILLIC CAPITAL LETTER SHORT I
         */
        CYRILLIC_UCASE_SHORTI,

        /**
         * U+042C CYRILLIC CAPITAL LETTER SOFT SIGN
         */
        CYRILLIC_UCASE_SOFTSIGN,

        /**
         * U+0422 CYRILLIC CAPITAL LETTER TE
         */
        CYRILLIC_UCASE_TE,

        /**
         * U+0426 CYRILLIC CAPITAL LETTER TSE
         */
        CYRILLIC_UCASE_TSE,

        /**
         * U+0423 CYRILLIC CAPITAL LETTER U
         */
        CYRILLIC_UCASE_U,

        /**
         * U+04EE CYRILLIC CAPITAL LETTER U WITH MACRON
         */
        CYRILLIC_UCASE_U_MACRON,

        /**
         * U+04AE CYRILLIC CAPITAL LETTER STRAIGHT U
         */
        CYRILLIC_UCASE_U_STRAIGHT,

        /**
         * U+04B0 CYRILLIC CAPITAL LETTER STRAIGHT U WITH STROKE
         */
        CYRILLIC_UCASE_U_STRAIGHT_BAR,

        /**
         * U+0412 CYRILLIC CAPITAL LETTER VE
         */
        CYRILLIC_UCASE_VE,

        /**
         * U+042F CYRILLIC CAPITAL LETTER YA
         */
        CYRILLIC_UCASE_YA,

        /**
         * U+042B CYRILLIC CAPITAL LETTER YERU
         */
        CYRILLIC_UCASE_YERU,

        /**
         * U+042E CYRILLIC CAPITAL LETTER YU
         */
        CYRILLIC_UCASE_YU,

        /**
         * U+0417 CYRILLIC CAPITAL LETTER ZE
         */
        CYRILLIC_UCASE_ZE,

        /**
         * U+0416 CYRILLIC CAPITAL LETTER ZHE
         */
        CYRILLIC_UCASE_ZHE,

        /**
         * U+0496 CYRILLIC CAPITAL LETTER ZHE WITH DESCENDER
         */
        CYRILLIC_UCASE_ZHE_DESCENDER,

        /**
         * U+0030 DIGIT ZERO
         */
        D0,

        /**
         * U+0031 DIGIT ONE
         */
        D1,

        /**
         * U+0032 DIGIT TWO
         */
        D2,

        /**
         * U+0033 DIGIT THREE
         */
        D3,

        /**
         * U+0034 DIGIT FOUR
         */
        D4,

        /**
         * U+0035 DIGIT FIVE
         */
        D5,

        /**
         * U+0036 DIGIT SIX
         */
        D6,

        /**
         * U+0037 DIGIT SEVEN
         */
        D7,

        /**
         * U+0038 DIGIT EIGHT
         */
        D8,

        /**
         * U+0039 DIGIT NINE
         */
        D9,

        /**
         * Dacute Accent
         */
        DACUTE_ACCENT,

        /**
         * U+2020 DAGGER
         */
        DAGGER,

        /**
         * Dcedilla Accent
         */
        DCEDILLA_ACCENT,

        /**
         * Dcircumflex Accent
         */
        DCIRCUMFLEX_ACCENT,

        /**
         * Ddiaeresis
         */
        DDIAERESIS,

        /**
         * Dead Abovecomma
         */
        DEAD_ABOVECOMMA,

        /**
         * Dead Abovedot
         */
        DEAD_ABOVEDOT,

        /**
         * Dead Abovereversedcomma
         */
        DEAD_ABOVEREVERSEDCOMMA,

        /**
         * Dead Abovering
         */
        DEAD_ABOVERING,

        /**
         * Dead Aboveverticalline
         */
        DEAD_ABOVEVERTICALLINE,

        /**
         * Dead Acute
         */
        DEAD_ACUTE,

        /**
         * Dead Belowbreve
         */
        DEAD_BELOWBREVE,

        /**
         * Dead Belowcircumflex
         */
        DEAD_BELOWCIRCUMFLEX,

        /**
         * Dead Belowcomma
         */
        DEAD_BELOWCOMMA,

        /**
         * Dead Belowdiaeresis
         */
        DEAD_BELOWDIAERESIS,

        /**
         * Dead Belowdot
         */
        DEAD_BELOWDOT,

        /**
         * Dead Belowmacron
         */
        DEAD_BELOWMACRON,

        /**
         * Dead Belowring
         */
        DEAD_BELOWRING,

        /**
         * Dead Belowtilde
         */
        DEAD_BELOWTILDE,

        /**
         * Dead Belowverticalline
         */
        DEAD_BELOWVERTICALLINE,

        /**
         * Dead Breve
         */
        DEAD_BREVE,

        /**
         * Dead Capital Schwa
         */
        DEAD_CAPITAL_SCHWA,

        /**
         * Dead Caron
         */
        DEAD_CARON,

        /**
         * Dead Cedilla
         */
        DEAD_CEDILLA,

        /**
         * Dead Circumflex
         */
        DEAD_CIRCUMFLEX,

        /**
         * Dead Currency
         */
        DEAD_CURRENCY,

        /**
         * alias for dead_abovereversedcomma
         */
        DEAD_DASIA,

        /**
         * Dead Diaeresis
         */
        DEAD_DIAERESIS,

        /**
         * Dead Doubleacute
         */
        DEAD_DOUBLEACUTE,

        /**
         * Dead Doublegrave
         */
        DEAD_DOUBLEGRAVE,

        /**
         * Dead Grave
         */
        DEAD_GRAVE,

        /**
         * Dead Greek
         */
        DEAD_GREEK,

        /**
         * Dead Hook
         */
        DEAD_HOOK,

        /**
         * Dead Horn
         */
        DEAD_HORN,

        /**
         * Dead Invertedbreve
         */
        DEAD_INVERTEDBREVE,

        /**
         * Dead Iota
         */
        DEAD_IOTA,

        /**
         * Dead Lcase A
         */
        DEAD_LCASE_A,

        /**
         * Dead Lcase E
         */
        DEAD_LCASE_E,

        /**
         * Dead Lcase I
         */
        DEAD_LCASE_I,

        /**
         * Dead Lcase O
         */
        DEAD_LCASE_O,

        /**
         * Dead Lcase U
         */
        DEAD_LCASE_U,

        /**
         * Dead Longsolidusoverlay
         */
        DEAD_LONGSOLIDUSOVERLAY,

        /**
         * Dead Lowline
         */
        DEAD_LOWLINE,

        /**
         * Dead Macron
         */
        DEAD_MACRON,

        /**
         * Dead Ogonek
         */
        DEAD_OGONEK,

        /**
         * alias for dead_tilde
         */
        DEAD_PERISPOMENI,

        /**
         * alias for dead_abovecomma
         */
        DEAD_PSILI,

        /**
         * Dead Semivoiced Sound
         */
        DEAD_SEMIVOICED_SOUND,

        /**
         * Dead Small Schwa
         */
        DEAD_SMALL_SCHWA,

        /**
         * Dead Stroke
         */
        DEAD_STROKE,

        /**
         * Dead Tilde
         */
        DEAD_TILDE,

        /**
         * Dead Ucase A
         */
        DEAD_UCASE_A,

        /**
         * Dead Ucase E
         */
        DEAD_UCASE_E,

        /**
         * Dead Ucase I
         */
        DEAD_UCASE_I,

        /**
         * Dead Ucase O
         */
        DEAD_UCASE_O,

        /**
         * Dead Ucase U
         */
        DEAD_UCASE_U,

        /**
         * Dead Voiced Sound
         */
        DEAD_VOICED_SOUND,

        /**
         * U+002E FULL STOP
         */
        DECIMALPOINT,

        /**
         * U+00B0 DEGREE SIGN
         */
        DEGREE,

        /**
         * Delete, rubout
         */
        DELETE,

        /**
         * Delete Char
         */
        DELETE_CHAR,

        /**
         * Delete Line
         */
        DELETE_LINE,

        /**
         * Dgrave Accent
         */
        DGRAVE_ACCENT,

        /**
         * U+00A8 DIAERESIS
         */
        DIAERESIS,

        /**
         * U+2666 BLACK DIAMOND SUIT
         */
        DIAMOND,

        /**
         * U+2007 FIGURE SPACE
         */
        DIGITSPACE,

        /**
         * U+222C DOUBLE INTEGRAL
         */
        DINTEGRAL,

        /**
         * U+00F7 DIVISION SIGN
         */
        DIVISION,

        /**
         * U+0024 DOLLAR SIGN
         */
        DOLLAR,

        /**
         * U+20AB DONG SIGN
         */
        DONG_SIGN,

        /**
         * U+2025 TWO DOT LEADER
         */
        DOUBBASELINEDOT,

        /**
         * U+02DD DOUBLE ACUTE ACCENT
         */
        DOUBLEACUTE,

        /**
         * U+2021 DOUBLE DAGGER
         */
        DOUBLEDAGGER,

        /**
         * U+201E DOUBLE LOW-9 QUOTATION MARK
         */
        DOUBLELOWQUOTEMARK,

        /**
         * Move down, down arrow
         */
        DOWN,

        /**
         * U+2193 DOWNWARDS ARROW
         */
        DOWNARROW,

        /**
         * U+2228 LOGICAL OR
         */
        DOWNCARET,

        /**
         * U+222A UNION
         */
        DOWNSHOE,

        /**
         * U+230A LEFT FLOOR
         */
        DOWNSTILE,

        /**
         * U+22A4 DOWN TACK
         */
        DOWNTACK,

        /**
         * Remove
         */
        DREMOVE,

        /**
         * Dring Accent
         */
        DRING_ACCENT,

        /**
         * Dtilde
         */
        DTILDE,

        /**
         * U+20A0 EURO-CURRENCY SIGN
         */
        ECU_SIGN,

        /**
         * U+2088 SUBSCRIPT EIGHT
         */
        EIGHTSUBSCRIPT,

        /**
         * U+2078 SUPERSCRIPT EIGHT
         */
        EIGHTSUPERIOR,

        /**
         * Alphanumeric Shift
         */
        EISU_SHIFT,

        /**
         * Alphanumeric toggle
         */
        EISU_TOGGLE,

        /**
         * U+2208 ELEMENT OF
         */
        ELEMENTOF,

        /**
         * U+2026 HORIZONTAL ELLIPSIS
         */
        ELLIPSIS,

        /**
         * U+2004 THREE-PER-EM SPACE
         */
        EM3SPACE,

        /**
         * U+2005 FOUR-PER-EM SPACE
         */
        EM4SPACE,

        /**
         * U+2014 EM DASH
         */
        EMDASH,

        /**
         * U+25CF BLACK CIRCLE
         */
        EMFILLEDCIRCLE,

        /**
         * U+25AE BLACK VERTICAL RECTANGLE
         */
        EMFILLEDRECT,

        /**
         * U+25CB WHITE CIRCLE
         */
        EMOPENCIRCLE,

        /**
         * U+25AF WHITE VERTICAL RECTANGLE
         */
        EMOPENRECTANGLE,

        /**
         * U+2205 NULL SET
         */
        EMPTYSET,

        /**
         * U+2003 EM SPACE
         */
        EMSPACE,

        /**
         * EOL
         */
        END,

        /**
         * U+2013 EN DASH
         */
        ENDASH,

        /**
         * U+2022 BULLET
         */
        ENFILLEDCIRCBULLET,

        /**
         * U+25AA BLACK SMALL SQUARE
         */
        ENFILLEDSQBULLET,

        /**
         * U+25E6 WHITE BULLET
         */
        ENOPENCIRCBULLET,

        /**
         * U+25AB WHITE SMALL SQUARE
         */
        ENOPENSQUAREBULLET,

        /**
         * U+2002 EN SPACE
         */
        ENSPACE,

        /**
         * U+003D EQUALS SIGN
         */
        EQUAL,

        /**
         * Escape
         */
        ESCAPE,

        /**
         * U+20AC EURO SIGN
         */
        EURO_SIGN,

        /**
         * U+0021 EXCLAMATION MARK
         */
        EXCLAM,

        /**
         * U+00A1 INVERTED EXCLAMATION MARK
         */
        EXCLAMDOWN,

        /**
         * Execute, run, do
         */
        EXECUTE,

        /**
         * Ext16Bit L
         */
        EXT16BIT_L,

        /**
         * Ext16Bit R
         */
        EXT16BIT_R,

        /**
         * F1
         */
        F1,

        /**
         * F10
         */
        F10,

        /**
         * F11
         */
        F11,

        /**
         * F12
         */
        F12,

        /**
         * F13
         */
        F13,

        /**
         * F14
         */
        F14,

        /**
         * F15
         */
        F15,

        /**
         * F16
         */
        F16,

        /**
         * F17
         */
        F17,

        /**
         * F18
         */
        F18,

        /**
         * F19
         */
        F19,

        /**
         * F2
         */
        F2,

        /**
         * F20
         */
        F20,

        /**
         * F21
         */
        F21,

        /**
         * F22
         */
        F22,

        /**
         * F23
         */
        F23,

        /**
         * F24
         */
        F24,

        /**
         * F25
         */
        F25,

        /**
         * F26
         */
        F26,

        /**
         * F27
         */
        F27,

        /**
         * F28
         */
        F28,

        /**
         * F29
         */
        F29,

        /**
         * F3
         */
        F3,

        /**
         * F30
         */
        F30,

        /**
         * F31
         */
        F31,

        /**
         * F32
         */
        F32,

        /**
         * F33
         */
        F33,

        /**
         * F34
         */
        F34,

        /**
         * F35
         */
        F35,

        /**
         * F4
         */
        F4,

        /**
         * F5
         */
        F5,

        /**
         * F6
         */
        F6,

        /**
         * F7
         */
        F7,

        /**
         * F8
         */
        F8,

        /**
         * F9
         */
        F9,

        /**
         * U+06F0 EXTENDED ARABIC-INDIC DIGIT ZERO
         */
        FARSI_0,

        /**
         * U+06F1 EXTENDED ARABIC-INDIC DIGIT ONE
         */
        FARSI_1,

        /**
         * U+06F2 EXTENDED ARABIC-INDIC DIGIT TWO
         */
        FARSI_2,

        /**
         * U+06F3 EXTENDED ARABIC-INDIC DIGIT THREE
         */
        FARSI_3,

        /**
         * U+06F4 EXTENDED ARABIC-INDIC DIGIT FOUR
         */
        FARSI_4,

        /**
         * U+06F5 EXTENDED ARABIC-INDIC DIGIT FIVE
         */
        FARSI_5,

        /**
         * U+06F6 EXTENDED ARABIC-INDIC DIGIT SIX
         */
        FARSI_6,

        /**
         * U+06F7 EXTENDED ARABIC-INDIC DIGIT SEVEN
         */
        FARSI_7,

        /**
         * U+06F8 EXTENDED ARABIC-INDIC DIGIT EIGHT
         */
        FARSI_8,

        /**
         * U+06F9 EXTENDED ARABIC-INDIC DIGIT NINE
         */
        FARSI_9,

        /**
         * U+06CC ARABIC LETTER FARSI YEH
         */
        FARSI_YEH,

        /**
         * U+2640 FEMALE SIGN
         */
        FEMALESYMBOL,

        /**
         * U+240C SYMBOL FOR FORM FEED
         */
        FF,

        /**
         * U+20A3 FRENCH FRANC SIGN
         */
        FFRANC_SIGN,

        /**
         * U+2012 FIGURE DASH
         */
        FIGDASH,

        /**
         * U+25C0 BLACK LEFT-POINTING TRIANGLE
         */
        FILLEDLEFTTRIBULLET,

        /**
         * U+25AC BLACK RECTANGLE
         */
        FILLEDRECTBULLET,

        /**
         * U+25B6 BLACK RIGHT-POINTING TRIANGLE
         */
        FILLEDRIGHTTRIBULLET,

        /**
         * U+25BC BLACK DOWN-POINTING TRIANGLE
         */
        FILLEDTRIBULLETDOWN,

        /**
         * U+25B2 BLACK UP-POINTING TRIANGLE
         */
        FILLEDTRIBULLETUP,

        /**
         * Find, search
         */
        FIND,

        /**
         * First Virtual Screen
         */
        FIRST_VIRTUAL_SCREEN,

        /**
         * U+215D VULGAR FRACTION FIVE EIGHTHS
         */
        FIVEEIGHTHS,

        /**
         * U+215A VULGAR FRACTION FIVE SIXTHS
         */
        FIVESIXTHS,

        /**
         * U+2085 SUBSCRIPT FIVE
         */
        FIVESUBSCRIPT,

        /**
         * U+2075 SUPERSCRIPT FIVE
         */
        FIVESUPERIOR,

        /**
         * U+2158 VULGAR FRACTION FOUR FIFTHS
         */
        FOURFIFTHS,

        /**
         * U+2084 SUBSCRIPT FOUR
         */
        FOURSUBSCRIPT,

        /**
         * U+2074 SUPERSCRIPT FOUR
         */
        FOURSUPERIOR,

        /**
         * U+221C FOURTH ROOT
         */
        FOURTHROOT,

        /**
         * U+0192 LATIN SMALL LETTER F WITH HOOK
         */
        FUNCTION,

        /**
         * U+10D0 GEORGIAN LETTER AN
         */
        GEORGIAN_AN,

        /**
         * U+10D1 GEORGIAN LETTER BAN
         */
        GEORGIAN_BAN,

        /**
         * U+10EA GEORGIAN LETTER CAN
         */
        GEORGIAN_CAN,

        /**
         * U+10ED GEORGIAN LETTER CHAR
         */
        GEORGIAN_CHAR,

        /**
         * U+10E9 GEORGIAN LETTER CHIN
         */
        GEORGIAN_CHIN,

        /**
         * U+10EC GEORGIAN LETTER CIL
         */
        GEORGIAN_CIL,

        /**
         * U+10D3 GEORGIAN LETTER DON
         */
        GEORGIAN_DON,

        /**
         * U+10D4 GEORGIAN LETTER EN
         */
        GEORGIAN_EN,

        /**
         * U+10F6 GEORGIAN LETTER FI
         */
        GEORGIAN_FI,

        /**
         * U+10D2 GEORGIAN LETTER GAN
         */
        GEORGIAN_GAN,

        /**
         * U+10E6 GEORGIAN LETTER GHAN
         */
        GEORGIAN_GHAN,

        /**
         * U+10F0 GEORGIAN LETTER HAE
         */
        GEORGIAN_HAE,

        /**
         * U+10F4 GEORGIAN LETTER HAR
         */
        GEORGIAN_HAR,

        /**
         * U+10F1 GEORGIAN LETTER HE
         */
        GEORGIAN_HE,

        /**
         * U+10F2 GEORGIAN LETTER HIE
         */
        GEORGIAN_HIE,

        /**
         * U+10F5 GEORGIAN LETTER HOE
         */
        GEORGIAN_HOE,

        /**
         * U+10D8 GEORGIAN LETTER IN
         */
        GEORGIAN_IN,

        /**
         * U+10EF GEORGIAN LETTER JHAN
         */
        GEORGIAN_JHAN,

        /**
         * U+10EB GEORGIAN LETTER JIL
         */
        GEORGIAN_JIL,

        /**
         * U+10D9 GEORGIAN LETTER KAN
         */
        GEORGIAN_KAN,

        /**
         * U+10E5 GEORGIAN LETTER KHAR
         */
        GEORGIAN_KHAR,

        /**
         * U+10DA GEORGIAN LETTER LAS
         */
        GEORGIAN_LAS,

        /**
         * U+10DB GEORGIAN LETTER MAN
         */
        GEORGIAN_MAN,

        /**
         * U+10DC GEORGIAN LETTER NAR
         */
        GEORGIAN_NAR,

        /**
         * U+10DD GEORGIAN LETTER ON
         */
        GEORGIAN_ON,

        /**
         * U+10DE GEORGIAN LETTER PAR
         */
        GEORGIAN_PAR,

        /**
         * U+10E4 GEORGIAN LETTER PHAR
         */
        GEORGIAN_PHAR,

        /**
         * U+10E7 GEORGIAN LETTER QAR
         */
        GEORGIAN_QAR,

        /**
         * U+10E0 GEORGIAN LETTER RAE
         */
        GEORGIAN_RAE,

        /**
         * U+10E1 GEORGIAN LETTER SAN
         */
        GEORGIAN_SAN,

        /**
         * U+10E8 GEORGIAN LETTER SHIN
         */
        GEORGIAN_SHIN,

        /**
         * U+10D7 GEORGIAN LETTER TAN
         */
        GEORGIAN_TAN,

        /**
         * U+10E2 GEORGIAN LETTER TAR
         */
        GEORGIAN_TAR,

        /**
         * U+10E3 GEORGIAN LETTER UN
         */
        GEORGIAN_UN,

        /**
         * U+10D5 GEORGIAN LETTER VIN
         */
        GEORGIAN_VIN,

        /**
         * U+10F3 GEORGIAN LETTER WE
         */
        GEORGIAN_WE,

        /**
         * U+10EE GEORGIAN LETTER XAN
         */
        GEORGIAN_XAN,

        /**
         * U+10D6 GEORGIAN LETTER ZEN
         */
        GEORGIAN_ZEN,

        /**
         * U+10DF GEORGIAN LETTER ZHAR
         */
        GEORGIAN_ZHAR,

        /**
         * U+0060 GRAVE ACCENT
         */
        GRAVE,

        /**
         * U+003E GREATER-THAN SIGN
         */
        GREATER,

        /**
         * U+2265 GREATER-THAN OR EQUAL TO
         */
        GREATERTHANEQUAL,

        /**
         * U+0385 GREEK DIALYTIKA TONOS
         */
        GREEK_ACCENTDIERESIS,

        /**
         * U+03C2 GREEK SMALL LETTER FINAL SIGMA
         */
        GREEK_FINALSMALLSIGMA,

        /**
         * U+2015 HORIZONTAL BAR
         */
        GREEK_HORIZBAR,

        /**
         * U+0390 GREEK SMALL LETTER IOTA WITH DIALYTIKA AND TONOS
         */
        GREEK_IOTAACCENTDIERESIS,

        /**
         * old typo
         */
        GREEK_IOTADIAERESIS,

        /**
         * U+03B1 GREEK SMALL LETTER ALPHA
         */
        GREEK_LCASE_ALPHA,

        /**
         * U+03AC GREEK SMALL LETTER ALPHA WITH TONOS
         */
        GREEK_LCASE_ALPHAACCENT,

        /**
         * U+03B2 GREEK SMALL LETTER BETA
         */
        GREEK_LCASE_BETA,

        /**
         * U+03C7 GREEK SMALL LETTER CHI
         */
        GREEK_LCASE_CHI,

        /**
         * U+03B4 GREEK SMALL LETTER DELTA
         */
        GREEK_LCASE_DELTA,

        /**
         * U+03B5 GREEK SMALL LETTER EPSILON
         */
        GREEK_LCASE_EPSILON,

        /**
         * U+03AD GREEK SMALL LETTER EPSILON WITH TONOS
         */
        GREEK_LCASE_EPSILONACCENT,

        /**
         * U+03B7 GREEK SMALL LETTER ETA
         */
        GREEK_LCASE_ETA,

        /**
         * U+03AE GREEK SMALL LETTER ETA WITH TONOS
         */
        GREEK_LCASE_ETAACCENT,

        /**
         * U+03B3 GREEK SMALL LETTER GAMMA
         */
        GREEK_LCASE_GAMMA,

        /**
         * U+03B9 GREEK SMALL LETTER IOTA
         */
        GREEK_LCASE_IOTA,

        /**
         * U+03AF GREEK SMALL LETTER IOTA WITH TONOS
         */
        GREEK_LCASE_IOTAACCENT,

        /**
         * U+03CA GREEK SMALL LETTER IOTA WITH DIALYTIKA
         */
        GREEK_LCASE_IOTADIERESIS,

        /**
         * U+03BA GREEK SMALL LETTER KAPPA
         */
        GREEK_LCASE_KAPPA,

        /**
         * U+03BB GREEK SMALL LETTER LAMDA
         */
        GREEK_LCASE_LAMBDA,

        /**
         * U+03BB GREEK SMALL LETTER LAMDA
         */
        GREEK_LCASE_LAMDA,

        /**
         * U+03BC GREEK SMALL LETTER MU
         */
        GREEK_LCASE_MU,

        /**
         * U+03BD GREEK SMALL LETTER NU
         */
        GREEK_LCASE_NU,

        /**
         * U+03C9 GREEK SMALL LETTER OMEGA
         */
        GREEK_LCASE_OMEGA,

        /**
         * U+03CE GREEK SMALL LETTER OMEGA WITH TONOS
         */
        GREEK_LCASE_OMEGAACCENT,

        /**
         * U+03BF GREEK SMALL LETTER OMICRON
         */
        GREEK_LCASE_OMICRON,

        /**
         * U+03CC GREEK SMALL LETTER OMICRON WITH TONOS
         */
        GREEK_LCASE_OMICRONACCENT,

        /**
         * U+03C6 GREEK SMALL LETTER PHI
         */
        GREEK_LCASE_PHI,

        /**
         * U+03C0 GREEK SMALL LETTER PI
         */
        GREEK_LCASE_PI,

        /**
         * U+03C8 GREEK SMALL LETTER PSI
         */
        GREEK_LCASE_PSI,

        /**
         * U+03C1 GREEK SMALL LETTER RHO
         */
        GREEK_LCASE_RHO,

        /**
         * U+03C3 GREEK SMALL LETTER SIGMA
         */
        GREEK_LCASE_SIGMA,

        /**
         * U+03C4 GREEK SMALL LETTER TAU
         */
        GREEK_LCASE_TAU,

        /**
         * U+03B8 GREEK SMALL LETTER THETA
         */
        GREEK_LCASE_THETA,

        /**
         * U+03C5 GREEK SMALL LETTER UPSILON
         */
        GREEK_LCASE_UPSILON,

        /**
         * U+03CD GREEK SMALL LETTER UPSILON WITH TONOS
         */
        GREEK_LCASE_UPSILONACCENT,

        /**
         * U+03CB GREEK SMALL LETTER UPSILON WITH DIALYTIKA
         */
        GREEK_LCASE_UPSILONDIERESIS,

        /**
         * U+03BE GREEK SMALL LETTER XI
         */
        GREEK_LCASE_XI,

        /**
         * U+03B6 GREEK SMALL LETTER ZETA
         */
        GREEK_LCASE_ZETA,

        /**
         * Alias for mode_switch
         */
        GREEK_SWITCH,

        /**
         * U+0391 GREEK CAPITAL LETTER ALPHA
         */
        GREEK_UCASE_ALPHA,

        /**
         * U+0386 GREEK CAPITAL LETTER ALPHA WITH TONOS
         */
        GREEK_UCASE_ALPHAACCENT,

        /**
         * U+0392 GREEK CAPITAL LETTER BETA
         */
        GREEK_UCASE_BETA,

        /**
         * U+03A7 GREEK CAPITAL LETTER CHI
         */
        GREEK_UCASE_CHI,

        /**
         * U+0394 GREEK CAPITAL LETTER DELTA
         */
        GREEK_UCASE_DELTA,

        /**
         * U+0395 GREEK CAPITAL LETTER EPSILON
         */
        GREEK_UCASE_EPSILON,

        /**
         * U+0388 GREEK CAPITAL LETTER EPSILON WITH TONOS
         */
        GREEK_UCASE_EPSILONACCENT,

        /**
         * U+0397 GREEK CAPITAL LETTER ETA
         */
        GREEK_UCASE_ETA,

        /**
         * U+0389 GREEK CAPITAL LETTER ETA WITH TONOS
         */
        GREEK_UCASE_ETAACCENT,

        /**
         * U+0393 GREEK CAPITAL LETTER GAMMA
         */
        GREEK_UCASE_GAMMA,

        /**
         * U+0399 GREEK CAPITAL LETTER IOTA
         */
        GREEK_UCASE_IOTA,

        /**
         * U+038A GREEK CAPITAL LETTER IOTA WITH TONOS
         */
        GREEK_UCASE_IOTAACCENT,

        /**
         * U+03AA GREEK CAPITAL LETTER IOTA WITH DIALYTIKA
         */
        GREEK_UCASE_IOTADIERESIS,

        /**
         * U+039A GREEK CAPITAL LETTER KAPPA
         */
        GREEK_UCASE_KAPPA,

        /**
         * U+039B GREEK CAPITAL LETTER LAMDA
         */
        GREEK_UCASE_LAMBDA,

        /**
         * U+039B GREEK CAPITAL LETTER LAMDA
         */
        GREEK_UCASE_LAMDA,

        /**
         * U+039C GREEK CAPITAL LETTER MU
         */
        GREEK_UCASE_MU,

        /**
         * U+039D GREEK CAPITAL LETTER NU
         */
        GREEK_UCASE_NU,

        /**
         * U+03A9 GREEK CAPITAL LETTER OMEGA
         */
        GREEK_UCASE_OMEGA,

        /**
         * U+038F GREEK CAPITAL LETTER OMEGA WITH TONOS
         */
        GREEK_UCASE_OMEGAACCENT,

        /**
         * U+039F GREEK CAPITAL LETTER OMICRON
         */
        GREEK_UCASE_OMICRON,

        /**
         * U+038C GREEK CAPITAL LETTER OMICRON WITH TONOS
         */
        GREEK_UCASE_OMICRONACCENT,

        /**
         * U+03A6 GREEK CAPITAL LETTER PHI
         */
        GREEK_UCASE_PHI,

        /**
         * U+03A0 GREEK CAPITAL LETTER PI
         */
        GREEK_UCASE_PI,

        /**
         * U+03A8 GREEK CAPITAL LETTER PSI
         */
        GREEK_UCASE_PSI,

        /**
         * U+03A1 GREEK CAPITAL LETTER RHO
         */
        GREEK_UCASE_RHO,

        /**
         * U+03A3 GREEK CAPITAL LETTER SIGMA
         */
        GREEK_UCASE_SIGMA,

        /**
         * U+03A4 GREEK CAPITAL LETTER TAU
         */
        GREEK_UCASE_TAU,

        /**
         * U+0398 GREEK CAPITAL LETTER THETA
         */
        GREEK_UCASE_THETA,

        /**
         * U+03A5 GREEK CAPITAL LETTER UPSILON
         */
        GREEK_UCASE_UPSILON,

        /**
         * U+038E GREEK CAPITAL LETTER UPSILON WITH TONOS
         */
        GREEK_UCASE_UPSILONACCENT,

        /**
         * U+03AB GREEK CAPITAL LETTER UPSILON WITH DIALYTIKA
         */
        GREEK_UCASE_UPSILONDIERESIS,

        /**
         * U+039E GREEK CAPITAL LETTER XI
         */
        GREEK_UCASE_XI,

        /**
         * U+0396 GREEK CAPITAL LETTER ZETA
         */
        GREEK_UCASE_ZETA,

        /**
         * U+03B0 GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND TONOS
         */
        GREEK_UPSILONACCENTDIERESIS,

        /**
         * Guilder
         */
        GUILDER,

        /**
         * U+00AB LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
         */
        GUILLEMOTLEFT,

        /**
         * U+00BB RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
         */
        GUILLEMOTRIGHT,

        /**
         * U+200A HAIR SPACE
         */
        HAIRSPACE,

        /**
         * Hangul start/stop(toggle)
         */
        HANGUL,

        /**
         * Hangul A
         */
        HANGUL_A,

        /**
         * Hangul Ae
         */
        HANGUL_AE,

        /**
         * Hangul Arae A
         */
        HANGUL_ARAE_A,

        /**
         * Hangul Arae Ae
         */
        HANGUL_ARAE_AE,

        /**
         * Banja mode
         */
        HANGUL_BANJA,

        /**
         * Hangul Cieuc
         */
        HANGUL_CIEUC,

        /**
         * Hangul code input mode
         */
        HANGUL_CODEINPUT,

        /**
         * Hangul Dikeud
         */
        HANGUL_DIKEUD,

        /**
         * Hangul E
         */
        HANGUL_E,

        /**
         * Hangul end, English start
         */
        HANGUL_END,

        /**
         * Hangul Eo
         */
        HANGUL_EO,

        /**
         * Hangul Eu
         */
        HANGUL_EU,

        /**
         * Start Hangul->Hanja Conversion
         */
        HANGUL_HANJA,

        /**
         * Hangul Hieuh
         */
        HANGUL_HIEUH,

        /**
         * Hangul I
         */
        HANGUL_I,

        /**
         * Hangul Ieung
         */
        HANGUL_IEUNG,

        /**
         * Hangul Jamo mode
         */
        HANGUL_JAMO,

        /**
         * Jeonja mode
         */
        HANGUL_JEONJA,

        /**
         * Hangul Jieuj
         */
        HANGUL_JIEUJ,

        /**
         * Hangul J Cieuc
         */
        HANGUL_J_CIEUC,

        /**
         * Hangul J Dikeud
         */
        HANGUL_J_DIKEUD,

        /**
         * Hangul J Hieuh
         */
        HANGUL_J_HIEUH,

        /**
         * Hangul J Ieung
         */
        HANGUL_J_IEUNG,

        /**
         * Hangul J Jieuj
         */
        HANGUL_J_JIEUJ,

        /**
         * Hangul J Khieuq
         */
        HANGUL_J_KHIEUQ,

        /**
         * Hangul J Kiyeog
         */
        HANGUL_J_KIYEOG,

        /**
         * Hangul J Kiyeog Sios
         */
        HANGUL_J_KIYEOG_SIOS,

        /**
         * Hangul J Kkogji Dalrin Ieung
         */
        HANGUL_J_KKOGJI_DALRIN_IEUNG,

        /**
         * Hangul J Mieum
         */
        HANGUL_J_MIEUM,

        /**
         * Hangul J Nieun
         */
        HANGUL_J_NIEUN,

        /**
         * Hangul J Nieun Hieuh
         */
        HANGUL_J_NIEUN_HIEUH,

        /**
         * Hangul J Nieun Jieuj
         */
        HANGUL_J_NIEUN_JIEUJ,

        /**
         * Hangul J Pan Sios
         */
        HANGUL_J_PAN_SIOS,

        /**
         * Hangul J Phieuf
         */
        HANGUL_J_PHIEUF,

        /**
         * Hangul J Pieub
         */
        HANGUL_J_PIEUB,

        /**
         * Hangul J Pieub Sios
         */
        HANGUL_J_PIEUB_SIOS,

        /**
         * Hangul J Rieul
         */
        HANGUL_J_RIEUL,

        /**
         * Hangul J Rieul Hieuh
         */
        HANGUL_J_RIEUL_HIEUH,

        /**
         * Hangul J Rieul Kiyeog
         */
        HANGUL_J_RIEUL_KIYEOG,

        /**
         * Hangul J Rieul Mieum
         */
        HANGUL_J_RIEUL_MIEUM,

        /**
         * Hangul J Rieul Phieuf
         */
        HANGUL_J_RIEUL_PHIEUF,

        /**
         * Hangul J Rieul Pieub
         */
        HANGUL_J_RIEUL_PIEUB,

        /**
         * Hangul J Rieul Sios
         */
        HANGUL_J_RIEUL_SIOS,

        /**
         * Hangul J Rieul Tieut
         */
        HANGUL_J_RIEUL_TIEUT,

        /**
         * Hangul J Sios
         */
        HANGUL_J_SIOS,

        /**
         * Hangul J Ssang Kiyeog
         */
        HANGUL_J_SSANG_KIYEOG,

        /**
         * Hangul J Ssang Sios
         */
        HANGUL_J_SSANG_SIOS,

        /**
         * Hangul J Tieut
         */
        HANGUL_J_TIEUT,

        /**
         * Hangul J Yeorin Hieuh
         */
        HANGUL_J_YEORIN_HIEUH,

        /**
         * Hangul Khieuq
         */
        HANGUL_KHIEUQ,

        /**
         * Hangul Kiyeog
         */
        HANGUL_KIYEOG,

        /**
         * Hangul Kiyeog Sios
         */
        HANGUL_KIYEOG_SIOS,

        /**
         * Hangul Kkogji Dalrin Ieung
         */
        HANGUL_KKOGJI_DALRIN_IEUNG,

        /**
         * Hangul Mieum
         */
        HANGUL_MIEUM,

        /**
         * Multiple candidate
         */
        HANGUL_MULTIPLE_CANDIDATE,

        /**
         * Hangul Nieun
         */
        HANGUL_NIEUN,

        /**
         * Hangul Nieun Hieuh
         */
        HANGUL_NIEUN_HIEUH,

        /**
         * Hangul Nieun Jieuj
         */
        HANGUL_NIEUN_JIEUJ,

        /**
         * Hangul O
         */
        HANGUL_O,

        /**
         * Hangul Oe
         */
        HANGUL_OE,

        /**
         * Hangul Pan Sios
         */
        HANGUL_PAN_SIOS,

        /**
         * Hangul Phieuf
         */
        HANGUL_PHIEUF,

        /**
         * Hangul Pieub
         */
        HANGUL_PIEUB,

        /**
         * Hangul Pieub Sios
         */
        HANGUL_PIEUB_SIOS,

        /**
         * Post Hanja conversion
         */
        HANGUL_POST_HANJA,

        /**
         * Previous candidate
         */
        HANGUL_PREVIOUS_CANDIDATE,

        /**
         * Pre Hanja conversion
         */
        HANGUL_PRE_HANJA,

        /**
         * Hangul Rieul
         */
        HANGUL_RIEUL,

        /**
         * Hangul Rieul Hieuh
         */
        HANGUL_RIEUL_HIEUH,

        /**
         * Hangul Rieul Kiyeog
         */
        HANGUL_RIEUL_KIYEOG,

        /**
         * Hangul Rieul Mieum
         */
        HANGUL_RIEUL_MIEUM,

        /**
         * Hangul Rieul Phieuf
         */
        HANGUL_RIEUL_PHIEUF,

        /**
         * Hangul Rieul Pieub
         */
        HANGUL_RIEUL_PIEUB,

        /**
         * Hangul Rieul Sios
         */
        HANGUL_RIEUL_SIOS,

        /**
         * Hangul Rieul Tieut
         */
        HANGUL_RIEUL_TIEUT,

        /**
         * Hangul Rieul Yeorin Hieuh
         */
        HANGUL_RIEUL_YEORIN_HIEUH,

        /**
         * Hangul Romaja mode
         */
        HANGUL_ROMAJA,

        /**
         * Single candidate
         */
        HANGUL_SINGLE_CANDIDATE,

        /**
         * Hangul Sios
         */
        HANGUL_SIOS,

        /**
         * Special symbols
         */
        HANGUL_SPECIAL,

        /**
         * Hangul Ssang Dikeud
         */
        HANGUL_SSANG_DIKEUD,

        /**
         * Hangul Ssang Jieuj
         */
        HANGUL_SSANG_JIEUJ,

        /**
         * Hangul Ssang Kiyeog
         */
        HANGUL_SSANG_KIYEOG,

        /**
         * Hangul Ssang Pieub
         */
        HANGUL_SSANG_PIEUB,

        /**
         * Hangul Ssang Sios
         */
        HANGUL_SSANG_SIOS,

        /**
         * Hangul start
         */
        HANGUL_START,

        /**
         * Hangul Sunkyeongeum Mieum
         */
        HANGUL_SUNKYEONGEUM_MIEUM,

        /**
         * Hangul Sunkyeongeum Phieuf
         */
        HANGUL_SUNKYEONGEUM_PHIEUF,

        /**
         * Hangul Sunkyeongeum Pieub
         */
        HANGUL_SUNKYEONGEUM_PIEUB,

        /**
         * Alias for mode_switch
         */
        HANGUL_SWITCH,

        /**
         * Hangul Tieut
         */
        HANGUL_TIEUT,

        /**
         * Hangul U
         */
        HANGUL_U,

        /**
         * Hangul Wa
         */
        HANGUL_WA,

        /**
         * Hangul Wae
         */
        HANGUL_WAE,

        /**
         * Hangul We
         */
        HANGUL_WE,

        /**
         * Hangul Weo
         */
        HANGUL_WEO,

        /**
         * Hangul Wi
         */
        HANGUL_WI,

        /**
         * Hangul Ya
         */
        HANGUL_YA,

        /**
         * Hangul Yae
         */
        HANGUL_YAE,

        /**
         * Hangul Ye
         */
        HANGUL_YE,

        /**
         * Hangul Yeo
         */
        HANGUL_YEO,

        /**
         * Hangul Yeorin Hieuh
         */
        HANGUL_YEORIN_HIEUH,

        /**
         * Hangul Yi
         */
        HANGUL_YI,

        /**
         * Hangul Yo
         */
        HANGUL_YO,

        /**
         * Hangul Yu
         */
        HANGUL_YU,

        /**
         * to Hankaku
         */
        HANKAKU,

        /**
         * U+2665 BLACK HEART SUIT
         */
        HEART,

        /**
         * U+05D0 HEBREW LETTER ALEF
         */
        HEBREW_ALEPH,

        /**
         * U+05E2 HEBREW LETTER AYIN
         */
        HEBREW_AYIN,

        /**
         * U+05D1 HEBREW LETTER BET
         */
        HEBREW_BET,

        /**
         * deprecated
         */
        HEBREW_BETH,

        /**
         * U+05D7 HEBREW LETTER HET
         */
        HEBREW_CHET,

        /**
         * U+05D3 HEBREW LETTER DALET
         */
        HEBREW_DALET,

        /**
         * deprecated
         */
        HEBREW_DALETH,

        /**
         * U+2017 DOUBLE LOW LINE
         */
        HEBREW_DOUBLELOWLINE,

        /**
         * U+05DA HEBREW LETTER FINAL KAF
         */
        HEBREW_FINALKAPH,

        /**
         * U+05DD HEBREW LETTER FINAL MEM
         */
        HEBREW_FINALMEM,

        /**
         * U+05DF HEBREW LETTER FINAL NUN
         */
        HEBREW_FINALNUN,

        /**
         * U+05E3 HEBREW LETTER FINAL PE
         */
        HEBREW_FINALPE,

        /**
         * U+05E5 HEBREW LETTER FINAL TSADI
         */
        HEBREW_FINALZADE,

        /**
         * deprecated
         */
        HEBREW_FINALZADI,

        /**
         * U+05D2 HEBREW LETTER GIMEL
         */
        HEBREW_GIMEL,

        /**
         * deprecated
         */
        HEBREW_GIMMEL,

        /**
         * U+05D4 HEBREW LETTER HE
         */
        HEBREW_HE,

        /**
         * deprecated
         */
        HEBREW_HET,

        /**
         * U+05DB HEBREW LETTER KAF
         */
        HEBREW_KAPH,

        /**
         * deprecated
         */
        HEBREW_KUF,

        /**
         * U+05DC HEBREW LETTER LAMED
         */
        HEBREW_LAMED,

        /**
         * U+05DE HEBREW LETTER MEM
         */
        HEBREW_MEM,

        /**
         * U+05E0 HEBREW LETTER NUN
         */
        HEBREW_NUN,

        /**
         * U+05E4 HEBREW LETTER PE
         */
        HEBREW_PE,

        /**
         * U+05E7 HEBREW LETTER QOF
         */
        HEBREW_QOPH,

        /**
         * U+05E8 HEBREW LETTER RESH
         */
        HEBREW_RESH,

        /**
         * U+05E1 HEBREW LETTER SAMEKH
         */
        HEBREW_SAMECH,

        /**
         * deprecated
         */
        HEBREW_SAMEKH,

        /**
         * U+05E9 HEBREW LETTER SHIN
         */
        HEBREW_SHIN,

        /**
         * Alias for mode_switch
         */
        HEBREW_SWITCH,

        /**
         * deprecated
         */
        HEBREW_TAF,

        /**
         * U+05EA HEBREW LETTER TAV
         */
        HEBREW_TAW,

        /**
         * U+05D8 HEBREW LETTER TET
         */
        HEBREW_TET,

        /**
         * deprecated
         */
        HEBREW_TETH,

        /**
         * U+05D5 HEBREW LETTER VAV
         */
        HEBREW_WAW,

        /**
         * U+05D9 HEBREW LETTER YOD
         */
        HEBREW_YOD,

        /**
         * U+05E6 HEBREW LETTER TSADI
         */
        HEBREW_ZADE,

        /**
         * deprecated
         */
        HEBREW_ZADI,

        /**
         * U+05D6 HEBREW LETTER ZAYIN
         */
        HEBREW_ZAIN,

        /**
         * deprecated
         */
        HEBREW_ZAYIN,

        /**
         * Help
         */
        HELP,

        /**
         * Alias for Henkan_Mode
         */
        HENKAN,

        /**
         * Start/Stop Conversion
         */
        HENKAN_MODE,

        /**
         * Hexagram
         */
        HEXAGRAM,

        /**
         * to Hiragana
         */
        HIRAGANA,

        /**
         * Hiragana/Katakana toggle
         */
        HIRAGANA_KATAKANA,

        /**
         * Home
         */
        HOME,

        /**
         * U+2500 BOX DRAWINGS LIGHT HORIZONTAL
         */
        HORIZCONNECTOR,

        /**
         * U+23BA HORIZONTAL SCAN LINE-1
         */
        HORIZLINESCAN1,

        /**
         * U+23BB HORIZONTAL SCAN LINE-3
         */
        HORIZLINESCAN3,

        /**
         * U+2500 BOX DRAWINGS LIGHT HORIZONTAL
         */
        HORIZLINESCAN5,

        /**
         * U+23BC HORIZONTAL SCAN LINE-7
         */
        HORIZLINESCAN7,

        /**
         * U+23BD HORIZONTAL SCAN LINE-9
         */
        HORIZLINESCAN9,

        /**
         * Hpblock
         */
        HPBLOCK,

        /**
         * Hpguilder
         */
        HPGUILDER,

        /**
         * Hplira
         */
        HPLIRA,

        /**
         * Hplongminus
         */
        HPLONGMINUS,

        /**
         * Hpmute Acute
         */
        HPMUTE_ACUTE,

        /**
         * Hpmute Asciicircum
         */
        HPMUTE_ASCIICIRCUM,

        /**
         * Hpmute Asciitilde
         */
        HPMUTE_ASCIITILDE,

        /**
         * Hpmute Diaeresis
         */
        HPMUTE_DIAERESIS,

        /**
         * Hpmute Grave
         */
        HPMUTE_GRAVE,

        /**
         * Hp Back Tab
         */
        HP_BACK_TAB,

        /**
         * Hp Clear Line
         */
        HP_CLEAR_LINE,

        /**
         * Hp Delete Char
         */
        HP_DELETE_CHAR,

        /**
         * Hp Delete Line
         */
        HP_DELETE_LINE,

        /**
         * Hp Insert Char
         */
        HP_INSERT_CHAR,

        /**
         * Hp Insert Line
         */
        HP_INSERT_LINE,

        /**
         * Hp Io
         */
        HP_IO,

        /**
         * Hp Kp Back Tab
         */
        HP_KP_BACK_TAB,

        /**
         * Hp Modelock1
         */
        HP_MODELOCK1,

        /**
         * Hp Modelock2
         */
        HP_MODELOCK2,

        /**
         * Hp Reset
         */
        HP_RESET,

        /**
         * Hp System
         */
        HP_SYSTEM,

        /**
         * Hp User
         */
        HP_USER,

        /**
         * Hp Ydiaeresis
         */
        HP_YDIAERESIS,

        /**
         * U+2409 SYMBOL FOR HORIZONTAL TABULATION
         */
        HT,

        /**
         * Left hyper
         */
        HYPER_L,

        /**
         * Right hyper
         */
        HYPER_R,

        /**
         * U+00AD SOFT HYPHEN
         */
        HYPHEN,

        /**
         * U+0130 LATIN CAPITAL LETTER I WITH DOT ABOVE
         */
        IABOVEDOT,

        /**
         * U+2261 IDENTICAL TO
         */
        IDENTICAL,

        /**
         * U+0131 LATIN SMALL LETTER DOTLESS I
         */
        IDOTLESS,

        /**
         * U+21D4 LEFT RIGHT DOUBLE ARROW
         */
        IFONLYIF,

        /**
         * U+21D2 RIGHTWARDS DOUBLE ARROW
         */
        IMPLIES,

        /**
         * U+2282 SUBSET OF
         */
        INCLUDEDIN,

        /**
         * U+2283 SUPERSET OF
         */
        INCLUDES,

        /**
         * U+221E INFINITY
         */
        INFINITY,

        /**
         * Insert, insert here
         */
        INSERT,

        /**
         * Insert Char
         */
        INSERT_CHAR,

        /**
         * Insert Line
         */
        INSERT_LINE,

        /**
         * U+222B INTEGRAL
         */
        INTEGRAL,

        /**
         * U+2229 INTERSECTION
         */
        INTERSECTION,

        /**
         * Io
         */
        IO,

        /**
         * Iso Center Object
         */
        ISO_CENTER_OBJECT,

        /**
         * Iso Continuous Underline
         */
        ISO_CONTINUOUS_UNDERLINE,

        /**
         * Iso Discontinuous Underline
         */
        ISO_DISCONTINUOUS_UNDERLINE,

        /**
         * Iso Emphasize
         */
        ISO_EMPHASIZE,

        /**
         * Iso Enter
         */
        ISO_ENTER,

        /**
         * Iso Fast Cursor Down
         */
        ISO_FAST_CURSOR_DOWN,

        /**
         * Iso Fast Cursor Left
         */
        ISO_FAST_CURSOR_LEFT,

        /**
         * Iso Fast Cursor Right
         */
        ISO_FAST_CURSOR_RIGHT,

        /**
         * Iso Fast Cursor Up
         */
        ISO_FAST_CURSOR_UP,

        /**
         * Iso First Group
         */
        ISO_FIRST_GROUP,

        /**
         * Iso First Group Lock
         */
        ISO_FIRST_GROUP_LOCK,

        /**
         * Iso Group Latch
         */
        ISO_GROUP_LATCH,

        /**
         * Iso Group Lock
         */
        ISO_GROUP_LOCK,

        /**
         * Alias for mode_switch
         */
        ISO_GROUP_SHIFT,

        /**
         * Iso Last Group
         */
        ISO_LAST_GROUP,

        /**
         * Iso Last Group Lock
         */
        ISO_LAST_GROUP_LOCK,

        /**
         * Iso Left Tab
         */
        ISO_LEFT_TAB,

        /**
         * Iso Level2 Latch
         */
        ISO_LEVEL2_LATCH,

        /**
         * Iso Level3 Latch
         */
        ISO_LEVEL3_LATCH,

        /**
         * Iso Level3 Lock
         */
        ISO_LEVEL3_LOCK,

        /**
         * Iso Level3 Shift
         */
        ISO_LEVEL3_SHIFT,

        /**
         * Iso Level5 Latch
         */
        ISO_LEVEL5_LATCH,

        /**
         * Iso Level5 Lock
         */
        ISO_LEVEL5_LOCK,

        /**
         * Iso Level5 Shift
         */
        ISO_LEVEL5_SHIFT,

        /**
         * Iso Lock
         */
        ISO_LOCK,

        /**
         * Iso Move Line Down
         */
        ISO_MOVE_LINE_DOWN,

        /**
         * Iso Move Line Up
         */
        ISO_MOVE_LINE_UP,

        /**
         * Iso Next Group
         */
        ISO_NEXT_GROUP,

        /**
         * Iso Next Group Lock
         */
        ISO_NEXT_GROUP_LOCK,

        /**
         * Iso Partial Line Down
         */
        ISO_PARTIAL_LINE_DOWN,

        /**
         * Iso Partial Line Up
         */
        ISO_PARTIAL_LINE_UP,

        /**
         * Iso Partial Space Left
         */
        ISO_PARTIAL_SPACE_LEFT,

        /**
         * Iso Partial Space Right
         */
        ISO_PARTIAL_SPACE_RIGHT,

        /**
         * Iso Prev Group
         */
        ISO_PREV_GROUP,

        /**
         * Iso Prev Group Lock
         */
        ISO_PREV_GROUP_LOCK,

        /**
         * Iso Release Both Margins
         */
        ISO_RELEASE_BOTH_MARGINS,

        /**
         * Iso Release Margin Left
         */
        ISO_RELEASE_MARGIN_LEFT,

        /**
         * Iso Release Margin Right
         */
        ISO_RELEASE_MARGIN_RIGHT,

        /**
         * Iso Set Margin Left
         */
        ISO_SET_MARGIN_LEFT,

        /**
         * Iso Set Margin Right
         */
        ISO_SET_MARGIN_RIGHT,

        /**
         * U+2218 RING OPERATOR
         */
        JOT,

        /**
         * U+30C1 KATAKANA LETTER TI
         */
        KANA_CHI,

        /**
         * U+300D RIGHT CORNER BRACKET
         */
        KANA_CLOSINGBRACKET,

        /**
         * U+3001 IDEOGRAPHIC COMMA
         */
        KANA_COMMA,

        /**
         * U+30FB KATAKANA MIDDLE DOT
         */
        KANA_CONJUNCTIVE,

        /**
         * U+30D5 KATAKANA LETTER HU
         */
        KANA_FU,

        /**
         * U+3002 IDEOGRAPHIC FULL STOP
         */
        KANA_FULLSTOP,

        /**
         * U+30CF KATAKANA LETTER HA
         */
        KANA_HA,

        /**
         * U+30D8 KATAKANA LETTER HE
         */
        KANA_HE,

        /**
         * U+30D2 KATAKANA LETTER HI
         */
        KANA_HI,

        /**
         * U+30DB KATAKANA LETTER HO
         */
        KANA_HO,

        /**
         * deprecated
         */
        KANA_HU,

        /**
         * U+30AB KATAKANA LETTER KA
         */
        KANA_KA,

        /**
         * U+30B1 KATAKANA LETTER KE
         */
        KANA_KE,

        /**
         * U+30AD KATAKANA LETTER KI
         */
        KANA_KI,

        /**
         * U+30B3 KATAKANA LETTER KO
         */
        KANA_KO,

        /**
         * U+30AF KATAKANA LETTER KU
         */
        KANA_KU,

        /**
         * U+30A1 KATAKANA LETTER SMALL A
         */
        KANA_LCASE_A,

        /**
         * U+30A7 KATAKANA LETTER SMALL E
         */
        KANA_LCASE_E,

        /**
         * U+30A3 KATAKANA LETTER SMALL I
         */
        KANA_LCASE_I,

        /**
         * U+30A9 KATAKANA LETTER SMALL O
         */
        KANA_LCASE_O,

        /**
         * U+30C3 KATAKANA LETTER SMALL TU
         */
        KANA_LCASE_TSU,

        /**
         * deprecated
         */
        KANA_LCASE_TU,

        /**
         * U+30A5 KATAKANA LETTER SMALL U
         */
        KANA_LCASE_U,

        /**
         * U+30E3 KATAKANA LETTER SMALL YA
         */
        KANA_LCASE_YA,

        /**
         * U+30E7 KATAKANA LETTER SMALL YO
         */
        KANA_LCASE_YO,

        /**
         * U+30E5 KATAKANA LETTER SMALL YU
         */
        KANA_LCASE_YU,

        /**
         * Kana Lock
         */
        KANA_LOCK,

        /**
         * U+30DE KATAKANA LETTER MA
         */
        KANA_MA,

        /**
         * U+30E1 KATAKANA LETTER ME
         */
        KANA_ME,

        /**
         * U+30DF KATAKANA LETTER MI
         */
        KANA_MI,

        /**
         * deprecated
         */
        KANA_MIDDLEDOT,

        /**
         * U+30E2 KATAKANA LETTER MO
         */
        KANA_MO,

        /**
         * U+30E0 KATAKANA LETTER MU
         */
        KANA_MU,

        /**
         * U+30F3 KATAKANA LETTER N
         */
        KANA_N,

        /**
         * U+30CA KATAKANA LETTER NA
         */
        KANA_NA,

        /**
         * U+30CD KATAKANA LETTER NE
         */
        KANA_NE,

        /**
         * U+30CB KATAKANA LETTER NI
         */
        KANA_NI,

        /**
         * U+30CE KATAKANA LETTER NO
         */
        KANA_NO,

        /**
         * U+30CC KATAKANA LETTER NU
         */
        KANA_NU,

        /**
         * U+300C LEFT CORNER BRACKET
         */
        KANA_OPENINGBRACKET,

        /**
         * U+30E9 KATAKANA LETTER RA
         */
        KANA_RA,

        /**
         * U+30EC KATAKANA LETTER RE
         */
        KANA_RE,

        /**
         * U+30EA KATAKANA LETTER RI
         */
        KANA_RI,

        /**
         * U+30ED KATAKANA LETTER RO
         */
        KANA_RO,

        /**
         * U+30EB KATAKANA LETTER RU
         */
        KANA_RU,

        /**
         * U+30B5 KATAKANA LETTER SA
         */
        KANA_SA,

        /**
         * U+30BB KATAKANA LETTER SE
         */
        KANA_SE,

        /**
         * U+30B7 KATAKANA LETTER SI
         */
        KANA_SHI,

        /**
         * Kana Shift
         */
        KANA_SHIFT,

        /**
         * U+30BD KATAKANA LETTER SO
         */
        KANA_SO,

        /**
         * U+30B9 KATAKANA LETTER SU
         */
        KANA_SU,

        /**
         * Alias for mode_switch
         */
        KANA_SWITCH,

        /**
         * U+30BF KATAKANA LETTER TA
         */
        KANA_TA,

        /**
         * U+30C6 KATAKANA LETTER TE
         */
        KANA_TE,

        /**
         * deprecated
         */
        KANA_TI,

        /**
         * U+30C8 KATAKANA LETTER TO
         */
        KANA_TO,

        /**
         * U+30A2 KATAKANA LETTER A
         */
        KANA_UCASE_A,

        /**
         * U+30A8 KATAKANA LETTER E
         */
        KANA_UCASE_E,

        /**
         * U+30A4 KATAKANA LETTER I
         */
        KANA_UCASE_I,

        /**
         * U+30AA KATAKANA LETTER O
         */
        KANA_UCASE_O,

        /**
         * U+30C4 KATAKANA LETTER TU
         */
        KANA_UCASE_TSU,

        /**
         * deprecated
         */
        KANA_UCASE_TU,

        /**
         * U+30A6 KATAKANA LETTER U
         */
        KANA_UCASE_U,

        /**
         * U+30E4 KATAKANA LETTER YA
         */
        KANA_UCASE_YA,

        /**
         * U+30E8 KATAKANA LETTER YO
         */
        KANA_UCASE_YO,

        /**
         * U+30E6 KATAKANA LETTER YU
         */
        KANA_UCASE_YU,

        /**
         * U+30EF KATAKANA LETTER WA
         */
        KANA_WA,

        /**
         * U+30F2 KATAKANA LETTER WO
         */
        KANA_WO,

        /**
         * Kanji, Kanji convert
         */
        KANJI,

        /**
         * Codeinput
         */
        KANJI_BANGOU,

        /**
         * deprecated
         */
        KAPPA,

        /**
         * to Katakana
         */
        KATAKANA,

        /**
         * U+20A9 WON SIGN
         */
        KOREAN_WON,

        /**
         * Kp 0
         */
        KP_0,

        /**
         * Kp 1
         */
        KP_1,

        /**
         * Kp 2
         */
        KP_2,

        /**
         * Kp 3
         */
        KP_3,

        /**
         * Kp 4
         */
        KP_4,

        /**
         * Kp 5
         */
        KP_5,

        /**
         * Kp 6
         */
        KP_6,

        /**
         * Kp 7
         */
        KP_7,

        /**
         * Kp 8
         */
        KP_8,

        /**
         * Kp 9
         */
        KP_9,

        /**
         * Kp Add
         */
        KP_ADD,

        /**
         * Kp Back Tab
         */
        KP_BACK_TAB,

        /**
         * Kp Begin
         */
        KP_BEGIN,

        /**
         * Kp Decimal
         */
        KP_DECIMAL,

        /**
         * Kp Delete
         */
        KP_DELETE,

        /**
         * Kp Divide
         */
        KP_DIVIDE,

        /**
         * Kp Down
         */
        KP_DOWN,

        /**
         * Kp End
         */
        KP_END,

        /**
         * Enter
         */
        KP_ENTER,

        /**
         * Equals
         */
        KP_EQUAL,

        /**
         * PF1, KP_A, ...
         */
        KP_F1,

        /**
         * Kp F2
         */
        KP_F2,

        /**
         * Kp F3
         */
        KP_F3,

        /**
         * Kp F4
         */
        KP_F4,

        /**
         * Kp Home
         */
        KP_HOME,

        /**
         * Kp Insert
         */
        KP_INSERT,

        /**
         * Kp Left
         */
        KP_LEFT,

        /**
         * Kp Multiply
         */
        KP_MULTIPLY,

        /**
         * Kp Next
         */
        KP_NEXT,

        /**
         * Kp Page Down
         */
        KP_PAGE_DOWN,

        /**
         * Kp Page Up
         */
        KP_PAGE_UP,

        /**
         * Kp Prior
         */
        KP_PRIOR,

        /**
         * Kp Right
         */
        KP_RIGHT,

        /**
         * Separator, often comma
         */
        KP_SEPARATOR,

        /**
         * Space
         */
        KP_SPACE,

        /**
         * Kp Subtract
         */
        KP_SUBTRACT,

        /**
         * Kp Tab
         */
        KP_TAB,

        /**
         * Kp Up
         */
        KP_UP,

        /**
         * U+0138 LATIN SMALL LETTER KRA
         */
        KRA,

        /**
         * L1
         */
        L1,

        /**
         * L10
         */
        L10,

        /**
         * L2
         */
        L2,

        /**
         * L3
         */
        L3,

        /**
         * L4
         */
        L4,

        /**
         * L5
         */
        L5,

        /**
         * L6
         */
        L6,

        /**
         * L7
         */
        L7,

        /**
         * L8
         */
        L8,

        /**
         * L9
         */
        L9,

        /**
         * Last Virtual Screen
         */
        LAST_VIRTUAL_SCREEN,

        /**
         * U+271D LATIN CROSS
         */
        LATINCROSS,

        /**
         * U+0061 LATIN SMALL LETTER A
         */
        LCASE_A,

        /**
         * U+00E1 LATIN SMALL LETTER A WITH ACUTE
         */
        LCASE_AACUTE,

        /**
         * U+1EA1 LATIN SMALL LETTER A WITH DOT BELOW
         */
        LCASE_ABELOWDOT,

        /**
         * U+0103 LATIN SMALL LETTER A WITH BREVE
         */
        LCASE_ABREVE,

        /**
         * U+1EAF LATIN SMALL LETTER A WITH BREVE AND ACUTE
         */
        LCASE_ABREVEACUTE,

        /**
         * U+1EB7 LATIN SMALL LETTER A WITH BREVE AND DOT BELOW
         */
        LCASE_ABREVEBELOWDOT,

        /**
         * U+1EB1 LATIN SMALL LETTER A WITH BREVE AND GRAVE
         */
        LCASE_ABREVEGRAVE,

        /**
         * U+1EB3 LATIN SMALL LETTER A WITH BREVE AND HOOK ABOVE
         */
        LCASE_ABREVEHOOK,

        /**
         * U+1EB5 LATIN SMALL LETTER A WITH BREVE AND TILDE
         */
        LCASE_ABREVETILDE,

        /**
         * U+00E2 LATIN SMALL LETTER A WITH CIRCUMFLEX
         */
        LCASE_ACIRCUMFLEX,

        /**
         * U+1EA5 LATIN SMALL LETTER A WITH CIRCUMFLEX AND ACUTE
         */
        LCASE_ACIRCUMFLEXACUTE,

        /**
         * U+1EAD LATIN SMALL LETTER A WITH CIRCUMFLEX AND DOT BELOW
         */
        LCASE_ACIRCUMFLEXBELOWDOT,

        /**
         * U+1EA7 LATIN SMALL LETTER A WITH CIRCUMFLEX AND GRAVE
         */
        LCASE_ACIRCUMFLEXGRAVE,

        /**
         * U+1EA9 LATIN SMALL LETTER A WITH CIRCUMFLEX AND HOOK ABOVE
         */
        LCASE_ACIRCUMFLEXHOOK,

        /**
         * U+1EAB LATIN SMALL LETTER A WITH CIRCUMFLEX AND TILDE
         */
        LCASE_ACIRCUMFLEXTILDE,

        /**
         * U+00E4 LATIN SMALL LETTER A WITH DIAERESIS
         */
        LCASE_ADIAERESIS,

        /**
         * U+00E6 LATIN SMALL LETTER AE
         */
        LCASE_AE,

        /**
         * U+00E0 LATIN SMALL LETTER A WITH GRAVE
         */
        LCASE_AGRAVE,

        /**
         * U+1EA3 LATIN SMALL LETTER A WITH HOOK ABOVE
         */
        LCASE_AHOOK,

        /**
         * U+0101 LATIN SMALL LETTER A WITH MACRON
         */
        LCASE_AMACRON,

        /**
         * U+0105 LATIN SMALL LETTER A WITH OGONEK
         */
        LCASE_AOGONEK,

        /**
         * U+00E5 LATIN SMALL LETTER A WITH RING ABOVE
         */
        LCASE_ARING,

        /**
         * U+00E3 LATIN SMALL LETTER A WITH TILDE
         */
        LCASE_ATILDE,

        /**
         * U+0062 LATIN SMALL LETTER B
         */
        LCASE_B,

        /**
         * U+1E03 LATIN SMALL LETTER B WITH DOT ABOVE
         */
        LCASE_BABOVEDOT,

        /**
         * U+0063 LATIN SMALL LETTER C
         */
        LCASE_C,

        /**
         * U+010B LATIN SMALL LETTER C WITH DOT ABOVE
         */
        LCASE_CABOVEDOT,

        /**
         * U+0107 LATIN SMALL LETTER C WITH ACUTE
         */
        LCASE_CACUTE,

        /**
         * U+010D LATIN SMALL LETTER C WITH CARON
         */
        LCASE_CCARON,

        /**
         * U+00E7 LATIN SMALL LETTER C WITH CEDILLA
         */
        LCASE_CCEDILLA,

        /**
         * U+0109 LATIN SMALL LETTER C WITH CIRCUMFLEX
         */
        LCASE_CCIRCUMFLEX,

        /**
         * Lcase Ch
         */
        LCASE_CH,

        /**
         * Lcase C H
         */
        LCASE_C_H,

        /**
         * U+0064 LATIN SMALL LETTER D
         */
        LCASE_D,

        /**
         * U+1E0B LATIN SMALL LETTER D WITH DOT ABOVE
         */
        LCASE_DABOVEDOT,

        /**
         * U+010F LATIN SMALL LETTER D WITH CARON
         */
        LCASE_DCARON,

        /**
         * U+0111 LATIN SMALL LETTER D WITH STROKE
         */
        LCASE_DSTROKE,

        /**
         * U+0065 LATIN SMALL LETTER E
         */
        LCASE_E,

        /**
         * U+0117 LATIN SMALL LETTER E WITH DOT ABOVE
         */
        LCASE_EABOVEDOT,

        /**
         * U+00E9 LATIN SMALL LETTER E WITH ACUTE
         */
        LCASE_EACUTE,

        /**
         * U+1EB9 LATIN SMALL LETTER E WITH DOT BELOW
         */
        LCASE_EBELOWDOT,

        /**
         * U+011B LATIN SMALL LETTER E WITH CARON
         */
        LCASE_ECARON,

        /**
         * U+00EA LATIN SMALL LETTER E WITH CIRCUMFLEX
         */
        LCASE_ECIRCUMFLEX,

        /**
         * U+1EBF LATIN SMALL LETTER E WITH CIRCUMFLEX AND ACUTE
         */
        LCASE_ECIRCUMFLEXACUTE,

        /**
         * U+1EC7 LATIN SMALL LETTER E WITH CIRCUMFLEX AND DOT BELOW
         */
        LCASE_ECIRCUMFLEXBELOWDOT,

        /**
         * U+1EC1 LATIN SMALL LETTER E WITH CIRCUMFLEX AND GRAVE
         */
        LCASE_ECIRCUMFLEXGRAVE,

        /**
         * U+1EC3 LATIN SMALL LETTER E WITH CIRCUMFLEX AND HOOK ABOVE
         */
        LCASE_ECIRCUMFLEXHOOK,

        /**
         * U+1EC5 LATIN SMALL LETTER E WITH CIRCUMFLEX AND TILDE
         */
        LCASE_ECIRCUMFLEXTILDE,

        /**
         * U+00EB LATIN SMALL LETTER E WITH DIAERESIS
         */
        LCASE_EDIAERESIS,

        /**
         * U+00E8 LATIN SMALL LETTER E WITH GRAVE
         */
        LCASE_EGRAVE,

        /**
         * U+1EBB LATIN SMALL LETTER E WITH HOOK ABOVE
         */
        LCASE_EHOOK,

        /**
         * U+0113 LATIN SMALL LETTER E WITH MACRON
         */
        LCASE_EMACRON,

        /**
         * U+014B LATIN SMALL LETTER ENG
         */
        LCASE_ENG,

        /**
         * U+0119 LATIN SMALL LETTER E WITH OGONEK
         */
        LCASE_EOGONEK,

        /**
         * U+00F0 LATIN SMALL LETTER ETH
         */
        LCASE_ETH,

        /**
         * U+1EBD LATIN SMALL LETTER E WITH TILDE
         */
        LCASE_ETILDE,

        /**
         * U+0292 LATIN SMALL LETTER EZH
         */
        LCASE_EZH,

        /**
         * U+0066 LATIN SMALL LETTER F
         */
        LCASE_F,

        /**
         * U+1E1F LATIN SMALL LETTER F WITH DOT ABOVE
         */
        LCASE_FABOVEDOT,

        /**
         * U+0067 LATIN SMALL LETTER G
         */
        LCASE_G,

        /**
         * U+0121 LATIN SMALL LETTER G WITH DOT ABOVE
         */
        LCASE_GABOVEDOT,

        /**
         * U+011F LATIN SMALL LETTER G WITH BREVE
         */
        LCASE_GBREVE,

        /**
         * U+01E7 LATIN SMALL LETTER G WITH CARON
         */
        LCASE_GCARON,

        /**
         * U+0123 LATIN SMALL LETTER G WITH CEDILLA
         */
        LCASE_GCEDILLA,

        /**
         * U+011D LATIN SMALL LETTER G WITH CIRCUMFLEX
         */
        LCASE_GCIRCUMFLEX,

        /**
         * U+0068 LATIN SMALL LETTER H
         */
        LCASE_H,

        /**
         * U+0125 LATIN SMALL LETTER H WITH CIRCUMFLEX
         */
        LCASE_HCIRCUMFLEX,

        /**
         * U+0127 LATIN SMALL LETTER H WITH STROKE
         */
        LCASE_HSTROKE,

        /**
         * U+0069 LATIN SMALL LETTER I
         */
        LCASE_I,

        /**
         * U+00ED LATIN SMALL LETTER I WITH ACUTE
         */
        LCASE_IACUTE,

        /**
         * U+1ECB LATIN SMALL LETTER I WITH DOT BELOW
         */
        LCASE_IBELOWDOT,

        /**
         * U+012D LATIN SMALL LETTER I WITH BREVE
         */
        LCASE_IBREVE,

        /**
         * U+00EE LATIN SMALL LETTER I WITH CIRCUMFLEX
         */
        LCASE_ICIRCUMFLEX,

        /**
         * U+00EF LATIN SMALL LETTER I WITH DIAERESIS
         */
        LCASE_IDIAERESIS,

        /**
         * U+00EC LATIN SMALL LETTER I WITH GRAVE
         */
        LCASE_IGRAVE,

        /**
         * U+1EC9 LATIN SMALL LETTER I WITH HOOK ABOVE
         */
        LCASE_IHOOK,

        /**
         * U+012B LATIN SMALL LETTER I WITH MACRON
         */
        LCASE_IMACRON,

        /**
         * U+012F LATIN SMALL LETTER I WITH OGONEK
         */
        LCASE_IOGONEK,

        /**
         * U+0129 LATIN SMALL LETTER I WITH TILDE
         */
        LCASE_ITILDE,

        /**
         * U+006A LATIN SMALL LETTER J
         */
        LCASE_J,

        /**
         * U+0135 LATIN SMALL LETTER J WITH CIRCUMFLEX
         */
        LCASE_JCIRCUMFLEX,

        /**
         * U+006B LATIN SMALL LETTER K
         */
        LCASE_K,

        /**
         * U+0137 LATIN SMALL LETTER K WITH CEDILLA
         */
        LCASE_KCEDILLA,

        /**
         * U+006C LATIN SMALL LETTER L
         */
        LCASE_L,

        /**
         * U+013A LATIN SMALL LETTER L WITH ACUTE
         */
        LCASE_LACUTE,

        /**
         * U+1E37 LATIN SMALL LETTER L WITH DOT BELOW
         */
        LCASE_LBELOWDOT,

        /**
         * U+013E LATIN SMALL LETTER L WITH CARON
         */
        LCASE_LCARON,

        /**
         * U+013C LATIN SMALL LETTER L WITH CEDILLA
         */
        LCASE_LCEDILLA,

        /**
         * U+0142 LATIN SMALL LETTER L WITH STROKE
         */
        LCASE_LSTROKE,

        /**
         * U+006D LATIN SMALL LETTER M
         */
        LCASE_M,

        /**
         * U+1E41 LATIN SMALL LETTER M WITH DOT ABOVE
         */
        LCASE_MABOVEDOT,

        /**
         * U+006E LATIN SMALL LETTER N
         */
        LCASE_N,

        /**
         * U+0144 LATIN SMALL LETTER N WITH ACUTE
         */
        LCASE_NACUTE,

        /**
         * U+0148 LATIN SMALL LETTER N WITH CARON
         */
        LCASE_NCARON,

        /**
         * U+0146 LATIN SMALL LETTER N WITH CEDILLA
         */
        LCASE_NCEDILLA,

        /**
         * U+00F1 LATIN SMALL LETTER N WITH TILDE
         */
        LCASE_NTILDE,

        /**
         * U+006F LATIN SMALL LETTER O
         */
        LCASE_O,

        /**
         * U+00F3 LATIN SMALL LETTER O WITH ACUTE
         */
        LCASE_OACUTE,

        /**
         * U+0275 LATIN SMALL LETTER BARRED O
         */
        LCASE_OBARRED,

        /**
         * U+1ECD LATIN SMALL LETTER O WITH DOT BELOW
         */
        LCASE_OBELOWDOT,

        /**
         * U+01D2 LATIN SMALL LETTER O WITH CARON
         */
        LCASE_OCARON,

        /**
         * U+00F4 LATIN SMALL LETTER O WITH CIRCUMFLEX
         */
        LCASE_OCIRCUMFLEX,

        /**
         * U+1ED1 LATIN SMALL LETTER O WITH CIRCUMFLEX AND ACUTE
         */
        LCASE_OCIRCUMFLEXACUTE,

        /**
         * U+1ED9 LATIN SMALL LETTER O WITH CIRCUMFLEX AND DOT BELOW
         */
        LCASE_OCIRCUMFLEXBELOWDOT,

        /**
         * U+1ED3 LATIN SMALL LETTER O WITH CIRCUMFLEX AND GRAVE
         */
        LCASE_OCIRCUMFLEXGRAVE,

        /**
         * U+1ED5 LATIN SMALL LETTER O WITH CIRCUMFLEX AND HOOK ABOVE
         */
        LCASE_OCIRCUMFLEXHOOK,

        /**
         * U+1ED7 LATIN SMALL LETTER O WITH CIRCUMFLEX AND TILDE
         */
        LCASE_OCIRCUMFLEXTILDE,

        /**
         * U+00F6 LATIN SMALL LETTER O WITH DIAERESIS
         */
        LCASE_ODIAERESIS,

        /**
         * U+0151 LATIN SMALL LETTER O WITH DOUBLE ACUTE
         */
        LCASE_ODOUBLEACUTE,

        /**
         * U+0153 LATIN SMALL LIGATURE OE
         */
        LCASE_OE,

        /**
         * U+00F2 LATIN SMALL LETTER O WITH GRAVE
         */
        LCASE_OGRAVE,

        /**
         * U+1ECF LATIN SMALL LETTER O WITH HOOK ABOVE
         */
        LCASE_OHOOK,

        /**
         * U+01A1 LATIN SMALL LETTER O WITH HORN
         */
        LCASE_OHORN,

        /**
         * U+1EDB LATIN SMALL LETTER O WITH HORN AND ACUTE
         */
        LCASE_OHORNACUTE,

        /**
         * U+1EE3 LATIN SMALL LETTER O WITH HORN AND DOT BELOW
         */
        LCASE_OHORNBELOWDOT,

        /**
         * U+1EDD LATIN SMALL LETTER O WITH HORN AND GRAVE
         */
        LCASE_OHORNGRAVE,

        /**
         * U+1EDF LATIN SMALL LETTER O WITH HORN AND HOOK ABOVE
         */
        LCASE_OHORNHOOK,

        /**
         * U+1EE1 LATIN SMALL LETTER O WITH HORN AND TILDE
         */
        LCASE_OHORNTILDE,

        /**
         * U+014D LATIN SMALL LETTER O WITH MACRON
         */
        LCASE_OMACRON,

        /**
         * U+00F8 LATIN SMALL LETTER O WITH STROKE
         */
        LCASE_OOBLIQUE,

        /**
         * U+00F8 LATIN SMALL LETTER O WITH STROKE
         */
        LCASE_OSLASH,

        /**
         * U+00F5 LATIN SMALL LETTER O WITH TILDE
         */
        LCASE_OTILDE,

        /**
         * U+0070 LATIN SMALL LETTER P
         */
        LCASE_P,

        /**
         * U+1E57 LATIN SMALL LETTER P WITH DOT ABOVE
         */
        LCASE_PABOVEDOT,

        /**
         * U+0071 LATIN SMALL LETTER Q
         */
        LCASE_Q,

        /**
         * U+0072 LATIN SMALL LETTER R
         */
        LCASE_R,

        /**
         * U+0155 LATIN SMALL LETTER R WITH ACUTE
         */
        LCASE_RACUTE,

        /**
         * U+0159 LATIN SMALL LETTER R WITH CARON
         */
        LCASE_RCARON,

        /**
         * U+0157 LATIN SMALL LETTER R WITH CEDILLA
         */
        LCASE_RCEDILLA,

        /**
         * U+0073 LATIN SMALL LETTER S
         */
        LCASE_S,

        /**
         * U+1E61 LATIN SMALL LETTER S WITH DOT ABOVE
         */
        LCASE_SABOVEDOT,

        /**
         * U+015B LATIN SMALL LETTER S WITH ACUTE
         */
        LCASE_SACUTE,

        /**
         * U+0161 LATIN SMALL LETTER S WITH CARON
         */
        LCASE_SCARON,

        /**
         * U+015F LATIN SMALL LETTER S WITH CEDILLA
         */
        LCASE_SCEDILLA,

        /**
         * U+0259 LATIN SMALL LETTER SCHWA
         */
        LCASE_SCHWA,

        /**
         * U+015D LATIN SMALL LETTER S WITH CIRCUMFLEX
         */
        LCASE_SCIRCUMFLEX,

        /**
         * U+0074 LATIN SMALL LETTER T
         */
        LCASE_T,

        /**
         * U+1E6B LATIN SMALL LETTER T WITH DOT ABOVE
         */
        LCASE_TABOVEDOT,

        /**
         * U+0165 LATIN SMALL LETTER T WITH CARON
         */
        LCASE_TCARON,

        /**
         * U+0163 LATIN SMALL LETTER T WITH CEDILLA
         */
        LCASE_TCEDILLA,

        /**
         * U+00FE LATIN SMALL LETTER THORN
         */
        LCASE_THORN,

        /**
         * U+0167 LATIN SMALL LETTER T WITH STROKE
         */
        LCASE_TSLASH,

        /**
         * U+0075 LATIN SMALL LETTER U
         */
        LCASE_U,

        /**
         * U+00FA LATIN SMALL LETTER U WITH ACUTE
         */
        LCASE_UACUTE,

        /**
         * U+1EE5 LATIN SMALL LETTER U WITH DOT BELOW
         */
        LCASE_UBELOWDOT,

        /**
         * U+016D LATIN SMALL LETTER U WITH BREVE
         */
        LCASE_UBREVE,

        /**
         * U+00FB LATIN SMALL LETTER U WITH CIRCUMFLEX
         */
        LCASE_UCIRCUMFLEX,

        /**
         * U+00FC LATIN SMALL LETTER U WITH DIAERESIS
         */
        LCASE_UDIAERESIS,

        /**
         * U+0171 LATIN SMALL LETTER U WITH DOUBLE ACUTE
         */
        LCASE_UDOUBLEACUTE,

        /**
         * U+00F9 LATIN SMALL LETTER U WITH GRAVE
         */
        LCASE_UGRAVE,

        /**
         * U+1EE7 LATIN SMALL LETTER U WITH HOOK ABOVE
         */
        LCASE_UHOOK,

        /**
         * U+01B0 LATIN SMALL LETTER U WITH HORN
         */
        LCASE_UHORN,

        /**
         * U+1EE9 LATIN SMALL LETTER U WITH HORN AND ACUTE
         */
        LCASE_UHORNACUTE,

        /**
         * U+1EF1 LATIN SMALL LETTER U WITH HORN AND DOT BELOW
         */
        LCASE_UHORNBELOWDOT,

        /**
         * U+1EEB LATIN SMALL LETTER U WITH HORN AND GRAVE
         */
        LCASE_UHORNGRAVE,

        /**
         * U+1EED LATIN SMALL LETTER U WITH HORN AND HOOK ABOVE
         */
        LCASE_UHORNHOOK,

        /**
         * U+1EEF LATIN SMALL LETTER U WITH HORN AND TILDE
         */
        LCASE_UHORNTILDE,

        /**
         * U+016B LATIN SMALL LETTER U WITH MACRON
         */
        LCASE_UMACRON,

        /**
         * U+0173 LATIN SMALL LETTER U WITH OGONEK
         */
        LCASE_UOGONEK,

        /**
         * U+016F LATIN SMALL LETTER U WITH RING ABOVE
         */
        LCASE_URING,

        /**
         * U+0169 LATIN SMALL LETTER U WITH TILDE
         */
        LCASE_UTILDE,

        /**
         * U+0076 LATIN SMALL LETTER V
         */
        LCASE_V,

        /**
         * U+0077 LATIN SMALL LETTER W
         */
        LCASE_W,

        /**
         * U+1E83 LATIN SMALL LETTER W WITH ACUTE
         */
        LCASE_WACUTE,

        /**
         * U+0175 LATIN SMALL LETTER W WITH CIRCUMFLEX
         */
        LCASE_WCIRCUMFLEX,

        /**
         * U+1E85 LATIN SMALL LETTER W WITH DIAERESIS
         */
        LCASE_WDIAERESIS,

        /**
         * U+1E81 LATIN SMALL LETTER W WITH GRAVE
         */
        LCASE_WGRAVE,

        /**
         * U+0078 LATIN SMALL LETTER X
         */
        LCASE_X,

        /**
         * U+1E8B LATIN SMALL LETTER X WITH DOT ABOVE
         */
        LCASE_XABOVEDOT,

        /**
         * U+0079 LATIN SMALL LETTER Y
         */
        LCASE_Y,

        /**
         * U+00FD LATIN SMALL LETTER Y WITH ACUTE
         */
        LCASE_YACUTE,

        /**
         * U+1EF5 LATIN SMALL LETTER Y WITH DOT BELOW
         */
        LCASE_YBELOWDOT,

        /**
         * U+0177 LATIN SMALL LETTER Y WITH CIRCUMFLEX
         */
        LCASE_YCIRCUMFLEX,

        /**
         * U+00FF LATIN SMALL LETTER Y WITH DIAERESIS
         */
        LCASE_YDIAERESIS,

        /**
         * U+1EF3 LATIN SMALL LETTER Y WITH GRAVE
         */
        LCASE_YGRAVE,

        /**
         * U+1EF7 LATIN SMALL LETTER Y WITH HOOK ABOVE
         */
        LCASE_YHOOK,

        /**
         * U+1EF9 LATIN SMALL LETTER Y WITH TILDE
         */
        LCASE_YTILDE,

        /**
         * U+007A LATIN SMALL LETTER Z
         */
        LCASE_Z,

        /**
         * U+017C LATIN SMALL LETTER Z WITH DOT ABOVE
         */
        LCASE_ZABOVEDOT,

        /**
         * U+017A LATIN SMALL LETTER Z WITH ACUTE
         */
        LCASE_ZACUTE,

        /**
         * U+017E LATIN SMALL LETTER Z WITH CARON
         */
        LCASE_ZCARON,

        /**
         * U+01B6 LATIN SMALL LETTER Z WITH STROKE
         */
        LCASE_ZSTROKE,

        /**
         * Move left, left arrow
         */
        LEFT,

        /**
         * U+27E8 MATHEMATICAL LEFT ANGLE BRACKET
         */
        LEFTANGLEBRACKET,

        /**
         * U+2190 LEFTWARDS ARROW
         */
        LEFTARROW,

        /**
         * U+003C LESS-THAN SIGN
         */
        LEFTCARET,

        /**
         * U+201C LEFT DOUBLE QUOTATION MARK
         */
        LEFTDOUBLEQUOTEMARK,

        /**
         * U+23A8 LEFT CURLY BRACKET MIDDLE PIECE
         */
        LEFTMIDDLECURLYBRACE,

        /**
         * U+25C1 WHITE LEFT-POINTING TRIANGLE
         */
        LEFTOPENTRIANGLE,

        /**
         * U+261C WHITE LEFT POINTING INDEX
         */
        LEFTPOINTER,

        /**
         * U+23B7 RADICAL SYMBOL BOTTOM
         */
        LEFTRADICAL,

        /**
         * U+2282 SUBSET OF
         */
        LEFTSHOE,

        /**
         * U+2018 LEFT SINGLE QUOTATION MARK
         */
        LEFTSINGLEQUOTEMARK,

        /**
         * U+251C BOX DRAWINGS LIGHT VERTICAL AND RIGHT
         */
        LEFTT,

        /**
         * U+22A3 LEFT TACK
         */
        LEFTTACK,

        /**
         * U+003C LESS-THAN SIGN
         */
        LESS,

        /**
         * U+2264 LESS-THAN OR EQUAL TO
         */
        LESSTHANEQUAL,

        /**
         * U+240A SYMBOL FOR LINE FEED
         */
        LF,

        /**
         * Linefeed, LF
         */
        LINEFEED,

        /**
         * Lira
         */
        LIRA,

        /**
         * U+20A4 LIRA SIGN
         */
        LIRA_SIGN,

        /**
         * U+2227 LOGICAL AND
         */
        LOGICALAND,

        /**
         * U+2228 LOGICAL OR
         */
        LOGICALOR,

        /**
         * Longminus
         */
        LONGMINUS,

        /**
         * U+2514 BOX DRAWINGS LIGHT UP AND RIGHT
         */
        LOWLEFTCORNER,

        /**
         * U+2518 BOX DRAWINGS LIGHT UP AND LEFT
         */
        LOWRIGHTCORNER,

        /**
         * U+0455 CYRILLIC SMALL LETTER DZE
         */
        MACEDONIA_LCASE_DSE,

        /**
         * U+0453 CYRILLIC SMALL LETTER GJE
         */
        MACEDONIA_LCASE_GJE,

        /**
         * U+045C CYRILLIC SMALL LETTER KJE
         */
        MACEDONIA_LCASE_KJE,

        /**
         * U+0405 CYRILLIC CAPITAL LETTER DZE
         */
        MACEDONIA_UCASE_DSE,

        /**
         * U+0403 CYRILLIC CAPITAL LETTER GJE
         */
        MACEDONIA_UCASE_GJE,

        /**
         * U+040C CYRILLIC CAPITAL LETTER KJE
         */
        MACEDONIA_UCASE_KJE,

        /**
         * U+00AF MACRON
         */
        MACRON,

        /**
         * Previous Candidate
         */
        MAE_KOHO,

        /**
         * U+2642 MALE SIGN
         */
        MALESYMBOL,

        /**
         * U+2720 MALTESE CROSS
         */
        MALTESECROSS,

        /**
         * Marker
         */
        MARKER,

        /**
         * U+00BA MASCULINE ORDINAL INDICATOR
         */
        MASCULINE,

        /**
         * Delete from Dictionary
         */
        MASSYO,

        /**
         * Menu
         */
        MENU,

        /**
         * Left meta
         */
        META_L,

        /**
         * Right meta
         */
        META_R,

        /**
         * U+20A5 MILL SIGN
         */
        MILL_SIGN,

        /**
         * U+002D HYPHEN-MINUS
         */
        MINUS,

        /**
         * U+2032 PRIME
         */
        MINUTES,

        /**
         * Character set switch
         */
        MODE_SWITCH,

        /**
         * Mouse Keys Accel Enable
         */
        MOUSE_KEYS_ACCEL_ENABLE,

        /**
         * Mouse Keys Enable
         */
        MOUSE_KEYS_ENABLE,

        /**
         * U+00B5 MICRO SIGN
         */
        MU,

        /**
         * Cancel Conversion
         */
        MUHENKAN,

        /**
         * Multiple Candidate
         */
        MULTIPLE_CANDIDATE,

        /**
         * U+00D7 MULTIPLICATION SIGN
         */
        MULTIPLY,

        /**
         * Multi-key character compose
         */
        MULTI_KEY,

        /**
         * U+266D MUSIC FLAT SIGN
         */
        MUSICALFLAT,

        /**
         * U+266F MUSIC SHARP SIGN
         */
        MUSICALSHARP,

        /**
         * Mute Acute
         */
        MUTE_ACUTE,

        /**
         * Mute Asciicircum
         */
        MUTE_ASCIICIRCUM,

        /**
         * Mute Asciitilde
         */
        MUTE_ASCIITILDE,

        /**
         * Mute Diaeresis
         */
        MUTE_DIAERESIS,

        /**
         * Mute Grave
         */
        MUTE_GRAVE,

        /**
         * U+2207 NABLA
         */
        NABLA,

        /**
         * U+20A6 NAIRA SIGN
         */
        NAIRA_SIGN,

        /**
         * U+20AA NEW SHEQEL SIGN
         */
        NEW_SHEQEL_SIGN,

        /**
         * Next
         */
        NEXT,

        /**
         * Next Virtual Screen
         */
        NEXT_VIRTUAL_SCREEN,

        /**
         * U+2089 SUBSCRIPT NINE
         */
        NINESUBSCRIPT,

        /**
         * U+2079 SUPERSCRIPT NINE
         */
        NINESUPERIOR,

        /**
         * U+2424 SYMBOL FOR NEWLINE
         */
        NL,

        /**
         * U+00A0 NO-BREAK SPACE
         */
        NOBREAKSPACE,

        /**
         * U+2247 NOT ALMOST EQUAL TO
         */
        NOTAPPROXEQ,

        /**
         * U+2209 NOT AN ELEMENT OF
         */
        NOTELEMENTOF,

        /**
         * U+2260 NOT EQUAL TO
         */
        NOTEQUAL,

        /**
         * U+2262 NOT IDENTICAL TO
         */
        NOTIDENTICAL,

        /**
         * U+00AC NOT SIGN
         */
        NOTSIGN,

        /**
         * Special KeySym
         */
        NO_SYMBOL,

        /**
         * U+0023 NUMBER SIGN
         */
        NUMBERSIGN,

        /**
         * U+2116 NUMERO SIGN
         */
        NUMEROSIGN,

        /**
         * Num Lock
         */
        NUM_LOCK,

        /**
         * U+02DB OGONEK
         */
        OGONEK,

        /**
         * U+215B VULGAR FRACTION ONE EIGHTH
         */
        ONEEIGHTH,

        /**
         * U+2155 VULGAR FRACTION ONE FIFTH
         */
        ONEFIFTH,

        /**
         * U+00BD VULGAR FRACTION ONE HALF
         */
        ONEHALF,

        /**
         * U+00BC VULGAR FRACTION ONE QUARTER
         */
        ONEQUARTER,

        /**
         * U+2159 VULGAR FRACTION ONE SIXTH
         */
        ONESIXTH,

        /**
         * U+2081 SUBSCRIPT ONE
         */
        ONESUBSCRIPT,

        /**
         * U+00B9 SUPERSCRIPT ONE
         */
        ONESUPERIOR,

        /**
         * U+2153 VULGAR FRACTION ONE THIRD
         */
        ONETHIRD,

        /**
         * U+25AD WHITE RECTANGLE
         */
        OPENRECTBULLET,

        /**
         * U+2606 WHITE STAR
         */
        OPENSTAR,

        /**
         * U+25BD WHITE DOWN-POINTING TRIANGLE
         */
        OPENTRIBULLETDOWN,

        /**
         * U+25B3 WHITE UP-POINTING TRIANGLE
         */
        OPENTRIBULLETUP,

        /**
         * U+00AA FEMININE ORDINAL INDICATOR
         */
        ORDFEMININE,

        /**
         * Osf Activate
         */
        OSF_ACTIVATE,

        /**
         * Osf Add Mode
         */
        OSF_ADD_MODE,

        /**
         * Osf Back Space
         */
        OSF_BACK_SPACE,

        /**
         * Osf Back Tab
         */
        OSF_BACK_TAB,

        /**
         * Osf Begin Data
         */
        OSF_BEGIN_DATA,

        /**
         * Osf Begin Line
         */
        OSF_BEGIN_LINE,

        /**
         * Osf Cancel
         */
        OSF_CANCEL,

        /**
         * Osf Clear
         */
        OSF_CLEAR,

        /**
         * Osf Copy
         */
        OSF_COPY,

        /**
         * Osf Cut
         */
        OSF_CUT,

        /**
         * Osf Delete
         */
        OSF_DELETE,

        /**
         * Osf Deselect All
         */
        OSF_DESELECT_ALL,

        /**
         * Osf Down
         */
        OSF_DOWN,

        /**
         * Osf End Data
         */
        OSF_END_DATA,

        /**
         * Osf End Line
         */
        OSF_END_LINE,

        /**
         * Osf Escape
         */
        OSF_ESCAPE,

        /**
         * Osf Extend
         */
        OSF_EXTEND,

        /**
         * Osf Help
         */
        OSF_HELP,

        /**
         * Osf Insert
         */
        OSF_INSERT,

        /**
         * Osf Left
         */
        OSF_LEFT,

        /**
         * Osf Menu
         */
        OSF_MENU,

        /**
         * Osf Menu Bar
         */
        OSF_MENU_BAR,

        /**
         * Osf Next Field
         */
        OSF_NEXT_FIELD,

        /**
         * Osf Next Menu
         */
        OSF_NEXT_MENU,

        /**
         * Osf Page Down
         */
        OSF_PAGE_DOWN,

        /**
         * Osf Page Left
         */
        OSF_PAGE_LEFT,

        /**
         * Osf Page Right
         */
        OSF_PAGE_RIGHT,

        /**
         * Osf Page Up
         */
        OSF_PAGE_UP,

        /**
         * Osf Paste
         */
        OSF_PASTE,

        /**
         * Osf Prev Field
         */
        OSF_PREV_FIELD,

        /**
         * Osf Prev Menu
         */
        OSF_PREV_MENU,

        /**
         * Osf Primary Paste
         */
        OSF_PRIMARY_PASTE,

        /**
         * Osf Quick Paste
         */
        OSF_QUICK_PASTE,

        /**
         * Osf Reselect
         */
        OSF_RESELECT,

        /**
         * Osf Restore
         */
        OSF_RESTORE,

        /**
         * Osf Right
         */
        OSF_RIGHT,

        /**
         * Osf Select
         */
        OSF_SELECT,

        /**
         * Osf Select All
         */
        OSF_SELECT_ALL,

        /**
         * Osf Undo
         */
        OSF_UNDO,

        /**
         * Osf Up
         */
        OSF_UP,

        /**
         * U+00AF MACRON
         */
        OVERBAR,

        /**
         * Overlay1 Enable
         */
        OVERLAY1_ENABLE,

        /**
         * Overlay2 Enable
         */
        OVERLAY2_ENABLE,

        /**
         * U+203E OVERLINE
         */
        OVERLINE,

        /**
         * Page Down
         */
        PAGE_DOWN,

        /**
         * Page Up
         */
        PAGE_UP,

        /**
         * U+00B6 PILCROW SIGN
         */
        PARAGRAPH,

        /**
         * U+0028 LEFT PARENTHESIS
         */
        PARENLEFT,

        /**
         * U+0029 RIGHT PARENTHESIS
         */
        PARENRIGHT,

        /**
         * U+2202 PARTIAL DIFFERENTIAL
         */
        PARTDIFFERENTIAL,

        /**
         * U+2202 PARTIAL DIFFERENTIAL
         */
        PARTIALDERIVATIVE,

        /**
         * Pause, hold
         */
        PAUSE,

        /**
         * U+0025 PERCENT SIGN
         */
        PERCENT,

        /**
         * U+002E FULL STOP
         */
        PERIOD,

        /**
         * U+00B7 MIDDLE DOT
         */
        PERIODCENTERED,

        /**
         * U+2030 PER MILLE SIGN
         */
        PERMILLE,

        /**
         * U+20A7 PESETA SIGN
         */
        PESETA_SIGN,

        /**
         * U+2117 SOUND RECORDING COPYRIGHT
         */
        PHONOGRAPHCOPYRIGHT,

        /**
         * U+002B PLUS SIGN
         */
        PLUS,

        /**
         * U+00B1 PLUS-MINUS SIGN
         */
        PLUSMINUS,

        /**
         * Pointer Accelerate
         */
        POINTER_ACCELERATE,

        /**
         * Pointer Button1
         */
        POINTER_BUTTON1,

        /**
         * Pointer Button2
         */
        POINTER_BUTTON2,

        /**
         * Pointer Button3
         */
        POINTER_BUTTON3,

        /**
         * Pointer Button4
         */
        POINTER_BUTTON4,

        /**
         * Pointer Button5
         */
        POINTER_BUTTON5,

        /**
         * Pointer Button Dflt
         */
        POINTER_BUTTON_DFLT,

        /**
         * Pointer Dbl Click1
         */
        POINTER_DBL_CLICK1,

        /**
         * Pointer Dbl Click2
         */
        POINTER_DBL_CLICK2,

        /**
         * Pointer Dbl Click3
         */
        POINTER_DBL_CLICK3,

        /**
         * Pointer Dbl Click4
         */
        POINTER_DBL_CLICK4,

        /**
         * Pointer Dbl Click5
         */
        POINTER_DBL_CLICK5,

        /**
         * Pointer Dbl Click Dflt
         */
        POINTER_DBL_CLICK_DFLT,

        /**
         * Pointer Dflt Btn Next
         */
        POINTER_DFLT_BTN_NEXT,

        /**
         * Pointer Dflt Btn Prev
         */
        POINTER_DFLT_BTN_PREV,

        /**
         * Pointer Down
         */
        POINTER_DOWN,

        /**
         * Pointer Down Left
         */
        POINTER_DOWN_LEFT,

        /**
         * Pointer Down Right
         */
        POINTER_DOWN_RIGHT,

        /**
         * Pointer Drag1
         */
        POINTER_DRAG1,

        /**
         * Pointer Drag2
         */
        POINTER_DRAG2,

        /**
         * Pointer Drag3
         */
        POINTER_DRAG3,

        /**
         * Pointer Drag4
         */
        POINTER_DRAG4,

        /**
         * Pointer Drag5
         */
        POINTER_DRAG5,

        /**
         * Pointer Drag Dflt
         */
        POINTER_DRAG_DFLT,

        /**
         * Pointer Enable Keys
         */
        POINTER_ENABLE_KEYS,

        /**
         * Pointer Left
         */
        POINTER_LEFT,

        /**
         * Pointer Right
         */
        POINTER_RIGHT,

        /**
         * Pointer Up
         */
        POINTER_UP,

        /**
         * Pointer Up Left
         */
        POINTER_UP_LEFT,

        /**
         * Pointer Up Right
         */
        POINTER_UP_RIGHT,

        /**
         * U+211E PRESCRIPTION TAKE
         */
        PRESCRIPTION,

        /**
         * Previous Candidate
         */
        PREVIOUS_CANDIDATE,

        /**
         * Prev Virtual Screen
         */
        PREV_VIRTUAL_SCREEN,

        /**
         * Print
         */
        PRINT,

        /**
         * Prior, previous
         */
        PRIOR,

        /**
         * U+30FC KATAKANA-HIRAGANA PROLONGED SOUND MARK
         */
        PROLONGEDSOUND,

        /**
         * U+2008 PUNCTUATION SPACE
         */
        PUNCTSPACE,

        /**
         * U+2395 APL FUNCTIONAL SYMBOL QUAD
         */
        QUAD,

        /**
         * U+003F QUESTION MARK
         */
        QUESTION,

        /**
         * U+00BF INVERTED QUESTION MARK
         */
        QUESTIONDOWN,

        /**
         * U+0022 QUOTATION MARK
         */
        QUOTEDBL,

        /**
         * deprecated
         */
        QUOTELEFT,

        /**
         * deprecated
         */
        QUOTERIGHT,

        /**
         * R1
         */
        R1,

        /**
         * R10
         */
        R10,

        /**
         * R11
         */
        R11,

        /**
         * R12
         */
        R12,

        /**
         * R13
         */
        R13,

        /**
         * R14
         */
        R14,

        /**
         * R15
         */
        R15,

        /**
         * R2
         */
        R2,

        /**
         * R3
         */
        R3,

        /**
         * R4
         */
        R4,

        /**
         * R5
         */
        R5,

        /**
         * R6
         */
        R6,

        /**
         * R7
         */
        R7,

        /**
         * R8
         */
        R8,

        /**
         * R9
         */
        R9,

        /**
         * U+221A SQUARE ROOT
         */
        RADICAL,

        /**
         * Redo, again
         */
        REDO,

        /**
         * U+00AE REGISTERED SIGN
         */
        REGISTERED,

        /**
         * Repeat Keys Enable
         */
        REPEAT_KEYS_ENABLE,

        /**
         * Reset
         */
        RESET,

        /**
         * Return, enter
         */
        RETURN,

        /**
         * Move right, right arrow
         */
        RIGHT,

        /**
         * U+27E9 MATHEMATICAL RIGHT ANGLE BRACKET
         */
        RIGHTANGLEBRACKET,

        /**
         * U+2192 RIGHTWARDS ARROW
         */
        RIGHTARROW,

        /**
         * U+003E GREATER-THAN SIGN
         */
        RIGHTCARET,

        /**
         * U+201D RIGHT DOUBLE QUOTATION MARK
         */
        RIGHTDOUBLEQUOTEMARK,

        /**
         * U+23AC RIGHT CURLY BRACKET MIDDLE PIECE
         */
        RIGHTMIDDLECURLYBRACE,

        /**
         * Rightmiddlesummation
         */
        RIGHTMIDDLESUMMATION,

        /**
         * U+25B7 WHITE RIGHT-POINTING TRIANGLE
         */
        RIGHTOPENTRIANGLE,

        /**
         * U+261E WHITE RIGHT POINTING INDEX
         */
        RIGHTPOINTER,

        /**
         * U+2283 SUPERSET OF
         */
        RIGHTSHOE,

        /**
         * U+2019 RIGHT SINGLE QUOTATION MARK
         */
        RIGHTSINGLEQUOTEMARK,

        /**
         * U+2524 BOX DRAWINGS LIGHT VERTICAL AND LEFT
         */
        RIGHTT,

        /**
         * U+22A2 RIGHT TACK
         */
        RIGHTTACK,

        /**
         * to Romaji
         */
        ROMAJI,

        /**
         * U+20A8 RUPEE SIGN
         */
        RUPEE_SIGN,

        /**
         * Alias for mode_switch
         */
        SCRIPT_SWITCH,

        /**
         * Scroll Lock
         */
        SCROLL_LOCK,

        /**
         * U+2033 DOUBLE PRIME
         */
        SECONDS,

        /**
         * U+00A7 SECTION SIGN
         */
        SECTION,

        /**
         * Select, mark
         */
        SELECT,

        /**
         * U+003B SEMICOLON
         */
        SEMICOLON,

        /**
         * U+309C KATAKANA-HIRAGANA SEMI-VOICED SOUND MARK
         */
        SEMIVOICEDSOUND,

        /**
         * U+0452 CYRILLIC SMALL LETTER DJE
         */
        SERBIAN_LCASE_DJE,

        /**
         * deprecated
         */
        SERBIAN_LCASE_DZE,

        /**
         * deprecated
         */
        SERBIAN_LCASE_JE,

        /**
         * deprecated
         */
        SERBIAN_LCASE_LJE,

        /**
         * deprecated
         */
        SERBIAN_LCASE_NJE,

        /**
         * U+045B CYRILLIC SMALL LETTER TSHE
         */
        SERBIAN_LCASE_TSHE,

        /**
         * U+0402 CYRILLIC CAPITAL LETTER DJE
         */
        SERBIAN_UCASE_DJE,

        /**
         * deprecated
         */
        SERBIAN_UCASE_DZE,

        /**
         * deprecated
         */
        SERBIAN_UCASE_JE,

        /**
         * deprecated
         */
        SERBIAN_UCASE_LJE,

        /**
         * deprecated
         */
        SERBIAN_UCASE_NJE,

        /**
         * U+040B CYRILLIC CAPITAL LETTER TSHE
         */
        SERBIAN_UCASE_TSHE,

        /**
         * U+215E VULGAR FRACTION SEVEN EIGHTHS
         */
        SEVENEIGHTHS,

        /**
         * U+2087 SUBSCRIPT SEVEN
         */
        SEVENSUBSCRIPT,

        /**
         * U+2077 SUPERSCRIPT SEVEN
         */
        SEVENSUPERIOR,

        /**
         * Left shift
         */
        SHIFT_L,

        /**
         * Shift lock
         */
        SHIFT_LOCK,

        /**
         * Right shift
         */
        SHIFT_R,

        /**
         * U+2613 SALTIRE
         */
        SIGNATUREMARK,

        /**
         * U+2423 OPEN BOX
         */
        SIGNIFBLANK,

        /**
         * U+2243 ASYMPTOTICALLY EQUAL TO
         */
        SIMILAREQUAL,

        /**
         * U+201A SINGLE LOW-9 QUOTATION MARK
         */
        SINGLELOWQUOTEMARK,

        /**
         * Single Candidate
         */
        SINGLE_CANDIDATE,

        /**
         * U+0D85 SINHALA AYANNA
         */
        SINH_A,

        /**
         * U+0D86 SINHALA AAYANNA
         */
        SINH_AA,

        /**
         * U+0DCF SINHALA AELA-PILLA
         */
        SINH_AA2,

        /**
         * U+0D87 SINHALA AEYANNA
         */
        SINH_AE,

        /**
         * U+0DD0 SINHALA AEDA-PILLA
         */
        SINH_AE2,

        /**
         * U+0D88 SINHALA AEEYANNA
         */
        SINH_AEE,

        /**
         * U+0DD1 SINHALA DIGA AEDA-PILLA
         */
        SINH_AEE2,

        /**
         * U+0D93 SINHALA AIYANNA
         */
        SINH_AI,

        /**
         * U+0DDB SINHALA KOMBU DEKA
         */
        SINH_AI2,

        /**
         * U+0DCA SINHALA AL-LAKUNA
         */
        SINH_AL,

        /**
         * U+0D96 SINHALA AUYANNA
         */
        SINH_AU,

        /**
         * U+0DDE SINHALA KOMBUVA HAA GAYANUKITTA
         */
        SINH_AU2,

        /**
         * U+0DB6 SINHALA BAYANNA
         */
        SINH_BA,

        /**
         * U+0DB7 SINHALA MAHA. BAYANNA
         */
        SINH_BHA,

        /**
         * U+0DA0 SINHALA CAYANNA
         */
        SINH_CA,

        /**
         * U+0DA1 SINHALA MAHA. CAYANNA
         */
        SINH_CHA,

        /**
         * U+0DA9 SINHALA DDAYANNA
         */
        SINH_DDA,

        /**
         * U+0DAA SINHALA MAHA. DDAYANNA
         */
        SINH_DDHA,

        /**
         * U+0DAF SINHALA DAYANNA
         */
        SINH_DHA,

        /**
         * U+0DB0 SINHALA MAHA. DAYANNA
         */
        SINH_DHHA,

        /**
         * U+0D91 SINHALA EYANNA
         */
        SINH_E,

        /**
         * U+0DD9 SINHALA KOMBUVA
         */
        SINH_E2,

        /**
         * U+0D92 SINHALA EEYANNA
         */
        SINH_EE,

        /**
         * U+0DDA SINHALA DIGA KOMBUVA
         */
        SINH_EE2,

        /**
         * U+0DC6 SINHALA FAYANNA
         */
        SINH_FA,

        /**
         * U+0D9C SINHALA GAYANNA
         */
        SINH_GA,

        /**
         * U+0D9D SINHALA MAHA. GAYANNA
         */
        SINH_GHA,

        /**
         * U+0D83 SINHALA VISARGAYA
         */
        SINH_H2,

        /**
         * U+0DC4 SINHALA HAYANNA
         */
        SINH_HA,

        /**
         * U+0D89 SINHALA IYANNA
         */
        SINH_I,

        /**
         * U+0DD2 SINHALA IS-PILLA
         */
        SINH_I2,

        /**
         * U+0D8A SINHALA IIYANNA
         */
        SINH_II,

        /**
         * U+0DD3 SINHALA DIGA IS-PILLA
         */
        SINH_II2,

        /**
         * U+0DA2 SINHALA JAYANNA
         */
        SINH_JA,

        /**
         * U+0DA3 SINHALA MAHA. JAYANNA
         */
        SINH_JHA,

        /**
         * U+0DA5 SINHALA TAALUJA SANYOOGA NAASIKYAYA
         */
        SINH_JNYA,

        /**
         * U+0D9A SINHALA KAYANNA
         */
        SINH_KA,

        /**
         * U+0D9B SINHALA MAHA. KAYANNA
         */
        SINH_KHA,

        /**
         * U+0DF4 SINHALA KUNDDALIYA
         */
        SINH_KUNDDALIYA,

        /**
         * U+0DBD SINHALA DANTAJA LAYANNA
         */
        SINH_LA,

        /**
         * U+0DC5 SINHALA MUURDHAJA LAYANNA
         */
        SINH_LLA,

        /**
         * U+0D8F SINHALA ILUYANNA
         */
        SINH_LU,

        /**
         * U+0DDF SINHALA GAYANUKITTA
         */
        SINH_LU2,

        /**
         * U+0D90 SINHALA ILUUYANNA
         */
        SINH_LUU,

        /**
         * U+0DF3 SINHALA DIGA GAYANUKITTA
         */
        SINH_LUU2,

        /**
         * U+0DB8 SINHALA MAYANNA
         */
        SINH_MA,

        /**
         * U+0DB9 SINHALA AMBA BAYANNA
         */
        SINH_MBA,

        /**
         * U+0DB1 SINHALA DANTAJA NAYANNA
         */
        SINH_NA,

        /**
         * U+0DAC SINHALA SANYAKA DDAYANNA
         */
        SINH_NDDA,

        /**
         * U+0DB3 SINHALA SANYAKA DAYANNA
         */
        SINH_NDHA,

        /**
         * U+0D82 SINHALA ANUSVARAYA
         */
        SINH_NG,

        /**
         * U+0D9E SINHALA KANTAJA NAASIKYAYA
         */
        SINH_NG2,

        /**
         * U+0D9F SINHALA SANYAKA GAYANNA
         */
        SINH_NGA,

        /**
         * U+0DA6 SINHALA SANYAKA JAYANNA
         */
        SINH_NJA,

        /**
         * U+0DAB SINHALA MUURDHAJA NAYANNA
         */
        SINH_NNA,

        /**
         * U+0DA4 SINHALA TAALUJA NAASIKYAYA
         */
        SINH_NYA,

        /**
         * U+0D94 SINHALA OYANNA
         */
        SINH_O,

        /**
         * U+0DDC SINHALA KOMBUVA HAA AELA-PILLA
         */
        SINH_O2,

        /**
         * U+0D95 SINHALA OOYANNA
         */
        SINH_OO,

        /**
         * U+0DDD SINHALA KOMBUVA HAA DIGA AELA-PILLA
         */
        SINH_OO2,

        /**
         * U+0DB4 SINHALA PAYANNA
         */
        SINH_PA,

        /**
         * U+0DB5 SINHALA MAHA. PAYANNA
         */
        SINH_PHA,

        /**
         * U+0DBB SINHALA RAYANNA
         */
        SINH_RA,

        /**
         * U+0D8D SINHALA IRUYANNA
         */
        SINH_RI,

        /**
         * U+0D8E SINHALA IRUUYANNA
         */
        SINH_RII,

        /**
         * U+0DD8 SINHALA GAETTA-PILLA
         */
        SINH_RU2,

        /**
         * U+0DF2 SINHALA DIGA GAETTA-PILLA
         */
        SINH_RUU2,

        /**
         * U+0DC3 SINHALA DANTAJA SAYANNA
         */
        SINH_SA,

        /**
         * U+0DC1 SINHALA TAALUJA SAYANNA
         */
        SINH_SHA,

        /**
         * U+0DC2 SINHALA MUURDHAJA SAYANNA
         */
        SINH_SSHA,

        /**
         * U+0DAD SINHALA TAYANNA
         */
        SINH_THA,

        /**
         * U+0DAE SINHALA MAHA. TAYANNA
         */
        SINH_THHA,

        /**
         * U+0DA7 SINHALA TTAYANNA
         */
        SINH_TTA,

        /**
         * U+0DA8 SINHALA MAHA. TTAYANNA
         */
        SINH_TTHA,

        /**
         * U+0D8B SINHALA UYANNA
         */
        SINH_U,

        /**
         * U+0DD4 SINHALA PAA-PILLA
         */
        SINH_U2,

        /**
         * U+0D8C SINHALA UUYANNA
         */
        SINH_UU,

        /**
         * U+0DD6 SINHALA DIGA PAA-PILLA
         */
        SINH_UU2,

        /**
         * U+0DC0 SINHALA VAYANNA
         */
        SINH_VA,

        /**
         * U+0DBA SINHALA YAYANNA
         */
        SINH_YA,

        /**
         * U+2086 SUBSCRIPT SIX
         */
        SIXSUBSCRIPT,

        /**
         * U+2076 SUPERSCRIPT SIX
         */
        SIXSUPERIOR,

        /**
         * U+002F SOLIDUS
         */
        SLASH,

        /**
         * Slow Keys Enable
         */
        SLOW_KEYS_ENABLE,

        /**
         * U+25C6 BLACK DIAMOND
         */
        SOLIDDIAMOND,

        /**
         * U+0020 SPACE
         */
        SPACE,

        /**
         * U+221A SQUARE ROOT
         */
        SQUAREROOT,

        /**
         * U+00DF LATIN SMALL LETTER SHARP S
         */
        SSHARP,

        /**
         * U+00A3 POUND SIGN
         */
        STERLING,

        /**
         * Sticky Keys Enable
         */
        STICKY_KEYS_ENABLE,

        /**
         * U+2263 STRICTLY EQUIVALENT TO
         */
        STRICTEQ,

        /**
         * Same as XK_Redo
         */
        SUN_AGAIN,

        /**
         * Same as XK_Mode_switch
         */
        SUN_ALT_GRAPH,

        /**
         * Sun Audio Lower Volume
         */
        SUN_AUDIO_LOWER_VOLUME,

        /**
         * Sun Audio Mute
         */
        SUN_AUDIO_MUTE,

        /**
         * Sun Audio Raise Volume
         */
        SUN_AUDIO_RAISE_VOLUME,

        /**
         * Same as XK_Multi_key
         */
        SUN_COMPOSE,

        /**
         * Sun Copy
         */
        SUN_COPY,

        /**
         * Sun Cut
         */
        SUN_CUT,

        /**
         * Labeled F11
         */
        SUN_F36,

        /**
         * Labeled F12
         */
        SUN_F37,

        /**
         * Sun Fa Acute
         */
        SUN_FA_ACUTE,

        /**
         * Sun Fa Cedilla
         */
        SUN_FA_CEDILLA,

        /**
         * Sun Fa Circum
         */
        SUN_FA_CIRCUM,

        /**
         * Sun Fa Diaeresis
         */
        SUN_FA_DIAERESIS,

        /**
         * Sun Fa Grave
         */
        SUN_FA_GRAVE,

        /**
         * Sun Fa Tilde
         */
        SUN_FA_TILDE,

        /**
         * Same as XK_Find
         */
        SUN_FIND,

        /**
         * Sun Front
         */
        SUN_FRONT,

        /**
         * Sun Open
         */
        SUN_OPEN,

        /**
         * Same as XK_Next
         */
        SUN_PAGE_DOWN,

        /**
         * Same as XK_Prior
         */
        SUN_PAGE_UP,

        /**
         * Sun Paste
         */
        SUN_PASTE,

        /**
         * Sun Power Switch
         */
        SUN_POWER_SWITCH,

        /**
         * Sun Power Switch Shift
         */
        SUN_POWER_SWITCH_SHIFT,

        /**
         * Same as XK_Print
         */
        SUN_PRINT_SCREEN,

        /**
         * Sun Props
         */
        SUN_PROPS,

        /**
         * Same as XK_Cancel
         */
        SUN_STOP,

        /**
         * Sun Sys Req
         */
        SUN_SYS_REQ,

        /**
         * Same as XK_Undo
         */
        SUN_UNDO,

        /**
         * Sun Video Degauss
         */
        SUN_VIDEO_DEGAUSS,

        /**
         * Sun Video Lower Brightness
         */
        SUN_VIDEO_LOWER_BRIGHTNESS,

        /**
         * Sun Video Raise Brightness
         */
        SUN_VIDEO_RAISE_BRIGHTNESS,

        /**
         * Left super
         */
        SUPER_L,

        /**
         * Right super
         */
        SUPER_R,

        /**
         * System
         */
        SYSTEM,

        /**
         * Sys Req
         */
        SYS_REQ,

        /**
         * T3270 Alt Cursor
         */
        T3270_ALT_CURSOR,

        /**
         * T3270 Attn
         */
        T3270_ATTN,

        /**
         * T3270 Back Tab
         */
        T3270_BACK_TAB,

        /**
         * T3270 Change Screen
         */
        T3270_CHANGE_SCREEN,

        /**
         * T3270 Copy
         */
        T3270_COPY,

        /**
         * T3270 Cursor Blink
         */
        T3270_CURSOR_BLINK,

        /**
         * T3270 Cursor Select
         */
        T3270_CURSOR_SELECT,

        /**
         * T3270 Delete Word
         */
        T3270_DELETE_WORD,

        /**
         * T3270 Duplicate
         */
        T3270_DUPLICATE,

        /**
         * T3270 Enter
         */
        T3270_ENTER,

        /**
         * T3270 Erase Eof
         */
        T3270_ERASE_EOF,

        /**
         * T3270 Erase Input
         */
        T3270_ERASE_INPUT,

        /**
         * T3270 Ex Select
         */
        T3270_EX_SELECT,

        /**
         * T3270 Field Mark
         */
        T3270_FIELD_MARK,

        /**
         * T3270 Ident
         */
        T3270_IDENT,

        /**
         * T3270 Jump
         */
        T3270_JUMP,

        /**
         * T3270 Key Click
         */
        T3270_KEY_CLICK,

        /**
         * T3270 Left2
         */
        T3270_LEFT2,

        /**
         * T3270 Pa1
         */
        T3270_PA1,

        /**
         * T3270 Pa2
         */
        T3270_PA2,

        /**
         * T3270 Pa3
         */
        T3270_PA3,

        /**
         * T3270 Play
         */
        T3270_PLAY,

        /**
         * T3270 Print Screen
         */
        T3270_PRINT_SCREEN,

        /**
         * T3270 Quit
         */
        T3270_QUIT,

        /**
         * T3270 Record
         */
        T3270_RECORD,

        /**
         * T3270 Reset
         */
        T3270_RESET,

        /**
         * T3270 Right2
         */
        T3270_RIGHT2,

        /**
         * T3270 Rule
         */
        T3270_RULE,

        /**
         * T3270 Setup
         */
        T3270_SETUP,

        /**
         * T3270 Test
         */
        T3270_TEST,

        /**
         * Tab
         */
        TAB,

        /**
         * Tcase Ch
         */
        TCASE_CH,

        /**
         * Tcase C H
         */
        TCASE_C_H,

        /**
         * deprecated
         */
        TCASE_ETH,

        /**
         * deprecated
         */
        TCASE_THORN,

        /**
         * U+260E BLACK TELEPHONE
         */
        TELEPHONE,

        /**
         * U+2315 TELEPHONE RECORDER
         */
        TELEPHONERECORDER,

        /**
         * Terminate Server
         */
        TERMINATE_SERVER,

        /**
         * U+0E3F THAI CURRENCY SYMBOL BAHT
         */
        THAI_BAHT,

        /**
         * U+0E1A THAI CHARACTER BO BAIMAI
         */
        THAI_BOBAIMAI,

        /**
         * U+0E08 THAI CHARACTER CHO CHAN
         */
        THAI_CHOCHAN,

        /**
         * U+0E0A THAI CHARACTER CHO CHANG
         */
        THAI_CHOCHANG,

        /**
         * U+0E09 THAI CHARACTER CHO CHING
         */
        THAI_CHOCHING,

        /**
         * U+0E0C THAI CHARACTER CHO CHOE
         */
        THAI_CHOCHOE,

        /**
         * U+0E0E THAI CHARACTER DO CHADA
         */
        THAI_DOCHADA,

        /**
         * U+0E14 THAI CHARACTER DO DEK
         */
        THAI_DODEK,

        /**
         * U+0E1D THAI CHARACTER FO FA
         */
        THAI_FOFA,

        /**
         * U+0E1F THAI CHARACTER FO FAN
         */
        THAI_FOFAN,

        /**
         * U+0E2B THAI CHARACTER HO HIP
         */
        THAI_HOHIP,

        /**
         * U+0E2E THAI CHARACTER HO NOKHUK
         */
        THAI_HONOKHUK,

        /**
         * U+0E02 THAI CHARACTER KHO KHAI
         */
        THAI_KHOKHAI,

        /**
         * U+0E05 THAI CHARACTER KHO KHON
         */
        THAI_KHOKHON,

        /**
         * U+0E03 THAI CHARACTER KHO KHUAT
         */
        THAI_KHOKHUAT,

        /**
         * U+0E04 THAI CHARACTER KHO KHWAI
         */
        THAI_KHOKHWAI,

        /**
         * U+0E06 THAI CHARACTER KHO RAKHANG
         */
        THAI_KHORAKHANG,

        /**
         * U+0E01 THAI CHARACTER KO KAI
         */
        THAI_KOKAI,

        /**
         * U+0E45 THAI CHARACTER LAKKHANGYAO
         */
        THAI_LAKKHANGYAO,

        /**
         * U+0E57 THAI DIGIT SEVEN
         */
        THAI_LEKCHET,

        /**
         * U+0E55 THAI DIGIT FIVE
         */
        THAI_LEKHA,

        /**
         * U+0E56 THAI DIGIT SIX
         */
        THAI_LEKHOK,

        /**
         * U+0E59 THAI DIGIT NINE
         */
        THAI_LEKKAO,

        /**
         * U+0E51 THAI DIGIT ONE
         */
        THAI_LEKNUNG,

        /**
         * U+0E58 THAI DIGIT EIGHT
         */
        THAI_LEKPAET,

        /**
         * U+0E53 THAI DIGIT THREE
         */
        THAI_LEKSAM,

        /**
         * U+0E54 THAI DIGIT FOUR
         */
        THAI_LEKSI,

        /**
         * U+0E52 THAI DIGIT TWO
         */
        THAI_LEKSONG,

        /**
         * U+0E50 THAI DIGIT ZERO
         */
        THAI_LEKSUN,

        /**
         * U+0E2C THAI CHARACTER LO CHULA
         */
        THAI_LOCHULA,

        /**
         * U+0E25 THAI CHARACTER LO LING
         */
        THAI_LOLING,

        /**
         * U+0E26 THAI CHARACTER LU
         */
        THAI_LU,

        /**
         * U+0E4B THAI CHARACTER MAI CHATTAWA
         */
        THAI_MAICHATTAWA,

        /**
         * U+0E48 THAI CHARACTER MAI EK
         */
        THAI_MAIEK,

        /**
         * U+0E31 THAI CHARACTER MAI HAN-AKAT
         */
        THAI_MAIHANAKAT,

        /**
         * Thai Maihanakat Maitho
         */
        THAI_MAIHANAKAT_MAITHO,

        /**
         * U+0E47 THAI CHARACTER MAITAIKHU
         */
        THAI_MAITAIKHU,

        /**
         * U+0E49 THAI CHARACTER MAI THO
         */
        THAI_MAITHO,

        /**
         * U+0E4A THAI CHARACTER MAI TRI
         */
        THAI_MAITRI,

        /**
         * U+0E46 THAI CHARACTER MAIYAMOK
         */
        THAI_MAIYAMOK,

        /**
         * U+0E21 THAI CHARACTER MO MA
         */
        THAI_MOMA,

        /**
         * U+0E07 THAI CHARACTER NGO NGU
         */
        THAI_NGONGU,

        /**
         * U+0E4D THAI CHARACTER NIKHAHIT
         */
        THAI_NIKHAHIT,

        /**
         * U+0E13 THAI CHARACTER NO NEN
         */
        THAI_NONEN,

        /**
         * U+0E19 THAI CHARACTER NO NU
         */
        THAI_NONU,

        /**
         * U+0E2D THAI CHARACTER O ANG
         */
        THAI_OANG,

        /**
         * U+0E2F THAI CHARACTER PAIYANNOI
         */
        THAI_PAIYANNOI,

        /**
         * U+0E3A THAI CHARACTER PHINTHU
         */
        THAI_PHINTHU,

        /**
         * U+0E1E THAI CHARACTER PHO PHAN
         */
        THAI_PHOPHAN,

        /**
         * U+0E1C THAI CHARACTER PHO PHUNG
         */
        THAI_PHOPHUNG,

        /**
         * U+0E20 THAI CHARACTER PHO SAMPHAO
         */
        THAI_PHOSAMPHAO,

        /**
         * U+0E1B THAI CHARACTER PO PLA
         */
        THAI_POPLA,

        /**
         * U+0E23 THAI CHARACTER RO RUA
         */
        THAI_RORUA,

        /**
         * U+0E24 THAI CHARACTER RU
         */
        THAI_RU,

        /**
         * U+0E30 THAI CHARACTER SARA A
         */
        THAI_SARAA,

        /**
         * U+0E32 THAI CHARACTER SARA AA
         */
        THAI_SARAAA,

        /**
         * U+0E41 THAI CHARACTER SARA AE
         */
        THAI_SARAAE,

        /**
         * U+0E44 THAI CHARACTER SARA AI MAIMALAI
         */
        THAI_SARAAIMAIMALAI,

        /**
         * U+0E43 THAI CHARACTER SARA AI MAIMUAN
         */
        THAI_SARAAIMAIMUAN,

        /**
         * U+0E33 THAI CHARACTER SARA AM
         */
        THAI_SARAAM,

        /**
         * U+0E40 THAI CHARACTER SARA E
         */
        THAI_SARAE,

        /**
         * U+0E34 THAI CHARACTER SARA I
         */
        THAI_SARAI,

        /**
         * U+0E35 THAI CHARACTER SARA II
         */
        THAI_SARAII,

        /**
         * U+0E42 THAI CHARACTER SARA O
         */
        THAI_SARAO,

        /**
         * U+0E38 THAI CHARACTER SARA U
         */
        THAI_SARAU,

        /**
         * U+0E36 THAI CHARACTER SARA UE
         */
        THAI_SARAUE,

        /**
         * U+0E37 THAI CHARACTER SARA UEE
         */
        THAI_SARAUEE,

        /**
         * U+0E39 THAI CHARACTER SARA UU
         */
        THAI_SARAUU,

        /**
         * U+0E29 THAI CHARACTER SO RUSI
         */
        THAI_SORUSI,

        /**
         * U+0E28 THAI CHARACTER SO SALA
         */
        THAI_SOSALA,

        /**
         * U+0E0B THAI CHARACTER SO SO
         */
        THAI_SOSO,

        /**
         * U+0E2A THAI CHARACTER SO SUA
         */
        THAI_SOSUA,

        /**
         * U+0E4C THAI CHARACTER THANTHAKHAT
         */
        THAI_THANTHAKHAT,

        /**
         * U+0E11 THAI CHARACTER THO NANGMONTHO
         */
        THAI_THONANGMONTHO,

        /**
         * U+0E12 THAI CHARACTER THO PHUTHAO
         */
        THAI_THOPHUTHAO,

        /**
         * U+0E17 THAI CHARACTER THO THAHAN
         */
        THAI_THOTHAHAN,

        /**
         * U+0E10 THAI CHARACTER THO THAN
         */
        THAI_THOTHAN,

        /**
         * U+0E18 THAI CHARACTER THO THONG
         */
        THAI_THOTHONG,

        /**
         * U+0E16 THAI CHARACTER THO THUNG
         */
        THAI_THOTHUNG,

        /**
         * U+0E0F THAI CHARACTER TO PATAK
         */
        THAI_TOPATAK,

        /**
         * U+0E15 THAI CHARACTER TO TAO
         */
        THAI_TOTAO,

        /**
         * U+0E27 THAI CHARACTER WO WAEN
         */
        THAI_WOWAEN,

        /**
         * U+0E22 THAI CHARACTER YO YAK
         */
        THAI_YOYAK,

        /**
         * U+0E0D THAI CHARACTER YO YING
         */
        THAI_YOYING,

        /**
         * U+2234 THEREFORE
         */
        THEREFORE,

        /**
         * U+2009 THIN SPACE
         */
        THINSPACE,

        /**
         * U+215C VULGAR FRACTION THREE EIGHTHS
         */
        THREEEIGHTHS,

        /**
         * U+2157 VULGAR FRACTION THREE FIFTHS
         */
        THREEFIFTHS,

        /**
         * U+00BE VULGAR FRACTION THREE QUARTERS
         */
        THREEQUARTERS,

        /**
         * U+2083 SUBSCRIPT THREE
         */
        THREESUBSCRIPT,

        /**
         * U+00B3 SUPERSCRIPT THREE
         */
        THREESUPERIOR,

        /**
         * U+222D TRIPLE INTEGRAL
         */
        TINTEGRAL,

        /**
         * U+2320 TOP HALF INTEGRAL
         */
        TOPINTEGRAL,

        /**
         * U+239B LEFT PARENTHESIS UPPER HOOK
         */
        TOPLEFTPARENS,

        /**
         * U+250C BOX DRAWINGS LIGHT DOWN AND RIGHT
         */
        TOPLEFTRADICAL,

        /**
         * U+23A1 LEFT SQUARE BRACKET UPPER CORNER
         */
        TOPLEFTSQBRACKET,

        /**
         * Topleftsummation
         */
        TOPLEFTSUMMATION,

        /**
         * U+239E RIGHT PARENTHESIS UPPER HOOK
         */
        TOPRIGHTPARENS,

        /**
         * U+23A4 RIGHT SQUARE BRACKET UPPER CORNER
         */
        TOPRIGHTSQBRACKET,

        /**
         * Toprightsummation
         */
        TOPRIGHTSUMMATION,

        /**
         * U+252C BOX DRAWINGS LIGHT DOWN AND HORIZONTAL
         */
        TOPT,

        /**
         * Topvertsummationconnector
         */
        TOPVERTSUMMATIONCONNECTOR,

        /**
         * Add to Dictionary
         */
        TOUROKU,

        /**
         * U+2122 TRADE MARK SIGN
         */
        TRADEMARK,

        /**
         * Trademarkincircle
         */
        TRADEMARKINCIRCLE,

        /**
         * U+2156 VULGAR FRACTION TWO FIFTHS
         */
        TWOFIFTHS,

        /**
         * U+2082 SUBSCRIPT TWO
         */
        TWOSUBSCRIPT,

        /**
         * U+00B2 SUPERSCRIPT TWO
         */
        TWOSUPERIOR,

        /**
         * U+2154 VULGAR FRACTION TWO THIRDS
         */
        TWOTHIRDS,

        /**
         * U+0041 LATIN CAPITAL LETTER A
         */
        UCASE_A,

        /**
         * U+00C1 LATIN CAPITAL LETTER A WITH ACUTE
         */
        UCASE_AACUTE,

        /**
         * U+1EA0 LATIN CAPITAL LETTER A WITH DOT BELOW
         */
        UCASE_ABELOWDOT,

        /**
         * U+0102 LATIN CAPITAL LETTER A WITH BREVE
         */
        UCASE_ABREVE,

        /**
         * U+1EAE LATIN CAPITAL LETTER A WITH BREVE AND ACUTE
         */
        UCASE_ABREVEACUTE,

        /**
         * U+1EB6 LATIN CAPITAL LETTER A WITH BREVE AND DOT BELOW
         */
        UCASE_ABREVEBELOWDOT,

        /**
         * U+1EB0 LATIN CAPITAL LETTER A WITH BREVE AND GRAVE
         */
        UCASE_ABREVEGRAVE,

        /**
         * U+1EB2 LATIN CAPITAL LETTER A WITH BREVE AND HOOK ABOVE
         */
        UCASE_ABREVEHOOK,

        /**
         * U+1EB4 LATIN CAPITAL LETTER A WITH BREVE AND TILDE
         */
        UCASE_ABREVETILDE,

        /**
         * U+00C2 LATIN CAPITAL LETTER A WITH CIRCUMFLEX
         */
        UCASE_ACIRCUMFLEX,

        /**
         * U+1EA4 LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND ACUTE
         */
        UCASE_ACIRCUMFLEXACUTE,

        /**
         * U+1EAC LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND DOT BELOW
         */
        UCASE_ACIRCUMFLEXBELOWDOT,

        /**
         * U+1EA6 LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND GRAVE
         */
        UCASE_ACIRCUMFLEXGRAVE,

        /**
         * U+1EA8 LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND HOOK ABOVE
         */
        UCASE_ACIRCUMFLEXHOOK,

        /**
         * U+1EAA LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND TILDE
         */
        UCASE_ACIRCUMFLEXTILDE,

        /**
         * U+00C4 LATIN CAPITAL LETTER A WITH DIAERESIS
         */
        UCASE_ADIAERESIS,

        /**
         * U+00C6 LATIN CAPITAL LETTER AE
         */
        UCASE_AE,

        /**
         * U+00C0 LATIN CAPITAL LETTER A WITH GRAVE
         */
        UCASE_AGRAVE,

        /**
         * U+1EA2 LATIN CAPITAL LETTER A WITH HOOK ABOVE
         */
        UCASE_AHOOK,

        /**
         * U+0100 LATIN CAPITAL LETTER A WITH MACRON
         */
        UCASE_AMACRON,

        /**
         * U+0104 LATIN CAPITAL LETTER A WITH OGONEK
         */
        UCASE_AOGONEK,

        /**
         * U+00C5 LATIN CAPITAL LETTER A WITH RING ABOVE
         */
        UCASE_ARING,

        /**
         * U+00C3 LATIN CAPITAL LETTER A WITH TILDE
         */
        UCASE_ATILDE,

        /**
         * U+0042 LATIN CAPITAL LETTER B
         */
        UCASE_B,

        /**
         * U+1E02 LATIN CAPITAL LETTER B WITH DOT ABOVE
         */
        UCASE_BABOVEDOT,

        /**
         * U+0043 LATIN CAPITAL LETTER C
         */
        UCASE_C,

        /**
         * U+010A LATIN CAPITAL LETTER C WITH DOT ABOVE
         */
        UCASE_CABOVEDOT,

        /**
         * U+0106 LATIN CAPITAL LETTER C WITH ACUTE
         */
        UCASE_CACUTE,

        /**
         * U+010C LATIN CAPITAL LETTER C WITH CARON
         */
        UCASE_CCARON,

        /**
         * U+00C7 LATIN CAPITAL LETTER C WITH CEDILLA
         */
        UCASE_CCEDILLA,

        /**
         * U+0108 LATIN CAPITAL LETTER C WITH CIRCUMFLEX
         */
        UCASE_CCIRCUMFLEX,

        /**
         * Ucase Ch
         */
        UCASE_CH,

        /**
         * Ucase C H
         */
        UCASE_C_H,

        /**
         * U+0044 LATIN CAPITAL LETTER D
         */
        UCASE_D,

        /**
         * U+1E0A LATIN CAPITAL LETTER D WITH DOT ABOVE
         */
        UCASE_DABOVEDOT,

        /**
         * U+010E LATIN CAPITAL LETTER D WITH CARON
         */
        UCASE_DCARON,

        /**
         * U+0110 LATIN CAPITAL LETTER D WITH STROKE
         */
        UCASE_DSTROKE,

        /**
         * U+0045 LATIN CAPITAL LETTER E
         */
        UCASE_E,

        /**
         * U+0116 LATIN CAPITAL LETTER E WITH DOT ABOVE
         */
        UCASE_EABOVEDOT,

        /**
         * U+00C9 LATIN CAPITAL LETTER E WITH ACUTE
         */
        UCASE_EACUTE,

        /**
         * U+1EB8 LATIN CAPITAL LETTER E WITH DOT BELOW
         */
        UCASE_EBELOWDOT,

        /**
         * U+011A LATIN CAPITAL LETTER E WITH CARON
         */
        UCASE_ECARON,

        /**
         * U+00CA LATIN CAPITAL LETTER E WITH CIRCUMFLEX
         */
        UCASE_ECIRCUMFLEX,

        /**
         * U+1EBE LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND ACUTE
         */
        UCASE_ECIRCUMFLEXACUTE,

        /**
         * U+1EC6 LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND DOT BELOW
         */
        UCASE_ECIRCUMFLEXBELOWDOT,

        /**
         * U+1EC0 LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND GRAVE
         */
        UCASE_ECIRCUMFLEXGRAVE,

        /**
         * U+1EC2 LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND HOOK ABOVE
         */
        UCASE_ECIRCUMFLEXHOOK,

        /**
         * U+1EC4 LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND TILDE
         */
        UCASE_ECIRCUMFLEXTILDE,

        /**
         * U+00CB LATIN CAPITAL LETTER E WITH DIAERESIS
         */
        UCASE_EDIAERESIS,

        /**
         * U+00C8 LATIN CAPITAL LETTER E WITH GRAVE
         */
        UCASE_EGRAVE,

        /**
         * U+1EBA LATIN CAPITAL LETTER E WITH HOOK ABOVE
         */
        UCASE_EHOOK,

        /**
         * U+0112 LATIN CAPITAL LETTER E WITH MACRON
         */
        UCASE_EMACRON,

        /**
         * U+014A LATIN CAPITAL LETTER ENG
         */
        UCASE_ENG,

        /**
         * U+0118 LATIN CAPITAL LETTER E WITH OGONEK
         */
        UCASE_EOGONEK,

        /**
         * U+00D0 LATIN CAPITAL LETTER ETH
         */
        UCASE_ETH,

        /**
         * U+1EBC LATIN CAPITAL LETTER E WITH TILDE
         */
        UCASE_ETILDE,

        /**
         * U+01B7 LATIN CAPITAL LETTER EZH
         */
        UCASE_EZH,

        /**
         * U+0046 LATIN CAPITAL LETTER F
         */
        UCASE_F,

        /**
         * U+1E1E LATIN CAPITAL LETTER F WITH DOT ABOVE
         */
        UCASE_FABOVEDOT,

        /**
         * U+0047 LATIN CAPITAL LETTER G
         */
        UCASE_G,

        /**
         * U+0120 LATIN CAPITAL LETTER G WITH DOT ABOVE
         */
        UCASE_GABOVEDOT,

        /**
         * U+011E LATIN CAPITAL LETTER G WITH BREVE
         */
        UCASE_GBREVE,

        /**
         * U+01E6 LATIN CAPITAL LETTER G WITH CARON
         */
        UCASE_GCARON,

        /**
         * U+0122 LATIN CAPITAL LETTER G WITH CEDILLA
         */
        UCASE_GCEDILLA,

        /**
         * U+011C LATIN CAPITAL LETTER G WITH CIRCUMFLEX
         */
        UCASE_GCIRCUMFLEX,

        /**
         * U+0048 LATIN CAPITAL LETTER H
         */
        UCASE_H,

        /**
         * U+0124 LATIN CAPITAL LETTER H WITH CIRCUMFLEX
         */
        UCASE_HCIRCUMFLEX,

        /**
         * U+0126 LATIN CAPITAL LETTER H WITH STROKE
         */
        UCASE_HSTROKE,

        /**
         * U+0049 LATIN CAPITAL LETTER I
         */
        UCASE_I,

        /**
         * U+00CD LATIN CAPITAL LETTER I WITH ACUTE
         */
        UCASE_IACUTE,

        /**
         * U+1ECA LATIN CAPITAL LETTER I WITH DOT BELOW
         */
        UCASE_IBELOWDOT,

        /**
         * U+012C LATIN CAPITAL LETTER I WITH BREVE
         */
        UCASE_IBREVE,

        /**
         * U+00CE LATIN CAPITAL LETTER I WITH CIRCUMFLEX
         */
        UCASE_ICIRCUMFLEX,

        /**
         * U+00CF LATIN CAPITAL LETTER I WITH DIAERESIS
         */
        UCASE_IDIAERESIS,

        /**
         * U+00CC LATIN CAPITAL LETTER I WITH GRAVE
         */
        UCASE_IGRAVE,

        /**
         * U+1EC8 LATIN CAPITAL LETTER I WITH HOOK ABOVE
         */
        UCASE_IHOOK,

        /**
         * U+012A LATIN CAPITAL LETTER I WITH MACRON
         */
        UCASE_IMACRON,

        /**
         * U+012E LATIN CAPITAL LETTER I WITH OGONEK
         */
        UCASE_IOGONEK,

        /**
         * U+0128 LATIN CAPITAL LETTER I WITH TILDE
         */
        UCASE_ITILDE,

        /**
         * U+004A LATIN CAPITAL LETTER J
         */
        UCASE_J,

        /**
         * U+0134 LATIN CAPITAL LETTER J WITH CIRCUMFLEX
         */
        UCASE_JCIRCUMFLEX,

        /**
         * U+004B LATIN CAPITAL LETTER K
         */
        UCASE_K,

        /**
         * U+0136 LATIN CAPITAL LETTER K WITH CEDILLA
         */
        UCASE_KCEDILLA,

        /**
         * U+004C LATIN CAPITAL LETTER L
         */
        UCASE_L,

        /**
         * U+0139 LATIN CAPITAL LETTER L WITH ACUTE
         */
        UCASE_LACUTE,

        /**
         * U+1E36 LATIN CAPITAL LETTER L WITH DOT BELOW
         */
        UCASE_LBELOWDOT,

        /**
         * U+013D LATIN CAPITAL LETTER L WITH CARON
         */
        UCASE_LCARON,

        /**
         * U+013B LATIN CAPITAL LETTER L WITH CEDILLA
         */
        UCASE_LCEDILLA,

        /**
         * U+0141 LATIN CAPITAL LETTER L WITH STROKE
         */
        UCASE_LSTROKE,

        /**
         * U+004D LATIN CAPITAL LETTER M
         */
        UCASE_M,

        /**
         * U+1E40 LATIN CAPITAL LETTER M WITH DOT ABOVE
         */
        UCASE_MABOVEDOT,

        /**
         * U+004E LATIN CAPITAL LETTER N
         */
        UCASE_N,

        /**
         * U+0143 LATIN CAPITAL LETTER N WITH ACUTE
         */
        UCASE_NACUTE,

        /**
         * U+0147 LATIN CAPITAL LETTER N WITH CARON
         */
        UCASE_NCARON,

        /**
         * U+0145 LATIN CAPITAL LETTER N WITH CEDILLA
         */
        UCASE_NCEDILLA,

        /**
         * U+00D1 LATIN CAPITAL LETTER N WITH TILDE
         */
        UCASE_NTILDE,

        /**
         * U+004F LATIN CAPITAL LETTER O
         */
        UCASE_O,

        /**
         * U+00D3 LATIN CAPITAL LETTER O WITH ACUTE
         */
        UCASE_OACUTE,

        /**
         * U+019F LATIN CAPITAL LETTER O WITH MIDDLE TILDE
         */
        UCASE_OBARRED,

        /**
         * U+1ECC LATIN CAPITAL LETTER O WITH DOT BELOW
         */
        UCASE_OBELOWDOT,

        /**
         * U+01D2 LATIN CAPITAL LETTER O WITH CARON
         */
        UCASE_OCARON,

        /**
         * U+00D4 LATIN CAPITAL LETTER O WITH CIRCUMFLEX
         */
        UCASE_OCIRCUMFLEX,

        /**
         * U+1ED0 LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND ACUTE
         */
        UCASE_OCIRCUMFLEXACUTE,

        /**
         * U+1ED8 LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND DOT BELOW
         */
        UCASE_OCIRCUMFLEXBELOWDOT,

        /**
         * U+1ED2 LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND GRAVE
         */
        UCASE_OCIRCUMFLEXGRAVE,

        /**
         * U+1ED4 LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND HOOK ABOVE
         */
        UCASE_OCIRCUMFLEXHOOK,

        /**
         * U+1ED6 LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND TILDE
         */
        UCASE_OCIRCUMFLEXTILDE,

        /**
         * U+00D6 LATIN CAPITAL LETTER O WITH DIAERESIS
         */
        UCASE_ODIAERESIS,

        /**
         * U+0150 LATIN CAPITAL LETTER O WITH DOUBLE ACUTE
         */
        UCASE_ODOUBLEACUTE,

        /**
         * U+0152 LATIN CAPITAL LIGATURE OE
         */
        UCASE_OE,

        /**
         * U+00D2 LATIN CAPITAL LETTER O WITH GRAVE
         */
        UCASE_OGRAVE,

        /**
         * U+1ECE LATIN CAPITAL LETTER O WITH HOOK ABOVE
         */
        UCASE_OHOOK,

        /**
         * U+01A0 LATIN CAPITAL LETTER O WITH HORN
         */
        UCASE_OHORN,

        /**
         * U+1EDA LATIN CAPITAL LETTER O WITH HORN AND ACUTE
         */
        UCASE_OHORNACUTE,

        /**
         * U+1EE2 LATIN CAPITAL LETTER O WITH HORN AND DOT BELOW
         */
        UCASE_OHORNBELOWDOT,

        /**
         * U+1EDC LATIN CAPITAL LETTER O WITH HORN AND GRAVE
         */
        UCASE_OHORNGRAVE,

        /**
         * U+1EDE LATIN CAPITAL LETTER O WITH HORN AND HOOK ABOVE
         */
        UCASE_OHORNHOOK,

        /**
         * U+1EE0 LATIN CAPITAL LETTER O WITH HORN AND TILDE
         */
        UCASE_OHORNTILDE,

        /**
         * U+014C LATIN CAPITAL LETTER O WITH MACRON
         */
        UCASE_OMACRON,

        /**
         * U+00D8 LATIN CAPITAL LETTER O WITH STROKE
         */
        UCASE_OOBLIQUE,

        /**
         * U+00D8 LATIN CAPITAL LETTER O WITH STROKE
         */
        UCASE_OSLASH,

        /**
         * U+00D5 LATIN CAPITAL LETTER O WITH TILDE
         */
        UCASE_OTILDE,

        /**
         * U+0050 LATIN CAPITAL LETTER P
         */
        UCASE_P,

        /**
         * U+1E56 LATIN CAPITAL LETTER P WITH DOT ABOVE
         */
        UCASE_PABOVEDOT,

        /**
         * U+0051 LATIN CAPITAL LETTER Q
         */
        UCASE_Q,

        /**
         * U+0052 LATIN CAPITAL LETTER R
         */
        UCASE_R,

        /**
         * U+0154 LATIN CAPITAL LETTER R WITH ACUTE
         */
        UCASE_RACUTE,

        /**
         * U+0158 LATIN CAPITAL LETTER R WITH CARON
         */
        UCASE_RCARON,

        /**
         * U+0156 LATIN CAPITAL LETTER R WITH CEDILLA
         */
        UCASE_RCEDILLA,

        /**
         * U+0053 LATIN CAPITAL LETTER S
         */
        UCASE_S,

        /**
         * U+1E60 LATIN CAPITAL LETTER S WITH DOT ABOVE
         */
        UCASE_SABOVEDOT,

        /**
         * U+015A LATIN CAPITAL LETTER S WITH ACUTE
         */
        UCASE_SACUTE,

        /**
         * U+0160 LATIN CAPITAL LETTER S WITH CARON
         */
        UCASE_SCARON,

        /**
         * U+015E LATIN CAPITAL LETTER S WITH CEDILLA
         */
        UCASE_SCEDILLA,

        /**
         * U+018F LATIN CAPITAL LETTER SCHWA
         */
        UCASE_SCHWA,

        /**
         * U+015C LATIN CAPITAL LETTER S WITH CIRCUMFLEX
         */
        UCASE_SCIRCUMFLEX,

        /**
         * U+0054 LATIN CAPITAL LETTER T
         */
        UCASE_T,

        /**
         * U+1E6A LATIN CAPITAL LETTER T WITH DOT ABOVE
         */
        UCASE_TABOVEDOT,

        /**
         * U+0164 LATIN CAPITAL LETTER T WITH CARON
         */
        UCASE_TCARON,

        /**
         * U+0162 LATIN CAPITAL LETTER T WITH CEDILLA
         */
        UCASE_TCEDILLA,

        /**
         * U+00DE LATIN CAPITAL LETTER THORN
         */
        UCASE_THORN,

        /**
         * U+0166 LATIN CAPITAL LETTER T WITH STROKE
         */
        UCASE_TSLASH,

        /**
         * U+0055 LATIN CAPITAL LETTER U
         */
        UCASE_U,

        /**
         * U+00DA LATIN CAPITAL LETTER U WITH ACUTE
         */
        UCASE_UACUTE,

        /**
         * U+1EE4 LATIN CAPITAL LETTER U WITH DOT BELOW
         */
        UCASE_UBELOWDOT,

        /**
         * U+016C LATIN CAPITAL LETTER U WITH BREVE
         */
        UCASE_UBREVE,

        /**
         * U+00DB LATIN CAPITAL LETTER U WITH CIRCUMFLEX
         */
        UCASE_UCIRCUMFLEX,

        /**
         * U+00DC LATIN CAPITAL LETTER U WITH DIAERESIS
         */
        UCASE_UDIAERESIS,

        /**
         * U+0170 LATIN CAPITAL LETTER U WITH DOUBLE ACUTE
         */
        UCASE_UDOUBLEACUTE,

        /**
         * U+00D9 LATIN CAPITAL LETTER U WITH GRAVE
         */
        UCASE_UGRAVE,

        /**
         * U+1EE6 LATIN CAPITAL LETTER U WITH HOOK ABOVE
         */
        UCASE_UHOOK,

        /**
         * U+01AF LATIN CAPITAL LETTER U WITH HORN
         */
        UCASE_UHORN,

        /**
         * U+1EE8 LATIN CAPITAL LETTER U WITH HORN AND ACUTE
         */
        UCASE_UHORNACUTE,

        /**
         * U+1EF0 LATIN CAPITAL LETTER U WITH HORN AND DOT BELOW
         */
        UCASE_UHORNBELOWDOT,

        /**
         * U+1EEA LATIN CAPITAL LETTER U WITH HORN AND GRAVE
         */
        UCASE_UHORNGRAVE,

        /**
         * U+1EEC LATIN CAPITAL LETTER U WITH HORN AND HOOK ABOVE
         */
        UCASE_UHORNHOOK,

        /**
         * U+1EEE LATIN CAPITAL LETTER U WITH HORN AND TILDE
         */
        UCASE_UHORNTILDE,

        /**
         * U+016A LATIN CAPITAL LETTER U WITH MACRON
         */
        UCASE_UMACRON,

        /**
         * U+0172 LATIN CAPITAL LETTER U WITH OGONEK
         */
        UCASE_UOGONEK,

        /**
         * U+016E LATIN CAPITAL LETTER U WITH RING ABOVE
         */
        UCASE_URING,

        /**
         * U+0168 LATIN CAPITAL LETTER U WITH TILDE
         */
        UCASE_UTILDE,

        /**
         * U+0056 LATIN CAPITAL LETTER V
         */
        UCASE_V,

        /**
         * U+0057 LATIN CAPITAL LETTER W
         */
        UCASE_W,

        /**
         * U+1E82 LATIN CAPITAL LETTER W WITH ACUTE
         */
        UCASE_WACUTE,

        /**
         * U+0174 LATIN CAPITAL LETTER W WITH CIRCUMFLEX
         */
        UCASE_WCIRCUMFLEX,

        /**
         * U+1E84 LATIN CAPITAL LETTER W WITH DIAERESIS
         */
        UCASE_WDIAERESIS,

        /**
         * U+1E80 LATIN CAPITAL LETTER W WITH GRAVE
         */
        UCASE_WGRAVE,

        /**
         * U+0058 LATIN CAPITAL LETTER X
         */
        UCASE_X,

        /**
         * U+1E8A LATIN CAPITAL LETTER X WITH DOT ABOVE
         */
        UCASE_XABOVEDOT,

        /**
         * U+0059 LATIN CAPITAL LETTER Y
         */
        UCASE_Y,

        /**
         * U+00DD LATIN CAPITAL LETTER Y WITH ACUTE
         */
        UCASE_YACUTE,

        /**
         * U+1EF4 LATIN CAPITAL LETTER Y WITH DOT BELOW
         */
        UCASE_YBELOWDOT,

        /**
         * U+0176 LATIN CAPITAL LETTER Y WITH CIRCUMFLEX
         */
        UCASE_YCIRCUMFLEX,

        /**
         * U+0178 LATIN CAPITAL LETTER Y WITH DIAERESIS
         */
        UCASE_YDIAERESIS,

        /**
         * U+1EF2 LATIN CAPITAL LETTER Y WITH GRAVE
         */
        UCASE_YGRAVE,

        /**
         * U+1EF6 LATIN CAPITAL LETTER Y WITH HOOK ABOVE
         */
        UCASE_YHOOK,

        /**
         * U+1EF8 LATIN CAPITAL LETTER Y WITH TILDE
         */
        UCASE_YTILDE,

        /**
         * U+005A LATIN CAPITAL LETTER Z
         */
        UCASE_Z,

        /**
         * U+017B LATIN CAPITAL LETTER Z WITH DOT ABOVE
         */
        UCASE_ZABOVEDOT,

        /**
         * U+0179 LATIN CAPITAL LETTER Z WITH ACUTE
         */
        UCASE_ZACUTE,

        /**
         * U+017D LATIN CAPITAL LETTER Z WITH CARON
         */
        UCASE_ZCARON,

        /**
         * U+01B5 LATIN CAPITAL LETTER Z WITH STROKE
         */
        UCASE_ZSTROKE,

        /**
         * U+0491 CYRILLIC SMALL LETTER GHE WITH UPTURN
         */
        UKRAINIAN_LCASE_GHE_WITH_UPTURN,

        /**
         * U+0456 CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
         */
        UKRAINIAN_LCASE_I,

        /**
         * U+0454 CYRILLIC SMALL LETTER UKRAINIAN IE
         */
        UKRAINIAN_LCASE_IE,

        /**
         * U+0457 CYRILLIC SMALL LETTER YI
         */
        UKRAINIAN_LCASE_YI,

        /**
         * U+0490 CYRILLIC CAPITAL LETTER GHE WITH UPTURN
         */
        UKRAINIAN_UCASE_GHE_WITH_UPTURN,

        /**
         * U+0406 CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I
         */
        UKRAINIAN_UCASE_I,

        /**
         * U+0404 CYRILLIC CAPITAL LETTER UKRAINIAN IE
         */
        UKRAINIAN_UCASE_IE,

        /**
         * U+0407 CYRILLIC CAPITAL LETTER YI
         */
        UKRAINIAN_UCASE_YI,

        /**
         * deprecated
         */
        UKRANIAN_LCASE_I,

        /**
         * deprecated
         */
        UKRANIAN_LCASE_JE,

        /**
         * deprecated
         */
        UKRANIAN_LCASE_YI,

        /**
         * deprecated
         */
        UKRANIAN_UCASE_I,

        /**
         * deprecated
         */
        UKRANIAN_UCASE_JE,

        /**
         * deprecated
         */
        UKRANIAN_UCASE_YI,

        /**
         * U+005F LOW LINE
         */
        UNDERBAR,

        /**
         * U+005F LOW LINE
         */
        UNDERSCORE,

        /**
         * Undo
         */
        UNDO,

        /**
         * U+222A UNION
         */
        UNION,

        /**
         * Move up, up arrow
         */
        UP,

        /**
         * U+2191 UPWARDS ARROW
         */
        UPARROW,

        /**
         * U+2227 LOGICAL AND
         */
        UPCARET,

        /**
         * U+250C BOX DRAWINGS LIGHT DOWN AND RIGHT
         */
        UPLEFTCORNER,

        /**
         * U+2510 BOX DRAWINGS LIGHT DOWN AND LEFT
         */
        UPRIGHTCORNER,

        /**
         * U+2229 INTERSECTION
         */
        UPSHOE,

        /**
         * U+2308 LEFT CEILING
         */
        UPSTILE,

        /**
         * U+22A5 UP TACK
         */
        UPTACK,

        /**
         * User
         */
        USER,

        /**
         * U+221D PROPORTIONAL TO
         */
        VARIATION,

        /**
         * U+2502 BOX DRAWINGS LIGHT VERTICAL
         */
        VERTBAR,

        /**
         * U+2502 BOX DRAWINGS LIGHT VERTICAL
         */
        VERTCONNECTOR,

        /**
         * U+309B KATAKANA-HIRAGANA VOICED SOUND MARK
         */
        VOICEDSOUND,

        /**
         * Void symbol
         */
        VOID_SYMBOL,

        /**
         * U+240B SYMBOL FOR VERTICAL TABULATION
         */
        VT,

        /**
         * U+20A9 WON SIGN
         */
        WON_SIGN,

        /**
         * Logitch iTouch- don't use
         */
        XF86I_TOUCH,

        /**
         * Add URL to favorites list
         */
        XF86_ADD_FAVORITE,

        /**
         * switch to application, left
         */
        XF86_APPLICATION_LEFT,

        /**
         * switch to application, right
         */
        XF86_APPLICATION_RIGHT,

        /**
         * cycle through audio tracks
         */
        XF86_AUDIO_CYCLE_TRACK,

        /**
         * fast-forward audio track
         */
        XF86_AUDIO_FORWARD,

        /**
         * Volume control down
         */
        XF86_AUDIO_LOWER_VOLUME,

        /**
         * Launch media collection app
         */
        XF86_AUDIO_MEDIA,

        /**
         * Mute the Mic from the system
         */
        XF86_AUDIO_MIC_MUTE,

        /**
         * Mute sound from the system
         */
        XF86_AUDIO_MUTE,

        /**
         * Next track
         */
        XF86_AUDIO_NEXT,

        /**
         * Pause audio playing
         */
        XF86_AUDIO_PAUSE,

        /**
         * Start playing of audio >
         */
        XF86_AUDIO_PLAY,

        /**
         * Previous track
         */
        XF86_AUDIO_PREV,

        /**
         * Volume control up
         */
        XF86_AUDIO_RAISE_VOLUME,

        /**
         * toggle shuffle mode
         */
        XF86_AUDIO_RANDOM_PLAY,

        /**
         * Record audio application
         */
        XF86_AUDIO_RECORD,

        /**
         * toggle repeat mode
         */
        XF86_AUDIO_REPEAT,

        /**
         * "rewind" audio track
         */
        XF86_AUDIO_REWIND,

        /**
         * Stop playing audio
         */
        XF86_AUDIO_STOP,

        /**
         * mark yourself as away
         */
        XF86_AWAY,

        /**
         * Like back on a browser
         */
        XF86_BACK,

        /**
         * ???
         */
        XF86_BACK_FORWARD,

        /**
         * Display battery information
         */
        XF86_BATTERY,

        /**
         * Blue button
         */
        XF86_BLUE,

        /**
         * Enable/disable Bluetooth
         */
        XF86_BLUETOOTH,

        /**
         * Launch bookreader
         */
        XF86_BOOK,

        /**
         * Invoke brightness adj. UI
         */
        XF86_BRIGHTNESS_ADJUST,

        /**
         * Launch Calculater
         */
        XF86_CALCULATER,

        /**
         * Invoke calculator program
         */
        XF86_CALCULATOR,

        /**
         * Invoke Calendar program
         */
        XF86_CALENDAR,

        /**
         * Launch CD/DVD player
         */
        XF86_CD,

        /**
         * Clear window, screen
         */
        XF86_CLEAR,

        /**
         * kill application with grab
         */
        XF86_CLEAR_GRAB,

        /**
         * Close window
         */
        XF86_CLOSE,

        /**
         * Display user's community
         */
        XF86_COMMUNITY,

        /**
         * Adjust screen contrast
         */
        XF86_CONTRAST_ADJUST,

        /**
         * Copy selection
         */
        XF86_COPY,

        /**
         * Cut selection
         */
        XF86_CUT,

        /**
         * cycle through angles
         */
        XF86_CYCLE_ANGLE,

        /**
         * Output switch key
         */
        XF86_DISPLAY,

        /**
         * Open documents window
         */
        XF86_DOCUMENTS,

        /**
         * Launch DOS (emulation)
         */
        XF86_DOS,

        /**
         * Eject device (e.g. DVD)
         */
        XF86_EJECT,

        /**
         * Launch spread sheet
         */
        XF86_EXCEL,

        /**
         * Launch file explorer
         */
        XF86_EXPLORER,

        /**
         * Show favorite locations
         */
        XF86_FAVORITES,

        /**
         * Display financial site
         */
        XF86_FINANCE,

        /**
         * Like forward on a browser
         */
        XF86_FORWARD,

        /**
         * video: go one frame back
         */
        XF86_FRAME_BACK,

        /**
         * video: go one frame forward
         */
        XF86_FRAME_FORWARD,

        /**
         * Launch game
         */
        XF86_GAME,

        /**
         * Go to URL
         */
        XF86_GO,

        /**
         * Green button
         */
        XF86_GREEN,

        /**
         * Sleep to disk
         */
        XF86_HIBERNATE,

        /**
         * Show history of web surfing
         */
        XF86_HISTORY,

        /**
         * Display user's home page
         */
        XF86_HOME_PAGE,

        /**
         * Show "hot" links
         */
        XF86_HOT_LINKS,

        /**
         * Keyboards may be lit
         */
        XF86_KBD_BRIGHTNESS_DOWN,

        /**
         * Keyboards may be lit
         */
        XF86_KBD_BRIGHTNESS_UP,

        /**
         * Keyboards may be lit
         */
        XF86_KBD_LIGHT_ON_OFF,

        /**
         * Launch Application
         */
        XF86_LAUNCH0,

        /**
         * Launch Application
         */
        XF86_LAUNCH1,

        /**
         * Launch Application
         */
        XF86_LAUNCH2,

        /**
         * Launch Application
         */
        XF86_LAUNCH3,

        /**
         * Launch Application
         */
        XF86_LAUNCH4,

        /**
         * Launch Application
         */
        XF86_LAUNCH5,

        /**
         * Launch Application
         */
        XF86_LAUNCH6,

        /**
         * Launch Application
         */
        XF86_LAUNCH7,

        /**
         * Launch Application
         */
        XF86_LAUNCH8,

        /**
         * Launch Application
         */
        XF86_LAUNCH9,

        /**
         * Launch Application
         */
        XF86_LAUNCH_A,

        /**
         * Launch Application
         */
        XF86_LAUNCH_B,

        /**
         * Launch Application
         */
        XF86_LAUNCH_C,

        /**
         * Launch Application
         */
        XF86_LAUNCH_D,

        /**
         * Launch Application
         */
        XF86_LAUNCH_E,

        /**
         * Launch Application
         */
        XF86_LAUNCH_F,

        /**
         * Light bulb keys exist
         */
        XF86_LIGHT_BULB,

        /**
         * print all active grabs to log
         */
        XF86_LOG_GRAB_INFO,

        /**
         * Log off system
         */
        XF86_LOG_OFF,

        /**
         * print window tree to log
         */
        XF86_LOG_WINDOW_TREE,

        /**
         * Invoke user's mail program
         */
        XF86_MAIL,

        /**
         * Forward in mail
         */
        XF86_MAIL_FORWARD,

        /**
         * ??
         */
        XF86_MARKET,

        /**
         * enter meeting in calendar
         */
        XF86_MEETING,

        /**
         * Invoke Memo taking program
         */
        XF86_MEMO,

        /**
         * distingush keyboard from PB
         */
        XF86_MENU_KB,

        /**
         * distinuish PB from keyboard
         */
        XF86_MENU_PB,

        /**
         * as in instant messaging
         */
        XF86_MESSENGER,

        /**
         * Mode Switch Lock
         */
        XF86_MODE_LOCK,

        /**
         * Monitor/panel brightness
         */
        XF86_MON_BRIGHTNESS_DOWN,

        /**
         * Monitor/panel brightness
         */
        XF86_MON_BRIGHTNESS_UP,

        /**
         * Launch music application
         */
        XF86_MUSIC,

        /**
         * Display "My Computer" window
         */
        XF86_MY_COMPUTER,

        /**
         * Favourites
         */
        XF86_MY_SITES,

        /**
         * New (folder, document...)
         */
        XF86_NEW,

        /**
         * News
         */
        XF86_NEWS,

        /**
         * next video mode available
         */
        XF86_NEXT_VMODE,

        /**
         * Office home (old Staroffice)
         */
        XF86_OFFICE_HOME,

        /**
         * Open
         */
        XF86_OPEN,

        /**
         * Open selected URL
         */
        XF86_OPEN_URL,

        /**
         * ??
         */
        XF86_OPTION,

        /**
         * Paste
         */
        XF86_PASTE,

        /**
         * Launch phone; dial number
         */
        XF86_PHONE,

        /**
         * Show pictures
         */
        XF86_PICTURES,

        /**
         * Deep sleep the system
         */
        XF86_POWER_DOWN,

        /**
         * Power off system entirely
         */
        XF86_POWER_OFF,

        /**
         * prev. video mode available
         */
        XF86_PREV_VMODE,

        /**
         * Compaq's Q - don't use
         */
        XF86_Q,

        /**
         * Red button
         */
        XF86_RED,

        /**
         * Refresh the page
         */
        XF86_REFRESH,

        /**
         * Reload web page, file, etc.
         */
        XF86_RELOAD,

        /**
         * Reply e.g., mail
         */
        XF86_REPLY,

        /**
         * and down
         */
        XF86_ROCKER_DOWN,

        /**
         * and let you press them
         */
        XF86_ROCKER_ENTER,

        /**
         * Rocker switches exist up
         */
        XF86_ROCKER_UP,

        /**
         * Rotate windows e.g. xrandr
         */
        XF86_ROTATE_WINDOWS,

        /**
         * don't use
         */
        XF86_ROTATION_KB,

        /**
         * don't use
         */
        XF86_ROTATION_PB,

        /**
         * Save (file, document, state)
         */
        XF86_SAVE,

        /**
         * Invoke screensaver
         */
        XF86_SCREEN_SAVER,

        /**
         * Use XKB mousekeys instead
         */
        XF86_SCROLL_CLICK,

        /**
         * Scrool window/contentd down
         */
        XF86_SCROLL_DOWN,

        /**
         * Scroll window/contents up
         */
        XF86_SCROLL_UP,

        /**
         * Search
         */
        XF86_SEARCH,

        /**
         * Select button on joypads and remotes
         */
        XF86_SELECT,

        /**
         * Send mail, file, object
         */
        XF86_SEND,

        /**
         * Display shopping web site
         */
        XF86_SHOP,

        /**
         * Put system to sleep
         */
        XF86_SLEEP,

        /**
         * Spell checker
         */
        XF86_SPELL,

        /**
         * Split window or screen
         */
        XF86_SPLIT_SCREEN,

        /**
         * System into standby mode
         */
        XF86_STANDBY,

        /**
         * Start application
         */
        XF86_START,

        /**
         * Stop current operation
         */
        XF86_STOP,

        /**
         * cycle through subtitle
         */
        XF86_SUBTITLE,

        /**
         * Get support (??)
         */
        XF86_SUPPORT,

        /**
         * Sleep to RAM
         */
        XF86_SUSPEND,

        /**
         * Xf86 Switch Vt 1
         */
        XF86_SWITCH_VT_1,

        /**
         * Xf86 Switch Vt 10
         */
        XF86_SWITCH_VT_10,

        /**
         * Xf86 Switch Vt 11
         */
        XF86_SWITCH_VT_11,

        /**
         * Xf86 Switch Vt 12
         */
        XF86_SWITCH_VT_12,

        /**
         * Xf86 Switch Vt 2
         */
        XF86_SWITCH_VT_2,

        /**
         * Xf86 Switch Vt 3
         */
        XF86_SWITCH_VT_3,

        /**
         * Xf86 Switch Vt 4
         */
        XF86_SWITCH_VT_4,

        /**
         * Xf86 Switch Vt 5
         */
        XF86_SWITCH_VT_5,

        /**
         * Xf86 Switch Vt 6
         */
        XF86_SWITCH_VT_6,

        /**
         * Xf86 Switch Vt 7
         */
        XF86_SWITCH_VT_7,

        /**
         * Xf86 Switch Vt 8
         */
        XF86_SWITCH_VT_8,

        /**
         * Xf86 Switch Vt 9
         */
        XF86_SWITCH_VT_9,

        /**
         * Show tasks
         */
        XF86_TASK_PANE,

        /**
         * Launch terminal emulator
         */
        XF86_TERMINAL,

        /**
         * display, or shows an entry for time seeking
         */
        XF86_TIME,

        /**
         * toolbox of desktop/app.
         */
        XF86_TOOLS,

        /**
         * Go to a top-level menu in a video
         */
        XF86_TOP_MENU,

        /**
         * The touchpad got switched off
         */
        XF86_TOUCHPAD_OFF,

        /**
         * The touchpad got switched on
         */
        XF86_TOUCHPAD_ON,

        /**
         * Toggle between touchpad/trackstick
         */
        XF86_TOUCHPAD_TOGGLE,

        /**
         * Invoke To Do List program
         */
        XF86_TO_DO_LIST,

        /**
         * ??
         */
        XF86_TRAVEL,

        /**
         * force ungrab
         */
        XF86_UNGRAB,

        /**
         * ??
         */
        XF86_USER1_KB,

        /**
         * ??
         */
        XF86_USER2_KB,

        /**
         * ??
         */
        XF86_USER_PB,

        /**
         * Enable/disable UWB
         */
        XF86_UWB,

        /**
         * Display vendor home web site
         */
        XF86_VENDOR_HOME,

        /**
         * Launch video player
         */
        XF86_VIDEO,

        /**
         * Show a view options/properties
         */
        XF86_VIEW,

        /**
         * Wake up system from sleep
         */
        XF86_WAKE_UP,

        /**
         * Launch web camera app.
         */
        XF86_WEB_CAM,

        /**
         * button from a mouse wheel
         */
        XF86_WHEEL_BUTTON,

        /**
         * Enable/disable WLAN
         */
        XF86_WLAN,

        /**
         * Launch word processor
         */
        XF86_WORD,

        /**
         * Invoke web browser
         */
        XF86_WWW,

        /**
         * Xf86 Xfer
         */
        XF86_XFER,

        /**
         * Yellow button
         */
        XF86_YELLOW,

        /**
         * zoom in view, map, etc.
         */
        XF86_ZOOM_IN,

        /**
         * zoom out view, map, etc.
         */
        XF86_ZOOM_OUT,

        /**
         * U+00A5 YEN SIGN
         */
        YEN,

        /**
         * to Zenkaku
         */
        ZENKAKU,

        /**
         * Zenkaku/Hankaku toggle
         */
        ZENKAKU_HANKAKU,

        /**
         * Multiple/All Candidate(s)
         */
        ZEN_KOHO,

        /**
         * U+2080 SUBSCRIPT ZERO
         */
        ZEROSUBSCRIPT,

        /**
         * U+2070 SUPERSCRIPT ZERO
         */
        ZEROSUPERIOR,
    }
}
