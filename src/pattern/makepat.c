/**
 ** MAKEPAT.C
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **
 **  Copyright (C) 1992, Csaba Biegl
 **    820 Stirrup Dr, Nashville, TN, 37221
 **    csaba@vuse.vanderbilt.edu
 **
 **  This file is distributed under the terms listed in the document
 **  "copying.cb", available from the author at the address above.
 **  A copy of "copying.cb" should accompany this file; if not, a copy
 **  should be available from where this file was obtained.  This file
 **  may not be distributed without a verbatim copy of "copying.cb".
 **  You should also have received a copy of the GNU General Public
 **  License along with this program (it is in the file "copying");
 **  if not, write to the Free Software Foundation, Inc., 675 Mass Ave,
 **  Cambridge, MA 02139, USA.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **/

#include "libgrx.h"
#include "allocate.h"

#define  BEST_MAX_LINE         128
#define  BEST_MAX_CONTEXT      2048L

#define GCM_MYMEMORY        1           /* set if my context memory */
#define GCM_MYCONTEXT       2           /* set if my context structure */

/*
 * try to replicate a pixmap for faster bitblt-s
 * in bitplane modes it is especially desirable to replicate until
 * the width is a multiple of 8
 */
static int _GrBestPixmapWidth(int wdt,int hgt)
{
        long total   = GrContextSize(wdt,hgt);
        int  linelen = GrLineOffset(wdt);
        int  factor  = 1;
        int  test;

        if(total == 0L) return(0);
#ifdef _MAXMEMPLANESIZE
        if(total > _MAXMEMPLANESIZE) return(0);
#endif
        if((test = (int)(BEST_MAX_CONTEXT / total)) > factor)
            factor = test;
        if((test = (BEST_MAX_LINE / linelen)) < factor)
            factor = test;
        while((factor >>= 1) != 0)
            wdt <<= 1;
        return(wdt);
}

GrPattern *GrBuildPixmap(char *pixels,int w,int h,GrColorTableP ct)
{
        GrContext csave,cwork;
        GrPixmap  *result;
        unsigned  char *src;
        int  wdt,wdt2,fullw;
        int  hgt;
        GrColor color;

        if((fullw = _GrBestPixmapWidth(w,h)) <= 0) return(NULL);
        result = (GrPixmap *)malloc(sizeof(GrPixmap));
        if (result == NULL) return(NULL);

        if (!GrCreateContext(fullw,h,NULL,&cwork)) {
          free(result);
          return NULL;
        }
        csave = *CURC;
        *CURC = cwork;
        for(hgt = 0; hgt < h; hgt++) {
            for(wdt2 = fullw; (wdt2 -= w) >= 0; ) {
                src = (unsigned char *)pixels;
                for(wdt = 0; wdt < w; wdt++) {
                    color = *src++;
                    if(ct != NULL) color = GR_CTABLE_COLOR(ct,color);
                    (*CURC->gc_driver->drawpixel)(wdt2+wdt,hgt,(color & C_COLOR));
                }
            }
            pixels += w;
        }
        *CURC = csave;
        result->pxp_source = cwork.gc_frame;
        result->pxp_source.gf_memflags = (GCM_MYCONTEXT | GCM_MYMEMORY);
        result->pxp_ispixmap = TRUE;
        result->pxp_width  = fullw;
        result->pxp_height = h;
        result->pxp_oper   = 0;
        return((GrPattern *)result);
}

GrPattern *GrBuildPixmapFromBits(char *bits,int w,int h,GrColor fgc,GrColor bgc)
{
        GrContext csave,cwork;
        GrPixmap  *result;
        unsigned  char *src;
        int  wdt,wdt2,fullw;
        int  hgt,mask,byte;

        if((fullw = _GrBestPixmapWidth(w,h)) <= 0) return(NULL);
        result = (GrPixmap *)malloc(sizeof(GrPixmap));
        if(result == NULL) return(NULL);

        if (!GrCreateContext(fullw,h,NULL,&cwork)) {
          free(result);
          return NULL;
        }
        csave = *CURC;
        *CURC = cwork;
        fgc &= C_COLOR;
        bgc &= C_COLOR;
        for(hgt = 0; hgt < h; hgt++) {
            for(wdt2 = fullw; (wdt2 -= w) >= 0; ) {
                src  = (unsigned char *)bits;
                mask = byte = 0;
                for(wdt = w; --wdt >= 0; ) {
                    if((mask >>= 1) == 0) { mask = 0x80; byte = *src++; }
                    (*CURC->gc_driver->drawpixel)(wdt2+wdt,hgt,((byte & mask) ? fgc : bgc));
                }
            }
            bits += (w + 7) >> 3;
        }
        *CURC = csave;
        result->pxp_source = cwork.gc_frame;
        result->pxp_source.gf_memflags = (GCM_MYCONTEXT | GCM_MYMEMORY);
        result->pxp_ispixmap = TRUE;
        result->pxp_width  = fullw;
        result->pxp_height = h;
        result->pxp_oper   = 0;
        return((GrPattern *)result);
}

GrPattern *GrConvertToPixmap(GrContext *src)
{
        GrPixmap *result;

        if(src->gc_onscreen) return(NULL);
        result = malloc(sizeof(GrPixmap));
        if(result == NULL) return(NULL);
        result->pxp_source = src->gc_frame;
        result->pxp_source.gf_memflags = GCM_MYCONTEXT;
        result->pxp_ispixmap = TRUE;
        result->pxp_width  = src->gc_xmax + 1;
        result->pxp_height = src->gc_ymax + 1;
        result->pxp_oper   = 0;
        return((GrPattern *)result);
}


void GrDestroyPattern(GrPattern *p)
{
  if (!p) return;
  if (p->gp_ispixmap) {
    if ( p->gp_pxp_source.gf_memflags & GCM_MYMEMORY) {
      int ii;
      for ( ii = p->gp_pxp_source.gf_driver->num_planes; ii > 0; ii-- )
         farfree(p->gp_pxp_source.gf_baseaddr[ii - 1]);
    }
    if ( p->gp_pxp_source.gf_memflags & GCM_MYCONTEXT )
      free(p);
    return;
  }
  if ( p->gp_bitmap.bmp_memflags ) free(p);
}

