/*
 * colors.c ---- color management functions
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
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

#include "colors.h"
#include "globals.h"
#include "libgrx.h"
#include "memfill.h"

static void (*DACload)(GrxColor c, GrxColor r, GrxColor g, GrxColor b) = NULL;

static void load_color(GrxColor c, GrxColor r, GrxColor g, GrxColor b)
{
    CLRINFO->ctable[c].r = (r &= CLRINFO->mask[0]);
    CLRINFO->ctable[c].g = (g &= CLRINFO->mask[1]);
    CLRINFO->ctable[c].b = (b &= CLRINFO->mask[2]);
    if (DACload)
        (*DACload)(c, r, g, b);
}

static void setbits(unsigned char *prec, unsigned char *pos)
{
    int i, tmp;
    CLRINFO->norm = 0;
    for (i = 0; i < 3; i++, prec++, pos++) {
        CLRINFO->prec[i] = *prec;
        CLRINFO->pos[i] = *prec + *pos - 1;
        CLRINFO->mask[i] = (tmp = 0xff ^ (0xff >> *prec));
        CLRINFO->round[i] = (tmp >> 1) & ~tmp;
        CLRINFO->shift[i] = (tmp = CLRINFO->pos[i] - 7);
        CLRINFO->norm = imax(CLRINFO->norm, (-tmp));
    }
    CLRINFO->shift[0] += CLRINFO->norm;
    CLRINFO->shift[1] += CLRINFO->norm;
    CLRINFO->shift[2] += CLRINFO->norm;
}

/**
 * grx_color_info_refresh_colors:
 *
 * Reloads the currently allocated color values into the video hardware.
 *
 * This function is not needed in typical applications, unless the display
 * adapter is programmed directly by the application.
 */
void grx_color_info_refresh_colors(void)
{
    int i;
    for (i = 0; i < (int)CLRINFO->ncolors; i++) {
        if (CLRINFO->ctable[i].defined)
            load_color((int)(i), CLRINFO->ctable[i].r, CLRINFO->ctable[i].g,
                CLRINFO->ctable[i].b);
    }
}

int _GrResetColors(void)
{
#define NSAVED 16
    static char *infosave = NULL;
    static int firsttime = TRUE;
    int i;
    if (firsttime) {
        infosave = malloc(offsetof(struct _GR_colorInfo, ctable[NSAVED]));
        if (infosave) {
            memcpy(infosave, CLRINFO, offsetof(struct _GR_colorInfo, ctable[NSAVED]));
        }
        firsttime = FALSE;
    }
    sttzero(CLRINFO);
    DACload = DRVINFO->actmode.extended_info->load_color;
    CLRINFO->black = GRX_COLOR_NONE;
    CLRINFO->white = GRX_COLOR_NONE;
    CLRINFO->ncolors = DRVINFO->actmode.bpp >= 32 ? 0 : (1L << DRVINFO->actmode.bpp);
    if (((CLRINFO->ncolors - 1) & GRX_COLOR_VALUE_MASK) != (CLRINFO->ncolors - 1)) {
        /* can happen on 32bpp systems. */
        int cbpp = 0;
        for (i = 0; i < 3; ++i)
            cbpp += DRVINFO->actmode.extended_info->cprec[i];
        CLRINFO->ncolors = 1L << cbpp;
    }
    setbits(
        DRVINFO->actmode.extended_info->cprec, DRVINFO->actmode.extended_info->cpos);
    switch (DRVINFO->actmode.bpp) {
    case 1:
    case 2:
        CLRINFO->palette_type = GRX_COLOR_PALETTE_TYPE_GRAYSCALE;
        break;
    case 4:
    case 8:
        CLRINFO->palette_type = GRX_COLOR_PALETTE_TYPE_COLOR_TABLE;

        if (infosave) {
            for (i = 0; i < NSAVED; i++) {
                load_color(i, ((struct _GR_colorInfo *)(infosave))->ctable[i].r,
                    ((struct _GR_colorInfo *)(infosave))->ctable[i].g,
                    ((struct _GR_colorInfo *)(infosave))->ctable[i].b);
                CLRINFO->ctable[i].defined = TRUE;
            }
        }
        break;
    default:
        CLRINFO->palette_type = GRX_COLOR_PALETTE_TYPE_RGB;
        break;
    }
    return ((CLRINFO->ncolors - 1) & GRX_COLOR_VALUE_MASK) == CLRINFO->ncolors - 1;
}

