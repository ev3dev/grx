/*
 * cxtinfo.c ---- the context info data structure
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

#include "globals.h"
#include "libgrx.h"
#include "memmode.h"
#undef GrContextInfo

const struct _GR_contextInfo *const GrContextInfo = &_GrContextInfo;
struct _GR_contextInfo _GrContextInfo = {
    .current = {
        .frame = {
            .base_address   = NULL,
            .selector       = 0,
            .is_on_screen   = TRUE,
            .memory_flags   = 0,
            .line_offset    = 0,
            .driver         = &DRVINFO->tdriver,
        },
        .root = NULL,
    },
    .screen = {
        .frame = {
            .base_address   = NULL,
            .selector       = 0,
            .is_on_screen   = TRUE,
            .memory_flags   = 0,
            .line_offset    = 0,
            .driver         = &DRVINFO->tdriver,
        },
        .root = NULL,
    },
};
