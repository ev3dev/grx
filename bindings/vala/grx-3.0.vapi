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
 * The GRX3 graphics library.
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

        public static Color get (uint8 r, uint8 g, uint8 b);
        public static Color get_inline (uint8 r, uint8 g, uint8 b);
        public static Color get2 (uint hcolor);
        public static Color get2_inline (uint hcolor);
        public void put ();

        public static Color lookup (GLib.Array<Color> table, uint index);

        //  public static unowned Color[16] get_ega_colors ();

        [CCode (cname = "grx_color_get_ega_colors", array_length = false)]
        static unowned Color[] _get_ega_colors ();
        [CCode (cname = "vala_grx_color_get_ega_colors")]
        public static unowned Color[] get_ega_colors () {
            // work around https://bugzilla.gnome.org/show_bug.cgi?id=784691
            unowned Color[] colors = _get_ega_colors ();
            colors.length = 16;
            return colors;
        }

        public void query (Color c, out uint8 r, out uint8 g, out uint8 b);
        public void query_inline (Color c, out uint8 r, out uint8 g, out uint8 b);
        public void query2 (Color c, out uint hcolor);
        public void query2_inline (Color c, out uint hcolor);
    }

    [CCode (has_destroy_function = false, has_copy_function = false, has_type_id = false)]
    [SimpleType]
    public struct ColorCell : Color {
        public static ColorCell get ();
        public void set (uint8 r, uint8 g, uint8 b);
        public void put ();
    }

    [CCode (has_type_id = false)]
    public enum ColorPaletteType {
        GRAYSCALE,
        COLOR_TABLE,
        RGB,
    }

    /**
     * Functions for manipulating the color table.
     */
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

    [CCode (cprefix = "GRX_GRAPHICS_MODE_", has_type_id = false)]
    public enum GraphicsMode {
        UNKNOWN_MODE,
        TEXT_80X25,
        TEXT_DEFAULT,
        TEXT_WIDTH_HEIGHT,
        TEXT_WIDTH_HEIGHT_COLOR,
        TEXT_WIDTH_HEIGHT_BPP,
        GRAPHICS_DEFAULT,
        GRAPHICS_WIDTH_HEIGHT,
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

    [CCode (cprefix = "GRX_FRAME_MODE_", has_type_id = false)]
    public enum FrameMode {
        /* ====== video frame buffer modes ====== */
        UNDEFINED,
        TEXT,
        /* ==== linear frame buffer modes  ====== */
        LFB_MONO01,
        LFB_MONO10,
        LFB_2BPP,
        LFB_8BPP,
        LFB_16BPP,
        LFB_24BPP,
        LFB_32BPP_LOW,
        LFB_32BPP_HIGH,
        /* ====== system RAM frame buffer modes ====== */
        RAM_1BPP,
        RAM_2BPP,
        RAM_4XBPP,
        RAM_8BPP,
        RAM_16BPP,
        RAM_24BPP,
        RAM_32BPP_LOW,
        RAM_32BPP_HIGH;

        public static FrameMode get_current ();
        public static FrameMode get_screen ();
        public static FrameMode get_screen_core ();
        public int get_bpp ();
        public int get_n_planes ();
        public int get_line_offset (int width);
        public int get_plane_size (int width, int height);
        public int get_context_size (int width, int height);
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
        public VideoDriverFlags flags;
    }

    /*
     * Video driver mode descriptor structure.
     */
    [CCode (has_type_id = false)]
    public struct VideoMode {
        public bool present;
        public uint8 bpp;
        public short width;
        public short height;
        public short mode;
        public int line_offset;
        public int user_data;
        public VideoModeExt *extended_info;
    }

    [CCode (cprefix = "GRX_VIDEO_MODE_FLAG_", has_type_id = false)]
    [Flags]
    public enum VideoModeFlags {
        LINEAR,
        ACCEL,
        FAST_SVGA8 ,
        MEMORY,
    }

    [CCode (has_type_id = false)]
    public struct VideoModeExt {
        public FrameMode mode;
        public FrameDriver *driver;
        [CCode (cname = "frame")]
        public uint8 *fb_data;
        [CCode (cname = "cpos", array_length_cexpr = "3")]
        public uint8[] color_precision;
        [CCode (cname = "cprec", array_length_cexpr = "3")]
        public uint8[] color_position;
        public VideoModeFlags flags;
        public int lfb_selector;
    }

    [CCode (has_type_id = false)]
    public struct FrameDriver {
        public FrameMode mode;
        public FrameMode rmode;
        public int is_video;
        public int row_align;
        public int num_planes;
        public int bits_per_pixel;
        public int max_plane_size;
    }

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

    public const uint DEFAULT_DPI;

    /*
     * screen extents
     */
    public int get_screen_width ();
    public int get_screen_height ();
    public int get_virtual_width ();
    public int get_virtual_height ();
    public bool is_screen_virtual ();
    public bool set_viewport (int x, int y);
    public int get_viewport_x ();
    public int get_viewport_y ();

    /*
     * RAM context geometry and memory allocation inquiry stuff
     */
    public int screen_get_n_planes ();
    public int screen_get_line_offset (int width);
    public int screen_get_plane_size (int width, int height);
    public int screen_get_context_size (int width, int height);

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
        public static Context? new (int w, int h, [CCode (array_length = false)]uint8*[]? memory = null, out Context? where = null);
        public static Context? new_full (FrameMode mode, int w, int h, [CCode (array_length = false)]uint8*[]? memory = null, out Context? where = null);
        public static Context? new_subcontext (int x1, int y1, int x2, int y2, Context parent, out Context? where = null);

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
        public void bit_blt_1bpp (int x, int y, Context src, int x1, int y1, int x2, int y2, Color fg, Color bg);
        public void fast_bit_blt(int x, int y, Context src, int x1, int y1, int x2, int y2, Color op = (Color)ColorMode.WRITE);

        public bool save_to_pbm (string filename, string? comment = null) throws GLib.Error;
        public bool save_to_pgm (string filename, string? comment = null) throws GLib.Error;
        public bool save_to_ppm (string filename, string? comment = null) throws GLib.Error;
        public bool load_from_pnm (string filename);

        public bool load_from_pnm_data (uint8 *buffer);

        public bool save_to_png (string filename) throws GLib.Error;
        public bool load_from_png(string filename, bool use_alpha = true) throws GLib.Error;

        public bool load_from_jpeg (string filename, int scale = 1) throws GLib.Error;
        public bool save_to_jpeg (string filename, int quality = 90) throws GLib.Error;
        public bool save_to_jpeg_grayscale (string filename, int quality = 90) throws GLib.Error;
    }

    public Context save_current_context (out Context? where = null);
    public unowned Context get_current_context ();
    public void set_current_context (Context? context);
    public unowned Context get_screen_context ();

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
    public void draw_rounded_box (int x1, int y1, int x2, int y2, int r, Color c);
    public void draw_filled_rounded_box (int x1, int y1, int x2, int y2, int r, Color c);
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
    public void draw_polyline ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
    public void draw_polygon ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
    public void draw_filled_convex_polygon ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
    public void draw_filled_polygon ([CCode (array_length_pos = 0.9)]Point[] points, Color c);
    public void bit_blt (int x, int y, Context src, int x1, int y1, int x2, int y2, Color op = (Color)ColorMode.WRITE);
    public void bit_blt_1bpp (int x, int y, Context src, int x1, int y1, int x2, int y2, Color fg, Color bg);
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
    public void fast_draw_framed_box(int x1, int y1, int x2, int y2, int width, FramedBoxColors c);
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

    [CCode (has_type_id = false, has_destroy_function = false)]
    public struct LineOptions {
        public Color color;
        public int width;
        public int n_dash_patterns;
        public uint8 dash_pattern0;
        public uint8 dash_pattern1;
        public uint8 dash_pattern2;
        public uint8 dash_pattern3;
        public uint8 dash_pattern4;
        public uint8 dash_pattern5;
        public uint8 dash_pattern6;
        public uint8 dash_pattern7;
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

    [CCode (cprefix = "GRX_PIXMAP_MIRROR_", has_type_id = false)]
    [Flags]
    public enum PixmapMirrorFlags {
        HORIZONTAL, /* inverse left right */
        VERTICAL /* inverse top down */
    }

    [CCode (copy_function = "grx_pixmap_copy", free_function = "grx_pixmap_free", has_type_id = false)]
    [Compact]
    public class Pixmap {
        public static Pixmap new ([CCode (array_length = false)]uint8 *pixels, int w, int h, GLib.Array<Color>? colors);
        public static Pixmap new_from_bits ([CCode (array_length = false)]uint8 *bits, int w, int h, Color fg, Color bg);
        public static Pixmap new_from_context (Context context);
        public Pixmap mirror (PixmapMirrorFlags flags);
        public Pixmap stretch (int new_width, int new_height);
    }

    public void draw_pixmap (int x, int y, Pixmap p);
    public void draw_pixmap_tiled (int x1, int y1, int x2, int y2, Pixmap p);

    public void draw_pixel_with_pixmap (int x, int y, Pixmap p);
    public void draw_pixel_with_offset_pixmap (int x0, int y0, int x, int y, Pixmap p);
    public void draw_line_with_pixmap (int x1, int y1, int x2, int y2, LineOptions o, Pixmap p);
    public void draw_hline_with_offset_pixmap (int x0, int y0, int x, int y, int width, Pixmap p);
    public void draw_box_with_pixmap (int x1, int y1, int x2, int y2, LineOptions o, Pixmap p);
    public void draw_circle_with_pixmap (int xc, int yc, int r, LineOptions o, Pixmap p);
    public void draw_ellipse_with_pixmap (int xc, int yc, int rx, int ry, LineOptions o, Pixmap p);
    public void draw_circle_arc_with_pixmap (int xc, int yc, int r, int start, int end, ArcStyle style, LineOptions o, Pixmap p);
    public void draw_ellipse_arc_with_pixmap (int xc, int yc, int rx, int ry, int start, int end, ArcStyle style, LineOptions o, Pixmap p);
    public void draw_polyline_with_pixmap ([CCode (array_length_pos = 0.9)]Point[] points, LineOptions o, Pixmap p);
    public void draw_polygon_with_pixmap ([CCode (array_length_pos = 0.9)]Point[] points, LineOptions o, Pixmap p);

    public void draw_filled_line_with_pixmap (int x1, int y1, int x2, int y2, Pixmap p);
    public void draw_filled_box_with_pixmap (int x1, int y1, int x2, int y2, Pixmap p);
    public void draw_filled_box_with_offset_pixmap (int x0, int y0, int x1, int y1, int x2, int y2, Pixmap p);
    public void draw_filled_circle_with_pixmap (int xc, int yc, int r, Pixmap p);
    public void draw_filled_ellipse_with_pixmap (int xc, int yc, int rx, int ry, Pixmap p);
    public void draw_filled_circle_arc_with_pixmap (int xc, int yc, int r, int start, int end, int style, Pixmap p);
    public void draw_filled_ellipse_arc_with_pixmap (int xc, int yc, int rx, int ry, int start, int end, int style, Pixmap p);
    public void draw_filled_convex_polygon_with_pixmap ([CCode (array_length_pos = 0.9)]Point[] points, Pixmap p);
    public void draw_filled_polygon_with_pixmap ([CCode (array_length_pos = 0.9)]Point[] points, Pixmap p);
    public void flood_fill_with_pixmap (int x, int y, Color border, Pixmap p);


    /* ================================================================== */
    /*               DRAWING IN USER WINDOW COORDINATES                   */
    /* ================================================================== */

    /**
     * Functions for drawing using user-defined coordinate systems.
     */
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
        public void draw_framed_box (int x1, int y1, int x2, int y2, int width, FramedBoxColors c);
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

        public void draw_line_with_options (int x1, int y1, int x2, int y2, LineOptions o);
        public void draw_box_with_options (int x1, int y1, int x2, int y2, LineOptions o);
        public void draw_circle_with_options (int xc, int yc, int r, LineOptions o);
        public void draw_ellipse_with_options (int xc, int yc, int rx, int ry, LineOptions o);
        public void draw_circle_arc_with_options (int xc, int yc, int r, int start, int end, ArcStyle style, LineOptions o);
        public void draw_ellipse_arc_with_options (int xc, int yc, int rx, int ry, int start, int end, ArcStyle style, LineOptions o);
        public void draw_polyline_with_options ([CCode (array_length_pos = 0.9)]Point[] points, LineOptions o);
        public void draw_polygon_with_options ([CCode (array_length_pos = 0.9)]Point[] points, LineOptions o);

        public void draw_line_with_pixmap (int x1, int y1, int x2, int y2, LineOptions o, Pixmap p);
        public void draw_box_with_pixmap (int x1, int y1, int x2, int y2, LineOptions o, Pixmap p);
        public void draw_circle_with_pixmap (int xc, int yc, int r, LineOptions o, Pixmap p);
        public void draw_ellipse_with_pixmap (int xc, int yc, int rx, int ry, LineOptions o, Pixmap p);
        public void draw_circle_arc_with_pixmap (int xc, int yc, int r, int start, int end, ArcStyle style, LineOptions o, Pixmap p);
        public void draw_ellipse_arc_with_pixmap (int xc, int yc, int rx, int ry, int start, int end, ArcStyle style, LineOptions o, Pixmap p);
        public void draw_polyline_with_pixmap ([CCode (array_length_pos = 0.9)]Point[] points, LineOptions o, Pixmap p);
        public void draw_polygon_with_pixmap ([CCode (array_length_pos = 0.9)]Point[] points, LineOptions o, Pixmap p);

        public void draw_pixel_with_pixmap (int x, int y, Pixmap p);
        public void draw_filled_line_with_pixmap (int x1, int y1, int x2, int y2, Pixmap p);
        public void draw_filled_box_with_pixmap (int x1, int y1, int x2, int y2, Pixmap p);
        public void draw_filled_circle_with_pixmap (int xc, int yc, int r, Pixmap p);
        public void draw_filled_ellipse_with_pixmap (int xc, int yc, int rx, int ry, Pixmap p);
        public void draw_filled_circle_arc_with_pixmap (int xc, int yc, int r, int start, int end, int style, Pixmap p);
        public void draw_filled_ellipse_arc_with_pixmap (int xc, int yc, int rx, int ry, int start, int end, int style, Pixmap p);
        public void draw_filled_convex_polygon_with_pixmap ([CCode (array_length_pos = 0.9)]Point[] points, Pixmap p);
        public void draw_filled_polygon_with_pixmap ([CCode (array_length_pos = 0.9)]Point[] points, Pixmap p);
        public void flood_fill_with_pixmap (int x, int y, Color border, Pixmap p);

        public void draw_text (string text, int x, int y, Color fg, Color bg);
    }

    /* ================================================================== */
    /*                    GRAPHICS CURSOR UTILITIES                       */
    /* ================================================================== */

    [CCode (has_type_id = false)]
    public enum CursorMode {
        NORMAL,
        RUBBER,
        LINE,
        BOX
    }

    [CCode (copy_function = "grx_cursor_ref", free_function = "grx_cursor_unref")]
    [Compact]
    public class Cursor {
        public Cursor ([CCode (array_length = false)]uint8[] pixels, int pitch, int width, int height, int x0, int y0, GLib.Array<Color> colors);

        public void show ();
        public void hide ();
        public void move (int x, int y);
    }

    /**
     * Functions for manipulating the mouse cursor.
     */
    namespace Mouse {
        public static Cursor? get_cursor ();
        public static void set_cursor (Cursor? cursor);
        public static void set_cursor_default (Color fill_color, Color border_color);
        public static bool is_cursor_shown ();
        public static uint block (Context? context, int x1, int y1, int x2, int y2);
        public static void unblock (uint flags);
    }

    /* ================================================================== */
    /*                           PNM FUNCTIONS                            */
    /* ================================================================== */

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

    public bool query_pnm_file (string filename, out PnmFormat format, out int width, out int height, out int maxval);
    public bool query_pnm_data (uint8 *buffer, out PnmFormat format, out int width, out int height, out int maxval);

    /* ================================================================== */
    /*                           PNG FUNCTIONS                            */
    /*  these functions may not be installed or available on all system   */
    /* ================================================================== */

    public bool query_png_file (string filename, out int width, out int height);

    /* ================================================================== */
    /*                          JPEG FUNCTIONS                            */
    /*  these functions may not be installed or available on all system   */
    /* ================================================================== */

    public bool query_jpeg_file (string filename, out int width, out int height);

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

    [CCode (has_type_id = false)]
    [Compact]
    public class Event {
        public EventType type;
        public KeyEvent key;
        public MotionEvent motion;
        public ButtonEvent button;
        public TouchEvent touch;
        public ModifierFlags modifiers { get; }
        public unowned Device? device { get; }
        public Key keysym { get; }
        public unichar keychar { get; }
        public uint32 keycode { get; }
        public void get_coords (out uint32 x, out uint32 y);
        public uint32 button_code { [CCode (cname="grx_event_get_button")] get; }
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
        ABOVEDOT,
        ACCESS_X_ENABLE,
        ACCESS_X_FEEDBACK_ENABLE,
        ACUTE,
        ALT_L,
        ALT_R,
        AMPERSAND,
        APOSTROPHE,
        APPROXEQ,
        APPROXIMATE,
        ARABIC_0,
        ARABIC_1,
        ARABIC_2,
        ARABIC_3,
        ARABIC_4,
        ARABIC_5,
        ARABIC_6,
        ARABIC_7,
        ARABIC_8,
        ARABIC_9,
        ARABIC_AIN,
        ARABIC_ALEF,
        ARABIC_ALEFMAKSURA,
        ARABIC_BEH,
        ARABIC_COMMA,
        ARABIC_DAD,
        ARABIC_DAL,
        ARABIC_DAMMA,
        ARABIC_DAMMATAN,
        ARABIC_DDAL,
        ARABIC_FARSI_YEH,
        ARABIC_FATHA,
        ARABIC_FATHATAN,
        ARABIC_FEH,
        ARABIC_FULLSTOP,
        ARABIC_GAF,
        ARABIC_GHAIN,
        ARABIC_HA,
        ARABIC_HAH,
        ARABIC_HAMZA,
        ARABIC_HAMZAONALEF,
        ARABIC_HAMZAONWAW,
        ARABIC_HAMZAONYEH,
        ARABIC_HAMZAUNDERALEF,
        ARABIC_HAMZA_ABOVE,
        ARABIC_HAMZA_BELOW,
        ARABIC_HEH,
        ARABIC_HEH_DOACHASHMEE,
        ARABIC_HEH_GOAL,
        ARABIC_JEEM,
        ARABIC_JEH,
        ARABIC_KAF,
        ARABIC_KASRA,
        ARABIC_KASRATAN,
        ARABIC_KEHEH,
        ARABIC_KHAH,
        ARABIC_LAM,
        ARABIC_MADDAONALEF,
        ARABIC_MADDA_ABOVE,
        ARABIC_MEEM,
        ARABIC_NOON,
        ARABIC_NOON_GHUNNA,
        ARABIC_PEH,
        ARABIC_PERCENT,
        ARABIC_QAF,
        ARABIC_QUESTION_MARK,
        ARABIC_RA,
        ARABIC_RREH,
        ARABIC_SAD,
        ARABIC_SEEN,
        ARABIC_SEMICOLON,
        ARABIC_SHADDA,
        ARABIC_SHEEN,
        ARABIC_SUKUN,
        ARABIC_SUPERSCRIPT_ALEF,
        ARABIC_SWITCH,
        ARABIC_TAH,
        ARABIC_TATWEEL,
        ARABIC_TCHEH,
        ARABIC_TEH,
        ARABIC_TEHMARBUTA,
        ARABIC_THAL,
        ARABIC_THEH,
        ARABIC_TTEH,
        ARABIC_VEH,
        ARABIC_WAW,
        ARABIC_YEH,
        ARABIC_YEH_BAREE,
        ARABIC_ZAH,
        ARABIC_ZAIN,
        ARMENIAN_ACCENT,
        ARMENIAN_AMANAK,
        ARMENIAN_APOSTROPHE,
        ARMENIAN_BUT,
        ARMENIAN_EXCLAM,
        ARMENIAN_FULL_STOP,
        ARMENIAN_HYPHEN,
        ARMENIAN_LCASE_AT,
        ARMENIAN_LCASE_AYB,
        ARMENIAN_LCASE_BEN,
        ARMENIAN_LCASE_CHA,
        ARMENIAN_LCASE_DA,
        ARMENIAN_LCASE_DZA,
        ARMENIAN_LCASE_E,
        ARMENIAN_LCASE_FE,
        ARMENIAN_LCASE_GHAT,
        ARMENIAN_LCASE_GIM,
        ARMENIAN_LCASE_HI,
        ARMENIAN_LCASE_HO,
        ARMENIAN_LCASE_INI,
        ARMENIAN_LCASE_JE,
        ARMENIAN_LCASE_KE,
        ARMENIAN_LCASE_KEN,
        ARMENIAN_LCASE_KHE,
        ARMENIAN_LCASE_LYUN,
        ARMENIAN_LCASE_MEN,
        ARMENIAN_LCASE_NU,
        ARMENIAN_LCASE_O,
        ARMENIAN_LCASE_PE,
        ARMENIAN_LCASE_PYUR,
        ARMENIAN_LCASE_RA,
        ARMENIAN_LCASE_RE,
        ARMENIAN_LCASE_SE,
        ARMENIAN_LCASE_SHA,
        ARMENIAN_LCASE_TCHE,
        ARMENIAN_LCASE_TO,
        ARMENIAN_LCASE_TSA,
        ARMENIAN_LCASE_TSO,
        ARMENIAN_LCASE_TYUN,
        ARMENIAN_LCASE_VEV,
        ARMENIAN_LCASE_VO,
        ARMENIAN_LCASE_VYUN,
        ARMENIAN_LCASE_YECH,
        ARMENIAN_LCASE_ZA,
        ARMENIAN_LCASE_ZHE,
        ARMENIAN_LIGATURE_EW,
        ARMENIAN_PARUYK,
        ARMENIAN_QUESTION,
        ARMENIAN_SEPARATION_MARK,
        ARMENIAN_SHESHT,
        ARMENIAN_UCASE_AT,
        ARMENIAN_UCASE_AYB,
        ARMENIAN_UCASE_BEN,
        ARMENIAN_UCASE_CHA,
        ARMENIAN_UCASE_DA,
        ARMENIAN_UCASE_DZA,
        ARMENIAN_UCASE_E,
        ARMENIAN_UCASE_FE,
        ARMENIAN_UCASE_GHAT,
        ARMENIAN_UCASE_GIM,
        ARMENIAN_UCASE_HI,
        ARMENIAN_UCASE_HO,
        ARMENIAN_UCASE_INI,
        ARMENIAN_UCASE_JE,
        ARMENIAN_UCASE_KE,
        ARMENIAN_UCASE_KEN,
        ARMENIAN_UCASE_KHE,
        ARMENIAN_UCASE_LYUN,
        ARMENIAN_UCASE_MEN,
        ARMENIAN_UCASE_NU,
        ARMENIAN_UCASE_O,
        ARMENIAN_UCASE_PE,
        ARMENIAN_UCASE_PYUR,
        ARMENIAN_UCASE_RA,
        ARMENIAN_UCASE_RE,
        ARMENIAN_UCASE_SE,
        ARMENIAN_UCASE_SHA,
        ARMENIAN_UCASE_TCHE,
        ARMENIAN_UCASE_TO,
        ARMENIAN_UCASE_TSA,
        ARMENIAN_UCASE_TSO,
        ARMENIAN_UCASE_TYUN,
        ARMENIAN_UCASE_VEV,
        ARMENIAN_UCASE_VO,
        ARMENIAN_UCASE_VYUN,
        ARMENIAN_UCASE_YECH,
        ARMENIAN_UCASE_ZA,
        ARMENIAN_UCASE_ZHE,
        ARMENIAN_VERJAKET,
        ARMENIAN_YENTAMNA,
        ASCIICIRCUM,
        ASCIITILDE,
        ASTERISK,
        AT,
        AUDIBLE_BELL_ENABLE,
        BACKSLASH,
        BACK_SPACE,
        BACK_TAB,
        BALLOTCROSS,
        BAR,
        BECAUSE,
        BEGIN,
        BLANK,
        BLOCK,
        BOTINTEGRAL,
        BOTLEFTPARENS,
        BOTLEFTSQBRACKET,
        BOTLEFTSUMMATION,
        BOTRIGHTPARENS,
        BOTRIGHTSQBRACKET,
        BOTRIGHTSUMMATION,
        BOTT,
        BOTVERTSUMMATIONCONNECTOR,
        BOUNCE_KEYS_ENABLE,
        BRACELEFT,
        BRACERIGHT,
        BRACKETLEFT,
        BRACKETRIGHT,
        BRAILLE_BLANK,
        BRAILLE_DOTS_1,
        BRAILLE_DOTS_12,
        BRAILLE_DOTS_123,
        BRAILLE_DOTS_1234,
        BRAILLE_DOTS_12345,
        BRAILLE_DOTS_123456,
        BRAILLE_DOTS_1234567,
        BRAILLE_DOTS_12345678,
        BRAILLE_DOTS_1234568,
        BRAILLE_DOTS_123457,
        BRAILLE_DOTS_1234578,
        BRAILLE_DOTS_123458,
        BRAILLE_DOTS_12346,
        BRAILLE_DOTS_123467,
        BRAILLE_DOTS_1234678,
        BRAILLE_DOTS_123468,
        BRAILLE_DOTS_12347,
        BRAILLE_DOTS_123478,
        BRAILLE_DOTS_12348,
        BRAILLE_DOTS_1235,
        BRAILLE_DOTS_12356,
        BRAILLE_DOTS_123567,
        BRAILLE_DOTS_1235678,
        BRAILLE_DOTS_123568,
        BRAILLE_DOTS_12357,
        BRAILLE_DOTS_123578,
        BRAILLE_DOTS_12358,
        BRAILLE_DOTS_1236,
        BRAILLE_DOTS_12367,
        BRAILLE_DOTS_123678,
        BRAILLE_DOTS_12368,
        BRAILLE_DOTS_1237,
        BRAILLE_DOTS_12378,
        BRAILLE_DOTS_1238,
        BRAILLE_DOTS_124,
        BRAILLE_DOTS_1245,
        BRAILLE_DOTS_12456,
        BRAILLE_DOTS_124567,
        BRAILLE_DOTS_1245678,
        BRAILLE_DOTS_124568,
        BRAILLE_DOTS_12457,
        BRAILLE_DOTS_124578,
        BRAILLE_DOTS_12458,
        BRAILLE_DOTS_1246,
        BRAILLE_DOTS_12467,
        BRAILLE_DOTS_124678,
        BRAILLE_DOTS_12468,
        BRAILLE_DOTS_1247,
        BRAILLE_DOTS_12478,
        BRAILLE_DOTS_1248,
        BRAILLE_DOTS_125,
        BRAILLE_DOTS_1256,
        BRAILLE_DOTS_12567,
        BRAILLE_DOTS_125678,
        BRAILLE_DOTS_12568,
        BRAILLE_DOTS_1257,
        BRAILLE_DOTS_12578,
        BRAILLE_DOTS_1258,
        BRAILLE_DOTS_126,
        BRAILLE_DOTS_1267,
        BRAILLE_DOTS_12678,
        BRAILLE_DOTS_1268,
        BRAILLE_DOTS_127,
        BRAILLE_DOTS_1278,
        BRAILLE_DOTS_128,
        BRAILLE_DOTS_13,
        BRAILLE_DOTS_134,
        BRAILLE_DOTS_1345,
        BRAILLE_DOTS_13456,
        BRAILLE_DOTS_134567,
        BRAILLE_DOTS_1345678,
        BRAILLE_DOTS_134568,
        BRAILLE_DOTS_13457,
        BRAILLE_DOTS_134578,
        BRAILLE_DOTS_13458,
        BRAILLE_DOTS_1346,
        BRAILLE_DOTS_13467,
        BRAILLE_DOTS_134678,
        BRAILLE_DOTS_13468,
        BRAILLE_DOTS_1347,
        BRAILLE_DOTS_13478,
        BRAILLE_DOTS_1348,
        BRAILLE_DOTS_135,
        BRAILLE_DOTS_1356,
        BRAILLE_DOTS_13567,
        BRAILLE_DOTS_135678,
        BRAILLE_DOTS_13568,
        BRAILLE_DOTS_1357,
        BRAILLE_DOTS_13578,
        BRAILLE_DOTS_1358,
        BRAILLE_DOTS_136,
        BRAILLE_DOTS_1367,
        BRAILLE_DOTS_13678,
        BRAILLE_DOTS_1368,
        BRAILLE_DOTS_137,
        BRAILLE_DOTS_1378,
        BRAILLE_DOTS_138,
        BRAILLE_DOTS_14,
        BRAILLE_DOTS_145,
        BRAILLE_DOTS_1456,
        BRAILLE_DOTS_14567,
        BRAILLE_DOTS_145678,
        BRAILLE_DOTS_14568,
        BRAILLE_DOTS_1457,
        BRAILLE_DOTS_14578,
        BRAILLE_DOTS_1458,
        BRAILLE_DOTS_146,
        BRAILLE_DOTS_1467,
        BRAILLE_DOTS_14678,
        BRAILLE_DOTS_1468,
        BRAILLE_DOTS_147,
        BRAILLE_DOTS_1478,
        BRAILLE_DOTS_148,
        BRAILLE_DOTS_15,
        BRAILLE_DOTS_156,
        BRAILLE_DOTS_1567,
        BRAILLE_DOTS_15678,
        BRAILLE_DOTS_1568,
        BRAILLE_DOTS_157,
        BRAILLE_DOTS_1578,
        BRAILLE_DOTS_158,
        BRAILLE_DOTS_16,
        BRAILLE_DOTS_167,
        BRAILLE_DOTS_1678,
        BRAILLE_DOTS_168,
        BRAILLE_DOTS_17,
        BRAILLE_DOTS_178,
        BRAILLE_DOTS_18,
        BRAILLE_DOTS_2,
        BRAILLE_DOTS_23,
        BRAILLE_DOTS_234,
        BRAILLE_DOTS_2345,
        BRAILLE_DOTS_23456,
        BRAILLE_DOTS_234567,
        BRAILLE_DOTS_2345678,
        BRAILLE_DOTS_234568,
        BRAILLE_DOTS_23457,
        BRAILLE_DOTS_234578,
        BRAILLE_DOTS_23458,
        BRAILLE_DOTS_2346,
        BRAILLE_DOTS_23467,
        BRAILLE_DOTS_234678,
        BRAILLE_DOTS_23468,
        BRAILLE_DOTS_2347,
        BRAILLE_DOTS_23478,
        BRAILLE_DOTS_2348,
        BRAILLE_DOTS_235,
        BRAILLE_DOTS_2356,
        BRAILLE_DOTS_23567,
        BRAILLE_DOTS_235678,
        BRAILLE_DOTS_23568,
        BRAILLE_DOTS_2357,
        BRAILLE_DOTS_23578,
        BRAILLE_DOTS_2358,
        BRAILLE_DOTS_236,
        BRAILLE_DOTS_2367,
        BRAILLE_DOTS_23678,
        BRAILLE_DOTS_2368,
        BRAILLE_DOTS_237,
        BRAILLE_DOTS_2378,
        BRAILLE_DOTS_238,
        BRAILLE_DOTS_24,
        BRAILLE_DOTS_245,
        BRAILLE_DOTS_2456,
        BRAILLE_DOTS_24567,
        BRAILLE_DOTS_245678,
        BRAILLE_DOTS_24568,
        BRAILLE_DOTS_2457,
        BRAILLE_DOTS_24578,
        BRAILLE_DOTS_2458,
        BRAILLE_DOTS_246,
        BRAILLE_DOTS_2467,
        BRAILLE_DOTS_24678,
        BRAILLE_DOTS_2468,
        BRAILLE_DOTS_247,
        BRAILLE_DOTS_2478,
        BRAILLE_DOTS_248,
        BRAILLE_DOTS_25,
        BRAILLE_DOTS_256,
        BRAILLE_DOTS_2567,
        BRAILLE_DOTS_25678,
        BRAILLE_DOTS_2568,
        BRAILLE_DOTS_257,
        BRAILLE_DOTS_2578,
        BRAILLE_DOTS_258,
        BRAILLE_DOTS_26,
        BRAILLE_DOTS_267,
        BRAILLE_DOTS_2678,
        BRAILLE_DOTS_268,
        BRAILLE_DOTS_27,
        BRAILLE_DOTS_278,
        BRAILLE_DOTS_28,
        BRAILLE_DOTS_3,
        BRAILLE_DOTS_34,
        BRAILLE_DOTS_345,
        BRAILLE_DOTS_3456,
        BRAILLE_DOTS_34567,
        BRAILLE_DOTS_345678,
        BRAILLE_DOTS_34568,
        BRAILLE_DOTS_3457,
        BRAILLE_DOTS_34578,
        BRAILLE_DOTS_3458,
        BRAILLE_DOTS_346,
        BRAILLE_DOTS_3467,
        BRAILLE_DOTS_34678,
        BRAILLE_DOTS_3468,
        BRAILLE_DOTS_347,
        BRAILLE_DOTS_3478,
        BRAILLE_DOTS_348,
        BRAILLE_DOTS_35,
        BRAILLE_DOTS_356,
        BRAILLE_DOTS_3567,
        BRAILLE_DOTS_35678,
        BRAILLE_DOTS_3568,
        BRAILLE_DOTS_357,
        BRAILLE_DOTS_3578,
        BRAILLE_DOTS_358,
        BRAILLE_DOTS_36,
        BRAILLE_DOTS_367,
        BRAILLE_DOTS_3678,
        BRAILLE_DOTS_368,
        BRAILLE_DOTS_37,
        BRAILLE_DOTS_378,
        BRAILLE_DOTS_38,
        BRAILLE_DOTS_4,
        BRAILLE_DOTS_45,
        BRAILLE_DOTS_456,
        BRAILLE_DOTS_4567,
        BRAILLE_DOTS_45678,
        BRAILLE_DOTS_4568,
        BRAILLE_DOTS_457,
        BRAILLE_DOTS_4578,
        BRAILLE_DOTS_458,
        BRAILLE_DOTS_46,
        BRAILLE_DOTS_467,
        BRAILLE_DOTS_4678,
        BRAILLE_DOTS_468,
        BRAILLE_DOTS_47,
        BRAILLE_DOTS_478,
        BRAILLE_DOTS_48,
        BRAILLE_DOTS_5,
        BRAILLE_DOTS_56,
        BRAILLE_DOTS_567,
        BRAILLE_DOTS_5678,
        BRAILLE_DOTS_568,
        BRAILLE_DOTS_57,
        BRAILLE_DOTS_578,
        BRAILLE_DOTS_58,
        BRAILLE_DOTS_6,
        BRAILLE_DOTS_67,
        BRAILLE_DOTS_678,
        BRAILLE_DOTS_68,
        BRAILLE_DOTS_7,
        BRAILLE_DOTS_78,
        BRAILLE_DOTS_8,
        BRAILLE_DOT_1,
        BRAILLE_DOT_10,
        BRAILLE_DOT_2,
        BRAILLE_DOT_3,
        BRAILLE_DOT_4,
        BRAILLE_DOT_5,
        BRAILLE_DOT_6,
        BRAILLE_DOT_7,
        BRAILLE_DOT_8,
        BRAILLE_DOT_9,
        BREAK,
        BREVE,
        BROKENBAR,
        BYELORUSSIAN_LCASE_SHORTU,
        BYELORUSSIAN_UCASE_SHORTU,
        CANCEL,
        CAPS_LOCK,
        CAREOF,
        CARET,
        CARON,
        CEDILLA,
        CENT,
        CHECKERBOARD,
        CHECKMARK,
        CIRCLE,
        CLEAR,
        CLEAR_LINE,
        CLUB,
        CODEINPUT,
        COLON,
        COLON_SIGN,
        COMMA,
        CONTAINSAS,
        CONTROL_L,
        CONTROL_R,
        COPYRIGHT,
        CR,
        CROSSINGLINES,
        CRUZEIRO_SIGN,
        CUBEROOT,
        CURRENCY,
        CURSOR,
        CYRILLIC_LCASE_A,
        CYRILLIC_LCASE_BE,
        CYRILLIC_LCASE_CHE,
        CYRILLIC_LCASE_CHE_DESCENDER,
        CYRILLIC_LCASE_CHE_VERTSTROKE,
        CYRILLIC_LCASE_DE,
        CYRILLIC_LCASE_DZHE,
        CYRILLIC_LCASE_E,
        CYRILLIC_LCASE_EF,
        CYRILLIC_LCASE_EL,
        CYRILLIC_LCASE_EM,
        CYRILLIC_LCASE_EN,
        CYRILLIC_LCASE_EN_DESCENDER,
        CYRILLIC_LCASE_ER,
        CYRILLIC_LCASE_ES,
        CYRILLIC_LCASE_GHE,
        CYRILLIC_LCASE_GHE_BAR,
        CYRILLIC_LCASE_HA,
        CYRILLIC_LCASE_HARDSIGN,
        CYRILLIC_LCASE_HA_DESCENDER,
        CYRILLIC_LCASE_I,
        CYRILLIC_LCASE_IE,
        CYRILLIC_LCASE_IO,
        CYRILLIC_LCASE_I_MACRON,
        CYRILLIC_LCASE_JE,
        CYRILLIC_LCASE_KA,
        CYRILLIC_LCASE_KA_DESCENDER,
        CYRILLIC_LCASE_KA_VERTSTROKE,
        CYRILLIC_LCASE_LJE,
        CYRILLIC_LCASE_NJE,
        CYRILLIC_LCASE_O,
        CYRILLIC_LCASE_O_BAR,
        CYRILLIC_LCASE_PE,
        CYRILLIC_LCASE_SCHWA,
        CYRILLIC_LCASE_SHA,
        CYRILLIC_LCASE_SHCHA,
        CYRILLIC_LCASE_SHHA,
        CYRILLIC_LCASE_SHORTI,
        CYRILLIC_LCASE_SOFTSIGN,
        CYRILLIC_LCASE_TE,
        CYRILLIC_LCASE_TSE,
        CYRILLIC_LCASE_U,
        CYRILLIC_LCASE_U_MACRON,
        CYRILLIC_LCASE_U_STRAIGHT,
        CYRILLIC_LCASE_U_STRAIGHT_BAR,
        CYRILLIC_LCASE_VE,
        CYRILLIC_LCASE_YA,
        CYRILLIC_LCASE_YERU,
        CYRILLIC_LCASE_YU,
        CYRILLIC_LCASE_ZE,
        CYRILLIC_LCASE_ZHE,
        CYRILLIC_LCASE_ZHE_DESCENDER,
        CYRILLIC_UCASE_A,
        CYRILLIC_UCASE_BE,
        CYRILLIC_UCASE_CHE,
        CYRILLIC_UCASE_CHE_DESCENDER,
        CYRILLIC_UCASE_CHE_VERTSTROKE,
        CYRILLIC_UCASE_DE,
        CYRILLIC_UCASE_DZHE,
        CYRILLIC_UCASE_E,
        CYRILLIC_UCASE_EF,
        CYRILLIC_UCASE_EL,
        CYRILLIC_UCASE_EM,
        CYRILLIC_UCASE_EN,
        CYRILLIC_UCASE_EN_DESCENDER,
        CYRILLIC_UCASE_ER,
        CYRILLIC_UCASE_ES,
        CYRILLIC_UCASE_GHE,
        CYRILLIC_UCASE_GHE_BAR,
        CYRILLIC_UCASE_HA,
        CYRILLIC_UCASE_HARDSIGN,
        CYRILLIC_UCASE_HA_DESCENDER,
        CYRILLIC_UCASE_I,
        CYRILLIC_UCASE_IE,
        CYRILLIC_UCASE_IO,
        CYRILLIC_UCASE_I_MACRON,
        CYRILLIC_UCASE_JE,
        CYRILLIC_UCASE_KA,
        CYRILLIC_UCASE_KA_DESCENDER,
        CYRILLIC_UCASE_KA_VERTSTROKE,
        CYRILLIC_UCASE_LJE,
        CYRILLIC_UCASE_NJE,
        CYRILLIC_UCASE_O,
        CYRILLIC_UCASE_O_BAR,
        CYRILLIC_UCASE_PE,
        CYRILLIC_UCASE_SCHWA,
        CYRILLIC_UCASE_SHA,
        CYRILLIC_UCASE_SHCHA,
        CYRILLIC_UCASE_SHHA,
        CYRILLIC_UCASE_SHORTI,
        CYRILLIC_UCASE_SOFTSIGN,
        CYRILLIC_UCASE_TE,
        CYRILLIC_UCASE_TSE,
        CYRILLIC_UCASE_U,
        CYRILLIC_UCASE_U_MACRON,
        CYRILLIC_UCASE_U_STRAIGHT,
        CYRILLIC_UCASE_U_STRAIGHT_BAR,
        CYRILLIC_UCASE_VE,
        CYRILLIC_UCASE_YA,
        CYRILLIC_UCASE_YERU,
        CYRILLIC_UCASE_YU,
        CYRILLIC_UCASE_ZE,
        CYRILLIC_UCASE_ZHE,
        CYRILLIC_UCASE_ZHE_DESCENDER,
        D0,
        D1,
        D2,
        D3,
        D4,
        D5,
        D6,
        D7,
        D8,
        D9,
        DACUTE_ACCENT,
        DAGGER,
        DCEDILLA_ACCENT,
        DCIRCUMFLEX_ACCENT,
        DDIAERESIS,
        DEAD_ABOVECOMMA,
        DEAD_ABOVEDOT,
        DEAD_ABOVEREVERSEDCOMMA,
        DEAD_ABOVERING,
        DEAD_ABOVEVERTICALLINE,
        DEAD_ACUTE,
        DEAD_BELOWBREVE,
        DEAD_BELOWCIRCUMFLEX,
        DEAD_BELOWCOMMA,
        DEAD_BELOWDIAERESIS,
        DEAD_BELOWDOT,
        DEAD_BELOWMACRON,
        DEAD_BELOWRING,
        DEAD_BELOWTILDE,
        DEAD_BELOWVERTICALLINE,
        DEAD_BREVE,
        DEAD_CAPITAL_SCHWA,
        DEAD_CARON,
        DEAD_CEDILLA,
        DEAD_CIRCUMFLEX,
        DEAD_CURRENCY,
        DEAD_DASIA,
        DEAD_DIAERESIS,
        DEAD_DOUBLEACUTE,
        DEAD_DOUBLEGRAVE,
        DEAD_GRAVE,
        DEAD_GREEK,
        DEAD_HOOK,
        DEAD_HORN,
        DEAD_INVERTEDBREVE,
        DEAD_IOTA,
        DEAD_LCASE_A,
        DEAD_LCASE_E,
        DEAD_LCASE_I,
        DEAD_LCASE_O,
        DEAD_LCASE_U,
        DEAD_LONGSOLIDUSOVERLAY,
        DEAD_LOWLINE,
        DEAD_MACRON,
        DEAD_OGONEK,
        DEAD_PERISPOMENI,
        DEAD_PSILI,
        DEAD_SEMIVOICED_SOUND,
        DEAD_SMALL_SCHWA,
        DEAD_STROKE,
        DEAD_TILDE,
        DEAD_UCASE_A,
        DEAD_UCASE_E,
        DEAD_UCASE_I,
        DEAD_UCASE_O,
        DEAD_UCASE_U,
        DEAD_VOICED_SOUND,
        DECIMALPOINT,
        DEGREE,
        DELETE,
        DELETE_CHAR,
        DELETE_LINE,
        DGRAVE_ACCENT,
        DIAERESIS,
        DIAMOND,
        DIGITSPACE,
        DINTEGRAL,
        DIVISION,
        DOLLAR,
        DONG_SIGN,
        DOUBBASELINEDOT,
        DOUBLEACUTE,
        DOUBLEDAGGER,
        DOUBLELOWQUOTEMARK,
        DOWN,
        DOWNARROW,
        DOWNCARET,
        DOWNSHOE,
        DOWNSTILE,
        DOWNTACK,
        DREMOVE,
        DRING_ACCENT,
        DTILDE,
        ECU_SIGN,
        EIGHTSUBSCRIPT,
        EIGHTSUPERIOR,
        EISU_SHIFT,
        EISU_TOGGLE,
        ELEMENTOF,
        ELLIPSIS,
        EM3SPACE,
        EM4SPACE,
        EMDASH,
        EMFILLEDCIRCLE,
        EMFILLEDRECT,
        EMOPENCIRCLE,
        EMOPENRECTANGLE,
        EMPTYSET,
        EMSPACE,
        END,
        ENDASH,
        ENFILLEDCIRCBULLET,
        ENFILLEDSQBULLET,
        ENOPENCIRCBULLET,
        ENOPENSQUAREBULLET,
        ENSPACE,
        EQUAL,
        ESCAPE,
        EURO_SIGN,
        EXCLAM,
        EXCLAMDOWN,
        EXECUTE,
        EXT16BIT_L,
        EXT16BIT_R,
        F1,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F2,
        F20,
        F21,
        F22,
        F23,
        F24,
        F25,
        F26,
        F27,
        F28,
        F29,
        F3,
        F30,
        F31,
        F32,
        F33,
        F34,
        F35,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        FARSI_0,
        FARSI_1,
        FARSI_2,
        FARSI_3,
        FARSI_4,
        FARSI_5,
        FARSI_6,
        FARSI_7,
        FARSI_8,
        FARSI_9,
        FARSI_YEH,
        FEMALESYMBOL,
        FF,
        FFRANC_SIGN,
        FIGDASH,
        FILLEDLEFTTRIBULLET,
        FILLEDRECTBULLET,
        FILLEDRIGHTTRIBULLET,
        FILLEDTRIBULLETDOWN,
        FILLEDTRIBULLETUP,
        FIND,
        FIRST_VIRTUAL_SCREEN,
        FIVEEIGHTHS,
        FIVESIXTHS,
        FIVESUBSCRIPT,
        FIVESUPERIOR,
        FOURFIFTHS,
        FOURSUBSCRIPT,
        FOURSUPERIOR,
        FOURTHROOT,
        FUNCTION,
        GEORGIAN_AN,
        GEORGIAN_BAN,
        GEORGIAN_CAN,
        GEORGIAN_CHAR,
        GEORGIAN_CHIN,
        GEORGIAN_CIL,
        GEORGIAN_DON,
        GEORGIAN_EN,
        GEORGIAN_FI,
        GEORGIAN_GAN,
        GEORGIAN_GHAN,
        GEORGIAN_HAE,
        GEORGIAN_HAR,
        GEORGIAN_HE,
        GEORGIAN_HIE,
        GEORGIAN_HOE,
        GEORGIAN_IN,
        GEORGIAN_JHAN,
        GEORGIAN_JIL,
        GEORGIAN_KAN,
        GEORGIAN_KHAR,
        GEORGIAN_LAS,
        GEORGIAN_MAN,
        GEORGIAN_NAR,
        GEORGIAN_ON,
        GEORGIAN_PAR,
        GEORGIAN_PHAR,
        GEORGIAN_QAR,
        GEORGIAN_RAE,
        GEORGIAN_SAN,
        GEORGIAN_SHIN,
        GEORGIAN_TAN,
        GEORGIAN_TAR,
        GEORGIAN_UN,
        GEORGIAN_VIN,
        GEORGIAN_WE,
        GEORGIAN_XAN,
        GEORGIAN_ZEN,
        GEORGIAN_ZHAR,
        GRAVE,
        GREATER,
        GREATERTHANEQUAL,
        GREEK_ACCENTDIERESIS,
        GREEK_FINALSMALLSIGMA,
        GREEK_HORIZBAR,
        GREEK_IOTAACCENTDIERESIS,
        GREEK_IOTADIAERESIS,
        GREEK_LCASE_ALPHA,
        GREEK_LCASE_ALPHAACCENT,
        GREEK_LCASE_BETA,
        GREEK_LCASE_CHI,
        GREEK_LCASE_DELTA,
        GREEK_LCASE_EPSILON,
        GREEK_LCASE_EPSILONACCENT,
        GREEK_LCASE_ETA,
        GREEK_LCASE_ETAACCENT,
        GREEK_LCASE_GAMMA,
        GREEK_LCASE_IOTA,
        GREEK_LCASE_IOTAACCENT,
        GREEK_LCASE_IOTADIERESIS,
        GREEK_LCASE_KAPPA,
        GREEK_LCASE_LAMBDA,
        GREEK_LCASE_LAMDA,
        GREEK_LCASE_MU,
        GREEK_LCASE_NU,
        GREEK_LCASE_OMEGA,
        GREEK_LCASE_OMEGAACCENT,
        GREEK_LCASE_OMICRON,
        GREEK_LCASE_OMICRONACCENT,
        GREEK_LCASE_PHI,
        GREEK_LCASE_PI,
        GREEK_LCASE_PSI,
        GREEK_LCASE_RHO,
        GREEK_LCASE_SIGMA,
        GREEK_LCASE_TAU,
        GREEK_LCASE_THETA,
        GREEK_LCASE_UPSILON,
        GREEK_LCASE_UPSILONACCENT,
        GREEK_LCASE_UPSILONDIERESIS,
        GREEK_LCASE_XI,
        GREEK_LCASE_ZETA,
        GREEK_SWITCH,
        GREEK_UCASE_ALPHA,
        GREEK_UCASE_ALPHAACCENT,
        GREEK_UCASE_BETA,
        GREEK_UCASE_CHI,
        GREEK_UCASE_DELTA,
        GREEK_UCASE_EPSILON,
        GREEK_UCASE_EPSILONACCENT,
        GREEK_UCASE_ETA,
        GREEK_UCASE_ETAACCENT,
        GREEK_UCASE_GAMMA,
        GREEK_UCASE_IOTA,
        GREEK_UCASE_IOTAACCENT,
        GREEK_UCASE_IOTADIERESIS,
        GREEK_UCASE_KAPPA,
        GREEK_UCASE_LAMBDA,
        GREEK_UCASE_LAMDA,
        GREEK_UCASE_MU,
        GREEK_UCASE_NU,
        GREEK_UCASE_OMEGA,
        GREEK_UCASE_OMEGAACCENT,
        GREEK_UCASE_OMICRON,
        GREEK_UCASE_OMICRONACCENT,
        GREEK_UCASE_PHI,
        GREEK_UCASE_PI,
        GREEK_UCASE_PSI,
        GREEK_UCASE_RHO,
        GREEK_UCASE_SIGMA,
        GREEK_UCASE_TAU,
        GREEK_UCASE_THETA,
        GREEK_UCASE_UPSILON,
        GREEK_UCASE_UPSILONACCENT,
        GREEK_UCASE_UPSILONDIERESIS,
        GREEK_UCASE_XI,
        GREEK_UCASE_ZETA,
        GREEK_UPSILONACCENTDIERESIS,
        GUILDER,
        GUILLEMOTLEFT,
        GUILLEMOTRIGHT,
        HAIRSPACE,
        HANGUL,
        HANGUL_A,
        HANGUL_AE,
        HANGUL_ARAE_A,
        HANGUL_ARAE_AE,
        HANGUL_BANJA,
        HANGUL_CIEUC,
        HANGUL_CODEINPUT,
        HANGUL_DIKEUD,
        HANGUL_E,
        HANGUL_END,
        HANGUL_EO,
        HANGUL_EU,
        HANGUL_HANJA,
        HANGUL_HIEUH,
        HANGUL_I,
        HANGUL_IEUNG,
        HANGUL_JAMO,
        HANGUL_JEONJA,
        HANGUL_JIEUJ,
        HANGUL_J_CIEUC,
        HANGUL_J_DIKEUD,
        HANGUL_J_HIEUH,
        HANGUL_J_IEUNG,
        HANGUL_J_JIEUJ,
        HANGUL_J_KHIEUQ,
        HANGUL_J_KIYEOG,
        HANGUL_J_KIYEOG_SIOS,
        HANGUL_J_KKOGJI_DALRIN_IEUNG,
        HANGUL_J_MIEUM,
        HANGUL_J_NIEUN,
        HANGUL_J_NIEUN_HIEUH,
        HANGUL_J_NIEUN_JIEUJ,
        HANGUL_J_PAN_SIOS,
        HANGUL_J_PHIEUF,
        HANGUL_J_PIEUB,
        HANGUL_J_PIEUB_SIOS,
        HANGUL_J_RIEUL,
        HANGUL_J_RIEUL_HIEUH,
        HANGUL_J_RIEUL_KIYEOG,
        HANGUL_J_RIEUL_MIEUM,
        HANGUL_J_RIEUL_PHIEUF,
        HANGUL_J_RIEUL_PIEUB,
        HANGUL_J_RIEUL_SIOS,
        HANGUL_J_RIEUL_TIEUT,
        HANGUL_J_SIOS,
        HANGUL_J_SSANG_KIYEOG,
        HANGUL_J_SSANG_SIOS,
        HANGUL_J_TIEUT,
        HANGUL_J_YEORIN_HIEUH,
        HANGUL_KHIEUQ,
        HANGUL_KIYEOG,
        HANGUL_KIYEOG_SIOS,
        HANGUL_KKOGJI_DALRIN_IEUNG,
        HANGUL_MIEUM,
        HANGUL_MULTIPLE_CANDIDATE,
        HANGUL_NIEUN,
        HANGUL_NIEUN_HIEUH,
        HANGUL_NIEUN_JIEUJ,
        HANGUL_O,
        HANGUL_OE,
        HANGUL_PAN_SIOS,
        HANGUL_PHIEUF,
        HANGUL_PIEUB,
        HANGUL_PIEUB_SIOS,
        HANGUL_POST_HANJA,
        HANGUL_PREVIOUS_CANDIDATE,
        HANGUL_PRE_HANJA,
        HANGUL_RIEUL,
        HANGUL_RIEUL_HIEUH,
        HANGUL_RIEUL_KIYEOG,
        HANGUL_RIEUL_MIEUM,
        HANGUL_RIEUL_PHIEUF,
        HANGUL_RIEUL_PIEUB,
        HANGUL_RIEUL_SIOS,
        HANGUL_RIEUL_TIEUT,
        HANGUL_RIEUL_YEORIN_HIEUH,
        HANGUL_ROMAJA,
        HANGUL_SINGLE_CANDIDATE,
        HANGUL_SIOS,
        HANGUL_SPECIAL,
        HANGUL_SSANG_DIKEUD,
        HANGUL_SSANG_JIEUJ,
        HANGUL_SSANG_KIYEOG,
        HANGUL_SSANG_PIEUB,
        HANGUL_SSANG_SIOS,
        HANGUL_START,
        HANGUL_SUNKYEONGEUM_MIEUM,
        HANGUL_SUNKYEONGEUM_PHIEUF,
        HANGUL_SUNKYEONGEUM_PIEUB,
        HANGUL_SWITCH,
        HANGUL_TIEUT,
        HANGUL_U,
        HANGUL_WA,
        HANGUL_WAE,
        HANGUL_WE,
        HANGUL_WEO,
        HANGUL_WI,
        HANGUL_YA,
        HANGUL_YAE,
        HANGUL_YE,
        HANGUL_YEO,
        HANGUL_YEORIN_HIEUH,
        HANGUL_YI,
        HANGUL_YO,
        HANGUL_YU,
        HANKAKU,
        HEART,
        HEBREW_ALEPH,
        HEBREW_AYIN,
        HEBREW_BET,
        HEBREW_BETH,
        HEBREW_CHET,
        HEBREW_DALET,
        HEBREW_DALETH,
        HEBREW_DOUBLELOWLINE,
        HEBREW_FINALKAPH,
        HEBREW_FINALMEM,
        HEBREW_FINALNUN,
        HEBREW_FINALPE,
        HEBREW_FINALZADE,
        HEBREW_FINALZADI,
        HEBREW_GIMEL,
        HEBREW_GIMMEL,
        HEBREW_HE,
        HEBREW_HET,
        HEBREW_KAPH,
        HEBREW_KUF,
        HEBREW_LAMED,
        HEBREW_MEM,
        HEBREW_NUN,
        HEBREW_PE,
        HEBREW_QOPH,
        HEBREW_RESH,
        HEBREW_SAMECH,
        HEBREW_SAMEKH,
        HEBREW_SHIN,
        HEBREW_SWITCH,
        HEBREW_TAF,
        HEBREW_TAW,
        HEBREW_TET,
        HEBREW_TETH,
        HEBREW_WAW,
        HEBREW_YOD,
        HEBREW_ZADE,
        HEBREW_ZADI,
        HEBREW_ZAIN,
        HEBREW_ZAYIN,
        HELP,
        HENKAN,
        HENKAN_MODE,
        HEXAGRAM,
        HIRAGANA,
        HIRAGANA_KATAKANA,
        HOME,
        HORIZCONNECTOR,
        HORIZLINESCAN1,
        HORIZLINESCAN3,
        HORIZLINESCAN5,
        HORIZLINESCAN7,
        HORIZLINESCAN9,
        HPBLOCK,
        HPGUILDER,
        HPLIRA,
        HPLONGMINUS,
        HPMUTE_ACUTE,
        HPMUTE_ASCIICIRCUM,
        HPMUTE_ASCIITILDE,
        HPMUTE_DIAERESIS,
        HPMUTE_GRAVE,
        HP_BACK_TAB,
        HP_CLEAR_LINE,
        HP_DELETE_CHAR,
        HP_DELETE_LINE,
        HP_INSERT_CHAR,
        HP_INSERT_LINE,
        HP_IO,
        HP_KP_BACK_TAB,
        HP_MODELOCK1,
        HP_MODELOCK2,
        HP_RESET,
        HP_SYSTEM,
        HP_USER,
        HP_YDIAERESIS,
        HT,
        HYPER_L,
        HYPER_R,
        HYPHEN,
        IABOVEDOT,
        IDENTICAL,
        IDOTLESS,
        IFONLYIF,
        IMPLIES,
        INCLUDEDIN,
        INCLUDES,
        INFINITY,
        INSERT,
        INSERT_CHAR,
        INSERT_LINE,
        INTEGRAL,
        INTERSECTION,
        IO,
        ISO_CENTER_OBJECT,
        ISO_CONTINUOUS_UNDERLINE,
        ISO_DISCONTINUOUS_UNDERLINE,
        ISO_EMPHASIZE,
        ISO_ENTER,
        ISO_FAST_CURSOR_DOWN,
        ISO_FAST_CURSOR_LEFT,
        ISO_FAST_CURSOR_RIGHT,
        ISO_FAST_CURSOR_UP,
        ISO_FIRST_GROUP,
        ISO_FIRST_GROUP_LOCK,
        ISO_GROUP_LATCH,
        ISO_GROUP_LOCK,
        ISO_GROUP_SHIFT,
        ISO_LAST_GROUP,
        ISO_LAST_GROUP_LOCK,
        ISO_LEFT_TAB,
        ISO_LEVEL2_LATCH,
        ISO_LEVEL3_LATCH,
        ISO_LEVEL3_LOCK,
        ISO_LEVEL3_SHIFT,
        ISO_LEVEL5_LATCH,
        ISO_LEVEL5_LOCK,
        ISO_LEVEL5_SHIFT,
        ISO_LOCK,
        ISO_MOVE_LINE_DOWN,
        ISO_MOVE_LINE_UP,
        ISO_NEXT_GROUP,
        ISO_NEXT_GROUP_LOCK,
        ISO_PARTIAL_LINE_DOWN,
        ISO_PARTIAL_LINE_UP,
        ISO_PARTIAL_SPACE_LEFT,
        ISO_PARTIAL_SPACE_RIGHT,
        ISO_PREV_GROUP,
        ISO_PREV_GROUP_LOCK,
        ISO_RELEASE_BOTH_MARGINS,
        ISO_RELEASE_MARGIN_LEFT,
        ISO_RELEASE_MARGIN_RIGHT,
        ISO_SET_MARGIN_LEFT,
        ISO_SET_MARGIN_RIGHT,
        JOT,
        KANA_CHI,
        KANA_CLOSINGBRACKET,
        KANA_COMMA,
        KANA_CONJUNCTIVE,
        KANA_FU,
        KANA_FULLSTOP,
        KANA_HA,
        KANA_HE,
        KANA_HI,
        KANA_HO,
        KANA_HU,
        KANA_KA,
        KANA_KE,
        KANA_KI,
        KANA_KO,
        KANA_KU,
        KANA_LCASE_A,
        KANA_LCASE_E,
        KANA_LCASE_I,
        KANA_LCASE_O,
        KANA_LCASE_TSU,
        KANA_LCASE_TU,
        KANA_LCASE_U,
        KANA_LCASE_YA,
        KANA_LCASE_YO,
        KANA_LCASE_YU,
        KANA_LOCK,
        KANA_MA,
        KANA_ME,
        KANA_MI,
        KANA_MIDDLEDOT,
        KANA_MO,
        KANA_MU,
        KANA_N,
        KANA_NA,
        KANA_NE,
        KANA_NI,
        KANA_NO,
        KANA_NU,
        KANA_OPENINGBRACKET,
        KANA_RA,
        KANA_RE,
        KANA_RI,
        KANA_RO,
        KANA_RU,
        KANA_SA,
        KANA_SE,
        KANA_SHI,
        KANA_SHIFT,
        KANA_SO,
        KANA_SU,
        KANA_SWITCH,
        KANA_TA,
        KANA_TE,
        KANA_TI,
        KANA_TO,
        KANA_UCASE_A,
        KANA_UCASE_E,
        KANA_UCASE_I,
        KANA_UCASE_O,
        KANA_UCASE_TSU,
        KANA_UCASE_TU,
        KANA_UCASE_U,
        KANA_UCASE_YA,
        KANA_UCASE_YO,
        KANA_UCASE_YU,
        KANA_WA,
        KANA_WO,
        KANJI,
        KANJI_BANGOU,
        KAPPA,
        KATAKANA,
        KOREAN_WON,
        KP_0,
        KP_1,
        KP_2,
        KP_3,
        KP_4,
        KP_5,
        KP_6,
        KP_7,
        KP_8,
        KP_9,
        KP_ADD,
        KP_BACK_TAB,
        KP_BEGIN,
        KP_DECIMAL,
        KP_DELETE,
        KP_DIVIDE,
        KP_DOWN,
        KP_END,
        KP_ENTER,
        KP_EQUAL,
        KP_F1,
        KP_F2,
        KP_F3,
        KP_F4,
        KP_HOME,
        KP_INSERT,
        KP_LEFT,
        KP_MULTIPLY,
        KP_NEXT,
        KP_PAGE_DOWN,
        KP_PAGE_UP,
        KP_PRIOR,
        KP_RIGHT,
        KP_SEPARATOR,
        KP_SPACE,
        KP_SUBTRACT,
        KP_TAB,
        KP_UP,
        KRA,
        L1,
        L10,
        L2,
        L3,
        L4,
        L5,
        L6,
        L7,
        L8,
        L9,
        LAST_VIRTUAL_SCREEN,
        LATINCROSS,
        LCASE_A,
        LCASE_AACUTE,
        LCASE_ABELOWDOT,
        LCASE_ABREVE,
        LCASE_ABREVEACUTE,
        LCASE_ABREVEBELOWDOT,
        LCASE_ABREVEGRAVE,
        LCASE_ABREVEHOOK,
        LCASE_ABREVETILDE,
        LCASE_ACIRCUMFLEX,
        LCASE_ACIRCUMFLEXACUTE,
        LCASE_ACIRCUMFLEXBELOWDOT,
        LCASE_ACIRCUMFLEXGRAVE,
        LCASE_ACIRCUMFLEXHOOK,
        LCASE_ACIRCUMFLEXTILDE,
        LCASE_ADIAERESIS,
        LCASE_AE,
        LCASE_AGRAVE,
        LCASE_AHOOK,
        LCASE_AMACRON,
        LCASE_AOGONEK,
        LCASE_ARING,
        LCASE_ATILDE,
        LCASE_B,
        LCASE_BABOVEDOT,
        LCASE_C,
        LCASE_CABOVEDOT,
        LCASE_CACUTE,
        LCASE_CCARON,
        LCASE_CCEDILLA,
        LCASE_CCIRCUMFLEX,
        LCASE_CH,
        LCASE_C_H,
        LCASE_D,
        LCASE_DABOVEDOT,
        LCASE_DCARON,
        LCASE_DSTROKE,
        LCASE_E,
        LCASE_EABOVEDOT,
        LCASE_EACUTE,
        LCASE_EBELOWDOT,
        LCASE_ECARON,
        LCASE_ECIRCUMFLEX,
        LCASE_ECIRCUMFLEXACUTE,
        LCASE_ECIRCUMFLEXBELOWDOT,
        LCASE_ECIRCUMFLEXGRAVE,
        LCASE_ECIRCUMFLEXHOOK,
        LCASE_ECIRCUMFLEXTILDE,
        LCASE_EDIAERESIS,
        LCASE_EGRAVE,
        LCASE_EHOOK,
        LCASE_EMACRON,
        LCASE_ENG,
        LCASE_EOGONEK,
        LCASE_ETH,
        LCASE_ETILDE,
        LCASE_EZH,
        LCASE_F,
        LCASE_FABOVEDOT,
        LCASE_G,
        LCASE_GABOVEDOT,
        LCASE_GBREVE,
        LCASE_GCARON,
        LCASE_GCEDILLA,
        LCASE_GCIRCUMFLEX,
        LCASE_H,
        LCASE_HCIRCUMFLEX,
        LCASE_HSTROKE,
        LCASE_I,
        LCASE_IACUTE,
        LCASE_IBELOWDOT,
        LCASE_IBREVE,
        LCASE_ICIRCUMFLEX,
        LCASE_IDIAERESIS,
        LCASE_IGRAVE,
        LCASE_IHOOK,
        LCASE_IMACRON,
        LCASE_IOGONEK,
        LCASE_ITILDE,
        LCASE_J,
        LCASE_JCIRCUMFLEX,
        LCASE_K,
        LCASE_KCEDILLA,
        LCASE_L,
        LCASE_LACUTE,
        LCASE_LBELOWDOT,
        LCASE_LCARON,
        LCASE_LCEDILLA,
        LCASE_LSTROKE,
        LCASE_M,
        LCASE_MABOVEDOT,
        LCASE_N,
        LCASE_NACUTE,
        LCASE_NCARON,
        LCASE_NCEDILLA,
        LCASE_NTILDE,
        LCASE_O,
        LCASE_OACUTE,
        LCASE_OBARRED,
        LCASE_OBELOWDOT,
        LCASE_OCARON,
        LCASE_OCIRCUMFLEX,
        LCASE_OCIRCUMFLEXACUTE,
        LCASE_OCIRCUMFLEXBELOWDOT,
        LCASE_OCIRCUMFLEXGRAVE,
        LCASE_OCIRCUMFLEXHOOK,
        LCASE_OCIRCUMFLEXTILDE,
        LCASE_ODIAERESIS,
        LCASE_ODOUBLEACUTE,
        LCASE_OE,
        LCASE_OGRAVE,
        LCASE_OHOOK,
        LCASE_OHORN,
        LCASE_OHORNACUTE,
        LCASE_OHORNBELOWDOT,
        LCASE_OHORNGRAVE,
        LCASE_OHORNHOOK,
        LCASE_OHORNTILDE,
        LCASE_OMACRON,
        LCASE_OOBLIQUE,
        LCASE_OSLASH,
        LCASE_OTILDE,
        LCASE_P,
        LCASE_PABOVEDOT,
        LCASE_Q,
        LCASE_R,
        LCASE_RACUTE,
        LCASE_RCARON,
        LCASE_RCEDILLA,
        LCASE_S,
        LCASE_SABOVEDOT,
        LCASE_SACUTE,
        LCASE_SCARON,
        LCASE_SCEDILLA,
        LCASE_SCHWA,
        LCASE_SCIRCUMFLEX,
        LCASE_T,
        LCASE_TABOVEDOT,
        LCASE_TCARON,
        LCASE_TCEDILLA,
        LCASE_THORN,
        LCASE_TSLASH,
        LCASE_U,
        LCASE_UACUTE,
        LCASE_UBELOWDOT,
        LCASE_UBREVE,
        LCASE_UCIRCUMFLEX,
        LCASE_UDIAERESIS,
        LCASE_UDOUBLEACUTE,
        LCASE_UGRAVE,
        LCASE_UHOOK,
        LCASE_UHORN,
        LCASE_UHORNACUTE,
        LCASE_UHORNBELOWDOT,
        LCASE_UHORNGRAVE,
        LCASE_UHORNHOOK,
        LCASE_UHORNTILDE,
        LCASE_UMACRON,
        LCASE_UOGONEK,
        LCASE_URING,
        LCASE_UTILDE,
        LCASE_V,
        LCASE_W,
        LCASE_WACUTE,
        LCASE_WCIRCUMFLEX,
        LCASE_WDIAERESIS,
        LCASE_WGRAVE,
        LCASE_X,
        LCASE_XABOVEDOT,
        LCASE_Y,
        LCASE_YACUTE,
        LCASE_YBELOWDOT,
        LCASE_YCIRCUMFLEX,
        LCASE_YDIAERESIS,
        LCASE_YGRAVE,
        LCASE_YHOOK,
        LCASE_YTILDE,
        LCASE_Z,
        LCASE_ZABOVEDOT,
        LCASE_ZACUTE,
        LCASE_ZCARON,
        LCASE_ZSTROKE,
        LEFT,
        LEFTANGLEBRACKET,
        LEFTARROW,
        LEFTCARET,
        LEFTDOUBLEQUOTEMARK,
        LEFTMIDDLECURLYBRACE,
        LEFTOPENTRIANGLE,
        LEFTPOINTER,
        LEFTRADICAL,
        LEFTSHOE,
        LEFTSINGLEQUOTEMARK,
        LEFTT,
        LEFTTACK,
        LESS,
        LESSTHANEQUAL,
        LF,
        LINEFEED,
        LIRA,
        LIRA_SIGN,
        LOGICALAND,
        LOGICALOR,
        LONGMINUS,
        LOWLEFTCORNER,
        LOWRIGHTCORNER,
        MACEDONIA_LCASE_DSE,
        MACEDONIA_LCASE_GJE,
        MACEDONIA_LCASE_KJE,
        MACEDONIA_UCASE_DSE,
        MACEDONIA_UCASE_GJE,
        MACEDONIA_UCASE_KJE,
        MACRON,
        MAE_KOHO,
        MALESYMBOL,
        MALTESECROSS,
        MARKER,
        MASCULINE,
        MASSYO,
        MENU,
        META_L,
        META_R,
        MILL_SIGN,
        MINUS,
        MINUTES,
        MODE_SWITCH,
        MOUSE_KEYS_ACCEL_ENABLE,
        MOUSE_KEYS_ENABLE,
        MU,
        MUHENKAN,
        MULTIPLE_CANDIDATE,
        MULTIPLY,
        MULTI_KEY,
        MUSICALFLAT,
        MUSICALSHARP,
        MUTE_ACUTE,
        MUTE_ASCIICIRCUM,
        MUTE_ASCIITILDE,
        MUTE_DIAERESIS,
        MUTE_GRAVE,
        NABLA,
        NAIRA_SIGN,
        NEW_SHEQEL_SIGN,
        NEXT,
        NEXT_VIRTUAL_SCREEN,
        NINESUBSCRIPT,
        NINESUPERIOR,
        NL,
        NOBREAKSPACE,
        NOTAPPROXEQ,
        NOTELEMENTOF,
        NOTEQUAL,
        NOTIDENTICAL,
        NOTSIGN,
        NO_SYMBOL,
        NUMBERSIGN,
        NUMEROSIGN,
        NUM_LOCK,
        OGONEK,
        ONEEIGHTH,
        ONEFIFTH,
        ONEHALF,
        ONEQUARTER,
        ONESIXTH,
        ONESUBSCRIPT,
        ONESUPERIOR,
        ONETHIRD,
        OPENRECTBULLET,
        OPENSTAR,
        OPENTRIBULLETDOWN,
        OPENTRIBULLETUP,
        ORDFEMININE,
        OSF_ACTIVATE,
        OSF_ADD_MODE,
        OSF_BACK_SPACE,
        OSF_BACK_TAB,
        OSF_BEGIN_DATA,
        OSF_BEGIN_LINE,
        OSF_CANCEL,
        OSF_CLEAR,
        OSF_COPY,
        OSF_CUT,
        OSF_DELETE,
        OSF_DESELECT_ALL,
        OSF_DOWN,
        OSF_END_DATA,
        OSF_END_LINE,
        OSF_ESCAPE,
        OSF_EXTEND,
        OSF_HELP,
        OSF_INSERT,
        OSF_LEFT,
        OSF_MENU,
        OSF_MENU_BAR,
        OSF_NEXT_FIELD,
        OSF_NEXT_MENU,
        OSF_PAGE_DOWN,
        OSF_PAGE_LEFT,
        OSF_PAGE_RIGHT,
        OSF_PAGE_UP,
        OSF_PASTE,
        OSF_PREV_FIELD,
        OSF_PREV_MENU,
        OSF_PRIMARY_PASTE,
        OSF_QUICK_PASTE,
        OSF_RESELECT,
        OSF_RESTORE,
        OSF_RIGHT,
        OSF_SELECT,
        OSF_SELECT_ALL,
        OSF_UNDO,
        OSF_UP,
        OVERBAR,
        OVERLAY1_ENABLE,
        OVERLAY2_ENABLE,
        OVERLINE,
        PAGE_DOWN,
        PAGE_UP,
        PARAGRAPH,
        PARENLEFT,
        PARENRIGHT,
        PARTDIFFERENTIAL,
        PARTIALDERIVATIVE,
        PAUSE,
        PERCENT,
        PERIOD,
        PERIODCENTERED,
        PERMILLE,
        PESETA_SIGN,
        PHONOGRAPHCOPYRIGHT,
        PLUS,
        PLUSMINUS,
        POINTER_ACCELERATE,
        POINTER_BUTTON1,
        POINTER_BUTTON2,
        POINTER_BUTTON3,
        POINTER_BUTTON4,
        POINTER_BUTTON5,
        POINTER_BUTTON_DFLT,
        POINTER_DBL_CLICK1,
        POINTER_DBL_CLICK2,
        POINTER_DBL_CLICK3,
        POINTER_DBL_CLICK4,
        POINTER_DBL_CLICK5,
        POINTER_DBL_CLICK_DFLT,
        POINTER_DFLT_BTN_NEXT,
        POINTER_DFLT_BTN_PREV,
        POINTER_DOWN,
        POINTER_DOWN_LEFT,
        POINTER_DOWN_RIGHT,
        POINTER_DRAG1,
        POINTER_DRAG2,
        POINTER_DRAG3,
        POINTER_DRAG4,
        POINTER_DRAG5,
        POINTER_DRAG_DFLT,
        POINTER_ENABLE_KEYS,
        POINTER_LEFT,
        POINTER_RIGHT,
        POINTER_UP,
        POINTER_UP_LEFT,
        POINTER_UP_RIGHT,
        PRESCRIPTION,
        PREVIOUS_CANDIDATE,
        PREV_VIRTUAL_SCREEN,
        PRINT,
        PRIOR,
        PROLONGEDSOUND,
        PUNCTSPACE,
        QUAD,
        QUESTION,
        QUESTIONDOWN,
        QUOTEDBL,
        QUOTELEFT,
        QUOTERIGHT,
        R1,
        R10,
        R11,
        R12,
        R13,
        R14,
        R15,
        R2,
        R3,
        R4,
        R5,
        R6,
        R7,
        R8,
        R9,
        RADICAL,
        REDO,
        REGISTERED,
        REPEAT_KEYS_ENABLE,
        RESET,
        RETURN,
        RIGHT,
        RIGHTANGLEBRACKET,
        RIGHTARROW,
        RIGHTCARET,
        RIGHTDOUBLEQUOTEMARK,
        RIGHTMIDDLECURLYBRACE,
        RIGHTMIDDLESUMMATION,
        RIGHTOPENTRIANGLE,
        RIGHTPOINTER,
        RIGHTSHOE,
        RIGHTSINGLEQUOTEMARK,
        RIGHTT,
        RIGHTTACK,
        ROMAJI,
        RUPEE_SIGN,
        SCRIPT_SWITCH,
        SCROLL_LOCK,
        SECONDS,
        SECTION,
        SELECT,
        SEMICOLON,
        SEMIVOICEDSOUND,
        SERBIAN_LCASE_DJE,
        SERBIAN_LCASE_DZE,
        SERBIAN_LCASE_JE,
        SERBIAN_LCASE_LJE,
        SERBIAN_LCASE_NJE,
        SERBIAN_LCASE_TSHE,
        SERBIAN_UCASE_DJE,
        SERBIAN_UCASE_DZE,
        SERBIAN_UCASE_JE,
        SERBIAN_UCASE_LJE,
        SERBIAN_UCASE_NJE,
        SERBIAN_UCASE_TSHE,
        SEVENEIGHTHS,
        SEVENSUBSCRIPT,
        SEVENSUPERIOR,
        SHIFT_L,
        SHIFT_LOCK,
        SHIFT_R,
        SIGNATUREMARK,
        SIGNIFBLANK,
        SIMILAREQUAL,
        SINGLELOWQUOTEMARK,
        SINGLE_CANDIDATE,
        SINH_A,
        SINH_AA,
        SINH_AA2,
        SINH_AE,
        SINH_AE2,
        SINH_AEE,
        SINH_AEE2,
        SINH_AI,
        SINH_AI2,
        SINH_AL,
        SINH_AU,
        SINH_AU2,
        SINH_BA,
        SINH_BHA,
        SINH_CA,
        SINH_CHA,
        SINH_DDA,
        SINH_DDHA,
        SINH_DHA,
        SINH_DHHA,
        SINH_E,
        SINH_E2,
        SINH_EE,
        SINH_EE2,
        SINH_FA,
        SINH_GA,
        SINH_GHA,
        SINH_H2,
        SINH_HA,
        SINH_I,
        SINH_I2,
        SINH_II,
        SINH_II2,
        SINH_JA,
        SINH_JHA,
        SINH_JNYA,
        SINH_KA,
        SINH_KHA,
        SINH_KUNDDALIYA,
        SINH_LA,
        SINH_LLA,
        SINH_LU,
        SINH_LU2,
        SINH_LUU,
        SINH_LUU2,
        SINH_MA,
        SINH_MBA,
        SINH_NA,
        SINH_NDDA,
        SINH_NDHA,
        SINH_NG,
        SINH_NG2,
        SINH_NGA,
        SINH_NJA,
        SINH_NNA,
        SINH_NYA,
        SINH_O,
        SINH_O2,
        SINH_OO,
        SINH_OO2,
        SINH_PA,
        SINH_PHA,
        SINH_RA,
        SINH_RI,
        SINH_RII,
        SINH_RU2,
        SINH_RUU2,
        SINH_SA,
        SINH_SHA,
        SINH_SSHA,
        SINH_THA,
        SINH_THHA,
        SINH_TTA,
        SINH_TTHA,
        SINH_U,
        SINH_U2,
        SINH_UU,
        SINH_UU2,
        SINH_VA,
        SINH_YA,
        SIXSUBSCRIPT,
        SIXSUPERIOR,
        SLASH,
        SLOW_KEYS_ENABLE,
        SOLIDDIAMOND,
        SPACE,
        SQUAREROOT,
        SSHARP,
        STERLING,
        STICKY_KEYS_ENABLE,
        STRICTEQ,
        SUN_AGAIN,
        SUN_ALT_GRAPH,
        SUN_AUDIO_LOWER_VOLUME,
        SUN_AUDIO_MUTE,
        SUN_AUDIO_RAISE_VOLUME,
        SUN_COMPOSE,
        SUN_COPY,
        SUN_CUT,
        SUN_F36,
        SUN_F37,
        SUN_FA_ACUTE,
        SUN_FA_CEDILLA,
        SUN_FA_CIRCUM,
        SUN_FA_DIAERESIS,
        SUN_FA_GRAVE,
        SUN_FA_TILDE,
        SUN_FIND,
        SUN_FRONT,
        SUN_OPEN,
        SUN_PAGE_DOWN,
        SUN_PAGE_UP,
        SUN_PASTE,
        SUN_POWER_SWITCH,
        SUN_POWER_SWITCH_SHIFT,
        SUN_PRINT_SCREEN,
        SUN_PROPS,
        SUN_STOP,
        SUN_SYS_REQ,
        SUN_UNDO,
        SUN_VIDEO_DEGAUSS,
        SUN_VIDEO_LOWER_BRIGHTNESS,
        SUN_VIDEO_RAISE_BRIGHTNESS,
        SUPER_L,
        SUPER_R,
        SYSTEM,
        SYS_REQ,
        T3270_ALT_CURSOR,
        T3270_ATTN,
        T3270_BACK_TAB,
        T3270_CHANGE_SCREEN,
        T3270_COPY,
        T3270_CURSOR_BLINK,
        T3270_CURSOR_SELECT,
        T3270_DELETE_WORD,
        T3270_DUPLICATE,
        T3270_ENTER,
        T3270_ERASE_EOF,
        T3270_ERASE_INPUT,
        T3270_EX_SELECT,
        T3270_FIELD_MARK,
        T3270_IDENT,
        T3270_JUMP,
        T3270_KEY_CLICK,
        T3270_LEFT2,
        T3270_PA1,
        T3270_PA2,
        T3270_PA3,
        T3270_PLAY,
        T3270_PRINT_SCREEN,
        T3270_QUIT,
        T3270_RECORD,
        T3270_RESET,
        T3270_RIGHT2,
        T3270_RULE,
        T3270_SETUP,
        T3270_TEST,
        TAB,
        TCASE_CH,
        TCASE_C_H,
        TCASE_ETH,
        TCASE_THORN,
        TELEPHONE,
        TELEPHONERECORDER,
        TERMINATE_SERVER,
        THAI_BAHT,
        THAI_BOBAIMAI,
        THAI_CHOCHAN,
        THAI_CHOCHANG,
        THAI_CHOCHING,
        THAI_CHOCHOE,
        THAI_DOCHADA,
        THAI_DODEK,
        THAI_FOFA,
        THAI_FOFAN,
        THAI_HOHIP,
        THAI_HONOKHUK,
        THAI_KHOKHAI,
        THAI_KHOKHON,
        THAI_KHOKHUAT,
        THAI_KHOKHWAI,
        THAI_KHORAKHANG,
        THAI_KOKAI,
        THAI_LAKKHANGYAO,
        THAI_LEKCHET,
        THAI_LEKHA,
        THAI_LEKHOK,
        THAI_LEKKAO,
        THAI_LEKNUNG,
        THAI_LEKPAET,
        THAI_LEKSAM,
        THAI_LEKSI,
        THAI_LEKSONG,
        THAI_LEKSUN,
        THAI_LOCHULA,
        THAI_LOLING,
        THAI_LU,
        THAI_MAICHATTAWA,
        THAI_MAIEK,
        THAI_MAIHANAKAT,
        THAI_MAIHANAKAT_MAITHO,
        THAI_MAITAIKHU,
        THAI_MAITHO,
        THAI_MAITRI,
        THAI_MAIYAMOK,
        THAI_MOMA,
        THAI_NGONGU,
        THAI_NIKHAHIT,
        THAI_NONEN,
        THAI_NONU,
        THAI_OANG,
        THAI_PAIYANNOI,
        THAI_PHINTHU,
        THAI_PHOPHAN,
        THAI_PHOPHUNG,
        THAI_PHOSAMPHAO,
        THAI_POPLA,
        THAI_RORUA,
        THAI_RU,
        THAI_SARAA,
        THAI_SARAAA,
        THAI_SARAAE,
        THAI_SARAAIMAIMALAI,
        THAI_SARAAIMAIMUAN,
        THAI_SARAAM,
        THAI_SARAE,
        THAI_SARAI,
        THAI_SARAII,
        THAI_SARAO,
        THAI_SARAU,
        THAI_SARAUE,
        THAI_SARAUEE,
        THAI_SARAUU,
        THAI_SORUSI,
        THAI_SOSALA,
        THAI_SOSO,
        THAI_SOSUA,
        THAI_THANTHAKHAT,
        THAI_THONANGMONTHO,
        THAI_THOPHUTHAO,
        THAI_THOTHAHAN,
        THAI_THOTHAN,
        THAI_THOTHONG,
        THAI_THOTHUNG,
        THAI_TOPATAK,
        THAI_TOTAO,
        THAI_WOWAEN,
        THAI_YOYAK,
        THAI_YOYING,
        THEREFORE,
        THINSPACE,
        THREEEIGHTHS,
        THREEFIFTHS,
        THREEQUARTERS,
        THREESUBSCRIPT,
        THREESUPERIOR,
        TINTEGRAL,
        TOPINTEGRAL,
        TOPLEFTPARENS,
        TOPLEFTRADICAL,
        TOPLEFTSQBRACKET,
        TOPLEFTSUMMATION,
        TOPRIGHTPARENS,
        TOPRIGHTSQBRACKET,
        TOPRIGHTSUMMATION,
        TOPT,
        TOPVERTSUMMATIONCONNECTOR,
        TOUROKU,
        TRADEMARK,
        TRADEMARKINCIRCLE,
        TWOFIFTHS,
        TWOSUBSCRIPT,
        TWOSUPERIOR,
        TWOTHIRDS,
        UCASE_A,
        UCASE_AACUTE,
        UCASE_ABELOWDOT,
        UCASE_ABREVE,
        UCASE_ABREVEACUTE,
        UCASE_ABREVEBELOWDOT,
        UCASE_ABREVEGRAVE,
        UCASE_ABREVEHOOK,
        UCASE_ABREVETILDE,
        UCASE_ACIRCUMFLEX,
        UCASE_ACIRCUMFLEXACUTE,
        UCASE_ACIRCUMFLEXBELOWDOT,
        UCASE_ACIRCUMFLEXGRAVE,
        UCASE_ACIRCUMFLEXHOOK,
        UCASE_ACIRCUMFLEXTILDE,
        UCASE_ADIAERESIS,
        UCASE_AE,
        UCASE_AGRAVE,
        UCASE_AHOOK,
        UCASE_AMACRON,
        UCASE_AOGONEK,
        UCASE_ARING,
        UCASE_ATILDE,
        UCASE_B,
        UCASE_BABOVEDOT,
        UCASE_C,
        UCASE_CABOVEDOT,
        UCASE_CACUTE,
        UCASE_CCARON,
        UCASE_CCEDILLA,
        UCASE_CCIRCUMFLEX,
        UCASE_CH,
        UCASE_C_H,
        UCASE_D,
        UCASE_DABOVEDOT,
        UCASE_DCARON,
        UCASE_DSTROKE,
        UCASE_E,
        UCASE_EABOVEDOT,
        UCASE_EACUTE,
        UCASE_EBELOWDOT,
        UCASE_ECARON,
        UCASE_ECIRCUMFLEX,
        UCASE_ECIRCUMFLEXACUTE,
        UCASE_ECIRCUMFLEXBELOWDOT,
        UCASE_ECIRCUMFLEXGRAVE,
        UCASE_ECIRCUMFLEXHOOK,
        UCASE_ECIRCUMFLEXTILDE,
        UCASE_EDIAERESIS,
        UCASE_EGRAVE,
        UCASE_EHOOK,
        UCASE_EMACRON,
        UCASE_ENG,
        UCASE_EOGONEK,
        UCASE_ETH,
        UCASE_ETILDE,
        UCASE_EZH,
        UCASE_F,
        UCASE_FABOVEDOT,
        UCASE_G,
        UCASE_GABOVEDOT,
        UCASE_GBREVE,
        UCASE_GCARON,
        UCASE_GCEDILLA,
        UCASE_GCIRCUMFLEX,
        UCASE_H,
        UCASE_HCIRCUMFLEX,
        UCASE_HSTROKE,
        UCASE_I,
        UCASE_IACUTE,
        UCASE_IBELOWDOT,
        UCASE_IBREVE,
        UCASE_ICIRCUMFLEX,
        UCASE_IDIAERESIS,
        UCASE_IGRAVE,
        UCASE_IHOOK,
        UCASE_IMACRON,
        UCASE_IOGONEK,
        UCASE_ITILDE,
        UCASE_J,
        UCASE_JCIRCUMFLEX,
        UCASE_K,
        UCASE_KCEDILLA,
        UCASE_L,
        UCASE_LACUTE,
        UCASE_LBELOWDOT,
        UCASE_LCARON,
        UCASE_LCEDILLA,
        UCASE_LSTROKE,
        UCASE_M,
        UCASE_MABOVEDOT,
        UCASE_N,
        UCASE_NACUTE,
        UCASE_NCARON,
        UCASE_NCEDILLA,
        UCASE_NTILDE,
        UCASE_O,
        UCASE_OACUTE,
        UCASE_OBARRED,
        UCASE_OBELOWDOT,
        UCASE_OCARON,
        UCASE_OCIRCUMFLEX,
        UCASE_OCIRCUMFLEXACUTE,
        UCASE_OCIRCUMFLEXBELOWDOT,
        UCASE_OCIRCUMFLEXGRAVE,
        UCASE_OCIRCUMFLEXHOOK,
        UCASE_OCIRCUMFLEXTILDE,
        UCASE_ODIAERESIS,
        UCASE_ODOUBLEACUTE,
        UCASE_OE,
        UCASE_OGRAVE,
        UCASE_OHOOK,
        UCASE_OHORN,
        UCASE_OHORNACUTE,
        UCASE_OHORNBELOWDOT,
        UCASE_OHORNGRAVE,
        UCASE_OHORNHOOK,
        UCASE_OHORNTILDE,
        UCASE_OMACRON,
        UCASE_OOBLIQUE,
        UCASE_OSLASH,
        UCASE_OTILDE,
        UCASE_P,
        UCASE_PABOVEDOT,
        UCASE_Q,
        UCASE_R,
        UCASE_RACUTE,
        UCASE_RCARON,
        UCASE_RCEDILLA,
        UCASE_S,
        UCASE_SABOVEDOT,
        UCASE_SACUTE,
        UCASE_SCARON,
        UCASE_SCEDILLA,
        UCASE_SCHWA,
        UCASE_SCIRCUMFLEX,
        UCASE_T,
        UCASE_TABOVEDOT,
        UCASE_TCARON,
        UCASE_TCEDILLA,
        UCASE_THORN,
        UCASE_TSLASH,
        UCASE_U,
        UCASE_UACUTE,
        UCASE_UBELOWDOT,
        UCASE_UBREVE,
        UCASE_UCIRCUMFLEX,
        UCASE_UDIAERESIS,
        UCASE_UDOUBLEACUTE,
        UCASE_UGRAVE,
        UCASE_UHOOK,
        UCASE_UHORN,
        UCASE_UHORNACUTE,
        UCASE_UHORNBELOWDOT,
        UCASE_UHORNGRAVE,
        UCASE_UHORNHOOK,
        UCASE_UHORNTILDE,
        UCASE_UMACRON,
        UCASE_UOGONEK,
        UCASE_URING,
        UCASE_UTILDE,
        UCASE_V,
        UCASE_W,
        UCASE_WACUTE,
        UCASE_WCIRCUMFLEX,
        UCASE_WDIAERESIS,
        UCASE_WGRAVE,
        UCASE_X,
        UCASE_XABOVEDOT,
        UCASE_Y,
        UCASE_YACUTE,
        UCASE_YBELOWDOT,
        UCASE_YCIRCUMFLEX,
        UCASE_YDIAERESIS,
        UCASE_YGRAVE,
        UCASE_YHOOK,
        UCASE_YTILDE,
        UCASE_Z,
        UCASE_ZABOVEDOT,
        UCASE_ZACUTE,
        UCASE_ZCARON,
        UCASE_ZSTROKE,
        UKRAINIAN_LCASE_GHE_WITH_UPTURN,
        UKRAINIAN_LCASE_I,
        UKRAINIAN_LCASE_IE,
        UKRAINIAN_LCASE_YI,
        UKRAINIAN_UCASE_GHE_WITH_UPTURN,
        UKRAINIAN_UCASE_I,
        UKRAINIAN_UCASE_IE,
        UKRAINIAN_UCASE_YI,
        UKRANIAN_LCASE_I,
        UKRANIAN_LCASE_JE,
        UKRANIAN_LCASE_YI,
        UKRANIAN_UCASE_I,
        UKRANIAN_UCASE_JE,
        UKRANIAN_UCASE_YI,
        UNDERBAR,
        UNDERSCORE,
        UNDO,
        UNION,
        UP,
        UPARROW,
        UPCARET,
        UPLEFTCORNER,
        UPRIGHTCORNER,
        UPSHOE,
        UPSTILE,
        UPTACK,
        USER,
        VARIATION,
        VERTBAR,
        VERTCONNECTOR,
        VOICEDSOUND,
        VOID_SYMBOL,
        VT,
        WON_SIGN,
        XF86I_TOUCH,
        XF86_ADD_FAVORITE,
        XF86_APPLICATION_LEFT,
        XF86_APPLICATION_RIGHT,
        XF86_AUDIO_CYCLE_TRACK,
        XF86_AUDIO_FORWARD,
        XF86_AUDIO_LOWER_VOLUME,
        XF86_AUDIO_MEDIA,
        XF86_AUDIO_MIC_MUTE,
        XF86_AUDIO_MUTE,
        XF86_AUDIO_NEXT,
        XF86_AUDIO_PAUSE,
        XF86_AUDIO_PLAY,
        XF86_AUDIO_PREV,
        XF86_AUDIO_RAISE_VOLUME,
        XF86_AUDIO_RANDOM_PLAY,
        XF86_AUDIO_RECORD,
        XF86_AUDIO_REPEAT,
        XF86_AUDIO_REWIND,
        XF86_AUDIO_STOP,
        XF86_AWAY,
        XF86_BACK,
        XF86_BACK_FORWARD,
        XF86_BATTERY,
        XF86_BLUE,
        XF86_BLUETOOTH,
        XF86_BOOK,
        XF86_BRIGHTNESS_ADJUST,
        XF86_CALCULATER,
        XF86_CALCULATOR,
        XF86_CALENDAR,
        XF86_CD,
        XF86_CLEAR,
        XF86_CLEAR_GRAB,
        XF86_CLOSE,
        XF86_COMMUNITY,
        XF86_CONTRAST_ADJUST,
        XF86_COPY,
        XF86_CUT,
        XF86_CYCLE_ANGLE,
        XF86_DISPLAY,
        XF86_DOCUMENTS,
        XF86_DOS,
        XF86_EJECT,
        XF86_EXCEL,
        XF86_EXPLORER,
        XF86_FAVORITES,
        XF86_FINANCE,
        XF86_FORWARD,
        XF86_FRAME_BACK,
        XF86_FRAME_FORWARD,
        XF86_GAME,
        XF86_GO,
        XF86_GREEN,
        XF86_HIBERNATE,
        XF86_HISTORY,
        XF86_HOME_PAGE,
        XF86_HOT_LINKS,
        XF86_KBD_BRIGHTNESS_DOWN,
        XF86_KBD_BRIGHTNESS_UP,
        XF86_KBD_LIGHT_ON_OFF,
        XF86_LAUNCH0,
        XF86_LAUNCH1,
        XF86_LAUNCH2,
        XF86_LAUNCH3,
        XF86_LAUNCH4,
        XF86_LAUNCH5,
        XF86_LAUNCH6,
        XF86_LAUNCH7,
        XF86_LAUNCH8,
        XF86_LAUNCH9,
        XF86_LAUNCH_A,
        XF86_LAUNCH_B,
        XF86_LAUNCH_C,
        XF86_LAUNCH_D,
        XF86_LAUNCH_E,
        XF86_LAUNCH_F,
        XF86_LIGHT_BULB,
        XF86_LOG_GRAB_INFO,
        XF86_LOG_OFF,
        XF86_LOG_WINDOW_TREE,
        XF86_MAIL,
        XF86_MAIL_FORWARD,
        XF86_MARKET,
        XF86_MEETING,
        XF86_MEMO,
        XF86_MENU_KB,
        XF86_MENU_PB,
        XF86_MESSENGER,
        XF86_MODE_LOCK,
        XF86_MON_BRIGHTNESS_DOWN,
        XF86_MON_BRIGHTNESS_UP,
        XF86_MUSIC,
        XF86_MY_COMPUTER,
        XF86_MY_SITES,
        XF86_NEW,
        XF86_NEWS,
        XF86_NEXT_VMODE,
        XF86_OFFICE_HOME,
        XF86_OPEN,
        XF86_OPEN_URL,
        XF86_OPTION,
        XF86_PASTE,
        XF86_PHONE,
        XF86_PICTURES,
        XF86_POWER_DOWN,
        XF86_POWER_OFF,
        XF86_PREV_VMODE,
        XF86_Q,
        XF86_RED,
        XF86_REFRESH,
        XF86_RELOAD,
        XF86_REPLY,
        XF86_ROCKER_DOWN,
        XF86_ROCKER_ENTER,
        XF86_ROCKER_UP,
        XF86_ROTATE_WINDOWS,
        XF86_ROTATION_KB,
        XF86_ROTATION_PB,
        XF86_SAVE,
        XF86_SCREEN_SAVER,
        XF86_SCROLL_CLICK,
        XF86_SCROLL_DOWN,
        XF86_SCROLL_UP,
        XF86_SEARCH,
        XF86_SELECT,
        XF86_SEND,
        XF86_SHOP,
        XF86_SLEEP,
        XF86_SPELL,
        XF86_SPLIT_SCREEN,
        XF86_STANDBY,
        XF86_START,
        XF86_STOP,
        XF86_SUBTITLE,
        XF86_SUPPORT,
        XF86_SUSPEND,
        XF86_SWITCH_VT_1,
        XF86_SWITCH_VT_10,
        XF86_SWITCH_VT_11,
        XF86_SWITCH_VT_12,
        XF86_SWITCH_VT_2,
        XF86_SWITCH_VT_3,
        XF86_SWITCH_VT_4,
        XF86_SWITCH_VT_5,
        XF86_SWITCH_VT_6,
        XF86_SWITCH_VT_7,
        XF86_SWITCH_VT_8,
        XF86_SWITCH_VT_9,
        XF86_TASK_PANE,
        XF86_TERMINAL,
        XF86_TIME,
        XF86_TOOLS,
        XF86_TOP_MENU,
        XF86_TOUCHPAD_OFF,
        XF86_TOUCHPAD_ON,
        XF86_TOUCHPAD_TOGGLE,
        XF86_TO_DO_LIST,
        XF86_TRAVEL,
        XF86_UNGRAB,
        XF86_USER1_KB,
        XF86_USER2_KB,
        XF86_USER_PB,
        XF86_UWB,
        XF86_VENDOR_HOME,
        XF86_VIDEO,
        XF86_VIEW,
        XF86_WAKE_UP,
        XF86_WEB_CAM,
        XF86_WHEEL_BUTTON,
        XF86_WLAN,
        XF86_WORD,
        XF86_WWW,
        XF86_XFER,
        XF86_YELLOW,
        XF86_ZOOM_IN,
        XF86_ZOOM_OUT,
        YEN,
        ZENKAKU,
        ZENKAKU_HANKAKU,
        ZEN_KOHO,
        ZEROSUBSCRIPT,
        ZEROSUPERIOR,
    }
}
