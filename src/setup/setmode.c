/**
 ** SETMODE.C ---- video mode setup
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 ** Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 ** Andrzej Lawa [FidoNet: Andrzej Lawa 2:480/19.77]
 **/

#include <ctype.h>
#include <stdarg.h>

#include "libgrx.h"
#include "arith.h"
#include "memfill.h"
#include "memcopy.h"
#include "grdriver.h"

static GrVideoMode *selectmode(GrVideoDriver *drv,int w,int h,int bpp,int txt,uint *ep)
{
#       define ERROR(des,act)   ((des > act) ? ((des - act) + 20000) : (act - des))
        int  n;
        uint cerr,serr,err[2];
        GrVideoMode *best = NULL,*mp = drv->modes;
        for(n = drv->nmodes; --n >= 0; mp++) {
            if(!mp->present) continue;
            if(!mp->extinfo) continue;
            if((mp->extinfo->mode != GR_frameText) ? txt : !txt) continue;
            cerr = ERROR(bpp,mp->bpp);
            serr = ERROR(w,mp->width) + ERROR(h,mp->height);
            if(((ep) ? FALSE : ((ep = err),TRUE)) ||
               ((cerr <  ep[0])) ||
               ((cerr == ep[0]) && (serr < ep[1]))) {
                best  = mp;
                ep[0] = cerr;
                ep[1] = serr;
            }
        }
        if(drv->inherit) {
            mp = selectmode(drv->inherit,w,h,bpp,txt,ep);
            if(mp) best = mp;
        }
        return(best);
}

static int buildframedriver(GrVideoMode *mp,GrFrameDriver *drv)
{
        GrFrameDriver *d1 = _GrFindFrameDriver(mp->extinfo->mode);
        GrFrameDriver *d2 = mp->extinfo->drv;
        if(d1) sttcopy(drv,d1);
        if(d2) {
            int compl = TRUE;
#           define MERGE(F) if(d2->F) drv->F = d2->F; else compl = FALSE;
            MERGE(readpixel);
            MERGE(drawpixel);
            MERGE(drawhline);
            MERGE(drawvline);
            MERGE(drawblock);
            MERGE(drawline);
            MERGE(drawbitmap);
            MERGE(drawpattern);
            MERGE(bitblt);
            MERGE(bltv2r);
            MERGE(bltr2v);
            if(compl) {
                memcopy(drv,d2,offsetof(GrFrameDriver,readpixel));
                return(TRUE);
            }
            if(!d1) return(FALSE);
            if((d2->mode == d1->mode) &&
               (d2->rmode == d1->rmode) &&
               (d1->is_video ? d2->is_video : !d2->is_video) &&
               (d2->row_align <= d1->row_align) && !(d1->row_align % d2->row_align) &&
               (d2->num_planes == d1->num_planes) &&
               (d2->max_plane_size >= d1->max_plane_size) &&
               (d2->bits_per_pixel == d1->bits_per_pixel)) {
                drv->init = d2->init ? d2->init : d1->init;
                return(TRUE);
            }
        }
        if(!d1) return(FALSE);
        sttcopy(drv,d1);
        return(TRUE);
}

static int buildcontext(GrVideoMode *mp,GrFrameDriver *fdp,GrContext *cxt)
{
        long plsize = umul32(mp->lineoffset,mp->height);
        sttzero(cxt);
        if(mp->extinfo->flags&GR_VMODEF_LINEAR)
        {
            cxt->gc_baseaddr[0] =
            cxt->gc_baseaddr[1] =
            cxt->gc_baseaddr[2] =
            cxt->gc_baseaddr[3] = 
#ifdef LFB_BY_NEAR_POINTER
                                  LINP_PTR(mp->extinfo->frame);
            cxt->gc_selector    = LINP_SEL(mp->extinfo->frame);
#else
                                  0;
            cxt->gc_selector    = mp->extinfo->LFB_Selector;
#endif
        }
        else
        {
            if(plsize > fdp->max_plane_size) return(FALSE);
            if(!mp->extinfo->setbank && (plsize > 0x10000L)) return(FALSE);
            if(mp->lineoffset % fdp->row_align) return(FALSE);
            cxt->gc_baseaddr[0] =
            cxt->gc_baseaddr[1] =
            cxt->gc_baseaddr[2] =
            cxt->gc_baseaddr[3] = LINP_PTR(mp->extinfo->frame);
            cxt->gc_selector    = LINP_SEL(mp->extinfo->frame);
        }
        cxt->gc_onscreen    = TRUE;
        cxt->gc_lineoffset  = mp->lineoffset;
        cxt->gc_xcliphi     = cxt->gc_xmax = mp->width  - 1;
        cxt->gc_ycliphi     = cxt->gc_ymax = mp->height - 1;
        cxt->gc_driver      = &DRVINFO->sdriver;
        return(TRUE);
}

static int errhdlr(char *msg)
{
        if(DRVINFO->errsfatal) {
            DRVINFO->moderestore = TRUE;
            _GrCloseVideoDriver();
            fprintf(stderr,"GrSetMode: %s\n",msg);
            exit(1);
        }
        return(FALSE);
}

