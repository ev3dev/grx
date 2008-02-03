/**
 ** auxintl.c ---- auxiliary intl routines 
 **
 ** Copyright (C) 2008 Mariano Alvarez Fernandez
 ** [e-mail: malfer at telefonica.net]
 **
 ** This file is part of the GRX graphics library.
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

#include <stdlib.h>
#include <string.h>
#include "libgrx.h"
#include "ninput.h"

char *GrStrEncoding(int nenc)
{
  if (nenc == GRENC_CP437)
    return ("CP437");
  else if (nenc == GRENC_CP850)
    return ("CP850");
  else if (nenc == GRENC_CP1252)
    return ("CP1252");
  else if (nenc == GRENC_ISO_8859_1)
    return ("ISO_8859_1");
  else if (nenc == GRENC_UTF_8)
    return ("UTF_8");
  else if (nenc == GRENC_UCS_2)
    return ("UCS_2");

  return ("UNKNOW");
}

int GrFindEncoding(char *strenc)
{
  if (strcmp(strenc,"CP437") == 0)
    return GRENC_CP437;
  else if (strcmp(strenc,"CP850") == 0)
    return GRENC_CP850;
  else if (strcmp(strenc,"CP1252") == 0)
    return GRENC_CP1252;
  else if (strcmp(strenc,"ISO_8859_1") == 0)
    return GRENC_ISO_8859_1;
  else if (strcmp(strenc,"UTF_8") == 0)
    return GRENC_UTF_8;
  else if (strcmp(strenc,"UCS_2") == 0)
    return GRENC_UCS_2;

  return -1;
}
