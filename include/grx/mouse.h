/*
 * mouse.h
 *
 * Copyright (c) 2015 David Lechner <david@lechnology.com>
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

#ifndef __GRX_INPUT_H__
#define __GRX_INPUT_H__

#ifndef __GI_SCANNER__

#include <grx/color.h>
#include <grx/common.h>

/* ================================================================== */
/*                    GRAPHICS CURSOR UTILITIES                       */
/* ================================================================== */

typedef struct _GR_cursor {
    GrxContext work;                            /* work areas (4) */
    int     xcord,ycord;                        /* cursor position on screen */
    int     xsize,ysize;                        /* cursor size */
    int     xoffs,yoffs;                        /* LU corner to hot point offset */
    int     xwork,ywork;                        /* save/work area sizes */
    int     xwpos,ywpos;                        /* save/work area position on screen */
    int     displayed;                          /* set if displayed */
} GrCursor;

GrCursor *GrBuildCursor(char *pixels,int pitch,int w,int h,int xo,int yo,const GrxColorTable c);
void GrDestroyCursor(GrCursor *cursor);
void GrDisplayCursor(GrCursor *cursor);
void GrEraseCursor(GrCursor *cursor);
void GrMoveCursor(GrCursor *cursor,int x,int y);

/* ================================================================== */
/*               MOUSE AND KEYBOARD INPUT UTILITIES                   */
/* ================================================================== */

#define GR_M_MOTION         0x001               /* mouse event flag bits */
#define GR_M_LEFT_DOWN      0x002
#define GR_M_LEFT_UP        0x004
#define GR_M_RIGHT_DOWN     0x008
#define GR_M_RIGHT_UP       0x010
#define GR_M_MIDDLE_DOWN    0x020
#define GR_M_MIDDLE_UP      0x040
#define GR_M_P4_DOWN        0x400
#define GR_M_P4_UP          0x800
#define GR_M_P5_DOWN        0x2000
#define GR_M_P5_UP          0x4000
#define GR_M_BUTTON_DOWN    (GR_M_LEFT_DOWN | GR_M_MIDDLE_DOWN | GR_M_RIGHT_DOWN | GR_M_P4_DOWN | GR_M_P5_DOWN)
#define GR_M_BUTTON_UP      (GR_M_LEFT_UP   | GR_M_MIDDLE_UP   | GR_M_RIGHT_UP   | GR_M_P4_UP   | GR_M_P5_UP)
#define GR_M_BUTTON_CHANGE  (GR_M_BUTTON_UP | GR_M_BUTTON_DOWN )

#define GR_M_LEFT           0x01                /* mouse button index bits */
#define GR_M_RIGHT          0x02
#define GR_M_MIDDLE         0x04
#define GR_M_P4             0x08
#define GR_M_P5             0x10

#define GR_M_KEYPRESS       0x080               /* other event flag bits */
#define GR_M_POLL           0x100
#define GR_M_NOPAINT        0x200
#define GR_COMMAND          0x1000
#define GR_M_EVENT          (GR_M_MOTION | GR_M_KEYPRESS | GR_M_BUTTON_CHANGE | GR_COMMAND)

#define GR_KB_RIGHTSHIFT    0x01                /* Keybd states: right shift key depressed */
#define GR_KB_LEFTSHIFT     0x02                /* left shift key depressed */
#define GR_KB_CTRL          0x04                /* CTRL depressed */
#define GR_KB_ALT           0x08                /* ALT depressed */
#define GR_KB_SCROLLOCK     0x10                /* SCROLL LOCK active */
#define GR_KB_NUMLOCK       0x20                /* NUM LOCK active */
#define GR_KB_CAPSLOCK      0x40                /* CAPS LOCK active */
#define GR_KB_INSERT        0x80                /* INSERT state active */
#define GR_KB_SHIFT         (GR_KB_LEFTSHIFT | GR_KB_RIGHTSHIFT)

#define GR_M_CUR_NORMAL     0                   /* MOUSE CURSOR modes: just the cursor */
#define GR_M_CUR_RUBBER     1                   /* rectangular rubber band (XOR-d to the screen) */
#define GR_M_CUR_LINE       2                   /* line attached to the cursor */
#define GR_M_CUR_BOX        3                   /* rectangular box dragged by the cursor */

#define GR_M_QUEUE_SIZE     128                 /* default queue size */

typedef struct _GR_mouseEvent {                 /* mouse event buffer structure */
        int  flags;                                 /* event type flags (see above) */
        int  x,y;                                   /* mouse coordinates */
        int  buttons;                               /* mouse button state */
        int  key;                                   /* key code from keyboard */
        int  kbstat;                                /* keybd status (ALT, CTRL, etc..) */
        long dtime;                                 /* time since last event (msec) */
} GrMouseEvent;

/*
 * mouse status information
 */
