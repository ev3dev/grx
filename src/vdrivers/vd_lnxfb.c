/*
 * vd_lnxfb.c ---- Linux framebuffer driver
 *
 * Copyright (c) 2001 Mariano Alvarez Fernandez <malfer@telefonica.net>
 * Copyright (c) 2015-2016 David Lechner <david@lechnology.com>
 *
 * Contributions by Josu Onandia <jonandia@fagorautomation.es>
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

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
#include <linux/vt.h>

#include "libgrx.h"
#include "grdriver.h"
#include "arith.h"
#include "memcopy.h"
#include "memfill.h"

#define  NUM_MODES    80        /* max # of supported modes */
#define  NUM_EXTS     15        /* max # of mode extensions */

static int initted = -1;
static int fbfd = -1;
static int ttyfd = -1;
static struct fb_fix_screeninfo fbfix;
static struct fb_var_screeninfo fbvar;
static unsigned char *fbuffer = NULL;
static int ingraphicsmode = 0;
static int original_keyboard_mode;

static int detect(void)
{
    struct vt_stat vtstat;
    struct fb_con2fbmap con2fb_map;
    char ttyname[12];
    char fbname[10];
    int err;

    if (initted < 0) {
        initted = 0;

        // first, open the current console

        ttyfd = open("/dev/tty", O_RDONLY);
        if (ttyfd == -1) {
            err = ttyfd;
        } else {
            // This will fail if we are not on a virtual console
            err = ioctl(ttyfd, VT_GETSTATE, &vtstat);
            close(ttyfd);
        }
        if (err < 0) {
            // If we are not currently on a virtual console and we are root,
            // then we will take over the current virtual console (tty0).
            ttyfd = open("/dev/tty0", O_RDONLY);
            if (ttyfd == -1) {
                return FALSE;
            }
            err = ioctl(ttyfd, VT_GETSTATE, &vtstat);
            close(ttyfd);
            if (err < 0) {
                return FALSE;
            }
        }

        // Now, open the specific tty file for this console. We do this instead
        // of using /dev/tty because if we switch active consoles, /dev/tty is
        // no longer the one we started with.

        sprintf(ttyname, "/dev/tty%d", vtstat.v_active);
        ttyfd = open(ttyname, O_RDONLY);
        if (ttyfd == -1) {
            return FALSE;
        }

        err = ioctl(ttyfd, KDGKBMODE, &original_keyboard_mode);
        if (err < 0) {
            close(ttyfd);
            return FALSE;
        }

        // Then open up the first framebuffer device and use it to find the
        // fbdev that matches this console.

        fbfd = open("/dev/fb0", O_RDWR);
        if (fbfd == -1) {
            close(ttyfd);
            return FALSE;
        }
        con2fb_map.console = vtstat.v_active;
        if (ioctl(fbfd, FBIOGET_CON2FBMAP, &con2fb_map) < 0) {
            close(fbfd);
            close(ttyfd);
            return FALSE;
        }

        // Make sure there is actually a framebuffer for this console.

        if (con2fb_map.framebuffer < 0) {
            close(fbfd);
            close(ttyfd);
            return FALSE;
        }

        // We already have /dev/fb0 open. If it is not the right one then we
        // need to close it and open the correct one.

        if (con2fb_map.framebuffer != 0) {
            close(fbfd);
            sprintf(fbname, "/dev/fb%d", con2fb_map.framebuffer);
            fbfd = open(fbname, O_RDWR);
            if (fbfd < 0) {
                close(ttyfd);
                return FALSE;
            }
        }

        // Finally, we have to make sure the frambuffer is compatible.

        ioctl(fbfd, FBIOGET_FSCREENINFO, &fbfix);
        ioctl(fbfd, FBIOGET_VSCREENINFO, &fbvar);
        if (fbfix.type != FB_TYPE_PACKED_PIXELS) {
            close(fbfd);
            close(ttyfd);
            return FALSE;
        }

        initted = 1;
    }

    return (initted > 0);
}

static void reset(void)
{
    struct vt_mode vtm;

    if (fbuffer) {
        memzero(fbuffer, fbvar.yres * fbfix.line_length);
        munmap(fbuffer, fbfix.smem_len);
        fbuffer = NULL;
    }
    if (fbfd != -1) {
        close(fbfd);
        fbfd = -1;
    }
    if (ttyfd > -1) {
        ioctl(ttyfd, KDSKBMODE, original_keyboard_mode);
        ioctl(ttyfd, KDSETMODE, KD_TEXT);
        vtm.mode = VT_AUTO;
        vtm.relsig = 0;
        vtm.acqsig = 0;
        ioctl(ttyfd, VT_SETMODE, &vtm);
        close(ttyfd);
        ttyfd = -1;
        ingraphicsmode = 0;
    }
    initted = -1;
}

