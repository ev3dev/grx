/*
 * clrinlne.c ---- the color inline functions
 *
 * Copyright (c) 2016-2017 David Lechner <david@lechnology.com>
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

#include <grx/color.h>

#include "libgrx.h"

/**
 * grx_color_lookup:
 * @table: (element-type GrxColor): the color table
 * @index: the index in the color table
 *
 * Looks up a color in the color table.
 *
 * Returns: the color or %GRX_COLOR_NONE if @index is out of range
 */
GrxColor (grx_color_lookup)(GArray *table, guint index)
{
        return(grx_color_lookup(table, index));
}

/**
 * grx_color_get_value:
 * @c: the source color value
 *
 * Gets the color component of a color value.
 *
 * Returns: the color component.
 */
GrxColor (grx_color_get_value)(GrxColor c)
{
        return(grx_color_get_value(c));
}

/**
 * grx_color_get_mode:
 * @c: the source color value
 *
 * Gets the #GrxColorMode component of a color value.
 *
 * Returns: the #GrxColorMode component.
 */
GrxColorMode (grx_color_get_mode)(GrxColor c)
{
        return(grx_color_get_mode(c));
}

/**
 * grx_color_to_write_mode:
 * @c: the source color value
 *
 * Sets the color to #GRX_COLOR_MODE_WRITE.
 *
 * Returns: the modified color value.
 */
GrxColor (grx_color_to_write_mode)(GrxColor c)
{
        return(grx_color_to_write_mode(c));
}

/**
 * grx_color_to_xor_mode:
 * @c: the source color value
 *
 * Sets the color to #GRX_COLOR_MODE_XOR.
 *
 * Returns: the modified color value.
 */
GrxColor (grx_color_to_xor_mode)(GrxColor c)
{
        return(grx_color_to_xor_mode(c));
}

/**
 * grx_color_to_or_mode:
 * @c: the source color value
 *
 * Sets the color to #GRX_COLOR_MODE_OR.
 *
 * Returns: the modified color value.
 */
GrxColor (grx_color_to_or_mode)(GrxColor c)
{
        return(grx_color_to_or_mode(c));
}

/**
 * grx_color_to_and_mode:
 * @c: the source color value
 *
 * Sets the color to #GRX_COLOR_MODE_AND.
 *
 * Returns: the modified color value.
 */
GrxColor (grx_color_to_and_mode)(GrxColor c)
{
        return(grx_color_to_and_mode(c));
}

/**
 * grx_color_to_image_mode:
 * @c: the source color value
 *
 * Sets the color to #GRX_COLOR_MODE_IMAGE.
 *
 * Returns: the modified color value.
 */
GrxColor (grx_color_to_image_mode)(GrxColor c)
{
        return(grx_color_to_image_mode(c));
}

/**
 * grx_color_info_n_colors:
 *
 * Gets the number of colors supported by the current graphics mode.
 *
 * Returns: the number of colors.
 */
int (grx_color_info_n_colors)(void)
{
        return(grx_color_info_n_colors());
}

/**
 * grx_color_info_n_free_colors:
 *
 * Gets the number of unallocated colors remaining for the current graphics mode.
 *
 * For RGB graphics modes, this will always return 0.
 *
 * Returns: the number of unallocated colors.
 */
int (grx_color_info_n_free_colors)(void)
{
        return(grx_color_info_n_free_colors());
}

/**
 * grx_color_info_get_palette_type:
 *
 * Gets the current color palette type.
 *
 * Returns: a #GrxColorPaletteType.
 */
GrxColorPaletteType (grx_color_info_get_palette_type)(void)
{
        return(grx_color_info_get_palette_type());
}

/**
 * grx_color_build_rgb:
 * @r: the red component value
 * @g: the green component value
 * @b: the blue component value
 *
 * Build a RGB color value based on the current graphics mode.
 *
 * This will not allocate a new color even if a color table is being used.
 *
 * Returns: a new color value.
 */
GrxColor (grx_color_build_rgb)(unsigned char r,unsigned char g,unsigned char b)
{
        return(grx_color_build_rgb(r,g,b));
}

/**
 * grx_color_build_rgb_round:
 * @r: the red component value
 * @g: the green component value
 * @b: the blue component value
 *
 * Build a RGB color value based on the current graphics mode.
 *
 * This function is similar to grx_color_build_rgb(), but will limit
 * the RGB components to 255 during the scaling.
 *
 * This will not allocate a new color even if a color table is being used.
 *
 * Returns: a new color value.
 */
