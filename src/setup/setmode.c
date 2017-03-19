/*
 * setmode.c ---- video mode setup
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
 * Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 * Andrzej Lawa [FidoNet: Andrzej Lawa 2:480/19.77]
 *
 * This file is part of the GRX graphics library.
 *
 * The GRX graphics library is free software; you can redistribute it
 * and/or modify it under some conditions; see the "copying.grx" file
 * for details.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <ctype.h>
#include <stdarg.h>
#include <string.h>

#include "colors.h"
#include "grx/error.h"
#include "globals.h"
#include "libgrx.h"
#include "arith.h"
#include "memfill.h"
#include "memcopy.h"
#include "grdriver.h"
#include "util.h"
#include "../mouse/input.h"

GrxVideoMode * _gr_select_mode(GrxVideoDriver *drv,int w,int h,int bpp,
                              int txt,unsigned int *ep)
{
#       define ERROR(des,act)   ((des > act) ? ((des - act) + 20000) : (act - des))
        int  n;
        unsigned int cerr,serr,err[2];
        GrxVideoMode *best,*mp;
        GRX_ENTER();
        g_debug ("Attempting to set mode in %s", drv->name);
        best = NULL;
        mp = drv->modes;
        for(n = drv->n_modes; --n >= 0; mp++) {
            if (!mp->present) {
                g_debug ("Mode %d not present", n);
                continue;
            }
            if (!mp->extended_info) {
                g_debug ("Mode %d missing extended info", n);
                continue;
            }
            if ((mp->extended_info->mode != GRX_FRAME_MODE_TEXT) ? txt : !txt) {
                g_debug ("Mode %d txt/grx mismatch", n);
                continue;
            }
            cerr = ERROR(bpp,mp->bpp);
            serr = ERROR(w,mp->width) + ERROR(h,mp->height);
            if(((ep) ? FALSE : ((ep = err),TRUE)) ||
               ((cerr <  ep[0])) ||
               ((cerr == ep[0]) && (serr < ep[1]))) {
                g_debug ("Mode %d OK!", n);
                best  = mp;
                if (!cerr && !serr) break;
                ep[0] = cerr;
                ep[1] = serr;
            } else {
                g_debug ("Mode %d was not the best", n);
            }
        }
        if(drv->inherit) {
            mp = (drv->inherit->select_mode) (drv->inherit,w,h,bpp,txt,ep);
            if(mp) best = mp;
        }
        GRX_RETURN(best);
}

static int buildframedriver(GrxVideoMode *mp,GrxFrameDriver *drv)
{
        GrxFrameDriver *d1, *d2;
        int res = TRUE;
        GRX_ENTER();
        res = TRUE;
        d1 = _GrFindFrameDriver(mp->extended_info->mode);
        d2 = mp->extended_info->drv;
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
            MERGE(getindexedscanline);
            MERGE(putscanline);
            if(compl) {
                memcpy(drv,d2,offsetof(GrxFrameDriver,readpixel));
                goto done; /* TRUE */
            }
            if (!d1) {
                g_debug ("Could not find framedriver");
                res = FALSE;
                goto done;
            }
            if((d2->mode == d1->mode) &&
               (d2->rmode == d1->rmode) &&
               (d1->is_video ? d2->is_video : !d2->is_video) &&
               (d2->row_align <= d1->row_align) && !(d1->row_align % d2->row_align) &&
               (d2->num_planes == d1->num_planes) &&
               (d2->max_plane_size >= d1->max_plane_size) &&
               (d2->bits_per_pixel == d1->bits_per_pixel)) {
                drv->init = d2->init ? d2->init : d1->init;
                goto done; /* TRUE */
            }
        }
        if (!d1) {
            g_debug ("Could not find framedriver");
            res = FALSE;
            goto done;
        }
        sttcopy(drv,d1);
done:   GRX_RETURN(res);
}

