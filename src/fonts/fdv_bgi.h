/**
 ** FDV_BGI.H -- Borland BGI font file format
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#ifndef __FDV_BGI_H_INCLUDED__
#define __FDV_BGI_H_INCLUDED__

/* This is based on the font code in Hartmut Schirmer's BCC2GRX package */

#ifdef         __GNUC__
#define PACKED        __attribute ((packed))
#else 
#define PACKED
#endif

/* 
 * Stroked BGI font file layout:
 *
 * +-------------------------------------------------------------------+
 * | ASCII copyright message, starts with "PK\b\b", terminated with ^Z |
 * +-------------------------------------------------------------------+
 * | BGIfontFileHeader structure                                       |
 * +-------------------------------------------------------------------+
 * | BGIfontHeaderType structure (at offset 'header_size')               |
 * +-------------------------------------------------------------------+
 * | table of ushort offsets into the data table                       |
 * +-------------------------------------------------------------------+
 * | table of uchar character widths                                       |
 * +-------------------------------------------------------------------+
 * | data table containing stroke vector coordinates and commands      |
 * +-------------------------------------------------------------------+
 */

typedef struct {
    ushort  header_size  PACKED;         /* Version 2.0 Header Format */
    char    font_name[4] PACKED;        /* Font Internal Name */
    ushort  font_size    PACKED;             /* file size in bytes */
    uchar   font_major   PACKED;          /* Driver Version Information */
    uchar   font_minor   PACKED;
    uchar   bgi_major    PACKED;             /* BGI Revision Information */
    uchar   bgi_minor    PACKED;
} BGIfontFileHeader;
                                      
typedef struct {
    char    sig          PACKED;               /* SIGNATURE byte */
    ushort  nchrs        PACKED;        /* number of characters in file */
    char    unused1      PACKED;        /* Currently Undefined */
    uchar   firstch      PACKED;        /* first character in file */
    ushort  cdefs        PACKED;        /* offset to char definitions */
    uchar   scan_flag    PACKED;        /* (!= 0) -> set is scanable */
    char    org_to_cap   PACKED;        /* Height from origin to top of capitol */
    char    org_to_base  PACKED;        /* Height from origin to baseline */
    char    org_to_dec   PACKED;        /* Height from origin to bot of decender */
    char    unused2[5]   PACKED;        /* Currently undefined */
} BGIfontHeaderType;

#define FILEMARKER        "PK\b\b"        /* file should start with this */
#define MARKEREND        ('Z' - 0x40)        /* end of copyright message */
#define SIGBYTE                ('+')                /* value of signature byte */

/* 
 * macros to parse data words in the stroke vector table 
 */
#define SV_COMMAND(w)        ((w) & 0x8080U)
#define SV_XCOORD(w)        ((int)(signed char)((w) << 1) >> 1)
#define SV_YCOORD(w)        ((int)(signed char)((w) >> 7) >> 1)

#define SVC_END                0x0000U
#define SVC_MOVE        0x0080U
#define SVC_SCAN        0x8000U
#define SVC_DRAW        0x8080U

#endif        /* whole file */
