/**
 ** LIBGRX.H ---- GRX library private include file
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#ifndef __LIBGRX_H_INCLUDED__
#define __LIBGRX_H_INCLUDED__

#include <stdlib.h>
#include <stdio.h>

#ifndef __GRX20_H_INCLUDED__
#include "grx20.h"
#endif

#ifndef NULL
#define NULL            ((void *)(0))
#endif

#ifndef FALSE
#define FALSE           0
#endif

#ifndef TRUE
#define TRUE            1
#endif

#ifndef uint
#define uint            unsigned int
#define uchar           unsigned char
#define ushort          unsigned short
#define ulong           unsigned long
#endif

/*
** get system endian
*/
#ifdef __GO32__
#  include <machine/endian.h>
#elif defined(__TURBOC__)
#  define _LITTLE_ENDIAN
#elif defined(__linux__)
#  include <endian.h>
#else
#  include <sys/byteorder.h>
#endif

#if defined(__BYTE_ORDER__) && !defined(BYTE_ORDER)
#  define BYTE_ORDER    __BYTE_ORDER__
#  define LITTLE_ENDIAN __LITTLE_ENDIAN__
#  define BIG_ENDIAN    __BIG_ENDIAN__
#endif
#if !defined(BYTE_ORDER) && defined(_LITTLE_ENDIAN)
#define LITTLE_ENDIAN 0x1234
#define BIG_ENDIAN    0x4321
#define BYTE_ORDER    LITTLE_ENDIAN
#endif
#if !defined(BYTE_ORDER) && defined(_BIG_ENDIAN)
#define LITTLE_ENDIAN 0x1234
#define BIG_ENDIAN    0x4321
#define BYTE_ORDER    BIG_ENDIAN
#endif
#ifndef BYTE_ORDER
#error Unknown byte ordering !
#endif
#ifndef HARDWARE_BYTE_ORDER
#define HARDWARE_BYTE_ORDER BYTE_ORDER
#endif

/*
 * global library data structures
 */
extern  struct _GR_driverInfo  _GrDriverInfo;
extern  struct _GR_contextInfo _GrContextInfo;
extern  struct _GR_colorInfo   _GrColorInfo;
extern  struct _GR_mouseInfo   _GrMouseInfo;

#define GrDriverInfo    (&_GrDriverInfo)
#define GrContextInfo   (&_GrContextInfo)
#define GrColorInfo     (&_GrColorInfo)
#define GrMouseInfo     (&_GrMouseInfo)

#define DRVINFO         (&_GrDriverInfo)
#define CXTINFO         (&_GrContextInfo)
#define CLRINFO         (&_GrColorInfo)
#define MOUINFO         (&_GrMouseInfo)

#define CURC            (&(CXTINFO->current))
#define SCRN            (&(CXTINFO->screen))
#define FDRV            (&(DRVINFO->fdriver))
#define SDRV            (&(DRVINFO->sdriver))
#define VDRV            ( (DRVINFO->vdriver))

/*
 * banking stuff
 */
#ifndef BANKHOOK
#define BANKHOOK
#endif

#ifndef RWBANKHOOK
#define RWBANKHOOK
#endif

#define BANKPOS(offs)   ((ushort)(offs))
#ifdef  __TURBOC__
#define BANKNUM(offs)   (((ushort *)(&(offs)))[1])
#define BANKLFT(offs)   (_AX = -(int)(BANKPOS(offs)),(_AX ? _AX : 0xffffU))
#else
#define BANKNUM(offs)   ((int)((ulong)(offs) >> 16))
#define BANKLFT(offs)   (0x10000 - BANKPOS(offs))
#endif

#define SETBANK(bk) do {                            \
        register int _bankval_ = (bk);              \
        DRVINFO->curbank = _bankval_;               \
        (*DRVINFO->setbank)(_bankval_);             \
        BANKHOOK;                                   \
} while(0)

#define SRWBANK(rb,wb) do {                         \
        DRVINFO->curbank = (-1);                    \
        (*DRVINFO->setrwbanks)((rb),(wb));          \
        RWBANKHOOK;                                 \
} while(0)

#define CHKBANK(bk) do {                            \
        register int _bankval_ = (bk);              \
        if(_bankval_ != DRVINFO->curbank) {         \
        DRVINFO->curbank = _bankval_;               \
        (*DRVINFO->setbank)(_bankval_);             \
        BANKHOOK;                                   \
        }                                           \
} while(0)

/*
 * color stuff
 */
#ifdef  __TURBOC__
#define C_OPER(color)   (uint)(((uchar *)(&(color)))[3])
#else
#define C_OPER(color)   (uint)((ulong)(color) >> 24)
#endif
#define C_WRITE         (int)(GrWRITE >> 24)
#define C_XOR           (int)(GrXOR   >> 24)
#define C_OR            (int)(GrOR    >> 24)
#define C_AND           (int)(GrAND   >> 24)
#define C_IMAGE         (int)(GrIMAGE >> 24)

/*
 * mouse stuff
 */
#define mouse_block(c,x1,y1,x2,y2) {                                        \
        int __mouse_block_flag = 0;                                         \
        mouse_addblock(c,x1,y1,x2,y2);
#define mouse_addblock(c,x1,y1,x2,y2)                                       \
        if(MOUINFO->docheck && (c)->gc_onscreen) {                          \
        __mouse_block_flag |= (*MOUINFO->block)((c),(x1),(y1),(x2),(y2));   \
        }
#define mouse_unblock()                                                     \
        if(__mouse_block_flag) {                                            \
        (*MOUINFO->unblock)(__mouse_block_flag);                            \
        }                                                                       \
}

/*
 * internal utility functions
 */
GrFrameDriver *_GrFindFrameDriver(GrFrameMode mode);
GrFrameDriver *_GrFindRAMframeDriver(GrFrameMode mode);

void _GrCloseVideoDriver(void);
void _GrDummyFunction(void);

/*
 * missing functions for compatibility
 */
void _GrFillPattern(int x,int y,int width,GrPattern *p);
void _GrPatternFilledLine(int x1,int y1,int dx,int dy,GrPattern *p);

#endif  /* whole file */

