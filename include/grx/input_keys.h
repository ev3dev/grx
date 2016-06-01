/*
 * input_keys.h
 *
 * Copyright (c) 2016 David Lechner <david@lechnology.com>
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

#ifndef __GRX_INPUT_KEYS_H__
#define __GRX_INPUT_KEYS_H__

/**
 * SECTION:input_keys
 * @short_description: TODO
 * @title: Keyboard Keys
 * @section_id: input_keys
 * @include: grx-3.0.h
 *
 * TODO
 */

/**
 * GrxKey:
 * @GRX_KEY_NONE: no key
 * @GRX_KEY_A: a and A
 * @GRX_KEY_B: b and B
 * @GRX_KEY_C: c and C
 * @GRX_KEY_D: d and D
 * @GRX_KEY_E: e and E
 * @GRX_KEY_F: f and F
 * @GRX_KEY_G: g and G
 * @GRX_KEY_H: h and H
 * @GRX_KEY_I: i and I
 * @GRX_KEY_J: j and J
 * @GRX_KEY_K: k and K
 * @GRX_KEY_L: l and L
 * @GRX_KEY_M: m and M
 * @GRX_KEY_N: n and N
 * @GRX_KEY_O: o and O
 * @GRX_KEY_P: p and P
 * @GRX_KEY_Q: q and Q
 * @GRX_KEY_R: r and R
 * @GRX_KEY_S: s and S
 * @GRX_KEY_T: t and T
 * @GRX_KEY_U: u and U
 * @GRX_KEY_V: v and V
 * @GRX_KEY_W: w and W
 * @GRX_KEY_X: x and X
 * @GRX_KEY_Y: y and Y
 * @GRX_KEY_Z: z and Z
 * @GRX_KEY_1: 1 and !
 * @GRX_KEY_2: 2 and @
 * @GRX_KEY_3: 3 and #
 * @GRX_KEY_4: 4 and $
 * @GRX_KEY_5: 5 and %
 * @GRX_KEY_6: 6 and ^
 * @GRX_KEY_7: 7 and &
 * @GRX_KEY_8: 8 and *
 * @GRX_KEY_9: 9 and (
 * @GRX_KEY_0: 0 and )
 * @GRX_KEY_KB_1: bindings-friendly alias of @GRX_KEY_1
 * @GRX_KEY_KB_2: bindings-friendly alias of @GRX_KEY_2
 * @GRX_KEY_KB_3: bindings-friendly alias of @GRX_KEY_3
 * @GRX_KEY_KB_4: bindings-friendly alias of @GRX_KEY_4
 * @GRX_KEY_KB_5: bindings-friendly alias of @GRX_KEY_5
 * @GRX_KEY_KB_6: bindings-friendly alias of @GRX_KEY_6
 * @GRX_KEY_KB_7: bindings-friendly alias of @GRX_KEY_7
 * @GRX_KEY_KB_8: bindings-friendly alias of @GRX_KEY_8
 * @GRX_KEY_KB_9: bindings-friendly alias of @GRX_KEY_9
 * @GRX_KEY_KB_0: bindings-friendly alias of @GRX_KEY_0
 * @GRX_KEY_ENTER: enter/return
 * @GRX_KEY_ESCAPE: escape
 * @GRX_KEY_BACKSPACE: backspace
 * @GRX_KEY_TAB: tab
 * @GRX_KEY_SPACE: spacebar
 * @GRX_KEY_MINUS: \- and _
 * @GRX_KEY_EQUAL: = and +
 * @GRX_KEY_LEFT_BRACKET: [ and {
 * @GRX_KEY_RIGHT_BRACKET: ] and }
 * @GRX_KEY_BACKSLASH: \ and |
 * @GRX_KEY_SEMICOLON: ; and :
 * @GRX_KEY_APOSTROPHE: ' and "
 * @GRX_KEY_GRAVE_ACCENT: ` and ~
 * @GRX_KEY_COMMA: , and <
 * @GRX_KEY_PERIOD: . and >
 * @GRX_KEY_SLASH: / and ?
 * @GRX_KEY_CAPS_LOCK: caps lock
 * @GRX_KEY_F1: function F1
 * @GRX_KEY_F2: function F2
 * @GRX_KEY_F3: function F3
 * @GRX_KEY_F4: function F4
 * @GRX_KEY_F5: function F5
 * @GRX_KEY_F6: function F6
 * @GRX_KEY_F7: function F7
 * @GRX_KEY_F8: function F8
 * @GRX_KEY_F9: function F9
 * @GRX_KEY_F10: function F10
 * @GRX_KEY_F11: function F11
 * @GRX_KEY_F12: function F12
 * @GRX_KEY_PRINT_SCREEN: print screen
 * @GRX_KEY_SCROLL_LOCK: scroll lock
 * @GRX_KEY_PAUSE: pause
 * @GRX_KEY_INSERT: insert
 * @GRX_KEY_HOME: home
 * @GRX_KEY_PAGE_UP: page up
 * @GRX_KEY_DELETE: delete
 * @GRX_KEY_END: end
 * @GRX_KEY_PAGE_DOWN: page down
 * @GRX_KEY_RIGHT_ARROW: right arrow
 * @GRX_KEY_LEFT_ARROW: left arrow
 * @GRX_KEY_DOWN_ARROW: down arrow
 * @GRX_KEY_UP_ARROW: up arrow
 * @GRX_KEY_KP_NUM_LOCK: num lock and clear
 * @GRX_KEY_KP_SLASH: keypad /
 * @GRX_KEY_KP_ASTERISK: keypad *
 * @GRX_KEY_KP_MINUS: keypad -
 * @GRX_KEY_KP_PLUS: keypad +
 * @GRX_KEY_KP_ENTER: keypad enter
 * @GRX_KEY_KP_1: keypad 1 and end
 * @GRX_KEY_KP_2: keypad 2 and down arrow
 * @GRX_KEY_KP_3: keypad 3 and page down
 * @GRX_KEY_KP_4: keypad 4 and left arrow
 * @GRX_KEY_KP_5: keypad 5
 * @GRX_KEY_KP_6: keypad 6 and right arrow
 * @GRX_KEY_KP_7: keypad 7 and home
 * @GRX_KEY_KP_8: keypad 8 and up arrow
 * @GRX_KEY_KP_9: keypad 9 and page up
 * @GRX_KEY_KP_0: keypad 0 and insert
 * @GRX_KEY_KP_PERIOD: keypad . and delete
 * @GRX_KEY_LEFT_CTRL: left control modifier
 * @GRX_KEY_LEFT_SHIFT: left shift modifier
 * @GRX_KEY_LEFT_ALT: left alt modifier
 * @GRX_KEY_LEFT_SUPER: left super modifier
 * @GRX_KEY_RIGHT_CTRL: right control modifier
 * @GRX_KEY_RIGHT_SHIFT: right shift modifier
 * @GRX_KEY_RIGHT_ALT: right alt modifier
 * @GRX_KEY_RIGHT_SUPER: right super modifier
 *
 * Keyboard key codes.
 *
 * Based on USB HID specification.
 */
