/**
 ** XWININPUT.C ---- mouse and keyboard interface for X Windows
 **
 ** Author:     Ulrich Leodolter
 ** E-mail:     ulrich@lab1.psy.univie.ac.at
 ** Date:       Thu Sep 28 20:22:16 1995
 ** Comment:    Implements the same GRX functions as dosinput.c
 ** RCSId:      $Id: xwininput.c 1.2 1995/11/19 19:32:30 ulrich Exp $
 **/

#include <stdlib.h>
#include <sys/time.h>   /* for select() */

#include "libxwin.h"
#include "libgrx.h"
#include <X11/keysym.h>
#ifdef   __MSDOS__
#include <keys.h>
#else
#include "mouse/xwinkeys.h"
#endif
#include "allocate.h"
#include "arith.h"
#include "memfill.h"
#include "memcopy.h"
#include "mouse/input.h"

static int  kbd_enabled = TRUE;
static int  kbd_lastmod = 0;
static int  mou_enabled = TRUE;
static int  mou_buttons = 0;
static long evt_lasttime;

#if 0
long volatile   _XGrTickValue = -1;
static void     _XGrTickHandler (int signum)
{
  signal (signum, _XGrTickHandler);
  _XGrTickValue++;
}
#endif

static void uninit(void)
{
        if(MOUINFO->msstatus > 1) MOUINFO->msstatus = 1;
}

int GrMouseDetect(void)
{
        if(MOUINFO->msstatus == 0) {
          MOUINFO->msstatus = 1;                /* present, but not initted */
        }
        return((MOUINFO->msstatus > 0) ? TRUE : FALSE);
}

void GrMouseInitN(int queue_size)
{
        uninit();
        queue_size = umax(4,umin(256,queue_size));
        init_queue(queue_size);
        if(GrMouseDetect()) {
            GrMouseSetSpeed(1,1);
            GrMouseSetAccel(100,1);
            GrMouseSetLimits(0,0,SCRN->gc_xmax,SCRN->gc_ymax);
            GrMouseWarp((SCRN->gc_xmax >> 1),(SCRN->gc_ymax >> 1));
            _GrInitMouseCursor();
            MOUINFO->msstatus = 2;
            mou_buttons = 0;
            /*
             * Define an invisible X cursor for _XGrWindow
             */
            {
              static char cbits[8] = { 0,0,0,0,0,0,0,0, };
              Pixmap csource, cmask;
              XColor cfore, cback;
              Cursor curs;

              csource = cmask = XCreateBitmapFromData (_XGrDisplay,
                                                       _XGrWindow,
                                                       cbits,
                                                       8,
                                                       8
                                                       );
              cfore.red = cfore.green = cfore.blue = 0;
              cback.red = cback.green = cback.blue = 0;
              curs = XCreatePixmapCursor (_XGrDisplay,
                                          csource,
                                          cmask,
                                          &cfore,
                                          &cback,
                                          0,
                                          0
                                          );
              XDefineCursor (_XGrDisplay, _XGrWindow, curs);
            }
        }
        GrMouseEventEnable(TRUE,TRUE);
        read_time(evt_lasttime);
        MOUINFO->uninit = uninit;
}

void GrMouseSetSpeed(int spmult,int spdiv)
{
        MOUINFO->spmult = umin(16,umax(1,spmult));
        MOUINFO->spdiv  = umin(16,umax(1,spdiv));
}

void GrMouseSetAccel(int thresh,int accel)
{
        MOUINFO->thresh = umin(64,umax(1,thresh));
        MOUINFO->accel  = umin(16,umax(1,accel));
}

void GrMouseSetLimits(int x1,int y1,int x2,int y2)
{
        isort(x1,x2);
        isort(y1,y2);
        MOUINFO->xmin = imax(0,imin(x1,SCRN->gc_xmax));
        MOUINFO->ymin = imax(0,imin(y1,SCRN->gc_ymax));
        MOUINFO->xmax = imax(0,imin(x2,SCRN->gc_xmax));
        MOUINFO->ymax = imax(0,imin(y2,SCRN->gc_ymax));
}

