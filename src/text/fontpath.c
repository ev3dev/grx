/**
 ** FONTPATH.C ---- set up the font search path
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/
#include <string.h>
#include <ctype.h>

#include "grfontdv.h"
#include "libgrx.h"
#include "allocate.h"
#include "alloca.h"
#include "arith.h"

void GrSetFontPath(char *p)
{
        int  chr,totlen = 0,npath = 0,plen = 0;
        char path[200],*plist[100];
        if(!p || (*p == '\0')) return;
        setup_alloca();
        path[0] = '\0';
        while(((chr = *p++) != '\0') || (plen > 0)) {
            int pathchr = TRUE;
            switch(chr) {
              case ':':
#ifdef __MSDOS__
                if((plen == 1) && isalpha(path[0])) break;
#endif
              case ';':
                pathchr = FALSE;
                break;
              case '\0':
                p--;
                pathchr = FALSE;
                break;
#ifdef __MSDOS__
              case '\\':
                chr = '/';
                break;
#endif
              default:
#ifdef __MSDOS__
                chr = tolower(chr);
#endif
                if(isspace(chr)) pathchr = FALSE;
                break;
            }
            if(pathchr) {
                path[plen++] = chr;
                continue;
            }
            if(plen > 0) {
                if(path[plen - 1] != '/') path[plen++] = '/';
                path[plen++] = '\0';
                plist[npath] = alloca(plen);
                if(plist[npath] == NULL) goto error;
                strcpy(plist[npath],path);
                totlen += plen;
                plen = 0;
                if(++npath == itemsof(plist)) break;
            }
        }
        if(_GrFontFileInfo.path != NULL) free(_GrFontFileInfo.path);
        _GrFontFileInfo.path  = NULL;
        _GrFontFileInfo.npath = npath;
        if(npath > 0) {
            _GrFontFileInfo.path = malloc((sizeof(char *) * npath) + totlen);
            if(_GrFontFileInfo.path == NULL) goto error;
            p = (char *)(&_GrFontFileInfo.path[npath]);
            for(plen = 0; plen < npath; plen++) {
                _GrFontFileInfo.path[plen] = p;
                strcpy(p,plist[plen]);
                p += strlen(p) + 1;
            }
        }
        goto done;
      error:
        if(_GrFontFileInfo.path != NULL) free(_GrFontFileInfo.path);
        _GrFontFileInfo.path  = NULL;
        _GrFontFileInfo.npath = 0;
      done:
        reset_alloca();
}
