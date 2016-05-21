/*
 * text.h
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

#ifndef __GRX_TEXT_H__
#define __GRX_TEXT_H__

#include <glib.h>
#include <glib-object.h>

#include <grx/color.h>
#include <grx/common.h>

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
#define GRX_UNDERLINE_TEXT       (GRX_COLOR_MODE_XOR << 4)

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
    GrxFontConversionFlags modified;/* "tweaked" font (resized, etc..) */
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
    guint     ref_count;            /* private reference count */
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

GType grx_font_get_type(void);

GrxFont *grx_font_load(gchar *name);
GrxFont *grx_font_load_converted(gchar *name, GrxFontConversionFlags cvt,
                                 gint w, gint h, gint minch, gint maxch);
GrxFont *grx_font_build_converted(const GrxFont *from, GrxFontConversionFlags cvt,
                                  gint w, gint h, gint minch, gint maxch);

GrxFont *grx_font_ref(GrxFont *font);
void grx_font_unref(GrxFont *font);
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

struct _GrxTextOption {                   /* text drawing option structure */
    GrxFont                 *txo_font;    /* font to be used */
    GrxTextColor             txo_fgcolor; /* foreground color */
    GrxTextColor             txo_bgcolor; /* background color */
    GrxCharType              txo_chrtype; /* character type */
    GrxTextDirection         txo_direct;  /* direction */
    GrxTextAlignment         txo_xalign;  /* X alignment */
    GrxTextVerticalAlignment txo_yalign;  /* Y alignment */
};

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

#endif /* __GRX_TEXT_H__ */