void GrMouseWarp(int x,int y)
{
        MOUINFO->xpos = imax(MOUINFO->xmin,imin(MOUINFO->xmax,x));
        MOUINFO->ypos = imax(MOUINFO->ymin,imin(MOUINFO->ymax,y));
        GrMouseUpdateCursor();
        /*
         * Move the X cursor only if inside _XGrWindow
         */
        XWarpPointer (_XGrDisplay,
                      _XGrWindow,
                      _XGrWindow,
                      0,
                      0,
                      GrScreenX(),
                      GrScreenY(),
                      MOUINFO->xpos,
                      MOUINFO->ypos);
}

void GrMouseEventEnable(int enable_kb,int enable_ms)
{
        kbd_enabled = enable_kb;
        mou_enabled = enable_ms;
}

/* Keyboard Translation Table */

typedef struct {
  short key;
  unsigned short state;
  KeySym keysym;
} KeyEntry;

static KeyEntry _KeyTable[] = {
#if 0
  { K_Control_A            , ControlMask, XK_a },
  { K_Control_B            , ControlMask, XK_b },
  { K_Control_C            , ControlMask, XK_c },
  { K_Control_D            , ControlMask, XK_d },
  { K_Control_E            , ControlMask, XK_e },
  { K_Control_F            , ControlMask, XK_f },
  { K_Control_G            , ControlMask, XK_g },
  { K_BackSpace            , 0, XK_BackSpace },
  { K_Control_H            , ControlMask, XK_h },
  { K_Tab                  , 0, XK_Tab },
  { K_Control_I            , ControlMask, XK_i },
  { K_LineFeed             , 0, XK_Linefeed },
  { K_Control_J            , ControlMask, XK_j },
  { K_Control_K            , ControlMask, XK_k },
  { K_Control_L            , ControlMask, XK_l },
  { K_Return               , 0, XK_Return },
  { K_Control_M            , ControlMask, XK_m },
  { K_Control_N            , ControlMask, XK_n },
  { K_Control_O            , ControlMask, XK_o },
  { K_Control_P            , ControlMask, XK_p },
  { K_Control_Q            , ControlMask, XK_q },
  { K_Control_R            , ControlMask, XK_r },
  { K_Control_S            , ControlMask, XK_s },
  { K_Control_T            , ControlMask, XK_t },
  { K_Control_U            , ControlMask, XK_u },
  { K_Control_V            , ControlMask, XK_v },
  { K_Control_W            , ControlMask, XK_w },
  { K_Control_X            , ControlMask, XK_x },
  { K_Control_Y            , ControlMask, XK_y },
  { K_Control_Z            , ControlMask, XK_z },
  { K_Control_LBracket     , ControlMask, XK_bracketleft },
  { K_Escape               , 0, XK_Escape },
  { K_Control_BackSlash    , ControlMask, XK_backslash },
  { K_Control_RBracket     , ControlMask, XK_bracketright },
  { K_Control_Caret        , ControlMask, 0 },
  { K_Control_Underscore   , ControlMask, XK_underscore },
  { K_Space                , 0, XK_space },
  { K_ExclamationPoint     , 0, XK_exclam },
  { K_DoubleQuote          , 0, XK_quotedbl },
  { K_Hash                 , 0, 0 },
  { K_Dollar               , 0, XK_dollar },
  { K_Percent              , 0, XK_percent },
  { K_Ampersand            , 0, XK_ampersand },
  { K_Quote                , 0, XK_quoteleft },
  { K_LParen               , 0, XK_parenleft },
  { K_RParen               , 0, XK_parenright },
  { K_Star                 , 0, XK_multiply },
  { K_Plus                 , 0, XK_plus },
  { K_Comma                , 0, XK_comma },
  { K_Dash                 , 0, XK_minus },
  { K_Period               , 0, XK_period },
  { K_Slash                , 0, XK_slash },
  { K_Colon                , 0, XK_colon },
  { K_SemiColon            , 0, XK_semicolon },
  { K_LAngle               , 0, 0 },
  { K_Equals               , 0, XK_equal },
  { K_RAngle               , 0, 0 },
  { K_QuestionMark         , 0, XK_question },
  { K_At                   , 0, XK_at },
  { K_LBracket             , 0, XK_bracketleft },
  { K_BackSlash            , 0, XK_backslash },
  { K_RBracket             , 0, XK_bracketright },
  { K_Caret                , 0, 0 },
  { K_UnderScore           , 0, XK_underscore },
  { K_BackQuote            , 0, XK_quoteright },
  { K_LBrace               , 0, XK_braceleft },
  { K_Pipe                 , 0, XK_bar },
  { K_RBrace               , 0, XK_braceright },
  { K_Tilde                , 0, XK_asciitilde },
  { K_Control_Backspace    , ControlMask, XK_BackSpace },
#endif
  { K_Alt_Escape           , Mod1Mask, XK_Escape },
  { K_Control_At           , ControlMask, XK_at },
  { K_Alt_Backspace        , Mod1Mask, XK_BackSpace },
  { K_Alt_Q                , Mod1Mask, XK_q },
  { K_Alt_W                , Mod1Mask, XK_w },
  { K_Alt_E                , Mod1Mask, XK_e },
  { K_Alt_R                , Mod1Mask, XK_r },
  { K_Alt_T                , Mod1Mask, XK_t },
  { K_Alt_Y                , Mod1Mask, XK_y },
  { K_Alt_U                , Mod1Mask, XK_u },
  { K_Alt_I                , Mod1Mask, XK_i },
  { K_Alt_O                , Mod1Mask, XK_o },
  { K_Alt_P                , Mod1Mask, XK_p },
  { K_Alt_LBracket         , Mod1Mask, XK_bracketleft },
  { K_Alt_RBracket         , Mod1Mask, XK_bracketright },
  { K_Alt_Return           , Mod1Mask, XK_Return },
  { K_Alt_A                , Mod1Mask, XK_a },
  { K_Alt_S                , Mod1Mask, XK_s },
  { K_Alt_D                , Mod1Mask, XK_d },
  { K_Alt_F                , Mod1Mask, XK_f },
  { K_Alt_G                , Mod1Mask, XK_g },
  { K_Alt_H                , Mod1Mask, XK_h },
  { K_Alt_J                , Mod1Mask, XK_j },
  { K_Alt_K                , Mod1Mask, XK_k },
  { K_Alt_L                , Mod1Mask, XK_l },
  { K_Alt_Semicolon        , Mod1Mask, XK_semicolon },
  { K_Alt_Quote            , Mod1Mask, XK_quoteleft },
  { K_Alt_Backquote        , Mod1Mask, XK_quoteright },
  { K_Alt_Backslash        , Mod1Mask, XK_backslash },
  { K_Alt_Z                , Mod1Mask, XK_z },
  { K_Alt_X                , Mod1Mask, XK_x },
  { K_Alt_C                , Mod1Mask, XK_c },
  { K_Alt_V                , Mod1Mask, XK_v },
  { K_Alt_B                , Mod1Mask, XK_b },
  { K_Alt_N                , Mod1Mask, XK_n },
  { K_Alt_M                , Mod1Mask, XK_m },
  { K_Alt_Comma            , Mod1Mask, XK_comma },
  { K_Alt_Period           , Mod1Mask, XK_period },
  { K_Alt_Slash            , Mod1Mask, XK_slash },
  { K_Alt_KPStar           , Mod1Mask, XK_multiply },
  { K_F1                   , 0, XK_F1 },
  { K_F2                   , 0, XK_F2 },
  { K_F3                   , 0, XK_F3 },
  { K_F4                   , 0, XK_F4 },
  { K_F5                   , 0, XK_F5 },
  { K_F6                   , 0, XK_F6 },
  { K_F7                   , 0, XK_F7 },
  { K_F8                   , 0, XK_F8 },
  { K_F9                   , 0, XK_F9 },
  { K_F10                  , 0, XK_F10 },
  { K_Home                 , 0, XK_Home },
  { K_Up                   , 0, XK_Up },
  { K_PageUp               , 0, XK_Prior },
  { K_Alt_KPMinus          , Mod1Mask, XK_minus },
  { K_Left                 , 0, XK_Left },
  { K_Center               , 0, XK_5 },
  { K_Right                , 0, XK_Right },
  { K_Alt_KPPlus           , Mod1Mask, XK_plus },
  { K_End                  , 0, XK_End },
  { K_Down                 , 0, XK_Down },
  { K_PageDown             , 0, XK_Next },
  { K_Insert               , 0, XK_Insert },
  { K_Delete               , 0, XK_Delete },
  { K_Shift_F1             , ShiftMask, XK_F1 },
  { K_Shift_F2             , ShiftMask, XK_F2 },
  { K_Shift_F3             , ShiftMask, XK_F3 },
  { K_Shift_F4             , ShiftMask, XK_F4 },
  { K_Shift_F5             , ShiftMask, XK_F5 },
  { K_Shift_F6             , ShiftMask, XK_F6 },
  { K_Shift_F7             , ShiftMask, XK_F7 },
  { K_Shift_F8             , ShiftMask, XK_F8 },
  { K_Shift_F9             , ShiftMask, XK_F9 },
  { K_Shift_F10            , ShiftMask, XK_F10 },
  { K_Control_F1           , ControlMask, XK_F1 },
  { K_Control_F2           , ControlMask, XK_F2 },
  { K_Control_F3           , ControlMask, XK_F3 },
  { K_Control_F4           , ControlMask, XK_F4 },
  { K_Control_F5           , ControlMask, XK_F5 },
  { K_Control_F6           , ControlMask, XK_F6 },
  { K_Control_F7           , ControlMask, XK_F7 },
  { K_Control_F8           , ControlMask, XK_F8 },
  { K_Control_F9           , ControlMask, XK_F9 },
  { K_Control_F10          , ControlMask, XK_F10 },
  { K_Alt_F1               , Mod1Mask, XK_F1 },
  { K_Alt_F2               , Mod1Mask, XK_F2 },
  { K_Alt_F3               , Mod1Mask, XK_F3 },
  { K_Alt_F4               , Mod1Mask, XK_F4 },
  { K_Alt_F5               , Mod1Mask, XK_F5 },
  { K_Alt_F6               , Mod1Mask, XK_F6 },
  { K_Alt_F7               , Mod1Mask, XK_F7 },
  { K_Alt_F8               , Mod1Mask, XK_F8 },
  { K_Alt_F9               , Mod1Mask, XK_F9 },
  { K_Alt_F10              , Mod1Mask, XK_F10 },
  { K_Control_Left         , ControlMask, XK_Left },
  { K_Control_Right        , ControlMask, XK_Right },
  { K_Control_End          , ControlMask, XK_End },
  { K_Control_PageDown     , ControlMask, XK_Next },
  { K_Control_Home         , ControlMask, XK_Home },
  { K_Alt_1                , Mod1Mask, XK_1 },
  { K_Alt_2                , Mod1Mask, XK_2 },
  { K_Alt_3                , Mod1Mask, XK_3 },
  { K_Alt_4                , Mod1Mask, XK_4 },
  { K_Alt_5                , Mod1Mask, XK_5 },
  { K_Alt_6                , Mod1Mask, XK_6 },
  { K_Alt_7                , Mod1Mask, XK_7 },
  { K_Alt_8                , Mod1Mask, XK_8 },
  { K_Alt_9                , Mod1Mask, XK_9 },
  { K_Alt_0                , Mod1Mask, XK_0 },
  { K_Alt_Dash             , Mod1Mask, XK_minus },
  { K_Alt_Equals           , Mod1Mask, XK_equal },
  { K_Control_PageUp       , ControlMask, XK_Prior },
  { K_F11                  , 0, XK_F11 },
  { K_F12                  , 0, XK_F12 },
  { K_Shift_F11            , ShiftMask, XK_F11 },
  { K_Shift_F12            , ShiftMask, XK_F12 },
  { K_Control_F11          , ControlMask, XK_F11 },
  { K_Control_F12          , ControlMask, XK_F12 },
  { K_Alt_F11              , Mod1Mask, XK_F11 },
  { K_Alt_F12              , Mod1Mask, XK_F12 },
  { K_Control_Up           , ControlMask, XK_Up },
  { K_Control_KPDash       , ControlMask, XK_minus },
  { K_Control_Center       , ControlMask, XK_5 },
  { K_Control_KPPlus       , ControlMask, XK_plus },
  { K_Control_Down         , ControlMask, XK_Down },
  { K_Control_Insert       , ControlMask, XK_Insert },
  { K_Control_Delete       , ControlMask, XK_Delete },
  { K_Control_KPSlash      , ControlMask, XK_slash },
  { K_Control_KPStar       , ControlMask, XK_multiply },
  { K_Alt_EHome            , Mod1Mask, XK_Home },
  { K_Alt_EUp              , Mod1Mask, XK_Up },
  { K_Alt_EPageUp          , Mod1Mask, XK_Prior },
  { K_Alt_ELeft            , Mod1Mask, XK_Left },
  { K_Alt_ERight           , Mod1Mask, XK_Right },
  { K_Alt_EEnd             , Mod1Mask, XK_End },
  { K_Alt_EDown            , Mod1Mask, XK_Down },
  { K_Alt_EPageDown        , Mod1Mask, XK_Next },
  { K_Alt_EInsert          , Mod1Mask, XK_Insert },
  { K_Alt_EDelete          , Mod1Mask, XK_Delete },
  { K_Alt_KPSlash          , Mod1Mask, XK_KP_Divide },
  { K_Alt_Tab              , Mod1Mask, XK_Tab },
  { K_Alt_Enter            , Mod1Mask, XK_KP_Enter },

  { K_EHome                , 0, XK_Home },
  { K_EUp                  , 0, XK_Up },
  { K_EPageUp              , 0, XK_Prior },
  { K_ELeft                , 0, XK_Left },
  { K_ERight               , 0, XK_Right },
  { K_EEnd                 , 0, XK_End },
  { K_EDown                , 0, XK_Down },
  { K_EPageDown            , 0, XK_Next },
  { K_EInsert              , 0, XK_Insert },
  { K_EDelete              , 0, XK_Delete },
  { K_Control_ELeft        , ControlMask, XK_Left },
  { K_Control_ERight       , ControlMask, XK_Right },
  { K_Control_EEnd         , ControlMask, XK_End },
  { K_Control_EPageDown    , ControlMask, XK_Next },
  { K_Control_EHome        , ControlMask, XK_Home },
  { K_Control_EPageUp      , ControlMask, XK_Prior },
  { K_Control_EUp          , ControlMask, XK_Up },
  { K_Control_EDown        , ControlMask, XK_Down },
  { K_Control_EInsert      , ControlMask, XK_Insert },
  { K_Control_EDelete      , ControlMask, XK_Delete },
};

