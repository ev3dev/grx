/*
 * input.h ---- declarations and code pieces for input processing
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

#include "mouse.h"

#define test_unblock(flag)          \
    do {                            \
        static int checkcnt = 1000; \
        (flag) = FALSE;             \
        if (--checkcnt <= 0) {      \
            checkcnt = 1000;        \
            flag = (TRUE);          \
        }                           \
    } while (0)

#define COMPATIBLE(c) ((c)->work.gc_driver->mode == SDRV->rmode)

void _GrInitMouseCursor(void);
int _GrCheckKeyboardHit(void);
int _GrReadCharFromKeyboard(void);
