/**
 ** vd_lnxfb.c ---- Linux framebuffer driver
 **
 ** Copyright (c) 2001 Mariano Alvarez Fernandez
 ** [e-mail: malfer@teleline.es]
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

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/kd.h>

#include "libgrx.h"
#include "grdriver.h"
#include "arith.h"
#include "memcopy.h"
#include "memfill.h"

#define  NUM_MODES    80                /* max # of supported modes */
#define  NUM_EXTS     15                /* max # of mode extensions */

static int initted  = (-1);
static int fbfd = -1;
static int ttyfd = -1;
static struct fb_fix_screeninfo fbfix;
static struct fb_var_screeninfo fbvar;
char *fbuffer = NULL;

static int detect( void )
{
        char *fbname;
        char *default_fbname = "/dev/fb0";

        if( initted < 0 ){
                initted = 0;
                fbname = getenv( "FRAMEBUFFER" );
                if( fbname == NULL ) fbname = default_fbname;
                fbfd = open( fbname,O_RDWR );
                if( fbfd == -1 ) return FALSE;
                ioctl( fbfd,FBIOGET_FSCREENINFO,&fbfix );
                ioctl( fbfd,FBIOGET_VSCREENINFO,&fbvar );
                if( fbfix.type != FB_TYPE_PACKED_PIXELS ) return FALSE;
                if( fbfix.visual != FB_VISUAL_TRUECOLOR ) return FALSE;
                ttyfd = open( "/dev/tty",O_RDONLY );
                initted = 1;
        }
        return( (initted > 0) ? TRUE : FALSE );
}

static void reset(void)
{
        if( fbuffer ){
            memzero( fbuffer,fbvar.yres*fbfix.line_length );
            munmap( fbuffer,fbfix.smem_len );
        }
        if( fbfd != -1 )
            close( fbfd );
        if( ttyfd > -1 ){
            ioctl( ttyfd,KDSETMODE,KD_TEXT );
            close( ttyfd );
        }
}

//static void setrwbanks( int rb, int wb )
//{
//}

//static void loadcolor( int c, int r, int g, int b )
//{
//}

static int setmode( GrVideoMode *mp, int noclear )
{
        fbuffer = mp->extinfo->frame = mmap( 0,
//                                   fbvar.yres*fbfix.line_length,
                                   fbfix.smem_len,
                                   PROT_READ|PROT_WRITE,
                                   MAP_SHARED,
                                   fbfd,
                                   0 );
        if( mp->extinfo->frame && ttyfd > -1 ){
            ioctl( ttyfd,KDSETMODE,KD_GRAPHICS );
        }
        if( mp->extinfo->frame && !noclear )
            memzero( mp->extinfo->frame,fbvar.yres*fbfix.line_length );
        return( (mp->extinfo->frame) ? TRUE : FALSE );
}

static int settext( GrVideoMode *mp, int noclear )
{
        if( fbuffer )
            memzero( fbuffer,fbvar.yres*fbfix.line_length );
        ioctl( ttyfd,KDSETMODE,KD_TEXT );
        return TRUE;
}

GrVideoModeExt grtextextfb = {
    GR_frameText,                       /* frame driver */
    NULL,                               /* frame driver override */
    NULL,                               /* frame buffer address */
    { 6, 6, 6 },                        /* color precisions */
    { 0, 0, 0 },                        /* color component bit positions */
    0,                                  /* mode flag bits */
    settext,                            /* mode set */
    NULL,                               /* virtual size set */
    NULL,                               /* virtual scroll */
    NULL,                               /* bank set function */
    NULL,                               /* double bank set function */
    NULL,                               /* color loader */
};

static GrVideoModeExt exts[NUM_EXTS];
static GrVideoMode   modes[NUM_MODES] = {
    /* pres.  bpp wdt   hgt   mode   scan  priv. &ext                             */
    {  TRUE,  4,  80,   25,      0,  160,  0,    &grtextextfb },
    {  0  }
};

static int build_video_mode( GrVideoMode *mp, GrVideoModeExt *ep )
{
        mp->present    = TRUE;
        mp->width      = fbvar.xres;
        mp->height     = fbvar.yres;
        mp->lineoffset = fbfix.line_length;
        mp->extinfo    = NULL;
        mp->privdata   = 0;
        ep->drv        = NULL;
        ep->frame      = NULL;          /* filled in after mode set */
        ep->flags      = 0;
        ep->setup      = setmode;
        ep->setvsize   = NULL;          /* tbd */
        ep->scroll     = NULL;          /* tbd */
        ep->setbank    = NULL;
        ep->setrwbanks = NULL;
        ep->loadcolor  = NULL;
        switch( fbvar.bits_per_pixel ) {
          case 15:
            mp->bpp       = 15;
            ep->mode      = GR_frameSVGA16_LFB;
            ep->flags    |= GR_VMODEF_LINEAR;
            ep->cprec[0]  = fbvar.red.length;
            ep->cprec[1]  = fbvar.green.length;
            ep->cprec[2]  = fbvar.blue.length;
            ep->cpos[0]   = fbvar.red.offset;
            ep->cpos[1]   = fbvar.green.offset;
            ep->cpos[2]   = fbvar.blue.offset;
            break;
          case 16:
            mp->bpp       = 16;
            ep->mode      = GR_frameSVGA16_LFB;
            ep->flags    |= GR_VMODEF_LINEAR;
            ep->cprec[0]  = fbvar.red.length;
            ep->cprec[1]  = fbvar.green.length;
            ep->cprec[2]  = fbvar.blue.length;
            ep->cpos[0]   = fbvar.red.offset;
            ep->cpos[1]   = fbvar.green.offset;
            ep->cpos[2]   = fbvar.blue.offset;
            break;
          case 24:
            mp->bpp       = 24;
            ep->mode      = GR_frameSVGA24_LFB;
            ep->flags    |= GR_VMODEF_LINEAR;
            ep->cprec[0]  = fbvar.red.length;
            ep->cprec[1]  = fbvar.green.length;
            ep->cprec[2]  = fbvar.blue.length;
            ep->cpos[0]   = fbvar.red.offset;
            ep->cpos[1]   = fbvar.green.offset;
            ep->cpos[2]   = fbvar.blue.offset;
            break;
          default:
            return(FALSE);
        }
        return(TRUE);
}

static void add_video_mode(
    GrVideoMode *mp,  GrVideoModeExt *ep,
    GrVideoMode **mpp,GrVideoModeExt **epp
){
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

static int init( char *options )
{
        if( detect() ){
            GrVideoMode    mode,*modep = &modes[1];
            GrVideoModeExt ext, *extp  = &exts[0];

            memzero( modep,(sizeof(modes) - sizeof(modes[0])) );
            if( (build_video_mode( &mode,&ext )) ){
                add_video_mode( &mode,&ext,&modep,&extp );
            }
            return( TRUE );
        }
        return( FALSE );
}

GrVideoDriver _GrVideoDriverLINUXFB = {
    "linuxfb",                          /* name */
    GR_LNXFB,                           /* adapter type */
    NULL,                               /* inherit modes from this driver */
    modes,                              /* mode table */
    itemsof(modes),                     /* # of modes */
    detect,                             /* detection routine */
    init,                               /* initialization routine */
    reset,                              /* reset routine */
    _gr_selectmode,                     /* standard mode select routine */
    0                                   /* no additional capabilities */
};

