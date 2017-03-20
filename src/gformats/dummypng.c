/*
 * dummypng.c ---- dummy png funtions if not PNG support in the library
 *
 * Copyright (C) 2001 Mariano Alvarez Fernandez
 * [e-mail: malfer@teleline.es]
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

#include <stdio.h>
#include <stdlib.h>

/*
** grx_context_save_to_png - Returns error
*/

int grx_context_save_to_png( GrxContext *grc, char *pngfn )
{
  return -1;
}

/*
** grx_context_load_from_png - Returns error
*/

int grx_context_load_from_png( GrxContext *grc, char *pngfn, int use_alpha )
{
  return -1;
}

/*
** grx_query_pnm_file - Returns error
*/

int grx_query_png_file( char *pngfn, int *width, int *height )
{
  return -1;
}
