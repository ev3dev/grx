/**
 ** FDV_BGI.C -- driver for Borland BGI font file format
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 ** Contrib by Christian Domp (alma.student.uni-kl.de) See "doc/contrib.doc"
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@xaphod.techfak.uni-kiel.d400.de)
 **/

#include <stdio.h>
#include <string.h>

#include "grfontdv.h"
#include "libgrx.h"
#include "allocate.h"
#include "arith.h"
#include "memfill.h"
#include "fonts/fdv_bgi.h"

/* This is based on the font code in Hartmut Schirmer's BCC2GRX package */

#ifdef  __MSDOS__
#define OPENMODE        "rb"
#else
#define OPENMODE        "r"
#endif

#ifndef SEEK_SET
#define SEEK_SET        0
#endif
#ifndef SEEK_END
#define SEEK_END        2
#endif

#ifdef  DEBUG
#define DPRINTF(x)      debugprintf x
#include <stdarg.h>
static void debugprintf(char *fmt,...)
{
        static FILE *dfp = NULL;
        va_list ap;
        if(!dfp) dfp = fopen("bgiload.dbg","w");
        if(!dfp) return;
        va_start(ap,fmt);
        vfprintf(dfp,fmt,ap);
        va_end(ap);
        fflush(dfp);
}
#else
#define DPRINTF(x)
#endif

static char   *fdata   = NULL;
static uchar  *wtable  = NULL;
static ushort *offsets = NULL;
static uchar  *vecdata = NULL;
static int    *realwdt = NULL;
static int    *xoffset = NULL;
static int     realhgt = 0;
static int     yoffset = 0;
static BGIfontFileHeader *fhdr = NULL;
static BGIfontHeaderType *fhtp = NULL;

static void cleanup(void)
{
        if(fdata)   free(fdata);
        if(realwdt) free(realwdt);
        fdata   = NULL;
        realwdt = NULL;
}

/* fix up up sizes to account for out of box characters */
static void fixlimits(void)
{
        int i,ymin  = 32000,ymax = -32000;
        int orighgt = fhtp->org_to_cap - fhtp->org_to_dec;
        for(i = 0; i < fhtp->nchrs; i++) {
            int xpos = 0,ypos = 0,xend,yend;
            int xmin = 32000,xmax = -32000;
            ushort *vp = (ushort *)(vecdata + offsets[i]);
            for( ; ; vp++) {
                switch(SV_COMMAND(*vp)) {
                  case SVC_END:
                    break;
                  case SVC_MOVE:
                    xpos = SV_XCOORD(*vp);
                    ypos = fhtp->org_to_cap - SV_YCOORD(*vp);
                    continue;
                  case SVC_SCAN:
                    /* what to do here ? */
                    continue;
                  case SVC_DRAW:
                    xend = SV_XCOORD(*vp);
                    yend = fhtp->org_to_cap - SV_YCOORD(*vp);
                    xmin = imin(xmin,imin(xpos,xend));
                    ymin = imin(ymin,imin(ypos,yend));
                    xmax = imax(xmax,imax(xpos,xend));
                    ymax = imax(ymax,imax(ypos,yend));
                    xpos = xend;
                    ypos = yend;
                    continue;
                }
                break;
            }
            xoffset[i] = imax(0,(-xmin));
            realwdt[i] = imax(1,(imax((xmax + 1),wtable[i]) + xoffset[i]));
            DPRINTF((
                "character %3d: origwdt=%-3d realwdt=%-3d xoffset=%-3d %c\n",
                (i + fhtp->firstch),
                wtable[i],
                realwdt[i],
                xoffset[i],
                (realwdt[i] > wtable[i]) ? '*' : ' '
            ));
        }
        yoffset = imax(0,(-ymin));
        realhgt = imax((ymax + 1),orighgt) + yoffset;
        DPRINTF((
            "FONT orighgt=%-3d realhgt=%-3d yoffset=%-3d\n",
            orighgt,
            realhgt,
            yoffset
        ));
}

