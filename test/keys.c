/*
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
 *
 * Changes by D.Zhekov (jimmy@is-vn.bg) 16/02/2004
 *   - converted to a standard test program [GRX I/O only].
 */

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdarg.h>

#include "test.h"

#define ISPRINT(k) (((unsigned int)(k)) <= 255 && isprint(k))

static int curx = 0, cury = 0;

static void gputc(int c)
{
        GrxFont *font;
        char c2[2];

        font = grx_text_options_get_font(white_text_black_bg);

        if(c == '\n' || curx + grx_font_get_width(font) > grx_get_width()) {
            cury += grx_font_get_height(font);
            curx = 0;
            if(cury + grx_font_get_height(font) > grx_get_height()) {
                grx_clear_screen(GRX_COLOR_BLACK);
                cury = 0;
            }
        }

        if(c != '\n') {
            c2[0] = c;
            c2[1] = 0;
            grx_draw_text(c2, curx, cury, white_text_black_bg);
            curx += grx_font_get_width(font);
        }
}

static void gprintf(const char *format, ...)
{
        va_list ap;
        char buffer[0x100], *s;

        va_start(ap, format);
        vsprintf(buffer, format, ap);
        va_end(ap);

        for(s = buffer; *s; s++) gputc(*s);
}

TESTFUNC(keys) {
  int spaces_count = 0;
  GrxKey k;
  int ok;

  gprintf("\n\n Checking GrxKey... style interface"
           "\n Type 3 spaces to quit the test\n\n");
  while (spaces_count < 3) {
    k = run_main_loop_until_key_press();
    if (k == ' ') ++spaces_count; else spaces_count = 0;

    ok = 0;
    // FIXME: update for new key events
    // for ( kp = Keys; kp < &Keys[KEYS]; ++kp ) {
    //   if (k == kp->key) {
    //     gprintf("code 0x%04x     symbol %s\n", (unsigned)k, kp->name);
    //     ok = 1;
    //     break;
    //   }
    // }
    if (!ok)
      gprintf("code 0x%04x     symbol UNKNOWN\n", (unsigned)k);
  }
}