static int buildcontext(GrxVideoMode *mp,GrxFrameDriver *fdp,GrxContext *cxt)
{
        long plsize;
        int res;
        GRX_ENTER();
        res = FALSE;
        plsize = umul32(mp->line_offset,mp->height);
        g_debug ("buildcontext - Mode Frame buffer = %p",
                 mp->extended_info->frame);
        g_debug ("buildcontext - Mode Frame selector = 0x%x",
                 mp->extended_info->lfb_selector);
        g_debug ("plane size: %ld", plsize);
        sttzero(cxt);
#if !(defined(__XWIN__) && !defined(XF86DGA_FRAMEBUFFER) && !defined(__SDL__))
        if(mp->extended_info->flags&GRX_VIDEO_MODE_FLAG_LINEAR)
        {
            g_debug ("buildcontext - Linear Mode");
            cxt->gc_base_address.plane0 =
            cxt->gc_base_address.plane1 =
            cxt->gc_base_address.plane2 =
            cxt->gc_base_address.plane3 = LINP_PTR(mp->extended_info->frame);
            cxt->gc_selector    = mp->extended_info->lfb_selector;
        } else
#endif /* !(__XWIN__ && !XF86DGA_FRAMEBUFFER && !__SDL__) */
        if (mp->extended_info->flags&GRX_VIDEO_MODE_FLAG_MEMORY)
        {
            g_debug ("buildcontext - Memory Mode");
            if (plsize > fdp->max_plane_size) {
                g_debug ("plane size too big");
                goto done; /* FALSE */
            }
            if (mp->line_offset % fdp->row_align) {
                g_debug ("offset does not match alignment");
                goto done; /* FALSE */
            }
            g_debug ("buildcontext - mp->present     = %d",mp->present);
            g_debug ("buildcontext - mp->bpp         = %d",mp->bpp);
            g_debug ("buildcontext - mp->width       = %d",mp->width);
            g_debug ("buildcontext - mp->height      = %d",mp->height);
            g_debug ("buildcontext - mp->mode        = %d",mp->mode);
            g_debug ("buildcontext - mp->line_offset = %d",mp->line_offset);
            g_debug ("buildcontext - mp->ext->mode   = %d",mp->extended_info->mode);
            g_debug ("buildcontext - mp->ext->flags  = 0x%x",mp->extended_info->flags);
#ifdef GRX_USE_RAM3x8
            if (mp->bpp==24)
              {
                 int m_incr = mp->line_offset*mp->height;
                 cxt->gc_base_address.plane0 = mp->extended_info->frame;
                 cxt->gc_base_address.plane1 = cxt->gc_base_address.plane0 + m_incr;
                 cxt->gc_base_address.plane2 = cxt->gc_base_address.plane1 + m_incr;
                 cxt->gc_base_address.plane3 = NULL;
              }
            else
#endif
            if (mp->bpp==4)
              {
                 int m_incr = mp->line_offset*mp->height;
                 cxt->gc_base_address.plane0 = mp->extended_info->frame;
                 cxt->gc_base_address.plane1 = cxt->gc_base_address.plane0 + m_incr;
                 cxt->gc_base_address.plane2 = cxt->gc_base_address.plane1 + m_incr;
                 cxt->gc_base_address.plane3 = cxt->gc_base_address.plane2 + m_incr;
              }
            else
              {
                 cxt->gc_base_address.plane0 =
                 cxt->gc_base_address.plane1 =
                 cxt->gc_base_address.plane2 =
                 cxt->gc_base_address.plane3 = mp->extended_info->frame;
              }
        } else {
            if (plsize > fdp->max_plane_size) {
                g_debug ("plane size too big");
                goto done; /* FALSE */
            }
            if (!mp->extended_info->set_bank && (plsize > 0x10000L)) {
                g_debug ("plane size exceeds 64KiB");
                goto done; /* FALSE */
            }
            if (mp->line_offset % fdp->row_align) {
                g_debug ("offset does not match alignment");
                goto done; /* FALSE */
            }
            cxt->gc_base_address.plane0 =
            cxt->gc_base_address.plane1 =
            cxt->gc_base_address.plane2 =
            cxt->gc_base_address.plane3 = LINP_PTR(mp->extended_info->frame);
            cxt->gc_selector    = LINP_SEL(mp->extended_info->frame);
        }
        cxt->gc_is_on_screen    = !(mp->extended_info->flags&GRX_VIDEO_MODE_FLAG_MEMORY);
        /* Why do we default to screen driver ?? */
        cxt->gc_is_on_screen    = TRUE;
        cxt->gc_line_offset  = mp->line_offset;
        cxt->x_clip_high     = cxt->x_max = mp->width  - 1;
        cxt->y_clip_high     = cxt->y_max = mp->height - 1;
        cxt->gc_driver      = &DRVINFO->sdriver;

        res = TRUE;

        g_debug ("buildcontext - context buffer 0 = %p",cxt->gc_base_address.plane0);
        g_debug ("buildcontext - context buffer 1 = %p",cxt->gc_base_address.plane1);
        g_debug ("buildcontext - context buffer 2 = %p",cxt->gc_base_address.plane2);
        g_debug ("buildcontext - context buffer 3 = %p",cxt->gc_base_address.plane3);
done:
        GRX_RETURN(res);
}

