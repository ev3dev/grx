/**
 ** input.h ---- declarations and code pieces for input processing
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu]
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

#define update_coord(WHICH,MICKEYS) do {                                    \
        static int fract = 0;                                               \
        int delta,ddelta,pos;                                               \
        delta   = (MICKEYS) * MOUINFO->spmult;                              \
        ddelta  = (delta + fract) / MOUINFO->spdiv;                         \
        fract   = (delta + fract) % MOUINFO->spdiv;                         \
        if(iabs(ddelta) >= MOUINFO->thresh) ddelta *= MOUINFO->accel;       \
        pos     = MOUINFO->WHICH##pos + ddelta;                             \
        if(pos  < MOUINFO->WHICH##min) pos = MOUINFO->WHICH##min;           \
        if(pos  > MOUINFO->WHICH##max) pos = MOUINFO->WHICH##max;           \
        if(pos != MOUINFO->WHICH##pos) {                                    \
        MOUINFO->WHICH##pos = pos;                                          \
        MOUINFO->moved      = TRUE;                                         \
        }                                                                   \
} while(0)

#define fill_mouse_ev(EV,OLDBT,NEWBT,LB,MB,RB,KBSTAT) do {                  \
        int bdown   = NEWBT & (~OLDBT);                                     \
        int btnup   = OLDBT & (~NEWBT);                                     \
        EV.flags    = MOUINFO->moved ? GR_M_MOTION : 0;                     \
        EV.x        = MOUINFO->xpos;                                        \
        EV.y        = MOUINFO->ypos;                                        \
        EV.kbstat   = KBSTAT;                                               \
        EV.key      = 0;                                                    \
        EV.buttons  = 0;                                                    \
        if(NEWBT & LB) EV.buttons |= GR_M_LEFT;                             \
        if(NEWBT & MB) EV.buttons |= GR_M_MIDDLE;                           \
        if(NEWBT & RB) EV.buttons |= GR_M_RIGHT;                            \
        if(bdown & LB) EV.flags   |= GR_M_LEFT_DOWN;                        \
        if(bdown & MB) EV.flags   |= GR_M_MIDDLE_DOWN;                      \
        if(bdown & RB) EV.flags   |= GR_M_RIGHT_DOWN;                       \
        if(btnup & LB) EV.flags   |= GR_M_LEFT_UP;                          \
        if(btnup & MB) EV.flags   |= GR_M_MIDDLE_UP;                        \
        if(btnup & RB) EV.flags   |= GR_M_RIGHT_UP;                         \
} while(0)

#define fill_keybd_ev(EV,KEY,KBSTAT) do {                                   \
        EV.flags    = GR_M_KEYPRESS | (MOUINFO->moved ? GR_M_MOTION : 0);   \
        EV.x        = MOUINFO->xpos;                                        \
        EV.y        = MOUINFO->ypos;                                        \
        EV.key      = KEY;                                                  \
        EV.kbstat   = KBSTAT;                                               \
        EV.buttons = 0;                                                     \
} while(0)

#define enqueue_event(EV) do {                                              \
        sttcopy(&MOUINFO->queue[MOUINFO->qwrite],&EV);                      \
        if(++MOUINFO->qwrite == MOUINFO->qsize) MOUINFO->qwrite = 0;        \
        if(++MOUINFO->qlength > MOUINFO->qsize) {                           \
        MOUINFO->qlength--;                                                 \
        if(++MOUINFO->qread == MOUINFO->qsize) MOUINFO->qread = 0;          \
        }                                                                   \
} while(0)

#define dequeue_event(EV) do {                                              \
        if(MOUINFO->qlength > 0) {                                          \
        sttcopy(&EV,&MOUINFO->queue[MOUINFO->qread]);                       \
        if(++MOUINFO->qread == MOUINFO->qsize) MOUINFO->qread = 0;          \
        MOUINFO->qlength--;                                                 \
        }                                                                   \
} while(0)

#define init_queue(N) do {                                                  \
        if(MOUINFO->qsize != N) {                                           \
        if(MOUINFO->queue != NULL) free(MOUINFO->queue);                    \
        MOUINFO->queue = malloc(sizeof(MOUINFO->queue[0]) * N);             \
        MOUINFO->qsize = MOUINFO->queue ? N : 0;                            \
        }                                                                   \
        MOUINFO->qread   = 0;                                               \
        MOUINFO->qwrite  = 0;                                               \
        MOUINFO->qlength = 0;                                               \
} while(0);


#if defined(__TURBOC__) || defined(__WATCOMC__) /* GS - Watcom C++ 11.0 */
#define user_time(tv) do {                                                  \
        (tv) = *(long far *)(MK_FP(0x40,0x6c));                             \
} while(0)
#define real_time(tv) user_time(tv)
#define MS_PER_TICK 55
#endif

#ifdef __GO32__
#define user_time(tv) do {                                                  \
        setup_far_selector(LINP_SEL(0x0000046c));                           \
        (tv) = peek_l_f(LINP_PTR(0x0000046c));                              \
} while(0)
#define real_time(tv) user_time(tv)
#define MS_PER_TICK 55
#endif

#if !defined(user_time) && ( defined(unix) || defined(__WIN32__) )
#include <time.h>
#define user_time(tv) do {                                                  \
        (tv) = clock();                                                     \
} while(0)
#define MS_PER_TICK (int)(1000 / CLOCKS_PER_SEC)
#endif

#if !defined(real_time) && defined(unix)
#include <sys/times.h>
#define real_time(tv) do {                                                  \
        (tv) = times(NULL);                                                 \
} while(0)
#endif

#if !defined(real_time)
#define real_time(tv) user_time(tv)
#endif

#define user_dtime(dt,oldtime) do {                                         \
        long newtime;                                                       \
        user_time(newtime);                                                 \
        (dt) = (newtime - oldtime) * MS_PER_TICK;                           \
        oldtime = newtime;                                                  \
} while(0)

#define real_dtime(dt,oldtime) do {                                         \
        long newtime;                                                       \
        real_time(newtime);                                                 \
        (dt) = (newtime - oldtime) * MS_PER_TICK;                           \
        oldtime = newtime;                                                  \
} while(0)

#ifdef __MSDOS__
#define test_unblock(flag) do {                                             \
        static long lastcheck = 0L;                                         \
        long checktime;                                                     \
        user_time(checktime);                                               \
        flag = (int)(checktime - lastcheck);                                \
        lastcheck = checktime;                                              \
} while(0)
#else
#define test_unblock(flag) do {                                             \
        static int checkcnt = 1000;                                         \
        (flag) = FALSE;                                                     \
        if(--checkcnt <= 0) {                                               \
        checkcnt = 1000;                                                    \
        flag = (TRUE);                                                      \
        }                                                                   \
} while(0)
#endif

#define COMPATIBLE(c)   ((c)->work.gc_driver->mode == SDRV->rmode)

void _GrUpdateInputs(void);
void _GrInitMouseCursor(void);
#ifndef __MSDOS__
int  _GrCheckKeyboardHit(void);
int  _GrReadCharFromKeyboard(void);
#endif