extern const struct _GR_mouseInfo {
        int   (*block)(GrxContext*,int,int,int,int); /* mouse block function */
        void  (*unblock)(int flags);                /* mouse unblock function */
        void  (*uninit)(void);                      /* mouse cleanupt function */
        struct _GR_cursor     *cursor;              /* the mouse cursor */
        struct _GR_mouseEvent *queue;               /* queue of pending input events */
        int     msstatus;                           /* -1:missing, 0:unknown, 1:present, 2:initted */
        int     displayed;                          /* cursor is (generally) drawn */
        int     blockflag;                          /* cursor temp. erase/block flag */
        int     docheck;                            /* need to check before gr. op. to screen */
        int     cursmode;                           /* mouse cursor draw mode */
        int     x1,y1,x2,y2;                        /* auxiliary params for some cursor draw modes */
        GrxColor curscolor;                          /* color for some cursor draw modes */
        int     owncursor;                          /* auto generated cursor */
        int     xpos,ypos;                          /* current mouse position */
        int     xmin,xmax;                          /* mouse movement X coordinate limits */
        int     ymin,ymax;                          /* mouse movement Y coordinate limits */
        int     spmult,spdiv;                       /* mouse cursor speed factors */
        int     thresh,accel;                       /* mouse acceleration parameters */
        int     moved;                              /* mouse cursor movement flag */
        int     qsize;                              /* max size of the queue */
        int     qlength;                            /* current # of items in the queue */
        int     qread;                              /* read pointer for the queue */
        int     qwrite;                             /* write pointer for the queue */
} * const GrMouseInfo;

int  GrMouseDetect(void);
void GrMouseEventMode(int dummy);
void GrMouseInit(void);
void GrMouseInitN(int queue_size);
void GrMouseUnInit(void);
void GrMouseSetSpeed(int spmult,int spdiv);
void GrMouseSetAccel(int thresh,int accel);
void GrMouseSetLimits(int x1,int y1,int x2,int y2);
void GrMouseGetLimits(int *x1,int *y1,int *x2,int *y2);
void GrMouseWarp(int x,int y);
void GrMouseEventEnable(int enable_kb,int enable_ms);
void GrMouseGetEvent(int flags,GrMouseEvent *event);

void GrMouseGetEventT(int flags,GrMouseEvent *event,long timout_msecs);
/* Note:
**       event->dtime is only valid if any event occured (event->flags !=0)
**       otherwise it's set as -1.
**       Additionally event timing is now real world time. (X11 && Linux
**       used clock(), user process time, up to 2.28f)
*/

int  GrMousePendingEvent(void);

GrCursor *GrMouseGetCursor(void);
void GrMouseSetCursor(GrCursor *cursor);
void GrMouseSetColors(GrxColor fg,GrxColor bg);
void GrMouseSetCursorMode(int mode,...);
void GrMouseDisplayCursor(void);
void GrMouseEraseCursor(void);
void GrMouseUpdateCursor(void);
int  GrMouseCursorIsDisplayed(void);

int  GrMouseBlock(GrxContext *c,int x1,int y1,int x2,int y2);
void GrMouseUnBlock(int return_value_from_GrMouseBlock);

#ifndef GRX_SKIP_INLINES
#define GrMouseEventMode(x)         /* nothing! */
#define GrMouseGetCursor()          (GrMouseInfo->cursor)
#define GrMouseCursorIsDisplayed()  (GrMouseInfo->displayed)
#define GrMouseInit()               GrMouseInitN(GR_M_QUEUE_SIZE);
#define GrMouseGetEvent(f,ev)       GrMouseGetEventT((f),(ev),(-1L));
#define GrMousePendingEvent() (                                                \
        GrMouseUpdateCursor(),                                                 \
   (GrMouseInfo->qlength > 0)                                                  \
)
#define GrMouseUnInit() do {                                                   \
        if(GrMouseInfo->uninit) {                                              \
        (*GrMouseInfo->uninit)();                                              \
        }                                                                      \
} while(0)
#define GrMouseGetLimits(x1p,y1p,x2p,y2p) do {                                 \
        *(x1p) = GrMouseInfo->xmin; *(y1p) = GrMouseInfo->ymin;                \
        *(x2p) = GrMouseInfo->xmax; *(y2p) = GrMouseInfo->ymax;                \
} while(0)
#define GrMouseBlock(c,x1,y1,x2,y2) (                                          \
        (((c) ? (const GrxContext*)(c) : grx_context_get_current())->gc_is_on_screen && \
         (GrMouseInfo->docheck)) ?                                             \
        (*GrMouseInfo->block)((c),(x1),(y1),(x2),(y2)) :                       \
        0                                                                      \
)
#define GrMouseUnBlock(f) do {                                                 \
        if((f) && GrMouseInfo->displayed) {                                    \
        (*GrMouseInfo->unblock)((f));                                          \
        }                                                                      \
} while(0)
#endif  /* GRX_SKIP_INLINES */

#endif /* __GI_SCANNER__ */

#endif /* __GRX_INPUT_H__ */
