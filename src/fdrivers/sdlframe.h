/*
 * sdlframe.h ---- the SDL wrapper for linear framebuffer drivers
 *
 * Copyright (C) 2004 Dimitar Zhekov
 * [e-mail: jimmy@is-vn.bg]
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
 *
 * Using SDL_FillRect() does not provide any speedup above the margin
 * of error. Our main problem is sdl_drawpixel() and sdl_drawline()
 * being too slow in windowed modes.
 */

#include "libsdl.h"
#include "memcopy.h"

static INLINE void sdl_drawpixel(int x, int y, GrxColor color)
{
    drawpixel(x, y, color);
    SDL_UpdateRect(_SGrScreen, x, y, 1, 1);
}

static void sdl_drawline(int x, int y, int dx, int dy, GrxColor color)
{
    drawline(x, y, dx, dy, color);

    if(dx < 0) {
        x += dx;
        dx = -dx;
    }
    if(dy < 0) {
        y += dy;
        dy = -dy;
    }

    SDL_UpdateRect(_SGrScreen, x, y, dx + 1, dy + 1);
}

static INLINE void sdl_drawhline(int x, int y, int w, GrxColor color)
{
    drawhline(x, y, w, color);
    SDL_UpdateRect(_SGrScreen, x, y, w, 1);
}

static INLINE void sdl_drawvline(int x, int y, int h, GrxColor color)
{
    drawvline(x, y, h, color);
    SDL_UpdateRect(_SGrScreen, x, y, 1, h);
}

static INLINE void sdl_drawblock(int x, int y, int w, int h, GrxColor color)
{
    drawblock(x, y, w, h, color);
    SDL_UpdateRect(_SGrScreen, x, y, w, h);
}

static INLINE void sdl_drawbitmap(int x, int y, int w, int h, unsigned char *bmp,
                                  int pitch, int start, GrxColor fg, GrxColor bg)
{
    drawbitmap(x, y, w, h, bmp, pitch, start, fg, bg);
    SDL_UpdateRect(_SGrScreen, x, y, w, h);
}

static INLINE void sdl_drawpattern(int x, int y, int w, unsigned char patt,
                                   GrxColor fg, GrxColor bg)
{
    drawpattern(x, y, w, patt, fg, bg);
    SDL_UpdateRect(_SGrScreen, x, y, w, 1);
}

static INLINE void sdl_bitblt(GrxFrame *dst, int dx, int dy, GrxFrame *src,
                              int sx, int sy,int w,int h, GrxColor op)
{
    bitblt(dst, dx, dy, src, sx, sy, w, h, op);
    SDL_UpdateRect(_SGrScreen, dx, dy, w, h);
}

static INLINE void sdl_bltr2v(GrxFrame *dst, int dx, int dy, GrxFrame *src,
                              int sx, int sy, int w, int h, GrxColor op)
{
    bltr2v(dst, dx, dy, src, sx, sy, w, h, op);
    SDL_UpdateRect(_SGrScreen, dx, dy, w, h);
}

static INLINE void sdl_putscanline(int x, int y, int w, const GrxColor *scl,
                                   GrxColor op)
{
    putscanline(x, y, w, scl, op);
    SDL_UpdateRect(_SGrScreen, x, y, w, 1);
}
