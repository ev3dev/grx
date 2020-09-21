/*
 * memcopy.h ---- inline assembly memory copy macros
 *
 * Copyright (c) 1998 Hartmut Schirmer
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
 *
 * Basic and optimized memory block copy operations in byte, word and
 * long sizes. The copys are available in WRITE, XOR, OR and AND modes.
 */

#ifndef __MEMCOPY_H_INCLUDED__
#define __MEMCOPY_H_INCLUDED__

#ifndef __MEMMODE_H_INCLUDED__
#include "memmode.h"
#endif
#ifndef __MEMPEEK_H_INCLUDED__
#include "mempeek.h"
#endif
#ifndef __ARITH_H_INCLUDED__
#include "arith.h"
#endif

#if !defined(CPSIZE_b) && defined(GR_int8)
#define CPSIZE_b sizeof(GR_int8)
#endif
#if !defined(CPSIZE_w) && defined(GR_int16)
#define CPSIZE_w sizeof(GR_int16)
#endif
#if !defined(CPSIZE_l) && defined(GR_int32)
#define CPSIZE_l sizeof(GR_int32)
#endif
#if !defined(CPSIZE_h) && defined(GR_int64)
#define CPSIZE_h sizeof(GR_int64)
#endif

#ifdef __GNUC__
#include "gcc/memcopy.h"
#endif

#if !defined(GR_int64) && !defined(NO_64BIT_COPY)
#define NO_64BIT_COPY
#endif
#if !defined(GR_int32) && !defined(NO_32BIT_COPY)
#define NO_32BIT_COPY
#endif
#if !defined(GR_int16) && !defined(NO_16BIT_COPY)
#define NO_16BIT_COPY
#endif

/* Note:
**   These functions alter the destination and source pointers and the
**   counter. Destination and source pointer _must_ point to next copy
**   location at return
*/

/* memory -> memory copies */
#ifndef INLINE_STD_COLCOPY
#define INLINE_STD_COLCOPY(D, DSKP, S, SSKP, C, DM, SM, INS, SIZE, TYPE) \
    do {                                                                 \
        poke##DM((D), peek##SM(S));                                      \
        ptrinc((D), (DSKP));                                             \
        ptrinc((S), (SSKP));                                             \
    } while (--(C))
#endif

#ifndef INLINE_STD_OPRCOPY
#define INLINE_STD_OPRCOPY(D, S, C, DM, SM, INS, SIZE, TY) \
    INLINE_STD_COLCOPY(D, sizeof(TY), S, sizeof(TY), C, DM, SM, INS, SIZE, TY)
#endif

#ifndef INLINE_STD_ROWCOPY
#define INLINE_STD_ROWCOPY(D, S, C, DM, SM, SIZE, TYPE) \
    INLINE_STD_OPRCOPY(D, S, C, DM, SM, MOV_INS, SIZE, TYPE)
#endif

/*
 * rowcopy_<SIZE>[_<OPER>](dst,src,count)
 *                ^-^-^-dst-^-^-^   ^src^
 */
/* memory -> memory copy */
#ifndef rowcopy_b_set
#define rowcopy_b_set(d, s, c) INLINE_STD_ROWCOPY(d, s, c, _b, _b, OP8b, GR_int8)
#endif
#ifndef rowcopy_w_set
#ifdef NO_16BIT_COPY
#define rowcopy_w_set(d, s, c) rowcopy_b_set(d, s, (c) << 1)
#else
#define rowcopy_w_set(d, s, c) INLINE_STD_ROWCOPY(d, s, c, _w, _w, OP16b, GR_int16)
#endif
#endif
#ifndef rowcopy_l_set
#ifdef NO_32BIT_COPY
#define rowcopy_l_set(d, s, c) rowcopy_w_set(d, s, (c) << 1)
#else
#define rowcopy_l_set(d, s, c) INLINE_STD_ROWCOPY(d, s, c, _l, _l, OP32b, GR_int32)
#endif
#endif
#if defined(GR_int64) && !defined(rowcopy_h_set)
#ifdef NO_64BIT_COPY
#define rowcopy_h_set(d, s, c) rowcopy_l_set(d, s, (c) << 1)
#else
#define rowcopy_h_set(d, s, c) INLINE_STD_ROWCOPY(d, s, c, _h, _h, OP64b, GR_int64)
#endif
#endif