GrxColor (grx_color_build_rgb_round)(unsigned char r,unsigned char g,unsigned char b)
{
        return(grx_color_build_rgb_round(r,g,b));
}

/**
 * grx_color_build_grayscale:
 * @r: the red component value
 * @g: the green component value
 * @b: the blue component value
 *
 * Build a grayscale color value based on the current graphics mode.
 *
 * This will not allocate a new color even if a color table is being used.
 *
 * Returns: a new color value.
 */
GrxColor (grx_color_build_grayscale)(unsigned char r,unsigned char g,unsigned char b)
{
        return(grx_color_build_grayscale(r,g,b));
}

/**
 * grx_color_get_red:
 * @c: the source color value.
 *
 * Gets the red component of a color value.
 *
 * This function only works on RGB colors and not color table values. For color
 * table values, use grx_color_query() instead.
 *
 * Returns: the red component.
 */
unsigned char (grx_color_get_red)(GrxColor c)
{
        return(grx_color_get_red(c));
}

/**
 * grx_color_get_green:
 * @c: the source color value.
 *
 * Gets the green component of a color value.
 *
 * This function only works on RGB colors and not color table values. For color
 * table values, use grx_color_query() instead.
 *
 * Returns: the green component.
 */
unsigned char (grx_color_get_green)(GrxColor c)
{
        return(grx_color_get_green(c));
}

/**
 * grx_color_get_blue:
 * @c: the source color value.
 *
 * Gets the blue component of a color value.
 *
 * This function only works on RGB colors and not color table values. For color
 * table values, use grx_color_query() instead.
 *
 * Returns: the blue component.
 */
unsigned char (grx_color_get_blue)(GrxColor c)
{
        return(grx_color_get_blue(c));
}

/**
 * grx_color_get_inline: (skip)
 * @r: the red component value
 * @g: the green component value
 * @b: the blue component value
 *
 * Gets a color in the color table based on the RGB value.
 *
 * This is the same as grx_color_get(), but inlined in C code.
 *
 * Returns: the composite color value or #GRX_COLOR_NONE if there are no more
 *          free colors.
 */
GrxColor (grx_color_get_inline)(unsigned char r,unsigned char g,unsigned char b)
{
        return(grx_color_get_inline(r,g,b));
}

/**
 * grx_color_get2:
 * @hcolor: the color in 0xRRGGBB format.
 *
 * Gets a color in the color table based on @hcolor.
 *
 * A new color is allocated in the color table if needed.
 *
 * The color should be released with grx_color_put() when it is no longer used.
 *
 * Returns: the composite color value or #GRX_COLOR_NONE if there are no more
 *          free colors.
 */
GrxColor (grx_color_get2)(unsigned int hcolor)
{
        return(grx_color_get2(hcolor));
}

/**
 * grx_color_get2_inline: (skip)
 * @hcolor: the color in 0xRRGGBB format.
 *
 * Gets a color in the color table based on @hcolor.
 *
 * This is the same as grx_color_get2(), but inlined in C code.
 *
 * Returns: the composite color value or #GRX_COLOR_NONE if there are no more
 *          free colors.
 */
GrxColor (grx_color_get2_inline)(unsigned int hcolor)
{
        return(grx_color_get2_inline(hcolor));
}

/**
 * grx_color_query_inline: (skip)
 * @c: the color
 * @r: (out): the red component
 * @g: (out): the green component
 * @b: (out): the blue component
 *
 * Gets the RGB color components of a color.
 *
 * This is the same as grx_color_query(), but inlined in C code.
 */
void (grx_color_query_inline)(GrxColor c,unsigned char *r,unsigned char *g,unsigned char *b)
{
        grx_color_query_inline(c,r,g,b);
}

/**
 * grx_color_query2_inline: (skip)
 * @c: the color
 * @hcolor: (out): the components in 0xRRGGBB format
 *
 * Gets the RGB color components of a color.
 *
 * This is the same as grx_color_query2(), but inlined in C code.
 */
void (grx_color_query2_inline)(GrxColor c,unsigned int *hcolor)
{
        grx_color_query2_inline(c,hcolor);
}
