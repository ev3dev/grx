/*
 * dummyjpg.c ---- dummy jpeg funtions if not JPEG support in the library
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
** grx_context_save_to_jpeg - Returns error
*/

int grx_context_save_to_jpeg( GrxContext *grc, char *jpegfn, int quality )
{
  return -1;
}

/*
** grx_context_save_to_jpeg_grayscale - Returns error
*/

int grx_context_save_to_jpeg_grayscale( GrxContext *grc, char *jpegfn, int quality )
{
  return -1;
}

/*
** grx_context_load_from_jpeg - Returns error
*/

int grx_context_load_from_jpeg( GrxContext *grc, char *jpegfn, int scale )
{
  return -1;
}

/*
** grx_check_jpeg_file - Returns error
*/

int grx_check_jpeg_file( char *jpegfn, int *width, int *height )
{
  return -1;
}
