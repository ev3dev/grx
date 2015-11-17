/**
 ** vd_lnxfb.c ---- Linux framebuffer driver
 **
 ** Copyright (c) 2001 Mariano Alvarez Fernandez
 ** [e-mail: malfer@telefonica.net]
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
 ** Contributions by Josu Onandia (jonandia@fagorautomation.es) 13/12/2002
 **   - Added the 8bpp paletted mode.
 ** Modifications by Mariano Alvarez Fernandez 21/12/2002
 **   - Added function to change virtual terminals, _SwitchConsoleLnxfbDriver
 **     to be called from the input driver.
 **   - Some cleanups, now the text screen shows ok on exit.
 ** Modifications by Mariano Alvarez Fernandez 1/3/2002
 **   - Added code to catch a signal when user wants to change virtual
 **     terminals. The driver sets _lnxfb_waiting_to_switch_console and the
 **     input driver must calls _LnxfbSwitchConsoleAndWait then.
 **   - _SwitchConsoleLnxfbDriver renamed to _LnxfbSwitchConsoleVt, not used,
 **     is here only for possible future use.
 **/

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
static char *fbuffer = NULL;
static int ingraphicsmode = 0;
static GrContext *grc;
static char* frame_addr[4];

int _lnxfb_waiting_to_switch_console = 0;

static int detect(void)
{
    char *fbname;
    char *default_fbname = "/dev/fb0";
    char ttyname[12];
    int err;
    struct vt_stat vtstat;

    if (initted < 0) {
        initted = 0;
        fbname = getenv("FRAMEBUFFER");
        if (fbname == NULL)
            fbname = default_fbname;
        fbfd = open(fbname, O_RDWR);
        if (fbfd == -1)
            return FALSE;
        ioctl(fbfd, FBIOGET_FSCREENINFO, &fbfix);
        ioctl(fbfd, FBIOGET_VSCREENINFO, &fbvar);
        if (fbfix.type != FB_TYPE_PACKED_PIXELS)
            return FALSE;
        ttyfd = open("/dev/tty", O_RDONLY);
    if (ttyfd == -1)
        err = ttyfd;
    else
        err = ioctl(ttyfd, VT_GETSTATE, &vtstat);
        if (err < 0) {
        // if we are not currently on a virtual console and we are root, then
        // we will take over the current virtual console (tty0).
        ttyfd = open("/dev/tty0", O_RDONLY);
        if (ttyfd == -1)
               return FALSE;
        err = ioctl(ttyfd, VT_GETSTATE, &vtstat);
        if (err < 0)
            return FALSE;
    }
        sprintf(ttyname, "/dev/tty%d", vtstat.v_active);
        ttyfd = open(ttyname, O_RDONLY);
        if (ttyfd == -1)
            return FALSE;
        initted = 1;
    }
    return ((initted > 0) ? TRUE : FALSE);
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
        ioctl(ttyfd, KDSETMODE, KD_TEXT);
        vtm.mode = VT_AUTO;
        vtm.relsig = 0;
        vtm.acqsig = 0;
        ioctl(ttyfd, VT_SETMODE, &vtm);
        signal(SIGUSR1, SIG_IGN);
        close(ttyfd);
        ttyfd = -1;
        ingraphicsmode = 0;
    }
    initted = -1;
}

void _LnxfbSwitchToConsoleVt(unsigned short vt)
{
    struct vt_stat vtst;
    unsigned short myvt;
    GrContext *grc;

    if (!ingraphicsmode) return;
    if (ttyfd < 0) return;
    if (ioctl(ttyfd, VT_GETSTATE, &vtst) < 0) return;
    myvt = vtst.v_active;
    if (vt == myvt) return; 

    grc = GrCreateContext(GrScreenX(), GrScreenY(), NULL, NULL);
    if (grc != NULL) {
        GrBitBlt(grc, 0, 0, GrScreenContext(), 0, 0,
                 GrScreenX()-1, GrScreenY()-1, GrWRITE);
    }
    ioctl(ttyfd, KDSETMODE, KD_TEXT);
    if (ioctl(ttyfd, VT_ACTIVATE, vt) == 0) {
        ioctl(ttyfd, VT_WAITACTIVE, vt);
        ioctl(ttyfd, VT_WAITACTIVE, myvt);
    }
    ioctl(ttyfd, KDSETMODE, KD_GRAPHICS);
    if (grc != NULL) {
        GrBitBlt(GrScreenContext(), 0, 0, grc, 0, 0,
                 GrScreenX()-1, GrScreenY()-1, GrWRITE);
        GrDestroyContext(grc);
    }
}

