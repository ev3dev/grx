/**
 ** UNLOADFN.C ---- remove a font from memory
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "allocate.h"
#include "arith.h"

void GrUnloadFont(GrFont *f)
{
        if((f != NULL) && !f->h.preloaded) {
            uint i;
            free(f->h.name);
            free(f->h.family);
            farfree(f->bitmap);
            if(f->auxmap) farfree(f->auxmap);
            for(i = 0; i < itemsof(f->auxoffs); i++) {
                if(f->auxoffs[i]) free(f->auxoffs[i]);
            }
            free(f);
        }
}