/* release control of the vt */
void grx_linuxfb_release (void)
{
    if (!ingraphicsmode) {
        return;
    }
    if (ttyfd < 0) {
        return;
    }
    ioctl(ttyfd, VT_RELDISP, 1);
}

/* resume control of the vt */
void grx_linuxfb_aquire (void)
{
    if (!ingraphicsmode) return;
    if (ttyfd < 0) return;
    ioctl(ttyfd, VT_RELDISP, VT_ACKACQ);
    ioctl(ttyfd, KDSKBMODE, K_OFF);
    ioctl(ttyfd, KDSETMODE, KD_GRAPHICS);
}

void grx_linuxfb_chvt (int vt_num)
{
    if (!ingraphicsmode) {
        return;
    }
    if (ttyfd < 0) {
        return;
    }
    ioctl(ttyfd, VT_ACTIVATE, vt_num);
}

static void load_color(GrxColor c, GrxColor r, GrxColor g, GrxColor b)
{
    __u16 red, green, blue, transp;
    struct fb_cmap cmap;

    red = (r << 8);
    green = (g << 8);
    blue = (b << 8);
    transp = 0;
    cmap.start = c;
    cmap.len = 1;
    cmap.red = &red;
    cmap.green = &green;
    cmap.blue = &blue;
    cmap.transp = &transp;
    ioctl(fbfd, FBIOPUTCMAP, &cmap);
}

static int setmode(GrxVideoMode * mp, int noclear)
{
    struct vt_mode vtm;

    fbuffer = mp->extended_info->frame = mmap(0,
                                        fbfix.smem_len,
                                        PROT_READ | PROT_WRITE,
                                        MAP_SHARED, fbfd, 0);
    if (mp->extended_info->frame && ttyfd > -1) {
        ioctl(ttyfd, KDSKBMODE, K_OFF);
        ioctl(ttyfd, KDSETMODE, KD_GRAPHICS);
        vtm.mode = VT_PROCESS;
        // Setting SIGUSER1 here, but not a handler. It is to be handled via
        // GrxLinuxConsoleApplication or user code by calling grx_linuxfb_release
        // and grx_linuxfb_aquire.
        vtm.relsig = SIGUSR1;
        vtm.acqsig = SIGUSR1;
        ioctl(ttyfd, VT_SETMODE, &vtm);
        ingraphicsmode = 1;
    }
    if (mp->extended_info->frame && !noclear)
        memzero(mp->extended_info->frame, fbvar.yres * fbfix.line_length);
    return ((mp->extended_info->frame) ? TRUE : FALSE);
}

static int settext(GrxVideoMode * mp, int noclear)
{
    struct vt_mode vtm;

    if (fbuffer) {
        memzero(fbuffer, fbvar.yres * fbfix.line_length);
        munmap(fbuffer, fbfix.smem_len);
        fbuffer = NULL;
    }
    if (ttyfd > -1) {
        ioctl(ttyfd, KDSETMODE, KD_TEXT);
        vtm.mode = VT_AUTO;
        vtm.relsig = 0;
        vtm.acqsig = 0;
        ioctl(ttyfd, VT_SETMODE, &vtm);
        ingraphicsmode = 0;
    }
    return TRUE;
}

GrxVideoModeExt grtextextfb = {
    .mode             = GRX_FRAME_MODE_TEXT, /* frame driver */
    .drv              = NULL,                /* frame driver override */
    .frame            = NULL,                /* frame buffer address */
    .cprec            = {6, 6, 6},           /* color precisions */
    .cpos             = {0, 0, 0},           /* color component bit positions */
    .flags            = 0,                   /* mode flag bits */
    .setup            = settext,             /* mode set */
    .set_virtual_size = NULL,                /* virtual size set */
    .scroll           = NULL,                /* virtual scroll */
    .set_bank         = NULL,                /* bank set function */
    .set_rw_banks     = NULL,                /* double bank set function */
    .load_color       = NULL,                /* color loader */
};