static int _XKeyEventToGrKey (XKeyEvent *xkey)
{
  KeyEntry *kp;
  unsigned int state;
  char buffer[20];
  KeySym keysym;
  int count;

  state = xkey->state & (ShiftMask | ControlMask | Mod1Mask);
  count = XLookupString (xkey,
                         buffer,
                         sizeof(buffer),
                         &keysym, (XComposeStatus *) NULL);

  if ((count == 1) && ((state & Mod1Mask) == 0))
    return (unsigned char) buffer[0];

  for (kp = _KeyTable;
       kp < &_KeyTable[sizeof(_KeyTable)/sizeof(_KeyTable[0])];
       kp = kp + 1)
    {
      if (keysym == kp->keysym && state == kp->state)
        return kp->key;
    }
  return EOF;
}

static inline
unsigned int _XButtonEventToGrButton (XButtonEvent *event)
{
  unsigned int state = event->state;
  unsigned int mask = 0;

  switch (event->button) {
  case Button1: mask = Button1Mask; break;
  case Button2: mask = Button2Mask; break;
  case Button3: mask = Button3Mask; break;
  }
  switch (event->type) {
  case ButtonPress:     state |= mask; break;
  case ButtonRelease:   state &= ~mask; break;
  }
  return (  ((state & Button1Mask) ? GR_M_LEFT : 0)
          | ((state & Button2Mask) ? GR_M_MIDDLE : 0)
          | ((state & Button3Mask) ? GR_M_RIGHT : 0));
}