/**
 * grx_color_info_reset_colors:
 *
 * Frees all colors (except for black and white).
 *
 * If grx_color_info_set_palette_type_rgb() was called, this will also change
 * the color system back to using a color table.
 */
void grx_color_info_reset_colors(void)
{
    _GrResetColors();
}

/**
 * grx_color_info_set_palette_type_rgb:
 *
 * Changes the graphics mode to RGB color values instead of using a color table.
 *
 * Initially, graphics modes with 256 colors or less use a color table.
 * Currently, this function only works for modes with 16 or 256 colors.
 *
 * This has no effect if the color mode is already RGB or if it is grayscale.
 */
void grx_color_info_set_palette_type_rgb(void)
{
    if (CLRINFO->palette_type == GRX_COLOR_PALETTE_TYPE_COLOR_TABLE) {
        GrxColor c;
        switch (CLRINFO->ncolors) {
        case 16L:
            setbits((unsigned char *)"\1\2\1", (unsigned char *)"\3\1\0");
            break;
        case 256L:
            setbits((unsigned char *)"\3\3\2", (unsigned char *)"\5\2\0");
            break;
        default:
            return;
        }
        CLRINFO->palette_type = GRX_COLOR_PALETTE_TYPE_RGB;
        CLRINFO->nfree = 0L;
        CLRINFO->black = 0L;
        CLRINFO->white = CLRINFO->ncolors - 1L;
        for (c = 0; c < CLRINFO->ncolors; c++)
            load_color((int)(c), (int)grx_color_get_red(c), (int)grx_color_get_green(c),
                (int)grx_color_get_blue(c));
    }
}

#define ROUNDCOLORCOMP(x, n)                 \
    (((unsigned int)(x) >= CLRINFO->mask[n]) \
            ? CLRINFO->mask[n]               \
            : (((x) + CLRINFO->round[n]) & CLRINFO->mask[n]))

/**
 * grx_color_get:
 * @r: the red component value
 * @g: the green component value
 * @b: the blue component value
 *
 * Gets a color based on the RGB value.
 *
 * A new color is allocated in the color table if needed.
 *
 * The color should be released with grx_color_put() when it is no longer used.
 *
 * Returns: the composite color value or #GRX_COLOR_NONE if there are no more
 *          free colors.
 */