int GrSetMode(GrGraphicsMode which,...)
{
        int  w,h,pl,vw = 0,vh = 0;
        int  t = FALSE,noclear = FALSE;
        long c;
        va_list ap;
        if(DRVINFO->vdriver == NULL) {
            GrSetDriver(NULL);
            if(DRVINFO->vdriver == NULL) {
                return(errhdlr("could not find suitable video driver"));
            }
        }
        va_start(ap,which);
        switch(which) {
          case GR_NC_80_25_text:
            noclear = TRUE;
          case GR_80_25_text:
            w = 80;
            h = 25;
            c = DRVINFO->deftc;
            t = TRUE;
            break;
          case GR_NC_default_text:
            noclear = TRUE;
          case GR_default_text:
            w = DRVINFO->deftw;
            h = DRVINFO->defth;
            c = DRVINFO->deftc;
            t = TRUE;
            break;
          case GR_NC_width_height_text:
            noclear = TRUE;
          case GR_width_height_text:
            w = va_arg(ap,int);
            h = va_arg(ap,int);
            c = DRVINFO->deftc;
            t = TRUE;
            break;
          case GR_NC_biggest_text:
            noclear = TRUE;
          case GR_biggest_text:
            w = 200;
            h = 80;
            c = DRVINFO->deftc;
            t = TRUE;
            break;
          case GR_NC_width_height_color_text:
            noclear = TRUE;
          case GR_width_height_color_text:
            w = va_arg(ap,int);
            h = va_arg(ap,int);
            c = va_arg(ap,long);
            t = TRUE;
            break;
          case GR_NC_320_200_graphics:
            noclear = TRUE;
          case GR_320_200_graphics:
            w = 320;
            h = 200;
            c = DRVINFO->defgc;
            break;
          case GR_NC_default_graphics:
            noclear = TRUE;
          case GR_default_graphics:
            w = DRVINFO->defgw;
            h = DRVINFO->defgh;
            c = DRVINFO->defgc;
            break;
          case GR_NC_width_height_graphics:
            noclear = TRUE;
          case GR_width_height_graphics:
            w = va_arg(ap,int);
            h = va_arg(ap,int);
            c = DRVINFO->defgc;
            break;
          case GR_NC_biggest_noninterlaced_graphics:
            noclear = TRUE;
          case GR_biggest_noninterlaced_graphics:
            w = 800;
            h = 600;
            c = DRVINFO->defgc;
            break;
          case GR_NC_biggest_graphics:
            noclear = TRUE;
          case GR_biggest_graphics:
            w = 4096;
            h = 4096;
            c = DRVINFO->defgc;
            break;
          case GR_NC_width_height_color_graphics:
            noclear = TRUE;
          case GR_width_height_color_graphics:
            w = va_arg(ap,int);
            h = va_arg(ap,int);
            c = va_arg(ap,long);
            break;
          case GR_NC_custom_graphics:
            noclear = TRUE;
          case GR_custom_graphics:
            w  = va_arg(ap,int);
            h  = va_arg(ap,int);
            c  = va_arg(ap,long);
            vw = va_arg(ap,int);
            vh = va_arg(ap,int);
            break;
          default:
            va_end(ap);
            return(errhdlr("unknown video mode"));
        }
        va_end(ap);
        for(pl = 1; (pl < 32) && ((1UL << pl) < (ulong)c); pl++);
        for( ; ; ) {
            GrContext     cxt;
            GrFrameDriver fdr;
            GrVideoMode  *mdp,vmd;
            mdp = selectmode(DRVINFO->vdriver,w,h,pl,t,NULL);
            if(!mdp) return(errhdlr("could not find suitable video mode"));
            sttcopy(&vmd,mdp);
            if((t || buildframedriver(&vmd,&fdr)) &&
               (t || buildcontext(&vmd,&fdr,&cxt)) &&
               (vmd.extinfo->setup)(&vmd,noclear)) {
                if((!t) &&
                   ((vw > vmd.width) || (vh > vmd.height)) &&
                   (vmd.extinfo->setvsize != NULL) &&
                   (vmd.extinfo->scroll   != NULL)) {
                    int w = vmd.width  = imax(vw,vmd.width);
                    int h = vmd.height = imax(vh,vmd.height);
                    if(!(*vmd.extinfo->setvsize)(&vmd,w,h,&vmd) ||
                       !buildcontext(&vmd,&fdr,&cxt)) {
                        sttcopy(&vmd,mdp);
                        buildcontext(&vmd,&fdr,&cxt);
                        (*vmd.extinfo->setup)(&vmd,noclear);
                    }
                }
                GrMouseUnInit();
                DRVINFO->setbank    = (void (*)(int    ))_GrDummyFunction;
                DRVINFO->setrwbanks = (void (*)(int,int))_GrDummyFunction;
                DRVINFO->curbank    = (-1);
                DRVINFO->splitbanks = FALSE;
                if(!t) {
                    if(vmd.extinfo->setbank) {
                        DRVINFO->setbank = vmd.extinfo->setbank;
                    }
                    if(vmd.extinfo->setrwbanks) {
                        DRVINFO->setrwbanks = vmd.extinfo->setrwbanks;
                        DRVINFO->splitbanks = TRUE;
                    }
                    if(umul32(vmd.lineoffset,vmd.height) <= 0x10000L) {
                        DRVINFO->splitbanks = TRUE;
                    }
                }
                else {
                    sttzero(&cxt);
                    sttcopy(&fdr,&DRVINFO->tdriver);
                    cxt.gc_driver = &DRVINFO->tdriver;
                }
                sttcopy(&CXTINFO->current,&cxt);
                sttcopy(&CXTINFO->screen, &cxt);
                sttcopy(&DRVINFO->fdriver,&fdr);
                sttcopy(&DRVINFO->sdriver,&fdr);
                sttcopy(&DRVINFO->actmode,&vmd);
                DRVINFO->curmode = mdp;
                DRVINFO->mcode   = which;
                DRVINFO->vposx   = 0;
                DRVINFO->vposy   = 0;
                GrResetColors();
                if(fdr.init) (*fdr.init)(&DRVINFO->actmode);
                if(DRVINFO->mdsethook) (*DRVINFO->mdsethook)();
                return(TRUE);
            }
            mdp->present = FALSE;
        }
}