static inline
unsigned int _XGrModifierKey (KeySym keysym, int type)
{
  if (type == KeyPress) {
    switch (keysym) {
    case XK_Shift_L:    kbd_lastmod |= GR_KB_LEFTSHIFT; break;
    case XK_Shift_R:    kbd_lastmod |= GR_KB_RIGHTSHIFT; break;
    case XK_Control_L:
    case XK_Control_R:  kbd_lastmod |= GR_KB_CTRL; break;
    case XK_Alt_L:
    case XK_Alt_R:
    case XK_Meta_L:
    case XK_Meta_R:     kbd_lastmod |= GR_KB_ALT; break;
    case XK_Num_Lock:   kbd_lastmod |= GR_KB_NUMLOCK; break;
    case XK_Caps_Lock:  kbd_lastmod |= GR_KB_CAPSLOCK; break;
    case XK_Insert:     kbd_lastmod |= GR_KB_INSERT; break;
    }
  }
  if (type == KeyRelease) {
    switch (keysym) {
    case XK_Shift_L:    kbd_lastmod &= ~GR_KB_LEFTSHIFT; break;
    case XK_Shift_R:    kbd_lastmod &= ~GR_KB_RIGHTSHIFT; break;
    case XK_Control_L:
    case XK_Control_R:  kbd_lastmod &= ~GR_KB_CTRL; break;
    case XK_Alt_L:
    case XK_Alt_R:
    case XK_Meta_L:
    case XK_Meta_R:     kbd_lastmod &= ~GR_KB_ALT; break;
    case XK_Num_Lock:   kbd_lastmod &= ~GR_KB_NUMLOCK; break;
    case XK_Caps_Lock:  kbd_lastmod &= ~GR_KB_CAPSLOCK; break;
    case XK_Insert:     kbd_lastmod &= ~GR_KB_INSERT; break;
    }
  }
  return kbd_lastmod;
}