typedef enum {
    GRX_KEY_NONE            = 0,
    GRX_KEY_A               = 4,
    GRX_KEY_B               = 5,
    GRX_KEY_C               = 6,
    GRX_KEY_D               = 7,
    GRX_KEY_E               = 8,
    GRX_KEY_F               = 9,
    GRX_KEY_G               = 10,
    GRX_KEY_H               = 11,
    GRX_KEY_I               = 12,
    GRX_KEY_J               = 13,
    GRX_KEY_K               = 14,
    GRX_KEY_L               = 15,
    GRX_KEY_M               = 16,
    GRX_KEY_N               = 17,
    GRX_KEY_O               = 18,
    GRX_KEY_P               = 19,
    GRX_KEY_Q               = 20,
    GRX_KEY_R               = 21,
    GRX_KEY_S               = 22,
    GRX_KEY_T               = 23,
    GRX_KEY_U               = 24,
    GRX_KEY_V               = 25,
    GRX_KEY_W               = 26,
    GRX_KEY_X               = 27,
    GRX_KEY_Y               = 28,
    GRX_KEY_Z               = 29,
#ifndef __GI_SCANNER__
    GRX_KEY_1               = 30,
    GRX_KEY_2               = 31,
    GRX_KEY_3               = 32,
    GRX_KEY_4               = 33,
    GRX_KEY_5               = 34,
    GRX_KEY_6               = 35,
    GRX_KEY_7               = 36,
    GRX_KEY_8               = 37,
    GRX_KEY_9               = 38,
    GRX_KEY_0               = 39,
#endif /* __GI_SCANNER__ */
    GRX_KEY_KB_1            = GRX_KEY_1,
    GRX_KEY_KB_2            = GRX_KEY_2,
    GRX_KEY_KB_3            = GRX_KEY_3,
    GRX_KEY_KB_4            = GRX_KEY_4,
    GRX_KEY_KB_5            = GRX_KEY_5,
    GRX_KEY_KB_6            = GRX_KEY_6,
    GRX_KEY_KB_7            = GRX_KEY_7,
    GRX_KEY_KB_8            = GRX_KEY_8,
    GRX_KEY_KB_9            = GRX_KEY_9,
    GRX_KEY_KB_0            = GRX_KEY_0,
    GRX_KEY_ENTER           = 40,
    GRX_KEY_ESCAPE          = 41,
    GRX_KEY_BACKSPACE       = 42,
    GRX_KEY_TAB             = 43,
    GRX_KEY_SPACE           = 44,
    GRX_KEY_MINUS           = 45,
    GRX_KEY_EQUAL           = 46,
    GRX_KEY_LEFT_BRACKET    = 47,
    GRX_KEY_RIGHT_BRACKET   = 48,
    GRX_KEY_BACKSLASH       = 49,
    GRX_KEY_SEMICOLON       = 51,
    GRX_KEY_APOSTROPHE      = 52,
    GRX_KEY_GRAVE_ACCENT    = 53,
    GRX_KEY_COMMA           = 54,
    GRX_KEY_PERIOD          = 55,
    GRX_KEY_SLASH           = 56,
    GRX_KEY_CAPS_LOCK       = 57,
    GRX_KEY_F1              = 58,
    GRX_KEY_F2              = 59,
    GRX_KEY_F3              = 60,
    GRX_KEY_F4              = 61,
    GRX_KEY_F5              = 62,
    GRX_KEY_F6              = 63,
    GRX_KEY_F7              = 64,
    GRX_KEY_F8              = 65,
    GRX_KEY_F9              = 66,
    GRX_KEY_F10             = 67,
    GRX_KEY_F11             = 68,
    GRX_KEY_F12             = 69,
    GRX_KEY_PRINT_SCREEN    = 70,
    GRX_KEY_SCROLL_LOCK     = 71,
    GRX_KEY_PAUSE           = 72,
    GRX_KEY_INSERT          = 73,
    GRX_KEY_HOME            = 74,
    GRX_KEY_PAGE_UP         = 75,
    GRX_KEY_DELETE          = 76,
    GRX_KEY_END             = 77,
    GRX_KEY_PAGE_DOWN       = 78,
    GRX_KEY_RIGHT_ARROW     = 79,
    GRX_KEY_LEFT_ARROW      = 80,
    GRX_KEY_DOWN_ARROW      = 81,
    GRX_KEY_UP_ARROW        = 82,
    GRX_KEY_KP_NUM_LOCK     = 83,
    GRX_KEY_KP_SLASH        = 84,
    GRX_KEY_KP_ASTERISK     = 85,
    GRX_KEY_KP_MINUS        = 86,
    GRX_KEY_KP_PLUS         = 87,
    GRX_KEY_KP_ENTER        = 88,
    GRX_KEY_KP_1            = 89,
    GRX_KEY_KP_2            = 90,
    GRX_KEY_KP_3            = 91,
    GRX_KEY_KP_4            = 92,
    GRX_KEY_KP_5            = 93,
    GRX_KEY_KP_6            = 94,
    GRX_KEY_KP_7            = 95,
    GRX_KEY_KP_8            = 96,
    GRX_KEY_KP_9            = 97,
    GRX_KEY_KP_0            = 98,
    GRX_KEY_KP_PERIOD       = 99,
    GRX_KEY_LEFT_CTRL       = 224,
    GRX_KEY_LEFT_SHIFT      = 225,
    GRX_KEY_LEFT_ALT        = 226,
    GRX_KEY_LEFT_SUPER      = 227,
    GRX_KEY_RIGHT_CTRL      = 228,
    GRX_KEY_RIGHT_SHIFT     = 229,
    GRX_KEY_RIGHT_ALT       = 230,
    GRX_KEY_RIGHT_SUPER     = 231,
} GrxKey;

/**
 * GrxKeyModifier:
 * @GRX_KEY_MODIFIER_CTRL: control modifier
 * @GRX_KEY_MODIFIER_SHIFT: shift modifier
 * @GRX_KEY_MODIFIER_ALT: alt (option) modifier
 * @GRX_KEY_MODIFIER_SUPER: super (windows key) modifier
 *
 * Keyboard key modifier flags.
 */
typedef enum /*<flags>*/ {
    GRX_KEY_MODIFIER_CTRL   = 0x01,
    GRX_KEY_MODIFIER_SHIFT  = 0x02,
    GRX_KEY_MODIFIER_ALT    = 0x04,
    GRX_KEY_MODIFIER_SUPER  = 0x08,
} GrxKeyModifier;

#endif /* __GRX_INPUT_KEYS_H__ */
