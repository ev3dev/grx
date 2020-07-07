/**
 ** textpatt.c
 **
 ** This is a test/demo file of the GRX graphics library.
 ** You can use GRX test/demo files as you want.
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

#include <stdio.h>
#include <string.h>
#include "mgrx.h"
#include "mgrxkeys.h"

#define FONT "../fonts/tms38b.fnt"

static void print_hellos(int x, int y, GrTextOption *opt,
                         GrPattern *p1, GrPattern *p2, int hv)
{
    if (hv) {
        GrPatternDrawString(" Hello world ! ", 15, x, y, opt, p1);
        GrPatternDrawString(" Hello world ! ", 15, x+40, y+4, opt, p2);
        GrPatternDrawStringExt(" Hello world !! ", 16, x+90, y, opt, p1);
        GrPatternDrawStringExt(" Hello world !! ", 16, x+130, y+4, opt, p2);
    } else {
        GrPatternDrawString(" Hello world ! ", 15, x, y, opt, p1);
        GrPatternDrawString(" Hello world ! ", 15, x+4, y+40, opt, p2);
        GrPatternDrawStringExt(" Hello world !! ", 16, x, y+90, opt, p1);
        GrPatternDrawStringExt(" Hello world !! ", 16, x+4, y+130, opt, p2);
    }
}

int main(void)
{
    char bits[] = {0, 76, 50, 0, 0, 76, 60, 0};
    GrPattern *p1, *p2;
    GrFont *font;
    GrTextOption opt;
    GrEvent ev;
    GrColor *egacolors;
    
    GrSetMode(GR_default_graphics);
    GrEventInit();
    GrMouseDisplayCursor();
    egacolors = GrAllocEgaColors();
    p1 = GrBuildPixmapFromBits(bits, 8, 8, egacolors[11], egacolors[3]);
    p2 = GrBuildPixmapFromBits(bits, 8, 8, egacolors[3], egacolors[11]);
    font = GrLoadFont(FONT);
    if (font && p1 && p2) {
        memset(&opt, 0, sizeof(GrTextOption));
        opt.txo_font   = font;
        opt.txo_xalign = 0;
        opt.txo_yalign = 0;
        opt.txo_direct = GR_TEXT_RIGHT;
        opt.txo_fgcolor = GrNOCOLOR;
        opt.txo_bgcolor = GrNOCOLOR;
        GrPatternFilledBox(0, 0, GrMaxX(), GrMaxY(), p1);
        GrEventWaitKeyOrClick(&ev);
        opt.txo_direct = GR_TEXT_RIGHT;
        print_hellos(40, 10, &opt, p1, p2, 0);
        opt.txo_direct = GR_TEXT_LEFT;
        print_hellos(280, 10, &opt, p1, p2, 0);
        opt.txo_direct = GR_TEXT_DOWN;
        print_hellos(40, 220, &opt, p1, p2, 1);
        opt.txo_direct = GR_TEXT_UP;
        print_hellos(280, 220, &opt, p1, p2, 1);
        GrEventWaitKeyOrClick(&ev);
        opt.txo_bgcolor = GrBlack();
        opt.txo_direct = GR_TEXT_RIGHT;
        print_hellos(40, 10, &opt, p1, p2, 0);
        opt.txo_direct = GR_TEXT_LEFT;
        print_hellos(280, 10, &opt, p1, p2, 0);
        opt.txo_direct = GR_TEXT_DOWN;
        print_hellos(40, 220, &opt, p1, p2, 1);
        opt.txo_direct = GR_TEXT_UP;
        print_hellos(280, 220, &opt, p1, p2, 1);
        GrEventWaitKeyOrClick(&ev);
    }
    if (p1)   GrDestroyPattern(p1);
    if (p2)   GrDestroyPattern(p2);
    if (font) GrUnloadFont(font);
    GrEventUnInit();
    GrSetMode(GR_default_text);
    if (!p1) fprintf(stderr, "Couldn't create first pattern\n");
    if (!p2) fprintf(stderr, "Couldn't create second pattern\n");
    if (!font) fprintf(stderr, "Couldn't load font %s\n", FONT);
    
    return 0;
}

