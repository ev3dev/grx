/**
 ** FDV_GRX.H -- GRX native font file format
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#ifndef __FDV_GRX_H_INCLUDED__
#define __FDV_GRX_H_INCLUDED__

/*
 *  font file structure:
 *  +-----------------------+
 *  |          FILE HEADER            |
 *  +-----------------------+
 *  |          PROPORTIONAL            |
 *  |          WIDTH TABLE            |
 *  |          (16 bit ints)            |
 *  |          (prop fonts only) |
 *  +-----------------------+
 *  |          BITMAP            |
 *  +-----------------------+
 */

#define GRX_NAMEWIDTH        16
#define GRX_FONTMAGIC        0x19590214L

typedef struct _GR_fontFileHeaderGRX {  /* the header */
    unsigned long  magic;                /* font file magic number */
    unsigned long  bmpsize;                /* character bitmap size */
    unsigned short width;                /* width (average for proportional) */
    unsigned short height;                /* font height */
    unsigned short minchar;                /* lowest character code in font */
    unsigned short maxchar;                /* highest character code in font */
    unsigned short isfixed;                /* nonzero if fixed font */
    unsigned short reserved;                /* ??? */
    unsigned short baseline;                /* baseline from top of font */
    unsigned short undwidth;                /* underline width (at bottom) */
    char     fnname[GRX_NAMEWIDTH];        /* font file name (w/o path) */
    char     family[GRX_NAMEWIDTH];        /* font family name */
} GrFontFileHeaderGRX;

#endif /* whole file */

