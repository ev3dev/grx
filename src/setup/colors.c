/**
 ** COLORS.C ---- color management functions
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "libgrx.h"
#include "memfill.h"
#include "memcopy.h"

static void (*DACload)(int c,int r,int g,int b) = NULL;

static void loadcolor(int c,int r,int g,int b)
{
        CLRINFO->ctable[c].r = (r &= CLRINFO->mask[0]);
        CLRINFO->ctable[c].g = (g &= CLRINFO->mask[1]);
        CLRINFO->ctable[c].b = (b &= CLRINFO->mask[2]);
        if(DACload) (*DACload)(c,r,g,b);
}

static void setbits(char *prec,char *pos)
{
        int i,tmp;
        CLRINFO->norm = 0;
        for(i = 0; i < 3; i++,prec++,pos++) {
            CLRINFO->prec[i]  = *prec;
            CLRINFO->pos[i]   = *prec + *pos - 1;
            CLRINFO->mask[i]  = (tmp = 0xff ^ (0xff >> *prec));
            CLRINFO->round[i] = (tmp >> 1) & ~tmp;
            CLRINFO->shift[i] = (tmp = CLRINFO->pos[i] - 7);
            CLRINFO->norm     = imax(CLRINFO->norm,(-tmp));
        }
        CLRINFO->shift[0] += CLRINFO->norm;
        CLRINFO->shift[1] += CLRINFO->norm;
        CLRINFO->shift[2] += CLRINFO->norm;
}

void GrRefreshColors(void)
{
        int i;
        for(i = 0; i < (int)CLRINFO->ncolors; i++) {
            if(CLRINFO->ctable[i].defined) loadcolor(
                (int)(i),
                CLRINFO->ctable[i].r,
                CLRINFO->ctable[i].g,
                CLRINFO->ctable[i].b
            );
        }
}

void GrResetColors(void)
{
#       define NSAVED 16
        static char infosave[offsetof(struct _GR_colorInfo,ctable[NSAVED])];
        static int  firsttime = TRUE;
        int i;
        if(firsttime) {
            memcopy(infosave,CLRINFO,sizeof(infosave));
            firsttime = FALSE;
        }
        sttzero(CLRINFO);
        if(DRVINFO->actmode.extinfo->mode == GR_frameText) {
            memcopy(CLRINFO,infosave,sizeof(infosave));
            return;
        }
        DACload = DRVINFO->actmode.extinfo->loadcolor;
        CLRINFO->ncolors = 1L << DRVINFO->actmode.bpp;
        CLRINFO->black   = GrNOCOLOR;
        CLRINFO->white   = GrNOCOLOR;
        setbits(
            DRVINFO->actmode.extinfo->cprec,
            DRVINFO->actmode.extinfo->cpos
        );
        switch(DRVINFO->actmode.bpp) {
          case 4:
          case 8:
            CLRINFO->nfree = CLRINFO->ncolors;
            for(i = 0; i < NSAVED; i++) {
                loadcolor(
                    (int)(i),
                    ((struct _GR_colorInfo *)(infosave))->ctable[i].r,
                    ((struct _GR_colorInfo *)(infosave))->ctable[i].g,
                    ((struct _GR_colorInfo *)(infosave))->ctable[i].b
                );
                CLRINFO->ctable[i].defined = TRUE;
            }
            break;
          default:
            CLRINFO->RGBmode = TRUE;
            break;
        }
}

void GrSetRGBcolorMode(void)
{
        if(!CLRINFO->RGBmode) {
            unsigned long c;
            switch(CLRINFO->ncolors) {
                case 16L:  setbits("\1\2\1","\3\1\0"); break;
                case 256L: setbits("\3\3\2","\5\2\0"); break;
                default:   return;
            }
            CLRINFO->RGBmode = TRUE;
            CLRINFO->nfree   = 0L;
            CLRINFO->black   = 0L;
            CLRINFO->white   = CLRINFO->ncolors - 1L;
            for(c = 0; c < CLRINFO->ncolors; c++) loadcolor(
                (int)(c),
                GrRGBcolorRed(c),
                GrRGBcolorGreen(c),
                GrRGBcolorBlue(c)
            );
        }
}

#define ROUNDCOLORCOMP(x,n) (                                   \
    ((uint)(x) >= CLRINFO->mask[n]) ?                           \
        CLRINFO->mask[n] :                                      \
        (((x) + CLRINFO->round[n]) & CLRINFO->mask[n])          \
)

long GrAllocColor(int r,int g,int b)
{
        r = ROUNDCOLORCOMP(r,0);
        g = ROUNDCOLORCOMP(g,1);
        b = ROUNDCOLORCOMP(b,2);
        if(CLRINFO->RGBmode) {
            return(GrBuildRGBcolorT(r,g,b));
        }
        else {
            int i,minerr = 10000;
            int free = (-1),allfree = (-1),best = (-1);
            int ndef = (int)CLRINFO->ncolors - (int)CLRINFO->nfree;
            for(i = 0; i < (int)CLRINFO->ncolors; i++) {
                if(CLRINFO->ctable[i].defined) {
                    if(!CLRINFO->ctable[i].writable) {
                        int err =
                            iabs(r - CLRINFO->ctable[i].r) +
                            iabs(g - CLRINFO->ctable[i].g) +
                            iabs(b - CLRINFO->ctable[i].b);
                        if(err < minerr) {
                            best = i;
                            if((minerr = err) == 0) goto foundbest;
                        }
                        if((free <= 0) && !CLRINFO->ctable[i].nused) {
                            free = i;
                        }
                    }
                    if(CLRINFO->ctable[i].nused) ndef--;
                }
                else {
                    if(allfree < 0) allfree = i;
                }
                if((allfree >= 0) && (ndef <= 0)) {
                    break;
                }
            }
            if(allfree >= 0) {
                free = allfree;
            }
            if(free >= 0) {
                CLRINFO->ctable[free].defined  = TRUE;
                CLRINFO->ctable[free].writable = FALSE;
                CLRINFO->ctable[free].nused    = 1;
                CLRINFO->nfree--;
                loadcolor(free,r,g,b);
                return((long)(free));
            }
          foundbest:
            if(best >= 0) {
                if(!CLRINFO->ctable[best].nused) CLRINFO->nfree--;
                CLRINFO->ctable[best].nused++;
                return((long)(best));
            }
        }
        return(GrNOCOLOR);
}

long GrAllocCell(void)
{
        if(!CLRINFO->RGBmode && CLRINFO->nfree) {
            int i,free = (-1);
            for(i = 0; i < (int)CLRINFO->ncolors; i++) {
                if(!CLRINFO->ctable[i].defined) {
                    free = i;
                    break;
                }
                if(!CLRINFO->ctable[i].nused) {
                    if(free < 0) free = i;
                }
            }
            if(free >= 0) {
                CLRINFO->ctable[free].defined  = TRUE;
                CLRINFO->ctable[free].writable = TRUE;
                CLRINFO->ctable[free].nused    = 1;
                CLRINFO->nfree--;
                loadcolor(free,0,0,0);
                return((long)(free));
            }
        }
        return(GrNOCOLOR);
}

void GrFreeColor(long c)
{
        if(!CLRINFO->RGBmode && ((ulong)(c) < CLRINFO->ncolors) &&
            !CLRINFO->ctable[(int)(c)].writable &&
            CLRINFO->ctable[(int)(c)].defined &&
            (--CLRINFO->ctable[(int)(c)].nused == 0)) {
                CLRINFO->nfree++;
                CLRINFO->ctable[(int)(c)].defined  = FALSE;
                CLRINFO->ctable[(int)(c)].writable = FALSE;
                CLRINFO->ctable[(int)(c)].nused    = 0;
            }
}

void GrFreeCell(long c)
{
        if(!CLRINFO->RGBmode && ((ulong)(c) < CLRINFO->ncolors)) {
            if(CLRINFO->ctable[(int)(c)].writable) {
                CLRINFO->nfree++;
                CLRINFO->ctable[(int)(c)].defined  = FALSE;
                CLRINFO->ctable[(int)(c)].writable = FALSE;
                CLRINFO->ctable[(int)(c)].nused    = 0;
            }
        }
}

void GrSetColor(long c,int r,int g,int b)
{
        if(!CLRINFO->RGBmode && ((ulong)(c) < CLRINFO->ncolors)) {
            if(!CLRINFO->ctable[(int)(c)].defined) {
                CLRINFO->ctable[(int)(c)].defined  = TRUE;
                CLRINFO->ctable[(int)(c)].nused    = 0;
            }
            if(!CLRINFO->ctable[(int)(c)].nused) {
                CLRINFO->ctable[(int)(c)].writable = TRUE;
                CLRINFO->ctable[(int)(c)].nused    = 1;
                CLRINFO->nfree--;
            }
            if(CLRINFO->ctable[(int)(c)].writable) loadcolor(
                (int)(c),
                ROUNDCOLORCOMP(r,0),
                ROUNDCOLORCOMP(g,1),
                ROUNDCOLORCOMP(b,2)
            );
        }
}

void GrQueryColor(long c,int *r,int *g,int *b)
{
        GrQueryColorID(c,r,g,b);
}

long (GrBlack)(void)
{
        if(CLRINFO->black == GrNOCOLOR) CLRINFO->black = GrAllocColor(0,0,0);
        return(CLRINFO->black);
}

long (GrWhite)(void)
{
        if(CLRINFO->white == GrNOCOLOR) CLRINFO->white = GrAllocColor(255,255,255);
        return(CLRINFO->white);
}

#define CSAVE_MAGIC     0x7abf5698UL

typedef struct {
    unsigned long magic;
    unsigned long nc;
    struct _GR_colorInfo info;
} colorsave;

int GrColorSaveBufferSize(void)
{
        return(sizeof(colorsave));
}

void GrSaveColors(void *buffer)
{
        colorsave *cp = (colorsave *)buffer;
        cp->magic = CSAVE_MAGIC;
        cp->nc    = GrNumColors();
        sttcopy(&cp->info,CLRINFO);
}

void GrRestoreColors(void *buffer)
{
        colorsave *cp = (colorsave *)buffer;
        if((cp->magic == CSAVE_MAGIC) && (cp->nc == GrNumColors())) {
            sttcopy(CLRINFO,&cp->info);
            GrRefreshColors();
        }
}