#define rowcopy_b rowcopy_b_set
#define rowcopy_w rowcopy_w_set
#define rowcopy_l rowcopy_l_set
#ifdef rowcopy_h_set
#define rowcopy_h rowcopy_h_set
#endif

#ifndef rowcopy_b_xor
#define rowcopy_b_xor(d, s, c) \
    INLINE_STD_OPRCOPY(d, s, c, _b_xor, _b, XOR_INS, OP8b, GR_int8)
#endif
#ifndef rowcopy_w_xor
#ifdef NO_16BIT_COPY
#define rowcopy_w_xor(d, s, c) rowcopy_b_xor(d, s, (c) << 1)
#else
#define rowcopy_w_xor(d, s, c) \
    INLINE_STD_OPRCOPY(d, s, c, _w_xor, _w, XOR_INS, OP16b, GR_int16)
#endif
#endif
#ifndef rowcopy_l_xor
#ifdef NO_32BIT_COPY
#define rowcopy_l_xor(d, s, c) rowcopy_w_xor(d, s, (c) << 1)
#else
#define rowcopy_l_xor(d, s, c) \
    INLINE_STD_OPRCOPY(d, s, c, _l_xor, _l, XOR_INS, OP32b, GR_int32)
#endif
#endif
#if defined(GR_int64) && !defined(rowcopy_h_xor)
#ifdef NO_64BIT_COPY
#define rowcopy_h_xor(d, s, c) rowcopy_l_xor(d, s, (c) << 1)
#else
#define rowcopy_h_xor(d, s, c) \
    INLINE_STD_OPRCOPY(d, s, c, _h_xor, _h, XOR_INS, OP64b, GR_int64)
#endif
#endif

#ifndef rowcopy_b_or
#define rowcopy_b_or(d, s, c) \
    INLINE_STD_OPRCOPY(d, s, c, _b_or, _b, OR_INS, OP8b, GR_int8)
#endif
#ifndef rowcopy_w_or
#ifdef NO_16BIT_COPY
#define rowcopy_w_or(d, s, c) rowcopy_b_or(d, s, (c) << 1)
#else
#define rowcopy_w_or(d, s, c) \
    INLINE_STD_OPRCOPY(d, s, c, _w_or, _w, OR_INS, OP16b, GR_int16)
#endif
#endif
#ifndef rowcopy_l_or
#ifdef NO_32BIT_COPY
#define rowcopy_l_or(d, s, c) rowcopy_w_or(d, s, (c) << 1)
#else
#define rowcopy_l_or(d, s, c) \
    INLINE_STD_OPRCOPY(d, s, c, _l_or, _l, OR_INS, OP32b, GR_int32)
#endif
#endif
#if defined(GR_int64) && !defined(rowcopy_h_or)
#ifdef NO_64BIT_COPY
#define rowcopy_h_or(d, s, c) rowcopy_l_or(d, s, (c) << 1)
#else
#define rowcopy_h_or(d, s, c) \
    INLINE_STD_OPRCOPY(d, s, c, _h_or, _h, OR_INS, OP64b, GR_int64)
#endif
#endif

#ifndef rowcopy_b_and
#define rowcopy_b_and(d, s, c) \
    INLINE_STD_OPRCOPY(d, s, c, _b_and, _b, AND_INS, OP8b, GR_int8)
#endif
#ifndef rowcopy_w_and
#ifdef NO_16BIT_COPY
#define rowcopy_w_and(d, s, c) rowcopy_b_and(d, s, (c) << 1)
#else
#define rowcopy_w_and(d, s, c) \
    INLINE_STD_OPRCOPY(d, s, c, _w_and, _w, AND_INS, OP16b, GR_int16)