static int openfile(char *fname)
{
        char *p;
        FILE *fp;
        long flen;
        cleanup();
        fp = fopen(fname,OPENMODE);
        if(!fp) return(FALSE);
        fseek(fp,0L,SEEK_END);
        flen = ftell(fp);
        fseek(fp,0L,SEEK_SET);
        if((flen <= (signed long)(sizeof(*fhdr) + sizeof(*fhtp))) ||
           (flen != (long)(ushort)flen) ||
           (!(fdata = malloc((uint)flen))) ||
           (fread(fdata,1,(uint)flen,fp) != (size_t)flen) ||
           (strncmp(fdata,FILEMARKER,strlen(FILEMARKER)) != 0) ||
           (!(p = strchr(fdata,MARKEREND))) ||
           ((++p) > &fdata[128 - sizeof(*fhdr)]) ||
           (!(fhdr = (BGIfontFileHeader *)p)) ||
           (!(fhtp = (BGIfontHeaderType *)(fdata + fhdr->header_size))) ||
           (fhtp->sig != SIGBYTE)) {
            cleanup();
            fclose(fp);
            return(FALSE);
        }
        fclose(fp);
        offsets = (ushort *)(fhtp + 1);
        wtable  = (uchar  *)(offsets + fhtp->nchrs);
        vecdata = (uchar  *)((char *)fhtp + fhtp->cdefs);
        realwdt = malloc(sizeof(int) * fhtp->nchrs * 2);
        xoffset = realwdt + fhtp->nchrs;
        if(!realwdt) {
            cleanup();
            return(FALSE);
        }
        fixlimits();
        return(TRUE);
}

static uint avgwidth(void)
{
        uint i,total = 0;
        for(i = 0; i < fhtp->nchrs; i++) total += realwdt[i];
        return((total + (fhtp->nchrs >> 1)) / fhtp->nchrs);
}

static int header(GrFontHeader *hdr)
{
        if(!fdata) return(FALSE);
        strcpy(hdr->family,"BGI");
        memcpy(hdr->name,fhdr->font_name,sizeof(fhdr->font_name));
        hdr->name[sizeof(fhdr->font_name)] = '\0';
        hdr->proportional = TRUE;
        hdr->scalable     = TRUE;
        hdr->preloaded    = FALSE;
        hdr->modified     = GR_FONTCVT_NONE;
        hdr->width        = avgwidth();
        hdr->height       = realhgt;
        hdr->baseline     = fhtp->org_to_cap - fhtp->org_to_base + yoffset;
        hdr->ulheight     = imax(1,(hdr->height / 15));
        hdr->ulpos        = hdr->height - hdr->ulheight;
        hdr->minchar      = fhtp->firstch;
        hdr->numchars     = fhtp->nchrs;
        DPRINTF((
            "Font header:\n"
            "  name         = %s\n"
            "  family       = %s\n"
            "  proportional = %d\n"
            "  scalable     = %d\n"
            "  preloaded    = %d\n"
            "  modified     = %d\n"
            "  width        = %d\n"
            "  height       = %d\n"
            "  baseline     = %d\n"
            "  ulheight     = %d\n"
            "  ulpos        = %d\n"
            "  minchar      = %d\n"
            "  numchars     = %d\n",
            hdr->name,
            hdr->family,
            hdr->proportional,
            hdr->scalable,
            hdr->preloaded,
            hdr->modified,
            hdr->width,
            hdr->height,
            hdr->baseline,
            hdr->ulheight,
            hdr->ulpos,
            hdr->minchar,
            hdr->numchars
        ));
        return(TRUE);
}

static int charwdt(int chr)
{
        chr -= fhtp->firstch;
        if(!fdata) return(-1);
        if((uint)chr >= (uint)fhtp->nchrs) return(-1);
        DPRINTF(("charwdt for %d is %d\n",(chr + fhtp->firstch),realwdt[chr]));
        return(realwdt[chr]);
}

