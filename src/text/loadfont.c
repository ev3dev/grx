/*
 * loadfont.c ---- load a font from a disk file
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
 *
 */

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "libgrx.h"
#include "grfontdv.h"

static GrxFont *doit(char *fname,char *path,GrxFontConversionFlags cvt,int w,int h,int lo,int hi)
{
    GrFontDriver **fd;
    GrxFontHeader hdr;
    GrxFont *f, *res;
    char pathname[200];
    char tempstring[200];
    int  plen;
    GRX_ENTER();
    res = NULL;
    strcpy(pathname,path);
    strcat(pathname,fname);
    DBGPRINTF(DBG_FONT,("searching font %s\n", pathname));
    plen = strlen(pathname);
    hdr.name   = &tempstring[0];
    hdr.family = &tempstring[100];
    for(fd = _GrFontDriverTable; (*fd) != NULL; fd++) {
        DBGPRINTF(DBG_FONT,("Driver \"%s\", extension \"%s\"\n", (*fd)->name, (*fd)->ext));
        pathname[plen] = '\0';
        if(!((*fd)->openfile)(pathname)) {
            strcpy(&pathname[plen],(*fd)->ext);
            if(!((*fd)->openfile)(pathname)) continue;
        }
        if(!((*fd)->header)(&hdr)) {
            DBGPRINTF(DBG_FONT,("fd->header failed for %s\n", pathname));
            (*fd)->cleanup();
            continue;
        }
        f = _GrBuildFont(
            &hdr,
            cvt,
            w,h,
            lo,hi,
            (*fd)->charwdt,
            (*fd)->bitmap,
            (*fd)->scalable
        );
        if(!f) {
            DBGPRINTF(DBG_FONT,("_GrBuildFont failed for %s\n", pathname));
            (*fd)->cleanup();
            continue;
        }
        (*fd)->cleanup();
        res = f;
        break;
    }
    GRX_RETURN(res);
}

/**
 * grx_font_load_converted:
 * @filename: (type filename): the file path
 * @flags: the conversion flags
 * @width: the width of the converted font
 * @height: the height of the converted font
 * @min_ch: the first character to convert
 * @max_ch: the last character to convert
 *
 * Loads a font from a file and converts it to a new style and/or size. The
 * changes are determined by @flags. @width and @height are only used when
 * #GRX_FONT_CONV_FLAG_RESIZE is set in @flags. @min_ch and @max_ch are only
 * used when #GRX_FONT_CONV_FLAG_SKIP_CHARS is set in @flags.
 *
 * If @filename starts with any path separator character or character
 * sequence (':', '/' or '\') then it is loaded from the specified directory,
 * otherwise the library try load the font first from the current directory and
 * next from the default font path (see grx_font_set_path()).
 *
 * Returns: (transfer full) (nullable): the font or %NULL if there was an error
 * such as the font was not found.
 */
GrxFont *grx_font_load_converted(const char *name,GrxFontConversionFlags cvt,int w,int h,int minc,int maxc)
{
    GrxFont *f;
    int  chr,len,abspath,dc;
    char fname[200];
    GRX_ENTER();
    len = 0;
    abspath = FALSE;
    dc = TRUE;
    while((chr = *name++) != '\0') {
        switch(chr) {
#if defined(__WIN32__)
          case ':':
            abspath = TRUE;
            break;
          case '\\':
            chr = '/';
#endif
          case '/':
            dc = TRUE;
            if(len == 0) abspath = TRUE;
            break;
          default:
            if(isspace(chr)) {
                if(len == 0) continue;
                name = "";
                chr  = '\0';
            }
#if defined(__WIN32__)
                chr = tolower(chr);
#endif
            break;
        }
        fname[len++] = chr;
    }
    fname[len] = '\0';
    f = doit(fname,"",cvt,w,h,minc,maxc);
    if((f == NULL) && !abspath) {
        if(_GrFontFileInfo.npath < 0) {
            char *fPath = getenv("GRXFONT");
#ifdef GRX_DEFAULT_FONT_PATH
            if (!fPath) fPath = GRX_DEFAULT_FONT_PATH;
#endif            
            grx_font_set_path(fPath);
        }
        for(len = 0; len < _GrFontFileInfo.npath; len++) {
            f = doit(fname,_GrFontFileInfo.path[len],cvt,w,h,minc,maxc);
            if(f != NULL) break;
        }
    }
    GRX_RETURN(f);
}

/**
 * grx_font_load:
 * @filename: (type filename): the file path
 *
 * Loads a font from a file.
 *
 * If @filename starts with any path separator character or character
 * sequence (':', '/' or '\') then it is loaded from the specified directory,
 * otherwise the library try load the font first from the current directory and
 * next from the default font path (see grx_font_set_path()).
 *
 * Returns: (transfer full) (nullable): the font or %NULL if there was an error
 * such as the font was not found.
 */
GrxFont *grx_font_load(const char *name)
{
    return(grx_font_load_converted(name,GRX_FONT_CONV_FLAG_NONE,0,0,0,0));
}