#endif
#endif
#ifndef rowcopy_l_and
#ifdef NO_32BIT_COPY
#define rowcopy_l_and(d, s, c) rowcopy_w_and(d, s, (c) << 1)
#else
#define rowcopy_l_and(d, s, c) \
    INLINE_STD_OPRCOPY(d, s, c, _l_and, _l, AND_INS, OP32b, GR_int32)
#endif
#endif
#if defined(GR_int64) && !defined(rowcopy_h_and)
#ifdef NO_64BIT_COPY
#define rowcopy_h_and(d, s, c) rowcopy_l_and(d, s, (c) << 1)
#else
#define rowcopy_h_and(d, s, c) \
    INLINE_STD_OPRCOPY(d, s, c, _h_and, _h, AND_INS, OP64b, GR_int64)
#endif
#endif

#define rowcopy_b_n_set_n rowcopy_b_set
#define rowcopy_b_n_xor_n rowcopy_b_xor
#define rowcopy_b_n_or_n  rowcopy_b_or
#define rowcopy_b_n_and_n rowcopy_b_and
#define rowcopy_w_n_set_n rowcopy_w_set
#define rowcopy_w_n_xor_n rowcopy_w_xor
#define rowcopy_w_n_or_n  rowcopy_w_or
#define rowcopy_w_n_and_n rowcopy_w_and
#define rowcopy_l_n_set_n rowcopy_l_set
#define rowcopy_l_n_xor_n rowcopy_l_xor
#define rowcopy_l_n_or_n  rowcopy_l_or
#define rowcopy_l_n_and_n rowcopy_l_and
#ifdef rowcopy_l_set
#define rowcopy_h_n_set_n rowcopy_h_set
#define rowcopy_h_n_xor_n rowcopy_h_xor
#define rowcopy_h_n_or_n  rowcopy_h_or
#define rowcopy_h_n_and_n rowcopy_h_and
#endif

/*
 * colcopy_<SIZE>[_<OPER>](dst,doffs,src,srcoffs,count)
 *                ^-^-^-dst-^-^-^   ^src^
 */
/* memory -> memory copy */
#ifndef colcopy_b_set
#define colcopy_b_set(d, dof, s, sof, c) \
    INLINE_STD_COLCOPY(d, dof, s, sof, c, _b, _b, MOV_INS, OP8b, GR_int8)
#endif
#ifndef colcopy_w_set
#define colcopy_w_set(d, dof, s, sof, c) \
    INLINE_STD_COLCOPY(d, dof, s, sof, c, _w, _w, MOV_INS, OP16b, GR_int16)
#endif
#if defined(GR_int32) && !defined(colcopy_l_set)
#define colcopy_l_set(d, dof, s, sof, c) \
    INLINE_STD_COLCOPY(d, dof, s, sof, c, _l, _l, MOV_INS, OP32b, GR_int32)
#endif
#if defined(GR_int64) && !defined(colcopy_h_set)
#define colcopy_h_set(d, dof, s, sof, c) \
    INLINE_STD_COLCOPY(d, dof, s, sof, c, _h, _h, MOV_INS, OP64b, GR_int64)
#endif

#define colcopy_b colcopy_b_set
#define colcopy_w colcopy_w_set
#define colcopy_l colcopy_l_set
#ifdef colcopy_h_set
#define colcopy_h colcopy_h_set
#endif

#ifndef colcopy_b_xor
#define colcopy_b_xor(d, dof, s, sof, c) \
    INLINE_STD_COLCOPY(d, dof, s, sof, c, _b_xor, _b, XOR_INS, OP8b, GR_int8)
#endif
#ifndef colcopy_w_xor
#define colcopy_w_xor(d, dof, s, sof, c) \
    INLINE_STD_COLCOPY(d, dof, s, sof, c, _w_xor, _w, XOR_INS, OP16b, GR_int16)
