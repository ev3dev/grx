/**
 ** BUILDAUX.C ---- generate and store a rotated character for a font
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "allocate.h"
#include "arith.h"
#include "memfill.h"
#include "memcopy.h"

char far *GrBuildAuxiliaryBitmap(GrFont *f,int chr,int dir,int ul)
{
        uint idx = (uint)chr - f->h.minchar;
        uint bpos,rbpos,size,rsize,w,h;
        int  boff,rboff,rbinc;
        char far *stdmap,far *cvtmap;
        if(idx >= f->h.numchars) return(NULL);
        stdmap = &f->bitmap[f->chrinfo[idx].offset];
        dir = (dir & 3) + ((ul && (f->h.ulheight > 0)) ? 4 : 0);
        if(dir == GR_TEXT_RIGHT) return(stdmap);
        if(f->auxoffs[--dir] != NULL) {
            uint offs = f->auxoffs[dir][idx];
            if(offs > 0) return(&f->auxmap[offs - 1]);
        }
        else {
            size = sizeof(f->auxoffs[0][0]) * f->h.numchars;
            f->auxoffs[dir] = farmalloc(size);
            if(f->auxoffs[dir] == NULL) return(NULL);
            memzero(f->auxoffs[dir],size);
        }
        h     = f->h.height;
        w     = f->chrinfo[idx].width;
        size  = h * (boff  = (w + 7) & ~7);
        rsize = w * (rboff = (h + 7) & ~7);
        switch(dir) {
          case (GR_TEXT_RIGHT - 1 + 4):
            rboff = boff;
            rsize = size;
            rbpos = 0;
            rbinc = 1;
            break;
          case (GR_TEXT_DOWN - 1):                /* downward */
          case (GR_TEXT_DOWN - 1 + 4):
            rbpos = h - 1;
            rbinc = rboff;
            rboff = -1;
            break;
          case (GR_TEXT_LEFT - 1):                /* upside down, right to left */
          case (GR_TEXT_LEFT - 1 + 4):
            rboff = boff;
            rsize = size;
            rbpos = rsize - rboff + w - 1;
            rbinc = -1;
            rboff = -rboff;
            break;
          case (GR_TEXT_UP - 1):                /* upward */
          case (GR_TEXT_UP - 1 + 4):
            rbpos = rsize - rboff;
            rbinc = -rboff;
            rboff = 1;
            break;
          default:
            return(NULL);
        }
        if((rsize >>= 3) == 0) return(NULL);
        if(rsize > (f->auxsize - f->auxnext)) {
            /* add space for 32 (average) characters */
            uint newsize = (((f->h.width + 7) >> 3) * f->h.height) << 6;
            newsize = umax(newsize,(rsize << 2));
            newsize = umin(newsize,((uint)(-4) - f->auxsize));
            newsize += f->auxsize;
            if(rsize > (newsize - f->auxnext)) return(NULL);
            cvtmap = farmalloc(newsize);
            if(cvtmap == NULL) return(NULL);
            if(f->auxsize > 0) {
                memcopy(cvtmap,f->auxmap,f->auxsize);
                farfree(f->auxmap);
            }
            f->auxmap  = cvtmap;
            f->auxsize = newsize;
        }
        cvtmap = &f->auxmap[f->auxnext];
        f->auxoffs[dir][idx] = f->auxnext + 1;
        f->auxnext += rsize;
        memfill_b(cvtmap,0,rsize);
        for(h = bpos = 0; bpos < size; bpos += boff,rbpos += rboff,h++) {
            uint bp    = bpos;
            uint bptop = bpos + w;
            uint rbp   = rbpos;
            uint ulrow = ul && ((h - f->h.ulpos) < f->h.ulheight);
            for( ; bp < bptop; bp++,rbp += rbinc) {
                if(stdmap[bp >> 3] & (0x80 >> (bp & 7)) || ulrow) {
                    cvtmap[rbp >> 3] |= (0x80 >> (rbp & 7));
                }
            }
        }
        return(cvtmap);
}