/**
 * grx_set_mode: (skip)
 * @mode: The graphics mode.
 * @error: Error pointer.
 * @...: Extra parameters determined by @mode.
 *
 * Set the graphics mode.
 *
 * If a driver has not already been set, this will attempt to load the best
 * available driver (the one with the most modes). The driver will then try to
 * find the mode that best fits the size and color depth parameters. The exact
 * behavior will depend on the video driver.
 *
 * Returns: %TRUE on success, otherwise %FALSE.
 */
int grx_set_mode(GrxGraphicsMode which, GError **error, ...)
{
        int  w,h,pl,vw,vh;
        int  t,noclear,res;
        GrxColor c;
        va_list ap;
        GRX_ENTER();
        pl = 0;
        vw = 0;
        vh = 0;
        t = FALSE;
        noclear = FALSE;
        c = 0;
        res = FALSE;
        g_debug ("Mode: %d",(int)which);
        if (!DRVINFO->vdriver) {
            res = grx_set_driver(NULL, error);
            if (!res) {
                goto done;
            }
        }
        va_start(ap,error);
        switch(which) {
          case GRX_GRAPHICS_MODE_TEXT_80X25_NC:
            noclear = TRUE;
          case GRX_GRAPHICS_MODE_TEXT_80X25:
            w = 80;
            h = 25;
            c = DRVINFO->deftc;
            t = TRUE;
            break;
          case GRX_GRAPHICS_MODE_TEXT_DEFAULT_NC:
            noclear = TRUE;
          case GRX_GRAPHICS_MODE_TEXT_DEFAULT:
            w = DRVINFO->deftw;
            h = DRVINFO->defth;
            c = DRVINFO->deftc;
            t = TRUE;
            break;
          case GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_NC:
            noclear = TRUE;
          case GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT:
            w = va_arg(ap,int);
            h = va_arg(ap,int);
            c = DRVINFO->deftc;
            t = TRUE;
            break;
          case GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_COLOR_NC:
            noclear = TRUE;
          case GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_COLOR:
            w = va_arg(ap,int);
            h = va_arg(ap,int);
            c = va_arg(ap,GrxColor);
            t = TRUE;
            break;
          case GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_BPP_NC:
            noclear = TRUE;
          case GRX_GRAPHICS_MODE_TEXT_WIDTH_HEIGHT_BPP:
            w  = va_arg(ap,int);
            h  = va_arg(ap,int);
            pl = va_arg(ap,int);
            t  = TRUE;
            break;
          case GRX_GRAPHICS_MODE_GRAPHICS_DEFAULT_NC:
            noclear = TRUE;
          case GRX_GRAPHICS_MODE_GRAPHICS_DEFAULT:
            w = DRVINFO->defgw;
            h = DRVINFO->defgh;
            c = DRVINFO->defgc;
            break;
          case GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_NC:
            noclear = TRUE;
          case GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT:
            w = va_arg(ap,int);
            h = va_arg(ap,int);
            c = DRVINFO->defgc;
            break;
          case GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_COLOR_NC:
            noclear = TRUE;
          case GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_COLOR:
            w = va_arg(ap,int);
            h = va_arg(ap,int);
            c = va_arg(ap,GrxColor);
            break;
          case GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_BPP_NC:
            noclear = TRUE;
          case GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_BPP:
            w  = va_arg(ap,int);
            h  = va_arg(ap,int);
            pl = va_arg(ap,int);
            break;
          case GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM_NC:
            noclear = TRUE;
          case GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM:
            w  = va_arg(ap,int);
            h  = va_arg(ap,int);
            c  = va_arg(ap,GrxColor);
            vw = va_arg(ap,int);
            vh = va_arg(ap,int);
            break;
          case GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM_BPP_NC:
            noclear = TRUE;
          case GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM_BPP:
            w  = va_arg(ap,int);
            h  = va_arg(ap,int);
            pl = va_arg(ap,int);
            vw = va_arg(ap,int);
            vh = va_arg(ap,int);
            break;
          default:
            va_end(ap);
            res = FALSE;
            g_set_error(error, GRX_ERROR, GRX_ERROR_FAILED, "unknown video mode");
            goto done;
        }
        va_end(ap);
        if (c)
          for(pl = 1; (pl < 32) && ((1UL << pl) < (GrxColor)c); pl++) ;
        for( ; ; ) {
            GrxContext     cxt;
            GrxFrameDriver fdr;
            GrxVideoMode  *mdp,vmd;
            mdp = (DRVINFO->vdriver->select_mode)(DRVINFO->vdriver,w,h,pl,t,NULL);
            if(!mdp) {
                res = FALSE;
                g_set_error(error, GRX_ERROR, GRX_ERROR_FAILED,
                            "could not find suitable video mode");
                goto done;
            }
            sttcopy(&vmd,mdp);
            if((t || buildframedriver(&vmd,&fdr)) &&
               (*vmd.extended_info->setup)(&vmd,noclear) &&
               (t || buildcontext(&vmd,&fdr,&cxt))) {
                if((!t) &&
                   ((vw > vmd.width) || (vh > vmd.height)) &&
                   (vmd.extended_info->set_virtual_size != NULL) &&
                   (vmd.extended_info->scroll   != NULL)) {
                    int ww = vmd.width  = imax(vw,vmd.width);
                    int hh = vmd.height = imax(vh,vmd.height);
                    if(!(*vmd.extended_info->set_virtual_size)(&vmd,ww,hh,&vmd) ||
                       !buildcontext(&vmd,&fdr,&cxt)) {
                        sttcopy(&vmd,mdp);
                        buildcontext(&vmd,&fdr,&cxt);
                        (*vmd.extended_info->setup)(&vmd,noclear);
                    }
                }
                DRVINFO->set_bank    = (void (*)(int    ))_GrDummyFunction;
                DRVINFO->set_rw_banks = (void (*)(int,int))_GrDummyFunction;
                DRVINFO->curbank    = (-1);
                DRVINFO->splitbanks = FALSE;
                if(!t) {
                    if(vmd.extended_info->set_bank) {
                        DRVINFO->set_bank = vmd.extended_info->set_bank;
                    }
                    if(vmd.extended_info->set_rw_banks) {
                        DRVINFO->set_rw_banks = vmd.extended_info->set_rw_banks;
                        DRVINFO->splitbanks = TRUE;
                    }
                    if(umul32(vmd.line_offset,vmd.height) <= 0x10000L) {
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
                g_debug ("grx_color_info_reset_colors ...");
                if ( !_GrResetColors() ) {
                    res = FALSE;
                    g_set_error(error, GRX_ERROR, GRX_ERROR_FAILED,
                                "could not set color mode");
                    goto done;
                }
                g_debug ("grx_color_info_reset_colors done");
                if(fdr.init) {
                    g_debug ("fdr.init ...");
                    (*fdr.init)(&DRVINFO->actmode);
                    g_debug ("fdr.init done");
                }
                g_debug ("grx_set_mode complete");
                res = TRUE;
                g_debug ("_GrInitMouseCursor ...");
                _GrInitMouseCursor();
                g_debug ("_GrInitMouseCursor done");
                goto done;
            }
            g_debug ("disabling mode due to failed setup");
            mdp->present = FALSE;
        }
done:   GRX_RETURN(res);
}

/**
 * grx_set_mode_default_graphics:
 * @clear: If %TRUE, clear the screen after setting the mode.
 * @error: Error pointer.
 *
 * Set the mode to the default graphics mode.
 *
 * This is 640x480x16 by default but can be changed by grx_set_driver().
 *
 * Returns: %TRUE on success, otherwise %FALSE.
 */
gboolean grx_set_mode_default_graphics(gboolean clear, GError **error)
{
    return grx_set_mode(clear ? GRX_GRAPHICS_MODE_GRAPHICS_DEFAULT
                              : GRX_GRAPHICS_MODE_GRAPHICS_DEFAULT_NC, error);
}