void _LnxfbSwitchConsoleAndWait(void)
{
    struct vt_stat vtst;
    unsigned short myvt;
    GrContext *grc;

    _lnxfb_waiting_to_switch_console = 0;
    if (!ingraphicsmode) return;
    if (ttyfd < 0) return;
    if (ioctl(ttyfd, VT_GETSTATE, &vtst) < 0) return;
    myvt = vtst.v_active;

    grc = GrCreateContext(GrScreenX(), GrScreenY(), NULL, NULL);
    if (grc != NULL) {
        GrBitBlt(grc, 0, 0, GrScreenContext(), 0, 0,
                 GrScreenX()-1, GrScreenY()-1, GrWRITE);
    }

    ioctl(ttyfd, KDSETMODE, KD_TEXT);

    ioctl(ttyfd, VT_RELDISP, 1);
    ioctl(ttyfd, VT_WAITACTIVE, myvt);

    ioctl(ttyfd, KDSETMODE, KD_GRAPHICS);

    if (grc != NULL) {
        GrBitBlt(GrScreenContext(), 0, 0, grc, 0, 0,
                 GrScreenX()-1, GrScreenY()-1, GrWRITE);
        GrDestroyContext(grc);
    }
}

void _LnxfbAcqsigHandle(int sig);
void _LnxfbRelsigHandle(int sig)
{
    if (!ingraphicsmode) return;
    if (ttyfd < 0) return;

    /* create a new context from the screen */
    grc = GrCreateContext(GrScreenX(), GrScreenY(), NULL, NULL);
    if (grc == NULL)
        return;
    /* copy framebuffer to new context */
    if (GrScreenFrameMode() == GRX_FRAME_MODE_LFB_MONO01) {
        /* Need to invert the colors on this one. */
        GrClearContextC(grc, 1);
        GrBitBlt(grc, 0, 0, GrScreenContext(), 0, 0,
                 GrScreenX()-1, GrScreenY()-1, GrXOR);
    } else {
        GrBitBlt(grc, 0, 0, GrScreenContext(), 0, 0,
                 GrScreenX()-1, GrScreenY()-1, GrWRITE);
    }
    /*
     * swap out the framebuffer memory with the new context so that the
     * application can continue to run in the background without actually
     * writing to the framebuffer.
     */
    frame_addr[0] = GrScreenContext()->gc_baseaddr[0];
    frame_addr[1] = GrScreenContext()->gc_baseaddr[1];
    frame_addr[2] = GrScreenContext()->gc_baseaddr[2];
    frame_addr[3] = GrScreenContext()->gc_baseaddr[3];
    GrScreenContext()->gc_baseaddr[0] = grc->gc_baseaddr[0];
    GrScreenContext()->gc_baseaddr[1] = grc->gc_baseaddr[1];
    GrScreenContext()->gc_baseaddr[2] = grc->gc_baseaddr[2];
    GrScreenContext()->gc_baseaddr[3] = grc->gc_baseaddr[3];
    /* release control of the vt */
    ioctl(ttyfd, VT_RELDISP, 1);
    signal(SIGUSR1, _LnxfbAcqsigHandle);
}

