/*
 * test.h ---- common declarations for test programs
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
 *
 * This is a test/demo file of the GRX graphics library.
 * You can use GRX test/demo files as you want.
 *
 * The GRX graphics library is free software; you can redistribute it
 * and/or modify it under some conditions; see the "copying.grx" file
 * for details.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef __TEST_H_INCLUDED__
#define __TEST_H_INCLUDED__

#include <grx-3.0.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grxkeys.h"
#include "drawing.h"

extern void (*testfunc)(void);
char   exit_message[2000] = { "" };
int    Argc;
char **Argv;
GrxTextOptions *black_text;
GrxTextOptions *white_text;
GrxTextOptions *white_text_black_bg;

#define TESTFUNC(name)      \
void name(void);        \
void (*testfunc)(void) = name;  \
void name(void)

int main(int argc,char **argv)
{
        GrxFont *font;
        int  x = 0;
        int  y = 0;
        long c = 0;
        int xv = 0;
        int yv = 0;
        int ret;
        GError *error = NULL;

        font = grx_font_load(NULL, -1, &error);
        if (!font) {
            g_error("%s", error->message);
        }

        black_text = grx_text_options_new(font, GRX_COLOR_BLACK);
        white_text = grx_text_options_new(font, GRX_COLOR_WHITE);
        white_text_black_bg = grx_text_options_new_full(font, GRX_COLOR_WHITE,
            GRX_COLOR_BLACK, GRX_TEXT_HALIGN_LEFT, GRX_TEXT_VALIGN_TOP);
        grx_font_unref(font);

        Argc = argc - 1;
        Argv = argv + 1;
        if((Argc >= 2) &&
           (sscanf(Argv[0],"%d",&x) == 1) && (x >= 320) &&
           (sscanf(Argv[1],"%d",&y) == 1) && (y >= 200)) {
                Argc -= 2;
                Argv += 2;
                if (Argc > 0) {
                   char *endp;
                   c = strtol(Argv[0], &endp, 0);
                   switch (*endp) {
                     case 'k':
                     case 'K': c <<= 10; break;
                     case 'm':
                     case 'M': c <<= 20; break;
                   }
                   Argc--;
                   Argv++;
                }
        }
        if((Argc >= 2) &&
           (sscanf(Argv[0],"%d",&xv) == 1) && (xv >= x) &&
           (sscanf(Argv[1],"%d",&yv) == 1) && (yv >= y)) {
                Argc -= 2;
                Argv += 2;
        }
        if((xv >= x) && (yv >= y) && (c >= 2))
            ret = grx_set_mode(GRX_GRAPHICS_MODE_GRAPHICS_CUSTOM,&error,x,y,c,xv,yv);
        else if(c >= 2)
            ret = grx_set_mode(GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_COLOR,&error,x,y,c);
        else if((x >= 320) && (y >= 200))
            ret = grx_set_mode(GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT,&error,x,y);
        else
            ret = grx_set_mode(GRX_GRAPHICS_MODE_GRAPHICS_DEFAULT, &error);
        if (!ret) {
            g_error("%s", error->message);
        }
        (*testfunc)();
        grx_set_mode(GRX_GRAPHICS_MODE_TEXT_DEFAULT, NULL);
        if(strlen(exit_message) > 0) {
                puts(exit_message);
        }
        grx_text_options_unref(white_text_black_bg);
        grx_text_options_unref(white_text);

        return 0;
}

#endif /* _TEST_H_ */
