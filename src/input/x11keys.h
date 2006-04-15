/**
 ** x11keys.h ---- Keyboard Translation Table
 **
 ** Copyright (C) 2006 Mariano Alvarez Fernandez
 ** [e-mail: malfer@telefonica.net]
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

#ifndef _X11KEYS_H_
#define _X11KEYS_H_

typedef struct {
  unsigned short key;
  unsigned short state;
  KeySym keysym;
} KeyEntry;

static KeyEntry _KeyTable[] = {
  { GrKey_Alt_0                , Mod1Mask, XK_0 },
  { GrKey_Alt_1                , Mod1Mask, XK_1 },
  { GrKey_Alt_2                , Mod1Mask, XK_2 },
  { GrKey_Alt_3                , Mod1Mask, XK_3 },
  { GrKey_Alt_4                , Mod1Mask, XK_4 },
  { GrKey_Alt_5                , Mod1Mask, XK_5 },
  { GrKey_Alt_6                , Mod1Mask, XK_6 },
  { GrKey_Alt_7                , Mod1Mask, XK_7 },
  { GrKey_Alt_8                , Mod1Mask, XK_8 },
  { GrKey_Alt_9                , Mod1Mask, XK_9 },
  { GrKey_Alt_A                , Mod1Mask, XK_a },
  { GrKey_Alt_At               , Mod1Mask, XK_at },
  { GrKey_Alt_B                , Mod1Mask, XK_b },
  { GrKey_Alt_Backquote        , Mod1Mask, XK_quoteright },
  { GrKey_Alt_Backslash        , Mod1Mask, XK_backslash },
  { GrKey_Alt_Backspace        , Mod1Mask, XK_BackSpace },
  { GrKey_Alt_C                , Mod1Mask, XK_c },
#ifdef XK_KP_Begin
  { GrKey_Alt_Center           , Mod1Mask, XK_KP_Begin },
#endif
  { GrKey_Alt_Comma            , Mod1Mask, XK_comma },
  { GrKey_Alt_D                , Mod1Mask, XK_d },
  { GrKey_Alt_Dash             , Mod1Mask, XK_minus },
#ifdef XK_KP_Delete
  { GrKey_Alt_Delete           , Mod1Mask, XK_KP_Delete },
#endif
  { GrKey_Alt_Down             , Mod1Mask, XK_Down },
#ifdef XK_KP_Down
  { GrKey_Alt_Down             , Mod1Mask, XK_KP_Down },
#endif
  { GrKey_Alt_E                , Mod1Mask, XK_e },
#ifdef XK_KP_End
  { GrKey_Alt_End              , Mod1Mask, XK_KP_End },
#endif
#ifdef XK_KP_Enter
  { GrKey_Alt_Enter            , Mod1Mask, XK_KP_Enter },
#endif
  { GrKey_Alt_Equals           , Mod1Mask, XK_equal },
  { GrKey_Alt_Escape           , Mod1Mask, XK_Escape },
  { GrKey_Alt_F                , Mod1Mask, XK_f },
  { GrKey_Alt_F1               , Mod1Mask, XK_F1 },
  { GrKey_Alt_F2               , Mod1Mask, XK_F2 },
  { GrKey_Alt_F3               , Mod1Mask, XK_F3 },
  { GrKey_Alt_F4               , Mod1Mask, XK_F4 },
  { GrKey_Alt_F5               , Mod1Mask, XK_F5 },
  { GrKey_Alt_F6               , Mod1Mask, XK_F6 },
  { GrKey_Alt_F7               , Mod1Mask, XK_F7 },
  { GrKey_Alt_F8               , Mod1Mask, XK_F8 },
  { GrKey_Alt_F9               , Mod1Mask, XK_F9 },
  { GrKey_Alt_F10              , Mod1Mask, XK_F10 },
  { GrKey_Alt_F11              , Mod1Mask, XK_F11 },
  { GrKey_Alt_F12              , Mod1Mask, XK_F12 },
  { GrKey_Alt_G                , Mod1Mask, XK_g },
  { GrKey_Alt_H                , Mod1Mask, XK_h },
#ifdef XK_KP_Home
  { GrKey_Alt_Home             , Mod1Mask, XK_KP_Home },
#endif
  { GrKey_Alt_I                , Mod1Mask, XK_i },
#ifdef XK_KP_Insert
  { GrKey_Alt_Insert           , Mod1Mask, XK_KP_Insert },
#endif
  { GrKey_Alt_J                , Mod1Mask, XK_j },
  { GrKey_Alt_K                , Mod1Mask, XK_k },
#ifdef XK_KP_Subtract
  { GrKey_Alt_KPMinus          , Mod1Mask, XK_KP_Subtract },
#endif
#ifdef XK_KP_Add
  { GrKey_Alt_KPPlus           , Mod1Mask, XK_KP_Add },
#endif
#ifdef XK_KP_Divide
  { GrKey_Alt_KPSlash          , Mod1Mask, XK_KP_Divide },
#endif
#ifdef XK_KP_Multiply
  { GrKey_Alt_KPStar           , Mod1Mask, XK_KP_Multiply },
#endif
  { GrKey_Alt_KPStar           , Mod1Mask, XK_multiply },
  { GrKey_Alt_L                , Mod1Mask, XK_l },
  { GrKey_Alt_LAngle           , Mod1Mask, XK_less },
  { GrKey_Alt_LBrace           , Mod1Mask, XK_braceleft },
  { GrKey_Alt_LBracket         , Mod1Mask, XK_bracketleft },
#ifdef XK_KP_Left
  { GrKey_Alt_Left             , Mod1Mask, XK_KP_Left },
#endif
  { GrKey_Alt_Left             , Mod1Mask, XK_Left },
  { GrKey_Alt_M                , Mod1Mask, XK_m },
  { GrKey_Alt_N                , Mod1Mask, XK_n },
  { GrKey_Alt_O                , Mod1Mask, XK_o },
  { GrKey_Alt_P                , Mod1Mask, XK_p },
#ifdef XK_KP_Next
  { GrKey_Alt_PageDown         , Mod1Mask, XK_KP_Next },
#endif
#ifdef XK_KP_Page_Down
  { GrKey_Alt_PageDown         , Mod1Mask, XK_KP_Page_Down },
#endif
#ifdef XK_KP_Page_Up
  { GrKey_Alt_PageUp           , Mod1Mask, XK_KP_Page_Up },
#endif
#ifdef XK_KP_Prior
  { GrKey_Alt_PageUp           , Mod1Mask, XK_KP_Prior },
#endif
  { GrKey_Alt_Period           , Mod1Mask, XK_period },
  { GrKey_Alt_Pipe             , Mod1Mask, XK_bar },
  { GrKey_Alt_Q                , Mod1Mask, XK_q },
  { GrKey_Alt_Quote            , Mod1Mask, XK_quoteleft },
  { GrKey_Alt_R                , Mod1Mask, XK_r },
  { GrKey_Alt_RAngle           , Mod1Mask, XK_greater },
  { GrKey_Alt_RBrace           , Mod1Mask, XK_braceright },
  { GrKey_Alt_RBracket         , Mod1Mask, XK_bracketright },
  { GrKey_Alt_Return           , Mod1Mask, XK_Return },
#ifdef XK_KP_Right
  { GrKey_Alt_Right            , Mod1Mask, XK_KP_Right },
#endif
  { GrKey_Alt_Right            , Mod1Mask, XK_Right },
  { GrKey_Alt_S                , Mod1Mask, XK_s },
  { GrKey_Alt_Semicolon        , Mod1Mask, XK_semicolon },
  { GrKey_Alt_Slash            , Mod1Mask, XK_slash },
  { GrKey_Alt_T                , Mod1Mask, XK_t },
  { GrKey_Alt_Tab              , Mod1Mask, XK_Tab },
  { GrKey_Alt_U                , Mod1Mask, XK_u },
#ifdef XK_KP_Up
  { GrKey_Alt_Up               , Mod1Mask, XK_KP_Up },
#endif
  { GrKey_Alt_Up               , Mod1Mask, XK_Up },
  { GrKey_Alt_V                , Mod1Mask, XK_v },
  { GrKey_Alt_W                , Mod1Mask, XK_w },
  { GrKey_Alt_X                , Mod1Mask, XK_x },
  { GrKey_Alt_Y                , Mod1Mask, XK_y },
  { GrKey_Alt_Z                , Mod1Mask, XK_z },
#ifdef XK_ISO_Left_Tab
  { GrKey_BackTab              , ShiftMask, XK_ISO_Left_Tab },
#endif
  { GrKey_Center               , 0, XK_5 },
#ifdef XK_KP_Begin
  { GrKey_Center               , 0, XK_KP_Begin },
#endif
  { GrKey_Control_At           , ControlMask, XK_at },
  { GrKey_Control_Center       , ControlMask, XK_5 },
#ifdef XK_KP_Begin
  { GrKey_Control_Center       , ControlMask, XK_KP_Begin },
#endif
  { GrKey_Control_Delete       , ControlMask, XK_Delete },
#ifdef XK_KP_Delete
  { GrKey_Control_Delete       , ControlMask, XK_KP_Delete },
#endif
  { GrKey_Control_Down         , ControlMask, XK_Down },
#ifdef XK_KP_Down
  { GrKey_Control_Down         , ControlMask, XK_KP_Down },
#endif
  { GrKey_Control_End          , ControlMask, XK_End },
#ifdef XK_KP_End
  { GrKey_Control_End          , ControlMask, XK_KP_End },
#endif
  { GrKey_Control_F1           , ControlMask, XK_F1 },
  { GrKey_Control_F2           , ControlMask, XK_F2 },
  { GrKey_Control_F3           , ControlMask, XK_F3 },
  { GrKey_Control_F4           , ControlMask, XK_F4 },
  { GrKey_Control_F5           , ControlMask, XK_F5 },
  { GrKey_Control_F6           , ControlMask, XK_F6 },
  { GrKey_Control_F7           , ControlMask, XK_F7 },
  { GrKey_Control_F8           , ControlMask, XK_F8 },
  { GrKey_Control_F9           , ControlMask, XK_F9 },
  { GrKey_Control_F10          , ControlMask, XK_F10 },
  { GrKey_Control_F11          , ControlMask, XK_F11 },
  { GrKey_Control_F12          , ControlMask, XK_F12 },
  { GrKey_Control_Home         , ControlMask, XK_Home },
#ifdef XK_KP_Home
  { GrKey_Control_Home         , ControlMask, XK_KP_Home },
#endif
  { GrKey_Control_Insert       , ControlMask, XK_Insert },
#ifdef XK_KP_Insert
  { GrKey_Control_Insert       , ControlMask, XK_KP_Insert },
#endif
#ifdef XK_KP_Subtract
  { GrKey_Control_KPDash       , ControlMask, XK_KP_Subtract },
#endif
#ifdef XK_KP_Add
  { GrKey_Control_KPPlus       , ControlMask, XK_KP_Add },
#endif
  { GrKey_Control_KPSlash      , ControlMask, XK_slash },
  { GrKey_Control_KPStar       , ControlMask, XK_multiply },
  { GrKey_Control_Left         , ControlMask, XK_Left },
#ifdef XK_KP_Left
  { GrKey_Control_Left         , ControlMask, XK_KP_Left },
#endif
  { GrKey_Control_PageDown     , ControlMask, XK_Next },
#ifdef XK_KP_Next
  { GrKey_Control_PageDown     , ControlMask, XK_KP_Next },
#endif
  { GrKey_Control_PageUp       , ControlMask, XK_Prior },
#ifdef XK_KP_Prior
  { GrKey_Control_PageUp       , ControlMask, XK_KP_Prior },
#endif
  { GrKey_Control_Right        , ControlMask, XK_Right },
#ifdef XK_KP_Right
  { GrKey_Control_Right        , ControlMask, XK_KP_Right },
#endif
  { GrKey_Control_Up           , ControlMask, XK_Up },
#ifdef XK_KP_Up
  { GrKey_Control_Up           , ControlMask, XK_KP_Up },
#endif
#ifdef XK_KP_Subtract
  { GrKey_Dash                 , 0, XK_KP_Subtract },
#endif
  { GrKey_Delete               , 0, XK_Delete },
#ifdef XK_KP_Delete
  { GrKey_Delete               , 0, XK_KP_Delete },
#endif
  { GrKey_Down                 , 0, XK_Down },
#ifdef XK_KP_Down
  { GrKey_Down                 , 0, XK_KP_Down },
#endif
  { GrKey_End                  , 0, XK_End },
#ifdef XK_KP_End
  { GrKey_End                  , 0, XK_KP_End },
#endif
  { GrKey_F1                   , 0, XK_F1 },
  { GrKey_F2                   , 0, XK_F2 },
  { GrKey_F3                   , 0, XK_F3 },
  { GrKey_F4                   , 0, XK_F4 },
  { GrKey_F5                   , 0, XK_F5 },
  { GrKey_F6                   , 0, XK_F6 },
  { GrKey_F7                   , 0, XK_F7 },
  { GrKey_F8                   , 0, XK_F8 },
  { GrKey_F9                   , 0, XK_F9 },
  { GrKey_F10                  , 0, XK_F10 },
  { GrKey_F11                  , 0, XK_F11 },
  { GrKey_F12                  , 0, XK_F12 },
  { GrKey_Home                 , 0, XK_Home },
#ifdef XK_KP_Home
  { GrKey_Home                 , 0, XK_KP_Home },
#endif
  { GrKey_Insert               , 0, XK_Insert },
#ifdef XK_KP_Insert
  { GrKey_Insert               , 0, XK_KP_Insert },
#endif
  { GrKey_Left                 , 0, XK_Left },
#ifdef XK_KP_Left
  { GrKey_Left                 , 0, XK_KP_Left },
#endif
  { GrKey_PageDown             , 0, XK_Next },
#ifdef XK_KP_Next
  { GrKey_PageDown             , 0, XK_KP_Next },
#endif
  { GrKey_PageUp               , 0, XK_Prior },
#ifdef XK_KP_Prior
  { GrKey_PageUp               , 0, XK_KP_Prior },
#endif
#ifdef XK_KP_Add
  { GrKey_Plus                 , 0, XK_KP_Add },
#endif
  { GrKey_Print                , 0, XK_Print },
  { GrKey_Right                , 0, XK_Right },
#ifdef XK_KP_Right
  { GrKey_Right                , 0, XK_KP_Right },
#endif
  { GrKey_Shift_Down           , ShiftMask, XK_Down },
  { GrKey_Shift_End            , ShiftMask, XK_End },
  { GrKey_Shift_F1             , ShiftMask, XK_F1 },
  { GrKey_Shift_F2             , ShiftMask, XK_F2 },
  { GrKey_Shift_F3             , ShiftMask, XK_F3 },
  { GrKey_Shift_F4             , ShiftMask, XK_F4 },
  { GrKey_Shift_F5             , ShiftMask, XK_F5 },
  { GrKey_Shift_F6             , ShiftMask, XK_F6 },
  { GrKey_Shift_F7             , ShiftMask, XK_F7 },
  { GrKey_Shift_F8             , ShiftMask, XK_F8 },
  { GrKey_Shift_F9             , ShiftMask, XK_F9 },
  { GrKey_Shift_F10            , ShiftMask, XK_F10 },
  { GrKey_Shift_F11            , ShiftMask, XK_F11 },
  { GrKey_Shift_F12            , ShiftMask, XK_F12 },
  { GrKey_Shift_Home           , ShiftMask, XK_Home },
  { GrKey_Shift_Insert         , ShiftMask, XK_Insert },
  { GrKey_Shift_Left           , ShiftMask, XK_Left },
  { GrKey_Shift_PageDown       , ShiftMask, XK_Next },
  { GrKey_Shift_PageUp         , ShiftMask, XK_Prior },
  { GrKey_Shift_Right          , ShiftMask, XK_Right },
  { GrKey_Shift_Up             , ShiftMask, XK_Up },
  { GrKey_Up                   , 0, XK_Up },
#ifdef XK_KP_Up
  { GrKey_Up                   , 0, XK_KP_Up },
#endif
};

#endif
