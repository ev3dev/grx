/**
 ** FDV_GRX.C -- driver for GRX native font file format
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include <stdio.h>
#include <string.h>

#include "grfontdv.h"
#include "libgrx.h"
#include "allocate.h"
#include "fonts/fdv_grx.h"

#ifdef         __MSDOS__
#define  OPENMODE        "rb"
#else
#define  OPENMODE        "r"
#endif

#ifndef  SEEK_SET
#define  SEEK_SET        0
#endif

static GrFontFileHeaderGRX fhdr;
static FILE   *fontfp = NULL;
static ushort *wtable = NULL;
static uint    wtsize = 0;
static int     nextch = 0;

static void cleanup(void)
{
        if(fontfp != NULL) fclose(fontfp);
        if(wtable != NULL) free(wtable);
        fontfp = NULL;
        wtable = NULL;
        nextch = 0;
        wtsize = 0;
}

static int openfile(char *fname)
{
        cleanup();
        fontfp = fopen(fname,OPENMODE);
        if(fontfp == NULL)                            return(cleanup(),FALSE);
        if(fread(&fhdr,sizeof(fhdr),1,fontfp) != 1) return(cleanup(),FALSE);
        if(fhdr.magic != GRX_FONTMAGIC)                    return(cleanup(),FALSE);
        if(!fhdr.isfixed) {
            wtsize = sizeof(ushort) * (fhdr.maxchar - fhdr.minchar + 1);
            wtable = malloc(wtsize);
            if(wtable == NULL)                            return(cleanup(),FALSE);
            if(fread(wtable,wtsize,1,fontfp) != 1)  return(cleanup(),FALSE);
        }
        nextch = fhdr.minchar;
        return(TRUE);
}

static int header(GrFontHeader *hdr)
{
        if(fontfp == NULL) return(FALSE);
        memcpy(hdr->name,  fhdr.fnname,sizeof(fhdr.fnname));
        memcpy(hdr->family,fhdr.family,sizeof(fhdr.family));
        hdr->name  [sizeof(fhdr.fnname)] = '\0';
        hdr->family[sizeof(fhdr.family)] = '\0';
        hdr->proportional = fhdr.isfixed ? FALSE : TRUE;
        hdr->scalable          = FALSE;
        hdr->preloaded          = FALSE;
        hdr->modified          = GR_FONTCVT_NONE;
        hdr->width          = fhdr.width;
        hdr->height          = fhdr.height;
        hdr->baseline          = fhdr.baseline;
        hdr->ulpos          = fhdr.height - fhdr.undwidth;
        hdr->ulheight          = fhdr.undwidth;
        hdr->minchar          = fhdr.minchar;
        hdr->numchars          = fhdr.maxchar - fhdr.minchar + 1;
        return(TRUE);
}

static int charwdt(int chr)
{
        if(fontfp == NULL)        return(-1);
        if(chr < fhdr.minchar)  return(-1);
        if(chr > fhdr.maxchar)  return(-1);
        return(fhdr.isfixed ? fhdr.width : wtable[chr - fhdr.minchar]);
}

static int bitmap(int chr,int w,int h,char *buffer)
{
        if((w <= 0) || (w != charwdt(chr))) return(FALSE);
        if((h <= 0) || (h != fhdr.height))  return(FALSE);
        if(chr != nextch) {
            long fpos = sizeof(fhdr) + (fhdr.isfixed ? 0 : wtsize);
            for(nextch = fhdr.minchar; nextch != chr; nextch++) {
                fpos += ((charwdt(nextch) + 7) >> 3) * fhdr.height;
            }
            fseek(fontfp,fpos,SEEK_SET);
        }
        nextch = chr + 1;
        return(fread(buffer,(((w + 7) >> 3) * h),1,fontfp) == 1);
}

GrFontDriver _GrFontDriverGRX = {
    "GRX",                                /* driver name (doc only) */
    ".fnt",                                /* font file extension */
    FALSE,                                /* scalable */
    openfile,                                /* file open and check routine */
    header,                                /* font header reader routine */
    charwdt,                                /* character width reader routine */
    bitmap,                                /* character bitmap reader routine */
    cleanup                                /* cleanup routine */
};