void _LnxfbAcqsigHandle(int sig)
{
    if (!ingraphicsmode) return;
    if (ttyfd < 0) return;
    /* resume control of the vt */
    ioctl(ttyfd, VT_RELDISP, VT_ACKACQ);
    ioctl(ttyfd, KDSETMODE, KD_GRAPHICS);
    /* restore framebuffer address */
    GrScreenContext()->gc_baseaddr[0] = frame_addr[0];
    GrScreenContext()->gc_baseaddr[1] = frame_addr[1];
    GrScreenContext()->gc_baseaddr[2] = frame_addr[2];
    GrScreenContext()->gc_baseaddr[3] = frame_addr[3];
    /* copy the temporary context back to the framebuffer */
    if (GrScreenFrameMode() == GRX_FRAME_MODE_LFB_MONO01) {
        /* need to invert the colors on this one */
        GrClearScreen(1);
        GrBitBlt(GrScreenContext(), 0, 0, grc, 0, 0,
                 GrScreenX()-1, GrScreenY()-1, GrXOR);
    } else {
        GrBitBlt(GrScreenContext(), 0, 0, grc, 0, 0,
                 GrScreenX()-1, GrScreenY()-1, GrWRITE);
    }
    GrDestroyContext(grc);
    signal(SIGUSR1, _LnxfbRelsigHandle);
}

static void loadcolor(int c, int r, int g, int b)
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
        ioctl(ttyfd, KDSETMODE, KD_GRAPHICS);
        vtm.mode = VT_PROCESS;
        vtm.relsig = SIGUSR1;
        vtm.acqsig = SIGUSR1;
        ioctl(ttyfd, VT_SETMODE, &vtm);
        signal(SIGUSR1, _LnxfbRelsigHandle);
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
        signal(SIGUSR1, SIG_IGN);
        ingraphicsmode = 0;
    }
    return TRUE;
}

GrVideoModeExt grtextextfb = {
    GRX_FRAME_MODE_TEXT,         /* frame driver */
    NULL,                        /* frame driver override */
    NULL,                        /* frame buffer address */
    {6, 6, 6},                  /* color precisions */
    {0, 0, 0},                  /* color component bit positions */
    0,                                /* mode flag bits */
    settext,                        /* mode set */
    NULL,                        /* virtual size set */
    NULL,                        /* virtual scroll */
    NULL,                        /* bank set function */
    NULL,                        /* double bank set function */
    NULL,                        /* color loader */
};

static GrVideoModeExt exts[NUM_EXTS];
static GrxVideoMode modes[NUM_MODES] = {
    /* pres.  bpp wdt   hgt   mode   scan  priv. &ext                             */
    {TRUE, 4, 80, 25, 0, 160, 0, &grtextextfb},
    {0}
};

static int build_video_mode(GrxVideoMode * mp, GrVideoModeExt * ep)
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
    ep->setvsize = NULL;        /* tbd */
    ep->scroll = NULL;                /* tbd */
    ep->setbank = NULL;
    ep->setrwbanks = NULL;
    ep->loadcolor = NULL;
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
        ep->loadcolor = loadcolor;
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
    ep->flags |= GR_VMODEF_LINEAR;
    ep->cprec[0] = fbvar.red.length;
    ep->cprec[1] = fbvar.green.length;
    ep->cprec[2] = fbvar.blue.length;
    ep->cpos[0] = fbvar.red.offset;
    ep->cpos[1] = fbvar.green.offset;
    ep->cpos[2] = fbvar.blue.offset;
    return (TRUE);
}

static void add_video_mode(GrxVideoMode * mp, GrVideoModeExt * ep,
                           GrxVideoMode ** mpp, GrVideoModeExt ** epp)
{
    if (*mpp < &modes[NUM_MODES]) {
        if (!mp->extended_info) {
            GrVideoModeExt *etp = &exts[0];
            while (etp < *epp) {
                if (memcmp(etp, ep, sizeof(GrVideoModeExt)) == 0) {
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
        GrVideoModeExt ext, *extp = &exts[0];
        memzero(modep, (sizeof(modes) - sizeof(modes[0])));
        if ((build_video_mode(&mode, &ext))) {
            add_video_mode(&mode, &ext, &modep, &extp);
        }
        return (TRUE);
    }
    return (FALSE);
}

GrxVideoDriver _GrVideoDriverLINUXFB = {
    "linuxfb",                        /* name */
    GRX_VIDEO_ADAPTER_LINUX_FB,       /* adapter type */
    NULL,                        /* inherit modes from this driver */
    modes,                        /* mode table */
    itemsof(modes),                /* # of modes */
    detect,                        /* detection routine */
    init,                        /* initialization routine */
    reset,                        /* reset routine */
    _gr_select_mode,                 /* standard mode select routine */
    0                                /* no additional capabilities */
};
