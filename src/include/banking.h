/*
 * banking.h ---- GRX library private include file
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

#ifndef __INCLUDE_BANKING_H__
#define __INCLUDE_BANKING_H__

#include "globals.h"

/*
 * banking stuff
 */
#ifndef BANKHOOK
#define BANKHOOK
#endif

#ifndef RWBANKHOOK
#define RWBANKHOOK
#endif

#ifndef BANKPOS
#define BANKPOS(offs) ((GR_int16u)(offs))
#endif
#ifndef BANKNUM
#define BANKNUM(offs) ((int)((GR_int32u)(offs) >> 16))
#endif
#ifndef BANKLFT
#define BANKLFT(offs) (0x10000 - BANKPOS(offs))
#endif

#define SETBANK(bk)                      \
    do {                                 \
        register int _bankval_ = (bk);   \
        DRVINFO->curbank = _bankval_;    \
        (*DRVINFO->set_bank)(_bankval_); \
        BANKHOOK;                        \
    } while (0)

#define SRWBANK(rb, wb)                       \
    do {                                      \
        DRVINFO->curbank = (-1);              \
        (*DRVINFO->set_rw_banks)((rb), (wb)); \
        RWBANKHOOK;                           \
    } while (0)

#define CHKBANK(bk)                          \
    do {                                     \
        register int _bankval_ = (bk);       \
        if (_bankval_ != DRVINFO->curbank) { \
            DRVINFO->curbank = _bankval_;    \
            (*DRVINFO->set_bank)(_bankval_); \
            BANKHOOK;                        \
        }                                    \
    } while (0)

#endif /* __INCLUDE_BANKING_H__ */
