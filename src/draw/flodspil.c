/*
 * flodspil.c ---- Floodspill is a color replacer
 *
 * Copyright (c) 2007 Richard
 * [e-mail: richard at dogcreek.ca].
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
 */

#include <grx/context.h>
#include <grx/draw.h>

#include <stdlib.h>
#include "libgrx.h"

/**
 * grx_flood_spill:
 * @x1: the left edge of the bounding rectangle
 * @y1: the top edge of the bounding rectangle
 * @x2: the right edge of the bounding rectangle
 * @y2: the bottom edge of the bounding rectangle
 * @old_c: the color to be replaced
 * @new_c: the new color
 *
 * Replaces old color with new color in the current context in the area bounded
 * by the rectangle x1, y1, x2, y2.
 *
 * This is quite useful for highlighting a selected item in a list, or changing
 * a selected color(s) in a multi colored area.
 */
void grx_flood_spill(int x1, int y1, int x2, int y2,
                  GrxColor old_c, GrxColor new_c)
{
  int y;
  int x;

  GrxColor *scanline;

  for(y = y1; y <= y2; ++y)
  {
    if ((scanline = (GrxColor *)grx_get_scanline(x1, x2, y, NULL)) != NULL)
    {
      for(x = x1; x <= x2; ++x)
      {
        if (scanline[x] == old_c)
          scanline[x] = new_c;
      }
      grx_put_scanline(x1, x2, y, scanline, GRX_COLOR_MODE_WRITE);
    }
  }
}

/**
 * grx_flood_spill2:
 * @x1: the left edge of the bounding rectangle
 * @y1: the top edge of the bounding rectangle
 * @x2: the right edge of the bounding rectangle
 * @y2: the bottom edge of the bounding rectangle
 * @old_c1: the first color to be replaced
 * @new_c1: the first new color
 * @old_c2: the second color to be replaced
 * @new_c2: the second new color
 *
 * Replaces two old colors with two new colors in the current context in the
 * area bounded by the rectangle x1, y1, x2, y2. @old_c1 is replaced with
 * @new_c1 and @old_c2 is replaced with @new_c2.
 *
 * This is quite useful for highlighting a selected item in a list, or changing
 * a selected color(s) in a multi colored area.
 */
void grx_flood_spill2(int x1, int y1, int x2, int y2,
                  GrxColor old_c1, GrxColor new_c1,
                  GrxColor old_c2, GrxColor new_c2)
{
  int y;
  int x;

  GrxColor *scanline;

  for(y = y1; y <= y2; ++y)
  {
    if ((scanline = (GrxColor *)grx_get_scanline(x1, x2, y, NULL)) != NULL)
    {
      for(x = x1; x <= x2; ++x)
      {
        if (scanline[x] == old_c1)
          scanline[x] = new_c1;
        else if(scanline[x] == old_c2)
          scanline[x] = new_c2;
      }
      grx_put_scanline(x1, x2, y, scanline, GRX_COLOR_MODE_WRITE);
    }
  }
}

/**
 * grx_context_flood_spill:
 * @context: the context
 * @x1: the left edge of the bounding rectangle
 * @y1: the top edge of the bounding rectangle
 * @x2: the right edge of the bounding rectangle
 * @y2: the bottom edge of the bounding rectangle
 * @old_c: the color to be replaced
 * @new_c: the new color
 *
 * Replaces old color with new color in the context in the area bounded
 * by the rectangle x1, y1, x2, y2.
 *
 * This is quite useful for highlighting a selected item in a list, or changing
 * a selected color(s) in a multi colored area.
 *
 * Also see grx_flood_spill() for operating on the current context.
 */
void grx_context_flood_spill(GrxContext *ctx, int x1, int y1, int x2, int y2,
                   GrxColor old_c, GrxColor new_c)
{
  int y;
  int x;
  GrxContext ctx_save;
  GrxColor *scanline;

  grx_context_save(&ctx_save);
  grx_context_set_current(ctx);

  for(y = y1; y <= y2; ++y)
  {
    if ((scanline = (GrxColor *)grx_get_scanline(x1, x2, y, NULL)) != NULL)
    {
      for(x = x1; x <= x2; ++x)
      {
        if (scanline[x] == old_c)
          scanline[x] = new_c;
      }
      grx_put_scanline(x1, x2, y, scanline, GRX_COLOR_MODE_WRITE);
    }
  }
  grx_context_set_current(&ctx_save);
}

/**
 * grx_context_flood_spill2:
 * @context: the context
 * @x1: the left edge of the bounding rectangle
 * @y1: the top edge of the bounding rectangle
 * @x2: the right edge of the bounding rectangle
 * @y2: the bottom edge of the bounding rectangle
 * @old_c1: the first color to be replaced
 * @new_c1: the first new color
 * @old_c2: the second color to be replaced
 * @new_c2: the second new color
 *
 * Replaces two old colors with two new colors in the context in the
 * area bounded by the rectangle x1, y1, x2, y2. @old_c1 is replaced with
 * @new_c1 and @old_c2 is replaced with @new_c2.
 *
 * This is quite useful for highlighting a selected item in a list, or changing
 * a selected color(s) in a multi colored area.
 *
 * Also see grx_flood_spill2() for operating on the current context.
 */
void grx_context_flood_spill2(GrxContext *ctx, int x1, int y1, int x2, int y2,
                  GrxColor old_c1, GrxColor new_c1,
                  GrxColor old_c2, GrxColor new_c2)
{
  int y;
  int x;
  GrxContext ctx_save;
  GrxColor *scanline;

  grx_context_save(&ctx_save);
  grx_context_set_current(ctx);

  for(y = y1; y <= y2; ++y)
  {
    if ((scanline = (GrxColor *)grx_get_scanline(x1, x2, y, NULL)) != NULL)
    {
      for(x = x1; x <= x2; ++x)
      {
        if (scanline[x] == old_c1)
          scanline[x] = new_c1;
        else if(scanline[x] == old_c2)
          scanline[x] = new_c2;
      }
      grx_put_scanline(x1, x2, y, scanline, GRX_COLOR_MODE_WRITE);
    }
  }
  grx_context_set_current(&ctx_save);
}