void _GrUpdateInputs(void)
{
  int count;

#if 0
  if (_XGrTickValue == -1) {
    struct itimerval it;

    _XGrTickHandler (SIGALRM);
    it.it_interval.tv_sec = 0;
    it.it_interval.tv_usec = MS_PER_TICK * 1000L;
    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = MS_PER_TICK * 1000L;
    setitimer (ITIMER_REAL, &it, NULL);
  }
#endif

  count = XEventsQueued (_XGrDisplay, QueuedAfterReading);
  if (count <= 0) {
    XFlush (_XGrDisplay);
    return;
  }
  while (--count >= 0) {
    GrMouseEvent ev;
    XEvent xev;
    KeySym keysym;
    int btn;

    XNextEvent (_XGrDisplay, &xev);
    switch (xev.type) {
    case MotionNotify:
      if (mou_enabled && (MOUINFO->msstatus == 2)) {
        MOUINFO->xpos   = xev.xmotion.x;
        MOUINFO->ypos   = xev.xmotion.y;
        MOUINFO->moved  = TRUE;
      }
      break;

    case ButtonPress:
    case ButtonRelease:
      if (mou_enabled && (MOUINFO->msstatus == 2)) {
        btn = _XButtonEventToGrButton (&xev.xbutton);
        if(btn != mou_buttons) {
          fill_mouse_ev(
                        ev,
                        mou_buttons,btn,
                        GR_M_LEFT,
                        GR_M_MIDDLE,
                        GR_M_RIGHT,
                        kbd_lastmod
                        );
          get_dtime(ev.dtime, evt_lasttime);
          enqueue_event(ev);
          MOUINFO->moved = FALSE;
          mou_buttons = btn;
        }
      }
      break;

    case KeyPress:
      keysym = XKeycodeToKeysym (_XGrDisplay, xev.xkey.keycode, 0);
      if (IsModifierKey (keysym)) {
        _XGrModifierKey (keysym, xev.type);
      }
      else if (kbd_enabled) {
        fill_keybd_ev(
                      ev,
                      _XKeyEventToGrKey (&xev.xkey),
                      kbd_lastmod
                      );
        get_dtime(ev.dtime,evt_lasttime);
        enqueue_event(ev);
        MOUINFO->moved = FALSE;
      }
      break;

    case KeyRelease:
      keysym = XKeycodeToKeysym (_XGrDisplay, xev.xkey.keycode, 0);
      if (IsModifierKey (keysym)) {
        _XGrModifierKey (keysym, xev.type);
      }
      break;
    }
  }
}

