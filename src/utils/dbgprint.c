/**
 ** DBGPRINT.H ---- GRX debug support
 **
 ** Copyright (c) 1998 Hartmut Schirmer
 **/

#ifdef  DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "grxkeys.h"
#include "grxdebug.h"
#ifdef __XWIN__
#include "libxwin.h"
#endif

#ifndef DBG_LOGFILE
#define DBG_LOGFILE "grxdebug.log"
#endif

char *_GR_debug_file;
int   _GR_debug_line;
#ifdef __GNUC__
char *_GR_debug_function;
# endif
int   _GR_debug_flags = DEBUG-0;

void _GR_debug_printf(char *fmt,...)
{
        FILE *dfp = NULL;
        va_list ap;
        dfp = fopen(DBG_LOGFILE,"at");
        if(!dfp) return;
#ifdef __GNUC__
        fprintf(dfp,"%s|%s|%d: ",
                _GR_debug_file, _GR_debug_function, _GR_debug_line);
#else
        fprintf(dfp,"%s/%d: ", _GR_debug_file, _GR_debug_line);
#endif
        va_start(ap,fmt);
        vfprintf(dfp,fmt,ap);
        va_end(ap);
        fclose(dfp);
#ifndef __XWIN__
        if (GrKeyPressed()) {
          if (GrKeyRead() == GrKey_Escape) exit(1);
        }
#endif
}

#endif
