/**
 ** vd_xfdga.c -- DGA2 driver (interface to X11 DGA2)
 **
 ** Copyright (C) 2003 Dimitar Zhekov
 ** [e-mail: jimmy@is-vn.bg]
 **
 ** This file is part of the GRX graphics library.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 **/

#include "libgrx.h"
#include "libxwin.h"
#include "grdriver.h"
#include "arith.h"
#include "memcopy.h"
#include "memfill.h"

int _XGrWindowedMode = 1;

static int setmode(GrVideoMode *mp,int noclear)
{
        int res;
        XDGADevice *dev;
        GRX_ENTER();
        res = FALSE;
        DBGPRINTF(DBG_DRIVER, ("attempting to set DGA mode %d\n", mp->mode));
        XUngrabPointer(_XGrDisplay, CurrentTime);
        XUngrabKeyboard(_XGrDisplay, CurrentTime);
        dev = XDGASetMode(_XGrDisplay, _XGrScreen, mp->mode);
        if(mp->mode != 0) {
            if(dev == NULL) {
                DBGPRINTF(DBG_DRIVER, ("can't set DGA mode\n"));
                goto done;
            }
            mp->extinfo->frame = dev->data;
            XDGASync(_XGrDisplay, _XGrScreen);
            XDGASetViewport(_XGrDisplay, _XGrScreen, 0, 0, XDGAFlipRetrace);
            XGrabKeyboard(_XGrDisplay, _XGrWindow, True, GrabModeAsync,
                          GrabModeAsync, CurrentTime);
            XGrabPointer(_XGrDisplay, _XGrWindow, True, StructureNotifyMask |
                         ButtonPressMask | ButtonReleaseMask |
                         ButtonMotionMask | PointerMotionMask, GrabModeAsync,
                         GrabModeAsync, None, None, CurrentTime);
            if(mp->bpp == 8) {
                if(_XGrColormap == None) {
                    /* one colormap for all modes - XFreeColormap() crashes */
                    _XGrColormap = XDGACreateColormap(_XGrDisplay, _XGrScreen,
                                                      dev, AllocAll);
                }
                if(_XGrColormap != None) {
                    XDGAInstallColormap(_XGrDisplay, _XGrScreen, _XGrColormap);
                    _GR_lastFreeColor = 255;
                }
            }
            if(!noclear) memzero(dev->data, mp->lineoffset * mp->height);
        }
        res = TRUE;
done:        if(dev != NULL) XFree(dev);
        GRX_RETURN(res);
}

static void reset(void)
{
        XDGADevice *dev;
        GRX_ENTER();
        if(_XGrDisplay != NULL) {
            XUngrabPointer(_XGrDisplay, CurrentTime);
            XUngrabKeyboard(_XGrDisplay, CurrentTime);
            XDGACloseFramebuffer(_XGrDisplay, _XGrScreen);
            dev = XDGASetMode(_XGrDisplay, _XGrScreen, 0);
            if(dev != NULL) XFree(dev);
            XCloseDisplay(_XGrDisplay);
        }
        _XGrDisplay = NULL;
        _XGrColormap = None;
        _GR_firstFreeColor =  0;
        _GR_lastFreeColor  = -1;
        GRX_LEAVE();
}

static int detect(void)
{
        int res;
        int event, error;
        int major, minor;
        GRX_ENTER();
        res = FALSE;
        if (geteuid()) {
            DBGPRINTF(DBG_DRIVER, ("root priviledges required\n"));
            goto done;
        }
        if((_XGrDisplay = XOpenDisplay ("")) == NULL) {
            DBGPRINTF(DBG_DRIVER, ("can't connect to X server\n"));
            goto done;
        }
        if(!XDGAQueryExtension(_XGrDisplay, &event, &error)) {
            DBGPRINTF(DBG_DRIVER, ("can't query DGA extension\n"));
            goto done;
        }
        if(!XDGAQueryVersion(_XGrDisplay, &major, &minor)) {
            DBGPRINTF(DBG_DRIVER, ("can't query DGA version\n"));
            goto done;
        }
        if (major < 2) {
            DBGPRINTF(DBG_DRIVER, ("required DGA version 2.0, detected %d.%d\n",
                                   major, minor));
            goto done;
        }
        res = TRUE;
done:        if(!res) reset();
        GRX_RETURN(res);
}

/* from vd_xwin.c, cut; use 255? */
static void loadcolor(int c,int r,int g,int b)
{
        XColor xcolor;

        if (_XGrColormap != None) {
            xcolor.pixel = c;
            xcolor.red   = r * 257;
            xcolor.green = g * 257;
            xcolor.blue  = b * 257;
            xcolor.flags = DoRed | DoGreen | DoBlue;
            XStoreColor(_XGrDisplay, _XGrColormap, &xcolor);
        }
}