void GrMouseGetEventT(int flags,GrMouseEvent *ev,long tout)
{
  int  msdraw;

  if (MOUINFO->msstatus == 0) GrMouseInit();
  msdraw = !MOUINFO->displayed && !(flags & GR_M_NOPAINT);
  if (msdraw) GrMouseDisplayCursor();
  /* Note: avoid zero timeout for select */
  if (tout <= 0L) tout = 1L;
  for( ; ; ) {
    struct timeval tval;
    fd_set readfds;
    int fd;
    /*
     * Note: The select call with nonzero timeout avoids CPU usage
     *       of nearly 100%
     */
    fd = ConnectionNumber(_XGrDisplay);
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    tval.tv_sec = tout / 1000L;
    tval.tv_usec = (tout % 1000) * 1000L;
    select (fd + 1,
            &readfds,
            (fd_set *) 0,
            (fd_set *) 0,
            &tval);
    tout = tval.tv_sec * 1000L + tval.tv_usec / 1000L;

    _GrUpdateInputs();
    GrMouseUpdateCursor();
    while (MOUINFO->qlength > 0) {
      dequeue_event((*ev));
      if (ev->flags & flags) {
        if (msdraw) GrMouseEraseCursor();
        return;
      }
    }
    if ((flags & GR_M_POLL) ||
        (tout == 0L) ||
        (MOUINFO->moved && (flags & GR_M_MOTION))) {
      fill_mouse_ev(
                    (*ev),
                    mou_buttons,mou_buttons,
                    GR_M_LEFT,
                    GR_M_MIDDLE,
                    GR_M_RIGHT,
                    kbd_lastmod
                    );
      get_dtime(ev->dtime,evt_lasttime);
      MOUINFO->moved = FALSE;
      if (msdraw) GrMouseEraseCursor();
      return;
    }
  }
}


