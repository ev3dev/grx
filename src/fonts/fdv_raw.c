/**
 ** fdv_raw.c -- driver for raw font file format
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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libgrx.h"
#include "grfontdv.h"
#include "arith.h"
#include "fonts/fdv_raw.h"

#ifndef SEEK_SET
#define SEEK_SET        0
#endif

#ifndef SEEK_END
#define SEEK_END        2
#endif

static FILE *fontfp = NULL;
static int   nextch = 0;
static char  name[40];
static char  family[40];
static long  offset;
static int   height;
static int   numchars;

static void cleanup(void)
{
        GRX_ENTER();
        if(fontfp != NULL) fclose(fontfp);
        fontfp = NULL;
        nextch = 0;
        GRX_LEAVE();
}

static int openfile(char *fname)
{
        int res;
        long size;
        GrFontFileHeaderPSF fhdr;
        char *s;
        GRX_ENTER();
        res = FALSE;
        cleanup();
        /* open fname and read header */
        fontfp = fopen(fname, "rb");
        if(fontfp == NULL) {
            DBGPRINTF(DBG_FONT, ("fopen(\"%s\") failed\n", fname));
            goto done;
        }
        if(fread(&fhdr, 1, sizeof fhdr, fontfp) != sizeof fhdr) {
            DBGPRINTF(DBG_FONT, ("read header failed\n"));
            goto done;
        }
        if(fseek(fontfp, 0, SEEK_END) < 0) {
            DBGPRINTF(DBG_FONT, ("seek to end of file failed\n"));
            goto done;
        }
        size = ftell(fontfp);
        if(size < 0) {
            DBGPRINTF(DBG_FONT, ("tell file position failed\n"));
            goto done;
        }
        /* try to guess file type */
        if(fhdr.magic1 == PSF_FONTMAGIC1 && fhdr.magic2 == PSF_FONTMAGIC2 && fhdr.filemode <= PSF_FILEMODE_MAX) {
            offset = sizeof fhdr;
            height = fhdr.fontheight;
            numchars = (fhdr.filemode & PSF_512_CHARACTERS) == 0 ? 256 : 512;
            if(height == 0) {
                DBGPRINTF(DBG_FONT, ("invalid psf font height"));
                goto done;
            }
            if(size - offset < height * numchars) {
                DBGPRINTF(DBG_FONT, ("invalid psf file size\n"));
                goto done;
            }
        }
        else {
            if(size % 256) {
                DBGPRINTF(DBG_FONT, ("invalid raw file size\n"));
                goto done;
            }
            if(size / 256 > 255) {
                DBGPRINTF(DBG_FONT, ("invalid raw font height\n"));
                goto done;
            }
            offset = 0;
            height = size / 256;
            numchars = 256;
        }
        /* get font name and family */
        s = strrchr(fname, '/');
#if defined(__MSDOS__) || defined(__WIN32__)
        if(s == NULL) {
            s = strrchr(fname, '\\');
            if(s == NULL) s = strrchr(fname, ':');
        }
        else if(strrchr(s, '\\') != NULL) s = strrchr(s, '\\');
#endif
        if(s == NULL || *++s == '\0') s = fname;
        strncpy(name, s, sizeof name - 1);
        name[sizeof name - 1] = '\0';
        if((s = strrchr(name, '.')) != NULL) *s = '\0';
        if(*name == '\0') sprintf(name, offset != 0 ? "psf%d" : "raw%d", height);
        strcpy(family, name);
        for(s = family; isalpha(*s); s++);
        if(s > family) *s = '\0';
        /* finish and return */
        nextch = numchars;
        res = TRUE;
done:   if(!res) cleanup();
        GRX_RETURN(res);
}

static int header(GrFontHeader *hdr)
{
        int res;
        GRX_ENTER();
        res = FALSE;
        if(fontfp != NULL) {
            strcpy(hdr->name, name);
            strcpy(hdr->family, family);
            hdr->proportional = FALSE;
            hdr->scalable = FALSE;
            hdr->preloaded = FALSE;
            hdr->modified = GR_FONTCVT_NONE;
            hdr->width = 8;
            hdr->height = height;
            hdr->baseline = (hdr->height * 4) / 5 + (hdr->height < 10);
            hdr->ulheight = imax(1, hdr->height / 15);
            hdr->ulpos = hdr->height - hdr->ulheight;
            hdr->minchar = 0;
            hdr->numchars = numchars;
            res = TRUE;
        }
        GRX_RETURN(res);
}

static int charwdt(int chr)
{
        int res;
        GRX_ENTER();
        res = -1;
        if(fontfp != NULL && chr >= 0 && chr < numchars) res = 8;
        GRX_RETURN(res);
}

static int bitmap(int chr,int w,int h,char *buffer)
{
        int res;
        GRX_ENTER();
        res = FALSE;
        if(w != charwdt(chr) || h != height) goto done;
        if(chr != nextch && fseek(fontfp, offset + height * chr, SEEK_SET) < 0) goto done;
        if(fread(buffer, 1, height, fontfp) != height) goto done;
        nextch = chr + 1;
        res = TRUE;
done:        GRX_RETURN(res);
}

GrFontDriver _GrFontDriverRAW = {
    "RAW",                              /* driver name (doc only) */
    ".raw",                             /* font file extension */
    FALSE,                              /* scalable */
    openfile,                           /* file open and check routine */
    header,                             /* font header reader routine */
    charwdt,                            /* character width reader routine */
    bitmap,                             /* character bitmap reader routine */
    cleanup                             /* cleanup routine */
};