GrxColor grx_color_get(unsigned char r, unsigned char g, unsigned char b)
{
    GrxColor res;

    GRX_ENTER();
    res = GRX_COLOR_NONE;
    r = ROUNDCOLORCOMP(r, 0);
    g = ROUNDCOLORCOMP(g, 1);
    b = ROUNDCOLORCOMP(b, 2);
    if (CLRINFO->palette_type == GRX_COLOR_PALETTE_TYPE_RGB) {
        res = grx_color_build_rgb(r, g, b);
    }
    else if (CLRINFO->palette_type == GRX_COLOR_PALETTE_TYPE_GRAYSCALE) {
        res = grx_color_build_grayscale(r, g, b);
    }
    else {
        GR_int32u minerr = 1000;
        int i;
        int free_ = (-1), allfree = (-1), best = (-1);
        int ndef = (int)CLRINFO->ncolors - (int)CLRINFO->nfree;
        DBGPRINTF(DBG_COLOR, ("Allocating color: r=%d, g=%d, b=%d\n", r, g, b));
        for (i = 0; i < (int)CLRINFO->ncolors; i++) {
            if (CLRINFO->ctable[i].defined) {
                if (!CLRINFO->ctable[i].writable) {
                    GR_int32u err = 0;
                    GR_int16u colerr;
                    colerr = iabs(r - CLRINFO->ctable[i].r);
                    colerr = colerr * colerr;
                    err += colerr;
                    colerr = iabs(g - CLRINFO->ctable[i].g);
                    colerr = colerr * colerr;
                    err += colerr;
                    colerr = iabs(b - CLRINFO->ctable[i].b);
                    colerr = colerr * colerr;
                    err += colerr;
                    if (err < minerr) {
                        DBGPRINTF(DBG_COLOR,
                            ("New best color %d (err=%ld): r=%d, g=%d, b=%d\n", i, err,
                                (int)CLRINFO->ctable[i].r, (int)CLRINFO->ctable[i].g,
                                (int)CLRINFO->ctable[i].b));
                        best = i;
                        if ((minerr = err) == 0)
                            goto foundbest;
                    }
                    if ((free_ <= 0) && !CLRINFO->ctable[i].nused) {
                        DBGPRINTF(DBG_COLOR, ("First free color: r=%d\n", i));
                        free_ = i;
                    }
                }
                if (CLRINFO->ctable[i].nused)
                    ndef--;
            }
            else {
                if (allfree < 0)
                    allfree = i;
            }
            if ((allfree >= 0) && (ndef <= 0)) {
                DBGPRINTF(DBG_COLOR,
                    ("Found a usable color: allfree = %d, ndef = %d\n", allfree, ndef));
                break;
            }
        }
        if (allfree >= 0) {
            DBGPRINTF(
                DBG_COLOR, ("Using %d as free color (free=%d)\n", allfree, free_));
            free_ = allfree;
        }
        if (free_ >= 0) {
            DBGPRINTF(DBG_COLOR, ("Allocating %d\n", free_));
            CLRINFO->ctable[free_].defined = TRUE;
            CLRINFO->ctable[free_].writable = FALSE;
            CLRINFO->ctable[free_].nused = 1;
            CLRINFO->nfree--;
            load_color(free_, r, g, b);
            res = free_;
            goto done;
        }
    foundbest:
        if (best >= 0) {
            DBGPRINTF(DBG_COLOR, ("Using best %d\n", best));
            if (!CLRINFO->ctable[best].nused)
                CLRINFO->nfree--;
            CLRINFO->ctable[best].nused++;
            res = best;
            goto done;
        }
    }
done:
    GRX_RETURN(res);
}

/**
 * grx_color_cell_get:
 *
 * Allocates a new color that can be changed with grx_color_cell_set().
 *
 * The color must be freed with grx_color_cell_put().
 *
 * RGB and grayscale graphics modes will always return #GRX_COLOR_NONE.
 *
 * Returns: a newly allocated color cell or #GRX_COLOR_NONE if there are no more
 *          free cells.
 */
GrxColorCell grx_color_cell_get(void)
{
    if (CLRINFO->palette_type != GRX_COLOR_PALETTE_TYPE_COLOR_TABLE) {
        return GRX_COLOR_NONE;
    }
    if (CLRINFO->nfree) {
        int i, free_ = (-1);
        for (i = 0; i < (int)CLRINFO->ncolors; i++) {
            if (!CLRINFO->ctable[i].defined) {
                free_ = i;
                break;
            }
            if (!CLRINFO->ctable[i].nused) {
                if (free_ < 0)
                    free_ = i;
            }
        }
        if (free_ >= 0) {
            CLRINFO->ctable[free_].defined = TRUE;
            CLRINFO->ctable[free_].writable = TRUE;
            CLRINFO->ctable[free_].nused = 1;
            CLRINFO->nfree--;
            load_color(free_, 0, 0, 0);
            return (GrxColor)free_;
        }
    }
    return GRX_COLOR_NONE;
}

/**
 * grx_color_put:
 * @c: the color
 *
 * Releases a color that was obtained by grx_color_get() and friends.
 *
 * This has no effect in RGB or grayscale graphics modes.
 */
void grx_color_put(GrxColor c)
{
    if (CLRINFO->palette_type != GRX_COLOR_PALETTE_TYPE_COLOR_TABLE) {
        return;
    }

    if ((GrxColor)(c) < CLRINFO->ncolors && !CLRINFO->ctable[(int)(c)].writable
        && CLRINFO->ctable[(int)(c)].defined
        && (--CLRINFO->ctable[(int)(c)].nused == 0)) {
        CLRINFO->nfree++;
        CLRINFO->ctable[(int)(c)].defined = FALSE;
        CLRINFO->ctable[(int)(c)].writable = FALSE;
        CLRINFO->ctable[(int)(c)].nused = 0;
    }
}

