/**
 ** fdv_raw.h -- driver for raw font file format
 **
 ** Copyright (C) 2002 Dimitar Zhekov
 ** [e-mail: jimmy@is-vn.bg]
 **
 ** This file is part of the GRX graphics library.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 **/

#ifndef __FDV_RAW_H_INCLUDED__
#define __FDV_RAW_H_INCLUDED__

#ifdef __GNUC__
#define PACKED __attribute__((packed))
#else
#define PACKED
#endif

#define PSF_FONTMAGIC1  0x36
#define PSF_FONTMAGIC2  0x04

#define PSF_512_CHARACTERS 0x01
#define PSF_UNICODE_DATA 0x02
#define PSF_FILEMODE_MAX (PSF_512_CHARACTERS | PSF_UNICODE_DATA)

typedef struct _GR_fontFileHeaderPSF {  /* the header */
    GR_int8u magic1;
    GR_int8u magic2;
    GR_int8u filemode;
    GR_int8u fontheight;
} PACKED GrFontFileHeaderPSF;

#endif