#endif
#if defined(GR_int32) && !defined(colcopy_l_xor)
#define colcopy_l_xor(d, dof, s, sof, c) \
    INLINE_STD_COLCOPY(d, dof, s, sof, c, _l_xor, _l, XOR_INS, OP32b, GR_int32)
#endif
#if defined(GR_int64) && !defined(colcopy_h_xor)
#define colcopy_h_xor(d, dof, s, sof, c) \
    INLINE_STD_COLCOPY(d, dof, s, sof, c, _h_xor, _h, XOR_INS, OP64b, GR_int64)
#endif

#ifndef colcopy_b_or
#define colcopy_b_or(d, dof, s, sof, c) \
    INLINE_STD_COLCOPY(d, dof, s, sof, c, _b_or, _b, OR_INS, OP8b, GR_int8)
#endif
#ifndef colcopy_w_or
#define colcopy_w_or(d, dof, s, sof, c) \
    INLINE_STD_COLCOPY(d, dof, s, sof, c, _w_or, _w, OR_INS, OP16b, GR_int16)
#endif
#if defined(GR_int32) && !defined(colcopy_l_or)
#define colcopy_l_or(d, dof, s, sof, c) \
    INLINE_STD_COLCOPY(d, dof, s, sof, c, _l_or, _l, OR_INS, OP32b, GR_int32)
#endif
#if defined(GR_int64) && !defined(colcopy_h_or)
#define colcopy_h_or(d, dof, s, sof, c) \
    INLINE_STD_COLCOPY(d, dof, s, sof, c, _h_or, _h, OR_INS, OP64b, GR_int64)
#endif

#ifndef colcopy_b_and
#define colcopy_b_and(d, dof, s, sof, c) \
    INLINE_STD_COLCOPY(d, dof, s, sof, c, _b_and, _b, AND_INS, OP8b, GR_int8)
#endif
#ifndef colcopy_w_and
#define colcopy_w_and(d, dof, s, sof, c) \
    INLINE_STD_COLCOPY(d, dof, s, sof, c, _w_and, _w, AND_INS, OP16b, GR_int16)
#endif
#if defined(GR_int32) && !defined(colcopy_l_and)
#define colcopy_l_and(d, dof, s, sof, c) \
    INLINE_STD_COLCOPY(d, dof, s, sof, c, _l_and, _l, AND_INS, OP32b, GR_int32)
#endif
#if defined(GR_int64) && !defined(colcopy_h_and)
#define colcopy_h_and(d, dof, s, sof, c) \
    INLINE_STD_COLCOPY(d, dof, s, sof, c, _h_and, _h, AND_INS, OP64b, GR_int64)
#endif

#define colcopy_b_n_set_n colcopy_b_set
#define colcopy_b_n_xor_n colcopy_b_xor
#define colcopy_b_n_or_n  colcopy_b_or
#define colcopy_b_n_and_n colcopy_b_and
#define colcopy_w_n_set_n colcopy_w_set
#define colcopy_w_n_xor_n colcopy_w_xor
#define colcopy_w_n_or_n  colcopy_w_or
#define colcopy_w_n_and_n colcopy_w_and
#define colcopy_l_n_set_n colcopy_l_set
#define colcopy_l_n_xor_n colcopy_l_xor
#define colcopy_l_n_or_n  colcopy_l_or
#define colcopy_l_n_and_n colcopy_l_and
#ifdef colcopy_h_set
#define colcopy_h_n_set_n colcopy_h_set
#define colcopy_h_n_xor_n colcopy_h_xor
#define colcopy_h_n_or_n  colcopy_h_or
#define colcopy_h_n_and_n colcopy_h_and
#endif

/*
 * fwdcopy_<OPER>(alp,dst,src,cnt)
 * revcopy_<OPER>(alp,dst,src,count)
 *         ^-^-^-dst-^-^-^   ^src^
 *
 * optimized copy - will use fastest available copy
 * alp: alignment pointer will be moved to an aligned
 *      boundary by small size copy ops before bulk copy is
 *      done using max. data width. Must equal dst or src
 * dst: destination pointer, on return points to first address
 *      following the copy area
 * src: source pointer, on return points to first address
 *      following the copy area
 * cnt: number of bytes to be copied. Changed.
 */

