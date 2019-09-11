/**
 ** lfbinfo.c ---- print linux framebuffer information
 **
 ** Copyright (c) 2001, 2019 Mariano Alvarez Fernandez
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
 ** 190819 added printfs for all variable fields
 **/

#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int main()
{
    struct fb_fix_screeninfo refix;
    struct fb_var_screeninfo resul;
    int fd;
    char *fbname;
    char *default_fbname = "/dev/fb0";
    
    fbname = getenv( "FRAMEBUFFER" );
    if( fbname == NULL ) fbname = default_fbname;
    fd = open( fbname,O_RDWR );
    if( fd == -1 ){
        printf( "can't open %s\n",fbname );
        return 1;
    }

    printf( "Device file: %s\n",fbname );

    printf( "\nFixed data\n" );
    printf( "----------\n" );
    ioctl( fd,FBIOGET_FSCREENINFO,&refix );
    printf( "name: %16s\n",refix.id );
    printf( "smem_start: %lu\n",(unsigned long)refix.smem_start );
    printf( "smem_len: %d\n",(int)refix.smem_len );
    printf( "type: %d\n",(int)refix.type );
    printf( "type_aux: %d\n",(int)refix.type_aux );
    printf( "visual: %d\n",(int)refix.visual );
    printf( "xpanstep: %d  ypanstep: %d  ywrapstep: %d \n",
        refix.xpanstep,refix.ypanstep,refix.ywrapstep );
    printf( "line_length: %d\n",(int)refix.line_length );
    printf( "mmio_start: %lu\n",(unsigned long)refix.mmio_start );
    printf( "mmio_len: %d\n",(int)refix.mmio_len );
    printf( "accel: %d\n",(int)refix.accel );
    // not present in old linux
    //printf( "capabilities: %d\n",(int)refix.capabilities );
    ioctl( fd,FBIOGET_VSCREENINFO,&resul );
    printf( "\nVariable data\n" );
    printf( "-------------\n" );
    printf( "xres: %d\n",(int)resul.xres );
    printf( "yres: %d\n",(int)resul.yres );
    printf( "xres_virtual: %d\n",(int)resul.xres_virtual );
    printf( "yres_virtual: %d\n",(int)resul.yres_virtual );
    printf( "xoffset: %d\n",(int)resul.xoffset );
    printf( "yoffset: %d\n",(int)resul.yoffset );
    printf( "bpp: %d\n",(int)resul.bits_per_pixel );
    printf( "grayscale: %d\n",(int)resul.grayscale );
    printf( "red offset: %d  length: %d  msb_right: %d\n",
        resul.red.offset,resul.red.length,resul.red.msb_right );
    printf( "green offset: %d  length: %d  msb_right: %d\n",
        resul.green.offset,resul.green.length,resul.green.msb_right );
    printf( "blue offset: %d  length: %d  msb_right: %d\n",
        resul.blue.offset,resul.blue.length,resul.blue.msb_right );
    printf( "transp offset: %d  length: %d  msb_right: %d\n",
        resul.transp.offset,resul.transp.length,resul.transp.msb_right );
    printf( "nonstd: %d\n",(int)resul.nonstd );
    printf( "activate: %d\n",(int)resul.activate );
    printf( "height(mm): %d\n",(int)resul.height );
    printf( "width(mm): %d\n",(int)resul.width );
    printf( "accel_flags(obsolete): %d\n",(int)resul.accel_flags );
    printf( "pixclock(pico seconds): %d\n",(int)resul.pixclock );
    printf( "left_margin: %d\n",(int)resul.left_margin );
    printf( "right_margin: %d\n",(int)resul.right_margin );
    printf( "upper_margin: %d\n",(int)resul.upper_margin );
    printf( "lower_margin: %d\n",(int)resul.lower_margin );
    printf( "hsync_len: %d\n",(int)resul.hsync_len );
    printf( "vsync_len: %d\n",(int)resul.vsync_len );
    printf( "sync: %d\n",(int)resul.sync );
    printf( "vmode: %d\n",(int)resul.vmode );
    printf( "rotate: %d\n",(int)resul.rotate );
    // not present in old linux
    //printf( "colorspace: %d\n",(int)resul.colorspace );
    return 0;
}