static void bitline(int x1,int y1,int x2,int y2,char *buffer,int pitch)
{
        int  dx    = x2 - x1;
        int  dy    = y2 - y1;
        int  xstep = (dx < 0) ? ((dx = -dx),(-1)) : 1;
        int  ystep = ((dy < 0) ? ((dy = -dy),(-pitch)) : pitch) << 3;
        uint addr  = ((y1 * pitch) << 3) + x1;
        int  count,error,errsub,erradd;
        uint step1,step2;
        if(dy > dx) {
            count  = dy + 1;
            error  = dy >> 1;
            erradd = dy;
            errsub = dx;
            step1  = ystep;
            step2  = xstep + ystep;
        }
        else {
            count  = dx + 1;
            error  = dx >> 1;
            erradd = dx;
            errsub = dy;
            step1  = xstep;
            step2  = xstep + ystep;
        }
        do {
            buffer[addr >> 3] |= (0x80U >> (addr & 7));
            if((error -= errsub) < 0) {
                error += erradd;
                addr  += step2;
            }
            else {
                addr  += step1;
            }
        } while(--count);
}

static int bitmap(int chr,int w,int h,char *buffer)
{
        int xmul,xdiv,ymul,ydiv;
        int xpos,ypos,xend,yend;
        int offs;
        ushort *vp;
        chr -= fhtp->firstch;
        if(!fdata) return(FALSE);
        if((uint)chr >= (uint)fhtp->nchrs) return(FALSE);
        if((w <= 0) || (h <= 0)) return(FALSE);
        xmul = w - 1;
        xdiv = realwdt[chr] - 1;
        ymul = h - 1;
        ydiv = realhgt - 1;
        DPRINTF((
            "bitmap for %d, origsize = %d %d, rendered = %d %d\n",
            (chr  + fhtp->firstch),
            (xdiv + 1),(ydiv + 1),
            (xmul + 1),(ymul + 1)
        ));
        vp   = (ushort *)(vecdata + offsets[chr]);
        offs = (w + 7) >> 3;
        memfill_b(buffer,0,(offs * h));
        if(xdiv <= 0) return(TRUE);
        for(xpos = ypos = 0; ; vp++) {
            switch(SV_COMMAND(*vp)) {
              case SVC_END:
                break;
              case SVC_MOVE:
                xpos = SV_XCOORD(*vp) + xoffset[chr];
                ypos = fhtp->org_to_cap - SV_YCOORD(*vp) + yoffset;
                DPRINTF((
                    "  cmd=0x%04x, move   %-3d %d\n",
                    *vp,xpos,ypos
                ));
                continue;
              case SVC_SCAN:
                /* what to do here ? */
                DPRINTF(("  cmd=0x%04x **** SCAN COMMAND FOUND ****\n",*vp));
                continue;
              case SVC_DRAW:
                xend = SV_XCOORD(*vp) + xoffset[chr];
                yend = fhtp->org_to_cap - SV_YCOORD(*vp) + yoffset;
                DPRINTF((
                    "  cmd=0x%04x, vector %-3d %-3d [ %-3d %-3d ] %c\n",
                    *vp,xend,yend,xpos,ypos,
                    ((((uint)xend > (uint)xdiv) ||
                      ((uint)yend > (uint)ydiv) ||
                      ((uint)xpos > (uint)xdiv) ||
                      ((uint)ypos > (uint)ydiv)) ? '*' : ' '
                    )
                ));
                bitline(
                    urscale(xpos,xmul,xdiv),urscale(ypos,ymul,ydiv),
                    urscale(xend,xmul,xdiv),urscale(yend,ymul,ydiv),
                    buffer,offs
                );
                xpos = xend;
                ypos = yend;
                continue;
            }
            break;
        }
        return(TRUE);
}

GrFontDriver _GrFontDriverBGI = {
    "BGI",                              /* driver name (doc only) */
    ".chr",                             /* font file extension */
    TRUE,                               /* scalable */
    openfile,                           /* file open and check routine */
    header,                             /* font header reader routine */
    charwdt,                            /* character width reader routine */
    bitmap,                             /* character bitmap reader routine */
    cleanup                             /* cleanup routine */
};