static GrxVideoModeExt exts[NUM_EXTS];
static GrxVideoMode modes[NUM_MODES] = {
    /* pres.  bpp wdt   hgt   mode   scan  priv. &ext                             */
    {TRUE, 4, 80, 25, 0, 160, 0, &grtextextfb},
    {0}
};

static int build_video_mode(GrxVideoMode * mp, GrxVideoModeExt * ep)
{
    mp->present = TRUE;
    mp->width = fbvar.xres;
    mp->height = fbvar.yres;
    mp->line_offset = fbfix.line_length;
    mp->extended_info = NULL;
    mp->user_data = 0;
    ep->drv = NULL;
    ep->frame = NULL;                /* filled in after mode set */
    ep->flags = 0;
    ep->setup = setmode;
    ep->set_virtual_size = NULL;        /* tbd */
    ep->scroll = NULL;                /* tbd */
    ep->set_bank = NULL;
    ep->set_rw_banks = NULL;
    ep->load_color = NULL;
    switch (fbvar.bits_per_pixel) {
    case 1:
        if (fbfix.visual == FB_VISUAL_MONO01)
            ep->mode = GRX_FRAME_MODE_LFB_MONO01;
        else if (fbfix.visual == FB_VISUAL_MONO10)
            ep->mode = GRX_FRAME_MODE_LFB_MONO10;
        else
            return FALSE;
    break;
    case 8:
        if (fbfix.visual != FB_VISUAL_PSEUDOCOLOR)
            return FALSE;
        ep->mode = GRX_FRAME_MODE_LFB_8BPP;
        ep->load_color = load_color;
        break;
    case 15:
    case 16:
        if (fbfix.visual != FB_VISUAL_TRUECOLOR)
            return FALSE;
        ep->mode = GRX_FRAME_MODE_LFB_16BPP;
        break;
    case 24:
        if (fbfix.visual != FB_VISUAL_TRUECOLOR)
            return FALSE;
        ep->mode = GRX_FRAME_MODE_LFB_24BPP;
        break;
    default:
        return (FALSE);
    }
    mp->bpp = fbvar.bits_per_pixel;
    ep->flags |= GRX_VIDEO_MODE_FLAG_LINEAR;
    ep->cprec[0] = fbvar.red.length;
    ep->cprec[1] = fbvar.green.length;
    ep->cprec[2] = fbvar.blue.length;
    ep->cpos[0] = fbvar.red.offset;
    ep->cpos[1] = fbvar.green.offset;
    ep->cpos[2] = fbvar.blue.offset;
    return (TRUE);
}

static void add_video_mode(GrxVideoMode * mp, GrxVideoModeExt * ep,
                           GrxVideoMode ** mpp, GrxVideoModeExt ** epp)
{
    if (*mpp < &modes[NUM_MODES]) {
        if (!mp->extended_info) {
            GrxVideoModeExt *etp = &exts[0];
            while (etp < *epp) {
                if (memcmp(etp, ep, sizeof(GrxVideoModeExt)) == 0) {
                    mp->extended_info = etp;
                    break;
                }
                etp++;
            }
            if (!mp->extended_info) {
                if (etp >= &exts[NUM_EXTS])
                    return;
                sttcopy(etp, ep);
                mp->extended_info = etp;
                *epp = ++etp;
            }
        }
        sttcopy(*mpp, mp);
        (*mpp)++;
    }
}

static int init(char *options)
{
    if (detect()) {
        GrxVideoMode mode, *modep = &modes[1];
        GrxVideoModeExt ext, *extp = &exts[0];
        memzero(modep, (sizeof(modes) - sizeof(modes[0])));
        if ((build_video_mode(&mode, &ext))) {
            add_video_mode(&mode, &ext, &modep, &extp);
        }
        return (TRUE);
    }
    return (FALSE);
}

GrxVideoDriver _GrVideoDriverLINUXFB = {
    .name        = "linuxfb",                   /* name */
    .adapter     = GRX_VIDEO_ADAPTER_LINUX_FB,  /* adapter type */
    .inherit     = NULL,                        /* inherit modes from this driver */
    .modes       = modes,                       /* mode table */
    .n_modes     = itemsof(modes),              /* # of modes */
    .detect      = detect,                      /* detection routine */
    .init        = init,                        /* initialization routine */
    .reset       = reset,                       /* reset routine */
    .select_mode = _gr_select_mode,             /* standard mode select routine */
    .flags       = 0,                           /* no additional capabilities */
};