/**
 * grx_color_cell_put:
 * @c: the color
 *
 * Releases a cell that was obtained with grx_color_cell_get().
 */
void grx_color_cell_put(GrxColor c)
{
    GRX_ENTER();
    if (CLRINFO->palette_type != GRX_COLOR_PALETTE_TYPE_COLOR_TABLE) {
        GRX_LEAVE();
        return;
    }
    if ((GrxColor)(c) < CLRINFO->ncolors) {
        if (CLRINFO->ctable[(int)(c)].writable) {
            CLRINFO->nfree++;
            CLRINFO->ctable[(int)(c)].defined = FALSE;
            CLRINFO->ctable[(int)(c)].writable = FALSE;
            CLRINFO->ctable[(int)(c)].nused = 0;
        }
    }
    GRX_LEAVE();
}

/**
 * grx_color_cell_set:
 * @c: a color that was allocated by grx_color_cell_get()
 * @r: the red component
 * @g: the green component
 * @b: the blue component
 *
 * Sets/changes the color of a cell after it was allocated.
 */
void grx_color_cell_set(GrxColor c, unsigned char r, unsigned char g, unsigned char b)
{
    GRX_ENTER();
    if (CLRINFO->palette_type != GRX_COLOR_PALETTE_TYPE_COLOR_TABLE) {
        GRX_LEAVE();
        return;
    }
    if ((GrxColor)(c) < CLRINFO->ncolors) {
        if (!CLRINFO->ctable[(int)(c)].defined) {
            CLRINFO->ctable[(int)(c)].defined = TRUE;
            CLRINFO->ctable[(int)(c)].nused = 0;
        }
        if (!CLRINFO->ctable[(int)(c)].nused) {
            CLRINFO->ctable[(int)(c)].writable = TRUE;
            CLRINFO->ctable[(int)(c)].nused = 1;
            CLRINFO->nfree--;
        }
        if (CLRINFO->ctable[(int)(c)].writable)
            load_color((int)(c), (int)ROUNDCOLORCOMP(r, 0), (int)ROUNDCOLORCOMP(g, 1),
                (int)ROUNDCOLORCOMP(b, 2));
    }
    GRX_LEAVE();
}

/**
 * grx_color_query:
 * @c: the color
 * @r: (out): the red component
 * @g: (out): the green component
 * @b: (out): the blue component
 *
 * Gets the RGB color components of a color.
 */
void grx_color_query(GrxColor c, unsigned char *r, unsigned char *g, unsigned char *b)
{
    GRX_ENTER();
    grx_color_query_inline(c, r, g, b);
    GRX_LEAVE();
}

/**
 * grx_color_query2:
 * @c: the color
 * @hcolor: (out): the components in 0xRRGGBB format
 *
 * Gets the RGB color components of a color.
 */
void grx_color_query2(GrxColor c, unsigned int *hcolor)
{
    GRX_ENTER();
    grx_color_query2_inline(c, hcolor);
    GRX_LEAVE();
}

#define CSAVE_MAGIC 0x7abf5698UL

typedef struct {
    GrxColor magic;
    GrxColor nc;
    struct _GR_colorInfo info;
} colorsave;

int grx_color_info_get_save_buffer_size(void)
{
    return sizeof(colorsave);
}

void grx_color_info_save_colors(void *buffer)
{
    colorsave *cp = (colorsave *)buffer;
    cp->magic = CSAVE_MAGIC;
    cp->nc = grx_color_info_n_colors();
    memcpy(&cp->info, CLRINFO, sizeof(cp->info));
}

void grx_color_info_restore_colors(void *buffer)
{
    colorsave *cp = (colorsave *)buffer;
    if ((cp->magic == CSAVE_MAGIC) && (cp->nc == grx_color_info_n_colors())) {
        memcpy(CLRINFO, &cp->info, sizeof(*CLRINFO));
        grx_color_info_refresh_colors();
    }
}