int _XGrKeyboardHit (void)
{
  XEvent xev;
  KeySym keysym;

  if (_XGrDisplay) {
    if (XEventsQueued (_XGrDisplay, QueuedAfterReading) <= 0)   {
      XFlush (_XGrDisplay);
      return FALSE;
    }
    while (XCheckMaskEvent (_XGrDisplay, KeyPressMask|KeyReleaseMask, &xev)) {
      keysym = XKeycodeToKeysym (_XGrDisplay, xev.xkey.keycode, 0);
      if (IsModifierKey (keysym)) {
        _XGrModifierKey (keysym, xev.type);
        continue;
      }
      if (xev.type == KeyPress) {
        XPutBackEvent (_XGrDisplay, &xev);
        return TRUE;
      }
    }
  }
  return FALSE;
}

int _XGrKeyboardGetKey (void)
{
  XEvent xev;
  KeySym keysym;

  if (_XGrDisplay) {
    while (XMaskEvent (_XGrDisplay, KeyPressMask|KeyReleaseMask, &xev)) {
      keysym = XKeycodeToKeysym (_XGrDisplay, xev.xkey.keycode, 0);
      if (IsModifierKey (keysym)) {
        _XGrModifierKey (keysym, xev.type);
        continue;
      }
      if (xev.type == KeyPress) {
        return _XKeyEventToGrKey (&xev.xkey);
      }
    }
  }
  return 0;
}

int _XGrKeyboardGetState (void)
{
  return kbd_lastmod;
}

