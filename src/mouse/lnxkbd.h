#ifndef _LNXKBD_H
#define _LNXKBD_H

#define KB_NO_KEY         0

#define KB_F1             0x101
#define KB_F2             0x102
#define KB_F3             0x103
#define KB_F4             0x104
#define KB_F5             0x105
#define KB_F6             0x106
#define KB_F7             0x107
#define KB_F8             0x108
#define KB_F9             0x109 
#define KB_F10            0x10A  
#define KB_F11            0x10B  /* NB: same as KB_SHIFT_F1 */
#define KB_F12            0x10C  /* NB: same as KB_SHIFT_F2 */
#define KB_SHIFT_F1       0x10B
#define KB_SHIFT_F2       0x10C
#define KB_SHIFT_F3       0x10D
#define KB_SHIFT_F4       0x10E
#define KB_SHIFT_F5       0x10F
#define KB_SHIFT_F6       0x110
#define KB_SHIFT_F7       0x111
#define KB_SHIFT_F8       0x112
#define KB_SHIFT_F9       0x113
#define KB_SHIFT_F10      0x114

/* NB: under linux shift F11 and F12 are the same as normal F11 and F12, 
 * which in turn are the same as shift F1 and F2.
 */

#define KB_CURLF        0x200
#define KB_CURDN        0x201
#define KB_CURUP        0x202
#define KB_CURRT        0x203
#define KB_HOME                0x204
#define KB_END                0x205
#define KB_PGUP                0x206
#define KB_PGDN                0x207
#define KB_INSERT        0x208
#define KB_DELETE        0x209

#define KB_ENTER        13
#define KB_ESC                27
#define KB_TAB                 9

#endif /* notdef _LNXKBD_H */
