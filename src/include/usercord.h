/*
 * usercord.h
 *
 * Copyright (C) 1992, Csaba Biegl
 *   820 Stirrup Dr, Nashville, TN, 37221
 *   csaba@vuse.vanderbilt.edu
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

#include <grx/user.h>

#include "usrscale.h"

#define U2SX(x, c) SCALE(x, ((x) - (c)->user_x_base), (c)->x_max, (c)->user_width)
#define U2SY(y, c) SCALE(y, ((y) - (c)->user_y_base), (c)->y_max, (c)->user_height)

#define S2UX(x, c)                                \
    do {                                          \
        SCALE(x, x, (c)->user_width, (c)->x_max); \
        (x) += (c)->user_x_base;                  \
    } while (0)

#define S2UY(y, c)                                 \
    do {                                           \
        SCALE(y, y, (c)->user_height, (c)->y_max); \
        (y) += (c)->user_y_base;                   \
    } while (0)