/* generic single element copy */
#ifndef INLINE_1_COPY
#define INLINE_1_COPY(D, S, SZ, WOP, SF, INC)  \
    do {                                       \
        poke_##SZ##WOP((D), peek_##SZ##SF(S)); \
        ptrinc((D), (INC));                    \
        ptrinc((S), (INC));                    \
    } while (0)
#endif

/* generic optimized forward copy (meets NO alignment restriction :( */
#ifndef INLINE_STD_FWD_COPY
#define INLINE_STD_FWD_COPY(SZ, WOP, SF, ALP, DST, SRC, CNT)           \
    do {                                                               \
        if ((GR_PtrInt)(CNT) >= 2 * CPSIZE_##SZ - 1) {                 \
            GR_PtrInt _c_ = (-((GR_PtrInt)(ALP))) & (CPSIZE_##SZ - 1); \
            if (_c_) {                                                 \
                (CNT) -= _c_;                                          \
                rowcopy_b##WOP##SF(DST, SRC, _c_);                     \
            }                                                          \
            _c_ = ((GR_PtrInt)(CNT)) / CPSIZE_##SZ;                    \
            rowcopy_##SZ##WOP##SF(DST, SRC, _c_);                      \
            (CNT) &= (CPSIZE_##SZ - 1);                                \
        }                                                              \
        if ((GR_PtrInt)(CNT))                                          \
        rowcopy_b##WOP##SF(DST,SRC,CNT));                              \
    } while (0)
#endif

/* generic optimized reverse copy (meets NO alignment resrictions :( */
#ifndef INLINE_STD_REV_COPY
#define INLINE_STD_REV_COPY(SZ, WOP, SF, ALP, DST, SRC, CNT)                      \
    do {                                                                          \
        if ((GR_PtrInt)(CNT) >= 2 * CPSIZE_##SZ - 1) {                            \
            GR_PtrInt _c_ = ((GR_PtrInt)(ALP) + 1) & (CPSIZE_##SZ - 1);           \
            if (_c_) {                                                            \
                (CNT) -= _c_;                                                     \
                colcopy_b##WOP##SF(DST, (-1), SRC, (-1), _c_);                    \
            }                                                                     \
            _c_ = ((GR_PtrInt)(CNT)) / CPSIZE_##SZ;                               \
            ptrinc(DST, -(CPSIZE_##SZ) + 1);                                      \
            ptrinc(SRC, -(CPSIZE_##SZ) + 1);                                      \
            colcopy_##SZ##WOP##SF(DST, -(CPSIZE_##SZ), SRC, -(CPSIZE_##SZ), _c_); \
            ptrinc(DST, (CPSIZE_##SZ) - 1);                                       \
            ptrinc(SRC, (CPSIZE_##SZ) - 1);                                       \
            (CNT) &= (CPSIZE_##SZ - 1);                                           \
        }                                                                         \
        if ((GR_PtrInt)(CNT))                                                     \
            colcopy_b##WOP##SF(DST, (-1), SRC, (-1), CNT);                        \
    } while (0)
#endif

/* forward copy: copy and step to next higher alignment boundary */
#ifndef INLINE_FWD_ALIGN
#define INLINE_FWD_ALIGN(SZ, WOP, SF, AP, D, S, C)     \
    if (((GR_PtrInt)(AP)) & CPSIZE_##SZ) {             \
        INLINE_1_COPY(D, S, SZ, WOP, SF, CPSIZE_##SZ); \
        (C) -= CPSIZE_##SZ;                            \
        if (!((GR_PtrInt)(C)))                         \
            break;                                     \
    }
#endif

/* forward copy: copy one remaining element */
#ifndef INLINE_FWD_TAIL
#define INLINE_FWD_TAIL(SZ, WOP, SF, D, S, C)              \
    do                                                     \
        if (((GR_PtrInt)(C)) & CPSIZE_##SZ)                \
            INLINE_1_COPY(D, S, SZ, WOP, SF, CPSIZE_##SZ); \
    while (0)
#endif

/* ------------------- check for system alignment requirements ------------- */
#if defined(MISALIGNED_64bit_OK) && !defined(NO_64BIT_COPY) \
    && defined(MISALIGNED_32bit_OK) && defined(MISALIGNED_16bit_OK)
/* 8,16,32,64bit misaligned ok */
#ifndef INLINE_FWD_COPY
#define INLINE_FWD_COPY(WOP, SF, AP, D, S, C)          \
    do {                                               \
        if (((GR_PtrInt)(C)) >= 7) {                   \
            INLINE_FWD_ALIGN(b, WOP, SF, AP, D, S, C); \
            INLINE_FWD_ALIGN(w, WOP, SF, AP, D, S, C); \
            INLINE_FWD_ALIGN(l, WOP, SF, AP, D, S, C); \
            {                                          \
                GR_PtrInt _c_ = ((GR_PtrInt)(C)) >> 3; \
                if (_c_)                               \
                    rowcopy_h##WOP##SF(D, S, _c_);     \
            }                                          \
        }                                              \
        INLINE_FWD_TAIL(l, WOP, SF, D, S, C);          \
        INLINE_FWD_TAIL(w, WOP, SF, D, S, C);          \
        INLINE_FWD_TAIL(b, WOP, SF, D, S, C);          \
    } while (0)
#endif
#ifndef INLINE_REV_COPY
#define INLINE_REV_COPY(WOP, SF, AP, D, S, C)                \
    do {                                                     \
        if (((GR_PtrInt)(C)) >= 7) {                         \
            if (!(((GR_PtrInt)(AP)) & 1)) {                  \
                INLINE_1_COPY(D, S, b, WOP, SF, -1);         \
                (C)--;                                       \
            }                                                \
            if (!(((GR_PtrInt)(AP)) & 2)) {                  \
                ptrinc((D), -1);                             \
                ptrinc((S), -1);                             \
                INLINE_1_COPY(D, S, w, WOP, SF, -1);         \
                (C) -= 2;                                    \
            }                                                \
            if (!(((GR_PtrInt)(AP)) & 4)) {                  \
                ptrinc((D), -3);                             \
                ptrinc((S), -3);                             \
                INLINE_1_COPY(D, S, l, WOP, SF, -1);         \
                (C) -= 4;                                    \
            }                                                \
            {                                                \
                GR_PtrInt _rch_ = ((GR_PtrInt)(C)) >> 3;     \
                if (_rch_) {                                 \
                    ptrinc((D), -7);                         \
                    ptrinc((S), -7);                         \
                    colcopy_h##WOP##SF(D, -8, S, -8, _rch_); \
                    ptrinc((D), +7);                         \
                    ptrinc((S), +7);                         \
                }                                            \
            }                                                \
        }                                                    \
        if (((GR_PtrInt)(C)) & 4) {                          \
            ptrinc((D), -3);                                 \
            ptrinc((S), -3);                                 \
            INLINE_1_COPY(D, S, l, WOP, SF, -1);             \
        }                                                    \
        if (((GR_PtrInt)(C)) & 2) {                          \
            ptrinc((D), -1);                                 \
            ptrinc((S), -1);                                 \
            INLINE_1_COPY(D, S, w, WOP, SF, -1);             \
        }                                                    \
        if (((GR_PtrInt)(C)) & 1)                            \
            INLINE_1_COPY(D, S, b, WOP, SF, -1);             \
    } while (0)
#endif

#elif defined(MISALIGNED_64bit_OK) && !defined(NO_64BIT_COPY)
/* 8,64bit misaligned ok -> standard code */
#ifndef INLINE_FWD_COPY
#define INLINE_FWD_COPY(WOP, SF, AP, D, S, C) \
    INLINE_STD_FWD_COPY(h, WOP, SF, AP, D, S, C)
#endif
#ifndef INLINE_REV_COPY
#define INLINE_REV_COPY(WOP, SF, AP, D, S, C) \
    INLINE_STD_REV_COPY(h, WOP, SF, AP, D, S, C)
#endif

#elif defined(MISALIGNED_32bit_OK) && !defined(NO_32BIT_COPY) \
    && defined(MISALIGNED_16bit_OK)
/* 8,16,32bit misaligned ok */
#ifndef INLINE_FWD_COPY
#define INLINE_FWD_COPY(WOP, SF, AP, D, S, C)          \
    do {                                               \
        if (((GR_PtrInt)(C)) >= 3) {                   \
            GR_PtrInt _fcl_;                           \
            INLINE_FWD_ALIGN(b, WOP, SF, AP, D, S, C); \
            INLINE_FWD_ALIGN(w, WOP, SF, AP, D, S, C); \
            if ((_fcl_ = ((GR_PtrInt)(C)) >> 2) != 0)  \
                rowcopy_l##WOP##SF(D, S, _fcl_);       \
        }                                              \
        INLINE_FWD_TAIL(w, WOP, SF, D, S, C);          \
        INLINE_FWD_TAIL(b, WOP, SF, D, S, C);          \
    } while (0)
#endif
#ifndef INLINE_REV_COPY
#define INLINE_REV_COPY(WOP, SF, AP, D, S, C)                \
    do {                                                     \
        if (((GR_PtrInt)(C)) >= 3) {                         \
            if (!(((GR_PtrInt)(AP)) & 1)) {                  \
                INLINE_1_COPY(D, S, b, WOP, SF, -1);         \
                (C)--;                                       \
            }                                                \
            if (!(((GR_PtrInt)(AP)) & 2)) {                  \
                ptrinc((D), -1);                             \
                ptrinc((S), -1);                             \
                INLINE_1_COPY(D, S, w, WOP, SF, -1);         \
                (C) -= 2;                                    \
            }                                                \
            {                                                \
                GR_PtrInt _rcl_ = ((GR_PtrInt)(C)) >> 2;     \
                if (_rcl_) {                                 \
                    ptrinc((D), -3);                         \
                    ptrinc((S), -3);                         \
                    colcopy_l##WOP##SF(D, -4, S, -4, _rcl_); \
                    ptrinc((D), +3);                         \
                    ptrinc((S), +3);                         \
                }                                            \
            }                                                \
        }                                                    \
        if (((GR_PtrInt)(C)) & 2) {                          \
            ptrinc((D), -1);                                 \
            ptrinc((S), -1);                                 \
            INLINE_1_COPY(D, S, w, WOP, SF, -1);             \
        }                                                    \
        if (((GR_PtrInt)(C)) & 1)                            \
            INLINE_1_COPY(D, S, b, WOP, SF, -1);             \
    } while (0)
#endif

#elif defined(MISALIGNED_32bit_OK) && !defined(NO_32BIT_COPY)
/* 8,32bit misaligned ok -> standard code */
#ifndef INLINE_FWD_COPY
#define INLINE_FWD_COPY(WOP, SF, AP, D, S, C) \
    INLINE_STD_FWD_COPY(l, WOP, SF, AP, D, S, C)
#endif
#ifndef INLINE_REV_COPY
#define INLINE_REV_COPY(WOP, SF, AP, D, S, C) \
    INLINE_STD_REV_COPY(l, WOP, SF, AP, D, S, C)
#endif

#elif defined(MISALIGNED_16bit_OK) && !defined(NO_16BIT_COPY)
/* 8,16bit misaligned ok */
#ifndef INLINE_FWD_COPY
#define INLINE_FWD_COPY(WOP, SF, AP, D, S, C)            \
    do                                                   \
        if ((GR_PtrInt)(C)) {                            \
            INLINE_FWD_ALIGN(b, WOP, SF, AP, D, S, C);   \
            {                                            \
                GR_PtrInt _fcw_ = ((GR_PtrInt)(C)) >> 1; \
                if (_fcw_)                               \
                    rowcopy_w##WOP##SF(D, S, _fcw_);     \
            }                                            \
            INLINE_FWD_TAIL(b, WOP, SF, D, S, C);        \
        }                                                \
    while (0)
#endif
#ifndef INLINE_REV_COPY
#define INLINE_REV_COPY(WOP, SF, AP, D, S, C)                \
    do                                                       \
        if ((GR_PtrInt)(C)) {                                \
            if (!(((GR_PtrInt)(AP)) & 1)) {                  \
                INLINE_1_COPY(D, S, b, WOP, SF, -1);         \
                (C)--;                                       \
                if (!((GR_PtrInt)(C)))                       \
                    break;                                   \
            }                                                \
            {                                                \
                GR_PtrInt _rcw_ = ((GR_PtrInt)(C)) >> 1;     \
                if (_rcw_) {                                 \
                    ptrinc((D), -1);                         \
                    ptrinc((S), -1);                         \
                    colcopy_w##WOP##SF(D, -2, S, -2, _rcw_); \
                    ptrinc((D), +1);                         \
                    ptrinc((S), +1);                         \
                }                                            \
            }                                                \
            if (((GR_PtrInt)(C)) & 1)                        \
                INLINE_1_COPY(D, S, b, WOP, SF, -1);         \
        }                                                    \
    while (0)
#endif

#endif

/* ---------------------------------------- fall back to byte copy */
#ifndef INLINE_FWD_COPY
#define INLINE_FWD_COPY(WOP, SF, AP, D, S, C) rowcopy_b##WOP##SF(D, S, C)
#endif
#ifndef INLINE_REV_COPY
#define INLINE_REV_COPY(WOP, SF, AP, D, S, C) colcopy_b##WOP##SF(D, -1, S, -1, C)
#endif

/* memory -> memory copy */
#ifndef fwdcopy_set
#define fwdcopy_set(ap, d, s, c) INLINE_FWD_COPY(_n_set, _n, ap, d, s, c)
#endif
#ifndef fwdcopy_xor
#define fwdcopy_xor(ap, d, s, c) INLINE_FWD_COPY(_n_xor, _n, ap, d, s, c)
#endif
#ifndef fwdcopy_or
#define fwdcopy_or(ap, d, s, c) INLINE_FWD_COPY(_n_or, _n, ap, d, s, c)
#endif
#ifndef fwdcopy_and
#define fwdcopy_and(ap, d, s, c) INLINE_FWD_COPY(_n_and, _n, ap, d, s, c)
#endif
#ifndef revcopy_set
#define revcopy_set(ap, d, s, c) INLINE_REV_COPY(_n_set, _n, ap, d, s, c)
#endif
#ifndef revcopy_xor
#define revcopy_xor(ap, d, s, c) INLINE_REV_COPY(_n_xor, _n, ap, d, s, c)
#endif
#ifndef revcopy_or
#define revcopy_or(ap, d, s, c) INLINE_REV_COPY(_n_or, _n, ap, d, s, c)
#endif
#ifndef revcopy_and
#define revcopy_and(ap, d, s, c) INLINE_REV_COPY(_n_and, _n, ap, d, s, c)
#endif

/*
 * stuff to copy arrays, structures
 */
#define memcopy(d, s, sze)                                           \
    do {                                                             \
        register void *_CD = (void *)(d);                            \
        register void *_CS = (void *)(s);                            \
        register unsigned GR_PtrInt _CC = (unsigned GR_PtrInt)(sze); \
        DBGPRINTF(DBG_COPYFILL, ("memcopy size=%u\n", _CC));         \
        fwdcopy_set(_CD, _CD, _CS, _CC);                             \
    } while (0)
#define sttcopy(dstp, srcp) memcopy((dstp), (srcp), sizeof(*(srcp)))

#endif /* whole file */
