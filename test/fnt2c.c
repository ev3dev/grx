/**
 ** FONT2C.C ---- convert a font to C source code
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include <stdio.h>
#include "grx20.h"

int main(int argc,char **argv)
{
        GrFont *f;
        if(argc < 4) {
            fprintf(
                    stderr,
                    "%s: too few arguments\n"
                    "usage: font2c fontFile outputFile CfontSymbolName\n",
                    argv[0]
            );
            return(1);
        }
        f = GrLoadFont(argv[1]);
        if(!f) {
            fprintf(
                    stderr,
                    "%s: could not load font \"%s\"\n",
                    argv[0],
                    argv[1]
            );
            return(1);
        }
        GrDumpFont(f,argv[3],argv[2]);
        return(0);
}

