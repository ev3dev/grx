/**
 ** LOADFONT.C ---- load a font from a disk file
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "grfontdv.h"
#include "libgrx.h"

static GrFont *doit(char *fname,char *path,int cvt,int w,int h,int lo,int hi)
{
        GrFontDriver **fd;
        GrFontHeader hdr;
        GrFont *f;
        char pathname[200];
        char tempstring[200];
        int  plen;
        strcpy(pathname,path);
        strcat(pathname,fname);
        plen = strlen(pathname);
        hdr.name   = &tempstring[0];
        hdr.family = &tempstring[100];
        for(fd = _GrFontDriverTable; (*fd) != NULL; fd++) {
            pathname[plen] = '\0';
            if(!((*fd)->openfile)(pathname)) {
                strcpy(&pathname[plen],(*fd)->ext);
                if(!((*fd)->openfile)(pathname)) continue;
            }
            if(!((*fd)->header)(&hdr)) {
                (*fd)->cleanup();
                break;
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
                (*fd)->cleanup();
                break;
            }
            return(f);
        }
        return(NULL);
}

GrFont *GrLoadConvertedFont(char *name,int cvt,int w,int h,int minc,int maxc)
{
        GrFont *f;
        int  chr,len = 0,abspath = FALSE;
        char fname[200];
        while((chr = *name++) != '\0') {
            switch(chr) {
#ifdef __MSDOS__
              case ':':
                abspath = TRUE;
                break;
              case '\\':
                chr = '/';
#endif
              case '/':
                if(len == 0) abspath = TRUE;
                break;
              default:
                if(isspace(chr)) {
                    if(len == 0) continue;
                    name = "";
                    chr  = '\0';
                }
#ifdef __MSDOS__
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
                GrSetFontPath(getenv("GRXFONT"));
            }
            for(len = 0; len < _GrFontFileInfo.npath; len++) {
                f = doit(fname,_GrFontFileInfo.path[len],cvt,w,h,minc,maxc);
                if(f != NULL) break;
            }
        }
        return(f);
}

GrFont *GrLoadFont(char *name)
{
        return(GrLoadConvertedFont(name,GR_FONTCVT_NONE,0,0,0,0));
}