static int build_video_mode(XDGAMode *ip, GrVideoMode *mp, GrVideoModeExt *ep)
{
        mp->present    = TRUE;
        mp->bpp        = ip->depth;
        mp->width      = ip->viewportWidth;
        mp->height     = ip->viewportHeight;
        mp->mode       = ip->num;
        mp->lineoffset = ip->bytesPerScanline;
        mp->privdata   = 0;
        mp->extinfo    = NULL;

        ep->drv        = NULL;
        ep->frame      = NULL;
        ep->flags      = GR_VMODEF_LINEAR;
        ep->setup      = setmode;
        ep->setvsize   = NULL;
        ep->scroll     = NULL;
        ep->setbank    = NULL;
        ep->setrwbanks = NULL;
        ep->loadcolor  = NULL;

        if(ip->visualClass != (mp->bpp == 8 ? PseudoColor : TrueColor)) {
            DBGPRINTF(DBG_DRIVER, ("visual class %s not supported in %dbpp\n",
                                   _XGrClassNames[ip->visualClass], mp->bpp));
            return(FALSE);
        }

        switch(mp->bpp) {
            case 8 :
                ep->mode = GR_frameSVGA8_LFB;
                ep->cprec[0] =
                ep->cprec[1] =
                ep->cprec[2] = 6;
                ep->cpos[0]  =
                ep->cpos[1]  =
                ep->cpos[2]  = 0;
                ep->loadcolor = loadcolor;
                break;
            case 15 :
                ep->mode = GR_frameSVGA16_LFB;
                ep->cprec[0] =
                ep->cprec[1] =
                ep->cprec[2] = 5;
                ep->cpos[0]  = 10;
                ep->cpos[1]  = 5;
                ep->cpos[2]  = 0;
                break;
            case 16 :
                ep->mode = GR_frameSVGA16_LFB;
                ep->cprec[0] = 5;
                ep->cprec[1] = 6;
                ep->cprec[2] = 5;
                ep->cpos[0]  = 11;
                ep->cpos[1]  = 5;
                ep->cpos[2]  = 0;
                break;
            case 24 :
                ep->cprec[0]  =
                ep->cprec[1]  =
                ep->cprec[2]  = 8;
                ep->cpos[0]   = 16;
                ep->cpos[1]   = 8;
                ep->cpos[2]   = 0;
                ep->mode = GR_frameSVGA24_LFB;
                if(ip->bitsPerPixel == 32) {
                    mp->bpp = 32;
                    ep->mode = GR_frameSVGA32L_LFB;
                    if(ip->redMask == 0xFF000000) {
                        ep->cpos[0]   = 24;
                        ep->cpos[1]   = 16;
                        ep->cpos[2]   = 8;
                        ep->mode = GR_frameSVGA32H_LFB;
                    }
                }
                break;
            default:
                return(FALSE);
        }

        return(TRUE);
}

GrVideoModeExt grtextextdga = {
    GR_frameText,                       /* frame driver */
    NULL,                               /* frame driver override */
    NULL,                               /* frame buffer address */
    { 0, 0, 0 },                        /* color precisions */
    { 0, 0, 0 },                        /* color component bit positions */
    0,                                  /* mode flag bits */
    setmode,                            /* mode set */
    NULL,                               /* virtual size set */
    NULL,                               /* virtual scroll */
    NULL,                               /* bank set function */
    NULL,                               /* double bank set function */
    NULL                                /* color loader */
};

#define  NUM_MODES    40                /* max # of supported modes */
#define  NUM_EXTS     10                /* max # of mode extensions */

static GrVideoModeExt exts[NUM_EXTS];
static GrVideoMode   modes[NUM_MODES] = {
    /* pres.  bpp wdt   hgt   BIOS   scan  priv. &ext  */
    {  TRUE,  8,   80,   25,  0x00,    80, 1,    &grtextextdga  },
    {  0  }
};

/* from svgalib.c, unmodified */
static void add_video_mode(
    GrVideoMode *mp,  GrVideoModeExt *ep,
    GrVideoMode **mpp,GrVideoModeExt **epp
) {
        if(*mpp < &modes[NUM_MODES]) {
            if(!mp->extinfo) {
                GrVideoModeExt *etp = &exts[0];
                while(etp < *epp) {
                    if(memcmp(etp,ep,sizeof(GrVideoModeExt)) == 0) {
                        mp->extinfo = etp;
                        break;
                    }
                    etp++;
                }
                if(!mp->extinfo) {
                    if(etp >= &exts[NUM_EXTS]) return;
                    sttcopy(etp,ep);
                    mp->extinfo = etp;
                    *epp = ++etp;
                }
            }
            sttcopy(*mpp,mp);
            (*mpp)++;
        }
}

static int init(char *options)
{
        int res;
        XDGAMode *modev = NULL;
        int modec, mindex;
        GrVideoMode mode, *modep = &modes[1];
        GrVideoModeExt ext, *extp = &exts[0];
        GRX_ENTER();
        res = FALSE;
        if(detect()) {
            _XGrScreen = DefaultScreen(_XGrDisplay);
            _XGrWindow = DefaultRootWindow(_XGrDisplay);
            if(!XDGAOpenFramebuffer(_XGrDisplay, _XGrScreen)) {
                DBGPRINTF(DBG_DRIVER, ("can't open framebuffer\n"));
                goto done;
            }
            modev = XDGAQueryModes(_XGrDisplay, _XGrScreen, &modec);
            if(modev == NULL) {
                DBGPRINTF(DBG_DRIVER, ("can't query DGA modes"));
                goto done;
            }
            memzero(modep,(sizeof(modes) - sizeof(modes[0])));
            for(mindex = 0; mindex < modec; mindex++) {
                if(!build_video_mode(&modev[mindex], &mode, &ext)) continue;
                add_video_mode(&mode,&ext,&modep,&extp);
            }
            _XGrWindowedMode = 0;
            _XGrColormap = None;
            res = TRUE;
        }
done:        if(modev != NULL) XFree(modev);
        if(!res) reset();
        GRX_RETURN(res);
}

GrVideoDriver _GrVideoDriverXF86DGA = {
    "xf86dga",                          /* name */
    GR_XWIN,                            /* adapter type */
    NULL,                               /* inherit modes from this driver */
    modes,                              /* mode table */
    itemsof(modes),                     /* # of modes */
    detect,                             /* detection routine */
    init,                               /* initialization routine */
    reset,                              /* reset routine */
    _gr_selectmode,                     /* standard mode select routine */
    0                                   /* no additional capabilities */
};

