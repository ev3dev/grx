/*
 * Copyright (C) 1993-97 by Hartmut Schirmer
 */

#ifndef __BCCLNX_H
#define __BCCLNX_H

#include <unistd.h>

/* Borland like delay() function */
static __inline__ void delay(int ms) {
  usleep(ms*1000);
}

#endif
