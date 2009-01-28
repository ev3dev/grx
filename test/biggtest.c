/**
 ** biggtest.c ---- test GR_biggest_graphics
 **
 ** Copyright (C) 2009 Mariano Alvarez Fernandez
 ** [e-mail: malfer@telefonica.net]
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
#include <stdlib.h>
#include <string.h>

#include "mgrx.h"
#include "mgrxkeys.h"
#include "drawing.h"

static void TestFunc(void)
{
  int  x = GrSizeX();
  int  y = GrSizeY();
  int  ww = (x / 2) - 10;
  int  wh = (y / 2) - 10;
  GrColor c;

  GrContext *w1 = GrCreateSubContext(5, 5, ww+4, wh+4, NULL, NULL);
  GrContext *w2 = GrCreateSubContext(15+ww, 5, ww+ww+14, wh+4, NULL, NULL);
  GrContext *w3 = GrCreateSubContext(5, 15+wh, ww+4, wh+wh+14, NULL, NULL);
  GrContext *w4 = GrCreateSubContext(15+ww, 15+wh, ww+ww+14, wh+wh+14, NULL, NULL);

  GrSetContext(w1);
  c = GrAllocColor(200, 100, 100);
  drawing(0, 0, ww, wh, c, GrBlack());
  GrBox(0, 0, ww-1, wh-1, c);

  GrSetContext(w2);
  c = GrAllocColor(100, 200, 200);
  drawing(0, 0, ww, wh, c, GrBlack());
  GrBox(0, 0, ww-1, wh-1, c);

  GrSetContext(w3);
  c = GrAllocColor(200, 200, 0);
  drawing(0, 0, ww, wh, c, GrBlack());
  GrBox(0, 0,ww-1, wh-1, c);

  GrSetContext(w4);
  c = GrAllocColor(0, 100, 200);
  drawing(0, 0, ww, wh, c, GrBlack());
  GrBox(0, 0, ww-1, wh-1, c);

  GrSetContext(NULL);
}

static void PrintInfo(void)
{
    char aux[81];
    int x, y;

    sprintf(aux, " Mode: %dx%d %d bpp ", GrCurrentVideoMode()->width,
    GrCurrentVideoMode()->height, GrCurrentVideoMode()->bpp);
    x = (GrMaxX() -
        GrFontStringWidth(&GrDefaultFont, aux, strlen(aux), GR_BYTE_TEXT)) / 2;
    y = (GrMaxY() -
        GrFontStringHeight(&GrDefaultFont, aux, strlen(aux), GR_BYTE_TEXT)) / 2;
    GrTextXY(x, y, aux, GrWhite(), GrBlack());
}

int main(int argc,char **argv)
{
  GrEvent ev;

  GrSetMode(GR_biggest_graphics);
  GrEventInit();
  GrMouseDisplayCursor();
  TestFunc();
  PrintInfo();
  GrEventWait(&ev);
  GrEventUnInit();
  GrSetMode(GR_default_text);
  return(0);
}
